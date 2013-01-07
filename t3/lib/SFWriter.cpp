//==============================================================================
// SFWriter.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "SFWriter.h"
#include <string>
#include <sndfile.h>


//==============================================================================

SFWriter::SFWriter ( std::string _filename,
	                 uint32_t frames, uint32_t samplerate, uint32_t channels )
	: filename(_filename), file(NULL)
{
	info.frames     = frames;
	info.samplerate = samplerate;
	info.channels   = channels;
	info.format     = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
	info.sections   = 1;
	info.seekable   = 1;
}

SFWriter::~SFWriter (  )
{
	close();
}


//==============================================================================

int SFWriter::open ( void )
{
	file = sf_open(filename.c_str(), SFM_WRITE, &info);

	return (file == NULL ? -1 : 0);
}

int SFWriter::writeFrame ( int16_t frame[2] )
{
	return (sf_writef_short(file, frame, 1) != 1 ? -1 : 0);
}

int SFWriter::close ( void )
{
	int err = sf_close(file);

	file = NULL;

	return err;
}

void SFWriter::printInfo ( void )
{
	printf("frames : %d\n", (int) info.frames);
	printf("samplerate : %d\n", info.samplerate);
	printf("channels : %d\n", info.channels);
	printf("format : %d\n", info.format);
	printf("sections : %d\n", info.sections);
	printf("seekable : %d\n\n", info.seekable);
}


// EOF =========================================================================
