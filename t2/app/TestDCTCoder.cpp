#include <cstdio>
#include "Golomb.h"
#include "BitStream.h"
#include "YuvReader.h"
#include "YuvDisplay.h"
#include "DCTCoder.h"

int main( int argc, char** argv ){
	char* file = "file2"; //BACK OFF

	int nFrames;
	uint rows, cols, fps, type;
	// uint qY = 1, qU = 3, qV = 3;
	uint quant = 100;
	uint mode = 7;

	BitStream bs = BitStream(file, BitStream::WRITE);

	YuvReader reader("../coastguard-176x144-30.yuv");
	printf("%d\n", reader.open());
	printf("%d\n", reader.readHeader());

	rows = reader.getNRows();
	cols = reader.getNCols();
	fps = reader.getFps();
	type = reader.getType();

	printf("%d\n", bs.open());
	printf("type: %d", type);

	YuvFrame frame1(rows, cols);

	while (reader.readFrame(frame1) == 0 && nFrames < 60){
		printf("frame #%d\n",nFrames++);
		DCTCoder::encode(frame1, bs, quant);
	}

	bs.close();

	printf("Done! %d frames!\n",nFrames);

	//nFrames = rows = cols = fps = type = quant = mode = 0;

	nFrames = 0;

	BitStream bs1 = BitStream(file, BitStream::READ);
	printf("%d\n", bs1.open());

	YuvFrame frame2(YuvFrame::YUV420, rows, cols);

	//THIS!
	// frame2.get_write_yBuff_444();
	// frame2.get_write_uBuff_444();
	// frame2.get_write_vBuff_444();

	YuvDisplay display((char*)"YuvShow", fps, rows, cols);

	display.start();

	while (DCTCoder::decode(bs1, frame2, quant) == 0){
		printf("frame #%d\n",nFrames++);
		display.displayFrame(frame2);
	}

	bs1.close();


	return 0;
}