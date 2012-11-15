//==============================================================================
// YuvDisplay.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __YUVDISPLAY_H__
#define __YUVDISPLAY_H__


//==============================================================================

#include "base.h"
#include "YuvFrame.h"
#include <cv.h>
#include <highgui.h>


//==============================================================================

class YuvDisplay
{
public:
	YuvDisplay ( char* _windowName, uint _fps, uint _nRows, uint _nCols );
	~YuvDisplay (  );

	void start        ( void );
	void displayFrame ( YuvFrame& frame);
	void stop         ( void );

	uint getFps ( void ) const { return fps; }
	void setFps ( uint _fps ) { fps = _fps; }


private:
	void YUVtoRGB ( YuvFrame& frame );
	void YUVtoRGB ( int y, int u, int v, int &r, int &g, int &b );

private:
	char* windowName;

	uint fps;
	uint nRows;
	uint nCols;

	IplImage* img;
};


//==============================================================================
#endif /* __YUVDISPLAY_H__ */
