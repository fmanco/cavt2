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
	YuvFrame frame(90, 80);
	Block    block(13, 9);
	uchar*   fBuff;
	uchar*   bBuff;
	uint count;

	fBuff = frame.get_write_yBuff_444();
	bBuff = block.getBuff();

	count = 0;
	for (uint r = 0; r < 90; r++) {
		for (uint c = 0; c < 80; c++) {
			fBuff[(r * 80) + c] = count;
			count++;
		}
		count = 0;
	}

	frame.getYBlock(block, 2, 2);

	for (uint r = 0; r < 13; r++) {
		for (uint c = 0; c < 9; c++) {
			printf("%02hhd ", bBuff[(r * 9) + c]);
			bBuff[(r * 9) + c] = 55;
		}

		printf("\n");
	}

	frame.putYBlock(block, 76, 60);

	printf("\n\n");

	for (uint r = 70; r < 90; r++) {
		for (uint c = 60; c < 80; c++) {
			printf("%02hhd ", fBuff[(r * 80) + c]);
		}
		printf("\n");
	}
}


// EOF =========================================================================
