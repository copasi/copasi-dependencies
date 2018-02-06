#include "SBWBrokerRPC.h"
#include "ModuleInstance.h"
#include "StringUtil.h"
#include "Properties.h"
#include "BrokerDefines.h"
#include "BrokerSessionKey.h"
#include "ProxyModuleInstance.h"

#include "SBW/SBWConfig.h"
#include "SBW/SessionKey.h"
#include "SBW/SBWOSSocket.h"
#include "SBW/SBWException.h"
#include "SBW/SBWApplicationException.h"
#include "SBW/SBWCommunicationException.h"
#include "SBW/RuntimeProperties.h"

#include <sstream>

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#ifndef INADDR_NONE
#define INADDR_NONE ((unsigned long) -1)
#endif

using namespace std;
using namespace SystemsBiologyWorkbench;
using namespace SystemsBiologyWorkbench::Broker;

/** Cached reference to the table of running instances. **/
InstanceTable *SBWBrokerRPC::instanceTable = InstanceTable::getTable();

/** Cached reference to the host access table. **/
HostAccessTable *SBWBrokerRPC::hostAccessTable = HostAccessTable::getTable();

/** Line termination character, for easy reference. **/
std::string SBWBrokerRPC::lsep = "\n";//System.getProperty("line.separator");

/** The factory object for creating new BrokerInstance objects. **/
InstanceThread::Factory *SBWBrokerRPC::brokerInstanceFactory;

/** The factory object for creating new ModuleInstance objects. **/
InstanceThread::Factory *SBWBrokerRPC::moduleInstanceFactory;

/** The module connection thread. **/
ConnectionThread *SBWBrokerRPC::moduleConnThread;

/** The broker connection thread. **/
ConnectionThread *SBWBrokerRPC::brokerConnThread;

/** The current host, returned by getHost(). **/
Host *SBWBrokerRPC::myHost;

/**
* Instance hook for modules.  This defines the callbacks that are called
* by InstanceThread at different points in the life of a module
* instance.  See the definition of InstanceThread::Hook for an
* explanation of the different methods.
**/
InstanceThread::Hook *SBWBrokerRPC::moduleHook;
InstanceThread::Hook *SBWBrokerRPC::brokerHook;

SBWBrokerRPC::SBWBrokerRPC() : SystemsBiologyWorkbench::SBWModuleRPC(new SystemsBiologyWorkbench::SBWListenerConsolidator), brokerMutex("brokerMutex")
{
    // If we can't get a host name, use the address.

    char shortname[256];
    int nResult = gethostname(shortname, 255);
    if (  nResult  >= 0)
    {
        std::string thisHost(shortname);
        myHost = new Host(thisHost);
    }
    else
    {
        myHost = new Host(Sys::getHostAddress());
    }

    myHost->setPort(RuntimeProperties::getSBWBrokerPort());

    // Cache the instance factories.
    moduleHook = new ModuleInstance::Hook();
    brokerHook = new BrokerInstance::Hook();
    brokerInstanceFactory = BrokerInstance::getFactory(brokerHook, myHost);
    moduleInstanceFactory = ModuleInstance::getFactory(moduleHook, myHost);

    m_bStopped = false;

}
void SBWBrokerRPC::connect(const char * /*moduleNameIgnored*/, const char * /*hostNameIgnored*/)
{

    //
}

void SBWBrokerRPC::connect(const char* /* moduleNameIgnored*/ )
{
    //
}

