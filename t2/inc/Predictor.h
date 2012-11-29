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
	static int predictY(YuvFrame& frame, int r, int c, int mode=1);
	static int predictU(YuvFrame& frame, int r, int c, int mode=1);
	static int predictV(YuvFrame& frame, int r, int c, int mode=1);
};


//==============================================================================




//==============================================================================
#endif /* __<HEADER>_H__ */
