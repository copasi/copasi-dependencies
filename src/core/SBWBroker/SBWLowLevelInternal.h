#ifndef SBW_LOWLOEVEL_INTERNAL
#define SBW_LOWLOEVEL_INTERNAL

#include "SBW/SBWLowLevel.h"
namespace SystemsBiologyWorkbench
{
	namespace Broker
	{
		class SBWLowLevelInternal : public SBWLowLevel
		{
		public:
			virtual ~SBWLowLevelInternal() {}
			virtual int getModuleId() = 0;
			virtual void addListener(SBWListener listener) = 0;
			virtual void removeListener(SBWListener listener) = 0;
			virtual void receive(DataBlockReader reader) = 0;
		};
	}
}
#endif
