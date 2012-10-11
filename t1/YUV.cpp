#include "YUV.h"
#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>

YUV::YUV(int nR, int nC, int f, int t) {
	init(nR, nC, f, t);
}

YUV::YUV(char *filename) {
	int nR, nC; /* frame dimension */
	int f; /* frames per second */
	int t; /* YUV type [444, 422, 420] */

	if (readFileHeader(filename, &nC, &nR, &f, &t) != 0)
		throw new std::runtime_error("Unable to read file header!");

	init(nR, nC, f, t);
}

YUV::~YUV() {
	if (fp != NULL)
		fclose(fp);

	cvDestroyWindow("YUV");
	cvReleaseImage(&img);
}

void YUV::init(int nR, int nC, int f, int t) {
	nRows = nR;
	nCols = nC;
	fps   = f;
	type  = t;

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

	/* data structure for the OpenCv image */
	img = cvCreateImage(cvSize(nCols, nRows), IPL_DEPTH_8U, 3);

	//~ /* buffer to store the frame */
	//~ imgData = (unsigned char*)calloc(nCols * nRows * 3, sizeof(unsigned char));

	/* create a window */
	cvNamedWindow("YUV", CV_WINDOW_AUTOSIZE);
}

int YUV::readFileHeader(char* filename, int* yCols, int* yRows, int* fps,
		int* type) {
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

	if (sscanf(buf, "%d%d%d%d", yCols, yRows, fps, type) != 4) {
		fclose(fp);
		fp = NULL;
		fprintf(stderr, "Invalid file header\n");
		return -1;
	}

	return 0;
}

int YUV::readFrame() {
	if (fread(buffer, sizeof(unsigned char), bufferSize, fp) == 0)
		return -1;

	convert();

	return 0;
}

void YUV::displayFrame() {
	char inputKey;

	cvShowImage("YUV", img);

	/* wait according to the frame rate */
	inputKey = cvWaitKey(1.0 / fps * 1000);
}

void YUV::convert() {
	unsigned char *imgBuffer; /* cv image buffer */
	int n, r, g, b, y, u, v;  /* auxiliary variables */

	/* The video is stored in YUV planar mode but OpenCv uses packed modes */
	imgBuffer = (uchar*) img->imageData;
	bool a = false;
	switch (type) {
	                                         /* YUV444: |yR*yC| yR*yC| yR*yC| */
	case 444:
		for (unsigned int i = 0, yi = 0, ui = 0, vi = 0;
				i < 3 * nCols * nRows; i += 3, yi++, ui++, vi++) {

			/* Accessing to planar information */
			y = yBuffer[yi];
			u = uBuffer[ui];
			v = vBuffer[vi];

			YUVtoRGB(y, u, v, r, g, b);

			// Fill the OpenCV buffer - packed mode: BGRBGR...BGR
			imgBuffer[i] = b;
			imgBuffer[i + 1] = g;
			imgBuffer[i + 2] = r;
		}
		break;

	case 422:
		// Fill the OpenCV buffer - packed mode: BGRBGR...BGR
		for (unsigned int i = 0, j = 0; i < 3 * nCols * nRows; i += 6, j += 2) {
			y = yBuffer[j];
			u = uBuffer[j / 2];
			v = vBuffer[j / 2];

			YUVtoRGB(y, u, v, r, g, b);

			imgBuffer[i    ] = b;
			imgBuffer[i + 1] = g;
			imgBuffer[i + 2] = r;

			y = yBuffer[j + 1];

			YUVtoRGB(y, u, v, r, g, b);

			imgBuffer[i + 3] = b;
			imgBuffer[i + 4] = g;
			imgBuffer[i + 5] = r;
		}
		break;

	case 420:
		for (int i = 0; i < nCols * nRows; i++) {
		}
		break;
	}
}

void YUV::YUVtoRGB(int y, int u, int v, int &r, int &g, int &b) {
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

	// if you need the inverse formulas
	//y = r *  .299 + g *  .587 + b *  .114 ;
	//u = r * -.169 + g * -.332 + b *  .500  + 128.;
	//v = r *  .500 + g * -.419 + b * -.0813 + 128.;
}
