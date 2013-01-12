//==============================================================================
// Predictor.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __PREDICTOR_H__
#define __PREDICTOR_H__


//==============================================================================

#include "base.h"
#include <stdint.h>


//==============================================================================

class Predictor {
public:
	Predictor  ( int mode = 0 );
	~Predictor (  );

	void predict ( int16_t samples[2] );
	void update  ( const int16_t samples[2] );

private:
	int     mode;
	int16_t buffer[2][2];
};


//==============================================================================
#endif /* __PREDICTOR_H__ */
