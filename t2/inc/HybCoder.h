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
	int intraEncode ( void );
	int interEncode ( void );
	int intraDecode ( void );
	int interDecode ( void );

protected:
	bool inited;

	uint nRows;
	uint nCols;
	uint type;
	uint fps;

	uint bsize;
	uint area;
	uint keyFrameT;
	uint qY;
	uint qU;
	uint qV;

	int dr;
	int dc;

	uint counter;

	YuvFrame* currFrame;
	YuvFrame* prevFrame;
	YuvFrame* quantFrame;

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
	             uint _bsize, uint _area, uint _keyFrameT,
	             uint _qY = 1, uint _qU = 1, uint _qV = 1 );
	int encode ( YuvFrame& frame );

private:
	int writeHeader ( uint nRows, uint nCols, uint type, uint fps,
	                  uint bsize, uint area, uint keyFrameT,
	                  uint qY, uint qU, uint qV );
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
	                 uint* bsize, uint* area, uint* keyFrameT,
	                 uint* qY, uint* qU, uint* qV );
};


//==============================================================================
#endif /* __HYBCODER_H__ */