void SBWBrokerRPC::startBrokerRPC()
{
    startModuleConnectionThread();
    if (Properties::assertRuntimeProperty("sbw.broker.no-remote","false"))
        startBrokerConnectionThread();
}
void SBWBrokerRPC::startModuleConnectionThread()
{
    // Create a server socket on the local loopback interface.  This adds
    // security by preventing modules on other hosts from connecting here.
    std::vector<int> portRange = StringUtil::parseRange(Properties::getProperty("sbw.module.port.range", "10100-10200"));
    std::string key = SessionKey::getKey();
    ServerSocket *socket = createServerSocket(portRange, Properties::assertRuntimeProperty("sbw.broker.no-remote","false"));

    moduleConnThread = new ConnectionThread(moduleInstanceFactory, socket,
        key, hostAccessTable);
    moduleConnThread->startThread();
}
void SBWBrokerRPC::startBrokerConnectionThread()
{
    std::vector<int> portRange = StringUtil::parseRange(Properties::getProperty("sbw.broker.port.range", "10300-10400"));
    std::string key = SessionKey::getKey();
    ServerSocket *socket = createServerSocket(portRange, true);

    brokerConnThread = new ConnectionThread(brokerInstanceFactory, socket,
        key, hostAccessTable);
    brokerConnThread->startThread();
}
ServerSocket *SBWBrokerRPC::createServerSocket(std::vector<int> portRange, bool remoteAllowed)
{
    SBWOSMutexLock ml(brokerMutex);

    // try connecting on default port ...
    try
    {
        if (remoteAllowed)
        {
            return new ServerSocket(portRange[0]);
        }
        else
        {
            return new ServerSocket(portRange[0], "localhost");
        }
    }
    catch(...)
    {
    }
    // that did not work out ... so let's try to find a free port
    int port = Sys::findFreePort(portRange, remoteAllowed);
    if (port < 0) {
        std::stringstream sError;
        sError	<<  "Unable to find usable module port in range "
            <<  portRange[0] << "-" << portRange[1] << "";
        throw new SBWCommunicationException(sError.str().c_str());
    }

    try
    {
        // If remoteAllowed is set, we bind to the local loopback address
        // on this host, thus preventing remote connections.
        if (remoteAllowed)
        {
            if (!Sys::portInUse(port))
                return new ServerSocket(port);
            else
                return new ServerSocket(port++);

        }
        else
        {
            if (!SBWOSSocket::portInUse(port))
                return new ServerSocket(port, "localhost");
            else
                return new ServerSocket(port++, "localhost");
        }
    }
    catch (SBWException *e)
    {
        //TRACE( e->getMessage() );
        throw new SBWCommunicationException( e->getMessage().c_str() );
    }
    catch (...)
    {
        //TRACE("Unable to open server socket");
        throw new SBWCommunicationException( "Creating server socket failed.");
    }
}
void SBWBrokerRPC::exchangeModuleLists(BrokerInstance *brokerInst)
{
    char *array = NULL;
    int nLength = 0;
    try
    {
        // First, send.
        SBWOSMutexLock ml(brokerMutex);
        array = createModuleListMessage(&nLength);
        if (array != NULL)
            brokerInst->transmitDirect(array, 0, nLength);
        delete[] array;
        array = NULL;

        // Second, read.
        int remoteIdForLocalBroker = brokerInst->getRemoteIdForLocalBroker();
        DataBlockReader data = brokerInst->readDirect();
        vector < DataBlockReader > ::iterator oIterator;
        vector < DataBlockReader > oModuleList;
        data >> oModuleList;

        for (oIterator = oModuleList.begin(); oIterator != oModuleList.end(); oIterator++)
        {
            int remoteId; string remoteName; int brokerForRemote;
            (*oIterator) >> remoteId >> remoteName >> brokerForRemote;
            if (brokerForRemote != remoteIdForLocalBroker)
            {
                int localId = instanceTable->getNextId();
                Instance *newInst
                    = new ProxyModuleInstance(localId, remoteId,
                    remoteName, brokerInst);
                instanceTable->addInstance(newInst);
                notifyOthersOfStartup(newInst, true);
            }
        }
    }
    catch (...)
    {
        TRACE("an error occured while echangeing module lists");
    }
}
char* SBWBrokerRPC::createModuleListMessage(int *nLength)
{
    vector< DataBlockWriter > records;

    vector < Instance * >  instances = instanceTable->getInstances();
    for (unsigned int i = 0; i < instances.size(); i++)
    {
        // only return local modules ... other modules can be obtained by linking to the other brokers
        if (instances[i]->getId() != BROKER_MODULE  && instances[i]->getBrokerId() == BROKER_MODULE)
        {
            DataBlockWriter thisRecord;
            thisRecord << instances[i]->getId() << instances[i]->getName() << instances[i]->getBrokerId();
            records.push_back(thisRecord);
        }
    }

    try
    {
        DataBlockWriter final; final << records;
        unsigned char *msg = const_cast<unsigned char*>(final.getDataCopy(*nLength));
        return (char*)msg;
    }
    catch (SBWUnsupportedObjectTypeException *e)
    {
        TRACE("Internal SBW error: " << e->getMessage());
        delete e;
    }
    return NULL;
}

