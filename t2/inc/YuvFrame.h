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
	enum Type {
		YUV444 ,
		YUV422 ,
		YUV420 ,
	};

public:
	YuvFrame  ( uint _nRows, uint _nCols );
	YuvFrame  ( Type _type, uint _nRows, uint _nCols );
	YuvFrame  ( int _type, uint _nRows, uint _nCols );
	YuvFrame  ( const YuvFrame& obj );
	~YuvFrame (  );

public:
	//====================
	// Basic Getters
	//
	Type getType   ( void ) const;
	void setType   ( Type _type );
	void setType   ( uint _type );

	uint getNRows  ( void ) const;
	uint getNCols  ( void ) const;

	uint getYRows  ( void ) const;
	uint getYCols  ( void ) const;
	uint getURows  ( void ) const;
	uint getUCols  ( void ) const;
	uint getVRows  ( void ) const;
	uint getVCols  ( void ) const;

	uint getYRows  ( YuvFrame::Type _type ) const;
	uint getYCols  ( YuvFrame::Type _type ) const;
	uint getURows  ( YuvFrame::Type _type ) const;
	uint getUCols  ( YuvFrame::Type _type ) const;
	uint getVRows  ( YuvFrame::Type _type ) const;
	uint getVCols  ( YuvFrame::Type _type ) const;

	//====================
	// Get and Put blocks and pixels
	//
	void getYBlock ( Block &b, uint r, uint c );
	void getUBlock ( Block &b, uint r, uint c );
	void getVBlock ( Block &b, uint r, uint c );
	void putYBlock ( const Block &b, uint r, uint c );
	void putUBlock ( const Block &b, uint r, uint c );
	void putVBlock ( const Block &b, uint r, uint c );

	uchar getYPixel ( uint r, uint c );
	uchar getUPixel ( uint r, uint c );
	uchar getVPixel ( uint r, uint c );
	void  putYPixel ( uint r, uint c, uchar pixel );
	void  putUPixel ( uint r, uint c, uchar pixel );
	void  putVPixel ( uint r, uint c, uchar pixel );

	//====================
	// Get buffers' pointers
	//
	uchar* get_read_yBuff ( void );
	uchar* get_read_uBuff ( void );
	uchar* get_read_vBuff ( void );

	uchar* get_read_yBuff ( YuvFrame::Type type );
	uchar* get_read_uBuff ( YuvFrame::Type type );
	uchar* get_read_vBuff ( YuvFrame::Type type );

	uchar* get_write_yBuff ( void );
	uchar* get_write_uBuff ( void );
	uchar* get_write_vBuff ( void );

	uchar* get_write_yBuff ( YuvFrame::Type type );
	uchar* get_write_uBuff ( YuvFrame::Type type );
	uchar* get_write_vBuff ( YuvFrame::Type type );


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

	void getBlock ( Block& blk , uint r, uint c,
	                uchar* frame, uint fRows, uint fCols );
	void putBlock ( uchar* fBuff, uint fRows, uint fCols,
	                const Block& blk, uint r, uint c );


private:
	// Type of the frame
	Type type;

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
