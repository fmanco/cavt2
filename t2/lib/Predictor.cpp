//==============================================================================
// Predictor.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "Predictor.h"
#include "YuvFrame.h"



//==============================================================================

inline static int max(int a, int b) { return (a>b)?a:b;};
inline static int min(int a, int b) { return (a<b)?a:b;};

int Predictor::predictY(YuvFrame& frame, int r, int c, int mode) {

	int v = 0;
	int aV = 0;
	int bV = 0;
	int cV = 0;

	if (mode < 0 || mode > 7)
		mode = 7;

	if (c == 0 && mode != 1){
		return 0;
	}
	
	if (r == 0 && mode != 0) {
		return 0;
	}

	switch (mode){
		case 0:
			v = frame.getYPixel(r, c-1);
			break;
		case 1:
			v = frame.getYPixel(r-1, c);
			break;
		case 2:
			v = frame.getYPixel(r-1, c-1);	
			break;
		case 3:
			aV = frame.getYPixel(r, c-1);
			bV = frame.getYPixel(r-1, c);
			cV = frame.getYPixel(r-1, c-1);	

			v=aV+bV-cV;
			break;
		case 4:
			aV = frame.getYPixel(r, c-1);
			bV = frame.getYPixel(r-1, c);
			cV = frame.getYPixel(r-1, c-1);	

			v=aV+(bV-cV)/2;
			break;
		case 5:

			aV = frame.getYPixel(r, c-1);
			bV = frame.getYPixel(r-1, c);
			cV = frame.getYPixel(r-1, c-1);	

			v=bV+(aV-cV)/2;
			break;
		case 6:
			aV = frame.getYPixel(r, c-1);
			bV = frame.getYPixel(r-1, c);

			v=(aV+bV)/2;
			break;
		case 7:
			//NON-LINEAR
			aV = frame.getYPixel(r, c-1);
			bV = frame.getYPixel(r-1, c);
			cV = frame.getYPixel(r-1, c-1);	

			int max_ = max(aV, bV);
			int min_ = min(aV, bV);
			
			if (cV >= max_)
				v = min_;
			else if (cV <= min_)
				v = max_;	
			else 
				v = aV + bV - cV;

			break;
	}	

	return v;
}

int Predictor::predictU(YuvFrame& frame, int r, int c, int mode) {

	int v = 0;
	int aV = 0;
	int bV = 0;
	int cV = 0;

	if (mode < 0 || mode > 7)
		mode = 7;

	if (c == 0 && mode != 1){
		return 0;
	}
	
	if (r == 0 && mode != 0) {
		return 0;
	}

	switch (mode){
		case 0:
			v = frame.getUPixel(r, c-1);
			break;
		case 1:
			v = frame.getUPixel(r-1, c);
			break;
		case 2:
			v = frame.getUPixel(r-1, c-1);	
			break;
		case 3:
			aV = frame.getUPixel(r, c-1);
			bV = frame.getUPixel(r-1, c);
			cV = frame.getUPixel(r-1, c-1);	

			v=aV+bV-cV;
			break;
		case 4:
			aV = frame.getUPixel(r, c-1);
			bV = frame.getUPixel(r-1, c);
			cV = frame.getUPixel(r-1, c-1);	

			v=aV+(bV-cV)/2;
			break;
		case 5:

			aV = frame.getUPixel(r, c-1);
			bV = frame.getUPixel(r-1, c);
			cV = frame.getUPixel(r-1, c-1);	

			v=bV+(aV-cV)/2;
			break;
		case 6:
			aV = frame.getUPixel(r, c-1);
			bV = frame.getUPixel(r-1, c);

			v=(aV+bV)/2;
			break;
		case 7:
			//NON-LINEAR
			aV = frame.getUPixel(r, c-1);
			bV = frame.getUPixel(r-1, c);
			cV = frame.getUPixel(r-1, c-1);	

			int max_ = max(aV, bV);
			int min_ = min(aV, bV);
			
			if (cV >= max_)
				v = min_;
			else if (cV <= min_)
				v = max_;
			else 
				v = aV + bV - cV;

			break;
	}	
	return v;	
}

int Predictor::predictV(YuvFrame& frame, int r, int c, int mode) {

	int v=0;
	int aV = 0;
	int bV = 0;
	int cV = 0;

	if (mode < 0 || mode > 7)
		mode = 7;

	if (c == 0 && mode != 1){
		return 0;
	}
	
	if (r == 0 && mode != 0) {
		return 0;
	}

	switch (mode){
		case 0:
			v = frame.getVPixel(r, c-1);
			break;
		case 1:
			v = frame.getVPixel(r-1, c);
			break;
		case 2:
			v = frame.getVPixel(r-1, c-1);	
			break;
		case 3:
			aV = frame.getVPixel(r, c-1);
			bV = frame.getVPixel(r-1, c);
			cV = frame.getVPixel(r-1, c-1);	

			v=aV+bV-cV;
			break;
		case 4:
			aV = frame.getVPixel(r, c-1);
			bV = frame.getVPixel(r-1, c);
			cV = frame.getVPixel(r-1, c-1);	

			v=aV+(bV-cV)/2;
			break;
		case 5:
			aV = frame.getVPixel(r, c-1);
			bV = frame.getVPixel(r-1, c);
			cV = frame.getVPixel(r-1, c-1);	

			v=bV+(aV-cV)/2;
			break;
		case 6:
			aV = frame.getVPixel(r, c-1);
			bV = frame.getVPixel(r-1, c);

			v=(aV+bV)/2;
			break;
		case 7:
			//NON-LINEAR
			aV = frame.getVPixel(r, c-1);
			bV = frame.getVPixel(r-1, c);
			cV = frame.getVPixel(r-1, c-1);	

			int max_ = max(aV, bV);
			int min_ = min(aV, bV);
			
			if (cV >= max_)
				v = min_;
			else if (cV <= min_)
				v = max_;
			else 
				v = aV + bV - cV;

			break;
	}	
	return v;	
}

// EOF =========================================================================
