//==============================================================================
// PredCoder.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "PredCoder.h"


//==============================================================================

PredCoder::PredCoder ( int _quant )
	: quant(_quant)
{
	if (quant < 1)
		quant = 1;
}

PredCoder::~PredCoder (  )
{  }


//==============================================================================

void PredCoder::encode ( const int16_t frame[2], int32_t diff[2] )
{
	int16_t pred[2];
	int16_t quantf[2];

	p.predict(pred);

	diff[0] = (pred[0] - frame[0]) / quant;
	diff[1] = (pred[1] - frame[1]) / quant;

	quantf[0] = (pred[0] - (diff[0] * quant));
	quantf[1] = (pred[1] - (diff[1] * quant));

	p.update(quantf);
}

void PredCoder::decode ( const int32_t diff[2], int16_t frame[2] )
{
	int16_t pred[2];

	p.predict(pred);

	frame[0] = (pred[0] - (diff[0] * quant));
	frame[1] = (pred[1] - (diff[1] * quant));

	p.update(frame);
}


// EOF =========================================================================
