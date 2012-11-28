//==============================================================================
// HybCoder.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __HYBCODER_H__
#define __HYBCODER_H__


//==============================================================================

#include "base.h"
#include "YuvFrame.h"
#include "BitStream.h"
#include "Block.h"


//==============================================================================

class HybCoder
{
public:
	HybCoder  ( uint _bsize, uint _area, uint _keyFrameT, BitStream& _bs );
	~HybCoder (  );

public:
	void writeHeader ( uint nRows, uint nCols, uint fps, YuvFrame::Type type );

	int encode ( YuvFrame& frame );
	int decode ( YuvFrame& frame );

private:
	int intraEncode ( YuvFrame& frame );
	int interEncode ( YuvFrame& frame );
	int intraDecode ( void );
	int interDecode ( void );

	void findBestYBlock ( uint sr, uint sc );
	void findBestUBlock ( uint sr, uint sc );
	void findBestVBlock ( uint sr, uint sc );
	void findBestBlock  ( uchar* fBuff, uint nRows, uint nCols,
	                      Block& blk, uint sr, uint sc );

	uint Blockcmp ( uchar* fBuff, uint fRows, uint fCols,
	                Block& blk, uint r, uint c );

	void encodeDiff ( void );
	int  decodeDiff ( void );

private:
	uint bsize;
	uint area;
	uint keyFrameT;

	int dr;
	int dc;

	uint counter;

	YuvFrame* currFrame;
	YuvFrame* prevFrame;

	Block currBlock;
	Block prevBlock;

	BitStream& bs;
};


//==============================================================================
#endif /* __HYBCODER_H__ */
