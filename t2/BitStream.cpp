//==============================================================================
// BitStream.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "BitStream.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>


//==============================================================================

BitStream::BitStream ( char* _filename, Type _type )
	: filename(_filename), fp(NULL), type(_type), buffer(0), pos(0)
{ }

BitStream::~BitStream (  )
{
	close();
}

int BitStream::open ( void )
{
	if (fp != NULL)
		return -1;

	if (type == READ) {
		fp = std::fopen(filename, "rb");
	} else {
		fp = std::fopen(filename, "wb");
	}

	if (fp == NULL)
		return -1;

	return 0;
}

int BitStream::flush ( void )
{
	if (fp == NULL)
		return -1;

	putc(buffer, fp);

	return 0;
}

int BitStream::close ( void )
{
	if (fp == NULL)
		return -1;

	if (type == WRITE)
		flush();

	std::fclose(fp);
	fp = NULL;

	return 0;
}


int BitStream::writeBit ( uchar bit )
{
//printf("a %d %d\n", type, WRITE);

	if (fp == NULL || type != WRITE)
		return -1;

//printf("b\n");
	if (pos == 0) {
		pos = 1;
	}

	if (pos == 0x100)
	{
		if (flush())
			return -1;

		pos    = 1;
		buffer = 0;
	}

	buffer = buffer | ((bit & 0x01) * pos);
	pos = pos << 1;

	return 0;
}

int BitStream::writeBits ( uchar bits, uint nb )
{
	while(nb--) {
		if (writeBit((bits >> nb) & 1))
			return -1;
	}

	return 0;
}

int BitStream::readBit ( uchar* bit )
{
	if(pos == 0x100 || pos == 0)
	{
		if((buffer = fgetc(fp)) == EOF)
			return EOF;

		pos = 1;
	}

	(*bit) = (buffer & pos) != 0 ? 1 : 0;
	pos = pos << 1;

	return 0;
}

int BitStream::readBits ( uint nb, uchar* bits )
{
	uchar bit;

	(*bits) = 0;

	while (nb--)
	{
		if(readBit(&bit) == EOF)
			return EOF;

		(*bits) = ((*bits) << 1) | bit;
	}

	return 0;
}


// EOF =========================================================================
