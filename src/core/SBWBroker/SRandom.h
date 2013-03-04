#ifndef SBW_BROKER_SRANDOM
#define SBW_BROKER_SRANDOM

namespace SystemsBiologyWorkbench
{
	namespace Broker
	{
		class SRandom {
		private:
			long count;
			int timeLength;

		public:
			SRandom();
			static int longSize;
			char *generateData(int length);
			static int intSize;
		};
	}
}
#endif

