//==============================================================================
// YuvDisplay.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "YuvDisplay.h"
#include "YuvFrame.h"


//==============================================================================

YuvDisplay::YuvDisplay ( char* _windowName, uint _fps, uint _nRows, uint _nCols )
	: windowName(_windowName), fps(_fps), nRows(_nRows), nCols(_nCols)
{
	img = cvCreateImage(cvSize(nCols, nRows), IPL_DEPTH_8U, 3);
}

YuvDisplay::~YuvDisplay (  )
{
	cvReleaseImage(&img);
}


//==============================================================================

void YuvDisplay::start ( void )
{
	cvNamedWindow(windowName, CV_WINDOW_AUTOSIZE);
}

void YuvDisplay::displayFrame ( YuvFrame& frame)
{
	YUVtoRGB(frame);

	cvShowImage(windowName, img);

	/* wait according to the frame rate */
	cvWaitKey(1.0 / fps * 1000);
}

void YuvDisplay::stop ( void )
{
	cvDestroyWindow(windowName);
}


//==============================================================================

void YuvDisplay::YUVtoRGB ( YuvFrame& frame )
{
	uchar *imgBuffer;         // OpenCV image buffer
	uchar *yBuffer;           // YUV buffer
	uchar *uBuffer;           //
	uchar *vBuffer;           //

	int r, g, b, y, u, v;     // Auxiliary variables

	// Get YUV buffer
	yBuffer = frame.get_read_yBuff_444();
	uBuffer = frame.get_read_uBuff_444();
	vBuffer = frame.get_read_vBuff_444();

	// The video is stored in YUV planar mode but OpenCv uses packed modes
	imgBuffer = (uchar*) img->imageData;

	// YUV444: |yR*yC| yR*yC| yR*yC|
	for (uint i = 0, j = 0; i < frame.getNRows() * frame.getNCols(); i++, j+=3) {

		// Accessing to planar information
		y = yBuffer[i];
		u = uBuffer[i];
		v = vBuffer[i];

		YUVtoRGB(y, u, v, r, g, b);

		// Fill the OpenCV buffer - packed mode: BGRBGR...BGR
		imgBuffer[j    ] = b;
		imgBuffer[j + 1] = g;
		imgBuffer[j + 2] = r;
	}
}

void YuvDisplay::YUVtoRGB ( int y, int u, int v, int &r, int &g, int &b )
{
	// convert to RGB
	b = (int) (1.164 * (y - 16) + 2.018 * (u - 128));
	g = (int) (1.164 * (y - 16) - 0.813 * (u - 128) - 0.391 * (v - 128));
	r = (int) (1.164 * (y - 16) + 1.596 * (v - 128));

	// clipping to [0 ... 255]
	if (r < 0) r = 0;
	if (g < 0) g = 0;
	if (b < 0) b = 0;
	if (r > 255) r = 255;
	if (g > 255) g = 255;
	if (b > 255) b = 255;
}


// EOF =========================================================================
