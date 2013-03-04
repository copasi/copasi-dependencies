#ifndef SBW_OBJECT_ORIENTED_RECEIVER
#define SBW_OBJECT_ORIENTED_RECEIVER

#include "Broker.h"

#include "SBW/DataBlockWriter.h"
#include "SBW/DataBlockReader.h"
#include "SBW/ServiceImpl.h"
#include "SBW/Receiver.h"

#include "BrokerInterface.h"


#include <string>
#include <vector>

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{
		
		/**
		 * \ingroup C++Broker
		 *
		 * The ObjectOrientedReceiver is the Receiver object for the CBroker. Each time the CBroker receives
		 * a message the receive function will be called to handle it. 
		 *
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class ObjectOrientedReceiver :  public SystemsBiologyWorkbench::Receiver
		{
		public:
			/** 
			 * create a new Receiver object with the given moduleName
			 * \param moduleName the name of the Receiver
			 */
			ObjectOrientedReceiver(std::string moduleName);
			/**
			 * This function is used to register all the Methods of the Broker
			 * \param name the name of the Service
			 * \param displayName the display name
			 * \param category the category
			 * \param *impl pointer to the CBroker implementation
			 * \param help a help string
			 */
			void addService(std::string name, std::string displayName,
				std::string category, CBroker *impl, std::string help);
			/**
			 * The receive function will be called each time a message comes to the broker. 
			 * \param fromModuleId module identifier of the module sending or calling the CBroker
			 * \param serviceId the service identifier of the service to call
			 * \param methodId the method identifier of the method to call
			 * \param args a list containing all arguments for the method to call
			 * \return a list containing the result
			 */
			virtual DataBlockWriter receive(Integer fromModuleId, Integer serviceId,
				Integer methodId, DataBlockReader args);
			/**
			 * this sets the module id to the given value
			 * \param moduleId the value to be set
			 */
			void setModuleId(int moduleId);

			/**
			 * this method returns a collection of all broker methods
			 * \return list of all broker methods
			 */
			static std::vector<std::string> getBrokerMethods();

		private:
			/**
			* Returns an array of strings, one for each service implemented by this
			* receiver.  This is intended to be used for the GET_SERVICES_METHOD
			* and is not a high performance item.
			* <p>
			* @return an array of strings listing the services offered
			**/
			std::vector<std::string> getServices();
			/**
			 *	Returns the ServiceImpl object for the requested service
			 * \param serviceId the id of the service for which to return the object
			 * \return the ServiceImpl object for the requested service
			 */
			ServiceImpl *getService(int serviceId);
			/** Contains ServiceImpl implementing services. **/
			std::vector<ServiceImpl *> services;

			/** Number of services in 'services' array, cached for speedy lookup. **/
			int numServices;

			/** Name of the module that this receiver represents. **/
			std::string moduleName;

			/** Module id of the module that this receiver represents. **/
			int moduleId;

			// broker implemetation
			CBroker  *m_oBroker;

			static std::vector<std::string> m_oMethods; ///< list containing all broker method signatures
			static std::vector<std::string> m_oHelp; ///< list containing all broker method signatures
		};

	}
}
#endif
