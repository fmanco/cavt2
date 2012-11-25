//==============================================================================
// YuvWriter.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "YuvWriter.h"


//==============================================================================

YuvWriter::YuvWriter  ( char* _filename, uint _nRows, uint _nCols, uint _type, uint _fps )
	: inited(false), filename(_filename), fp(NULL), nRows(_nRows), nCols(_nCols), type(_type), fps(_fps)
{  }

YuvWriter::~YuvWriter (  )
{
	close();
}


//==============================================================================

int YuvWriter::open ( void )
{
	if (fp != NULL)
		return -1;

	fp = std::fopen(filename, "wb");

	if (fp == NULL)
		return -1;

	return 0;
}

int YuvWriter::writeHeader ( void )
{
	if (fp == NULL)
		return -1;

	if (fprintf(fp, "%u %u %u %u\n", nCols, nRows, fps, type) <= 0) {
		return -1;
	} else {
		inited = true;
		return 0;
	}
}

int YuvWriter::writeFrame ( YuvFrame& frame )
{
	if (fp == NULL || !inited) {
		return -1;
	}

	uchar* yBuff = NULL;
	uchar* uBuff = NULL;
	uchar* vBuff = NULL;

	uint nRows = frame.getNRows();
	uint nCols = frame.getNCols();

	uint size1 = 0;
	uint size2 = 0;

	switch (type) {
	case 444:
		yBuff = frame.get_read_yBuff_444();
		uBuff = frame.get_read_uBuff_444();
		vBuff = frame.get_read_vBuff_444();

		size1 = nRows * nCols;
		size2 = size1;
		break;

	case 422:
		yBuff = frame.get_read_yBuff_422();
		uBuff = frame.get_read_uBuff_422();
		vBuff = frame.get_read_vBuff_422();

		size1 = nRows * nCols;
		size2 = nRows * (nCols / 2);
		break;

	case 420:
		yBuff = frame.get_read_yBuff_420();
		uBuff = frame.get_read_uBuff_420();
		vBuff = frame.get_read_vBuff_420();

		size1 = nRows * nCols;
		size2 = (nRows / 2) * (nCols / 2);
		break;
	}

	if (fwrite(yBuff, sizeof(uchar), size1, fp) != size1) {
		return -1;
	}

	if (fwrite(uBuff, sizeof(uchar), size2, fp) != size2) {
		return -1;
	}

	if (fwrite(vBuff, sizeof(uchar), size2, fp) != size2) {
		return -1;
	}

	return 0;
}

int YuvWriter::close ( void )
{
	if (fp == NULL)
		return -1;

	fclose(fp);
	fp = NULL;

	return 0;
}


// EOF =========================================================================
