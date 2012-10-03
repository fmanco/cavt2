#include <stdio.h>
#include "YUV.h"

int main( int argc, char* argv[] )
{
	printf( "\nHi guise\n\n" );
	
	YUV *yuv = new YUV(argv[1]);
	
	while (yuv->readFrame() == 0){
		yuv->displayFrame();
	}
	
	
}
