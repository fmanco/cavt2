#include <cstdio>
#include <cstdlib>
#include "BitStream.h"
#include "YuvWriter.h"
#include "DCTCoder.h"

int main( int argc, char** argv ){

	if (argc != 3) {
		printf("USAGE: %s <input> <output>\n", argv[0]);
		return -1;
	}

	uint rows, cols, fps, type;
	uint nFrames = 0;

	char* fileIn = argv[1];
	char* fileOut = argv[2];
	uint quant;

	BitStream bs = BitStream(fileIn, BitStream::READ);

	if (bs.open()!=0){
		printf("Error opening %s\n", fileIn);
		return -1;
	}

	if (DCTCoder::readHeader(bs, &rows, &cols, &fps, &type, &quant)!=0){
		printf("Error reading header for %s\n", fileIn);
		return -1;
	}

	YuvFrame frame(type, rows, cols);
	
	YuvWriter writer(fileOut, rows, cols, type, fps);

	if (writer.open()!=0){
		printf("Error opening %s\n", fileOut);
		return -1;
	}

	writer.writeHeader();

	while (DCTCoder::decode(bs, frame, quant) == 0){
		writer.writeFrame(frame);
		nFrames++;
	}

	bs.close();
	writer.close();
	
	printf("Done! %d frames writen.\n",nFrames);

	return 0;
}