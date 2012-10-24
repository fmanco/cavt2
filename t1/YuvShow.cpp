#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include "YUV.h"
#include "YuvResize.h"

int main( int argc, char** argv )
{
	unsigned int fps; /* frames per second */
	char inputKey = '?'; /* parse the pressed key */
	int end = 0, playing = 1, loop = 0; /* control variables */

	/* check for the mandatory arguments */
	if( argc < 2 ) {
		fprintf( stderr, "Usage: YuvShow filename\n" );
		return 1;
	}

	/* Creating YUV */
	YUV yuv(argv[argc - 1]);

	/* Parse other command line arguments */
	for(int n = 1 ; n < argc ; n++)
	{
		if(!strcmp("-fps", argv[n]))
		{
			fps = atof(argv[n+1]);
			n++;

			yuv.setFps(fps);
		}

		// FIXME: How to do this?
		if(!strcmp("-wait", argv[n]))
		{
			playing = 0;
		}

		if(!strcmp("-loop", argv[n]))
		{
			loop = 1;
		}
	}
	
	YuvResize res = YuvResize(2, true);
	YUV resized = res.prepareCopy(yuv, YuvResize::EXPAND);
	
	while(!end)
	{
		if (!yuv.readFrame()) {
			//~ yuv.convertToBW();
			res.expand(yuv, resized);
			
			//~ yuv.displayFrame();
			resized.displayFrame();
		} else if (loop) {
			yuv.rewind();
		} else {
			end = true;
			break;
		}
	}

	return 0;
}
