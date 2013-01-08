//==============================================================================
// Decode-ulaw.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "BitStream.h"
#include "MuLawCoder.h"
#include "SFWriter.h"
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

	BitStream bs(finname, BitStream::READ);

	if (bs.open()) {
		printf("Unable to open file %s for reading!\n", argv[1]);
		return -1;
	}

	uint32_t frames;
	uint32_t samplerate;
	uint32_t channels;

	bs.readBits(32, &frames);
	bs.readBits(32, &samplerate);
	bs.readBits(32, &channels);

	SFWriter sfw(foutname, frames, samplerate, channels);

	if (sfw.open()) {
		printf("Unable to open file %s for writing!\n", argv[2]);
		return -1;
	}

	while (true) {
		int16_t sample[2];
		uint32_t code;

		if (bs.readBits(8, &code) != 0)
			break;

		sample[0] = MuLawCoder::decode((uint8_t) (code & 0xFF));

		if (bs.readBits(8, &code) != 0)
			break;

		sample[1] = MuLawCoder::decode((uint8_t) (code & 0xFF));

		sfw.writeFrame(sample);
	}

	bs.close();
	sfw.close();

	printf("Done! Decoded %u frames.\n", frames);
}


//==============================================================================

static void usage ( int argc, char** argv )
{
	printf("USAGE: %s <input.ulaw> <output.wav>\n", argv[0]);
}


// EOF =========================================================================
