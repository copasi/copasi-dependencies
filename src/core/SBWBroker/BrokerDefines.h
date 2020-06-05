#ifndef SBW_BROKER_DEFINES_H
#define SBW_BROKER_DEFINES_H

#define VERSION "2.12.2"

#include <iostream>

#ifndef WIN32
#define MKDIR(file) (mkdir(file, 0755))
#else
#if _MSC_VER > 1400
#define MKDIR(file) (_mkdir(file))
#else
#define MKDIR(file) (mkdir(file))
#endif
#endif

#define STARTUP_WAIT		60

/* \def DEFAULT_BROKER_STARTUP_WAIT defines the time the broker waits for a module to start up */
#define DEFAULT_BROKER_STARTUP_WAIT		600



/* \def BROKER_MODULE defines the module identifier of the broker */
#define BROKER_MODULE					-1
/* \def SYSTEM_SERVICE defines the service identifier for the system service */
#define SYSTEM_SERVICE					-1

/* \def GET_SERVICES_METHOD defines the id of the getService method */
#define GET_SERVICES_METHOD				0
/* \def GET_METHODS_METHOD defines the id of the getMethods method */
#define GET_METHODS_METHOD				1
/* \def ON_OTHER_MODULE_INSTANCE_SHUTDOWN_METHOD defines the id of the shutdown notification method */
#define ON_OTHER_MODULE_INSTANCE_SHUTDOWN_METHOD 2
/* \def SHUTDOWN_METHOD defines the id of the shutdown method */
#define SHUTDOWN_METHOD					3
/* \def GET_METHOD_HELP_METHOD defines the id of the getHelp method*/
#define GET_METHOD_HELP_METHOD			4
/* \def ON_OTHER_MODULE_INSTANCE_STARTUP_METHOD defines the id of the startup notification method */
#define ON_OTHER_MODULE_INSTANCE_STARTUP_METHOD 5
/* \def ON_REGISTRATION_CHANGE_METHOD */
#define ON_REGISTRATION_CHANGE_METHOD	6

/* \def REPLY_CODE defines the id of the reply code*/
#define REPLY_CODE						0
/* \def SEND_CODE defines the id of the send code*/
#define SEND_CODE						1
/* \def CALL_CODE defines the id of the call code*/
#define CALL_CODE						2
/* \def EXCEPTION_CODE defines the id of the exception code*/
#define EXCEPTION_CODE					3

/* \def DISCONNECT_MESSAGE specify the disconnect message */
#define DISCONNECT_MESSAGE				-1

/* \def STATUS_NORMAL normal status*/
#define STATUS_NORMAL					0
/* \def STATUS_BAD_ARG quit because of bad arguments */
#define STATUS_BAD_ARG					1
/* \def STATUS_BROKER_RUNNING quit because broker was already running */
#define STATUS_BROKER_RUNNING			2
/* \def STATUS_CANNOT_START failure to start*/
#define STATUS_CANNOT_START				3
/* \def STATUS_CANNOT_LOAD_KEY failure to start because session key couldn't be loaded*/
#define STATUS_CANNOT_LOAD_KEY			4
/* \def STATUS_CANNOT_SHUTDOWN failure to shutdown*/
#define STATUS_CANNOT_SHUTDOWN			5

/* \def UNIQUE unique module*/
#define UNIQUE							0
/* \def SELF_MANAGED self managed module */
#define SELF_MANAGED					1

/* \def DEFAULT_REGISTRY_FILENAME define the default registry name*/
#define DEFAULT_REGISTRY_FILENAME		"sbwreg.xml"

/* \def CREATE_STRING convert a standard type <code>x</code> into a existing string <code>target</code> */
#define CREATE_STRING(x,target)\
	{\
	std::stringstream strstr;\
	strstr << x;\
	target = strstr.str();\
	}\

#endif

#if !(defined SBW_UNREFERENCED_PARAMETER)
/* \def UNREFERENCED_PARAMETER unreferenced parameter should cause errors */
#ifdef LINUX
#define SBW_UNREFERENCED_PARAMETER(x) ((void)(x))
#else
#define SBW_UNREFERENCED_PARAMETER(x) ((x))
#endif
#endif
