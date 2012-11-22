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
	/**
	 * Writes a bitsream header for a video.
	 */
	int static writeHeader(uint rows, uint cols, uint fps, uint type, BitStream& bs); 
	int static encode(YuvFrame& frame, BitStream& bs);

	/**
	 * Reads a bitstream header for a video.
	 */
	int static readHeader(BitStream& bs, uint *rows, uint *cols, uint *fps, uint *type); 
	int static decode(BitStream& bs, YuvFrame& frame);
};

//==============================================================================
#endif /* __INTRACODER_H__ */
