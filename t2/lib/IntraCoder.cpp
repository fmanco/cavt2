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

#define INTRA_BIT 0
#define M 5
#define ROW_M 100
#define COL_M 100
#define FPS_M 10
#define TYPE_M 400


  //==============================================================================

int IntraCoder::writeHeader(uint rows, uint cols, uint fps, uint type, BitStream& bs){
	int err;


	if ((err = Golomb::encode(ROW_M, rows, bs)) != 0){
		return err;
	}

	if ((err = Golomb::encode(COL_M, cols, bs)) != 0){
		return err;
	}

	if ((err = Golomb::encode(FPS_M, fps, bs)) != 0){
		return err;
	}

	if ((err=Golomb::encode(TYPE_M, type, bs)) != 0){
		return err;
	}

	return 0;
}

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

int IntraCoder::readHeader(BitStream& bs, uint *rows, uint *cols, uint *fps, uint *type){
	int err;

	int _rows, _cols, _fps, _type;

	if ((err = Golomb::decode(ROW_M, &_rows, bs)) != 0){
		return err;
	}
	*rows=_rows;

	if ((err = Golomb::decode(COL_M, &_cols, bs)) != 0){
		return err;
	}
	*cols=_cols;

	if ((err = Golomb::decode(FPS_M, &_fps, bs)) != 0){
		return err;
	}
	*fps=_fps;

	if ((err = Golomb::decode(TYPE_M, &_type, bs)) != 0){
		return err;
	}
	*type=_type;

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
