//==============================================================================
// BitStream.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "BitStream.h"
#include <string>
#include <cstdio>
#include <stdint.h>


//==============================================================================

BitStream::BitStream ( std::string _filename, Type _type )
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
		fp = std::fopen(filename.c_str(), "rb");
	} else {
		fp = std::fopen(filename.c_str(), "wb");
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


int BitStream::writeBit ( uint8_t bit )
{
	if (fp == NULL || type != WRITE)
		return -1;

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

int BitStream::writeBits ( uint8_t bits, uint nb )
{
	if (fp == NULL || type != WRITE)
		return -1;

	while(nb--) {
		if (writeBit((bits >> nb) & 1))
			return -1;
	}

	return 0;
}

int BitStream::readBit ( uint8_t* bit )
{
	int temp;
	if (fp == NULL || type != READ)
		return -1;

	if(pos == 0x100 || pos == 0)
	{
		if((temp = fgetc(fp)) == EOF)
			return EOF;

		buffer = (char)temp;
		pos = 1;
	}

	(*bit) = (buffer & pos) != 0 ? 1 : 0;
	pos = pos << 1;

	return 0;
}

int BitStream::readBits ( uint nb, uint8_t* bits )
{
	if (fp == NULL || type != READ)
		return -1;

	uint8_t bit;

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
