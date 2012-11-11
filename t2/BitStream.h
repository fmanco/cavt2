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
#include <stdint.h>
#include <stdio.h>


//==============================================================================

class BitStream {
public:
	enum Type { READ, WRITE };

public:
	BitStream  ( char* filename, Type t );
	~BitStream (  );

public:
	void writeBit  ( uint8_t bit );
	void writeBits ( uint8_t bits, uint nb );

	uint8_t readBit  ( uint pos );
	uint8_t readBits ( uint pos, uint nb );

	void flush ( void );
	void close ( void );

private:
	FILE*    fp;
	Type     type;

	uint8_t* buffer;
	uint     pos;
};


//==============================================================================
#endif
