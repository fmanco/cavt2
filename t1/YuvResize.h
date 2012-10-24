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
	void expand(YUV& orig, YUV& resized);
	void reduce(YUV& orig, YUV& resized);
	
	
	/**
	 * 1D interpolation (along an axis)
	 */ 
	int interpolate(int a, int aVal, int b, int bVal, int c);
	
	
	static const uint REDUCE = 0;
	static const uint EXPAND = 1;
	
private:
	int factor; // resize factor
	bool improved; // use improved method or naive method
};


#endif
