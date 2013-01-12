//==============================================================================
// MonkeyCoder.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "MonkeyCoder.h"
// #include "stdio.h"
#include <cstdio>


//==============================================================================

MonkeyCoder::MonkeyCoder ( int _quant )
	: p(1), quant(_quant)
{
	if (quant < 1)
		quant = 1;
}

MonkeyCoder::~MonkeyCoder (  )
{  }


//==============================================================================

void MonkeyCoder::encode ( const int16_t frame[2], int32_t diff[2] )
{
	int16_t pred[2];
	int16_t quantf[2];
	int x, y;

	x = (frame[0] + frame[1])/2; // x = (L+R)/2
	y =  frame[0] - frame[1]; // y = L-R

	p.predict(pred);

	printf("px = %hd py = %hd | x = %d y = %d | l = %hd r = %hd\n", pred[0], pred[1], x, y, frame[0], frame[1]);

	diff[0] = (pred[0] - x) / quant;
	diff[1] = (pred[1] - y) / quant;

	quantf[0] = (pred[0] - (diff[0] * quant));
	quantf[1] = (pred[1] - (diff[1] * quant));

	p.update(quantf);
}

void MonkeyCoder::decode ( const int32_t diff[2], int16_t frame[2] )
{
	int16_t pred[2];
	int16_t xy[2];
	int x, y;

	p.predict(pred);

	x = (pred[0] - (diff[0] * quant));
	y = (pred[1] - (diff[1] * quant));

	// L = (2x + y) / 2
	// R = (2x - y) / 2

	// When everything you have is a hammer... just move on

	float lf = ((2*x) + y) / 2.0;
	float rf = ((2*x) - y) / 2.0;

	int lint = ((2*x) + y) / 2;
	int ldec = (((2*x) + y) % 2) ? 1 : 0;

	int rint = ((2*x) - y) / 2;
	int rdec = (((2*x) - y) % 2) ? 1 : 0;

	if (lf < 0 && rf < 0) {
		ldec = -ldec;
		rdec = -rdec;
	} else if (lf < 0) {
		if (x < 1) {
			ldec = -ldec;
			rdec = 0;
		} else {
			ldec = 0;
		}
	} else if (rf < 0) {
		if (x < 1) {
			ldec = 0;
			rdec = -rdec;
		} else {
			rdec = 0;
		}
	}

	frame[0] = ((uint16_t)lint) + ((uint16_t)ldec);
	frame[1] = ((uint16_t)rint) + ((uint16_t)rdec);

	printf("px = %hd py = %hd | x = %d y = %d | l = %hd r = %hd\n", pred[0], pred[1], x, y, frame[0], frame[1]);
	fprintf(stderr, "lint = %d ldec = %d lint16 = %hd rint = %d rdec = %d rint16 = %hd\n", lint, ldec, (uint16_t)lint, rint, rdec, (uint16_t)rint);

	xy[0] = x;
	xy[1] = y;
	p.update(xy);
}

// EOF =========================================================================
