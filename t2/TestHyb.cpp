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

	YuvReader reader(argv[1]);
	reader.open();
	reader.readHeader();

	BitStream bs_out((char*)"file", BitStream::WRITE);
	bs_out.open();

	HybCoder encoder(10, 2, 6, bs_out);
	YuvFrame frame(reader.getNRows(), reader.getNCols());

	nFrame = 0;
	while (reader.readFrame(frame) == 0) {
		printf("frame #%d\n", nFrame++);

		encoder.encode(frame);
	}
	bs_out.close();


	BitStream bs_in((char*)"file", BitStream::READ);
	bs_in.open();

	HybCoder decoder(10, 2, 6, bs_in);

	YuvDisplay disp((char*)"Hybrid Encoder", reader.getFps(), reader.getNRows(), reader.getNCols());
	YuvWriter writer((char*)"output.yuv", reader.getNRows(), reader.getNCols(), reader.getType(), reader.getFps());

	reader.close();
	disp.start();

	writer.open();
	writer.writeHeader();

	while(decoder.decode(frame) == 0) {
		disp.displayFrame(frame);
		writer.writeFrame(frame);
	}

	bs_in.close();
	writer.close();
	disp.stop();

	return 0;
}


// EOF =========================================================================