void SBWBrokerRPC::addStartupWatcher(std::string sModuleName)
{
    SBWOSMutexLock ml(brokerMutex);
    if (!isWaitingForModule(sModuleName))
    {
        m_oStartupIds.push_back(-1);
        m_oStartupNames.push_back(sModuleName);
    }
}
void SBWBrokerRPC::removeStartupWatcher (std::string sModuleName)
{
    SBWOSMutexLock ml(brokerMutex);
    for (unsigned int i = 0; i < m_oStartupNames.size(); i++)
    {
        if (m_oStartupNames[i] == sModuleName)
        {
            m_oStartupNames.erase(m_oStartupNames.begin() + i);
            m_oStartupIds.erase(m_oStartupIds.begin() + i);
            return;
        }
    }
}
int SBWBrokerRPC::getStartupStat(std::string sModuleName)
{
    SBWOSMutexLock ml(brokerMutex);
    for (unsigned int i = 0; i < m_oStartupNames.size(); i++)
    {
        if (m_oStartupNames[i] == sModuleName)
        {
            return m_oStartupIds[i];
        }
    }
    return -1;
}
void SBWBrokerRPC::setStartupStat (std::string sModuleName, int nModuleId)
{
    SBWOSMutexLock ml(brokerMutex);
    for (unsigned int i = 0; i < m_oStartupNames.size(); i++)
    {
        if (m_oStartupNames[i] == sModuleName)
        {
            m_oStartupIds[i] = nModuleId;
            return;
        }
    }
}
bool SBWBrokerRPC::isWaitingForModule(std::string sModuleName)
{
    SBWOSMutexLock ml(brokerMutex);
    vector< string >::iterator oIterator = m_oStartupNames.begin();
    while (oIterator != m_oStartupNames.end())
    {
        if (sModuleName == (*oIterator))
            return true;
        oIterator++;
    }
    return false;
}
void SBWBrokerRPC::notifyOthersOfStartup(Instance *instance, bool localOnly)
{
    SBWOSMutexLock ml(brokerMutex);
    DataBlockWriter data;
    data << instance->getId() << instance->getName();
    int methodId = ON_OTHER_MODULE_INSTANCE_STARTUP_METHOD;

    TRACE ("Sending notification of startup of module " << instance->getId())
        if (isWaitingForModule(instance->getName()) && instance->getBrokerId() == BROKER_MODULE)
            setStartupStat (instance->getName(), instance->getId());

    notifyLocals(methodId, &data, instance->getId());
    if (! localOnly)
        notifyRemotes(methodId, &data);
}

void SBWBrokerRPC::notifyOthersOfDisconnect(Instance *instance, bool localOnly)
{
    SBWOSMutexLock ml(brokerMutex);
    DataBlockWriter data ;
    int nId = instance->getId();
    data << nId;
    int methodId = ON_OTHER_MODULE_INSTANCE_SHUTDOWN_METHOD;

    TRACE("Sending notification of disconnect of module " << nId);

    notifyLocals(methodId, &data, nId);
    if (!localOnly)
        notifyRemotes(methodId, &data);
}

