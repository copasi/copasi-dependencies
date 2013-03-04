#include "BrokerDefines.h"
#include "ModuleRegistryFile.h"
#include "ModuleRegistry.h" 
#include "Sys.h"

#include <libxml/xmlreader.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#include "SBW/SBW.h"
#include "SBW/SBWConfig.h"
#include "SBW/DataBlockWriter.h"
#include "SBW/SBWApplicationException.h"

#include <string>
#include <ios>
#include <fstream>
#include <ctime>
#ifndef WIN32
#include <unistd.h>
#endif

using namespace std;
using namespace SystemsBiologyWorkbench::Broker;
using namespace SystemsBiologyWorkbench;

ModuleRegistryFile::~ModuleRegistryFile()
{
	xmlCleanupParser();
}
ModuleRegistryFile::ModuleRegistryFile(ModuleRegistry *registry) : registryMutex("registry Mutex")
{
	LIBXML_TEST_VERSION;
	this->registry = registry;
	this->registryFile = getRegistryFile();
	if (!exists())
	{
		// create the directory if necessary
		//Sys::createDirectory(getPath());
		Sys::createDirectory(Config::getRuntimeFilesDirectory() + SBWOS::DirectorySeparator());
	}
}
bool ModuleRegistryFile::exists()
{	
	FILE *regFile = fopen(registryFile.c_str(),"r");

	// Do some sanity checking on the value before returning it.

	if (regFile != NULL)
	{
		fclose(regFile);
		return true;

	}
	return false;

}
std::string ModuleRegistryFile::getPath()
{
	return Config::getSBWUserDir();
}
std::string ModuleRegistryFile::getRegistryFile()
{
	std::string sRegistryFile = Config::getSBWUserDir() + SBWOS::DirectorySeparator() + DEFAULT_REGISTRY_FILENAME;
	return sRegistryFile;
}
void ModuleRegistryFile::checkpoint()
{
	SBWOSMutexLock ml(registryMutex);
	writeRegistryFile();
}
void ModuleRegistryFile::parseNode(xmlNode *node) 
{
	xmlNode *current = NULL;
	bool bFound = false;
	std::string sName = "listOfModules";
	for (current = node->children; current; current = current->next)
	{
		if (sName == (char *)(current->name)) 
		{
			bFound = true;
			break;
		}
	}
	if (!bFound || current == NULL) 
	{
		throw new SBWApplicationException("listOfModules not found");
	}

	for (current = current->children; current; current = current->next)
	{
		if ( current->type == XML_ELEMENT_NODE) 
		{
			readModule(current);
		}
	}
}

void ModuleRegistryFile::readModule(xmlNode *oNode)
{
	std::string moduleName = getAttributeValue(oNode, "name");
	std::string moduleDisplayName = getAttributeValue(oNode, "displayName");
	std::string type = getAttributeValue(oNode, "type");
	std::string cmd = getAttributeValue(oNode, "startupCmd");
	std::string help = getAttributeValue(oNode, "help");

	DataBlockWriter oWriter;
	oWriter << moduleName << moduleDisplayName << typeToInt(type) << cmd << help;
	ModuleDescriptor *md = new ModuleDescriptor(oWriter.getReader());
	registry->addModuleDescriptor(moduleName, md, false);

	xmlNode *services = oNode->children;

	readService(services,md);
}
void ModuleRegistryFile::readService(xmlNode *oNode, ModuleDescriptor *md)
{
	if (oNode == NULL) 
	{
		TRACE( "Module '" << md->getName()<< "' has no <listOfServices>");
		return;
	}
	else 
	{
		if ( oNode->type != XML_ELEMENT_NODE) 
			oNode = oNode->next;
		for (oNode = oNode->children; oNode; oNode = oNode->next) 
		{
			if ( oNode ->type == XML_ELEMENT_NODE) 
			{
				std::string serviceName = getAttributeValue(oNode, "name");
				std::string serviceDisp = getAttributeValue(oNode, "displayName");
				std::string serviceCat  = getAttributeValue(oNode, "category");
				std::string serviceHelp = getAttributeValue(oNode, "help");

				DataBlockWriter oServiceWriter;
				oServiceWriter << md->getName() << serviceName << serviceDisp << serviceCat << serviceHelp;
				ServiceDescriptor *sd = new ServiceDescriptor (oServiceWriter.getReader(), *md);
				md -> addService(sd); 
			}
		}
	}
}
void ModuleRegistryFile::readRegistry()
{
	registryDoc		= NULL;
	registryRoot	= NULL;

	registryDoc = xmlParseFile(registryFile.c_str());
	if (registryDoc != NULL)
	{
		registryRoot = xmlDocGetRootElement(registryDoc);
		parseNode(registryRoot);
	}
	else 
	{
		// if we can't read the registry, throwing this exception will not help, because when 
		// we will start up next we will crash again. So we delete the file, throw the exception 
		// --> this will quit the program for now, *but* it will work the next time around
		unlink(registryFile.c_str());
		throw new SBWApplicationException("couldn't read registry.");
	}
}

