#ifndef SBW_RECEIVER
#define SBW_RECEIVER

#include "SBW/DataBlockWriter.h"
#include "SBW/DataBlockReader.h"

namespace SystemsBiologyWorkbench
{
	namespace Broker 
	{
		/**
		* The Receiver interface is the interface that must be implemented by
		* the object designated to process messages sent to a module.
		*
		* @author Andrew Finney
		* @author $Author: fbergmann $
		* @version $Revision: 1.1 $
		*/
		class Receiver
		{
		public:
			/**
			 * The reveive message gets called each time a message is accepted by the Communication threads. 
			 * \param fromModuleId module identifier of the module calling 
			 * \param serviceId the service to be called
			 * \param methodId the method to be called
			 * \param *args the arguments to be passed on
			 * \return a list containing the return value(s) of the method called
			 */
			DataBlockWriter *receive(int fromModuleId, int serviceId, 
				int methodId, DataBlockReader *args);
		};
	}
}
#endif
