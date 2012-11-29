//==============================================================================
// DCTCoder.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __DCTCODER_H__
#define __DCTCODER_H__


//==============================================================================

#include "base.h"
#include "YuvFrame.h"
#include "BitStream.h"

//==============================================================================


class DCTCoder{
public:
	enum Type {
		LUMINANCE , // Y
		CROMINANCE , // U/Cb, V/Cr
	};


	/**
	 * Writes a bitsream header for a video.
	 */
	int static writeHeader(uint rows, uint cols, uint fps, uint type, uint quantization, BitStream& bs); //use quantization/100?

	int static encode(YuvFrame& frame, BitStream& bs, uint quantization = 100);

	/**
	 * Reads a bitstream header for a video.
	 */
	int static readHeader(BitStream& bs, uint *rows, uint *cols, uint *fps, uint *type, uint *quantization); 

	int static decode(BitStream& bs, YuvFrame& frame, uint quantization = 100);


private:
	void static dct(int* const values, float* dct); // computes the dct components for the signal (values)
	void static invdct(float* dct, int* values); // computes the inverse dct for the components (dct)
	void static quantize(float* dct, float factor, Type type, int* quantizedDct); //quantizes the DCT coefficients using the matrices defined by the jpeg standard, depending on the indicated type.
	void static dequantize(int* quantizedDct, float factor, Type type, float* dct); //dequantizes the quantized DCT coefficients using the matrices defined by the jpeg standard, depending on the indicated type.

};

//==============================================================================
#endif /* __DCTCODER_H__ */
