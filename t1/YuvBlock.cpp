#include "YuvBlock.h"

YuvBlock::YuvBlock(int _nCols, int _nRows, unsigned char *_buffer) : 
	nCols(_nCols), nRows(_nRows), buffer(_buffer) {};
	
	
YuvBlock::~YuvBlock(){
}