void SBWBrokerRPC::notifyOthersOfRegistrationChange(int brokerId, bool localOnly)
{
    SBWOSMutexLock ml(brokerMutex);
    DataBlockWriter data;
    int methodId = ON_REGISTRATION_CHANGE_METHOD;
    TRACE("Sending notification of registration change");
    notifyLocals(methodId, &data, brokerId);
    if (localOnly)
        notifyRemotes(methodId, &data);

}

void SBWBrokerRPC::notifyLocals(int methodId, DataBlockWriter *data, int skip)
{
    SBWOSMutexLock ml(brokerMutex);
    std::vector<Instance *> instances= instanceTable->getInstances(BROKER_MODULE);
    for (unsigned int i = 0; i < instances.size(); i++)
    {
        Instance *inst = instances[i];
        int id = inst->getId();

        if (id != skip && id != BROKER_MODULE)
            try
        {
            TRACE("Notifying module " << id);
            doSend(id, SYSTEM_SERVICE, methodId, data);
        }
        catch (...)
        {
            TRACE("Unexpected problem sending notice to " << id);
        }
    }
}

void SBWBrokerRPC::notifyRemotes(int methodId, DataBlockWriter *data)
{
    SBWOSMutexLock ml(brokerMutex);
    std::vector<Instance *> instances = instanceTable->getInstances();
    for (unsigned int i = 0; i < instances.size(); i++)
    {
        Instance *inst = instances[i];
        int id = inst->getId();

        // Only brokers need to be notified -- they'll tell their modules.

        BrokerInstance* oBroker = dynamic_cast<BrokerInstance *>(inst);
        if (oBroker != NULL)
        {
            try
            {
                TRACE("Notifying broker " << inst->getId());
                doSend(id, SYSTEM_SERVICE, methodId, data);
            }
            catch (...)
            {
                TRACE("Unexpected problem sending notice to " <<  id);
            }
        }
    }
}

void SBWBrokerRPC::disconnect()
{
    //SBWOSMutexLock ml(brokerMutex);
    if (moduleConnThread != NULL)
        moduleConnThread->shutdown();

    if (brokerConnThread != NULL)
        brokerConnThread->shutdown();

    stopAllInstanceThreads();
    //cleanupPendingCalls();
}

void SBWBrokerRPC::stopAllInstanceThreads()
{
    try
    {
        TRACE("Shutting down instance threads.");

        std::vector<Instance *> instances = instanceTable->getInstances();
        for (unsigned int i = 0; i < instances.size(); i++ )
            instances[i]->disconnect();
    }
    catch (SBWException *e)
    {
        TRACE("Problem shutting down module instances" << e->getMessage());
        delete e;
    }
    catch (...)
    {
        TRACE("Problem shutting down module instances");
    }
}

void SBWBrokerRPC::pleaseStop()
{
    m_bStopped = true;
    SBWOSMutexLock ml(brokerMutex);
    TRACE("Broker has been asked to stop");
    disconnect();
}

bool SBWBrokerRPC::isConnected()
{
    return moduleConnThread->isOperational();
}

void SBWBrokerRPC::onShutdown(DataBlockReader * /*reader*/, int srcModuleId)
{
    TRACE("Broker received message from " << srcModuleId << " to shut down; ignoring");
}

