#ifndef _YUVRESIZE_H_
#define _YUVRESIZE_H_

//~ #include <cv.h>
#include <stdio.h>
#include "YUV.h"


class YuvResize {
	
public:
	
	YuvResize(uint _factor, bool _improved);

	/** Destructor */
	~YuvResize();
	
public:
	YUV prepareCopy(YUV& orig, uint type);
	
	/** Expand an image, the improved method uses bilinear interpolation*/
	void expand(YUV& orig, YUV& resized);
	
	/** Reduce an image, the improved method uses the average of the reduced pixels*/
	void reduce(YUV& orig, YUV& resized);
	
	/** Linear interpolation of 2 pixels in the same axis*/
	//static?
	int interpolate(int a, int aVal, int b, int bVal, int c);
	
	
	static const uint REDUCE = 0;
	static const uint EXPAND = 1;
	
private:
	int factor; // resize factor
	bool improved; // use improved method or naive method
};


#endif
