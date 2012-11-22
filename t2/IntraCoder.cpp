//==============================================================================
// IntraCoder.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "IntraCoder.h"
#include "YuvFrame.h"
#include "Predictor.h"
#include "Golomb.h"

#define M 5
  //==============================================================================


int IntraCoder::encode(YuvFrame& frame, BitStream& bs){

	int pY, pU, pV; //pixel values
	int predY, predU, predV; //predicted pixel values
	uint r, c;
	int err;

	for (r = 0; r < frame.getNRows(); r++){
		for (c = 0; c < frame.getNCols(); c++)
		{
			pY = frame.getYPixel(r, c);
			pU = frame.getUPixel(r, c);
			pV = frame.getVPixel(r, c);

			Predictor::predict(frame, r, c, 1, &predY, &predU, &predV);

			//encode difference
			if ((err = Golomb::encode(M, pY - predY, bs)) != 0){
				return err;
			}
			if ((err = Golomb::encode(M, pU - predU, bs)) != 0){
				return err;
			}
			if ((err = Golomb::encode(M, pV - predV, bs)) != 0){
				return err;
			}
		}
	}
	return 0;
}

int IntraCoder::decode(BitStream& bs, YuvFrame& frame){

	int dY, dU, dV; //difference values
	int predY = 0, predU = 0, predV = 0; //predicted pixel values
	int err;
	uint r, c;

	for (r = 0; r < frame.getNRows(); r++) {
		for (c = 0; c < frame.getNCols(); c++)
		{
			Predictor::predict(frame, r, c, 1, &predY, &predU, &predV);


			if ((err = Golomb::decode(M, &dY, bs)) != 0){
				return err;
			}

			if ((err = Golomb::decode(M, &dU, bs)) != 0){
				return err;
			}

			if ((err = Golomb::decode(M, &dV, bs)) != 0){
				return err;
			}

			// frame.putPixel(r,c, predY + dY, predU + dU, predV + dV);
			frame.putYPixel(r,c, predY + dY);
			frame.putUPixel(r,c, predU + dU);
			frame.putVPixel(r,c, predV + dV);
		}
	}
	return 0;
}


// EOF =========================================================================
