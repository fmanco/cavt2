//==============================================================================
// HybCoder.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "HybCoder.h"
#include "YuvFrame.h"
#include "BitStream.h"
#include "Block.h"
#include "Golomb.h"
#include "IntraCoder.h"
#include <cstdlib>
#include <climits>


//==============================================================================
#define DIFF_M   4
#define DELTA_M  32
#define ROW_M   100
#define COL_M   100
#define FPS_M    10
#define TYPE_M  400
#define BSIZE_M 400
#define AREA_M  400
#define KFT_M   400
#define QUANT_M 400


//==============================================================================

HybCoder::HybCoder ( BitStream& _bs )
	: inited(false),
	  nRows(0), nCols(0), type(0), fps(0),
	  bsize(0), area(0), keyFrameT(1), qY(1), qU(1), qV(1),
	  dr(0), dc(0), counter(0),
	  currFrame(NULL), prevFrame(NULL), quantFrame(NULL),
	  currBlock(NULL), prevBlock(NULL),
	  bs(_bs)
{ }

HybCoder::~HybCoder (  )
{
	if (prevFrame != NULL) {
		delete prevFrame;
	}

	if (quantFrame != NULL) {
		delete quantFrame;
	}

	if (currBlock != NULL) {
		delete currBlock;
	}

	if (prevBlock != NULL) {
		delete prevBlock;
	}
}

HybEncoder::HybEncoder ( BitStream& _bs )
	: HybCoder(_bs)
{
	// \todo Should assert bs is in write mode
}

HybEncoder::~HybEncoder ( )
{

}

HybDecoder::HybDecoder ( BitStream& _bs )
	: HybCoder(_bs)
{
	// \todo Should assert bs is in write mode
}

HybDecoder::~HybDecoder ( )
{

}


//==============================================================================

int HybEncoder::init ( uint _nRows, uint _nCols, uint _type, uint _fps,
	                   uint _bsize, uint _area, uint _keyFrameT,
	                   uint _qY, uint _qU, uint _qV )
{
	int err = -1;

	if (inited)
		return -1;

	nRows     = _nRows;
	nCols     = _nCols;
	type      = _type;
	fps       = _fps;

	bsize     = _bsize;
	area      = _area;
	keyFrameT = _keyFrameT;
	qY        = _qY;
	qU        = _qU;
	qV        = _qV;

	prevBlock = new Block(bsize);
	currBlock = new Block(bsize);

	err = writeHeader(nRows, nCols, type, fps, bsize, area, keyFrameT, qY, qU, qV);

	if (err == 0) {
		inited = true;
	}

	return err;
}

int HybEncoder::encode ( YuvFrame& frame )
{
	if (!inited)
		return -1;

	currFrame = &frame;
	if (counter % keyFrameT == 0) {
		intraEncode(frame);

		delete prevFrame;
		prevFrame = new YuvFrame(frame);
	} else {
		quantFrame = new YuvFrame(frame.getType(), frame.getNRows(), frame.getNCols());

		interEncode(frame);

		delete prevFrame;
		prevFrame  = quantFrame;
		quantFrame = NULL;
	}

	counter++;

	return 0;
}


int HybDecoder::init ( void )
{
	int err  = -1;

	err = readHeader(&nRows, &nCols, &type, &fps, &bsize, &area, &keyFrameT, &qY, &qU, &qV);

	if (err == 0) {
		prevBlock = new Block(bsize);
		currBlock = new Block(bsize);
		inited = true;
	}

	return err;
}

int HybDecoder::decode ( YuvFrame& frame )
{
	if (!inited)
		return -1;

	currFrame = &frame;

	if (counter % keyFrameT == 0) {
		if (intraDecode())
			return -1;
	} else {
		if (interDecode())
			return -1;
	}

	counter++;

	delete prevFrame;
	prevFrame = new YuvFrame(frame);

	return 0;
}


//==============================================================================

