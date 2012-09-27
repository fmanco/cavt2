#include <stdio.h>
#include <cv.h>
#include <highgui.h>

int main( int argc, char** argv )
{
	FILE *fp; /* file pointer to the video */
	int yCols, yRows; /* frame dimension */
	int fps; /* frames per second */
	int type; /* YUV type */
	int i, n, r, g, b, y, u, v; /* auxiliary variables */
	unsigned char *buffer, *imgData; /* data buffers */
	char buf[256]; /* auxiliary buffer to parse file header */
	char inputKey = '?'; /* parse the pressed key */
	int end = 0, playing = 1, loop = 0; /* control variables */

	/* check for the mandatory arguments */
	if( argc < 2 ) {
		fprintf( stderr, "Usage: YuvShow filename\n" );
		return 1;
	}

	/* Opening video file */
	fp = fopen(argv[argc - 1], "rb");
	if(fp == NULL)
	{
		fprintf(stderr, "Error opening file\n");
		return -1;
	}

	/* Processing header */
	if(!fgets(buf, 256, fp))
		return -1;

	if(sscanf(buf, "%d%d%d%d", &yCols, &yRows, &fps, &type) != 4)
		return -1;

	/* Parse other command line arguments */
	for(n = 1 ; n < argc ; n++)
	{
		if(!strcmp("-fps", argv[n]))
		{
			fps = atof(argv[n+1]);
			n++;
		}

		if(!strcmp("-wait", argv[n]))
		{
			playing = 0;
		}

		if(!strcmp("-loop", argv[n]))
		{
			loop = 1;
		}
	}

	/* data structure for the OpenCv image */
	IplImage *img = cvCreateImage(cvSize(yCols, yRows), IPL_DEPTH_8U, 3);

	/* buffer to store the frame */
	imgData = (unsigned char*)calloc(yCols * yRows * 3, sizeof(unsigned char));

	/* create a window */
	cvNamedWindow( "YUV", CV_WINDOW_AUTOSIZE );
	
	while(!end)
	{
		/* load a new frame, if possible */
		if(fread(imgData, sizeof(unsigned char), yCols * yRows * 3, fp) == 0)
		{
			if(loop)
			{
				rewind(fp); /* return to the beginning of the file */
				if(!fgets(buf, 256, fp)) /* read again the header */
					return -1;
				continue; /* try to read again */
			}
			else
			{
				end = 1; /* finish the main loop */
				break;
			}
		}

		/* The video is stored in YUV planar mode but OpenCv uses packed modes*/
		/* YUV444: |yR*yC| yR*yC| yR*yC| */
		buffer = (uchar*)img->imageData;
		for(i = 0 ; i < yRows * yCols * 3 ; i += 3)
		{ 
			/* Accessing to planar infor */
			y = imgData[i / 3]; 
			u = imgData[(i / 3) + (yRows * yCols)]; 
			v = imgData[(i / 3) + (yRows * yCols) * 2]; 

			/* convert to RGB */
			b = (int)(1.164*(y - 16) + 2.018*(u-128));
			g = (int)(1.164*(y - 16) - 0.813*(u-128) - 0.391*(v-128));
			r = (int)(1.164*(y - 16) + 1.596*(v-128));

			/* clipping to [0 ... 255] */
			if(r < 0) r = 0;
			if(g < 0) g = 0;
			if(b < 0) b = 0;
			if(r > 255) r = 255;
			if(g > 255) g = 255;
			if(b > 255) b = 255;

			/* if you need the inverse formulas */
			//y = r *  .299 + g *  .587 + b *  .114 ;
			//u = r * -.169 + g * -.332 + b *  .500  + 128.;
			//v = r *  .500 + g * -.419 + b * -.0813 + 128.;

			/* Fill the OpenCV buffer - packed mode: BGRBGR...BGR */
			buffer[i] = b;
			buffer[i + 1] = g;
			buffer[i + 2] = r;
		}

		/* display the image */
		cvShowImage( "rgb", img );
		
		if(playing)
		{
			/* wait according to the frame rate */
			inputKey = cvWaitKey(1.0 / fps * 1000);
		}
		else
		{
			/* wait until user press a key */
			inputKey = cvWaitKey(0);
		}
	
		/* parse the pressed keys, if any */
		switch((char)inputKey)
		{
			case 'q':
				end = 1;
				break;
			
			case 'p':
				playing = playing ? 0 : 1;
				break;
		}
	}
	
	/* free memory */
	cvDestroyWindow( "rgb" );
	cvReleaseImage( &img );
	
	return 0;
}