void ModuleRegistryFile::writeRegistryFile()
{
#ifndef SBW_DONT_WRITE_REGISTRY
	try 
	{
		static char buffer [80];
		static time_t rawtime;
		static struct tm * timeinfo;

		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		strftime (buffer,80,"%c",timeinfo);


		xmlTextWriterPtr writer;
		xmlDocPtr doc;
  

		writer = xmlNewTextWriterDoc(&doc, 0);
		xmlTextWriterStartDocument(writer, NULL, "UTF8", NULL);
		xmlTextWriterStartElement(writer, BAD_CAST "SBWRegistry");
		xmlTextWriterWriteAttribute(writer, BAD_CAST "lastUpdated",
			BAD_CAST buffer);
		xmlTextWriterStartElement(writer, BAD_CAST "listOfModules");
		std::vector<ModuleDescriptor *> modules = registry->getModuleDescriptors();
		for (unsigned int i = 0; i < modules.size(); i++) 
		{
			writeModuleDescriptor(writer, modules[i]);			
		}
		xmlTextWriterEndElement(writer); // end listOfModules
		xmlTextWriterEndElement(writer); // end SBWRegistry
		xmlTextWriterEndDocument(writer);
		xmlFreeTextWriter(writer);
		xmlSaveFormatFile (registryFile.c_str(), doc, 1);
		xmlFreeDoc(doc);
	}
	catch (...)	
	{
		throw new SBWApplicationException("couldn't write registry...");
	}	
#endif
}

void ModuleRegistryFile::writeModuleDescriptor(xmlTextWriterPtr writer, ModuleDescriptor *md)
{
#ifndef SBW_DONT_WRITE_REGISTRY
	if (md != NULL && md->getName() != "BROKER") 
	{
		xmlChar *tmp;
		xmlTextWriterStartElement(writer, BAD_CAST "module");
		tmp = toUTF8(md->getName().c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST "name",
			BAD_CAST tmp);
		xmlFree(tmp);
		tmp = toUTF8(md->getDisplayName().c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST "displayName",
			BAD_CAST tmp);
		xmlFree(tmp);
		tmp = toUTF8((md->getManagementType() == UNIQUE ? "UNIQUE" : "SELF_MANAGED"));
		xmlTextWriterWriteAttribute(writer, BAD_CAST "type",
			BAD_CAST tmp);
		xmlFree(tmp);
		tmp = toUTF8(md->getCommandLine().c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST "startupCmd",
			BAD_CAST tmp);
		xmlFree(tmp);
		tmp = toUTF8(md->getHelp().c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST "help",
			BAD_CAST tmp);
		xmlFree(tmp);
		std::vector<ServiceDescriptor*> services = md ->getServices();
		if (services.size() > 0 ) {
			xmlTextWriterStartElement(writer, BAD_CAST "listOfServices");
			for (unsigned int j = 0; j < services.size(); j++) 
			{
				writeServiceDescriptor(writer, services[j]);
			}
			xmlTextWriterEndElement(writer); // end listOfServices
		}
		xmlTextWriterEndElement(writer); // end module
	}
#endif
}
void ModuleRegistryFile::writeServiceDescriptor(xmlTextWriterPtr writer, ServiceDescriptor *sd)
{
#ifndef SBW_DONT_WRITE_REGISTRY
	if (sd != NULL) 
	{
		xmlChar *tmp;
		xmlTextWriterStartElement(writer, BAD_CAST "service");
		tmp = toUTF8(sd->getName().c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST "name",
			BAD_CAST tmp);
		xmlFree(tmp);
		tmp = toUTF8(sd->getDisplayName().c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST "displayName",
			BAD_CAST tmp);
		xmlFree(tmp);
		tmp = toUTF8(sd->getCategory().c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST "category",
			BAD_CAST tmp);
		xmlFree(tmp);
		tmp = toUTF8(sd->getHelp().c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST "help",
			BAD_CAST tmp);
		xmlFree(tmp);
		xmlTextWriterEndElement(writer); // end service
	}
#endif
}

/**
* toUTF8:
* @in: string in a given encoding
*
* Returns the converted UTF-8 string, or NULL in case of error.
*/
xmlChar * ModuleRegistryFile::toUTF8(const char *in)
{
	int size  = (int) strlen(in) + 1;
	int out_size = size * 2 - 1;
	xmlChar *out = (unsigned char *) xmlMalloc((size_t) out_size);
	if (out == NULL)
		return NULL;
	memset (out, 0, sizeof(unsigned char)*out_size);
	isolat1ToUTF8((unsigned char*) out, &out_size, (unsigned char*) in, &size);
	out = (unsigned char *) xmlRealloc(out, out_size + 1);
	out[out_size] = 0;  /*null terminating out */
	return out;
}
int ModuleRegistryFile::typeToInt(std::string type)
{
	if (type == "UNIQUE") 
	{
		return UNIQUE;
	} 
	else if (type == "SELF_MANAGED")
	{
		return SELF_MANAGED;
	}
	else
		throw new SBWApplicationException("Invalid module management type found ...");
}
std::string ModuleRegistryFile::getAttributeValue(xmlNode *elem, std::string attrName)
{
	char *result;
	result = (char*) xmlGetProp(elem, (xmlChar *)attrName.c_str());
	if (result == NULL)
		return "";
	std::string sResult (result);
	xmlFree(result);
	return sResult;
}
ModuleRegistry *ModuleRegistryFile::registry = NULL;
std::string ModuleRegistryFile::registryFile = "";
xmlNode *ModuleRegistryFile::registryRoot = NULL;
xmlDoc *ModuleRegistryFile::registryDoc = NULL;
xmlNode *ModuleRegistryFile::registryListOfModules = NULL;
