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

SFWriter::SFWriter(std::string _filename, SF_INFO& _info)
	: filename(_filename) {
		info.frames = _info.frames;
		info.samplerate = _info.samplerate;
		info.channels = _info.channels;
		info.format = _info.format;
		info.sections = _info.sections;
		info.seekable = _info.seekable;	
	}


SFWriter::~SFWriter() {
	close();
}

int SFWriter::open() {

	file = sf_open (filename.c_str(), SFM_WRITE, &info);
	if (file == NULL){
		fprintf(stderr, "Could not open file for writing: \"%s\"\n", filename.c_str());
		return -1;
	}
}

int SFWriter::writeFrame(short* frame){

	if (sf_writef_short(file, frame, 1) != 1){
		fprintf(stderr, "Error writing frames to the output:\n");
		return -1;
	}
}
int SFWriter::close(){
	return sf_close(file);
}

void SFWriter::printInfo(){
	printf("frames : %d\n", (int) info.frames);
	printf("samplerate : %d\n", info.samplerate);
	printf("channels : %d\n", info.channels);
	printf("format : %d\n", info.format);
	printf("sections : %d\n", info.sections);
	printf("seekable : %d\n\n", info.seekable);
}

// EOF =========================================================================
