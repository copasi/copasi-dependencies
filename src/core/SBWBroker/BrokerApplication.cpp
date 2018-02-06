#include "Broker.h"
#include "BrokerApplication.h"
#include "BrokerSessionKey.h"
#include "HostAccessTable.h"
#include "ModuleRegistry.h"
#include "ObjectOrientedReceiver.h"
#include "Properties.h"
#include "SBW.h"
#include "SBWBrokerRPC.h"
#include "StringUtil.h"
#include "Sys.h"

#include "SBW/SBW.h"
#include "SBW/SBWC.h"
#include "SBW/SBWConfig.h"
#include "SBW/SBWException.h"
#include "SBW/SBWLowLevel.h"
#include "SBW/SBWRPC.h"
#include "SBW/SBWModuleRPC.h"
#include "SBW/Module.h"
#include "SBW/Service.h"
#include "SBW/ModuleImpl.h"
#include "SBW/RuntimeProperties.h"
#include "SBW/SBWApplicationException.h"
#include "SBW/ImplDataBlockReaderClass.h"

#ifdef WIN32
#include <direct.h>
#include "SBW/portableOS.h"
#endif

#include <sstream>
#include <iostream>
#include <fstream>
#include <exception>

using namespace std;
using namespace SystemsBiologyWorkbench;
using namespace SystemsBiologyWorkbench::Broker;

void BrokerApplication::run(int argc, char* argv[])
{
	parseArgs(argc,argv);

	bool running = brokerIsRunning();
	// brokerIsRunning() checks the port. 
	// the port being in use is not sufficient, since it could be that
	// something aweful happened to the key file (deleted or something) ... 
	// or the running broker belongs to somebody else ... 
	// if thats the case, then the next broker would not start up 
	// but no module could connect ...
	if (running && !StringUtil::empty(BrokerSessionKey::getKey()))
	{				
		if (! flagRemote)
		{
			printError("The SBW Broker appears to be running already.");
			quit(STATUS_BROKER_RUNNING);
		}
		else
			TRACE("Broker already running, but given --remote-from");
	}

	initSessionKey(running);

	if (! running )
		startBroker();

	if (flagRemote )
		echoForRemote(running);
}
void BrokerApplication::quit(int code)
{
	if(code != STATUS_BROKER_RUNNING)
	{
		// clear flags ...
		Properties::changeProperty("sbw.module.port","");
		Properties::changeProperty("sbw.port.port","");
		Properties::save();
	}
	TRACE("Broker exiting with code " << code);
	exit(code);
}

