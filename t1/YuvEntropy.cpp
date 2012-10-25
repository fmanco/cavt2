
#include "YUV.h"
#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>



/**
 * Calculate entropy for each component (Y,U,V) of a frame 
 */
float frameEntropy(YUV& yuv, uint component){
	
	//TODO: verify a and b dimensions match
	
	uint lim = (component == 0 ? (yuv.nRows*yuv.nCols) : yuv.uvRows*yuv.uvCols);
	uint i;
	float entropy=0.0;
	float prob;
	uint occurrence[255];
	unsigned char *buffer;
	
	switch (component){
		case 0: 
			buffer = yuv.yBufferRaw;
			break;
		case 1: 
			buffer = yuv.uBufferRaw;
			break;
		case 2: 
			buffer = yuv.vBufferRaw;
			break;
	}
	
	//~ memset(occurrence, 0, 255); memsets, how do they work?
	
	for (i = 0; i < 255; i++){
		occurrence[i] = 0;
	}
	
	for(i = 0 ; i < lim; i++) {
		occurrence[buffer[i]]++;
	}	
	
	for (i = 0; i < 255; i++){
		//if no occurences, no adding is necessary
		if (occurrence[i]>0) {
			prob = ((float) occurrence[i])/(float)lim;
			entropy += prob*log2f(prob);
		}
	}
	
	return entropy;	
}


int main(int argc, char** argv){
	uint frames = 0;
	float entropy_Y = 0.0f;
	float entropy_U = 0.0f;
	float entropy_V = 0.0f;

	if( argc < 2 ) {
		fprintf( stderr, "Usage: YuvEntropy filename\n" );
		return 1;
	}
	
	YUV yuv(argv[argc - 1]);
	
	
	while (yuv.readFrame()!=-1){
		
		//~ reduce.apply(orig, reduced);
		//~ expand.apply(reduced, expanded);
		
		//~ reduced.displayFrame();
		//~ expanded.displayFrame();
		
		entropy_Y += frameEntropy(yuv, 0);
		entropy_U += frameEntropy(yuv, 1);
		entropy_V += frameEntropy(yuv, 2);
		
		frames++;
	}
	printf("Y: %f, U:%f, V:%f\n", entropy_Y/(float)frames, entropy_U/(float)frames, entropy_V/(float)frames);
	
	return 0;
}





