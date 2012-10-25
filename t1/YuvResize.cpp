#include "YuvResize.h"
#include <stdio.h>
#include <stdlib.h>
//~ #include <cv.h>
//~ #include <highgui.h>

//~ const int YuvResize::REDUCE;
//~ const int YuvResize::EXPAND;

YuvResize::YuvResize(uint _factor, bool _improved): factor(_factor), improved(_improved){};

YuvResize::~YuvResize(){}

//0 -> reduce
//1 -> expand
YUV YuvResize::prepareCopy(YUV& orig, uint type){
	int newRows = (type==EXPAND ? orig.nRows*factor : orig.nRows/factor);
	int newCols = (type==EXPAND ? orig.nCols*factor : orig.nCols/factor);
	
	//~ printf("%d: %dx%d -> %dx%d\n", type, orig.nRows, orig.nCols, newRows, newCols);
	
	return YUV(newRows, newCols, orig.fps, orig.type);
}

	
void YuvResize::reduce(YUV& orig, YUV& resized){
	
	int r, c; // row, column in the original frame
	int i ,j; // pixels created by the resizing
	int rIdx = 0; // pixel positions in resized buffer

	int sumY, sumU, sumV;
	int summed;
	//Y
	for(r = 0 ; r < orig.nRows ; r += factor) {
		for(c = 0 ; c < orig.nCols ; c += factor) {
			
			// the value of the new pixel is the average of the reduced(removed) pixels
			if (improved){
				sumY = 0;
				summed = 0;
				//~ resized.yBufferRaw[rIdx];
				for (int i=-factor/2; i<factor/2;i++)
					for (int j=-factor/2; j<factor/2; j++){
						if (r+i >=0 && r+i<orig.nRows  && c+j>=0 && c+j<=orig.nCols){
							sumY += orig.yBufferRaw[(r+i)*orig.nCols + (c+j)];
							summed++;
						}
					}
				resized.yBufferRaw[rIdx]=sumY/summed;
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
			
			if (improved){
				sumU = 0;
				sumV = 0;
				summed = 0;
				for (int i=-factor/2; i<factor/2;i++) 
					for (int j=-factor/2; j<factor/2; j++){
						if (r+i >=0 && r+i<orig.nRows  && c+j>=0 && c+j<=orig.nCols){
							sumU+= orig.uBufferRaw[(r+i)*orig.nCols + (c+j)];
							sumV+= orig.vBufferRaw[(r+i)*orig.nCols + (c+j)];
							summed++;
						}
					}
				resized.uBufferRaw[rIdx]=sumU/summed;
				resized.vBufferRaw[rIdx]=sumV/summed;
			} else {
				resized.uBufferRaw[rIdx] = orig.uBufferRaw[r*orig.uvCols + c];
				resized.vBufferRaw[rIdx] = orig.vBufferRaw[r*orig.uvCols + c];
			}
			rIdx++;
		}
	}
}
	
void YuvResize::expand(YUV& orig, YUV& resized){
	
	int r, c; // row, column in the original frame
	int i ,j; // pixels created by the resizing
	int rIdx, oIdx = 0; // pixel positions in original and resized buffers

	// for bilinear interpolation
	uint val; //current pixel value
	uint rpos, rval; // righ pixel x position, right pixel value;
	uint bpos, bval; // bottom pixel y position, bottom pixel value;
	uint rbval; // right-bottom pixel value, position is (rpos, bpos)
	
	uint tival, bival;// top "line" interpolation value, bottom "line" interpolation value
	
	//Y
	for(r = 0 ; r < orig.nRows ; r++) {
		for(c = 0 ; c < orig.nCols ; c++) {
			rIdx = r*factor*resized.nCols + c*factor;
			
			if (improved){
				val = orig.yBufferRaw[r*orig.nCols + c];
				
				rpos = (r+1)*factor;
				if ( c + 1 < orig.nCols){ // handle interpolation with pixels out of bounds
					rval = orig.yBufferRaw[r*orig.nCols + c +1];
				} else {
					rval = orig.yBufferRaw[r*orig.nCols + c]; //keep last pixel value
				}
				bpos = (c+1)*factor;
				
				if ( r + 1 < orig.nRows){ // handle interpolation with pixels out of bounds
					bval = orig.yBufferRaw[(r+1)*orig.nCols + c];
				} else {
					bval = orig.yBufferRaw[r*orig.nCols + c]; //keep last pixel value
				}
				
				if ( r + 1 < orig.nRows && c + 1 < orig.nCols){
					bval = orig.yBufferRaw[(r+1)*orig.nCols + c + 1];
				} else {
					bval = orig.yBufferRaw[r*orig.nCols + c];
				}
			} 
			
			for(i = 0 ; i < factor ; i++) {
				for(j = 0 ; j < factor ; j++) {
					if (improved) {
						tival = interpolate(r, val, rpos, rval, r+i);
						bival = interpolate(r, bval, rpos, rbval, r+i);
						
						resized.yBufferRaw[rIdx + j + i*resized.nCols] = interpolate(c, tival, bpos, bival, c+j);
					} else {
						resized.yBufferRaw[rIdx + j + i*resized.nCols] = orig.yBufferRaw[oIdx];	
					}
				}
			}
			oIdx++;
		}
	}
	
	
	//U and V
	oIdx = 0;
	for(r = 0 ; r < orig.uvRows ; r++) {
		for(c = 0 ; c < orig.uvCols ; c++) {
			rIdx = r*factor*resized.uvCols + c*factor;
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
int YuvResize::interpolate(int a, int aVal, int b, int bVal, int c){
	return aVal + (((c-a)*bVal-(c-a)*aVal)/(b-a));
}
