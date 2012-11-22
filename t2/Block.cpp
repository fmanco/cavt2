//==============================================================================
// Block.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "Block.h"


//==============================================================================

Block::Block ( uint _nRows, uint _nCols )
	: nRows(_nRows), nCols(_nCols)
{
	buff = new uchar[nRows * nCols];
}

Block::Block ( uint size )
	: nRows(size), nCols(size)
{
	buff = new uchar[nRows * nCols];
}

Block::~Block (  )
{
	delete[] buff;
}


//==============================================================================

uint Block::getnRows ( void )
{
	return nRows;
}

uint Block::getnCols ( void )
{
	return nCols;
}

uchar* Block::getBuff ( void )
{
	return buff;
}


// EOF =========================================================================
