#ifndef SBW_BROKER_MODULE_REGISTRY_FILE
#define SBW_BROKER_MODULE_REGISTRY_FILE

#include "SBW/SBW.h"
#include "SBW/ModuleDescriptor.h"
#include "SBW/ServiceDescriptor.h"

#include <libxml/parser.h>
#include <libxml/xmlwriter.h>

#include <string>
#include <fstream>
#include <iostream>

namespace SystemsBiologyWorkbench
{	
	namespace Broker
	{
		class ModuleRegistry;

		/**
		 * \ingroup C++Broker
		 *
		 * The ModuleRegistryFile provides the interface to access the SBW registry file stored in the user
		 * directory. This class provides methods to read and write proper XML to/from this file. As XML library
		 * libXML was used http://www.libxml.org. 
		 *
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class ModuleRegistryFile
		{
		public:
			/**
			 * constructs a new object
			 * \param *registry the SBW registry			 
			 */
			ModuleRegistryFile(ModuleRegistry *registry);
			/// destructor used to clean up the xml parser
			~ModuleRegistryFile();
			/**
			 * checks whether the registry file exists 
			 * \return <b>true</b> if the file exists, <b>false</b> otherwise
			 */
			bool exists();
			/**
			 *
			 * \return the path to the registry file
			 */
			std::string getPath();
			/**
			 * save latest changes to the registry file
			 */
			void checkpoint();
			/**
			 * read and parse the XML file
			 */
			void readRegistry();
		private:

			SBWOSMutex registryMutex ;
			
			/**
			 * reads a module descriptor from the current node
			 * \param *oNode the current node
			 */
			void readModule(xmlNode *oNode);

			/**
			 * reads a service descriptor from the current node
			 * \param *oNode the current node	
			 * \param *md the module descriptor to add the services to			 
			 */
			void readService(xmlNode *oNode,ModuleDescriptor *md);


			/**
			 * writes the given module descriptor to the xml writer
			 * \param writer the xml writer
			 * \param *md the module descriptor
			 */
			void writeModuleDescriptor(xmlTextWriterPtr writer, ModuleDescriptor *md);
			
			/**
			 * writes the given ServiceDescriptor to the xml writer
			 * \param writer the xml writer
			 * \param *sd the service descriptor
			 */
			void writeServiceDescriptor(xmlTextWriterPtr writer, ServiceDescriptor *sd);

			/**
			 *
			 * \return the filename of the current registry
			 */
			std::string getRegistryFile();
			/**
			 * writes the registry into the XML file
			 */
			void writeRegistryFile();
			/**
			 * converts the given character string into UTF8
			 * \param *in the character string
			 * \return utf8 string representing the original one
			 */
			xmlChar * toUTF8(const char *in);
			/**
			 *  help function used to parse the XML document
			 * \param node root node of the document
			 */
			void parseNode(xmlNode * node);

			/**
			 * Convert the MangementType of a Module (UNIQUE or SELF_MANAGED) to an integer. 
			 * \param type the type as string
			 * \return the corresponding integer
			 */
			int typeToInt(std::string type);
			/**
			 * this help function extrects an attribute from a Node
			 * \param *elem the xmlNode containing the attribute
			 * \param attrName the name of the attribute
			 * \return the value of the attribut or "" of not existing
			 */
			std::string getAttributeValue(xmlNode *elem, std::string attrName);
			static ModuleRegistry *registry;			///< the registry 
			static std::string registryFile;			///< filename to the registry file

			static xmlNode *registryRoot;				///< root of the XML document
			static xmlDoc *registryDoc;					///< the xml document
			static xmlNode *registryListOfModules;		///< the list of modules node
			
			
		};
	}
}

#endif

