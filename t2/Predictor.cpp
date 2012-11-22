//==============================================================================
// Predictor.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "Predictor.h"


//==============================================================================

void Predictor::predict(YuvFrame& frame, int r, int c, int mode, int* y, int* u, int* v){

	int aY = 0, aU = 0, aV = 0;
	int bY = 0, bU = 0, bV = 0;
	int cY = 0, cU = 0, cV = 0;

	//what to do?
	if (c==0){
		*y=0;
		*u=0;
		*v=0;
		return;
	}


	aY = frame.getYPixel(r, c-1);
	aU = frame.getUPixel(r, c-1);
	aV = frame.getVPixel(r, c-1);

	// bY = frame.getYPixel(c, r-1);
	// bU = frame.getUPixel(c, r-1);
	// bV = frame.getVPixel(c, r-1);

	// cY = frame.getYPixel(c-1, r-1);
	// cU = frame.getUPixel(c-1, r-1);
	// cV = frame.getVPixel(c-1, r-1);

	switch (mode){
		case 1:
			*y=aY;
			*u=aU;
			*v=aV;
			break;
		case 2:
			*y=bY;
			*u=bU;
			*v=bV;
			break;
		case 3:
			*y=cY;
			*u=cU;
			*v=cV;
			break;

		//TODO: others
	}
}

// EOF =========================================================================
