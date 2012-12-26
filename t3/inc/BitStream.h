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
#include <stdint.h>

//==============================================================================

class BitStream {
public:
	enum Type { READ, WRITE };

public:
	BitStream  ( std::string filename, Type t );
	~BitStream (  );

public:
	int writeBit  ( uint32_t bit );
	int writeBits ( uint32_t bits, uint nb );

	int readBit  ( uint32_t* bit );
	int readBits ( uint nb, uint32_t* bits );

	int open  ( void );
	int flush ( void );
	int close ( void );

private:
	std::string filename;
	FILE* fp;
	Type  type;

	uint8_t buffer;
	uint  pos;
};


//==============================================================================
#endif
