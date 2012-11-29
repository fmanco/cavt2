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
protected:
	HybCoder  ( BitStream& _bs );
	~HybCoder (  );

public:
	uint getNRows   ( void ) const { return nRows; }
	uint getNCols   ( void ) const { return nCols; }
	uint getType    ( void ) const { return type; }
	uint getFps     ( void ) const { return fps; }

protected:
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

protected:
	bool inited;

	uint nRows;
	uint nCols;
	uint type;
	uint fps;

	uint bsize;
	uint area;
	uint keyFrameT;

	int dr;
	int dc;

	uint counter;

	YuvFrame* currFrame;
	YuvFrame* prevFrame;

	Block* currBlock;
	Block* prevBlock;

	BitStream& bs;
};


class HybEncoder : public HybCoder {
public:
	HybEncoder  ( BitStream& _bs );
	~HybEncoder (  );

public:
	int init   ( uint nRows, uint nCols, uint fps, uint type,
	             uint _bsize, uint _area, uint _keyFrameT );
	int encode ( YuvFrame& frame );

private:
	int writeHeader ( uint nRows, uint nCols, uint type, uint fps,
	                  uint bsize, uint area, uint keyFrameT );
};


class HybDecoder : public HybCoder {
public:
	HybDecoder  ( BitStream& _bs );
	~HybDecoder (  );

public:
	int init   ( void );
	int decode ( YuvFrame& frame );

private:
	int readHeader ( uint* nRows, uint* nCols, uint* type, uint* fps,
	                 uint* bsize, uint* area, uint* keyFrameT );
};


//==============================================================================
#endif /* __HYBCODER_H__ */
