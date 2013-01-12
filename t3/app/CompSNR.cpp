#include <cstdio>
#include <string>
#include <math.h>
#include "SFReader.h"


int main( int argc, char** argv ){

	if (argc != 3) {
		printf("USAGE: %s <input a> <input b>\n", argv[0]);
		return -1;
	}

	int i;
	float snr;
	
	short sampleA[2];
	short sampleB[2];

	std::string fileA(argv[1]);
	std::string fileB(argv[2]);

	SFReader sfReaderA(fileA);
	SFReader sfReaderB(fileB);

	int nFrames = 0;
	float sign;
	float err;
	
	if (sfReaderA.open()!=0){
		printf("Error opening %s\n", fileA.c_str());
		return -1;
	}

	if (sfReaderB.open()!=0){
		printf("Error opening %s\n", fileB.c_str());
		return -1;
	}

	if (sfReaderA.getNFrames() != sfReaderB.getNFrames()){
		printf("Error, files don't have the same length");
		return -1;
	}

	long sqrSignalSum = 0;
	long sqrErrSum = 0;

	nFrames = sfReaderA.getNFrames();

	for (i = 0; i < nFrames; i++) {

		sfReaderA.nextFrame(sampleA);
		sfReaderB.nextFrame(sampleB);

		sqrSignalSum += sampleA[0]*sampleA[0];
		sqrErrSum += (sampleA[0]-sampleB[0])*(sampleA[0]-sampleB[0]);

		sqrSignalSum += sampleA[1]*sampleA[1];
		sqrErrSum += (sampleA[1]-sampleB[1])*(sampleA[1]-sampleB[1]);
	}

	sign = ((float) sqrSignalSum)/((float)nFrames);
	err = ((float) sqrErrSum)/((float)nFrames);

	snr = 20 * log10(sign/err);

	sfReaderA.close();
	sfReaderB.close();

	printf("SNR: %f dB\n", snr);

	return 0;
}