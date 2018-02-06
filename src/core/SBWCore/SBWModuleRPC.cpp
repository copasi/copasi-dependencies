/**
 * @file SBWModuleRPC.cpp
 * @brief low level socket management that is specific to a module (rather than the broker)
 * 
 * This file is part of SBW.  Please visit http://sbw.sf.org for more
 * information about SBW, and the latest version of libSBW.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the New BSD license.
 *
 * Copyright (c) 2010-2014, Frank T. Bergmann and 
 *                          University of Washington
 * Copyright (c) 2008-2010, University of Washington and 
 *                          Keck Graduate Institute.
 * Copyright (c) 2005-2008, Keck Graduate Institute.
 * Copyright (c) 2001-2004, California Institute of Technology and
 *               Japan Science and Technology Corporation.
 * 
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are 
 * met: 
 * 
 * 1. Redistributions of source code must retain the above 
 *    copyright notice, this list of conditions and the following disclaimer. 
 * 
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 * 
 * 3. Neither the name of the copyright holder nor the names of its 
 *    contributors may be used to endorse or promote products derived from 
 *    this software without specific prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * The original code contained here was initially developed by:
 *
 *     Andrew Finney, Herbert Sauro, Michael Hucka, Hamid Bolouri
 *     The Systems Biology Workbench Development Group
 *     ERATO Kitano Systems Biology Project
 *     Control and Dynamical Systems, MC 107-81
 *     California Institute of Technology
 *     Pasadena, CA, 91125, USA
 *
 *
 * Contributor(s):
 *
 */


// SBWModuleRPC.cpp: implementation of the SBWModuleRPC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SBWModuleRPC.h"
#include <SBW/portableOS.h>

#ifdef WIN32

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef _CRT_NONSTDC_NO_WARNINGBS
#define _CRT_NONSTDC_NO_WARNINGBS
#endif //_CRT_NONSTDC_NO_WARNINGBS

#include <io.h>

#endif

#include "SBWOSSocket.h"
#include "SBWRawException.h"
#include "DataBlockWriterClass.h"
#include "SBWCommunicationException.h"
#include "SBWBrokerStartException.h"
#include "DataBlockWriter.h"
#include "SBWConfig.h"
#include "SessionKey.h"
#include "RuntimeProperties.h"

#if defined(LINUX) || defined(DARWIN) || defined (SOLARIS)
#include <limits.h>
#define MAX_PATH PATH_MAX
#endif


#include <sys/stat.h>


using namespace SystemsBiologyWorkbench ;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/**
* create lowlevel socket connection for module or application.
* @param initialListener default event listener
*/
SBWModuleRPC::SBWModuleRPC(SBWRPCListener *initialListener)
: SBWRPC(initialListener)
, SBWThread("socket receive")
, socket(NULL)
, state(Disconnected)
, transmitMutex("transmit")
{
	// At the point this is created, a new SBWThread is created
	// but it is not yet start()'ed.
}

/// release resources used by SBWModuleRPC object.
SBWModuleRPC::~SBWModuleRPC()
{
}

/** 
* returns whether this application process is connected to the broker.
* @return whether this application process is connected to the broker.
*/
bool SBWModuleRPC::isConnected()
{
	return state == Connected ;
}

