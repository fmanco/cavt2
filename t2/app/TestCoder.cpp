#include <cstdio>
#include "Golomb.h"
#include "BitStream.h"
#include "YuvReader.h"
#include "IntraCoder.h"
#include "YuvDisplay.h"
int main( int argc, char** argv ){

	char* file = "file"; //BACK OFF

	int nFrames;
	uint rows, cols, fps, type;
	uint quant = 3;

	BitStream bs = BitStream(file, BitStream::WRITE);

	YuvReader reader("../ducks_take_off-1280x720-50-444.yuv");
	printf("%d\n", reader.open());
	printf("%d\n", reader.readHeader());

	rows = reader.getNRows();
	cols = reader.getNCols();
	fps = reader.getFps();
	type = reader.getType();

	printf("%d, %d, %d, %d\n", cols, rows, fps, type);

	printf("%d\n", bs.open());

	YuvFrame frame1(rows, cols);

	IntraCoder::writeHeader(rows, cols, fps, type, quant, bs);

	while (reader.readFrame(frame1) == 0){
		printf("frame #%d\n",nFrames++);
		IntraCoder::encode(frame1, bs, quant);
	}

	bs.close();

	printf("Done! %d frames!\n",nFrames);

	nFrames = rows = cols = fps = type = 0;

	BitStream bs1 = BitStream(file, BitStream::READ);
	printf("%d\n", bs1.open());

	printf("%d\n", IntraCoder::readHeader(bs1, &rows, &cols, &fps, &type, &quant));
	YuvFrame frame2(YuvFrame::YUV444, rows, cols);

	//THIS!
	// frame2.get_write_yBuff_444();
	// frame2.get_write_uBuff_444();
	// frame2.get_write_vBuff_444();

	YuvDisplay display((char*)"YuvShow", fps, rows, cols);

	printf("%d, %d, %d, %d\n", cols, rows, fps, type);

	display.start();

	while (IntraCoder::decode(bs1, frame2, quant) == 0){
		printf("frame #%d\n",nFrames++);
		display.displayFrame(frame2);
	}

	bs1.close();

	// printf("%d\n", Golomb::decode(15, bs1));
	// printf("%d\n", Golomb::decode(16, bs1));

	return 0;
}