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
#include "YuvWriter.h"
#include "HybCoder.h"
#include "BitStream.h"
#include <cstdio>

//==============================================================================

int main ( int argc, char** argv )
{
	if (argc != 2) {
		printf("USAGE: %s <input>\n", argv[0]);
		return -1;
	}

	int nFrame = 0;

{
	YuvReader reader(argv[1]);
	reader.open();
	reader.readHeader();

	YuvFrame frame(reader.getNRows(), reader.getNCols());

	// ==========
	BitStream bs_out((char*)"file", BitStream::WRITE);
	bs_out.open();

	HybEncoder encoder(bs_out);
	encoder.init(reader.getNRows(), reader.getNCols(),
	             reader.getType(), reader.getFps(),
		         10, 2, 6);

	nFrame = 0;
	while (reader.readFrame(frame) == 0) {
		printf("frame #%d\n", nFrame++);

		encoder.encode(frame);
	}

	bs_out.close();
	reader.close();
}

{
	BitStream bs_in((char*)"file", BitStream::READ);
	bs_in.open();

	HybDecoder decoder(bs_in);
	decoder.init();

	YuvFrame frame(decoder.getNRows(), decoder.getNCols());

	YuvWriter writer((char*)"output.yuv", decoder.getNRows(), decoder.getNCols(), decoder.getType(), decoder.getFps());
	writer.open();
	writer.writeHeader();

	YuvDisplay disp((char*)"Hybrid Encoder", decoder.getFps(), decoder.getNRows(), decoder.getNCols());
	disp.start();

	while(decoder.decode(frame) == 0) {
		disp.displayFrame(frame);
		writer.writeFrame(frame);
	}

	bs_in.close();
	writer.close();
	disp.stop();
}

	return 0;
}


// EOF =========================================================================
