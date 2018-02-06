#include <SBW/SBWHighLevel.h>
#include <SBW/SBWLowLevel.h>


void SystemsBiologyWorkbench::SBW::signalDisconnect() { SystemsBiologyWorkbench::SBWLowLevel::signalDisconnect(); }



/**
 * returns an object representing this module instance.
 * @return an object representing this module instance.
 */
SystemsBiologyWorkbench::Module SystemsBiologyWorkbench::SBW::getThisModule()
{
    return SystemsBiologyWorkbench::Module(SystemsBiologyWorkbench::SBWLowLevel::getThisModule());
}

/**
 * returns a module descriptor for a given module.
 * @param moduleName module identification string.
 * @param includeRunning whether a module descriptor for a module which isn't registered but is running will be returned.
 * @return module descriptor for the given module
 */
SystemsBiologyWorkbench::ModuleDescriptor SystemsBiologyWorkbench::SBW::getModuleDescriptor(
    std::string moduleName, bool includeRunning)
{
    return SystemsBiologyWorkbench::SBWLowLevel::getModuleDescriptor(moduleName, includeRunning);
}


/**
 * adds a given listener object to the C++ API to receive notification of SBW events.
 * @param x listener object
 */
void SystemsBiologyWorkbench::SBW::addListener(SBWListener *x)
{
    SystemsBiologyWorkbench::SBWLowLevel::addListener(x);
}

/**
 * removes a listener object from the C++ API.
 * @param x listener object
 */
void SystemsBiologyWorkbench::SBW::removeListener(SBWListener *x)
{
    SystemsBiologyWorkbench::SBWLowLevel::removeListener(x);
}

/**
 * connect this application to the broker as a module instance of a given module
 */
void SystemsBiologyWorkbench::SBW::connect()
{
    SystemsBiologyWorkbench::SBWLowLevel::connect();
}

/**
 * connect this application to a remote broker as a module instance of a given module
 * @see SBW::link
 * @param hostname the hostname of machine on which you wish to connect.
 */
void SystemsBiologyWorkbench::SBW::connect(std::string hostname)
{
    SystemsBiologyWorkbench::SBWLowLevel::connect("", hostname.c_str());
}

/**
 * request that the broker connects to a remote broker.  This ensures
 * that the SBW environment appears to be a combination of the facilities
 * of all the linked brokers.  This is the prefered mechanism for accessing
 * remote resources.
 * @param hostname the hostname of machine on which you wish to connect.
 */
void SystemsBiologyWorkbench::SBW::link(std::string hostname)
{
    SystemsBiologyWorkbench::SBWLowLevel::link(hostname.c_str());
}

/**
 * Convenience function for performing the tasks required for handling
 * the <code>-sbwdebug</code> to an application.  This method must be passed the array of command-line
 * arguments passed to the application.  This method then checks for the
 * presence of <code>-sbwdebug</code> and runs <code>SBWDebug::setTraceMode()</code>.
 * Returns whether the application should either register itself or run as a module based on the command line.
 * @param argc an integer indicating the number of arguments passed to
 * the program on the command line
 * @param argv an array of strings comprising the set of arguments
 * passed to the program on the command line
 * @returns whether the command line contains either <code>-sbwregister</code> or <code>-sbwmodule</code>
 *          ie whether the application should either register itself or run as a module respectively.
 **/
bool SystemsBiologyWorkbench::SBW::processArguments(int argc, char* argv[])
{
    return SystemsBiologyWorkbench::SBWLowLevel::processArguments(argc, argv);
}

/**
 * disconnects this application from the broker.
 * Blocks until the disconnection has occured.
 */
void SystemsBiologyWorkbench::SBW::disconnect()
{
    SystemsBiologyWorkbench::SBWLowLevel::disconnect();
}

/**
 * locates an instance of a given module, creating a new module instance where necessary.
 * Whether an new module instance is created depends on the management type of the module.
 * @param moduleName module identification string
 * @return instance of module with given identification string.
 */
SystemsBiologyWorkbench::Module SystemsBiologyWorkbench::SBW::getModuleInstance(const std::string& moduleName)
{
    return SystemsBiologyWorkbench::Module(SystemsBiologyWorkbench::SBWLowLevel::getModuleInstance(moduleName.c_str()));
}

/**
 * returns module descriptors for all the modules known to the broker.
 * the result should be deallocated using delete.
 * @param includeRunning the result should include running but unregistered modules.
 * @param localOnly the result should include only those modules register with the broker on this host
 * @return module descriptors for all the modules known to the broker.
 */
std::vector<SystemsBiologyWorkbench::ModuleDescriptor> *SystemsBiologyWorkbench::SBW::getModuleDescriptors(
    bool includeRunning, bool localOnly)
{
    return SystemsBiologyWorkbench::SBWLowLevel::getModuleDescriptors(includeRunning, localOnly);
}

/**
 * returns an array of service descriptors that are in a given category.
 * @param categroy a service category
 * @param recursive include service in subcategories of the given categroy (default true)
 * @return an array of service descriptors that are in a given category
 */
std::vector<SystemsBiologyWorkbench::ServiceDescriptor> *SystemsBiologyWorkbench::SBW::findServices(
        std::string serviceCategory, bool recursive /*= true */)
{
    return SystemsBiologyWorkbench::SBWLowLevel::findServices(serviceCategory, recursive);
}

/**
 * returns the top level categories or the sub categrories of a given category.
 * the result should be deallocated using delete.
 * @param category if its empty then this function returns the top level categories
 *                 otherwise returns the sub categrories of a given category.
 */
std::vector<std::string> *SystemsBiologyWorkbench::SBW::getServiceCategories(std::string category)
{
    return SystemsBiologyWorkbench::SBWLowLevel::getServiceCategories(category);
}

/**
 * returns an array of module instance objects representing all the module instances connected to the broker.
 * Deallocate the result using delete.
 * @return an array of module instance objects representing all the module instances connected to the broker.
 */
std::vector<SystemsBiologyWorkbench::Module> *SystemsBiologyWorkbench::SBW::getExistingModuleInstances()
{
    return SystemsBiologyWorkbench::SBWLowLevel::getExistingModuleInstances();
}

/**
 * returns an array of module instance objects representing all the instances of a given module connected to the broker.
 * Deallocate the result using delete.
 * @param moduleName module identification string
 * @return an array of module instance objects representing all the instances of the given connected to the broker.
 */
std::vector<SystemsBiologyWorkbench::Module> *SystemsBiologyWorkbench::SBW::getExistingModuleInstances(
    std::string moduleName)
{
    return SystemsBiologyWorkbench::SBWLowLevel::getExistingModuleInstances(moduleName);
}

/**
 * blocks until this application has disconnected from the broker.
 */
void SystemsBiologyWorkbench::SBW::waitForDisconnect()
{
    SystemsBiologyWorkbench::SBWLowLevel::waitForDisconnect();
}

/**
 * returns the version of the C & C++ library
 * @return the version of the C & C++ library
 */
std::string SystemsBiologyWorkbench::SBW::getVersion()
{
    return SystemsBiologyWorkbench::SBWLowLevel::getVersion();
}

/**
 * returns the version of the Broker
 * @return the version of the Broker
 */
std::string SystemsBiologyWorkbench::SBW::getBrokerVersion()
{
    return SystemsBiologyWorkbench::SBWLowLevel::getBrokerVersion();
}
