//==============================================================================
// PredCoder.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "PredCoder.h"


//==============================================================================

PredCoder::PredCoder ( void )
{  }

PredCoder::~PredCoder (  )
{  }


//==============================================================================

void PredCoder::encode ( const int16_t frame[2], int32_t diff[2] )
{
	int16_t pred[2];

	p.predict(pred);

	diff[0] = pred[0] - frame[0];
	diff[1] = pred[1] - frame[1];

	p.update(frame);
}

void PredCoder::decode ( const int32_t diff[2], int16_t frame[2] )
{
	int16_t pred[2];

	p.predict(pred);

	frame[0] = pred[0] - diff[0];
	frame[1] = pred[1] - diff[1];

	p.update(frame);
}


// EOF =========================================================================
