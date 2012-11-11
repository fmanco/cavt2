//==============================================================================
// YuvFrame.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __YUVFRAME_H__
#define __YUVFRAME_H__


//==============================================================================

#include <base.h>
#include "Block.h"


//==============================================================================

class YuvFrame {
	YuvFrame  ( uint _nRows, uint _nCols );
	~YuvFrame (  );

public:
	//====================
	// Basic Getters
	//
	uint getNRows  ( void ) const { return nRows; }
	uint getNCols  ( void ) const { return nCols; }

	//====================
	// Get and Put blocks and pixels in YUV444 mode
	//
	void getYBlock ( Block &b, uint r, uint c ) const;
	void getUBlock ( Block &b, uint r, uint c ) const;
	void getVBlock ( Block &b, uint r, uint c ) const;
	void putYBlock ( const Block &b, uint r, uint c );
	void putUBlock ( const Block &b, uint r, uint c );
	void putVBlock ( const Block &b, uint r, uint c );

	uchar getYPixel ( uint x, uint y ) const;
	uchar getUPixel ( uint x, uint y ) const;
	uchar getVPixel ( uint x, uint y ) const;

	//====================
	// Get buffers' pointers
	//
	uchar* getBuff_444  ( void );
	uchar* getyBuff_444 ( void );
	uchar* getuBuff_444 ( void );
	uchar* getvBuff_444 ( void );

	uchar* getBuff_422  ( void );
	uchar* getyBuff_422 ( void );
	uchar* getuBuff_422 ( void );
	uchar* getvBuff_422 ( void );

	uchar* getBuff_420  ( void );
	uchar* getyBuff_420 ( void );
	uchar* getuBuff_420 ( void );
	uchar* getvBuff_420 ( void );


private:
	// Internal buffer synchronization
	void read_444  ( void );
	void write_444 ( void );

	void read_422  ( void );
	void write_422 ( void );

	void read_420  ( void );
	void write_420 ( void );

private:
	// Video resolution
	uint nRows;
	uint nCols;

	// Buffers
	uchar* buff_444;
	uchar* yBuff_444;
	uchar* uBuff_444;
	uchar* vBuff_444;

	uchar* buff_422;
	uchar* yBuff_422;
	uchar* uBuff_422;
	uchar* vBuff_422;

	uchar* buff_420;
	uchar* yBuff_420;
	uchar* uBuff_420;
	uchar* vBuff_420;

	// Buffer synchronization info
	uint buffer_444;
	uint buffer_422;
	uint buffer_420;
};


//==============================================================================
#endif /* __YUVFRAME_H__ */