void SBWBrokerRPC::transmitExternalOnly(int toModuleId, unsigned char *message, int length)
{
    //SBWOSMutexLock ml(brokerMutex);
    Instance *instance = instanceTable->getInstance(toModuleId);

    if (instance == NULL)
    {
        // The instance may be in the process of shutting down when we
        // try to do something like deliver a notification of something.
        // Let's try just flagging this for now.

        TRACE("Unable to transmit to instance " << toModuleId << " because it no longer exits");
    }
    else
    {
        try
        {
            instance->transmit(message, length);
            try	{ delete[] message;	} catch (...) {}
        }
        catch (SBWCommunicationException *e)
        {
            // This is severe and means we've lost the socket to the
            // module.  Remove the instance from the Broker.
            TRACE("Unable to transmit to " << instance->getNameForMessage());
            try	{ delete[] message;	} catch (...) {}
            instance->disconnect();
            delete e;
        }
        catch (...)
        {
            throw new SBWCommunicationException(("Unable to transmit to " + instance->getNameForMessage()).c_str());
        }
    }
}
void SBWBrokerRPC::onOtherModuleInstanceStartup(DataBlockReader *reader, int brokerId)
{
    SBWOSMutexLock ml(brokerMutex);
    if (ShuttingDown()) return;
    try
    {
        int remoteId; string remoteName;
        (*reader) >> remoteId;
        (*reader) >> remoteName;

        TRACE("Notified by " << brokerId << " of startup of module '"
            << remoteName << "'");

        BrokerInstance *brokerInst
            = (BrokerInstance *) instanceTable->getInstance(brokerId);

        int localId = instanceTable->getNextId();
        Instance *newInst = reinterpret_cast<Instance *>(new ProxyModuleInstance(localId, remoteId,
            remoteName, brokerInst));
        instanceTable->addInstance(newInst);

        // Notify any watchers that this module has started up.
        notifyOthersOfStartup(newInst, true);
    }
    catch (SBWTypeMismatchException *e)
    {
        TRACE("Received badly formatted startup notification" << e->getMessage());
        delete e;
    }
}

void SBWBrokerRPC::onOtherModuleInstanceShutdown(DataBlockReader *reader, int brokerId)
{
    if (ShuttingDown()) return;
    try
    {
        SBWOSMutexLock ml(brokerMutex);
        int remoteId = 0;
        (*reader) >> remoteId;

        // The id sent in the message is the *remote* id.  We have to
        // look through the table of instances and match up the id.

        vector<Instance *> instances = instanceTable->getInstances();
        Instance *instance = NULL;
        for (unsigned int i = 0; i < instances.size(); i++)
        {
            BrokerInstance *oBroker = dynamic_cast<BrokerInstance *> (instances[i]);
            ProxyModuleInstance *oModule = dynamic_cast<ProxyModuleInstance *> (instances[i]);

            if (oBroker != NULL && oBroker->getRemoteId() == remoteId)
            {
                instance = oBroker;
                break;
            }
            if (oModule != NULL && oModule->getRemoteId() == remoteId)
            {
                instance = oModule;
                break;
            }
        }

        if (instance == NULL)
        {
            TRACE("Notified by " << brokerId << " of module "
                << remoteId << "'s shutdown, but cannot find"
                << " corresponding local instance");
        }
        else
        {
            TRACE("Notified by " << brokerId << " of shutdown of "
                << instance->getNameForMessage());

            notifyOthersOfDisconnect(instance, true);
            instanceTable->removeInstance(instance);
        }
    }
    catch (SBWTypeMismatchException *e)
    {
        TRACE("Received badly formatted shutdown notification: " << e->getMessage());
        delete e;
    }
}

void SBWBrokerRPC::onRegistrationChange(DataBlockReader * /*reader*/, int brokerId)
{
    if (ShuttingDown()) return;
    TRACE("Notified by " << brokerId << " of registration changes");
    notifyOthersOfRegistrationChange(brokerId, false);
}

void SBWBrokerRPC::doSend(int moduleTo, int service, int method, DataBlockWriter *args)
{
    try
    {
        Integer nLength = 0;
        unsigned char * message = args->createSend(moduleTo, getModuleId(),
            service, method,nLength);
        transmit(moduleTo, message, nLength);
    }
    catch (SBWCommunicationException *e)
    {
        // Something happened to the socket connection during transmit().
        // We need to propagate the error to the caller.

        TRACE("Send to " << moduleTo << " failed");
        throw e;
    }
    catch (...)
    {
        // Something happened to the socket connection during transmit().
        TRACE("Send to module " << moduleTo << " failed; connection lost?");
    }

}

