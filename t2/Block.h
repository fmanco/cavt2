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

class YuvFrame; // Forward declaration because of friendship


//==============================================================================

class Block
{
private:
friend class YuvFrame;

public:
	Block  ( uint _nRows, uint _nCols );
	Block  ( uint size );
	~Block (  );

public:
	uint getNRows  ( void ) const;
	uint getNCols  ( void ) const;
	uchar* getBuff ( void ) const;

private:
	uint nRows;
	uint nCols;

	uchar* buff;
};


//==============================================================================
#endif /* __BLOCK_H__ */
