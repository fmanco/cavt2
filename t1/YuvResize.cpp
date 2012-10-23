#include "YuvResize.h"
#include <stdio.h>
#include <stdlib.h>
//~ #include <cv.h>
//~ #include <highgui.h>

//~ const int YuvResize::REDUCE;
//~ const int YuvResize::EXPAND;

YuvResize::YuvResize(uint _type, uint _factor):type(_type), factor(_factor){};

YuvResize::~YuvResize(){}

//0 -> reduce
//1 -> expand
YUV YuvResize::prepareCopy(YUV& orig){
	int newRows = (type==EXPAND ? orig.nRows*factor : orig.nRows/factor);
	int newCols = (type==EXPAND ? orig.nCols*factor : orig.nCols/factor);
	
	//~ printf("%d: %dx%d -> %dx%d\n", type, orig.nRows, orig.nCols, newRows, newCols);
	
	return YUV(newRows, newCols, orig.fps, orig.type);
}

void YuvResize::apply(YUV& orig, YUV& resized){
	
	if (type==EXPAND){
		expand(orig, resized);
	} else {
		reduce(orig, resized);
	}
	
}
	
void YuvResize::reduce(YUV& orig, YUV& resized){
	
	int r, c; // row, column in the original frame
	int i ,j; // pixels created by the resizing
	int rIdx = 0; // pixel positions original and resized buffers

	int sum;
	int summed;
	//Y
	for(r = 0 ; r < orig.nRows ; r += factor) {
		for(c = 0 ; c < orig.nCols ; c += factor) {
			if (true){
				sum=0;
				summed=0;
				resized.yBufferRaw[rIdx];
				for (int i=-factor/2; i<factor/2;i++)
					for (int j=-factor/2; j<factor/2; j++){
						if (r+i >=0 && r+i<orig.nRows  && c+j>=0 && c+j<=orig.nCols){
							sum+= orig.yBufferRaw[(r+i)*orig.nCols + (c+j)];
							summed++;
						}
					}
				resized.yBufferRaw[rIdx]=sum/summed;
			} else {
				resized.yBufferRaw[rIdx] = orig.yBufferRaw[r*orig.nCols + c];
			}
			rIdx++;
		}
	}

	//U and V
	rIdx = 0;
	for(r = 0 ; r < orig.uvRows ; r += factor) {
		for(c = 0 ; c < orig.uvCols ; c += factor) {
			resized.uBufferRaw[rIdx] = orig.uBufferRaw[r*orig.uvCols + c];
			resized.vBufferRaw[rIdx] = orig.vBufferRaw[r*orig.uvCols + c];
			rIdx++;
		}
	}
}
	
void YuvResize::expand(YUV& orig, YUV& resized){
	
	int r, c; // row, column in the original frame
	int i ,j; // pixels created by the resizing
	int rIdx, oIdx = 0; // pixel positions original and resized buffers

	//Y
	for(r = 0 ; r < resized.nRows ; r += factor) {
		for(c = 0 ; c < resized.nCols ; c += factor) {
			rIdx = r*resized.nCols + c;
			for(i = 0 ; i < factor ; i++) {
				for(j = 0 ; j < factor ; j++) {
					resized.yBufferRaw[rIdx + j + i*resized.nCols] = orig.yBufferRaw[oIdx];	
				}
			}
			oIdx++;
		}
	}
	
	//U and V
	oIdx = 0;
	for(r = 0 ; r < resized.uvRows ; r += factor) {
		for(c = 0 ; c < resized.uvCols ; c += factor) {
			rIdx = r*resized.uvCols + c;
			for(i = 0 ; i < factor ; i++) {
				for(j = 0 ; j < factor ; j++) {
					resized.uBufferRaw[rIdx + j + i*resized.uvCols] = orig.uBufferRaw[oIdx];
					resized.vBufferRaw[rIdx + j + i*resized.uvCols] = orig.vBufferRaw[oIdx];
				}
			}
			oIdx++;
		}
	}
}
