#include "SRandom.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


using namespace SystemsBiologyWorkbench::Broker;


SRandom::SRandom()
{
	count = 0;
	timeLength = 8;
}
char *SRandom::generateData(int length) {

	char *data = (char*) malloc(length + 1 * sizeof(char));
	srand( (unsigned)time( NULL ) );
	char * result = data;
	double dRandom;
	for( int i = 0; i < length; i++ ) {
		dRandom = ((double) rand() / (double) RAND_MAX) * (0xFF);

		char cTemp = (char) dRandom;
		data[i] = cTemp;
	}

	return result;
}