void BrokerApplication::parseArgs(int argc, char* argv[])
{
	// Do not call SBWLog in the while/switch below.  (OK to do it after.)
	// Because of how the Grace log package works, we mustn't call SBWLog
	// before we've had a chance to set the "log" debugging property.

	for (int i = 1; i < argc; i++ )
	{
		string sCurrentArg (argv[i]);
#ifdef WIN32
		if (sCurrentArg == "--register" )
		{
			// register modules specified in the registerModules batch file ...
			ModuleRegistry::registerModules();
			exit(0);
		}
		else 

#endif
			if (sCurrentArg == "--accept-address" || sCurrentArg == "-a") 
			{			
				string addr(argv[i+1]);
				if (addressIsValid(addr))
					HostAccessTable::getTable()->acceptAddress(addr);
				else
				{
					printError("Address is not in a valid format: " + addr);
					printError("Address must be in the form 123.123.123.123");
					printError("with optional '*' wildcard characters");
					printUsageAndExit(STATUS_BAD_ARG);
				}
				i++;

			} 
			else if (sCurrentArg == "--deny-address" || sCurrentArg == "-d")
			{
				string addr(argv[i+1]);
				if (addressIsValid(addr))
				{
					HostAccessTable::getTable()->denyAddress(addr);
				}
				else
				{
					printError("Address is not in a valid format: " + addr);
					printError("Address must be in the form 123.123.123.123");
					printError("with optional '*' wildcard characters");
					printUsageAndExit(STATUS_BAD_ARG);
				}
				i++;
			}
			else if (sCurrentArg == "--broker-ports" || sCurrentArg == "-b") 
			{
				string range(argv[i+1]);

				// Check that we can parse the range.

				vector<int> portRange = StringUtil::parseRange(range);
				if (portRange.size() < 1)
				{
					string msg = "Invalid  broker port range:";
					stringstream sTemp (msg);
					for (unsigned int j = 0; j < portRange.size(); j++) 
						sTemp << " " << portRange[j];
					printError(sTemp.str());
					printError("Port ranges must be two positive integers separated by '-'");
					printUsageAndExit(STATUS_BAD_ARG);
				}

				Properties::setProperty("sbw.broker.port.range",range);
				i++;

			}
			else if (sCurrentArg == "--module-ports" || sCurrentArg == "-m") 
			{
				string range(argv[i+1]);
				// Check that we can parse the range.

				vector<int> portRange = StringUtil::parseRange(range);
				if (portRange.size() < 1)
				{
					string msg = "Invalid module port range: ";
					stringstream sTemp (msg);
					for (unsigned int j = 0; j < portRange.size(); j++) 
					{
						sTemp << " " << portRange[j];
					}
					printError(sTemp.str());
					printError("Port ranges must be two positive integers separated by '-'");
					printUsageAndExit(STATUS_BAD_ARG);
				}

				Properties::setProperty("sbw.module.port.range",range);
				i++;
			}
			else if (sCurrentArg == "--remote-modules" || sCurrentArg == "-e") 
			{
				flagAllowRemoteModules = true;
				Properties::setProperty("sbw.broker.allow-remote-modules","true");
			}
			else if (sCurrentArg == "--registry" || sCurrentArg == "-g") 
			{
				string file = argv[i+1];
				if (checkFileArg(file, "-g/--registry")) 
				{
					Properties::setProperty("sbw.broker.registry",file);
				}
				i++;
			}
			else if (sCurrentArg == "--info" || sCurrentArg == "-i") 
			{
				printInfo();
				quit(STATUS_NORMAL);
			}
			else if (sCurrentArg == "--key-file" || sCurrentArg == "-k") 
			{
				string keyFilePath = argv[i+1];
				if (checkFileArg(keyFilePath, "-k/--key-file"))
				{
					Properties::setProperty("sbw.key.file",keyFilePath);
				}
				i++;
			}
			else if (sCurrentArg == "--reload-key" || sCurrentArg == "-l") 
			{
				flagReloadKey = true;
			}
			else if (sCurrentArg == "--module-options" || sCurrentArg == "-o") 
			{
				Properties::setProperty("sbw.module.options",argv[i+1]);
				i++;
			}
			else if (sCurrentArg == "--no-remote" || sCurrentArg == "-R") 
			{
				flagNoRemoteBrokers = true;
			}
			else if (sCurrentArg == "--remote-from" || sCurrentArg == "-r") 
			{
				string startingHost = argv[i+1];
				if (startingHost.at(0) != '-'
					&& ! StringUtil::empty(startingHost))
				{
					startingHost = Sys::fullyQualifiedHostName(startingHost);
					Properties::setProperty("sbw.broker.remote-from",startingHost);
					flagRemote = true;
				}
				else
				{
					printError("-r/--remote-from argument needs a host name");
					printUsageAndExit(STATUS_BAD_ARG);
				}
				i++;
			}
			else if (sCurrentArg == "--sbw-home" || sCurrentArg == "-s") 
			{
				string sbwHome = argv[i+1];
				if (checkDirArg(sbwHome, "-s/--sbw-home"))
				{
					Properties::setProperty("sbw.home",sbwHome);
				}
				i++;
			}
			else if (sCurrentArg == "--trace" || sCurrentArg == "-t") 
			{
#ifndef _DEBUG
				printError("-t/--trace requires using the debug version of this program");
				printUsageAndExit(STATUS_BAD_ARG);
#else
				flagDebug = true;
				Properties::setProperty("log","true");
#endif				
			}
			else if (sCurrentArg == "--shutdown" || sCurrentArg == "-U") 
			{
				flagShutdown = true;
			}
			else if (sCurrentArg == "--user-dir" || sCurrentArg == "-u") 
			{
				string userDir = argv[i+1];
				if (checkDirArg(userDir, "-u/--user-dir"))
				{
					Properties::setProperty("sbw.user.dir",userDir);
				}
				i++;
			}
			else if (sCurrentArg == "--module-wait" || sCurrentArg == "-w") 
			{
				Properties::setProperty("sbw.module.startup.wait",argv[i+1]);
				i++;
			}
			else if (sCurrentArg == "--help" || sCurrentArg == "-?" || sCurrentArg == "-h") 
			{
				printUsageAndExit(STATUS_NORMAL);
			}
			else 
			{
				printUsageAndExit(STATUS_BAD_ARG);
			} 
	}


	// Set flags as appropriate and log the settings.

	if (flagShutdown)
	{
		if (flagNoRemoteBrokers || flagRemote 
			|| flagReloadKey || flagAllowRemoteModules)
		{
			printError("-U/--shutdown cannot be used with any other flag");
			printUsageAndExit(STATUS_BAD_ARG);
		}
		else
		{
			TRACE("--shutdown given; shutting down running Broker");
			shutDownBroker();
		}
	}

	if (flagNoRemoteBrokers)
	{
		if (flagRemote)
		{
			printError("Cannot specify both --no-remote and --remote");
			printUsageAndExit(STATUS_BAD_ARG);
		}
		else
		{
			TRACE("No remote Broker connections allowed");
			Properties::setProperty("sbw.broker.no-remote","true");
		}
	}
	else
		Properties::setProperty("sbw.broker.no-remote","false");

	if (flagRemote)
	{
		TRACE("Remote mode enabled");
	}
	else
	{
		Properties::setProperty("sbw.broker.remote-from","");
	}

	if (flagAllowRemoteModules)
	{
		TRACE("Remote module connections are allowed");
	}
	else
		Properties::setProperty("sbw.broker.allow-remote-modules","false");

	if (flagReloadKey)
		TRACE("--reload-key flag given; will reuse session key");

	//Config.resetValues();
}

