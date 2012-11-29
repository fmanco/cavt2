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

HybCoder::HybCoder ( BitStream& _bs )
	: inited(false),
	  bsize(0), area(0), keyFrameT(1),
	  dr(0), dc(0), counter(0),
	  currFrame(NULL), prevFrame(NULL),
	  currBlock(NULL), prevBlock(NULL),
	  bs(_bs)
{ }

HybCoder::~HybCoder (  )
{
	if (prevFrame != NULL) {
		delete prevFrame;
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

int HybCoder::init ( uint nRows, uint nCols, uint fps, uint type,
	                   uint _bsize, uint _area, uint _keyFrameT )
{
	if (inited)
		return -1;

	bsize = _bsize;
	area = _area;
	keyFrameT = _keyFrameT;

	prevBlock = new Block(bsize);
	currBlock = new Block(bsize);

	inited = true;
//	return writeHeader();
	return 0;
}

int HybEncoder::encode ( YuvFrame& frame )
{
	if (!inited)
		return -1;

	if (counter % keyFrameT == 0) {
		intraEncode(frame);
	} else {
		interEncode(frame);
	}

	counter++;

	delete prevFrame;
	prevFrame = new YuvFrame(frame);

	return 0;
}

// int HybEncoder::init ( )
// {
// 	return readHeader();
// }

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

void HybEncoder::writeHeader ( void )
{
	// \todo implement
}

void HybDecoder::readHeader ( void )
{
	// \todo implement
}

//==============================================================================

int HybCoder::intraEncode ( YuvFrame& frame )
{
	if (bsize > frame.getNRows() || bsize > frame.getNCols())
		return -1; // \todo What to do here?

	IntraCoder::encode(frame, bs);

	return 0;
}

int HybCoder::interEncode ( YuvFrame& frame )
{
	if (prevFrame->getNRows() != frame.getNRows() ||
			prevFrame->getNCols() != frame.getNCols())
		return -1; // \todo What to do here?

	currFrame = &frame;
	for (uint r = 0; r < frame.getYRows(); r += bsize) {
		for (uint c = 0; c < frame.getYCols(); c += bsize) {
			currFrame->getYBlock(*currBlock, r, c);
			findBestYBlock(r, c);
			prevFrame->getYBlock(*prevBlock, r + dr, c + dc);
			encodeDiff();
		}
	}

	for (uint r = 0; r < frame.getURows(); r += bsize) {
		for (uint c = 0; c < frame.getUCols(); c += bsize) {
			currFrame->getUBlock(*currBlock, r, c);
			findBestUBlock(r, c);
			prevFrame->getUBlock(*prevBlock, r + dr, c + dc);
			encodeDiff();
		}
	}

	for (uint r = 0; r < frame.getVRows(); r += bsize) {
		for (uint c = 0; c < frame.getVCols(); c += bsize) {
			currFrame->getVBlock(*currBlock, r, c);
			findBestVBlock(r, c);
			prevFrame->getVBlock(*prevBlock, r + dr, c + dc);
			encodeDiff();
		}
	}

	return 0;
}

int HybCoder::intraDecode ( void )
{
	if (bsize > currFrame->getNRows() || bsize > currFrame->getNCols())
		return -1; // \todo What to do here?

	IntraCoder::decode(bs, *currFrame);

	return 0;
}

int HybCoder::interDecode ( void )
{
	for (uint r = 0; r < currFrame->getYRows(); r += bsize) {
		for (uint c = 0; c < currFrame->getYCols(); c += bsize) {
			Golomb::decode(32, &dr, bs);
			Golomb::decode(32, &dc, bs);

			prevFrame->getYBlock(*prevBlock, r + dr, c + dc);

			if (decodeDiff())
				return -1;

			currFrame->putYBlock(*currBlock, r, c);
		}
	}

	for (uint r = 0; r < currFrame->getURows(); r += bsize) {
		for (uint c = 0; c < currFrame->getUCols(); c += bsize) {
			Golomb::decode(32, &dr, bs);
			Golomb::decode(32, &dc, bs);

			prevFrame->getUBlock(*prevBlock, r + dr, c + dc);

			if (decodeDiff())
				return -1;

			currFrame->putUBlock(*currBlock, r, c);
		}
	}

	for (uint r = 0; r < currFrame->getVRows(); r += bsize) {
		for (uint c = 0; c < currFrame->getVCols(); c += bsize) {
			Golomb::decode(32, &dr, bs);
			Golomb::decode(32, &dc, bs);

			prevFrame->getVBlock(*prevBlock, r + dr, c + dc);

			if (decodeDiff())
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

	uint r  = ((sr < area) ? 0 : (sr - area));
	uint c  = ((sc < area) ? 0 : (sc - area));
	uint er = ((sr + area) > fRows ? sr + 1 : (sr + area));
	uint ec = ((sc + area) > fCols ? sc + 1 : (sc + area));

	for (; r < er; r++) {
		for (; c < ec; c++) {
			err = Blockcmp(fBuff, fRows, fCols, blk, r, c);

			if (err < errmin) {
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
		}
	}

	for (; br < blk.nRows; br++) {
		for (bc = 0; bc < blk.nCols; bc++) {
			err += blk.buff[(br * blk.nCols) + bc];
		}
	}

	return err / (blk.nRows * blk.nCols);
}

void HybCoder::encodeDiff ( void )
{
	// \todo Is it necessary 32 bits for dr/dc?
	Golomb::encode(32, dr, bs);
	Golomb::encode(32, dc, bs);

	for (uint i = 0; i < (bsize * bsize); i++) {
		Golomb::encode(5, currBlock->buff[i] - prevBlock->buff[i], bs);
	}
}


int HybCoder::decodeDiff ( void )
{
	int aux;

	for (uint i = 0; i < (bsize * bsize); i++) {
		if (Golomb::decode(5, &aux, bs)) {
			return -1;
		}

		currBlock->buff[i] = prevBlock->buff[i] + aux;
	}

	return 0;
}


// EOF =========================================================================
