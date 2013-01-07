//==============================================================================
// SFReader.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __SFREADER_H__
#define __SFREADER_H__


//==============================================================================

#include "base.h"
#include <sndfile.h>
#include <string>
#include <stdint.h>

//==============================================================================


class SFReader {
public:
	SFReader  ( std::string _filename );
	~SFReader (  );

public:
	int open      ( void );
	int nextFrame ( int16_t frame[2] );
	int close     ( void );

	void printInfo ( void );

public:
	uint32_t getNFrames    ( void );
	uint32_t getSamplerate ( void );
	uint32_t getChannels   ( void );
	SF_INFO  getInfo       ( void );

private:
	std::string filename;
	SNDFILE*    file;
	SF_INFO     info;
};


//==============================================================================
#endif /* __SFREADER_H__ */