void BrokerApplication::initSessionKey(bool running)
{
	if (running || flagReloadKey)
	{
		if (StringUtil::empty(BrokerSessionKey::getKey()))
		{
			printError("Unable to load session key from runtime file");
			quit(STATUS_CANNOT_LOAD_KEY);
		}
	}
	else
	{
		BrokerSessionKey::generateKey();
		BrokerSessionKey::saveKey();
	}
}
void shutdownHandler()
{
	TRACE("The Broker was terminated ... clearing properties");
	Properties::changeProperty("sbw.module.port","");
	Properties::changeProperty("sbw.port.port","");
	Properties::save();
	exit(-1);
}
void BrokerApplication::startBroker()
{	
  SBWBrokerRPC rpc;
	set_terminate(shutdownHandler);
  SBW::setInternalAPI(&rpc);

	// Create the Broker service.  We bypass the use of ModuleImpl and
	// set up a receiver directly.

	ObjectOrientedReceiver *receiver = new ObjectOrientedReceiver("BROKER");
	receiver->addService("BROKER", "BROKER", "/", new CBroker(),
		"The BROKER service implements the services of the SBW Broker.");
  rpc.registerReceiver(receiver);
	try
	{
    rpc.startBrokerRPC();
		// The call above will return normally 
		// and this method would quit and end the program ... so block it.

    while(!(rpc.ShuttingDown()))
		{ 
			try {SBWThread::sleep(100);} catch(...){} 
		}
	}
	catch (SBWException *e)
	{
		printError(e->getMessage());
    delete e;
		printError("SBW Broker exiting");
		quit(STATUS_CANNOT_START);
  }

	TRACE("BROKER was shut down");
	quit(STATUS_NORMAL);

}
void BrokerApplication::echoForRemote(bool previouslyRunning)
{
	TRACE("Looking for available local port");

	int port = Properties::getPropertyAsInt("sbw.broker.port");

	if (previouslyRunning)
	{
		if (port != -1)
		{			
			TRACE("sbw.broker.port says Broker running on " << port);
		}
		else
		{
			TRACE("Broker running, but sbw.broker.port not set");
			quit(STATUS_CANNOT_START);
		}
	}
	else
	{
		TRACE("sbw.broker.port says Broker running on " << port);
	}

	cout << port << "," << BrokerSessionKey::getKey() << "\n";	
}
string BrokerApplication::statusToString(int statusCode)
{
	switch (statusCode)
	{
	case STATUS_NORMAL:
		return "Normal exit";
	case STATUS_BAD_ARG:
		return "Bad argument given";
	case STATUS_BROKER_RUNNING:
		return "Already running";
	case STATUS_CANNOT_START:
		return "Unable to start";
	case STATUS_CANNOT_LOAD_KEY:
		return "Unable to load authentication session key from file";
	case STATUS_CANNOT_SHUTDOWN:
		return "Unable to shut down running Broker";
	default:
    {
      std::stringstream str;
      str << "Unknown status code: " << statusCode;
		  return str.str();
    }
	}
}
void BrokerApplication::printInfo()
{
	cout << "SBW Broker version " <<  CBroker::getVersion() <<  ": ";
	if (brokerIsRunning())
	{
		cout << "running, ";

		int port = Properties::getPropertyAsInt("sbw.module.port");

		if (port != -1)
			cout << "module port = " << port <<  ", ";
		else
			cout << "no module port, ";

		port = Properties::getPropertyAsInt("sbw.broker.port");
		if (port != -1 )
			cout << "broker port = " << port;
		else
			cout << "no broker port";

		cout << ".\n";
	}
	else
	{
		cout << "not running\n";
	}
}
void BrokerApplication::printUsageAndExit(int exitCode)
{
#ifdef WIN32
	string e = "\n"; //Properties::getProperty("line.separator");
#else
	string e = "\r\n";
#endif

	string u =
		"This is the SBW Broker, version " + CBroker::getVersion() + "." + e +
		"" + e +
		"The SBW Broker runs as a background process on a computer to" + e +
		"coordinate the activities of SBW modules.  It enables locating" + e +
		"and starting modules on demand.  The SBW Broker should not" + e +
		"normally need to be started explicitly, because the SBW" + e +
		"libraries start the Broker if it is not already running." + e +
		"However, some functions (such as the --shutdown command)" + e +
		"do need to be invoked directly." + e +
		e +
		"Usage: C++Broker [options]" + e +
		e +
		"Options:" + e +
		e +
		"-a | --accept-address ADDRESS" + e +
		e +
		"  This is for use with host-based (non-SSH) authentication and" + e +
		"  when modules from other hosts are allowed to connect to the" + e +
		"  Broker.  This option adds the given ADDRESS to the list of" + e +
		"  host addresses allowed to connect to this SBW Broker.  The" + e +
		"  address must be specified in IPv4 dotted quad notation:" + e +
		"  \"123.123.123.123\".  Any of the four fields can be replaced" + e +
		"  with the wildcard character '*' to indicate that any value" + e +
		"  for that field is acceptable.  So, for example," + e +
		"  \"132.215.42.*\" indicates any host on the subnet" + e +
		"  \"132.215.42.*\" may connect to the Broker.  This option" + e +
		"  interacts with the --deny-address option documented below in" + e +
		"  the following way: addresses that are allowed, and not" + e +
		"  explicitly denied, are permitted.  (This option has no" + e +
		"  effect unless the options --remote-modules or --no-ssh are" + e +
		"  also given.)" + e +
		e +
		"-b | --broker-ports LOW-HIGH" + e +
		e +
		"  By default, the Broker opens a port for inter-Broker" + e +
		"  communications by searching for the first free port in the" + e +
		"  range 10102 through 10202, inclusive.  This option allows" + e +
		"  you to specify a different port range, from port number LOW" + e +
		"  through port number HIGH, inclusive.  For example," + e +
		"  \"--broker-ports 11000-12000\" would dictate that the Broker" + e +
		"  should look for a free port in the range 10001 through" + e +
		"  10010.  You can force the Broker to try to use one" + e +
		"  particular port by using the same number for LOW and HIGH." + e +
		e +
		"-d | --deny-address ADDRESS" + e +
		e +
		"  This is for use with host-based (non-SSH) authentication and" + e +
		"  when modules from other hosts are allowed to connect to the" + e +
		"  Broker.  This option the given ADDRESS to the list of host" + e +
		"  addresses prevented from connecting to this SBW Broker.  The" + e +
		"  address must be specified in IPv4 dotted quad notation:" + e +
		"  \"123.123.123.123\".  Any of the four fields can be replaced" + e +
		"  with the wildcard character '*' to indicate that any value" + e +
		"  for that field is acceptable.  So, for example," + e +
		"  \"132.215.42.*\" indicates any host on the subnet" + e +
		"  \"132.215.42.*\" is denied from connecting to the Broker." + e +
		"  This option interacts with the --accept-address option" + e +
		"  documented above in the following way: addresses that are" + e +
		"  allowed, and not explicitly denied, are permitted.  (This" + e +
		"  option has no effect unless the options --remote-modules or" + e +
		"  --no-ssh are also given.)" + e +
		e +
		"-e | --remote-modules" + e +
		e +
		"  By default, only connections originating from the local host" + e +
		"  can connect to the Broker.  The --remote-modules flag" + e +
		"  enables modules running on hosts other than the local host" + e +
		"  to connect to this Broker.  This flag is not needed in" + e +
		"  normal SBW use scenarios because a module running on a" + e +
		"  remote host should normally be connected to a Broker running" + e +
		"  on that host.  Also, note that this flag only affects" + e +
		"  whether network connections from non-local hosts are" + e +
		"  permitted; a remote module will still be denied if it does" + e +
		"  not have the same session key as is being used by the" + e +
		"  Broker." + e +
		e +
		"-g | --registry PATHNAME" + e +
		e +
		"  This flag specifies that the file given by PATHNAME should" + e +
		"  be used as the Broker's module registry file, instead of the" + e +
		"  Broker's built-in default." + e +
		e +
		"-h | --help" + e +
		e +
		"  This flag tells the Broker to print this help text and exit." + e +
		e +
		"-i | --info" + e +
		e +
		"  This produces a description of the running state of the" + e +
		"  Broker (if there is a Broker running on this computer) and" + e +
		"  then exits without doing anything else or processing any" + e +
		"  other flags." + e +
		e +
		"-k | --key-file PATHNAME" + e +
		e +
		"  Use the file given by PATHNAME as the session key file" + e +
		"  instead of the default pathname.  The default session" + e +
		"  authentication key is stored it in a private file inside the" + e +
		"  \".sbw\" directory/folder in the user's home directory." + e +
		e +
		"-l | --reload-key" + e +
		e +
		"  Normally, if a Broker is not already running on the local" + e +
		"  host when a new Broker is started, the Broker generates a" + e +
		"  new session authentication key and stores it in a private" + e +
		"  file inside the \".sbw\" directory/folder in the user's home" + e +
		"  directory.  The --reload-key flag tells the Broker to not" + e +
		"  generate a new key, and instead to reuse the key in the key" + e +
		"  file.   This flag should not normally be used; it is" + e +
		"  primarily for use by remote Brokers." + e +
		e +
		"-m | --module-ports LOW-HIGH" + e +
		e +
		"  By default, the Broker opens a port for module connections" + e +
		"  by searching for the first free port in the range 10002" + e +
		"  through 10102, inclusive.  This option allows you to specify" + e +
		"  a different port range, from port number LOW through port" + e +
		"  number HIGH, inclusive.  For example, \"--module-ports" + e +
		"  11000-12000\" would dictate that the Broker should look for a" + e +
		"  free port in the range 10001 through 10010.  You can force" + e +
		"  the Broker to try to use one particular port by using the" + e +
		"  same number for LOW and HIGH." + e +
		e +
		"-o | --module-options \"OPTIONS STRING\"" + e +
		e +
		"  Use the given options as command-line options to modules" + e +
		"  when they are started up.  THIS OPTION CURRENTLY DOES NOT" + e +
		"  WORK." + e +
		e +
		"-R | --no-remote" + e +
		e +
		"  By default, the Broker is always listening for connections" + e +
		"  from other Brokers.  This flag tells the Broker to disallow" + e +
		"  remote Broker connections.  It does this by not starting up" + e +
		"  the listener thread." + e +
		e +
		"-r | --remote-from HOSTNAME" + e +
		e +
		"  Flag handed to a Broker when it is started remotely by" + e +
		"  another Broker.  The HOSTNAME is the host that started this" + e +
		"  Broker." + e +
		e +
		"-S | --no-ssh" + e +
		e +
		"  Disable the use of SSH for remote Broker communications.  It" + e +
		"  will still be possible to connect Brokers manually, but" + e +
		"  there will be no security on the communications channel and" + e +
		"  it will require manually copying session key files between" + e +
		"  hosts and starting the individual Brokers.  Use of this" + e +
		"  option is not recommended." + e +
		e +
		"-s | --sbw-home PATHNAME" + e +
		e +
		"  Change what is assumed to be the root directory of the SBW" + e +
		"  installation to PATHNAME.  By default, SBW uses various" + e +
		"  approaches to determine the root of its installation; this" + e +
		"  flag allows you to override them and explicitly set the path" + e +
		"  to the installation directory." + e +
		e +
		"-t | --trace" + e +
		e +
		"  This flag can only be used with the debugging copy of the" + e +
		"  Broker (i.e., file SBWBroker-debug.jar).  It turns on" + e +
		"  tracing of activities in the Broker and leads to copious" + e +
		"  debugging output." + e +
		e +
		"-U | --shutdown" + e +
		e +
		"  This shuts down a running Broker (if any) on the local" + e +
		"  host.  A new Broker is not started.  This is the preferred" + e +
		"  method for shutting down Brokers." + e +
		e +
		"-u | --user-dir PATHNAME" + e +
		e +
		"  By default, SBW stores information in a user-specific" + e +
		"  subdirectory/folder called \".sbw\" in the user's home" + e +
		"  directory.  This flag allows you to override the default and" + e +
		"  set the user directory to an explicit PATHNAME.  This may be" + e +
		"  useful in distributed computing environments where home" + e +
		"  directories are not where they're expected." + e +
		e +
		"-w | --module-wait INTEGER" + e +
		e +
		"  The amount of time, in tenths of a second, that the Broker" + e +
		"  should wait for a module to start up.  The default is 600" + e +
		"  (1 minute)." + e +
		e + e +
		"The SBW home page is http://www.sys-bio.org." + e +
		"Please send bug reports and question to sysbio-team@caltech.edu" + e;

	// FIXME finish

	cout << "\n" << u << "\n";
	exit(exitCode);
}
void BrokerApplication::printError(string msg)
{
	cerr << msg << "\n";
}
bool BrokerApplication::addressIsValid(string &addr)
{
	int pos = 0;
	for (int i = 3; i > 0; i--)
		if ((pos = (int)addr.find('.', ++pos)) <= 0)
			return false;

	return true;
}
bool BrokerApplication::brokerIsRunning()
{

	// An entry in the properties file implies a running broker.  Check
	// if it still seems there.  (Note: this only tests if *something*
	// is using the port; it doesn't actually check it's a broker.)

	int port = RuntimeProperties::getSBWModulePort();
	if (port > 0)
	{
		TRACE("sbw.module.port set in properties file to " <<  port);

		if (SBWModuleRPC::fileExists(Config::getSessionKeyFileC()))
		{
			try
			{
				SBWOSSocket *socket = new SBWOSSocket(port, "localhost");
				std::string sKey = SessionKey::getKey();				
				const char *key = sKey.c_str();
				int length = 0;
				unsigned char *message
					= DataBlockWriter::createConnectMessage(key, "test", length);
				socket->transmit(message, length);
				delete[] message;
				sbwDataBlockReader::readInteger(socket);

				return true;
			}
			catch (SBWException *)
			{
					
			}			
		}
		
	}
	else 
	{
		// port == NULL.  Perhaps something mangled the number in the
		// properties file.  Fall through & return false; hopefully
		// other code will overwrite the value with something valid.
		TRACE("No sbw.module.port in properties file");
	}

	return false;
}
void BrokerApplication::shutDownBroker()
{
	if (! brokerIsRunning())
	{
		TRACE("There does not appear to be a Broker running");
		quit(STATUS_BROKER_RUNNING);
	}
	string sKey = BrokerSessionKey::getKey();
	if (sKey.length() == 0)
	{
		printError("Unable to load session key from runtime file");
		printError("Cannot shut down Broker -- you must do it manually");
		quit(STATUS_CANNOT_LOAD_KEY);
	}

	// We act as a client here.	
	try
	{
		//DataBlockWriter data ;
		SystemsBiologyWorkbench::SBW::connect();

		// Query the Broker for the ID of the shutdown method.

		TRACE("Asking Broker for the shutdownBroker method");

		//data.add(0);                // BROKER service id = 0.
		//Module oBrokerModule	= SystemsBiologyWorkbench::SBW::getModuleInstance("BROKER");
		//Service oBrokerService	= oBrokerModule.findServiceByName("BROKER");
		//ServiceMethod oMethod	= oBrokerService.getMethod("void shutdownBroker()");
		//oMethod.send(data);

		SBWMethodCall(-1, 0, 17, "void shutdownBroker()");

		// the following lines should not be needed because at this point the broker should
		// be down already ... 

		SystemsBiologyWorkbench::SBW::disconnect();

		// remove port from properties file ... 
		Properties::changeProperty("sbw.module.port","");
		Properties::changeProperty("sbw.port.port","");
		Properties::save();

	}
	catch (SBWException *e)
	{
		TRACE(e->getMessage());
		cout << e->getMessage() << "\n";
		delete e;
	}
	catch (...)
	{
		// at this point all is well so we don't care
	}
	quit(STATUS_NORMAL);

}

bool BrokerApplication::checkDirArg(string path, string arg)
{

	if (StringUtil::empty(path))
	{
		printError(arg + " requires a pathname argument");
		printUsageAndExit(STATUS_BAD_ARG);
		return false;
	}
	path.append("/.");
	ifstream oFile(path.c_str());		
	if (! oFile.is_open())
	{
		printError("Given filename is not a directory: " + path);
		printUsageAndExit(STATUS_BAD_ARG);
		return false;
	}
	return true;
}
bool BrokerApplication::checkFileArg(string path, string arg)
{
	if (path.empty())
	{
		printError(arg + " requires a pathname argument");
		printUsageAndExit(STATUS_BAD_ARG);
		return false;
	}

	ifstream oFile(path.c_str());
	if (! oFile.is_open())
	{
		printError("Given filename is invalid: " + path);
		printUsageAndExit(STATUS_BAD_ARG);
		return false;
	}
	oFile.close();
	return true;
}
bool BrokerApplication::flagDebug = false;
bool BrokerApplication::flagNoRemoteBrokers = false;
bool BrokerApplication::flagRemote = false;
bool BrokerApplication::flagAllowRemoteModules = false;
bool BrokerApplication::flagReloadKey = false;
bool BrokerApplication::flagShutdown = false;
