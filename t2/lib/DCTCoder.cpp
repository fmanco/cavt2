//==============================================================================
// DCTCoder.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "DCTCoder.h"
#include <math.h>
#include "BitStream.h"
#include "YuvFrame.h"
#include "Block.h"
#include "Golomb.h"

//JPEG defines 8x8 quantizing matrices, so we'll use 8x8 blocks for pixels
//try to use bigger blocks?

#define W 8
#define H 8
#define SIZE 64

static float S_0 = sqrt(1.0f/8.0f);
static float S_X = sqrt(2.0f/8.0f);
static float PI_8 = atan(1)/2; // pi/8

static float yQuantizeMatrix[] = {
	16, 11, 10, 16, 24, 40, 51, 61,
	12, 12, 14, 19, 26, 58, 60, 55,
	14, 13, 16, 24, 40, 57, 69, 56,
	14, 17, 22, 29, 51, 87, 80, 62,
	18, 22, 37, 56, 68,109,103, 77,
	24, 35, 55, 64, 81,104,113, 92,
	49, 64, 78, 87,103,121,120,101,
	72, 92, 95, 98,112,100,103, 99,
};

static float uvQuantizeMatrix[] = {
	17, 18, 24, 47, 99, 99, 99, 99,
	18, 21, 26, 66, 99, 99, 99, 99,
	24, 26, 56, 99, 99, 99, 99, 99,
	47, 66, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
};

//==============================================================================

inline float static s(int i) { return (i==0) ? S_0 : S_X; }; //scaling factor for the transform
inline int static round(float x) { return (int) (x>0.0f) ? floor(x+0.5f) : ceil(x-0.5f);};
inline void static shift(int* block, int offset) { for (int i = 0; i < SIZE; i++){ block[i] += offset; }}; // add a value to every element in the block

int DCTCoder::writeHeader(uint rows, uint cols, uint fps, uint type, uint quantization, BitStream& bs) {
	return -1;
}

int DCTCoder::encode(YuvFrame& frame, BitStream& bs, uint quantization) {

	// YuvBlock block(W,H);

	int err;
	int block[SIZE] = {0};
	float dctC[SIZE] = {.0f};
	int qBlock[SIZE] = {0};

	// Block block(W,H);


	int yRowBlocks = (frame.getYRows() + H - 1)/H; // ceil()
	int yColBlocks = (frame.getYCols() + W - 1)/W;
	int uRowBlocks = (frame.getURows() + H - 1)/H;
	int uColBlocks = (frame.getUCols() + W - 1)/W;
	int vRowBlocks = (frame.getVRows() + H - 1)/H;
	int vColBlocks = (frame.getVCols() + W - 1)/W;

	// int w, h;

	// printf ("rows %d -> %d\n", frame.getYRows(), yRowBlocks);
	// printf ("cols %d -> %d\n", frame.getYCols(), yColBlocks);

	//Y
	for (int i = 0; i < yRowBlocks; i++){
		// printf("%d\n", i);
		for (int j = 0; j < yColBlocks; j++){
			if ( (frame.getYRows() - i*H) % 8  != 0) {
				printf("i=%d continue\n", i);
				continue;
			}
			if ( (frame.getYCols() - j*W) % 8  != 0){
				printf("j=%d continue\n", j);
				continue;
			}

			for (int k = 0; k < H; k++){
				for (int l = 0; l < W; l++){
					// printf("\t%d, %d -> (%d, %d)", k, l, i*H+k, j*W+l);
					block[k*W+l] = frame.getYPixel(i*H+k,j*W+l);
				}
				// printf("\n");
			}

			shift(block, -128);
			dct(block, dctC);
			quantize(dctC, 1.0, LUMINANCE, qBlock);

			for (int c = 0; c < SIZE; c++){
				if ((err = Golomb::encode(5, qBlock[c], bs)) != 0){
					return err;
				}

			}
		}
	}

	return 0;

	//U
	for (int i = 0; i < uRowBlocks; i++){
		// printf("%d\n", i);
		for (int j = 0; j < uColBlocks; j++){
			if ( (frame.getURows() - i*H) % 8  != 0) {
				printf("i=%d continue\n", i);
				continue;
			}
			if ( (frame.getUCols() - j*W) % 8  != 0){
				printf("j=%d continue\n", j);
				continue;
			}



			for (int k = 0; k < H; k++){
				for (int l = 0; l < W; l++){
					// printf("\t%d, %d -> (%d, %d)", k, l, i*H+k, j*W+l);
					block[k*W+l] = frame.getUPixel(i*H+k,j*W+l);
				}
				// printf("\n");
			}

			shift(block, -128);
			dct(block, dctC);
			quantize(dctC, 1.0, CROMINANCE, qBlock);

			for (int c = 0; c < SIZE; c++){
				if ((err = Golomb::encode(5, qBlock[c], bs)) != 0){
					return err;
				}

			}
		}
	}

	//V
	for (int i = 0; i < vRowBlocks; i++){
		// printf("%d\n", i);
		for (int j = 0; j < vColBlocks; j++){
			if ( (frame.getVRows() - i*H) % 8  != 0) {
				printf("i=%d continue\n", i);
				continue;
			}
			if ( (frame.getVCols() - j*W) % 8  != 0){
				printf("j=%d continue\n", j);
				continue;
			}



			for (int k = 0; k < H; k++){
				for (int l = 0; l < W; l++){
					// printf("\t%d, %d -> (%d, %d)", k, l, i*H+k, j*W+l);
					block[k*W+l] = frame.getVPixel(i*H+k,j*W+l);
				}
				// printf("\n");
			}

			shift(block, -128);
			dct(block, dctC);
			quantize(dctC, 1.0, CROMINANCE, qBlock);

			for (int c = 0; c < SIZE; c++){
				if ((err = Golomb::encode(5, qBlock[c], bs)) != 0){
					return err;
				}

			}
		}
	}

	return 0;
}