int SBWBrokerRPC::connectBroker(Host *host)
{
    SBWOSMutexLock ml(brokerMutex);
    int port = host->getPort();
    if (host->getPort() == -1)
        port = Properties::getPropertyAsInt("sbw.broker.port");
    SOCKET socket = openSocketToBroker(host, port);
    return connectToBroker(host, socket, BrokerSessionKey::getKey());
}

SOCKET SBWBrokerRPC::openSocketToBroker(Host *host, int port)
{
    SOCKET sock = ::socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address ;

    address.sin_family = AF_INET;// short
    address.sin_port = htons((unsigned short)port) ; // u_short
#if defined(WIN32)
    unsigned long hostAddress ;
#else
    in_addr_t hostAddress ;
#endif
    if (host->getName() == "localhost")
    {
        TRACE("Host address = " << host->getAddress());
        hostAddress = inet_addr(host->getAddress().c_str());
    }
    else
    {
        TRACE("Trying to obtain address for host '" << host->getName() << "'");
        std::string given(host->getName());
        std::string::size_type first = given.find_first_not_of("1234567890.");

        if (first == std::string::npos)
        {
            hostAddress = inet_addr(host->getClearName().c_str()) ;
        }
        else
        {
            struct hostent *hptr = gethostbyname(host->getClearName().c_str());

            if (hptr == NULL)
            {
                TRACE("Failed to get host entry from gethostbyname: " << host->getName());
                throw new SBWSocketException(("Failed to get host entry from gethostbyname:" + host->getName()).c_str());
            }

            memcpy(&hostAddress, hptr->h_addr_list[0], sizeof(hostAddress));
        }
    }
#if defined(WIN32)
    address.sin_addr.S_un.S_addr = hostAddress;
    if (address.sin_addr.S_un.S_addr == INADDR_NONE)
        throw new SBWSocketException("address error");
#else
    address.sin_addr.s_addr = hostAddress;
    if (address.sin_addr.s_addr == INADDR_NONE)
        throw new SBWSocketException("address error");
#endif
    TRACE("Connecting socket");
    if (::connect(sock, (sockaddr *)&address, sizeof(address)) == SOCKET_ERROR)
        throw new SBWSocketException("couldn't connect to remote broker");
    return sock;
}

int SBWBrokerRPC::connectToBroker(Host *host, SOCKET socket, std::string key)
{
    TRACE("Attempting to connect to Broker on '"
        << host->getName() << "' (" << host->getAddress() << "/" << host->getPort() << ")");

    BrokerInstance *inst
        = (BrokerInstance*) brokerInstanceFactory->newInstance(socket, host);

    if (inst->connect(key.c_str()) && inst->start())
    {
        //instanceTable->addInstance(inst);
        return inst->getId();
    }
    else
    {
        SBWException *ex = inst->getException();
        if ( dynamic_cast<SBWCommunicationException *>( ex ) != NULL )
            throw dynamic_cast<SBWCommunicationException *>( ex );
        else if (dynamic_cast<SBWBrokerStartException *>( ex ) != NULL  )
            throw dynamic_cast<SBWBrokerStartException *>( ex );
    }

    throw new SBWCommunicationException(
        "Unable to interact with remote Broker on host '"
        + host->getName() + "'",
        "The SBW Broker on the local host ('" + myHost->getName()
        + "') encountered an error while attempting to communicate"\
        " with the Broker on host '" + host->getName() + "' ("
        + host->getAddress() + ")");
}

int SBWBrokerRPC::getModuleId()
{
    return BROKER_MODULE;
}

int SBWBrokerRPC::getThisModule()
{
    return BROKER_MODULE;
}

Host *SBWBrokerRPC::getHost()
{
    return myHost;
}