/**
* connect this application process to the broker.
* When this is invoked, the thread has not yet been started for
* SBWModuleRPC's run() method.  So this is executed in the single
* initial thread.
* @param moduleIdentificationString module identification string which indicates module this
*                                   application process is an instance of.
* @param hostname the hostname of machine on which you wish to connect. "" implies local
*/
void SBWModuleRPC::connect(const char *moduleName, const char *hostname)
{
	// change of plans ... if a module wants to connect to a remote broker
	// instead of connecting to the local broker we should check the hostname ... 

	// the first thing should be to make sure that previous disconnects completed
	if (state == Disconnecting)
	{
		sleep(100);
		state = Disconnected;
	}

	if (state != Disconnected)
	{
		TRACE("Tried to connect, but already connected");
		return;
	}

	int port = RuntimeProperties::getSBWModulePort();		

	state = Connecting;

	SBWOSMutexLock ml(transmitMutex);

	if (port > 0)
	{
		TRACE("Found sbw.module.port in runtime properties file: (" << port << ")");
		// since the code below seems not to work on Windows Server 2003 ... try this one:

		try
		{
			if (connectToBroker(moduleName, hostname, port))
				state = Connected;
			else			
				state = Disconnected;
		}
		catch(SBWException *e)
		{
			delete e;
			state = Disconnected;
		}

		//if (SBWOSSocket::portInUse(port) || !SBWOSSocket::isLocalAddress(hostname) )
		//{
		//	TRACE("Since port is in use, will assume Broker is running");
		//	if (connectToBroker(moduleName, hostname, port))
		//		state = Connected;
		//	else
		//	{
		//		state = Disconnected;
		//		// i think we can do without this exception ... if something goes wrong ... 
		//		// try starting up a new broker
		//		/*throw new SBWCommunicationException(
		//			"Unable to connect to the SBW Broker",
		//			"The SBW Broker appears to be running, based on"
		//			" the fact that the runtime properties file contains"
		//			" a port number and that port is in use.  However,"
		//			" the client library was unable to connect to the"
		//			" Broker");*/
		//	}
		//}
		//else
		//	TRACE("sbw.module.port is not in use");
	}

	if (state != Connected)
	{
		TRACE("Broker does not appear to be running");
		port = startBroker();

		// 2002-05-31 There's some weird timing bug we don't
		// understand, but re-reading the file here seems to
		// work around it.  FIXME.

		port = RuntimeProperties::getSBWModulePort();

		if (port <= 0)
		{
			state = Disconnected ;
			throw new SBWCommunicationException(
				"Unable to start the SBW Broker",
				"The SBW Broker was not running and the"
				" client library could not start it up.");
		}

		Integer attempts = 120;

		while (attempts != 0 && !connectToBroker(moduleName, hostname, port))
		{
			SBWThread::sleep(1000); 
			attempts--;
		}

		if (state != Connected)
		{
			throw new SBWCommunicationException(
				"Unable to connect to the SBW Broker",
				"The client library appears to have"
				" successfully started up the SBW Broker,"
				" but it was then unable to connect to the"
				" Broker.");
		}
	}

	TRACE("Successfully connected to Broker as module " << moduleId);
	start();
	TRACE("Started message thread; ending connect phase");
}


