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

//TODO: 3 quantization factors!

class IntraCoder{
public:
	/**
	 * Writes a bitsream header for a video.
	 */
	int static writeHeader(uint rows, uint cols, uint fps, uint type, uint predMode, uint quantY, uint quantU, uint quantV, BitStream& bs);

	int static encode(YuvFrame& frame, BitStream& bs, uint mode = 0, uint quantY = 0, uint quantU = 0, uint quantV = 0);

	/**
	 * Reads a bitstream header for a video.
	 */
	int static readHeader(BitStream& bs, uint *rows, uint *cols, uint *fps, uint *type, uint *predMode, uint *quantY, uint *quantU, uint *quantV); 

	int static decode(BitStream& bs, YuvFrame& frame, uint mode = 0, uint quantY = 0, uint quantU = 0, uint quantV = 0);
};

//==============================================================================
#endif /* __INTRACODER_H__ */