int DCTCoder::readHeader(BitStream& bs, uint *rows, uint *cols, uint *fps, uint *type, uint *quantization){
	


	return -1;
}

int DCTCoder::decode(BitStream& bs, YuvFrame& frame, uint quantization){


	int err;
	int block[SIZE] = {0};
	float dctC[SIZE] = {.0f};
	int qBlock[SIZE] = {0};

	// Block block(W,H);


	int yRowBlocks = (frame.getYRows() + H - 1)/H; // ceil()
	int yColBlocks = (frame.getYCols() + W - 1)/W;
	int uRowBlocks = (frame.getURows() + H - 1)/H;
	int uColBlocks = (frame.getUCols() + W - 1)/W;
	int vRowBlocks = (frame.getVRows() + H - 1)/H;
	int vColBlocks = (frame.getVCols() + W - 1)/W;

	// int w, h;

	int v;

	// printf ("rows %d -> %d\n", frame.getYRows(), yRowBlocks);
	// printf ("cols %d -> %d\n", frame.getYCols(), yColBlocks);


	// Y
	for (int i = 0; i < yRowBlocks; i++){
		// printf("%d\n", i);
		for (int j = 0; j < yColBlocks; j++){
			if ( (frame.getYRows() - i*H) % 8  != 0) {
				printf("i=%d continue\n", i);
				continue;
			}
			if ( (frame.getYCols() - j*W) % 8  != 0){
				printf("j=%d continue\n", j);
				continue;
			}


			for (int c = 0; c < SIZE; c++){
				if ((err = Golomb::decode(5, &v, bs)) != 0){
					return err;
				}
				qBlock[c] = v;

			}

			dequantize(qBlock, 1.0f, LUMINANCE, dctC);
			invdct(dctC, block);
			shift(block, 128);

			for (int k = 0; k < H; k++){
				for (int l = 0; l < W; l++){
					frame.putYPixel(i*H+k,j*W+l, block[k*W+l]);
				}
			}
		}
	}

	for (uint r = 0; r < frame.getURows(); r++){
		for (uint c = 0; c < frame.getUCols(); c++){
			frame.putUPixel(r, c,128);
			frame.putVPixel(r, c,128);
		}

	}


	return 0;

	//U
	for (int i = 0; i < uRowBlocks; i++){
		// printf("%d\n", i);
		for (int j = 0; j < uColBlocks; j++){
			if ( (frame.getURows() - i*H) % 8  != 0) {
				printf("i=%d continue\n", i);
				continue;
			}
			if ( (frame.getUCols() - j*W) % 8  != 0){
				printf("j=%d continue\n", j);
				continue;
			}

			for (int c = 0; c < SIZE; c++){
				if ((err = Golomb::decode(5, &v, bs)) != 0){
					return err;
				}
				qBlock[c] = v;

			}

			dequantize(qBlock, 1.0f, CROMINANCE, dctC);
			invdct(dctC, block);
			shift(block, 128);

			for (int k = 0; k < H; k++){
				for (int l = 0; l < W; l++){
					frame.putUPixel(i*H+k,j*W+l, block[k*W+l]);
				}
			}
		}
	}

	//V
	for (int i = 0; i < vRowBlocks; i++){
		// printf("%d\n", i);
		for (int j = 0; j < vColBlocks; j++){
			if ( (frame.getVRows() - i*H) % 8  != 0) {
				printf("i=%d continue\n", i);
				continue;
			}
			if ( (frame.getVCols() - j*W) % 8  != 0){
				printf("j=%d continue\n", j);
				continue;
			}


			for (int c = 0; c < SIZE; c++){
				if ((err = Golomb::decode(5, &v, bs)) != 0){
					return err;
				}
				qBlock[c] = v;

			}

			dequantize(qBlock, 1.0f, CROMINANCE, dctC);
			invdct(dctC, block);
			shift(block, 128);

			for (int k = 0; k < H; k++){
				for (int l = 0; l < W; l++){
					frame.putVPixel(i*H+k,j*W+l, block[k*W+l]);
				}
			}
		}
	}

	return 0;
}

