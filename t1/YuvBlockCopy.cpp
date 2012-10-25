#include "YUV.h"
#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>


int main(int argc, char** argv){
	
	int i, j, c;
	
	if (argc < 5){
		fprintf(stderr, "Usage: YuvBlockCopy <source> <destination> <block rows> <block cols>");
	}
	
	int cpCols, cpRows;
	
	YUV a(argv[1]);
	YUV b(argv[2], a.nRows,a.nCols,a.fps,a.type);
	
	unsigned char* buffer=new unsigned char[a.nRows*a.nCols];
	
	int rows = atoi(argv[3]);
	int cols = atoi(argv[4]);
	
	while (a.readFrame()!=-1){
		for (i = 0; i < a.nRows/rows; i++){
			for (j = 0; j < a.nCols/cols; j++){
				cpRows = (a.nRows-i*rows)%rows;
				cpCols = (a.nCols-j*cols)%cols;
				
				for (c=0; c<3; c++){ //copy all components
					a.getBlock(cpRows, cpCols, i*rows, j*cols, c, buffer);
					b.fillBlock(cpRows, cpCols, i*rows, j*cols, c, buffer);
				}
			}
		}
		
			
		b.appendFrame();
		b.displayFrame();
		
	}
	
	delete [] buffer;
}
