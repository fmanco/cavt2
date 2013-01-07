//==============================================================================
// SFWriter.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __SFWRITER_H__
#define __SFWRITER_H__


//==============================================================================

#include "base.h"
#include <string>
#include <sndfile.h>
#include <stdint.h>


//==============================================================================

class SFWriter {
public:
	SFWriter  ( std::string _filename,
		uint32_t frames, uint32_t samplerate, uint32_t channels );
	~SFWriter (  );

public:
	int open  ( void );
	int close ( void );

	int writeFrame ( int16_t frame[2]);

	void printInfo ( void );

private:
	std::string filename;
	SNDFILE*    file;
	SF_INFO     info;
};


//==============================================================================
#endif /* __SFWRITER_H__ */
