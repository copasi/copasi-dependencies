#ifndef SBW_BROKER_RPC
#define SBW_BROKER_RPC

#include "SBW/SBWOSSocket.h"
#include "SBW/SBW.h"
#include "SBW/SBWModuleRPC.h"

#include "Sys.h"
#include "InstanceTable.h"
#include "HostAccessTable.h"
#include "BrokerInstance.h"
#include "ModuleInstance.h"
#include "ConnectionThread.h"
#include "Host.h"
#include "SBWLowLevelInternal.h"
#include "ServerSocket.h"

#include <iostream>

namespace SystemsBiologyWorkbench
{
    namespace Broker
    {
        /**
         * \ingroup C++Broker
         *
         * lowlevel socket management specific to a broker instance (as opposed to the module instances)
         *
         *
         * \date 10-19-2004
         *
         * \author fbergman
         *
         *
         */
        class SBWBrokerRPC : public SystemsBiologyWorkbench::SBWModuleRPC
        {
        public:
            inline bool ShuttingDown( ) {return m_bStopped;}
            /**
            * Class constructor.  This is used only once, in
            * BrokerApplication#startBroker().
            **/
            SBWBrokerRPC();
            /**
            * The Broker's concrete implementation of the <code>connect()</code>
            * method in <kbd>SBWRPC.java<kbd>.  This is a no-op since the Broker
            * is started using #startBrokerRPC().
            * <p>
            * @param moduleNameIgnored parameter ignored in the case of the Broker
            * @param hostNameIgnored parameter ignored in the case of the Broker
            * @throws SBWCommunicationException if an error occurred while trying
            * to set up the server socket
            **/
            virtual void connect(const char* moduleNameIgnored, const char* hostNameIgnored);
            /**
            * The Broker's concrete implementation of the <code>connect()</code>
            * method in <kbd>SBWRPC.java<kbd>.  This is a no-op since the Broker
            * is started using #startBrokerRPC().
            * <p>
            * @param moduleNameIgnored parameter ignored in the case of the Broker
            * @throws SBWCommunicationException if an error occurred while trying
            * to set up the server socket
            **/
            virtual void connect(const char *moduleNameIgnored);
            /**
            * The Broker thread start-up call.  This is unique to the Broker.
            * Exceptions are percolated to the caller, BrokerApplication.
            * <p>
            * @throws SBWException if any exceptions arise
            **/
            virtual void startBrokerRPC();
            /**
            * Sends the current broker's list of running instances to the
            * broker indicated by the parameter <var>brokerInst</var>.
            * <p>
            * @param brokerInst the instance object for the broker with which
            * to exchange module lists
            **/
            virtual void exchangeModuleLists(BrokerInstance *brokerInst);
            /**
            * Notifies other modules and brokers that a given module has started up.
            * <p>
            * @param instance the instance of the module that has started up
            * @param localOnly if <code>true</code>, only notifies local modules,
            * otherwise it notifies remote brokers as well
            * @see #notifyLocals
            * @see #notifyRemotes
            **/
            virtual void notifyOthersOfStartup(Instance *instance, bool localOnly);
            /**
            * Notifies modules and brokers that a module has been disconnected.
            * <p>
            * @param instance the instance of the module that has been disconnected
            * @param localOnly if <code>true</code>, only notifies local modules,
            * otherwise it notifies remote brokers as well
            * @see #notifyLocals
            * @see #notifyRemotes
            **/
            virtual void notifyOthersOfDisconnect(Instance *instance, bool localOnly);
            /**
            * Notifies other modules and brokers that a change in the registry
            * entries the local broker has occurred.  It does not provide
            * information of the nature of the registry change, only that
            * something has occurred.
            * <p>
            * @param brokerId the broker on which the change occurred.
            * @see #notifyLocals
            * @see #notifyRemotes
            **/
            virtual void notifyOthersOfRegistrationChange(int brokerId, bool localOnly);
            /**
            * Concrete implementation of the <code>disconnect()</code> method
            * for SBWRPC.  This does much more than just disconnect,
            * however, because disconnecting the Broker has many ramifications.
            * <p>
            * {@see SBWRPC#disconnect()}.
            **/
            virtual void disconnect();
            /**
            * A kind way of telling the Broker to stop.  Invoking this method
            * has the effect of stopping the Broker *after* the current message
            * has been handled.  This is what we want for implementing the
            * shutdownBroker() operation, for example.
            */
            virtual void pleaseStop();
            /**
            * Concrete implementation of the <code>isConnected()</code> method in
            * <kbd>SBWRPC.java</kbd>.  Returns true if this module already has a
            * network connection to the Broker.  Since this implementation is on
            * the Broker side, we check whether the broker is operational and
            * return true if it is.
            * <p>
            * @return bool true if the Broker is connected and operational
            **/
            virtual bool isConnected();
            /**
            * This overrides SBWRPC#onShutdown.  We disallow the regular
            * shutdown method call on the broker; instead, we provide a separate
            * low-level call to shut it down.  This is to prevent accidentally
            * shutting down a broker due to a module error.
            * <p>
            * @param reader the DataBlockReader containing the arguments,
            * if any, for this method call
            * @param srcModuleId the identifier of the broker that sent the message
            **/
            virtual void onShutdown(DataBlockReader *reader, int srcModuleId);
            /**
            * Concrete implementation of the <code>transmitExternalOnly()</code>
            * method in <kbd>SBWRPC.java</kbd>.
            **/
            virtual void transmitExternalOnly(int toModuleId, unsigned char *message, int length);
            /**
            * Called by RPCInCall#doReceive to run the SBWListener
            * hooks when another module starts up.  This receives the identifier
            * of the module that started up; it also receives the identifier of the
            * broker that sent the notification.
            * <p>
            * This overrides SBWRPC#onOtherModuleInstanceStartup.
            * <p>
            * @param reader a DataBlockReader containing the arguments to
            * the notification message
            * @param brokerId the identifier of the broker that sent notification
            * @see SBWRPC#onOtherModuleInstanceStartup
            * @see notifyOthersOfStartup
            **/
            virtual void onOtherModuleInstanceStartup(DataBlockReader *reader, int brokerId);
            /**
            * Called by RPCInCall#doReceive to run the SBWListener
            * hooks when another module shuts up.  This receives the identifier
            * of the module shut down; it also receives the identifier of the
            * broker that sent the notification.  The latter argument is not used
            * by the default client code here, but this method is overridden by
            * the Broker and there this argument is used.
            * <p>
            * This overrides SBWRPC#onOtherModuleInstanceShutdown.
            * <p>
            * @param otherModuleId the identifier of the module that has shut down
            * @param brokerId the identifier of the broker that sent notification
            * @see SBWRPC#onOtherModuleInstanceShutdown
            **/
            virtual void onOtherModuleInstanceShutdown(DataBlockReader *reader, int brokerId);
            /**
            * Called by RPCInCall#doReceive to run the SBWListener
            * hooks when a module registration change has been received by the
            * broker.  This receives the identifier of the broker that sent the
            * notification.  This argument is not used by the default client code
            * here, but this method is overridden by the Broker and there this
            * argument is used.
            * <p>
            * This overrides SBWRPC#onRegistrationChange.
            * <p>
            * @param brokerId the identifier of the broker that sent notification
            * @see SBWRPC#onRegistrationChange
            **/
            virtual void onRegistrationChange(DataBlockReader *reader, int brokerId);
            /**
             * This method calls a method on a module.
             * \param moduleTo the module to be called
             * \param service the service of the module to be called
             * \param method the method of the service to be called
             * \param *args the arguments for the method
             */
            virtual void doSend(int moduleTo, int service, int method, DataBlockWriter *args);
            /**
             * connect to the broker on the given host
             * \param *host the host to connect to
             * \return the id for the remote broker
             */
            virtual int connectBroker(Host *host);
            /**
            * part of SBWRPC implementation
            */
            virtual int getModuleId();
            /**
            * part of SBWRPC implementation
            */
            virtual int getThisModule();
            /**
             *
             * \return current host
             */
            static Host *getHost();

