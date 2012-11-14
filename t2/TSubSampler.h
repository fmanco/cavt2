//==============================================================================
// TSubSampler.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __TSUBSAMPLER_H__
#define __TSUBSAMPLER_H__


//==============================================================================

#include "base.h"
#include "YuvFrame.h"
#include "YuvReader.h"


//==============================================================================

class TSubSampler {
public:
	TSubSampler  ( YuvReader& reader, uint rate );
	~TSubSampler (  );

public:
	int getNext ( YuvFrame& frame );

private:
	uint rate;
	YuvReader& reader;
};


//==============================================================================
#endif /* __TSUBSAMPLER_H__ */
