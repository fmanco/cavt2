#include "YUV.h"
#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>

YUV::YUV(int nR, int nC, int f, int t)
{
	init(nR, nC, f, t);
}

YUV::YUV(char *filename)
{
	char buf[256]; /* auxiliary buffer to parse file header */
	int yCols, yRows; /* frame dimension */
	int fps_; /* frames per second */
	int type_; /* YUV type */
	
	fp = fopen(filename, "rb");
	if(fp == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		//~ this is dumb
		//~ return -1;
	}
	
	//~ if(!fgets(buf, 256, fp))
		//~ return -1;
		
	fgets(buf, 256, fp);

	//~ if(sscanf(buf, "%d%d%d%d", &yCols, &yRows, &fps, &type) != 4)
		//~ return -1;
		
	sscanf(buf, "%d%d%d%d", &yCols, &yRows, &fps_, &type_);
	
	init(yRows, yCols, fps_, type_);
	
}

YUV::~YUV()
{
	//~ free memory son
}

void YUV::init(int nR, int nC, int f, int t){
	nRows = nR;
	nCols = nC;
	fps = f;
	type = t;
	
	switch (type)
	{
		case 444:
			uvCols = nCols;
			uvRows = nRows;
			break;
		case 422:
			uvCols = nCols/2;
			uvRows = nRows/2;
			break;
		case 420:
			uvCols = nCols/2;
			uvRows = nRows/2;
			break;
	}
	
	bufferSize = nRows * nCols + uvRows * uvCols * 2;
	buffer = new unsigned char[bufferSize];
	yBuffer = buffer;
	uBuffer = buffer + nRows*nCols;
	vBuffer = buffer + nRows*nCols + uvRows*uvCols;
	
	/* data structure for the OpenCv image */
	img = cvCreateImage(cvSize(nCols, nRows), IPL_DEPTH_8U, 3);

	//~ /* buffer to store the frame */
	//~ imgData = (unsigned char*)calloc(nCols * nRows * 3, sizeof(unsigned char));

	/* create a window */
	cvNamedWindow( "YUV", CV_WINDOW_AUTOSIZE );
}

int YUV::readFrame()
{
	unsigned char *imgBuffer; /** cv image buffer */
	int i, n, r, g, b, y, u, v; /* auxiliary variables */
	
	if(fread(buffer, sizeof(unsigned char), bufferSize, fp) == 0){
		return -1;
	}
	
	/* The video is stored in YUV planar mode but OpenCv uses packed modes*/
	/* YUV444: |yR*yC| yR*yC| yR*yC| */
	imgBuffer = (uchar*)img->imageData;
	for(i = 0 ; i < nRows * nCols * 3 ; i += 3)
	{ 
		/* Accessing to planar infor */
		y = buffer[i / 3]; 
		u = buffer[(i / 3) + (nRows * nCols)]; 
		v = buffer[(i / 3) + (nRows * nCols) * 2]; 

		/* convert to RGB */
		b = (int)(1.164*(y - 16) + 2.018*(u-128));
		g = (int)(1.164*(y - 16) - 0.813*(u-128) - 0.391*(v-128));
		r = (int)(1.164*(y - 16) + 1.596*(v-128));

		/* clipping to [0 ... 255] */
		if(r < 0) r = 0;
		if(g < 0) g = 0;
		if(b < 0) b = 0;
		if(r > 255) r = 255;
		if(g > 255) g = 255;
		if(b > 255) b = 255;

		/* if you need the inverse formulas */
		//y = r *  .299 + g *  .587 + b *  .114 ;
		//u = r * -.169 + g * -.332 + b *  .500  + 128.;
		//v = r *  .500 + g * -.419 + b * -.0813 + 128.;

		/* Fill the OpenCV buffer - packed mode: BGRBGR...BGR */
		imgBuffer[i] = b;
		imgBuffer[i + 1] = g;
		imgBuffer[i + 2] = r;
	}

	return 0;
}

void YUV::displayFrame()
{
	char inputKey;
	
	cvShowImage( "rgb", img );
	/* wait according to the frame rate */
	inputKey = cvWaitKey(1.0 / fps * 1000);
}
