#include "YUV.h"
#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>

////////////////////////////////////////////////////////////////////////////////

YUV::YUV(uint _nRows, uint _nCols, uint _fps, uint _type) :
		nRows(_nRows), nCols(_nCols), fps(_fps), type(_type), fp(NULL) {
	init();
}

YUV::YUV(char* filename, uint _nRows, uint _nCols, uint _fps, uint _type) :
		nRows(_nRows), nCols(_nCols), fps(_fps), type(_type) {
	init();

	if (writeFileHeader(filename))
		throw new std::runtime_error("Unable to write file header!");
}

YUV::YUV(char *filename) {
	if (readFileHeader(filename))
		throw new std::runtime_error("Unable to read file header!");

	init();
}

YUV::~YUV() {
	if (fp != NULL)
		fclose(fp);

	// TODO: Check if image is opened
	cvDestroyWindow("YUV");
	cvReleaseImage(&img);
}

void YUV::init() {
	switch (type) {
	case 444:
		uvCols = nCols;
		uvRows = nRows;
		break;
	case 422:
		uvCols = nCols / 2;
		uvRows = nRows;
		break;
	case 420:
		uvCols = nCols / 2;
		uvRows = nRows / 2;
		break;
	}

	bufferSize = (nRows * nCols) + (2 * uvRows * uvCols);
	buffer = new unsigned char[bufferSize];

	yBuffer = buffer;
	uBuffer = buffer + (nRows * nCols);
	vBuffer = buffer + (nRows * nCols) + (uvRows * uvCols);

	// FIXME: coiso
	/* data structure for the OpenCv image */
	img = cvCreateImage(cvSize(nCols, nRows), IPL_DEPTH_8U, 3);

	/* create a window */
	cvNamedWindow("YUV", CV_WINDOW_AUTOSIZE);
}

////////////////////////////////////////////////////////////////////////////////

int YUV::readFileHeader(char* filename) {
	char buf[256]; /* auxiliary buffer to parse file header */

	fp = fopen(filename, "rb");

	if (fp == NULL) {
		fprintf(stderr, "Error opening file\n");
		return -1;
	}

	if (!fgets(buf, 256, fp)) {
		fclose(fp);
		fp = NULL;
		fprintf(stderr, "Error reading file header\n");
		return -1;
	}

	if (sscanf(buf, "%u%u%u%u", &nCols, &nRows, &fps, &type) != 4) {
		fclose(fp);
		fp = NULL;
		fprintf(stderr, "Invalid file header\n");
		return -1;
	}

	return 0;
}

int YUV::writeFileHeader(char* filename) {
	fp = fopen(filename, "wb");

	if (fp == NULL) {
		fprintf(stderr, "Error opening file\n");
		return -1;
	}

	fprintf(fp, "%u %u %u %u", nCols, nRows, fps, type);

	return 0;
}

int YUV::readFrame() {
	if (fread(buffer, sizeof(unsigned char), bufferSize, fp) != bufferSize)
		return -1;

	return 0;
}

int YUV::appendFrame() {
	if (fwrite(buffer, sizeof(unsigned char), bufferSize, fp) != bufferSize)
		return -1;

	return 0;
}

////////////////////////////////////////////////////////////////////////////////

void YUV::displayFrame() {
	char inputKey;

	if (type != 444) {
		unsigned char tmp[3 * nRows * nCols];
//		YUVtoYUV444(tmp);
		YUVtoRGB(tmp);
	} else {
		YUVtoRGB(buffer);
	}

	cvShowImage("YUV", img);

	/* wait according to the frame rate */
	inputKey = cvWaitKey(1.0 / fps * 1000);
}

////////////////////////////////////////////////////////////////////////////////

void YUV::YUVtoRGB(unsigned char *yuvBuffer) {
	unsigned char *imgBuffer; /* openCV image buffer */
	unsigned char *yBuff;
	unsigned char *uBuff;
	unsigned char *vBuff;
	int n, r, g, b, y, u, v; /* auxiliary variables */

	/* The video is stored in YUV planar mode but OpenCv uses packed modes */
	imgBuffer = (uchar*) img->imageData;
	yBuff = yuvBuffer;
	uBuff = yuvBuffer + (nRows * nCols);
	vBuff = yuvBuffer + (2 * nRows * nCols);

	/* YUV444: |yR*yC| yR*yC| yR*yC| */
	for (unsigned int i = 0, j = 0; i < nRows * nCols; i++, j+=3) {

		/* Accessing to planar information */
		y = yBuff[i];
		u = uBuff[i];
		v = vBuff[i];

		YUVtoRGB(y, u, v, r, g, b);

		// Fill the OpenCV buffer - packed mode: BGRBGR...BGR
		imgBuffer[j    ] = b;
		imgBuffer[j + 1] = g;
		imgBuffer[j + 2] = r;
	}
}

void inline YUV::YUVtoRGB(int y, int u, int v, int &r, int &g, int &b) {
	// convert to RGB
	b = (int) (1.164 * (y - 16) + 2.018 * (u - 128));
	g = (int) (1.164 * (y - 16) - 0.813 * (u - 128) - 0.391 * (v - 128));
	r = (int) (1.164 * (y - 16) + 1.596 * (v - 128));

	// clipping to [0 ... 255]
	if (r < 0)
		r = 0;
	if (g < 0)
		g = 0;
	if (b < 0)
		b = 0;
	if (r > 255)
		r = 255;
	if (g > 255)
		g = 255;
	if (b > 255)
		b = 255;

	// if you need the inverse formulas
	//y = r *  .299 + g *  .587 + b *  .114 ;
	//u = r * -.169 + g * -.332 + b *  .500  + 128.;
	//v = r *  .500 + g * -.419 + b * -.0813 + 128.;
}
