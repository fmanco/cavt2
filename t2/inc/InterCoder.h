//==============================================================================
// InterCoder.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __INTERCODER_H__
#define __INTERCODER_H__


//==============================================================================

#include "base.h"
#include "YuvFrame.h"
#include "BitStream.h"


//==============================================================================

class InterCoder
{
private:
	InterCoder  ( void );
	~InterCoder (  );

public:
	int static encode ( YuvFrame& currFrame, YuvFrame& prevFrame,
		                uint bsize, uint area,
		                uint qY, uint qU, uint qV,
	                    YuvFrame& quantFrame, BitStream& bs );


	int static decode ( YuvFrame& currFrame, YuvFrame& prevFrame,
	                    uint bsize, uint area,
	                    uint qY, uint qU, uint qV,
	                    BitStream& bs );

private:
	void static findBestYBlock ( YuvFrame& prevFrame, Block& blk, uint area, uint sr, uint sc, int& dr, int& dc );
	void static findBestUBlock ( YuvFrame& prevFrame, Block& blk, uint area, uint sr, uint sc, int& dr, int& dc );
	void static findBestVBlock ( YuvFrame& prevFrame, Block& blk, uint area, uint sr, uint sc, int& dr, int& dc );
	void static findBestBlock  ( uchar* fBuff, uint fRows, uint fCols, Block& blk, uint area, uint sr, uint sc, int& dr, int& dc );

	uint static Blockcmp ( uchar* fBuff, uint fRows, uint fCols, Block& blk, uint r, uint c );

	void static encodeDelta ( int dr, int dc, BitStream& bs );
	void static encodeDiffs ( Block& prevBlock, Block& currBlock,
	                          uint bsize, uint quantization,
	                          BitStream& bs );

	int static decodeDelta  ( int& dr, int& dc, BitStream& bs );
	int static decodeDiffs  ( Block& prevBlock, Block& currBlock,
	                          uint bsize, uint quantization,
	                          BitStream& bs );
};

//==============================================================================
#endif /* __INTERCODER_H__ */
