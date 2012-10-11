#ifndef _YUV_H_
#define _YUV_H_

#include <cv.h>
#include <stdio.h>

/* 
 *@class YUV to manipulate YUV sequences
 */
class YUV
{
	public:
		/** Default constructor */
		YUV();
		/** Constructor that allows the specification of the video properties */
		YUV(int nR, int nC, int fps, int type);
		/** Constructor that uses the information presented in the file header
			to create the object
			\param filename The string containing the name of the video file. */
		YUV(char *filename);
		/** Destructor */
		~YUV();
		/** Reads a new frame from the file.
			\returns -1 if the read failed. */
		int readFrame();
		/** Displays a frame in the screen. Internally uses OpenCV.*/
		void displayFrame();
	private:
		/** Initialization function (no delegating constructors, boooo) **/
		void init(int nR, int nC, int fps, int type);

		/** Utility to read and parse file header */
		int readFileHeader(char* filename, int* yCols, int* yRows, int* fps,  int* type);

		void convert();

		void YUVtoRGB(int y, int u, int v, int &r, int &b, int &g);

		/** Buffer to store the frame data. */
		unsigned char *buffer;
		/** Pointer to the y component. */
		unsigned char *yBuffer;
		/** Pointer to the u component. */
		unsigned char *uBuffer;
		/** Pointer to the v component. */
		unsigned char *vBuffer;
		/** Resolution of the video. */
		int nRows, nCols;
		/** Frame rate of the video. */
		int fps;
		/** Type of the yuv video (444, 422, 420). */
		int type;
		/** Resolution of the UV component. */
		int uvCols, uvRows;
		/** Buffer size (Y + U + V). */
		int bufferSize;
		/** File pointer to the video file. */
		FILE *fp;
		/** Image displayed */
		IplImage *img;
};
#endif
