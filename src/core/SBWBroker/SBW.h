#ifndef SBW_SBW
#define SBW_SBW

#include "SBW/SBWOSSocket.h"
#include "SBW/SBWRPC.h"

#include <string>
#include <vector>

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{
		class SBWBrokerRPC;
		/**
		 * \ingroup C++Broker
		 *
		 * The SBW class contains some help functions. 
		 *
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class SBW
		{
		public:
			/**
			 *  This help method returns a list of services available on a certain module
			 * \param moduleId the module identifier of the module to get the service names from
			 * \return list of service names
			 */
			static std::vector<std::string> getServiceNamesFromModule(int moduleId);
			/**
			 * this function normalizes the category name
			 * \param category the category string to normalize
			 * \return the normalized category string
			 */
      static std::string normalizeCategoryName(const std::string& category);
			/**
			 *
			 * \return internal RPC API
			 */
			static SBWRPC *getInternalAPI();
			/**
			 * sets the internal RPC API
			 * \param *r internal RPC API
			 */
			static void setInternalAPI(SBWBrokerRPC *r);
		private:
			static SBWBrokerRPC *rpc;		///< the internal RPC API
		};
	}
}

#endif
