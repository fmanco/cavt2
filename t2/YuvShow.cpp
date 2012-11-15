#include <cstdio>
#include "YuvFrame.h"
#include "YuvDisplay.h"
#include "YuvReader.h"

int main( int argc, char** argv )
{
	unsigned int fps; /* frames per second */
	bool end = false, loop = false; /* control variables */

	/* check for the mandatory arguments */
	if( argc < 2 ) {
		fprintf( stderr, "Usage: YuvShow filename\n" );
		return 1;
	}

	YuvReader reader(argv[argc - 1]);

	reader.open();
	reader.readHeader();

	YuvDisplay display((char*)"YuvShow", reader.getFps(), reader.getNRows(), reader.getNCols());

	/* Parse other command line arguments */
	for(int n = 1 ; n < argc ; n++)
	{
		if(!strcmp("-fps", argv[n]))
		{
			fps = atof(argv[n+1]);
			n++;

			display.setFps(fps);
		}

		if(!strcmp("-loop", argv[n]))
		{
			loop = 1;
		}
	}


	YuvFrame frame(reader.getNRows(), reader.getNCols());

	display.start();

	while(!end)
	{
		if (reader.readFrame(frame) == 0) {
			display.displayFrame(frame);
		} else if (loop) {
			reader.rewind();
		} else {
			end = true;
			break;
		}
	}

	return 0;
}
