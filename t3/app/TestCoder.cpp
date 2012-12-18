#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sndfile.h>
#include "BitStream.h"
#include "Golomb.h"

#define VAL_M 128
//com M maior (512) dá erros, investigar


void printInfo(SF_INFO &info){
	printf("frames : %d\n", (int) info.frames);
	printf("samplerate : %d\n", info.samplerate);
	printf("channels : %d\n", info.channels);
	printf("format : %d\n", info.format);
	printf("sections : %d\n", info.sections);
	printf("seekable : %d\n\n", info.seekable);
}

int main(int argc, char **argv)
{

	SNDFILE *soundFileIn; /* Pointer for input sound file */
	
	SF_INFO soundInfoIn; /* Input sound file Info */
	

	int i;
	short sample[2];
	// short smp;
	short prevSample[2];

	sf_count_t nSamples = 1;
	int frames;
	int samplerate;
	int channels;
	char * file = "testfile";

	long valAvg = 0;
	long diffAvg = 0;

	prevSample[0] = 0;
	prevSample[1] = 0;

	if (argc < 3){
		fprintf(stderr, "Usage: wavCopy <input file> <output file>\n");
		return -1;
	}

	{

		/* When opening a file for read, the format field should be set to zero
		 * before calling sf_open(). All other fields of the structure are filled
		 * in by the library
		 */	
		soundInfoIn.format = 0;
		soundFileIn = sf_open (argv[argc-2], SFM_READ, &soundInfoIn);
		
		if (soundFileIn == NULL){
			fprintf(stderr, "Could not open file for reading: \"%s\"\n",
			  argv[argc-2]);
			sf_close(soundFileIn);
			return -1;
		}

		// fprintf(stderr, "Frames (samples): %d\n", (int) soundInfoIn.frames);
		// fprintf(stderr, "Samplerate: %d\n", soundInfoIn.samplerate);
		// fprintf(stderr, "Channels: %d\n", soundInfoIn.channels);

		printInfo(soundInfoIn);

		frames = (int) soundInfoIn.frames;
		samplerate = soundInfoIn.samplerate;
		channels = soundInfoIn.channels;

		BitStream bs1 = BitStream(file, BitStream::WRITE);


		if (bs1.open() != 0){
    		printf("Error closing!\n");
    	}

		// bs1.writeBits(frames, 32);
		// bs1.writeBits(samplerate, 32);
		// bs1.writeBits(channels, 32);

		for (i = 0; i < soundInfoIn.frames ; i++)
		{
			if (sf_readf_short(soundFileIn, sample, nSamples) == 0){
				fprintf(stderr, "Error: Reached end of file\n");
				sf_close(soundFileIn);
				break;
			}

			Golomb::encode(VAL_M, prevSample[0] - sample[0], bs1);
			Golomb::encode(VAL_M, prevSample[1] - sample[1], bs1);

			// valAvg += 2*abs(sample[0]);
			// valAvg += 2*abs(sample[1]);

			// diffAvg += abs(2*(prevSample[0] - sample[0]));
			// diffAvg += abs(2*(prevSample[1] - sample[1]));

			prevSample[0] = sample[0];
			prevSample[1] = sample[1];
		}

		// printf("%ld, %ld\n", valAvg/(long)frames*2, diffAvg/(long)frames*2);

		if (bs1.close() != 0){
        	printf("Error closing!\n");
    	}

		sf_close(soundFileIn);
	}

	{
		SNDFILE *soundFileOut; /* Pointer for output sound file */
		SF_INFO soundInfoOut; /* Output sound file Info */

		BitStream bs2 = BitStream(file, BitStream::READ);
		if (bs2.open() != 0){
    		printf("Error opening!\n");
    		return -1;
    	}

    	//header doesnt work
		// uchar tmp;
		// bs2.readBits(32, &tmp);
		// soundInfoOut.samplerate =  (int) tmp;

		// bs2.readBits(32, &tmp);
		// soundInfoOut.channels =  (int) tmp;

		// bs2.readBits(32, &tmp);
		// soundInfoOut.format =  (int) tmp;

		soundInfoOut.frames  = soundInfoIn.frames;
		soundInfoOut.samplerate = soundInfoIn.samplerate;
		soundInfoOut.channels = soundInfoIn.channels;
		soundInfoOut.format = soundInfoIn.format;
		soundInfoOut.sections = soundInfoIn.sections;
		soundInfoOut.seekable = soundInfoIn.seekable;

		printInfo(soundInfoOut);

		// fprintf(stderr, "Frames (samples): %d\n", (int) soundInfoOut.frames);
		// fprintf(stderr, "Samplerate: %d\n", soundInfoOut.samplerate);
		// fprintf(stderr, "Channels: %d\n", soundInfoOut.channels);


		soundFileOut = sf_open (argv[2], SFM_WRITE, &soundInfoOut);
		if (soundFileOut == NULL){
			fprintf(stderr, "Could not open file for writing: \"%s\"\n", argv[2]);
			bs2.close();
			return -1;
		}

		int smp = 0;

		prevSample[0] = 0;
		prevSample[1] = 0;

		while (true){
			if (Golomb::decode(VAL_M, &smp, bs2)!=0)
				break;

			sample[0] = prevSample[0] - (short)smp;

			if (Golomb::decode(VAL_M, &smp, bs2)!=0)
				break;

			sample[1] = prevSample[1] - (short)smp;

			if (sf_writef_short(soundFileOut, sample, nSamples) != 1){
				fprintf(stderr, "Error writing frames to the output:\n");
				bs2.close();
				return -1;
			}

			prevSample[0] = sample[0];
			prevSample[1] = sample[1];

		}
		bs2.close();
		sf_close(soundFileOut);

	}

	return 0;
}

