//==============================================================================
// Encode-llpred.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "SFReader.h"
#include "Predictor.h"
#include "BitStream.h"
#include "Golomb.h"
#include <string>


//==============================================================================

#define GOLOMB_ENC_M 1024


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

	Predictor p;

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

	if (sfr.open()) {
		printf("Unable to open file %s for reading!\n", argv[1]);
		return -1;
	}

	for (uint32_t i = 0; i < sfr.getNFrames(); i++) {
		int16_t sample[2];
		int16_t pred[2];

		p.predict(pred);
		sfr.nextFrame(sample);

		Golomb::encode(GOLOMB_ENC_M, pred[0] - sample[0], bs);
		Golomb::encode(GOLOMB_ENC_M, pred[1] - sample[1], bs);

		p.update(sample);
	}

	sfr.close();
	bs.close();
}


//==============================================================================

static void usage ( int argc, char** argv )
{
	printf("USAGE: %s <input.wav> <output.ll>\n", argv[0]);
}


// EOF =========================================================================