/**
* Helper
**/
int SBWModuleRPC::startBroker()
{
	std::string commandLine;
	bool bFound = false;
	/* Temporarily disabling the use of startbroker.bat under windows */

  std::string brokerStartupScript = Config::getSBWUserDir();
	brokerStartupScript+= SBWOS::DirectorySeparator();
	brokerStartupScript+= "brokerfile";
	if (fileExists(brokerStartupScript.c_str())) 
	{
		TRACE("found startup script - loading broker from: " << brokerStartupScript);
		FILE* oTempFile = fopen(brokerStartupScript.c_str(), "r");
		if (oTempFile != NULL)
		{
			char* sTemp = (char*) malloc(sizeof(char)*MAX_PATH);
			memset(sTemp, 0, sizeof(char)*MAX_PATH);			
			if (fgets(sTemp, MAX_PATH,oTempFile) != NULL)
			{
				std::string sRiddiculous = sTemp;
				sRiddiculous[sRiddiculous.length()-1]  = '\0';
				
				if (fileExists(sRiddiculous.c_str()))
				{
					commandLine = sRiddiculous;
					fclose(oTempFile);
					bFound = true;
					TRACE("broker specified in script found. Starting: " << sRiddiculous);
				}
				else if(fileExists(sTemp))
				{
					commandLine = sTemp;
					fclose(oTempFile);
					bFound = true;
					TRACE("broker specified in script found. Starting: " << sTemp);
				}
				else
				{
					TRACE("broker specified in script could not be found.");
				}
			}
			free (sTemp);

		}
	}
	
	if (!bFound)
	{

		brokerStartupScript = Config::getSBWHome();
		brokerStartupScript += SBWOS::DirectorySeparator();
		brokerStartupScript += "bin";
		brokerStartupScript += SBWOS::DirectorySeparator();
#ifdef WIN32
		// instead of calling the batchfile which will create a console
		// call directly the broker
		brokerStartupScript += "C++Broker.exe";
#else
		brokerStartupScript += "Broker";
#endif
		if (fileExists(brokerStartupScript.c_str()))
		{
			TRACE("Startup script is " << brokerStartupScript);
			commandLine = brokerStartupScript;
		}
		else
		{
			TRACE("Cannot find Broker startup script; trying Broker executable");
			TRACE ("tried: " << brokerStartupScript);
			
			throw new SBWBrokerStartException("Unable to start the SBW Broker",
										   "The Broker executable was not found.");
			//exit(-1);

		}
	}

	std::string file = RuntimeProperties::getPropertiesFile();
	time_t time;
	struct stat statbuff;

	if (stat(file.c_str(), &statbuff) == 0)
		time = statbuff.st_mtime;
	else
		time = 0;

	TRACE("original stat time = " << time);

	TRACE("Attempting to start Broker using command: " << commandLine.c_str());

	SBWOS::startProcess((char *)commandLine.c_str());

	int attempts = 25;
	int port = -1;

	try
	{
		while (attempts != 0 && port < 0)
		{
			SBWThread::sleep(200);
			if (stat(file.c_str(), &statbuff) == 0)
			{
				TRACE("stat = " << statbuff.st_mtime);
				if (time < statbuff.st_mtime
					&& (port = RuntimeProperties::getSBWModulePort()) > 0)
					return port;
			}
			attempts--;
		}

		if ((port = RuntimeProperties::getSBWModulePort()) > 0)
			return port;
		else 
			state = Disconnected;
	}
	catch (SBWException *)
	{
		try
		{
			delete socket ;
      socket = NULL;
		}
		catch (SBWException *)
		{
			// delibrately ignored - don't know or care if we have good connection
		}
		state = Disconnected;
		throw;
	}
	return -1;
}


bool SBWModuleRPC::fileExists(const char *path)
{
	if (path == NULL || (int)strlen(path)==0) return 0;
#ifdef LINUX
	struct stat statbuff;
#endif
#ifdef WIN32
#if  _MSC_VER >= 1400 && !defined(_CRT_SECURE_NO_DEPRECATE)
	return _access(path, 0 ) != -1;
#else
	return access(path, 0 ) != -1;
#endif
#else
	// printf("Stat'ing %s\n", jarFile.c_str());
	return stat(path, &statbuff) != -1;
#endif
}


bool SBWModuleRPC::connectToBroker(const char *moduleName,
								   const char *hostname, int port)
{
	if (hostname == NULL || strlen(hostname) == 0)
	{
		TRACE("Trying to connect to Broker on localhost port " << port);
	}
	else
	{
		TRACE("Trying to connect to Broker on host " << hostname
			<< ", port " << port);
	}

	try
	{
		socket = new SBWOSSocket(port, hostname);
		connectMessageStreams(moduleName);
		state = Connected;
		return true;
	}
	catch (SBWConnectException *e)
	{
		state = Disconnected ;
#ifdef DEBUG
		e->log();
#endif
		delete e;
	}
	catch (SBWException *e)
	{
		state = Disconnected ;
#ifdef DEBUG
		e->log();
#endif
		delete e ;
	}
	return false;
}


