//==============================================================================
// YuvReader.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __YUVREADER_H__
#define __YUVREADER_H__


//==============================================================================

#include "base.h"
#include "YuvFrame.h"


//==============================================================================

class YuvReader {
public:
	YuvReader  ( char* filename );
	~YuvReader (  );

public:
	int open        ( void );
	int readHeader  ( void );            // TODO: Should this be automatic?
	int readFrame   ( YuvFrame& frame );
	int ignoreFrame ( void );
	int rewind      ( void );
	int close       ( void );

	uint getNFrames ( void ) const { return nFrames; }
	uint getNRows   ( void ) const { return nRows; }
	uint getNCols   ( void ) const { return nCols; }
	uint getType    ( void ) const { return type; }
	uint getFps     ( void ) const { return fps; }

private:
	FILE *fp;
	fpos_t video;

	uint nFrames;
	uint nRows;
	uint nCols;
	uint type;
	uint fps;
};


//==============================================================================
#endif /* __YUVREADER_H__ */
