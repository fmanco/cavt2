//==============================================================================
// YuvWriter.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __YUVWRITER_H__
#define __YUVWRITER_H__


//==============================================================================

#include "base.h"
#include "YuvFrame.h"


//==============================================================================

class YuvWriter {
public:
	YuvWriter  ( char* filename, uint _nRows, uint _nCols, uint _type, uint _fps );
	~YuvWriter (  );

public:
	int open        ( void );
	int writeHeader ( void );           // TODO: Should this be automatic?
	int writeFrame  ( YuvFrame& frame );
	int close       ( void );

	// uint getNFrames ( void ) const { return nFrames; }
	uint getNRows   ( void ) const { return nRows; }
	uint getNCols   ( void ) const { return nCols; }
	uint getType    ( void ) const { return type; }
	uint getFps     ( void ) const { return fps; }

private:
	bool inited;

	char* filename;
	FILE* fp;

	// uint nFrames;
	uint nRows;
	uint nCols;
	uint type;
	uint fps;
};


//==============================================================================
#endif /* __YUVWRITER_H__ */
