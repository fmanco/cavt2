//==============================================================================
// IntraCoder.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __INTRACODER_H__
#define __INTRACODER_H__

//==============================================================================

#include "base.h"
#include "YuvFrame.h"
#include "BitStream.h"

//==============================================================================

class IntraCoder{
public:
	int static encode(YuvFrame& frame, BitStream& bs);
	int static decode(BitStream& bs, YuvFrame& frame);
};

//==============================================================================
#endif /* __INTRACODER_H__ */
