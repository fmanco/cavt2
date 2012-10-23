#ifndef _YUVBLOCK_H_
#define _YUVBLOCK_H_

#include <stdio.h>

class YuvBlock {

public:
	YuvBlock(int _nCols, int _nRows, unsigned char *_buffer);
	
public:
	int nCols;
	int nRows;
	unsigned char *buffer;

};

#endif