int HybEncoder::writeHeader ( uint nRows, uint nCols, uint type, uint fps,
	                          uint bsize, uint area, uint keyFrameT,
	                          uint qY, uint qU, uint qV )
{
	int err = 0;

	err = Golomb::encode(ROW_M, nRows, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::encode(COL_M, nCols, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::encode(TYPE_M, type, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::encode(FPS_M, fps, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::encode(BSIZE_M, bsize, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::encode(AREA_M, area, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::encode(KFT_M, keyFrameT, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::encode(QUANT_M, qY, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::encode(QUANT_M, qU, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::encode(QUANT_M, qV, bs);
	if (err != 0) {
		return err;
	}

	return 0;
}

int HybDecoder::readHeader ( uint* nRows, uint* nCols, uint* type, uint* fps,
	                         uint* bsize, uint* area, uint* keyFrameT,
	                         uint* qY, uint* qU, uint* qV )
{
	int err = 0;

	err = Golomb::decode(ROW_M, (int*) nRows, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::decode(COL_M, (int*) nCols, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::decode(TYPE_M, (int*) type, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::decode(FPS_M, (int*) fps, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::decode(BSIZE_M, (int*) bsize, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::decode(AREA_M, (int*) area, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::decode(KFT_M, (int*) keyFrameT, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::decode(QUANT_M, (int*) qY, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::decode(QUANT_M, (int*) qU, bs);
	if (err != 0) {
		return err;
	}

	err = Golomb::decode(QUANT_M, (int*) qV, bs);
	if (err != 0) {
		return err;
	}

	return 0;
}

//==============================================================================

int HybCoder::intraEncode ( YuvFrame& frame )
{
	if (bsize > frame.getNRows() || bsize > frame.getNCols())
		return -1; // \todo What to do here?

	IntraCoder::encode(frame, bs, 0, qY, qU, qV);

	return 0;
}

int HybCoder::interEncode ( YuvFrame& frame )
{
	if (prevFrame->getNRows() != frame.getNRows() ||
			prevFrame->getNCols() != frame.getNCols())
		return -1; // \todo What to do here?

	for (uint r = 0; r < frame.getYRows(); r += bsize) {
		for (uint c = 0; c < frame.getYCols(); c += bsize) {
			currFrame->getYBlock(*currBlock, r, c);
			findBestYBlock(r, c);
			prevFrame->getYBlock(*prevBlock, r + dr, c + dc);

			Golomb::encode(DELTA_M, dr, bs);
			Golomb::encode(DELTA_M, dc, bs);
			encodeDiff(qY);

			quantFrame->putYBlock(*currBlock, r, c);
		}
	}

	for (uint r = 0; r < frame.getURows(); r += bsize) {
		for (uint c = 0; c < frame.getUCols(); c += bsize) {
			currFrame->getUBlock(*currBlock, r, c);
			findBestUBlock(r, c);
			prevFrame->getUBlock(*prevBlock, r + dr, c + dc);

			Golomb::encode(DELTA_M, dr, bs);
			Golomb::encode(DELTA_M, dc, bs);
			encodeDiff(qU);

			quantFrame->putUBlock(*currBlock, r, c);
		}
	}

	for (uint r = 0; r < frame.getVRows(); r += bsize) {
		for (uint c = 0; c < frame.getVCols(); c += bsize) {
			currFrame->getVBlock(*currBlock, r, c);
			findBestVBlock(r, c);
			prevFrame->getVBlock(*prevBlock, r + dr, c + dc);

			Golomb::encode(DELTA_M, dr, bs);
			Golomb::encode(DELTA_M, dc, bs);
			encodeDiff(qV);

			quantFrame->putVBlock(*currBlock, r, c);
		}
	}

	return 0;
}

int HybCoder::intraDecode ( void )
{
	if (bsize > currFrame->getNRows() || bsize > currFrame->getNCols())
		return -1; // \todo What to do here?

	return IntraCoder::decode(bs, *currFrame, 0, qY, qU, qV);
}

int HybCoder::interDecode ( void )
{
	for (uint r = 0; r < currFrame->getYRows(); r += bsize) {
		for (uint c = 0; c < currFrame->getYCols(); c += bsize) {
			Golomb::decode(32, &dr, bs);
			Golomb::decode(32, &dc, bs);

			prevFrame->getYBlock(*prevBlock, r + dr, c + dc);

			if (decodeDiff(qY))
				return -1;

			currFrame->putYBlock(*currBlock, r, c);
		}
	}

	for (uint r = 0; r < currFrame->getURows(); r += bsize) {
		for (uint c = 0; c < currFrame->getUCols(); c += bsize) {
			Golomb::decode(32, &dr, bs);
			Golomb::decode(32, &dc, bs);

			prevFrame->getUBlock(*prevBlock, r + dr, c + dc);

			if (decodeDiff(qU))
				return -1;

			currFrame->putUBlock(*currBlock, r, c);
		}
	}

	for (uint r = 0; r < currFrame->getVRows(); r += bsize) {
		for (uint c = 0; c < currFrame->getVCols(); c += bsize) {
			Golomb::decode(32, &dr, bs);
			Golomb::decode(32, &dc, bs);

			prevFrame->getVBlock(*prevBlock, r + dr, c + dc);

			if (decodeDiff(qV))
				return -1;

			currFrame->putVBlock(*currBlock, r, c);
		}
	}

	return 0;
}


void HybCoder::findBestYBlock ( uint sr, uint sc )
{
	findBestBlock(prevFrame->get_read_yBuff(), prevFrame->getYRows(), prevFrame->getYCols(), *currBlock, sr, sc);
}

void HybCoder::findBestUBlock ( uint sr, uint sc )
{
	findBestBlock(prevFrame->get_read_uBuff(), prevFrame->getURows(), prevFrame->getUCols(), *currBlock, sr, sc);
}

void HybCoder::findBestVBlock ( uint sr, uint sc )
{
	findBestBlock(prevFrame->get_read_vBuff(), prevFrame->getVRows(), prevFrame->getVCols(), *currBlock, sr, sc);
}

void HybCoder::findBestBlock ( uchar* fBuff, uint fRows, uint fCols, Block& blk, uint sr, uint sc )
{
	dr = 0; // Reseting offset vector
	dc = 0; //

	uint err = 0;        // Auxiliar variable for block error
	uint errmin = UINT_MAX; // Starting with the max possible error

	uint ir  = ((sr < area) ? 0 : (sr - area));
	uint ic  = ((sc < area) ? 0 : (sc - area));
	uint er = ((sr + area) > fRows ? sr + 1 : (sr + area));
	uint ec = ((sc + area) > fCols ? sc + 1 : (sc + area));

	for (uint r = ir; r < er; r++) {
		for (uint c = ic; c < ec; c++) {
			err = Blockcmp(fBuff, fRows, fCols, blk, r, c);

			if (err <= errmin) {
				errmin = err;

				dr = r - sr;
				dc = c - sc;
			}
		}
	}
}


uint HybCoder::Blockcmp ( uchar* fBuff, uint fRows, uint fCols, Block& blk, uint r, uint c )
{
	uint br; // Block row index
	uint bc; // Block column index

	uint fr; // Frame row index
	uint fc; // Frame column index

	uint er = ((r + blk.nRows) > fRows ? (fRows) : (r + blk.nRows)); // Last frame row
	uint ec = ((c + blk.nCols) > fCols ? (fCols) : (c + blk.nCols)); // Last frame column

	uint err = 0;

	for (br = 0, fr = r; fr < er; br++, fr++) {
		for (bc = 0, fc = c; fc < ec; bc++, fc++) {
			err += abs(blk.buff[(br * blk.nCols) + bc] - fBuff[(fr * fCols) + fc]);
		}

		for (; bc < blk.nCols; bc++) {
			err += blk.buff[(br * blk.nCols) + bc];
		}
	}

	for (; br < blk.nRows; br++) {
		for (bc = 0; bc < blk.nCols; bc++) {
			err += blk.buff[(br * blk.nCols) + bc];
		}
	}

	return err / (blk.nRows * blk.nCols);
}

void HybCoder::encodeDiff ( uint quantization )
{
	if (quantization == 1) {
		for (uint i = 0; i < (bsize * bsize); i++) {
			Golomb::encode(DIFF_M, currBlock->buff[i] - prevBlock->buff[i], bs);
		}
	} else {
		for (uint i = 0; i < (bsize * bsize); i++) {
			int aux = (currBlock->buff[i] - prevBlock->buff[i]) / ((int)quantization);

			Golomb::encode(DIFF_M, aux, bs);

			currBlock->buff[i] = prevBlock->buff[i] + (aux * ((int)quantization));
		}
	}
}

int HybCoder::decodeDiff ( uint quantization )
{
	int aux;

	if (quantization == 1) {
		for (uint i = 0; i < (bsize * bsize); i++) {
			if (Golomb::decode(DIFF_M, &aux, bs)) {
				return -1;
			}

			currBlock->buff[i] = prevBlock->buff[i] + aux;
		}
	} else {
		for (uint i = 0; i < (bsize * bsize); i++) {
			if (Golomb::decode(DIFF_M, &aux, bs)) {
				return -1;
			}

			currBlock->buff[i] = prevBlock->buff[i] + (aux * ((int)quantization));
		}
	}

	return 0;
}


// EOF =========================================================================
