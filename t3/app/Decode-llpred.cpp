//==============================================================================
// Decode-llpred.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "SFWriter.h"
#include "PredCoder.h"
#include "BitStream.h"
#include "Golomb.h"
#include <string>


//==============================================================================

static void usage ( int argc, char** argv );


//==============================================================================

int main ( int argc, char** argv )
{
	int golombM;
	int quant;

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
	bs.readBits(32, (uint32_t*)&golombM);
	bs.readBits(32, (uint32_t*)&quant);

	SFWriter sfw(foutname, frames, samplerate, channels);
	PredCoder pc(quant);

	if (sfw.open()) {
		printf("Unable to open file %s for writing!\n", argv[2]);
		return -1;
	}

	while (true) {
		int16_t sample[2];
		int32_t diff[2];

		if (Golomb::decode(golombM, &diff[0], bs))
			break;

		if (Golomb::decode(golombM, &diff[1], bs))
			break;

		pc.decode(diff, sample);
		sfw.writeFrame(sample);
	}

	bs.close();
	sfw.close();

	printf("Done! Decoded %u frames.\n", frames);
}


//==============================================================================

static void usage ( int argc, char** argv )
{
	printf("USAGE: %s <input.ll> <output.wav>\n", argv[0]);
}


// EOF =========================================================================
