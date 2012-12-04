#include <cstdio>
#include <cstdlib>
#include "BitStream.h"
#include "YuvReader.h"
#include "DCTCoder.h"

int main( int argc, char** argv ){

	if (argc != 4) {
		printf("USAGE: %s <input> <output> <quantization>\n\tThe quantization parameter is expressed as floor(q*100) where q can be a real number, e.g. for a factor of 2.5 the value is 250.\n\tA value of 0 has the least losses (only rounding errors), altough it can produce bigger files.\n", argv[0]);
		return -1;
	}

	uint rows, cols, fps, type;
	uint nFrames = 0;

	char* fileIn = argv[1];
	char* fileOut = argv[2];
	uint quant = atoi(argv[3]);

	YuvReader reader(fileIn);
	BitStream bs = BitStream(fileOut, BitStream::WRITE);
	
	if (reader.open()!=0){
		printf("Error opening %s\n", fileIn);
		return -1;
	}

	if (reader.readHeader()!=0){
		printf("Error reading header for %s\n", fileIn);
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

	DCTCoder::writeHeader(rows, cols, fps, type, quant, bs);

	while (reader.readFrame(frame) == 0){
		DCTCoder::encode(frame, bs, quant);
		nFrames++;
	}

	reader.close();
	bs.close();

	printf("Done! %d frames writen.\n",nFrames);

	return 0;
}