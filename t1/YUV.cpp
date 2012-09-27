#include "YUV.h"
#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>

YUV::YUV(int nR, int nC, int f, int t)
{
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
}

YUV::YUV(char *filename)
{
	
}

YUV::~YUV()
{
}

int YUV::readFrame()
{
	return 0;
}

void YUV::displayFrame()
{
}
