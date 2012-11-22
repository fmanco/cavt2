#include <cstdio>
#include "Golomb.h"
#include "BitStream.h"
#include "YuvReader.h"
#include "IntraCoder.h"
#include "YuvDisplay.h"
int main( int argc, char** argv ){

	char* file = "file"; //BACK OFF

	int nFrames;

	BitStream bs = BitStream(file, BitStream::WRITE);


	YuvReader reader("../ducks_take_off-1280x720-50-444.yuv");
	reader.open();
	reader.readHeader();

	bs.open();

	YuvFrame frame(reader.getNRows(), reader.getNCols());

	printf("fps: %d, nFrames:%d , type: %d\n, nCols: %d, nRows: %d", reader.getFps(), reader.getNFrames(), reader.getType(), reader.getNCols(), reader.getNRows());

	while (reader.readFrame(frame) == 0){

		printf("frame #%d\n",nFrames++);
		IntraCoder::encode(frame, bs);
	}

	bs.close();

	printf("Done! %d frames!\n",nFrames);


	BitStream bs1 = BitStream(file, BitStream::READ);

	bs1.open();


	YuvDisplay display((char*)"YuvShow", reader.getFps(), reader.getNRows(), reader.getNCols());

	display.start();

	nFrames=0;
	while (IntraCoder::decode(bs, frame) == 0){
		printf("frame #%d\n",nFrames++);
		display.displayFrame(frame);
	}

	bs1.close();


	// printf("%d\n", Golomb::decode(15, bs1));
	// printf("%d\n", Golomb::decode(16, bs1));

	return 0;
}