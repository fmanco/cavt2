//==============================================================================
// resHist.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "SFReader.h"
#include "Histogram.h"
#include "Predictor.h"
#include <string>


//==============================================================================

static void usage ( int argc, char** argv );


//==============================================================================

int main ( int argc, char** argv )
{
	std::string finname;
	std::string csvl;
	std::string csvr;

	switch (argc) {
	case 2:
		finname.assign(argv[1]);
		csvl.assign("left.csv");
		csvr.assign("right.csv");
		break;

	case 4:
		finname.assign(argv[1]);
		csvl.assign(argv[2]);
		csvr.assign(argv[3]);
		break;

	default:
		usage(argc, argv);
		return -1;
	}

	Histogram hl;
	Histogram hr;

	Predictor p;

	SFReader sfr(finname);

	if (sfr.open()) {
		printf("Unable to open file %s for reading!\n", argv[1]);
		return -1;
	}

	for (uint32_t i = 0; i < sfr.getNFrames(); i++) {
		int16_t sample[2];
		int16_t pred[2];

		p.predict(pred);
		sfr.nextFrame(sample);

		hl.inc(pred[0] - sample[0]);
		hr.inc(pred[1] - sample[1]);

		p.update(sample);
	}

	sfr.close();

	hl.write_csv(csvl);
	hr.write_csv(csvr);
}


//==============================================================================

static void usage ( int argc, char** argv )
{
	printf("USAGE: %s <input.wav> [<left.csv> <right.csv>]\n", argv[0]);
}


// EOF =========================================================================
