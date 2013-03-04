#ifndef SBW_BROKER_REMOTE_INSTANCE
#define SBW_BROKER_REMOTE_INSTANCE

#include "Instance.h"

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{
		/**
		 * \ingroup C++Broker
		 *
		 * The RemoteInstance acts as an Inteface for RemoteInstances
		 *
		 *
		 * \date 10-19-2004
		 *
		 * \author fbergman
		 *
		 *
		 */
		class RemoteInstance : public Instance
		{
		public:
			/**
			* Returns the identifier for this instance on the remote broker.
			* <p>
			* @return the unique integer identifier for this instance.
			**/
			virtual int getRemoteId() = 0;
		};
	}
}

#endif

