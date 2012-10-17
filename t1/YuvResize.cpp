#include "YuvResize.h"
#include <stdio.h>
#include <stdlib.h>
//~ #include <cv.h>
//~ #include <highgui.h>

YuvResize::YuvResize(uint _type, uint _factor):type(_type), factor(_factor){};

YuvResize::~YuvResize(){}

//0 -> reduce
//1 -> expand
YUV YuvResize::prepareCopy(YUV& orig){
	int newRows = (type==1 ? orig.nRows*factor : orig.nRows/factor);
	int newCols = (type==1 ? orig.nCols*factor : orig.nCols/factor);
	
	
	printf("hey! %d x %d-> %d x %d\n", newRows, newCols, orig.nRows, orig.nCols);
	
	return YUV(newRows, newCols, orig.fps, orig.type);
	//return NULL;
}

void YuvResize::apply(YUV& orig, YUV& resized){
	
	
	int r, c; // row, column in the original frame
	int i ,j; // pixels created by the resizing
	int rIdx, oIdx = 0; // pixel positions original and resized buffers

	//Y
	for(r = 0 ; r < resized.nRows ; r += factor){
		for(c = 0 ; c < resized.nCols ; c += factor) {
			rIdx = r*resized.nCols + c;
			for(i = 0 ; i < factor ; i++){
				for(j = 0 ; j < factor ; j++){
					resized.yBufferRaw[rIdx + j +(i*resized.nCols)] = orig.yBufferRaw[oIdx];	
				}
			}
			oIdx++;
		}
	}
	
	//U and V
	oIdx = 0;
	for(r = 0 ; r < resized.uvRows ; r += factor){
		for(c = 0 ; c < resized.uvCols ; c += factor) {
			rIdx = r*resized.uvCols + c;
			for(i = 0 ; i < factor ; i++){
				for(j = 0 ; j < factor ; j++) {
					resized.uBufferRaw[rIdx + j + (i*resized.uvCols)] = orig.uBufferRaw[oIdx];
					resized.vBufferRaw[rIdx + j + (i*resized.uvCols)] = orig.vBufferRaw[oIdx];
				}
			}
			oIdx++;
		}
	}
	
}