void DCTCoder::dct(int* const values, float* dct) {
	float sum;

	for (int u = 0; u < H; u++){
		for (int v = 0; v < W; v++){
			sum = 0;
			for (int i = 0; i < H; i++){
				for (int j = 0; j < W; j++){
					sum += s(u)*s(v)*values[i * W + j]*cos(PI_8*(i+0.5f)*u)*cos(PI_8*(j+0.5f)*v);
				}
			}
			dct[u * W + v] = sum;
		}
	}
}

void DCTCoder::invdct(float* dct, int* values){
	float sum;

	for (int i = 0; i < H; i++){
		for (int j = 0; j < W; j++){
			sum = 0;
			for (int u = 0; u < H; u++){		
				for (int v = 0; v < W; v++){
					sum += s(u)*s(v)*dct[u * W + v]*cos(PI_8*(i+0.5f)*u)*cos(PI_8*(j+0.5f)*v);
				}
			}
			values[i * W + j] = round(sum);
		}
	}		
}

void DCTCoder::quantize(float* dct, float factor, Type type, int* quantizedDct){
	float* matrix = (type == LUMINANCE ) ? yQuantizeMatrix : uvQuantizeMatrix;

	for (int i = 0; i < H; i++){
		for (int j = 0; j < W; j++){
			quantizedDct[i*W+j] = round(dct[i*W+j]/(matrix[i*W+j]*factor));
		}
	}
}	

void DCTCoder::dequantize(int* quantizedDct, float factor, Type type, float* dct) {
	float* matrix = (type == LUMINANCE ) ? yQuantizeMatrix : uvQuantizeMatrix;

	for (int i = 0; i < H; i++){
		for (int j = 0; j < W; j++){
			dct[i*W+j] = quantizedDct[i*W+j]*(matrix[i*W+j]*factor);
		}
	}
}

// EOF =========================================================================
