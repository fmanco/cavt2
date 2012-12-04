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

	uchar* yBuff = frame.get_write_yBuff();
	uchar* uBuff = frame.get_write_uBuff();
	uchar* vBuff = frame.get_write_vBuff();

	uint nRows = frame.getNRows();
	uint nCols = frame.getNCols();

	uint size1 = nRows * nCols;
	uint size2 = frame.getURows() * frame.getUCols();

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

	fputc(0x0a, fp);

	fclose(fp);
	fp = NULL;

	return 0;
}


// EOF =========================================================================
