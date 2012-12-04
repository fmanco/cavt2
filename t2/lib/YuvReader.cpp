//==============================================================================
// YuvReader.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "YuvReader.h"
#include <cstdio>


//==============================================================================

YuvReader::YuvReader ( char* _filename )
{
	inited   = false;

	filename = _filename;
	fp       = NULL;

	nFrames = 0;
	nRows   = 0;
	nCols   = 0;
	type    = 0;
	fps     = 0;
}

YuvReader::~YuvReader (  )
{
	if (fp != NULL) {
		fclose(fp);
	}
}

//==============================================================================

int YuvReader::open ( void )
{
	fp = fopen(filename, "rb");

	if (fp == NULL) {
		return -1;
	}

	return 0;
}

int YuvReader::readHeader ( void )
{
	if (fp == NULL) {
		return -1;
	}

	if (fscanf(fp, "%u%u%u%u%*c]s", &nCols, &nRows, &fps, &type) != 4) {
		fclose(fp);
		fp = NULL;
		return -1;
	}

	fgetpos(fp, &video);

	inited = true;

	return 0;
}

int YuvReader::readFrame ( YuvFrame& frame )
{
	if (fp == NULL || !inited) {
		return -1;
	}

	frame.setType(type);

	uchar* yBuff = frame.get_write_yBuff();
	uchar* uBuff = frame.get_write_uBuff();
	uchar* vBuff = frame.get_write_vBuff();

	uint nRows = frame.getNRows();
	uint nCols = frame.getNCols();

	uint size1 = nRows * nCols;
	uint size2 = frame.getURows() * frame.getUCols();

	if (fread(yBuff, sizeof(uchar), size1, fp) != size1) {
		return -1;
	}

	if (fread(uBuff, sizeof(uchar), size2, fp) != size2) {
		return -1;
	}

	if (fread(vBuff, sizeof(uchar), size2, fp) != size2) {
		return -1;
	}

	return 0;
}

int YuvReader::ignoreFrame ( void )
{
	if (fp == NULL || !inited) {
		return -1;
	}

	uint size = 0;

	switch (type) {
	case 444:
		size = 3 * nRows * nCols;
		break;

	case 422:
		size = (nRows * nCols) + (2 * nRows * (nCols / 2));
		break;

	case 420:
		size = (nRows * nCols) + (2 * (nRows / 2) * (nCols / 2));
		break;
	}

	fseek(fp, size, SEEK_CUR);

	return 0;
}

int YuvReader::rewind ( void ) {
	if (fp == NULL || !inited) {
		return -1;
	}

	fsetpos(fp, &video);

	return 0;
}

int YuvReader::close ( void )
{
	if (fp) {
		fclose(fp);
		fp = NULL;
	}

	inited = false;

	return 0;
}


// EOF =========================================================================
