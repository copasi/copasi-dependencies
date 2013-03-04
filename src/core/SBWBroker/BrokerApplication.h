#ifndef SBW_BROKER_APPLICATION
#define SBW_BROKER_APPLICATION

#include "BrokerDefines.h"

#include <iostream>
#include <string>

namespace SystemsBiologyWorkbench
{
	namespace Broker 
	{
		/**
		* \ingroup C++Broker
		*
		* The BrokerApplication class is being used to start up the CBroker. It also handles the supported 
		* command line aguments. 
		*
		*
		* \date 10-19-2004
		*
		* \author fbergman
		*
		*
		*/
		class BrokerApplication
		{
			static bool flagDebug;					///< flag indicating whether debug messages should appear. <b>false</b> by default
			static bool flagNoRemoteBrokers;		///< Cmd-line flag: do not allow remote brokers. <b>false</b> by default
			static bool flagRemote;					///< Cmd-line flag: whether this broker has been started remotely. <b>false</b> by default
			static bool flagAllowRemoteModules;		///< Cmd-line flag: whether to allow non-local module connections. <b>false</b> by default
			static bool flagReloadKey;				///< Cmd-line flag: do not generate new session key, but reload previous. <b>false</b> by default
			static bool flagShutdown;				///< Cmd-line flag: shutdown a running Broker. <b>false</b> by default
		public:
			/**
			* Default constructor ... nothing to be done here. 
			*/
			BrokerApplication() { }

			/**
			* The top-level main routine for the Broker.  This parses the command
			* line flags, takes appropriate actions, and starts the various Broker
			* threads.
			* <p>
			* @param argv the array of command line argument strings
			* @param argc the number of command line arguments
			*/
			void run(int argc, char* argv[]);
		private:
			/**
			* Terminates execution with a specific status code.
			* <p>
			* @param code an integer code to use as the exit status code
			*/
			void quit(int code);
			/**
			* Parses the Broker command line arguments and sets internal flags
			* and properties as appropriate.
			* <p>
			* @param argv the array of command line argument strings
			* @param argc the number of command line arguments
			**/
			void parseArgs(int argc, char* argv[]);
			/**
			* Performs initializations and user-directed actions that can be done
			* prior to actually starting the Broker.
			* <p>
			* @param running if <b>true</b> take the current session key otherwise generate new
			* @see run(int argc, char* argv[])
			**/
			void initSessionKey(bool running);
			/**
			* Starts the Broker.  This must be called after the command-line
			* flags have been parsed and initSession() has executed.
			* This creates the "BROKER" service and actually starts the Broker
			* for real.
			* <p>
			* @see #run
			* @see initSession()
			**/
			void startBroker();
			/**
			* Method invoked when the broker is started with the
			* <code>--remote-from</code> flag.  The flag indicates that this
			* Broker was started remotely by another Broker.  This method
			* echoes the following information on two separate lines on the
			* standard output stream:
			* <pre>
			*   PORT
			*   KEY</pre>
			* where
			* <ul>
			* <li><kbd>PORT</kbd> is the port to be used for Broker connections
			* <li><kbd>KEY</kbd> is the session key
			* </ul>
			* <p>
			* The parameter <code>running</code> modifies the behavior of this
			* method.  If the parameter is <code>true</code>, indicating the
			* Broker is already believed to be running on the local host, this
			* method reads the sbw.broker.port port being used from the runtime
			* properties file.  If <code>false</code>, this method searches for
			* the next available free port in the range of ports given to the
			* Broker, and reports that.
			* <p>
			* Note that this approach is not robust.  This method merely finds
			* a free port and prints it on the standard output stream; it does
			* not return the port to the caller or otherwise record the port
			* that was found.  Instead, <i>another</i> method elsewhere in
			* the Broker then ends up looking for a free port again and then
			* binds to it.  This leaves a short period of time during which
			* some external process might hook into the same port, between the
			* time this method prints the port it finds and the later method
			* finds the same port.  This is an area that needs improving in
			* the current implementation.
			* <p>
			* @param running whether the Broker is already running
			**/
			void echoForRemote(bool previouslyRunning);
			/**
			* Prints a message given a Broker exit status code.
			* This is useful for other parts of the Broker to be able to print
			* a diagnostic message after a Broker process exits.
			* <p>
			* @param statusCode the integer status code returned by a Broker process
			* @return a string message explaining the code
			**/
			static std::string statusToString(int statusCode);
			/**
			* Prints info about the state of the Broker, such as whether it's
			* running and on what port.
			**/
			void printInfo();
			/**
			* Prints the usage text and exits with an exit code.
			* <p>
			* @param exitCode to code to pass to <code>System.exit(code)</code>
			**/
			void printUsageAndExit(int exitCode);
			/**
			* Print a line on the error output stream.
			* <p>
			* @param msg the string to print on the error output stream
			**/
			void printError(std::string msg);
			/**
			* Checks that the given IP address string really looks like an IP
			* address.  No verification is done that the address corresponds
			* to an actual host anywhere; this is a purely syntactic check
			* on the form of the string.
			* <p>
			* @param addr the string to check for being a valid address
			* @returns <code>true</code> if the string looks like a valid address,
			* <code>false</code> otherwise
			**/
			bool addressIsValid(std::string &addr);
			/**
			* Returns <code>true</code> or <code>false</code> depending on whether
			* there appears to be a Broker process running on the current machine.
			* This is determined by looking in the runtime properties file for the
			* port number recorded by the running Broker (if any), and then checking
			* whether the port is actually in use on the local host.  If it cannot
			* parse the "sbw.module.port" property from the runtime properties file
			* or it encounters some other problem, it returns <code>false</code>
			* <p>
			* Note that the algorithm employed here is weak and may give an
			* erroneous result, in particular if there really is a Broker running
			* but something prevents the code from reading the "sbw.module.port"
			* property from the runtime properties file.
			* <p>
			* @return <code>true</code> if a Broker appears to be running on
			* the current host, <code>false</code> otherwise
			**/
			bool brokerIsRunning();
			/**
			* Shuts down a running Broker on the local host.
			* <p>
			* Unlike the rest of the Broker, this method actually behaves like an
			* SBW client.  It attempts to connect to the Broker as a client and
			* invoke the <code>shutdownBroker()</code> method on the Broker.
			**/
			void shutDownBroker();
			/**
			* Helper method to check whether a directory pathname argument
			* appears to be valid.
			* <p>
			* @param path the pathname given as the argument to the flag
			* @param arg a short string describing the flag, used in printing
			* error messages if there's a problem with the <code>path</code>
			* @return <code>true if the <code>path</code> really is a directory,
			* <code>false</code> otherwise
			**/
			bool checkDirArg(std::string path, std::string arg);
			/**
			* Helper method to check whether a file pathname argument
			* appears to be valid.
			* <p>
			* @param path the pathname given as the argument to the flag
			* @param arg a short string describing the flag, used in printing
			* error messages if there's a problem with the <code>path</code>
			* @return <code>true if the <code>path</code> really is a file,
			* <code>false</code> otherwise
			**/
			bool checkFileArg(std::string path, std::string arg);
		};
	}
}
#endif
