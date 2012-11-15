//==============================================================================
// IntraCoder.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include <base.h>
#include <IntraCoder.h>


//==============================================================================


	void static IntraCoder::encode(YuvFrame& frame, BitStream& bs){
		for (r = 0; r < f.nRows; r++)
			for (c = 0; ...)
			{
				p1 = frame.getYPixel(r, c);
				p2 = f.preditor(r, c, tipo);
				e = p1 - p1;
				colomb.enc(e);
			}

	}

	void static IntraCoder::decode(BitStream& bs, YuvFrame& frame){

		for (linhas) {
			for (colunas)
				pr = preditor(...);
				e = Golomb::dec(m, bs)
				p1 =p2+e;
				yuv putYpixel(r,c,p1)
		}
	}


// EOF =========================================================================
