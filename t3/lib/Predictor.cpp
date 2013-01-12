//==============================================================================
// Predictor.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "Predictor.h"


//==============================================================================

Predictor::Predictor ( int _mode )
	: mode(_mode)
{
	buffer[0][0] = buffer[0][1] = 0;
	buffer[1][0] = buffer[1][1] = 0;
}

Predictor::~Predictor (  )
{  }


//==============================================================================

void Predictor::predict ( int16_t samples[2] )
{
	switch (mode) {
	case 0:
		samples[0] = buffer[0][0];
		samples[1] = buffer[1][0];
		break;

	case 1:
		samples[0] = (2 * buffer[0][0]) - buffer[0][1];
		samples[1] = (2 * buffer[1][0]) - buffer[1][1];
		break;
	}
}

void Predictor::update ( const int16_t samples[2] )
{
	switch (mode) {
	case 0:
		buffer[0][0] = samples[0];
		buffer[1][0] = samples[1];
		break;

	case 1:
		buffer[0][1] = buffer[0][0];
		buffer[0][0] = samples[0];
		buffer[1][1] = buffer[1][0];
		buffer[1][0] = samples[1];
		break;
	}
}


// EOF =========================================================================
