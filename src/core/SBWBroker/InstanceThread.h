#ifndef SBW_BROKER_INSTANCE_THREAD
#define SBW_BROKER_INSTANCE_THREAD

#include "SBW/SBWOSSocket.h"
#include "SBW/DataBlockReader.h"
#include "SBW/SBWException.h"

#include "Instance.h"
#include "InstanceBase.h"
#include "InstanceTable.h"

#include <string>
#include <vector>
#include <iostream>

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{

		/**
		 * \ingroup C++Broker
		 *
		 * This class is the cornerstone of the handlers for module and broker
		 * connections.  It extends InstanceBase with functionality that is
		 * common to both ModuleInstance and BrokerInstance, and
		 * also implements the SBWThread so that it can be used as
		 * a thread.  This class is abstract because certain methods (
		 * #connect, #transmit, #deliverMessage) are abstract and
		 * their implementations are left to the subclasses.
		 * <p>
		 * Instance threads are intended to be used in a certain way.  When a
		 * new module or broker is connected, the #connect(string key) method
		 * must be called to execute the socket connection and handshaking code,
		 * then the instance thread must be started by calling #start().
		 * Both methods return booleans to indicate success.  If both return
		 * <code>true</code>, then the instance was successfully connected and
		 * the thread started.  (See ConnectionThread and SBWBrokerRPC
		 * for the specific calls.)
		 * <p>
		 * There are two interfaces defined within this class: 
		 * InstanceThread::Factory and InstanceThread::Hook.  The first is
		 * used by subclasses to implement factory methods for creating new instances.
		 * The second is used in the constructor InstanceThread in order to
		 * allow callbacks at certain points in an instance's life cycle.
		 * <p>
		 * The sequence of events in starting an instance is best explained by looking
		 * at the implementation of #run() and #runMessageLoop().
		 *
		 * @see Instance
		 * @see RemoteInstance
		 * @see InstanceBase
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
		class InstanceThread : public InstanceBase ,  public SBWThread
		{
		public:
			/**
			* Interface for defining callbacks used to perform actions at certain
			* points in the life cycle of the instance.  The callbacks are defined
			* in SBWBrokerRPC.
			**/
			class Hook
			{
			public :
				virtual ~Hook(){}
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
				virtual void onStartup(InstanceThread *instance) ;

				/**
				* Callback invoked after the InstanceThread message loop
				* has stopped being operational.
				* <p>
				* @param instance the instance
				**/
				virtual void onShutdown(InstanceThread *instance) ;
			};
			/**
			* Interface used to implement factory methods for generating new
			* instances of subclasses of InstanceThread.  This is used
			* in ModuleInstance and BrokerInstance to implement
			* the <code>newInstance</code> method that each provides.  The factory
			* methods are used by SBWBrokerRPC and ConnectionThread.
			**/
			class Factory
			{
			public:
				virtual ~Factory() {}
				/**
				* Returns a new instance of this module/broker instance class.
				* @param socket the socket connecting to the application
				**/
				virtual InstanceThread *newInstance(SOCKET socket, Host* host);

				/**
				* Returns a string identifying the type of instance, such as
				* "module" or "broker".
				**/
				virtual std::string getType();
			};
			/**
			* Sets the exception for an exception that just occurred, so that
			* a call to #getException() will retrieve it.  This allows
			* passing exceptions in certain situations that are awkward to do
			* using direct throws.
			**/
			void setException(SBWException *ex)
			{
				exception = ex;
			}
			/**
			* Retrieve the exception being held for this instance.  Invoking
			* this method only makes sense after something has already invoked
			* #setException.
			**/
			SBWException *getException()
			{
				return exception;
			}

			/**
			* Performs the task of connecting the communications streams to the
			* module or remote broker and checking the session authentication key
			* provided by the remote process.  This method must be implemented by
			* the specific module or broker instance subclasses.  It must return
			* <code>true</code> if the connection was made successfully.
			* <p>
			* @param key the session authentication key that the module or
			* remote broker must provide to verify its authenticity
			* @return <code>true</code> if the connection was successfully made
			**/
			virtual bool connect(const char* key) = 0;
			/**
			* Method used by other instances to transmit data to this instance.
			* This is called from the message-processing loop of another instance
			* to deliver a message intended for this module.
			* <p>
			* @param data a byte array containing the data to be written
			* @param length the number of bytes in the message
			**/
			virtual void transmit(unsigned char *data, int length) = 0;
			/**
			* Deliver a message from our module to a (different) recipient.  This
			* is called by the message-processing loop in the superclass,
			* InstanceThread.
			* <p>
			* @param destid the integer identifier of the destination module or
			* broker
			* @param destInstance the instance object for the destination module
			* @param message the message to be delivered
			* @param length the length of the entire message sent by our module
			**/
			virtual void deliverMessage(int destId, Instance *destInstance, unsigned char * message, int length) = 0;
			/**
			* Method to start the underlying thread for this instance.  It is
			* important to wait until everything is ready with this thread and the
			* remote module or Broker before calling code makes use of the
			* instance.  This delays returning until the #operational flag
			* goes to <code>true</code>.
			* <p>
			* @return <code>true</code> if things get started successfully,
			* <code>false</code> otherwise
			**/
			bool start();
			/**
			* Overrides the default <code>run()</code> implementation of
			* SBWThread for a module or broker instance.  This proceeds
			* by running the startup hooks for this instance, then running
			* #runMessageLoop().  Once #runMessageLoop() exits,
			* this disconnects the instance, runs the shutdown hooks, and finally
			* shuts down this thread.
			* <p>
			* This method starts executing after the InstanceThread::start() 
			* method is executed by code in ConnectionThread.  The
			* InstanceThread::start() method runs the underlying thread
			* object's <code>start()</code> method, which in turn causes this
			* #run() method to be, well, run.
			* <p>
			**/
			void run();
			/**
			* The body of the message-handling loop for this module instance.  It
			* loops continually, reading each message sent by the module or broker
			* corresponding to this instance and then transmitting the message to
			* the destination module.  It transmits the message by looking up the
			* destination Instance object in the table of instances in the
			* local broker, and then handing the message to that instance's
			* <code>transmit()</code> method.
			**/
			void runMessageLoop();
			/**
			* This method is called both internally and by external code that
			* wants to tell an instance to disconnect.  This may get called while
			* #runMessageLoop() is running, so what it does is close the
			* streams and set #operational to <code>false</code>, thus
			* allowing #runMessageLoop() to exit and 
			* #disconnectInstance() to be called normally.
			**/
			void disconnect();
			/**
			* This method is called as the normal closing step in #run().
			* This is called after everything is done and we're not operational
			* anymore.
			**/
			void disconnectInstance();
			/**
			* This method sends a shutdown message to the module or remote broker
			* handled by this instance.  As the last step, it sets 
			* #operational to <code>false</code>, causing the message-handling
			* loop and ultimately this whole thread to shut down.
			**/
			void shutdown();
			/**
			* Unlike #shutdown(), this method merely shuts down the thread
			* for this instance.  This method is called as the normal closing step
			* in #run().  This is called after everything is done and
			* we're not operational anymore.
			**/
			void shutdownInstanceThread();
		private:
			/// the exception object used to store exceptions
			SBWException *exception;
			/** Boolean flag indicating whether we believe the client has started.  **/
			bool started;
			/** Boolean flag indicating whether the run loop is operational.  **/
			bool operational;
			/** The list of hooks for this instance.  **/
			std::vector<Hook *> myInstanceHooks;
			/** Cached pointer to the instance table.  **/
			InstanceTable* instanceTable;
			/**
			* Sets the exception for an exception that just occurred, so that
			* a call to #getException() will retrieve it.  This allows
			* passing exceptions in certain situations that are awkward to do
			* using direct throws.
			**/
			void runCreationHooks();
			/**
			* Run the InstanceThread.Hook#onStartup hook functions for
			* this instance.
			**/
			void runStartupHooks();
			/**
			* Run the InstanceThread.Hook#onShutdown hook functions for
			* this instance.
			**/
			void runShutdownHooks();
		protected:
			/** Socket for this instance's connection to its remote module/broker. **/
			SOCKET mySocket;
			/** How long to wait for the instance to start up.  **/
			int startupWaitTime;
			/**
			* Constructor for this class.  This sets up internal variables and
			* sets parameters on the internal thread object, but does not start
			* the thread.  The last step here is calling runCreationHooks.
			* <p>
			* @param socket the socket connected to the actual module or remote broker
			* @param type a string, either "module" or "broker"
			* @param hook an InstanceThread::Hook object providing the
			* callbacks to call for this instance.
			**/
			InstanceThread(SOCKET socket, std::string type, Hook *hook);
		};
	}
}
#endif
