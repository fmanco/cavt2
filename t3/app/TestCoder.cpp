#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sndfile.h>
#include "BitStream.h"
#include "Golomb.h"
#include "Predictor.h"
#include "SFReader.h"
#include "SFWriter.h"

#define VAL_M 2048


int main(int argc, char **argv)
{

	SNDFILE *soundFileIn; /* Pointer for input sound file */

	SF_INFO soundInfoIn; /* Input sound file Info */


	int i;
	short sample[2];
	short tmp[2];

	sf_count_t nSamples = 1;
	int frames;
	int samplerate;
	int channels;
	std::string file = "testfile";

	std::string fileIn(argv[argc-2]);
	std::string fileOut(argv[2]);

	long valAvg = 0;
	long diffAvg = 0;




	if (argc < 3){
		fprintf(stderr, "Usage: wavCopy <input file> <output file>\n");
		return -1;
	}

	{
		Predictor p(100);

		SFReader sfReader(fileIn);

		sfReader.open();
		sfReader.printInfo();

		BitStream bs1 = BitStream(file, BitStream::WRITE);


		if (bs1.open() != 0){
    		printf("Error closing!\n");
    	}


		for (i = 0; i < sfReader.getNFrames() ; i++)
		{

			sfReader.nextFrame(sample);

			p.predict(tmp);

			Golomb::encode(VAL_M, tmp[0] - sample[0], bs1);
			Golomb::encode(VAL_M, tmp[1] - sample[1], bs1);

			// valAvg += 2*abs(sample[0]);
			// valAvg += 2*abs(sample[1]);

			// diffAvg += abs(2*(prevSample[0] - sample[0]));
			// diffAvg += abs(2*(prevSample[1] - sample[1]));

			p.update(sample);
		}

		// printf("%ld, %ld\n", valAvg/(long)frames*2, diffAvg/(long)frames*2);

		if (bs1.close() != 0){
        	printf("Error closing!\n");
    	}
    	sfReader.close();
    	soundInfoIn = sfReader.getInfo();
	}

	{

		Predictor p(100);

		SFWriter sfWriter(fileOut, soundInfoIn);

		BitStream bs2 = BitStream(file, BitStream::READ);

		if (bs2.open() != 0){
    		printf("Error opening!\n");
    		return -1;
    	}

    	sfWriter.printInfo();
    	sfWriter.open();


		int smp = 0;


		while (true){
			p.predict(tmp);

			if (Golomb::decode(VAL_M, &smp, bs2)!=0)
				break;

			sample[0] = tmp[0] - (short)smp;

			if (Golomb::decode(VAL_M, &smp, bs2)!=0)
				break;

			sample[1] = tmp[1] - (short)smp;


			sfWriter.writeFrame(sample);

			p.update(sample);

		}
		bs2.close();
		sfWriter.close();

	}

	char dumbness[50];
	strcpy(dumbness, "cmp ");
	strcat(dumbness, argv[1]);
	strcat(dumbness, " ");
	strcat(dumbness, argv[2]);

	if (system(dumbness) != 0)
		printf("uh oh\n");
	else {
		printf("files match");
		strcpy(dumbness, "ls -sh -1 ");
		strcat(dumbness, argv[1]);
		strcat(dumbness, " ");
		strcat(dumbness, file.c_str());
		system(dumbness);
	}

	return 0;
}

