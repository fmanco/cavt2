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

HybCoder::HybCoder ( uint _bsize, uint _area, uint _keyFrameT, BitStream& _bs )
	: bsize(_bsize), area(_area), keyFrameT(_keyFrameT),
	  dr(0), dc(0), counter(0), currFrame(NULL), prevFrame(NULL),
	  currBlock(bsize), prevBlock(bsize), bs(_bs)
{ }

HybCoder::~HybCoder (  )
{
	if (prevFrame != NULL) {
		delete prevFrame;
	}
}


//==============================================================================

// void HybCoder::writeHeader ()
// {
// 	// \todo Implement this
// }

int HybCoder::encode ( YuvFrame& frame )
{
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

int HybCoder::decode ( YuvFrame& frame )
{
	if (counter % keyFrameT == 0) {
		if (intraDecode(frame))
			return -1;
	} else {
		if (interDecode(frame))
			return -1;
	}

	counter++;

	delete prevFrame;
	prevFrame = new YuvFrame(frame);

	return 0;
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
// printf(">>1\n");
	for (uint r = 0; r < frame.getYRows(); r += bsize) {
		for (uint c = 0; c < frame.getYCols(); c += bsize) {
// printf(">>1-1  %u\n", c);
			currFrame->getYBlock(currBlock, r, c);
// printf(">>1-2\n");
			findBestYBlock(r, c);
// printf(">>1-3\n");
			prevFrame->getYBlock(prevBlock, r + dr, c + dc);
// printf(">>1-4\n");
			encodeDiff();
// printf(">>1-5\n");
		}
	}

// printf(">>2\n");
	for (uint r = 0; r < frame.getURows(); r += bsize) {
		for (uint c = 0; c < frame.getUCols(); c += bsize) {
			currFrame->getUBlock(currBlock, r, c);
			findBestUBlock(r, c);
			prevFrame->getUBlock(prevBlock, r + dr, c + dc);
			encodeDiff();
		}
	}

// printf(">>3\n");
	for (uint r = 0; r < frame.getVRows(); r += bsize) {
		for (uint c = 0; c < frame.getVCols(); c += bsize) {
			currFrame->getVBlock(currBlock, r, c);
			findBestVBlock(r, c);
			prevFrame->getVBlock(prevBlock, r + dr, c + dc);
			encodeDiff();
		}
	}


// printf(">>4\n");
	return 0;
}

int HybCoder::intraDecode ( YuvFrame& frame )
{
	if (bsize > frame.getNRows() || bsize > frame.getNCols())
		return -1; // \todo What to do here?

	IntraCoder::decode(bs, frame);

	return 0;
}

int HybCoder::interDecode ( YuvFrame& frame )
{
	for (uint r = 0; r < frame.getYRows(); r += bsize) {
		for (uint c = 0; c < frame.getYCols(); c += bsize) {
			Golomb::decode(32, &dr, bs);
			Golomb::decode(32, &dc, bs);

			prevFrame->getYBlock(prevBlock, r + dr, c + dc);

			if (decodeDiff())
				return -1;

			currFrame->putYBlock(currBlock, r, c);
		}
	}

	for (uint r = 0; r < frame.getURows(); r += bsize) {
		for (uint c = 0; c < frame.getUCols(); c += bsize) {
			Golomb::decode(32, &dr, bs);
			Golomb::decode(32, &dc, bs);

			prevFrame->getUBlock(prevBlock, r + dr, c + dc);

			if (decodeDiff())
				return -1;

			currFrame->putUBlock(currBlock, r, c);
		}
	}

	for (uint r = 0; r < frame.getVRows(); r += bsize) {
		for (uint c = 0; c < frame.getVCols(); c += bsize) {
			Golomb::decode(32, &dr, bs);
			Golomb::decode(32, &dc, bs);

			prevFrame->getVBlock(prevBlock, r + dr, c + dc);

			if (decodeDiff())
				return -1;

			currFrame->putVBlock(currBlock, r, c);
		}
	}

	return 0;
}


void HybCoder::findBestYBlock ( uint sr, uint sc )
{
	findBestBlock(prevFrame->get_read_yBuff(), prevFrame->getYRows(), prevFrame->getYCols(), currBlock, sr, sc);
}

void HybCoder::findBestUBlock ( uint sr, uint sc )
{
	findBestBlock(prevFrame->get_read_uBuff(), prevFrame->getURows(), prevFrame->getUCols(), currBlock, sr, sc);
}

void HybCoder::findBestVBlock ( uint sr, uint sc )
{
	findBestBlock(prevFrame->get_read_vBuff(), prevFrame->getVRows(), prevFrame->getVCols(), currBlock, sr, sc);
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
		Golomb::encode(5, currBlock.buff[i] - prevBlock.buff[i], bs);
	}
}


int HybCoder::decodeDiff ( void )
{
	int aux;

	for (uint i = 0; i < (bsize * bsize); i++) {
		if (Golomb::decode(5, &aux, bs)) {
			return -1;
		}

		currBlock.buff[i] = prevBlock.buff[i] + aux;
	}

	return 0;
}

// EOF =========================================================================
