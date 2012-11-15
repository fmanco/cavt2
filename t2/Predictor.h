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

class Predictor{

public:
	static int predict(YuvFrame& frame, int r, int c, int mode /*enum/object*/);
};


//==============================================================================




//==============================================================================
#endif /* __<HEADER>_H__ */
