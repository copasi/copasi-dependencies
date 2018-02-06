#include "SBW.h"
#include "SBWBrokerRPC.h"

#include "SBW/SBWCommunicationException.h"
#include "SBW/SBWApplicationException.h"

using namespace SystemsBiologyWorkbench::Broker;
using namespace SystemsBiologyWorkbench;

SBWRPC *Broker::SBW::getInternalAPI()
{
	if (rpc == NULL)
	{
		rpc = new SBWBrokerRPC(); 
	}
	return rpc;
}
void Broker::SBW::setInternalAPI(SBWBrokerRPC *r)
{
	rpc = r;
}

std::string Broker::SBW::normalizeCategoryName(const std::string& category)
{
	if (StringUtil::empty(category))
		return "/";

	//category = category.trim();
	if (category == "")
		return "/";
	else
	{
		try
		{

			std::string newCategoryStr = "";

			char *string = strdup(category.c_str());
			char seps[]   = "/";
			char *token;

			token = strtok( string, seps );
			while( token != NULL )
			{
				/* While there are tokens in "string" */
				std::string piece = std::string(token);
				newCategoryStr = newCategoryStr + "/" + piece;
				/* Get next token: */
				token = strtok( NULL, seps );
			}
			
			//currPos = category.length();

			//newCategoryStr =
			//	newCategoryStr + "/"
			//	+ category.substr(lastPos, currPos);
      free(string);
			return newCategoryStr;
		}
		catch (...)
		{
			return category;
		}
	}
}
std::vector<std::string> Broker::SBW::getServiceNamesFromModule(int moduleId)
{
	std::vector<std::string> oTemp;
	std::string sTemp;
	TRACE("Querying module " << moduleId << " about its services");

	DataBlockWriter args;

	try
	{
		DataBlockReader result = rpc->call(moduleId, SYSTEM_SERVICE,
			GET_SERVICES_METHOD, args);

		// Normalize the strings returned by trimming possible whitespace.

		result >> oTemp;
	}
	catch (SBWException *e)
	{
		std::string msg = "Unable to get services list from module " + Properties::toString(moduleId);
		TRACE(msg);
		throw new SBWCommunicationException(e->getMessage(),
			e->getDetailedMessage());
	}
	return oTemp;
}

SBWBrokerRPC *Broker::SBW::rpc = NULL;
