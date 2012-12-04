#include <cstdio>
#include <cstdlib>
#include "BitStream.h"
#include "YuvReader.h"
#include "IntraCoder.h"

int main( int argc, char** argv ){

	if (argc != 7) {
		printf("USAGE: %s <input> <output> <prediction mode> <Y quantization> <U quantization> <V quantization>\n", argv[0]);
		return -1;
	}

	uint rows, cols, fps, type;
	uint nFrames = 0;

	char* fileIn = argv[1];
	char* fileOut = argv[2];
	uint mode = atoi(argv[3]);
	uint qY = atoi(argv[4]);
	uint qU = atoi(argv[5]);
	uint qV = atoi(argv[6]);

	YuvReader reader(fileIn);
	BitStream bs = BitStream(fileOut, BitStream::WRITE);
	
	if (reader.open()!=0){
		printf("Error opening %s\n", fileIn);
		return -1;
	}

	if (reader.readHeader()!=0){
		printf("Error reading header for %s\n", fileIn);
		return -1;
	}

	if (bs.open()!=0){
		printf("Error opening %s\n", fileOut);
		return -1;
	}
	
	rows = reader.getNRows();
	cols = reader.getNCols();
	fps = reader.getFps();
	type = reader.getType();

	YuvFrame frame(type, rows, cols);

	IntraCoder::writeHeader(rows, cols, fps, type, mode, qY, qU, qV, bs);

	while (reader.readFrame(frame) == 0){
		IntraCoder::encode(frame, bs, mode, qY, qU, qV);
		nFrames++;
	}

	reader.close();
	bs.close();

	printf("Done! %d frames writen.\n",nFrames);

	return 0;
}