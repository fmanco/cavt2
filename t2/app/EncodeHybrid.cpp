//==============================================================================
// TestHyb.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "YuvFrame.h"
#include "YuvDisplay.h"
#include "YuvReader.h"
#include "HybCoder.h"
#include "BitStream.h"
#include <cstdio>

//==============================================================================

int main ( int argc, char** argv )
{
	if (argc != 9) {
		printf("USAGE: %s <input> <output> <block size> <search area> <keyframe interval> <Y quantization> <U quantization> <V quantization>\n", argv[0]);
		return -1;
	}

	uint rows, cols, fps, type;
	uint nFrames = 0;

	char* fileIn = argv[1];
	char* fileOut = argv[2];
	uint blkSize = atoi(argv[3]);
	uint search = atoi(argv[4]);
	uint keyframeT = atoi(argv[5]);
	uint qY = atoi(argv[6]);
	uint qU = atoi(argv[7]);
	uint qV = atoi(argv[8]);

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


	HybEncoder encoder(bs);
	encoder.init(rows, cols, type, fps, blkSize, search, keyframeT, qY, qU, qV);


	while (reader.readFrame(frame) == 0) {
		// printf("frame #%d\n", nFrames++);

		encoder.encode(frame);
		nFrames++;
	}

	bs.close();
	reader.close();

	printf("Done! %d frames writen.\n",nFrames);

	return 0;
}


// EOF =========================================================================
