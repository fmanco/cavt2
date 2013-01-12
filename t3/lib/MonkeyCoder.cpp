//==============================================================================
// MonkeyCoder.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "MonkeyCoder.h"


//==============================================================================

MonkeyCoder::MonkeyCoder ( int _quant )
	: quant(_quant)
{
	if (quant < 1)
		quant = 1;

	Predictor p(1);
}

MonkeyCoder::~MonkeyCoder (  )
{  }


//==============================================================================

void MonkeyCoder::encode ( const int16_t frame[2], int32_t diff[2] )
{
	int16_t pred[2];
	int16_t quantf[2];
	int16_t x, y;

	x = (frame[0] + frame[1])/2; // x = (L+R)/2
	y =  frame[0] - frame[1]; // y = L-R

	p.predict(pred);

	diff[0] = (pred[0] - x) / quant;
	diff[1] = (pred[1] - y) / quant;

	quantf[0] = (pred[0] - (diff[0] * quant));
	quantf[1] = (pred[1] - (diff[1] * quant));

	p.update(quantf);
}

void MonkeyCoder::decode ( const int32_t diff[2], int16_t frame[2] )
{
	int16_t pred[2];
	int16_t x, y;

	p.predict(pred);

	x = (pred[0] - (diff[0] * quant));
	y = (pred[1] - (diff[1] * quant));

	frame[0] = x+(y/2); // L = x+y/2
	frame[1] = frame[0] - y; // R = L-y

	p.update(frame);
}

// EOF =========================================================================
