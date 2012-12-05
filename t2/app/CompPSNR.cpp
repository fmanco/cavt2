#include <cstdio>
#include "YuvReader.h"
#include "YuvAnalyse.h"


int main( int argc, char** argv ){

	if (argc != 3) {
		printf("USAGE: %s <input a> <input b>\n", argv[0]);
		return -1;
	}

	float psnrY;
	float psnrU;
	float psnrV;

	float sumPSNR_Y = 0.0f;
	float sumPSNR_U = 0.0f;
	float sumPSNR_V = 0.0f;

	uint aRows, aCols, aType;
	uint bRows, bCols, bType;
	uint nFrames = 0;

	char* fileA = argv[1];
	char* fileB = argv[2];

	YuvReader readerA(fileA);
	YuvReader readerB(fileB);
	
	if (readerA.open()!=0){
		printf("Error opening %s\n", fileA);
		return -1;
	}

	if (readerB.open()!=0){
		printf("Error opening %s\n", fileB);
		return -1;
	}

	if (readerA.readHeader()!=0){
		printf("Error reading header for %s\n", fileA);
		return -1;
	}

	if (readerB.readHeader()!=0){
		printf("Error reading header for %s\n", fileB);
		return -1;
	}

	
	aRows = readerA.getNRows();
	aCols = readerA.getNCols();
	aType = readerA.getType();

	bRows = readerB.getNRows();
	bCols = readerB.getNCols();
	bType = readerB.getType();

	YuvFrame frameA(aType, aRows, aCols);
	YuvFrame frameB(bType, bRows, bCols);

	while (readerA.readFrame(frameA) == 0 && readerB.readFrame(frameB) == 0){
		psnrY = psnrV = psnrU = 0.0f;

		YuvAnalyse::psnr(frameA, frameB, &psnrY, &psnrU, &psnrV);

		sumPSNR_Y += psnrY; 
		sumPSNR_U += psnrU;
		sumPSNR_V += psnrV;

		nFrames++;
	}

	readerA.close();
	readerB.close();

	printf("Y: %f dB U: %f dB V: %f dB\n", sumPSNR_Y/(float)nFrames, sumPSNR_U/(float)nFrames, sumPSNR_V/(float)nFrames);

	return 0;
}