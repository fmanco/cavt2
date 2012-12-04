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
#include "InterCoder.h"


//==============================================================================
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

	return InterCoder::encode(frame, *prevFrame, bsize, area, qY, qU, qV, *quantFrame, bs);
}

int HybCoder::intraDecode ( void )
{
	if (bsize > currFrame->getNRows() || bsize > currFrame->getNCols())
		return -1; // \todo What to do here?

	return IntraCoder::decode(bs, *currFrame, 0, qY, qU, qV);
}

int HybCoder::interDecode ( void )
{
	return InterCoder::decode(*currFrame, *prevFrame, bsize, area, qY, qU, qV, bs);
}


// EOF =========================================================================
