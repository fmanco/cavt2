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
#define MODE_M 2


  //==============================================================================

int IntraCoder::writeHeader(uint rows, uint cols, uint fps, uint type, uint predMode, uint quantY, uint quantU, uint quantV, BitStream& bs){	
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

	if ((err=Golomb::encode(MODE_M, predMode, bs)) != 0){
		return err;
	}

	if ((err=Golomb::encode(QUANT_M, quantY, bs)) != 0){
		return err;
	}	

	if ((err=Golomb::encode(QUANT_M, quantU, bs)) != 0){
		return err;
	}	

	if ((err=Golomb::encode(QUANT_M, quantV, bs)) != 0){
		return err;
	}	

	return 0;
}

int IntraCoder::encode(YuvFrame& frame, BitStream& bs, uint mode, uint quantY = 1, uint quantU = 1, uint quantV = 1){
	//TODO: validate quantization

	int pY, pU, pV; //pixel values
	int predY, predU, predV; //predicted pixel values
	uint r, c;
	int err;
	
	int qY = (int) quantY;
	int qU = (int) quantU;
	int qV = (int) quantV;

	int diff;

	for (r = 0; r < frame.getYRows(); r++){
		for (c = 0; c < frame.getYCols(); c++)
		{
			pY = frame.getYPixel(r, c);
			predY = Predictor::predictY(frame, r, c, mode);
			
			diff = (pY - predY)/qY;

			//encode difference
			if ((err = Golomb::encode(M, diff, bs)) != 0){
				return err;
			}

			if (quantY > 1){
				frame.putYPixel(r, c, predY + diff*qY);
			}
		}
	}

	for (r = 0; r < frame.getURows(); r++){
		for (c = 0; c < frame.getUCols(); c++)
		{
			pU = frame.getUPixel(r, c);

			predU = Predictor::predictU(frame, r, c, mode);

			diff = (pU - predU)/qU;
			//encode difference
			if ((err = Golomb::encode(M, diff, bs)) != 0){
				return err;
			}

			if (quantU > 1){
				frame.putUPixel(r, c, predU + diff*qU);
			}

		}
	}


	for (r = 0; r < frame.getVRows(); r++){
		for (c = 0; c < frame.getVCols(); c++)
		{
			pV = frame.getVPixel(r, c);

			predV = Predictor::predictV(frame, r, c, mode);

			diff = (pV - predV)/qV;
			//encode difference
			if ((err = Golomb::encode(M, diff, bs)) != 0){
				return err;
			}

			if (quantV > 1){
				frame.putVPixel(r, c, predV + diff*qV);
			}

		}
	}

	return 0;
}

int IntraCoder::readHeader(BitStream& bs, uint *rows, uint *cols, uint *fps, uint *type, uint* predMode, uint *quantY, uint *quantU, uint *quantV){
	int err;

	int _rows, _cols, _fps, _type, _predMode, _quantY, _quantU, _quantV;

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

	if ((err = Golomb::decode(MODE_M, &_predMode, bs)) != 0){
			return err;
	}
	*predMode=_predMode;

	if ((err = Golomb::decode(QUANT_M, &_quantY, bs)) != 0){
			return err;
	}
	*quantY=_quantY;

	if ((err = Golomb::decode(QUANT_M, &_quantU, bs)) != 0){
			return err;
	}
	*quantU=_quantU;

	if ((err = Golomb::decode(QUANT_M, &_quantV, bs)) != 0){
			return err;
	}
	*quantV=_quantV;


	return 0;
}

int IntraCoder::decode(BitStream& bs, YuvFrame& frame, uint mode, uint quantY = 1, uint quantU = 1, uint quantV = 1){
	//TODO: validate quantization

	int dY, dU, dV; //difference values
	int predY = 0, predU = 0, predV = 0; //predicted pixel values
	int err;
	uint r, c;

	for (r = 0; r < frame.getYRows(); r++) {
		for (c = 0; c < frame.getYCols(); c++)
		{
			predY = Predictor::predictY(frame, r, c, mode);

			if ((err = Golomb::decode(M, &dY, bs)) != 0){
				return err;
			}

			frame.putYPixel(r,c, predY + dY * quantY);
		}
	}

	for (r = 0; r < frame.getURows(); r++) {
		for (c = 0; c < frame.getUCols(); c++)
		{
			predU = Predictor::predictU(frame, r, c, mode);

			if ((err = Golomb::decode(M, &dU, bs)) != 0){
				return err;
			}


			frame.putUPixel(r,c, predU + dU * quantU);
		}
	}


	for (r = 0; r < frame.getVRows(); r++) {
		for (c = 0; c < frame.getVCols(); c++)
		{
			predV = Predictor::predictV(frame, r, c, mode);

			if ((err = Golomb::decode(M, &dV, bs)) != 0){
				return err;
			}

			frame.putVPixel(r,c, predV + dV * quantV);
		}
	}

	return 0;
}


// EOF =========================================================================
