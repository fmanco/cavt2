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
#include "YuvFrame.h"

class Predictor{

public:
	static void predict(YuvFrame& frame, int r, int c, int mode /*enum/object*/, int* y, int* u, int* v);
};


//==============================================================================




//==============================================================================
#endif /* __<HEADER>_H__ */
