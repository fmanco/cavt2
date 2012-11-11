//==============================================================================
// YuvDisplay.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __YUVDISPLAY_H__
#define __YUVDISPLAY_H__


//==============================================================================

#include <base.h>
#include <cv.h>
#include <highgui.h>


//==============================================================================

class YuvDisplay
{
public:
	YuvDisplay  ( char* _windowName, uint _fps );
	~YuvDisplay (  );

	void start        ( void );
	void displayFrame ( const YuvFrame& frame);
	void stop         ( void );

	uint getFps ( void ) const { return fps; }
	void setFps ( uint _fps ) { fps = _fps; }


private:
	void YUVtoRGB ( const YuvFrame& frame );
	void YUVtoRGB ( int y, int u, int v, int &r, int &g, int &b );

private:
	uint fps;

	IplImage* img;
	char* windowName;
};


//==============================================================================
#endif /* __YUVDISPLAY_H__ */
