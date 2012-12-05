//==============================================================================
// TestHyb.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "YuvFrame.h"
#include "YuvWriter.h"
#include "HybCoder.h"
#include "BitStream.h"
#include <cstdio>

//==============================================================================

int main ( int argc, char** argv )
{
	if (argc != 3) {
		printf("USAGE: %s <input> <output>\n", argv[0]);
		return -1;
	}

	int nFrames = 0;

	char* fileIn = argv[1];
	char* fileOut = argv[2];

	BitStream bs(fileIn, BitStream::READ);
	bs.open();

	HybDecoder decoder(bs);
	decoder.init();

	YuvFrame frame(decoder.getType(), decoder.getNRows(), decoder.getNCols());

	YuvWriter writer(fileOut, decoder.getNRows(), decoder.getNCols(), decoder.getType(), decoder.getFps());
	writer.open();
	writer.writeHeader();

	while(decoder.decode(frame) == 0) {
		writer.writeFrame(frame);
		nFrames++;
	}

	bs.close();
	writer.close();

	printf("Done! %d frames writen.\n",nFrames);

	return 0;
}


// EOF =========================================================================
