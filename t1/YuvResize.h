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

	
private:
	int type;
	int factor;
};


#endif
