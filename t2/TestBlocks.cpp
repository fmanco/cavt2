//==============================================================================
// TestBlock.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "YuvFrame.h"
#include "Block.h"


//==============================================================================

int main ( int argc, char** argv )
{
	YuvFrame frame (YuvFrame::YUV420, 10, 10);
	Block    block(4, 4);

	uchar* fBuff;
	uchar* bBuff;

	fBuff = frame.get_write_uBuff_444();
	bBuff = block.getBuff();

	for (uint r = 0, count = 0; r < 10; r++) {
		for (uint c = 0; c < 10; c++) {
			fBuff[(r * 10) + c] = (10 * r) + c;
			count++;
		}
	}

	printf("\n");
	for (uint r = 0; r < 10; r++) {
		for (uint c = 0; c < 10; c++) {
			printf("%02hhd ", fBuff[(r * 10) + c]);
		}
		printf("\n");
	}

	frame.getUBlock(block, 0, 0);

	printf("\n");
	for (uint r = 0; r < 4; r++) {
		for (uint c = 0; c < 4; c++) {
			printf("%02hhd ", bBuff[(r * 4) + c]);
			bBuff[(r * 4) + c] = (10 * r) + c;
		}

		printf("\n");
	}

	frame.putUBlock(block, 3, 3);

	fBuff = frame.get_read_uBuff_444();
	printf("\n");
	for (uint r = 0; r < 10; r++) {
		for (uint c = 0; c < 10; c++) {
			printf("%02hhd ", fBuff[(r * 10) + c]);
		}
		printf("\n");
	}
}


// EOF =========================================================================
