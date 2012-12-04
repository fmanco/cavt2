#include <cstdio>
#include "Golomb.h"
#include "BitStream.h"
#include "YuvReader.h"
#include "IntraCoder.h"
#include "YuvDisplay.h"
int main( int argc, char** argv ){

	if (argc != 2) {
		printf("USAGE: %s <input>\n", argv[0]);
		return -1;
	}

	char* file = (char *)"file";
	int nFrames;
	uint rows, cols, fps, type;
	uint qY = 8, qU = 16, qV = 16;
	uint mode = 7;

	BitStream bs = BitStream(file, BitStream::WRITE);

	YuvReader reader(argv[1]);
	printf("%d\n", reader.open());
	printf("%d\n", reader.readHeader());

	rows = reader.getNRows();
	cols = reader.getNCols();
	fps = reader.getFps();
	type = reader.getType();

	printf("%d, %d, %d, %d, %d, %d, %d, %d\n", cols, rows, fps, type, mode, qY, qU, qV);

	printf("%d\n", bs.open());

	YuvFrame frame1(type, rows, cols);

	IntraCoder::writeHeader(rows, cols, fps, type, mode, qY, qU, qV, bs);

	while (reader.readFrame(frame1) == 0){
		printf("frame #%d\n",nFrames++);
		IntraCoder::encode(frame1, bs, mode, qY, qU, qV);
	}

	bs.close();

	printf("Done! %d frames!\n",nFrames);

	nFrames = rows = cols = fps = type = qY = qU = qV = mode = 0;

	BitStream bs1 = BitStream(file, BitStream::READ);
	printf("%d\n", bs1.open());

	printf("%d\n", IntraCoder::readHeader(bs1, &rows, &cols, &fps, &type, &mode, &qY, &qU, &qV));
	YuvFrame frame2(type, rows, cols);

	YuvDisplay display((char*)"YuvShow", fps, rows, cols);
	printf("%d, %d, %d, %d, %d, %d, %d, %d\n", cols, rows, fps, type, mode, qY, qU, qV);

	display.start();

	while (IntraCoder::decode(bs1, frame2, mode, qY, qU, qV) == 0){
		printf("frame #%d\n",nFrames++);
		display.displayFrame(frame2);
	}

	bs1.close();

	return 0;
}