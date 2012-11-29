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
	int static writeHeader(uint rows, uint cols, uint fps, uint type, uint predMode, uint quantY, uint quantU, uint quantV, BitStream& bs); //add mode!?!?!

	int static encode(YuvFrame& frame, BitStream& bs, uint mode, uint quantY, uint quantU, uint quantV);

	/**
	 * Reads a bitstream header for a video.
	 */
	int static readHeader(BitStream& bs, uint *rows, uint *cols, uint *fps, uint *type, uint *predMode, uint *quantY, uint *quantU, uint *quantV); 

	int static decode(BitStream& bs, YuvFrame& frame, uint mode, uint quantY, uint quantU, uint quantV);
};

//==============================================================================
#endif /* __INTRACODER_H__ */
