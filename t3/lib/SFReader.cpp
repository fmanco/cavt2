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
	{}

SFReader::~SFReader() {
	close();
}

int SFReader::open(){
	info.format = 0;
	file = sf_open(filename.c_str(), SFM_READ, &info);

	if (file == NULL){
		fprintf(stderr, "Could not open file for reading: \"%s\"\n",
		  filename.c_str());
		sf_close(file);
		return -1;
	}

	return 0;
}

int SFReader::nextFrame(short* frame) {

	if (sf_readf_short(file, frame, 1) != 1){
		// fprintf(stderr, "Error: Reached end of file\n");
		// sf_close(soundFileIn);
		return -1;
	}

	return 0;
}

int SFReader::close(){
	return sf_close(file);
}

void SFReader::printInfo(){
	printf("frames : %d\n", (int) info.frames);
	printf("samplerate : %d\n", info.samplerate);
	printf("channels : %d\n", info.channels);
	printf("format : %d\n", info.format);
	printf("sections : %d\n", info.sections);
	printf("seekable : %d\n\n", info.seekable);
}


int SFReader::getNFrames() { return (int)info.frames; }
int SFReader::getSamplerate() { return info.samplerate; };
int SFReader::getChannels() { return info.channels; };
int SFReader::getFormat() { return info.format; };	
SF_INFO SFReader::getInfo() { return info; };	

// EOF =========================================================================
