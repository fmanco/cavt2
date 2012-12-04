#include <cstdio>
#include "YuvReader.h"
#include "YuvAnalyse.h"


int main( int argc, char** argv ){

	if (argc != 2) {
		printf("USAGE: %s <input>\n", argv[0]);
		return -1;
	}

	float t_Y, entropy_Y = 0.0f;
	float t_U, entropy_U = 0.0f;
	float t_V, entropy_V = 0.0f;

	uint aRows, aCols, aType;
	uint nFrames = 0;

	char* fileA = argv[1];

	YuvReader readerA(fileA);
	
	if (readerA.open()!=0){
		printf("Error opening %s\n", fileA);
		return -1;
	}

	if (readerA.readHeader()!=0){
		printf("Error reading header for %s\n", fileA);
		return -1;
	}

	aRows = readerA.getNRows();
	aCols = readerA.getNCols();
	aType = readerA.getType();

	YuvFrame frameA(aType, aRows, aCols);

	while (readerA.readFrame(frameA) == 0){

		YuvAnalyse::entropy(frameA, &t_Y, &t_U, &t_V);

		entropy_Y += t_Y;
		entropy_U += t_U;
		entropy_V += t_V;

		nFrames++;
	}

	readerA.close();

	printf("Y: %f, U:%f, V:%f\n", entropy_Y/(float)nFrames, entropy_U/(float)nFrames, entropy_V/(float)nFrames);

	return 0;
}