#include "YuvResize.h"
#include "YUV.h"
#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>


/**
 *	Y = 0
 * 	U = 1
 * 	V = 2 
 */
float calcMSE(YUV& a, YUV& b, uint component) {
	//TODO: verify a and b dimensions match
	
	int lim = (component == 0 ? (a.nRows*a.nCols) : a.uvRows*a.uvCols);
	unsigned char *aBuffer, *bBuffer;
	
	switch (component){
		case 0: 
			aBuffer = a.yBuffer;
			bBuffer = b.yBuffer;
			break;
		case 1: 
			aBuffer = a.uBuffer;
			bBuffer = b.uBuffer;
			break;
		case 2: 
			aBuffer = a.vBuffer;
			bBuffer = b.vBuffer;
			break;
	}
	
	int sum = 0, i = 0;
	float mse = 1.0/lim;
	
	//~ printf("lim: %d, mse: %f\n", lim, mse);
	
	for(i = 0 ; i < lim; i++) {
		sum += (aBuffer[i] - bBuffer[i])*(aBuffer[i] - bBuffer[i]);
	}
	
	mse*=sum;
	
	return mse;
}

float inline calcPSNR(YUV& a, YUV& b, int component){
	return 10.0 * log10f(255.0*255.0/calcMSE(a,b,component));
}

int main(int argc, char** argv){
	uint frames = 0;
	float sumPNSR_Y = 0.0f;
	float sumPNSR_U = 0.0f;
	float sumPNSR_V = 0.0f;

	if( argc < 2 ) {
		fprintf( stderr, "Usage: YuvCmp filename\n or YuvCmp filename filename" );
		return 1;
	}

	YUV orig(argv[1]);
	
	if (argc == 2) {
		
		YuvResize resize = YuvResize(2, true);
		YUV reduced = resize.prepareCopy(orig, YuvResize::REDUCE);
		YUV expanded = resize.prepareCopy(reduced, YuvResize::EXPAND);

		//chained conversion
		while (orig.readFrame()!=-1){
			
			resize.reduce(orig, reduced);
			resize.expand(reduced, expanded);
			
			expanded.displayFrame();
			
			sumPNSR_Y += calcPSNR(orig, expanded, 0);
			sumPNSR_U += calcPSNR(orig, expanded, 1);
			sumPNSR_V += calcPSNR(orig, expanded, 2);
			
			frames++;
		}
	} else {
		YUV orig2(argv[2]);
		while (orig.readFrame()!=-1 && orig2.readFrame()!=-1){
			
			
			sumPNSR_Y += calcPSNR(orig, orig2, 0);
			sumPNSR_U += calcPSNR(orig, orig2, 1);
			sumPNSR_V += calcPSNR(orig, orig2, 2);
			
			frames++;
		}
	}
	
	printf("Y: %f, U:%f, V:%f\n", sumPNSR_Y/(float)frames, sumPNSR_U/(float)frames, sumPNSR_V/(float)frames);
	
	return 0;
}




