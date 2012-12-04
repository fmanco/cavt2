//==============================================================================
// InterCoder.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "InterCoder.h"
#include "YuvFrame.h"
#include "BitStream.h"
#include "Golomb.h"
#include <climits>
#include <cstdlib>


//==============================================================================

#define DIFF_M   4
#define DELTA_M  32


//==============================================================================

int InterCoder::encode ( YuvFrame& frame, YuvFrame& prevFrame,
	                            uint bsize, uint area,
	                            uint qY, uint qU, uint qV,
                                YuvFrame& quantFrame, BitStream& bs )
{
	int dr = 0;
	int dc = 0;

	Block currBlock(bsize);
	Block prevBlock(bsize);

	for (uint r = 0; r < frame.getYRows(); r += bsize) {
		for (uint c = 0; c < frame.getYCols(); c += bsize) {
			frame.getYBlock(currBlock, r, c);
			findBestYBlock(frame, currBlock, area, r, c, dr, dc);
			prevFrame.getYBlock(prevBlock, r + dr, c + dc);

			encodeDelta(dr, dc, bs);
			encodeDiffs(prevBlock, currBlock, bsize, qY, bs);

			quantFrame.putYBlock(currBlock, r, c);
		}
	}

	for (uint r = 0; r < frame.getURows(); r += bsize) {
		for (uint c = 0; c < frame.getUCols(); c += bsize) {
			frame.getUBlock(currBlock, r, c);
			findBestUBlock(frame, currBlock, area, r, c, dr, dc);
			prevFrame.getUBlock(prevBlock, r + dr, c + dc);

			encodeDelta(dr, dc, bs);
			encodeDiffs(prevBlock, currBlock, bsize, qU, bs);

			quantFrame.putUBlock(currBlock, r, c);
		}
	}

	for (uint r = 0; r < frame.getVRows(); r += bsize) {
		for (uint c = 0; c < frame.getVCols(); c += bsize) {
			frame.getVBlock(currBlock, r, c);
			findBestVBlock(frame, currBlock, area, r, c, dr, dc);
			prevFrame.getVBlock(prevBlock, r + dr, c + dc);

			encodeDelta(dr, dc, bs);
			encodeDiffs(prevBlock, currBlock, bsize, qV, bs);

			quantFrame.putVBlock(currBlock, r, c);
		}
	}

	return 0;
}

int InterCoder::decode ( YuvFrame& currFrame, YuvFrame& prevFrame,
                                uint bsize, uint area,
                                uint qY, uint qU, uint qV,
                                BitStream& bs )
{
	int dr = 0;
	int dc = 0;

	Block currBlock(bsize);
	Block prevBlock(bsize);

	for (uint r = 0; r < currFrame.getYRows(); r += bsize) {
		for (uint c = 0; c < currFrame.getYCols(); c += bsize) {
			if (decodeDelta(dr, dc, bs))
				return -1;

			prevFrame.getYBlock(prevBlock, r + dr, c + dc);

			if (decodeDiffs(prevBlock, currBlock, bsize, qY, bs))
				return -1;

			currFrame.putYBlock(currBlock, r, c);
		}
	}

	for (uint r = 0; r < currFrame.getURows(); r += bsize) {
		for (uint c = 0; c < currFrame.getUCols(); c += bsize) {
			if (decodeDelta(dr, dc, bs))
				return -1;

			prevFrame.getUBlock(prevBlock, r + dr, c + dc);

			if (decodeDiffs(prevBlock, currBlock, bsize, qU, bs))
				return -1;

			currFrame.putUBlock(currBlock, r, c);
		}
	}

	for (uint r = 0; r < currFrame.getVRows(); r += bsize) {
		for (uint c = 0; c < currFrame.getVCols(); c += bsize) {
			if (decodeDelta(dr, dc, bs))
				return -1;

			prevFrame.getVBlock(prevBlock, r + dr, c + dc);

			if (decodeDiffs(prevBlock, currBlock, bsize, qV, bs))
				return -1;

			currFrame.putVBlock(currBlock, r, c);
		}
	}

	return 0;
}


//==============================================================================

void InterCoder::findBestYBlock ( YuvFrame& prevFrame, Block& blk, uint area, uint sr, uint sc, int& dr, int& dc )
{
	findBestBlock(prevFrame.get_read_yBuff(), prevFrame.getYRows(), prevFrame.getYCols(), blk, area, sr, sc, dr, dc);
}

void InterCoder::findBestUBlock ( YuvFrame& prevFrame, Block& blk, uint area, uint sr, uint sc, int& dr, int& dc )
{
	findBestBlock(prevFrame.get_read_uBuff(), prevFrame.getURows(), prevFrame.getUCols(), blk, area, sr, sc, dr, dc);
}

void InterCoder::findBestVBlock ( YuvFrame& prevFrame, Block& blk, uint area, uint sr, uint sc, int& dr, int& dc )
{
	findBestBlock(prevFrame.get_read_vBuff(), prevFrame.getVRows(), prevFrame.getVCols(), blk, area, sr, sc, dr, dc);
}

void InterCoder::findBestBlock ( uchar* fBuff, uint fRows, uint fCols, Block& blk, uint area, uint sr, uint sc, int& dr, int& dc )
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


uint InterCoder::Blockcmp ( uchar* fBuff, uint fRows, uint fCols, Block& blk, uint r, uint c )
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


void InterCoder::encodeDelta ( int dr, int dc, BitStream& bs )
{
	Golomb::encode(DELTA_M, dr, bs);
	Golomb::encode(DELTA_M, dc, bs);
}

void InterCoder::encodeDiffs ( Block& prevBlock, Block& currBlock,
                                      uint bsize, uint quantization,
                                      BitStream& bs)
{
	int aux;

	if (quantization == 1) {
		for (uint i = 0; i < (bsize * bsize); i++) {
			Golomb::encode(DIFF_M, currBlock.buff[i] - prevBlock.buff[i], bs);
		}
	} else {
		for (uint i = 0; i < (bsize * bsize); i++) {
			aux = (currBlock.buff[i] - prevBlock.buff[i]) / ((int)quantization);

			Golomb::encode(DIFF_M, aux, bs);

			currBlock.buff[i] = prevBlock.buff[i] + (aux * ((int)quantization));
		}
	}
}


int InterCoder::decodeDelta ( int& dr, int& dc, BitStream& bs )
{
	int err;

	err = Golomb::decode(DELTA_M, &dr, bs);
	if (err)
		return err;

	err = Golomb::decode(DELTA_M, &dc, bs);
	if (err)
		return err;

	return 0;
}

int InterCoder::decodeDiffs ( Block& prevBlock, Block& currBlock,
                                     uint bsize, uint quantization,
                                     BitStream& bs )
{
	int aux;
	int err;

	if (quantization == 1) {
		for (uint i = 0; i < (bsize * bsize); i++) {
			err = Golomb::decode(DIFF_M, &aux, bs);
			if (err)
				return err;

			currBlock.buff[i] = prevBlock.buff[i] + aux;
		}
	} else {
		for (uint i = 0; i < (bsize * bsize); i++) {
			err = Golomb::decode(DIFF_M, &aux, bs);
			if (err)
				return err;

			currBlock.buff[i] = prevBlock.buff[i] + (aux * ((int)quantization));
		}
	}

	return 0;
}


// EOF =========================================================================

