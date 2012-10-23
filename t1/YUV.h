#ifndef _YUV_H_
#define _YUV_H_

#include <cv.h>
#include <stdio.h>
#include "YuvBlock.h"

/* 
 *@class YUV to manipulate YUV sequences
 */
class YUV {
public:
	/** Constructor not associated to a file */
	YUV(uint _nRows, uint _nCols, uint _fps, uint _type);

	/** Constructor for writing to file */
	YUV(char* filename, uint _nRows, uint _nCols, uint _fps, uint _type);

	/** Constructor for reading from file
	 \param filename The string containing the name of the video file. */
	YUV(char *filename);

	/** Destructor */
	~YUV();

public:
	/** Reads a new frame from the file.
	 \returns -1 if the read failed. */
	int readFrame();

	/** Write the current frame to the file */
	int appendFrame();
	
	//~ /** Set the current frame, assuming the already specified dimensions and framerate */
	//~ void setFrame(unsigned char* buffer);

public:
	/** Set a specific frame rate */
	void setFps(unsigned int fps);

	/** Displays a frame in the screen. Internally uses OpenCV.*/
	void displayFrame();

	/** Rewind to the first frame */
	void rewind();

public:
	void convertToBW();
	int convertToBW(YUV& output);
	void invertColors();
	int invertColors(YUV& output);
	void changeLuminance(double factor);
	int changeLuminance(double factor, YUV& output);
	int subSampling422(YUV& output);
	int subSampling420(YUV& output);
	YuvBlock* getBlock(int nRows, int nCols, int x, int y);


public:
	friend class YuvResize;

public:
	/** Initialization function (no delegating constructors, boooo) **/
	void init();

	/** Utility to read and parse file header */
	int readFileHeader(char* filename);

	/** Utility to write file header */
	int writeFileHeader(char* filename);

	/** Convert from YUV422 or YUV420 to YUV444 */
	void YUVtoYUV444();

	/** Convert YUV444 to RGB */
	void YUVtoRGB();

	/** Convert a pixel from YUV to RGB */
	void inline YUVtoRGB(int y, int u, int v, int &r, int &g, int &b);

public:
	/** Resolution of the video. */
	uint nRows, nCols;
	/** Resolution of the UV component. */
	uint uvCols, uvRows;
	/** Frame rate of the video. */
	uint fps;
	/** Type of the yuv video (444, 422, 420). */
	uint type;

	/** Buffer size (Y + U + V) (for the original YUV format). */
	uint bufferSize;

	/** Buffer to store the frame data in original format. */
	unsigned char *bufferRaw;
	/** Pointer to the y component (Original YUV format). */
	unsigned char *yBufferRaw;
	/** Pointer to the u component (Original YUV format). */
	unsigned char *uBufferRaw;
	/** Pointer to the v component (Original YUV format). */
	unsigned char *vBufferRaw;

	/** Buffer to store U and V planes in YUV444 format. */
	unsigned char *buffer;
	/** Pointer to the y component (YUV444 format). */
	unsigned char *yBuffer;
	/** Pointer to the u component (YUV444 format). */
	unsigned char *uBuffer;
	/** Pointer to the v component (YUV444 format). */
	unsigned char *vBuffer;

	/** Signals if the current frame has been converted to 444*/
	unsigned int converted;

	/** File pointer to the video file. */
	FILE *fp;
	/** Video start position (for use in rewind) */
	fpos_t videoStart;

	/** Image displayed */
	IplImage *img;
};

#endif
