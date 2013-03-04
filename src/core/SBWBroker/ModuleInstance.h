#ifndef SBW_BROKER_MODULE_INSTANCE
#define SBW_BROKER_MODULE_INSTANCE

#include "SBW/SBWException.h"

#include "Host.h"
#include "Instance.h"
#include "InstanceThread.h"

#include <string>
#include <iostream>

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{

		/**
		* \ingroup C++Broker
		*
		* A ModuleInstance object is a thread that handles communications
		* with a local module that is connected to the local SBW Broker.  (Remote 
		* modules are handled by a different class, ProxyModuleInstance.)  
		* ModuleInstance implements the Instance interface and is
		* a subclass of InstanceThread.
		* <p>
		* The communications link to a module is in the form of a socket connected
		* to the SBW client library in the running module application.  The 
		* ModuleInstance object reads messages coming from the module and dispatches
		* them to whatever recipient instance is indicated in the message.  The
		* message-processing loop is started by the InstanceThread::run()
		* method, which invokes the InstanceThread::runMessageLoop().  The
		* latter continues looping until either the module disconnects or some
		* other event forces termination of the loop.
		* <p>
		* When a new message arrives for this instance, 
		* InstanceThread::runMessageLoop() invokes the #deliverMessage
		* method.  This takes care of delivering the message to the recipient
		* instance.  In the case of ModuleInstance, the delivery is
		* straightforward.  Some other kinds of instances such as remote instances
		* involved more elaborate massaging of the messages from a module before
		* the message can be delivered to its recipient.  (This is the reason for
		* the abstraction of #deliverMessage out of InstanceThread
		* and into the different kinds of instance objects.
		* <p>
		* @see Instance
		* @see RemoteInstance
		* @see InstanceBase
		* @see InstanceThread
		* @see ModuleInstance
		* @see BrokerInstance
		* @see ProxyModuleInstance
		* @see ProxyLocalBrokerInstance 
		*
		* \date 10-19-2004
		*
		* \author fbergman
		*
		*
		*/
		class ModuleInstance : public InstanceThread
		{
		public:
			/**
			* Interface used to implement factory methods for generating new
			* instances of subclasses of InstanceThread.  This is used
			* in ModuleInstance and BrokerInstance to implement
			* the <code>newInstance</code> method that each provides.  The factory
			* methods are used by SBWBrokerRPC and ConnectionThread.
			**/
			class Factory : public InstanceThread::Factory 
			{
			public:
				/// constructor
				Factory(Host *host, InstanceThread::Hook *hook) 
				{ 
					_host = host; _hook = hook; 
				}
				/// default constructor
				Factory() 
				{ 
					_host = NULL; _hook = NULL; 
				}
				/// creates a new ModuleInstance
				InstanceThread *newInstance(SOCKET socket, Host *host, InstanceThread::Hook *hook)
				{ 
					return new ModuleInstance(socket, host, hook); 
				}
				/// creates a new ModuleInstance
				virtual InstanceThread *newInstance(SOCKET socket, Host *host) 
				{
					return new ModuleInstance(socket, host, _hook); 
				} ;
				/// \return the type of the Factory
				virtual std::string getType() { 
					return "module"; 
				}
			private:
				Host *_host;						///< the host for which to create the Instances
				InstanceThread::Hook *_hook;		///< reference to the Hook object
			};
			/**
			* Interface for defining callbacks used to perform actions at certain
			* points in the life cycle of the instance.  The callbacks are defined
			* in SBWBrokerRPC.
			**/
			class Hook : public InstanceThread::Hook 
			{
			public :
				/**
				* Callback invoked as the last step by the InstanceThread
				* class constructor.
				* <p>
				* @param instance the instance
				**/
				virtual void onCreation(InstanceThread *instance);
				/**
				* Callback invoked as the first step by the InstanceThread#run
				* method.
				* <p>
				* @param instance the instance
				**/
				virtual void onStartup(InstanceThread *instance);

				/**
				* Callback invoked after the InstanceThread message loop
				* has stopped being operational.
				* <p>
				* @param instance the instance
				**/
				virtual void onShutdown(InstanceThread *instance);
			};

			/**
			* Constructor for creating a new ModuleInstance object.
			* This initializes some fields, but does not initialize certain others.
			* In particular, the name and full name are set at the time of
			* connection (in connect()) rather than at the time of
			* instance object creation.
			* <p>
			* Note that this constructor is private and cannot be called from other
			* classes.  Instead of constructing broker instances directly, other
			* classes must use the #getFactory method to get an object
			* that implements InstanceThread::Factory.  Using that, they can 
			* call InstanceThread::Factory::newInstance.
			* <p>
			* @param socket the socket connected to the module
			* @param host the name of the local host, so that it can be recorded
			* for display in messages and the InstanceBase::getHost() method
			* @param hook an InstanceThread::Hook object used as a callback
			* to execute code at certain times during the lifetime of this instance
			**/
			ModuleInstance(SOCKET socket, Host *host, InstanceThread::Hook *hook);
		private:
			/**
			* This method is used to report to our module the failure to convey a
			* message to a destination.  It is invoked from #deliverMessage.
			* It assumes that the length and destination id fields have been
			* consumed from the input message, and that the
			* given <code>length</code> argument is the length of the original whole
			* message (i.e., <i>including</i> the length and dest id portions, even
			* though they have been read off from the input stream).
			* <p>
			* This method is specifically designed for the case of a module
			* connected to the local broker.  For a version of this method
			* designed for remote brokers, see 
			* BrokerInstance::reportTransmitFailed}.
			* <p>
			* @param length the length of the message that failed to be transmitted
			* @param destModuleId the id of the intended destination module
			* @param message the message
			* @param ex an exception to transmit back to the originating module
			**/
			void reportTransmitFailed(int length, int destModuleId, unsigned char* message, SBWException *ex);
			SBWOSMutex transmitMutex ;				///< transmision mutex to ensure thread safety
		public:
			/**
			* Returns a factory object for creating new instances.  The object
			* returned implements the InstanceThread::Factory interface.
			* <p>
			* @param hook the InstanceThread::Hook object to be handed
			* to each new instance created
			* @param host the local host name
			* @return an object implementing the InstanceThread::Factory
			* interface for creating new instances
			**/
			static InstanceThread::Factory *getFactory(InstanceThread::Hook *hook,
				Host *host);
			/**
			* Connect the streams to the socket and shake hands with the module
			* instance.  The handshaking consists of reading the module's name
			* (which it sends as the first message immediately after the connection
			* is established) and then sending to the module its numeric module
			* identifier.
			* <p>
			* By the time this method is called, the
			* InstanceThread::Hook#onCreation hook method will have already
			* been called by the rest of the Broker.
			* <p>
			* @return <code>true</code> if the streams were successfully connected
			* to the socket and handshaking was successful; <code>false</code>
			* otherwise
			**/
			bool connect(const char* key);
			/**
			* Deliver a message from our module to a (different) recipient.  This
			* is called by the message-processing loop in the superclass,
			* InstanceThread.
			* <p>
			* It's important to keep in mind the context of this method.  When
			* this method is called, it is from a ModuleInstance object
			* that constitutes the local broker's representation of a module
			* connected to the broker.  The portion of the message still left in
			* the input stream  was written by that
			* module.  Unlike what happens in BrokerInstance, we don't
			* munge the message because we know, for example, that the source id
			* in the message refers to this local module instance.  (Compare this
			* to what is done in BrokerInstance::deliverMessage.)  Also
			* worth noting is the fact that when this method is called, the length
			* of the message and the destination field have been read off; what is
			* left in the input stream is the rest of the message after those two
			* fields, but the <code>length</code> parameter still refers to the
			* length of the <i>whole</i> intact message.
			* <p>
			* @param destid the integer identifier of the destination module or
			* broker
			* @param destInstance the instance object for the destination module
			* @param length the length of the entire message sent by our module
			**/
			void deliverMessage(int destId, Instance *destInstance, unsigned char* message, int length);
			/**
			* Method used by other instances to transmit data to this instance.
			* This is called from the message-processing loop of another instance
			* to deliver a message intended for this module.
			* <p>
			* This variant of the transmit method handles transmitting data to the
			* module for the case where the data is in the form of a byte array.
			* The contents of <code>data</code> must be a message in the
			* appropriate format.  
			* <p>
			* @param data a byte array containing the data to be written
			* @param length the number of bytes in the message
			**/
			virtual void transmit(unsigned char *data, int length);
		};
	}
}

#endif

