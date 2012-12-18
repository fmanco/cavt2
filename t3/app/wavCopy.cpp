#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sndfile.h>

int main(int argc, char **argv)
{

	SNDFILE *soundFileIn; /* Pointer for input sound file */
	SNDFILE *soundFileOut; /* Pointer for output sound file */
	SF_INFO soundInfoIn; /* Input sound file Info */
	SF_INFO soundInfoOut; /* Output sound file Info */

	int i;
	short sample[2];
	sf_count_t nSamples = 1;

	if (argc < 3){
		fprintf(stderr, "Usage: wavCopy <input file> <output file>\n");
		return -1;
	}


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

	fprintf(stderr, "Frames (samples): %d\n", (int) soundInfoIn.frames);
	fprintf(stderr, "Samplerate: %d\n", soundInfoIn.samplerate);
	fprintf(stderr, "Channels: %d\n", soundInfoIn.channels);

	/* When opening a file for write, it is necessary to fill in the
	 * structure members samplerate, channels and format.
	 */
	soundInfoOut.samplerate = soundInfoIn.samplerate;
	soundInfoOut.channels = soundInfoIn.channels;
	soundInfoOut.format = soundInfoIn.format;

	soundFileOut = sf_open (argv[2], SFM_WRITE, &soundInfoOut);

	if (soundFileOut == NULL){
		fprintf(stderr, "Could not open file for writing: \"%s\"\n", argv[2]);
		sf_close(soundFileIn);
		return -1;
	}

	for (i = 0; i < soundInfoIn.frames ; i++)
	{
		if (sf_readf_short(soundFileIn, sample, nSamples) == 0){
			fprintf(stderr, "Error: Reached end of file\n");
			sf_close(soundFileIn);
			sf_close(soundFileOut);
			break;
		}

		if (sf_writef_short(soundFileOut, sample, nSamples) != 1){
			fprintf(stderr, "Error writing frames to the output:\n");
			sf_close(soundFileIn);
			sf_close(soundFileOut);
			return -1;
		}
	}

	sf_close(soundFileIn);
	sf_close(soundFileOut);

	return 0;
}

