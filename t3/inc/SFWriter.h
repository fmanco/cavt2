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

//==============================================================================


class SFWriter {
public:
	SFWriter(std::string filename, SF_INFO& info);
	~SFWriter();

public:
	int open();
	int writeFrame(short* frame);
	int close();
	void printInfo(); //TODO: duplicated?

private:
	std::string filename;
	SNDFILE* file;
	SF_INFO info;

};


//==============================================================================
#endif /* __SFWRITER_H__ */
