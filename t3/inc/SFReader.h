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
#include <string>
#include <cstdio>
#include <sndfile.h>

//==============================================================================


class SFReader {
public:
	SFReader( std::string filename );
	~SFReader();
public:
	int open();
	int nextFrame(short* frame);
	int close();
	void printInfo();

public:
	int getNFrames();
	int getSamplerate();
	int getChannels();
	int getFormat();
	SF_INFO getInfo();


private:
	std::string filename;
	SNDFILE* file;
	SF_INFO info;

};


//==============================================================================
#endif /* __SFREADER_H__ */
