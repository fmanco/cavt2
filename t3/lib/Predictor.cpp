//==============================================================================
// Predictor.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "Predictor.h"


//==============================================================================


Predictor::Predictor(int _mode) : mode(_mode) {
	 buffer = new int16_t[2];
	 buffer[0] = buffer[1] = 0;
};

Predictor::~Predictor() {
	delete[] buffer;
};

void Predictor::predict(int16_t* samples){
	samples[0] = buffer[0];
	samples[1] = buffer[1];
}

void Predictor::update(int16_t* samples){
	buffer[0] = samples[0];
	buffer[1] = samples[1];
}

// EOF =========================================================================