void SBWModuleRPC::connectMessageStreams(const char *moduleName)
{
	TRACE("Trying to connect message streams");
	try
	{
		Integer length;
		std::string sKey = SessionKey::getKey();
		unsigned char *message
      = DataBlockWriter::createConnectMessage(sKey.c_str(), moduleName, length);
		socket->transmit(message, length);
		delete[] message;
		
		moduleId = sbwDataBlockReader::readInteger(socket);
		TRACE("Successfully connected message streams");
	}
	catch (SBWConnectException *)
	{
		TRACE("Could not connect message streams; disconnecting");
		try
		{
			delete socket;
      socket = NULL;
		}    
		catch (SBWException *)
		{
			// deliberately ignored - don't know or care if we have good connection
		}
    catch (const SBWException &)
    {
      // deliberately ignored - don't know or care if we have good connection
    }
		state = Disconnected;
		throw;
	}
}

/**
* send a message to the broker requesting that it disconnects from this application process.
* This method doesn't block until the disconnect occurs but instead returns as soon as the message has been transmitted.
*/
void SBWModuleRPC::signalDisconnect()
{
	if (state == Connected || state == Connecting)
	{
		try
		{
			Integer length ;
			unsigned char *message = DataBlockWriter::createDisconnectMessage(length);

			transmit(SBWBrokerModule, message, length);
		}
		catch (SBWException *e)
		{
			delete e ; // don't care if we've already disconnected!!
		}

		state = Disconnecting ;
	}
}

/**
* block until this application disconnects from the broker
*/
void SBWModuleRPC::waitForDisconnect()
{
	TRACE("rejoining thread");
	join();
}

/**
* return the numeric module instance identifier for this application process.
* @return the numeric module instance identifier for this application process.
*/
Integer SBWModuleRPC::getModuleId()
{
	if (state != Connected)
		throw new SBWCommunicationException("application has no module id when not connected");

	return moduleId;
}

/**
* transmit a message to another module or the broker.
* @param destinationModuleId numeric moduleinstance identifier of the destination.
*	                          guarenteed not to refer to this application.  can refer to the broker.
* @param message data to be transmitted.
* @param length length of data.
*/
void SBWModuleRPC::transmitExternalOnly(Integer /*destinationModuleId*/, unsigned char *message, Integer length)
{
	try
	{
		SBWOSMutexLock ml(transmitMutex);

		if (state == Disconnected)
			throw new SBWCommunicationException("attempting to transmit when not connected");

		socket->transmit(message, length);
		delete[] message ;
	}
	catch (SBWDisconnectException *e)
	{
		state = Disconnecting ;
		delete e ;
		delete[] message ;
		throw new SBWCommunicationException("broker disconnected");
	}
	catch (SBWException *)
	{
		delete[] message ; // just because C++ has no finally!
		throw ;
	}
}

/**
* Receive messages from the broker (and by implication other modules) and process those messages.
* This function executes until this module/application process disconnects from the broker and
* is normally run inside a thread.
*/
void SBWModuleRPC::run()
{
	TRACE("Receiver thread started");

	while (state == Connected)
	{
		try
		{
			TRACE("Waiting on socket for new message");

			Integer length = sbwDataBlockReader::readInteger(socket);
			Integer toModuleId = sbwDataBlockReader::readInteger(socket);

			TRACE("New message received");

			if (toModuleId == moduleId)
			{
				length -= 8;
				unsigned char *raw = new unsigned char[length];

				socket->receive(raw, length);
				receive(DataBlockReader(raw, length, false));  // don't duplicate/copy raw data
			}
			else
				SBWException::log("received message for different module");
		}
		catch (SBWDisconnectException *e)
		{
			delete e ;
			state = Disconnecting ;
		}
		catch (SBWException *e)
		{
			e->log();
			delete e;
		}
    catch (const SBWException &e)
    {
      e.log();
    }
		catch (...) {}
	}

	{
		SBWOSMutexLock ml(transmitMutex);
		try
		{
			delete socket;
      socket = NULL;
		}
		catch (SBWException * e)
		{
			// deliberately ignored - don't know or care if we have good connection
      delete e;
		}
    catch (...)
    {
      // deliberately ignored - don't know or care if we have good connection
    }
	}

	cleanup();
	state = Disconnected ;
}
