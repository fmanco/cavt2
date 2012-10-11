#ifndef _YUV_H_
#define _YUV_H_

#include <cv.h>
#include <stdio.h>

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

public:
	/** Displays a frame in the screen. Internally uses OpenCV.*/
	void displayFrame();

private:
	/** Initialization function (no delegating constructors, boooo) **/
	void init();

	/** Utility to read and parse file header */
	int readFileHeader(char* filename);

	/** Utility to write file header */
	int writeFileHeader(char* filename);

	void YUVtoRGB(unsigned char *yuvBuffer);

	/** Convert from YUV422 or YUV420 to YUV444 */
	void YUVtoYUV444(unsigned char *buffer);

	/** Convert a pixel from YUV to RGB */
	void inline YUVtoRGB(int y, int u, int v, int &r, int &g, int &b);

private:
	/** Resolution of the video. */
	uint nRows, nCols;
	/** Resolution of the UV component. */
	uint uvCols, uvRows;
	/** Frame rate of the video. */
	uint fps;
	/** Type of the yuv video (444, 422, 420). */
	uint type;

	/** Buffer size (Y + U + V). */
	uint bufferSize;
	/** Buffer to store the frame data. */
	unsigned char *buffer;
	/** Pointer to the y component. */
	unsigned char *yBuffer;
	/** Pointer to the u component. */
	unsigned char *uBuffer;
	/** Pointer to the v component. */
	unsigned char *vBuffer;

	/** File pointer to the video file. */
	FILE *fp;

	/** Image displayed */
	IplImage *img;
};

#endif
