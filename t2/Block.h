//==============================================================================
// Block.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __BLOCK_H__
#define __BLOCK_H__


//==============================================================================

#include "base.h"


//==============================================================================

class Block
{
public:
	Block  ( uint _nRows, uint _nCols );
	Block  ( uint size );
	~Block (  );

public:
	uint getnRows  ( void );
	uint getnCols  ( void );
	uchar* getBuff ( void );

private:
	uint nRows;
	uint nCols;

	uchar* buff;
};


//==============================================================================
#endif /* __BLOCK_H__ */
