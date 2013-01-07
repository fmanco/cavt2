//==============================================================================
// SFReader.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "SFReader.h"
#include <sndfile.h>


//==============================================================================

SFReader::SFReader( std::string _filename )
	: filename(_filename)
{  }

SFReader::~SFReader (  )
{
	close();
}


//==============================================================================

int SFReader::open ( void )
{
	info.format = 0;
	file = sf_open(filename.c_str(), SFM_READ, &info);

	/// \todo Missing info validation

	return (file == NULL ? -1 : 0);
}

int SFReader::nextFrame ( int16_t frame[2] )
{
	int ret;

	ret = sf_readf_short(file, frame, 1);

	if (ret != 1) {
		close();
	}

	return (ret != 1 ? -1 : 0);
}

int SFReader::close ( void )
{
	return sf_close(file);
}

void SFReader::printInfo ( void )
{
	printf("frames : %d\n", (int) info.frames);
	printf("samplerate : %d\n", info.samplerate);
	printf("channels : %d\n", info.channels);
	printf("format : %d\n", info.format);
	printf("sections : %d\n", info.sections);
	printf("seekable : %d\n\n", info.seekable);
}

uint32_t SFReader::getNFrames    ( void ) { return (uint32_t) info.frames;     };
uint32_t SFReader::getSamplerate ( void ) { return (uint32_t) info.samplerate; };
uint32_t SFReader::getChannels   ( void ) { return (uint32_t) info.channels;   };
SF_INFO  SFReader::getInfo       ( void ) { return info; };


// EOF =========================================================================
