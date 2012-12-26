//==============================================================================
// BitStream.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef _BITSTREAM_H_
#define _BITSTREAM_H_


//==============================================================================

#include "base.h"
#include <string>
#include <cstdio>


//==============================================================================

class BitStream {
public:
	enum Type { READ, WRITE };

public:
	BitStream  ( std::string filename, Type t );
	~BitStream (  );

public:
	int writeBit  ( uchar bit );
	int writeBits ( uchar bits, uint nb );

	int readBit  ( uchar* bit );
	int readBits ( uint nb, uchar* bits );

	int open  ( void );
	int flush ( void );
	int close ( void );

private:
	std::string filename;
	FILE* fp;
	Type  type;

	uchar buffer;
	uint  pos;
};


//==============================================================================
#endif
