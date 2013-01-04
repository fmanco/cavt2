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
	Predictor(int mode);
	~Predictor();

	void predict( int16_t* samples );
	void update( int16_t* samples );

private:
	int mode;
	int16_t* buffer;
};



//==============================================================================
#endif /* __PREDICTOR_H__ */
