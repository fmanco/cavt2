//==============================================================================
// <source>.cpp
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


//==============================================================================

int main ( int argc, char** argv )
{
	if (argc != 2) {
		printf("USAGE: %s <input>", argv[0]);
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
	printf("Coiso: %d\n", bs_in.open());

	HybCoder decoder(10, 2, 6, bs_in);

	YuvDisplay disp((char*)"Hybrid Encoder", reader.getFps(), reader.getNRows(), reader.getNCols());

	reader.close();
	disp.start();

	while(decoder.decode(frame) == 0) {
		disp.displayFrame(frame);
	}

	bs_in.close();

	return 0;
}


// EOF =========================================================================
