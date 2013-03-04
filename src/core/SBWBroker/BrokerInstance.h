#ifndef SBW_BROKER_INSTANCE
#define SBW_BROKER_INSTANCE

#include "InstanceThread.h"
#include "Host.h"
#include "IntegerHashtable.h"
#include "Instance.h"
#include "BrokerInterface.h"
#include "Properties.h"
#include "StringUtil.h"

#include "SBW/DataBlockReader.h"
#include "SBW/SBWLowLevel.h"
#include "SBW/SBWException.h"

#include <string>
#include <iostream>

#include "BrokerDefines.h"

using namespace SystemsBiologyWorkbench::Broker;

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{

		/**
		 * \ingroup C++Broker
		 *
		 * A BrokerInstance acts as a proxy for a remote broker connection.
		 *
		 * A BrokerInstance object is a thread that handles communications
		 * with a remote broker that is connected to the local SBW Broker.
		 * BrokerInstance implements the Instance interface and is
		 * a subclass of InstanceThread.
		 *
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class BrokerInstance : public InstanceThread
		{
		private:
			
			int myRemoteId;	///<  The identifier for this instance on the remote broker.			
			int remoteIdForLocalBroker; ///< The id for the local broker as it's known by the remote broker.			
			BrokerInterface *brokerInterface; ///< A proxy object for this broker's service methods.			
			Host *localHost;					///< The local (not remote) host.
			/**
			* A mapping of local instance identifiers for remote instances running
			* on the remote broker.  This mapping is updated by 
			* ProxyModuleInstance.  When a remote module starts up or shuts down,
			* the startup and shutdown notifications sent by the remote broker
			* include the id (from the remote broker's point of view).  This
			* information can be matched up to the id assigned to the 
			* ProxyModuleInstance by the local broker.  Thus, this mapping is
			* easy to maintain using only the standard SBW notification messages
			* sent by the brokers. 
			*/ 
			IntegerHashtable<int> remoteIdMap;  
			/**
			* Connect I/O streams to the socket.
			* <p>
			* @return <code>true</code> if the streams were successfully connected;
			* <code>false</code> otherwise
			**/
			bool connectStreams();
			/**
			* Exchange broker id's, host names and addresses.  The local broker
			* sends the instance identifier chosen for the remote broker to the
			* remote broker, so that it can record it internally.  It then reads
			* the remote broker's id for itself.  It does this also for the host
			* name and the IP address of itself and the remote broker.
			* <p>
			* The need to exchange host names and IP addresses here arises from
			* the fact that on Windows, a machine will sometimes (especially
			* using the normal Java InetAddress calls) return the *Windows* host
			* name, not the actual name by which it may be known on the IP network
			* to which it may be connected.  This leads to the problem that the
			* remote host thinks it's "HOST1", but when a remote Linux machine
			* tries to get the address for "HOST", it fails to find it.  Of course,
			* if we could simply get the IP address of the other end of the
			* socket, we could avoid this problem, but we can't rely on doing that
			* because the socket connection may be tunneled from another host.
			* So, the approach taken here to work around all these issues is to
			* have the Brokers send each other the names and addresses by which
			* they know themselves.
			* <p>
			* Note that when two brokers connect, <i>both</i> of them use the
			* BrokerInstance class and run the same methods.  Thus, the connection
			* code and the exchange of id's must be symmetric.  This means that
			* the method must first write something on the output stream, then
			* attempt to read from it; otherwise, if you're not careful about when
			* the reads are done, you can end up with a deadlock situation.
			* <p>
			* @return <code>true</code> if the exchange was successful;
			* <code>false</code> otherwise
			**/
			bool exchangeIds();
		public:
			/**
			* Constructor for a broker instance.  It sets up the internal data fields
			* (most of which come from the InstanceThread parent class).
			* <p>
			* Note that this constructor is private and cannot be called from other
			* classes.  Instead of constructing broker instances directly, other
			* classes must use the getFactory method to get an object
			* that implements InstanceThread::Factory.  Using that, they can 
			* call InstanceThread::Factory::newInstance.
			* <p>
			* @param socket the broker connection socket
			* @param host the LOCAL host address (not the remote broker's)
			* @param hook an InstanceThread::Hook object that is called
			* at instance creation, startup, and shutdown
			**/
			BrokerInstance(SOCKET socket, Host *host, InstanceThread::Hook *hook) :
			InstanceThread(socket, "broker", hook), transmitMutex("brokerinstance")
			{
				myRemoteId = BROKER_MODULE;
				InstanceBase::myBrokerId = InstanceBase::myId;
				InstanceBase::myName = "BROKER";
				localHost = host;

				startupWaitTime = DEFAULT_BROKER_STARTUP_WAIT;

				setLocalIdForRemote(myRemoteId, myId);
				brokerInterface = NULL;

				// remoteHost, myFullName & myInternalName are set in exchangeIds().
			}
			/**
			* Returns a factory object for creating new instances.  The object
			* returned implements the {@link InstanceThread.Factory} interface.
			* <p>
			* @param hook the {@link InstanceThread.Hook} object to be handed
			* to each new instance created
			* @param host the local host name
			* @return an object implementing the {@link InstanceThread.Factory}
			* interface for creating new instances
			**/
			static InstanceThread::Factory *getFactory(InstanceThread::Hook *hook,Host *host);
			/**
			* Connects this instance's streams to the socket and performs initial
			* handshaking with the remote broker.  The handshaking consists of
			* verifying the session key for the broker, and exchanging the id's
			* that each broker uses for the other one.
			* <p>
			* By the time this method is called, the
			* InstanceThread::Hook::onCreation hook method will have already
			* been called by the rest of the Broker.  This means that the field
			* InstanceBase::myId will have been assigned.
			* <p>
			* @return <code>true</code> if the streams were successfully connected
			* to the socket and handshaking was successful; <code>false</code>
			* otherwise
			* @see #connectStreams
			* @see #verifySessionKey
			* @see #exchangeIds
			**/
			bool connect(const char* key);
			/**
			* Deliver a message from the remote broker to a (different) recipient.
			* This is called by the message-processing loop in the superclass,
			* InstanceThread.
			* <p>
			* It's important to keep in mind the context of this method.  When
			* this method is called, we know that we are a broker instance and
			* that the message on the input stream was read from our (remote)
			* broker stream.  We are not the recipient; we are to deliver the
			* message to the destination instance.  Therefore, we know that the
			* source id refers to a module on the *remote* broker.  We don't have
			* to modify the destination id, but we do have to rewrite the source id
			* to refer to a module id on the local broker.  Also
			* worth noting is the fact that when this method is called, the length
			* of the message and the destination field have been read off; what is
			* left in the input stream is the rest of the message after those two
			* fields, but the <code>length</code> parameter still refers to the
			* length of the <i>whole</i> intact message.
			* <p>
			* @param destid the integer identifier of the destination module or
			* broker
			* @param message the actual message to deliver
			* @param destInstance the instance object for the destination module
			* @param length the length of the entire message sent by our module
			**/
			void deliverMessage(int destId, Instance *destInstance, unsigned char* message, int length);
			/**
			* Method used by other instances to transmit data to this broker.
			* This is called from the message-processing loop of another
			* instance to deliver a message intended for this broker.  (This
			* method is only called to transmit messages for the broker itself; a
			* different method, transmitDirect, is used when the
			* recipient is a module connected to the remote broker.)
			* <p>
			* This variant of the transmit method handles transmitting data to the
			* module for the case where the data is in the form of a byte array.
			* The contents of <code>data</code> must be a message in the
			* appropriate format. This version
			* assumes that the message in the parameter <code>data</code> is the
			* entire message to be sent to our module, i.e., including the length
			* and destination fields.
			* <p>
			* @param data a byte array containing the data to be written
			* @param length the number of bytes in the message
			*
			* @see transmitDirect
			**/
			virtual void transmit(unsigned char *data, int length);
			/**
			* Transmit a message to the (remote) broker without rewriting the
			* contents.  This method is most often used to send messages to modules
			* connected to the remote broker, rather than to the broker itself.
			* (However, SBWBrokerRPC also uses it to communicate directly
			* with a remote broker when first establishing a remote broker
			* connection.  See SBWBrokerRPC::exchangeModuleLists.)
			* <p>
			* The message handed to this method in the <code>raw</code> parameter
			* must be a complete message, including the length and destination id's.
			* <p>
			* @param raw a byte array containing the full message
			* @param offset an integer offset indicating the position in
			* <code>raw</code> at which to start reading off the message; useful when
			* initial bytes need to be skipped
			* @param length the length of the message
			* @see transmit(byte[] data, int length)
			**/
			void transmitDirect(char *raw, int offset, int length, bool addLength = true);
			/**
			* Read a message from the (remote) broker and return everything after
			* the length field in a
			* DataBlockReader.  This method is used
			* only in very specialized situations, notably in SBWBrokerRPC
			* during initial handshaking with remote brokers.
			* <p>
			* @returns a DataBlockReader containing the entirety of the
			* message minus the initial length field (the length of the rest of
			* the message can be determined by querying the DataBlockReader)
			* @see transmit(byte[] data, int length)
			**/
			DataBlockReader readDirect();
			/**
			* This method is used to report to our broker the failure to convey a
			* message to a destination.  It is invoked from deliverMessage.
			* It assumes that the length and destination id fields have been
			* consumed from the input stream , and that the
			* given <code>length</code> argument is the length of the original whole
			* message (i.e., <i>including</i> the length and dest id portions, even
			* though they have been read off from the input stream).
			* <p>
			* This method is specifically designed for the case of a broker
			* instance, representing a remote broker connected to this one.  For a
			* version of this method designed for local modules, see 
			* ModuleInstance#reportTransmitFailed.
			* <p>
			* @param length the length of the rest of the message still left
			* in the input stream
			* @param destModuleId the id of the intended destination module
			* @param message the actual message
			* @param ex an exception to transmit back to the originating module
			**/
			void reportTransmitFailed(int length, int destModuleId, unsigned char* message, SBWException *ex);
			/**
			* Returns a BrokerInterface object providing direct access
			* to the methods on the (remote) broker.
			* <p>
			* @returns a BrokerInterface object for this broker
			**/
			BrokerInterface *getBrokerInterface();
			/**
			* Concrete implementation of the RemoteInstance::getRemoteId()
			* method.  This returns the identifier of this instance on the remote
			* broker (or in other words, BROKER_MODULE, since
			* that's the identifier of the remote broker on the remote broker
			* itself.
			**/
			int getRemoteId();

			/**
			* Return the remote broker's identifier for the local broker.  The
			* local broker is known locally as BROKER_MODULE,
			* but on the remote broker represented by this instance object, the
			* local broker will have some other identifier.  This returns that
			* identifier.
			* <p>
			* @returns an integer identifier for the local broker as it's
			* represented by the broker at the other end of this instance
			**/
			int getRemoteIdForLocalBroker();
			/**
			* Map a given remote module identifier to the identifier assigned to it
			* by the <i>local</i> broker.  In other words, find out the local
			* identifier for an instance X on the remote broker.
			* <p>
			* This mapping is updated by ProxyModuleInstance itself.  When
			* a remote module starts up or shuts down, the startup and shutdown
			* notifications sent by the remote broker include the id (from the
			* remote broker's point of view).  This information can be matched up
			* to the id assigned to the ProxyModuleInstance by the local
			* broker.  Thus, this mapping is easy to maintain using only the
			* standard SBW notification messages sent by the brokers.
			* <p>
			* @param remoteModuleId the remote module identifier for which we
			* want to find out the local id
			* @returns an integer identifier for the module as it's known on
			* the local broker
			*
			* @see setLocalIdForRemote
			* @see removeLocalIdForRemote
			**/
			int getLocalIdForRemote(int remoteModuleId);

			/**
			* Add an entry to the mapping of remote module identifiers to local
			* instance identifiers.
			* <p>
			* This mapping is updated by ProxyModuleInstance itself.  When
			* a remote module starts up or shuts down, the startup and shutdown
			* notifications sent by the remote broker include the id (from the
			* remote broker's point of view).  This information can be matched up
			* to the id assigned to the ProxyModuleInstance by the local
			* broker.  Thus, this mapping is easy to maintain using only the
			* standard SBW notification messages sent by the brokers.
			* <p>
			* @param remoteModuleId the remote module identifier
			* @param localId the local instance id corresponding to
			* <code>remoteModuleId</code> on the remote broker
			*
			* @see getLocalIdForRemote
			* @see removeLocalIdForRemote
			**/
			void setLocalIdForRemote(int remoteModuleId, int localId);
			/**
			* Remove an entry from the mapping of remote module identifiers to local
			* instance identifiers.
			* <p>
			* @param remoteModuleId the module id to remove
			*
			* @see setLocalIdForRemote
			* @see getLocalIdForRemote
			**/
			void removeLocalIdForRemote(int remoteModuleId);
			SBWOSMutex transmitMutex ;				///< be sure that nothing goes wrong during transmition

			/**
			 * \ingroup C++Broker
			 *
			 * Factory is used to create new broker InstanceThreads
			 *
			 *
			 * \date 10-19-2004
			 *
			 * \author fbergman
			 *
			 *
			 */
			class Factory : public InstanceThread::Factory 
			{
			public:
				/// constructor
				Factory(Host *host, InstanceThread::Hook *hook) { _host = host; _hook = hook; }
				/// default constructor
				Factory() { _host = NULL; _hook = NULL; }
				virtual ~Factory() {}
				/// creates a new instance
				InstanceThread *newInstance(SOCKET socket, Host *host, InstanceThread::Hook *hook);
				/// creates a new instance
				virtual InstanceThread *newInstance(SOCKET socket, Host *host);
				/// returns the type of InstanceThreads created
				virtual std::string getType() { return "broker"; }
			private:
				Host *_host;						///< the host for which instances are created
				InstanceThread::Hook *_hook;		///< the hook class to be used

			};
			/**
			 * \ingroup C++Broker
			 *
			 * Hook contains some functions that will be called on startup, 
			 * shutdown or registration change
			 *
			 * \date 10-19-2004
			 *
			 * \author fbergman
			 *
			 *
			 */
			class Hook : public InstanceThread::Hook 
			{
			public :
				virtual ~Hook() {}
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
		};
	}
}

#endif
