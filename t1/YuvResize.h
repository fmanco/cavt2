#ifndef _YUVRESIZE_H_
#define _YUVRESIZE_H_

//~ #include <cv.h>
#include <stdio.h>
#include "YUV.h"


class YuvResize {
	
public:
	
	YuvResize(uint _type, uint _factor);

	/** Destructor */
	~YuvResize();
	
public:
	void apply(YUV& orig, YUV& resized);
	YUV prepareCopy(YUV& orig);
	static const uint REDUCE = 0;
	static const uint EXPAND = 1;

private:
	void expand(YUV& orig, YUV& resized);
	void reduce(YUV& orig, YUV& resized);

	
private:
	int type;
	int factor;
};


#endif
