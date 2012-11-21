//==============================================================================
// YuvFrame.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __YUVFRAME_H__
#define __YUVFRAME_H__


//==============================================================================

#include "base.h"
#include "Block.h"
#include <cstdio>


//==============================================================================

class YuvFrame {
public:
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

	uchar getYPixel ( uint r, uint c ) const;
	uchar getUPixel ( uint r, uint c ) const;
	uchar getVPixel ( uint r, uint c ) const;
	uchar putYPixel ( uint r, uint c, uchar pixel );
	uchar putUPixel ( uint r, uint c, uchar pixel );
	uchar putVPixel ( uint r, uint c, uchar pixel );

	//====================
	// Get buffers' pointers
	//
	uchar* get_read_yBuff_444 ( void );
	uchar* get_read_uBuff_444 ( void );
	uchar* get_read_vBuff_444 ( void );

	uchar* get_read_yBuff_422 ( void );
	uchar* get_read_uBuff_422 ( void );
	uchar* get_read_vBuff_422 ( void );

	uchar* get_read_yBuff_420 ( void );
	uchar* get_read_uBuff_420 ( void );
	uchar* get_read_vBuff_420 ( void );

	uchar* get_write_yBuff_444 ( void );
	uchar* get_write_uBuff_444 ( void );
	uchar* get_write_vBuff_444 ( void );

	uchar* get_write_yBuff_422 ( void );
	uchar* get_write_uBuff_422 ( void );
	uchar* get_write_vBuff_422 ( void );

	uchar* get_write_yBuff_420 ( void );
	uchar* get_write_uBuff_420 ( void );
	uchar* get_write_vBuff_420 ( void );


private:
	// Internal buffer synchronization
	void read_444  ( void );
	void write_444 ( void );

	void read_422  ( void );
	void write_422 ( void );

	void read_420  ( void );
	void write_420 ( void );

	void aloc_444 ( void );
	void aloc_422 ( void );
	void aloc_420 ( void );

	void convert_444_422 ( void );
	void convert_444_420 ( void );

	void convert_422_444 ( void );
	void convert_422_420 ( void );

	void convert_420_444 ( void );
	void convert_420_422 ( void );

private:
	// Video resolution
	uint nRows;
	uint nCols;

	// Buffers
	uchar* yBuff;

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

	// Buffer synchronization info (isUpdated)
	bool sync_buff_444;
	bool sync_buff_422;
	bool sync_buff_420;
};


//==============================================================================
#endif /* __YUVFRAME_H__ */
