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
	
	//TA MÁL, só ve em Y!
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
		fprintf( stderr, "Usage: YuvCmp filename\n" );
		return 1;
	}
	
	YuvResize reduce = YuvResize(YuvResize::REDUCE, 2);
	YuvResize expand = YuvResize(YuvResize::EXPAND, 2);

	YUV orig(argv[argc - 1]);
	
	YUV reduced = reduce.prepareCopy(orig);
	YUV expanded = expand.prepareCopy(reduced);
	
	//chained conversion
	while (orig.readFrame()!=-1){
		
		reduce.apply(orig, reduced);
		expand.apply(reduced, expanded);
		
		//~ reduced.displayFrame();
		expanded.displayFrame();
		
		sumPNSR_Y += calcPSNR(orig, expanded, 0);
		sumPNSR_U += calcPSNR(orig, expanded, 1);
		sumPNSR_V += calcPSNR(orig, expanded, 2);
		
		frames++;
	}
	printf("Y: %f, U:%f, V:%f\n", sumPNSR_Y/(float)frames, sumPNSR_U/(float)frames, sumPNSR_V/(float)frames);
	
	return 0;
}




