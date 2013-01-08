//==============================================================================
// Decode-llpred.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "SFWriter.h"
#include "Predictor.h"
#include "BitStream.h"
#include "Golomb.h"
#include <string>


//==============================================================================

static void usage ( int argc, char** argv );


//==============================================================================

int main ( int argc, char** argv )
{
	uint golombM;

	if (argc != 3) {
		usage(argc, argv);
		return -1;
	}

	std::string finname(argv[1]);
	std::string foutname(argv[2]);

	Predictor p;

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

	SFWriter sfw(foutname, frames, samplerate, channels);

	if (sfw.open()) {
		printf("Unable to open file %s for writing!\n", argv[2]);
		return -1;
	}

	while (true) {
		int16_t sample[2];
		int16_t pred[2];
		int enc;

		p.predict(pred);

		if (Golomb::decode(golombM, &enc, bs))
			break;

		sample[0] = pred[0] - ((int16_t)enc);

		if (Golomb::decode(golombM, &enc, bs))
			break;

		sample[1] = pred[1] - ((int16_t)enc);

		sfw.writeFrame(sample);

		p.update(sample);
	}

	bs.close();
	sfw.close();
}


//==============================================================================

static void usage ( int argc, char** argv )
{
	printf("USAGE: %s <input.ll> <output.wav>\n", argv[0]);
}


// EOF =========================================================================
