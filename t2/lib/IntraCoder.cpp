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
#define QUANT_M 2


  //==============================================================================

int IntraCoder::writeHeader(uint rows, uint cols, uint fps, uint type, uint quantization, BitStream& bs){
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

	if ((err=Golomb::encode(QUANT_M, quantization, bs)) != 0){
		return err;
	}	

	return 0;
}

int IntraCoder::encode(YuvFrame& frame, BitStream& bs, uint quantization){
	//TODO: validate quantization

	int pY, pU, pV; //pixel values
	int predY, predU, predV; //predicted pixel values
	uint r, c;
	int err;
	int quant = quantization; //unsigned is magic
	int diff;

	for (r = 0; r < frame.getYRows(); r++){
		for (c = 0; c < frame.getYCols(); c++)
		{
			pY = frame.getYPixel(r, c);
			Predictor::predict(frame, r, c, 1, &predY, &predU, &predV);
			
			diff = (pY - predY)/quant;

			//encode difference
			if ((err = Golomb::encode(M, diff, bs)) != 0){
				return err;
			}

			if (quantization > 1){
				frame.putYPixel(r, c, predY + diff*quant);
			}
		}
	}

	for (r = 0; r < frame.getURows(); r++){
		for (c = 0; c < frame.getUCols(); c++)
		{
			pU = frame.getUPixel(r, c);

			Predictor::predict(frame, r, c, 1, &predY, &predU, &predV);

			diff = (pU - predU)/quant;
			//encode difference
			if ((err = Golomb::encode(M, diff, bs)) != 0){
				return err;
			}

			if (quantization > 1){
				frame.putUPixel(r, c, predU + diff*quant);
			}

		}
	}


	for (r = 0; r < frame.getVRows(); r++){
		for (c = 0; c < frame.getVCols(); c++)
		{
			pV = frame.getVPixel(r, c);

			Predictor::predict(frame, r, c, 1, &predY, &predU, &predV);

			diff = (pV - predV)/quant;
			//encode difference
			if ((err = Golomb::encode(M, diff, bs)) != 0){
				return err;
			}

			if (quantization > 1){
				frame.putVPixel(r, c, predV + diff*quant);
			}

		}
	}

	return 0;
}

int IntraCoder::readHeader(BitStream& bs, uint *rows, uint *cols, uint *fps, uint *type, uint *quantization){
	int err;

	int _rows, _cols, _fps, _type, _quantization;

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

	if ((err = Golomb::decode(QUANT_M, &_quantization, bs)) != 0){
			return err;
	}
	*quantization=_quantization;

	return 0;
}

int IntraCoder::decode(BitStream& bs, YuvFrame& frame, uint quantization){
	//TODO: validate quantization

	int dY, dU, dV; //difference values
	int predY = 0, predU = 0, predV = 0; //predicted pixel values
	int err;
	uint r, c;

	for (r = 0; r < frame.getYRows(); r++) {
		for (c = 0; c < frame.getYCols(); c++)
		{
			Predictor::predict(frame, r, c, 1, &predY, &predU, &predV);

			if ((err = Golomb::decode(M, &dY, bs)) != 0){
				return err;
			}

			frame.putYPixel(r,c, predY + dY * quantization);
			// frame.putYPixel(r,c, (predY + dY) * quantization);
		}
	}

	for (r = 0; r < frame.getURows(); r++) {
		for (c = 0; c < frame.getUCols(); c++)
		{
			Predictor::predict(frame, r, c, 1, &predY, &predU, &predV);

			if ((err = Golomb::decode(M, &dU, bs)) != 0){
				return err;
			}


			frame.putUPixel(r,c, predU + dU * quantization);
			// frame.putUPixel(r,c, (predU + dU) * quantization);
		}
	}


	for (r = 0; r < frame.getVRows(); r++) {
		for (c = 0; c < frame.getVCols(); c++)
		{
			Predictor::predict(frame, r, c, 1, &predY, &predU, &predV);

			if ((err = Golomb::decode(M, &dV, bs)) != 0){
				return err;
			}

			frame.putVPixel(r,c, predV + dV * quantization);
			// frame.putVPixel(r,c, (predV + dV) * quantization);
		}
	}

	return 0;
}


// EOF =========================================================================