            /**
             * adds a module to the startup watchers ...
             * \param sModuleName the module to be added
             */
            void addStartupWatcher(std::string sModuleName);


            /**
             * removes a module name from the list of watchers
             * \param sModuleName the module name to remove
             */
            void removeStartupWatcher (std::string sModuleName);

            /**
             * polling function to test whether or not a module with a certain
             * name has been started up
             * \param sModuleName the module to check for
             * \return the module id if started up
             */
            int getStartupStat(std::string sModuleName);

            /**
             * this function sets the module id to return on a request for a specific module name
             * \param sModuleName the module name
             * \param nModuleId the module identifier
             */
            void setStartupStat (std::string sModuleName, int nModuleId);

            /**
             * this function checks the list of modules that were requested but have not yet
             * been started up for the given module name
             * \param sModuleName the module name
             * \return <b>true</b> if a module has requested the startup, <b>false</b> otherwise
             */
            bool isWaitingForModule(std::string sModuleName);

        private:
            bool m_bStopped;							///< flag indicating that the broker should shut down
            std::vector< std::string > m_oStartupNames; ///< list of module names
            std::vector< int > m_oStartupIds;			///< list of module identifiers
            /** Cached reference to the table of running instances. **/
            static InstanceTable *instanceTable;
            /** Cached reference to the host access table. **/
            static HostAccessTable *hostAccessTable;
            // = HostAccessTable.getTable();
            /** Line termination character, for easy reference. **/
            static std::string lsep;
            /** The factory object for creating new BrokerInstance objects. **/
            static InstanceThread::Factory *brokerInstanceFactory;
            /** The factory object for creating new ModuleInstance objects. **/
            static InstanceThread::Factory *moduleInstanceFactory;
            /** The module connection thread. **/
            static ConnectionThread *moduleConnThread;
            /** The broker connection thread. **/
            static ConnectionThread *brokerConnThread;
            /** The current host, returned by getHost(). **/
            static Host *myHost;
            /**
            * Instance hook for modules.  This defines the callbacks that are called
            * by InstanceThread at different points in the life of a module
            * instance.  See the definition of InstanceThread::Hook for an
            * explanation of the different methods.
            **/
            static InstanceThread::Hook *moduleHook;
            /**
            * Instance hook for brokers.  This defines the callbacks that are called
            * by InstanceThread at different points in the life of a broker
            * instance.  See the definition of InstanceThread::Hook for an
            * explanation of the different methods.
            **/
            static InstanceThread::Hook *brokerHook;
            /**
            * Creates a server socket for modules using a port in the range
            * determined by Config#getModulePortRange, then starts a
            * connection thread for modules.  It hands #moduleInstanceFactory
            * to the connection thread so that it can create new instances of
            * ModuleInstance as modules connect to the broker.
            *
            * @see ConnectionThread
            * @see #moduleInstanceFactory
            **/
            virtual void startModuleConnectionThread();
            /**
            * Creates a server socket for brokers using a port in the range
            * determined by Config#getBrokerPortRange, then starts a
            * connection thread for modules.  It hands #brokerInstanceFactory
            * to the connection thread so that it can create new instances of
            * BrokerInstance as remote brokers connect to this broker.
            *
            * @see ConnectionThread
            * @see #brokerInstanceFactory
            **/
            virtual void startBrokerConnectionThread();
            /**
            * Helper method used by startModuleConnectionThread and
            * startBrokerConnectionThread.  This searches for a free port
            * in the given range of ports and creates a java.net.ServerSocket
            * on the first free port found.  If the flag <var>remoteAllowed</var>
            * is <code>true</code>, the server socket is created such that non-local
            * hosts will be able to connect to it; if the flag is <code>false</code>,
            * the socket is connected on this host's loopback address, which means
            * that only connections from the same (local) computer will be possible.
            * <p>
            * @param portRange an array of two integers indicating the low and high
            * port numbers defining the allowable range of ports for the socket
            * @param remoteAllowed if <code>false</code>, the socket will be opened
            * on the local loopback address and connections from remote hosts will
            * be impossible
            * @return the server socket to use
            * @throws SBWCommunicationException if it is impossible to create the
            * server socket for one reason or another
            **/
            virtual ServerSocket *createServerSocket(std::vector<int> portRange, bool remoteAllowed);
            /**
            * Creates the body of a message containing information about the
            * module instances running on the local broker.  This is used in
            * the initial handshaking with a remote broker.
            * <p>
            * @return the byte array object containing the message contents
            **/
            virtual char* createModuleListMessage(int *nLength);
            /**
            * Utility routine used by notifyOthersOfStartup and others.
            * Sends a notification (corresponding to the given
            * <code>methodId</code> argument) to all other ModuleInstance
            * instances on this broker.  The given <code>data</data> argument is
            * the data passed in the message; typically this is the identifier of
            * the module that has either started up or shutdown.
            * <p>
            * @param methodId the system notification being sent
            * @param data the data to be put into the notification message
            * @param skip an id of a module to skip (i.e., the one that gave rise to
            * the notification in the first place)
            * @see notifyOthersOfDisconnect
            * @see notifyOthersOfRegistrationChange
            * @see notifyOthersOfStartup
            **/
            virtual void notifyLocals(int methodId, DataBlockWriter *data, int skip);
            /**
            * Utility routine used by notifyOthersOfStartup and others.
            * Sends a notification (corresponding to the given
            * <code>methodId</code> argument) to all other BrokerInstance
            * instances.  The given <code>data</data> argument is
            * the data passed in the message; typically this is the identifier of
            * the module that has either started up or shutdown.
            * <p>
            * @param methodId the system notification being sent
            * @param data the data to be put into the notification message
            * @see notifyOthersOfDisconnect
            * @see notifyOthersOfRegistrationChange
            * @see notifyOthersOfStartup
            **/
            virtual void notifyRemotes(int methodId, DataBlockWriter *data);
            /**
            * Runs the Instance#disconnect() method on all instances.
            **/
            virtual void stopAllInstanceThreads();
            /**
             * open a socket to the broker on the given host with the given port
             * \param *host the host of the broker
             * \param port the port of the broker
             * \return the SOCKET
             */
            virtual SOCKET openSocketToBroker(Host *host, int port);
            /**
            * This connects the local broker to a remote broker, for the case where
            * the local broker has been started by the remote one.  This normally
            * means that this local copy has been started with the --remote-from
            * flag (see BrokerApplication).
            **/
            virtual int connectToBroker(Host *host, SOCKET socket, std::string key);

            /// mutex that controls ensures that conncurrent transmits over the socket don't occur.
            SBWOSMutex brokerMutex ;
        };
    }
}

#endif
