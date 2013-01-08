//==============================================================================
// Encode-ulaw.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "BitStream.h"
#include "MuLawCoder.h"
#include "SFReader.h"
#include <string>


//==============================================================================

static void usage ( int argc, char** argv );


//==============================================================================

int main ( int argc, char** argv )
{
	if (argc != 3) {
		usage(argc, argv);
		return -1;
	}

	std::string finname(argv[1]);
	std::string foutname(argv[2]);

	SFReader sfr(finname);
	BitStream bs(foutname, BitStream::WRITE);

	if (sfr.open()) {
		printf("Unable to open file %s for reading!\n", argv[1]);
		return -1;
	}

	if (bs.open()) {
		printf("Unable to open file %s for writing!\n", argv[2]);
		return -1;
	}

	bs.writeBits((uint32_t)sfr.getNFrames(),    32);
	bs.writeBits((uint32_t)sfr.getSamplerate(), 32);
	bs.writeBits((uint32_t)sfr.getChannels(),   32);

	for (uint32_t i = 0; i < sfr.getNFrames(); i++) {
		int16_t sample[2];
		uint8_t code;

		sfr.nextFrame(sample);

		code = MuLawCoder::encode(sample[0]);
		bs.writeBits((uint32_t)code, 8);

		code = MuLawCoder::encode(sample[1]);
		bs.writeBits((uint32_t)code, 8);
	}

	sfr.close();
	bs.close();
}


//==============================================================================

static void usage ( int argc, char** argv )
{
	printf("USAGE: %s <input.wav> <output.ulaw>\n", argv[0]);
}


// EOF =========================================================================
