//==============================================================================
// Encode-llpred.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "SFReader.h"
#include "PredCoder.h"
#include "BitStream.h"
#include "Golomb.h"
#include <string>
#include <cstdlib>


//==============================================================================

#define GOLOMB_ENC_M 1024


//==============================================================================

static void usage ( int argc, char** argv );


//==============================================================================

int main ( int argc, char** argv )
{
	uint golombM;

	if (argc != 3 && argc != 4) {
		usage(argc, argv);
		return -1;
	}

	if (argc == 4) {
		golombM = std::atoi(argv[3]);
	} else {
		golombM = GOLOMB_ENC_M;
	}

	std::string finname(argv[1]);
	std::string foutname(argv[2]);

	SFReader sfr(finname);
	BitStream bs(foutname, BitStream::WRITE);

	PredCoder pc;

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
	bs.writeBits((uint32_t)golombM,             32);

	if (sfr.open()) {
		printf("Unable to open file %s for reading!\n", argv[1]);
		return -1;
	}

	for (uint32_t i = 0; i < sfr.getNFrames(); i++) {
		int16_t sample[2];
		int32_t diff[2];

		sfr.nextFrame(sample);
		pc.encode(sample, diff);

		Golomb::encode(golombM, diff[0], bs);
		Golomb::encode(golombM, diff[1], bs);
	}

	sfr.close();
	bs.close();
}


//==============================================================================

static void usage ( int argc, char** argv )
{
	printf("USAGE: %s <input.wav> <output.ll> [GolombM = %d]\n",
		argv[0], GOLOMB_ENC_M);
}


// EOF =========================================================================
