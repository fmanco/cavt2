

#include <math.h>
#include <stdio.h>

#define BLOCK_SIZE 8

static float S_0 = sqrt(1.0f/8.0f);
static float S_X = sqrt(2.0f/8.0f);
static float PI_8 = atan(1)/2; // pi/8
inline float s(int i) { return (i==0) ? S_0 : S_X; }; //scaling factor
inline int round(float x) { return (int) (x>0.0f) ? floor(x+0.5f) : ceil(x-0.5f);};

//JPEG defines 8x8 quantizing matrices 
static float yQuantizeMatrix[8*8] = {
	16, 11, 10, 16, 24, 40, 51, 61,
	12, 12, 14, 19, 26, 58, 60, 55,
	14, 13, 16, 24, 40, 57, 69, 56,
	14, 17, 22, 29, 51, 87, 80, 62,
	18, 22, 37, 56, 68,109,103, 77,
	24, 35, 55, 64, 81,104,113, 92,
	49, 64, 78, 87,103,121,120,101,
	72, 92, 95, 98,112,100,103, 99,
};

static float uvQuantizeMatrix[8*8] = {
	17, 18, 24, 47, 99, 99, 99, 99,
	18, 21, 26, 66, 99, 99, 99, 99,
	24, 26, 56, 99, 99, 99, 99, 99,
	47, 66, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
};


void shift(int* block, int down);
void print(int* block);
void print(float* block);
void dct(int* values, float* dct);
void invdct(float* dct, int* values);
void quantize(float* dct, float factor, int* quantizedDct);
void dequantize(int* quantizedDct, float factor, float* dct);
void diff(int* a, int* b, int* result);

int main( int argc, char** argv ){
	int block[BLOCK_SIZE*BLOCK_SIZE]= {
		52, 55, 61, 66, 70, 61, 64, 73,
		63, 59, 55, 90,109, 85, 69, 72,
		62, 59, 68,113,144,104, 66, 73,
		63, 58, 71,122,154,106, 70, 69,
		67, 61, 68,104,126, 88, 68, 70,
		79, 65, 60, 70, 77, 68, 58, 75,
		85, 71, 64, 59, 55, 61, 65, 83,
		87, 79, 69, 68, 65, 76, 78, 94,
	};

	float dctCoefs[BLOCK_SIZE*BLOCK_SIZE] = {0.0f};
	int qdct[BLOCK_SIZE*BLOCK_SIZE] = {0};
	int block_[BLOCK_SIZE*BLOCK_SIZE] = {0};

	printf("Block:\n");
	print(block);

	shift(block, -128);
	printf("Shift:\n");
	print(block);

	dct(block, dctCoefs);
	printf("DCT (coefficients):\n");
	print(dctCoefs);

	quantize(dctCoefs, 1.0f, qdct);
	printf("Quantize:\n");
	print(qdct);

	dequantize(qdct, 1.0f, dctCoefs);
	printf("Dequantize:\n");
	print(dctCoefs);

	invdct(dctCoefs, block_);
	printf("Inverse DCT:\n");
	print(block_);

	diff(block, block_, block_);
	printf("Difference:\n");
	print(block_);

	// shift(block_, 1);
	// printf("Unshift:\n");
	// print(block_);	

	return 0;
}

void diff(int* a, int* b, int* result){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			result[i*8+j] = a[i*8+j] - b[i*8+j];
		}
	}
}

void invdct(float* dct, int* values){
	float sum;

	for (int i = 0; i < BLOCK_SIZE; i++){
		for (int j = 0; j< BLOCK_SIZE; j++){
			sum = 0;
			for (int u = 0; u<BLOCK_SIZE; u++){		
				for (int v = 0; v < BLOCK_SIZE; v++){
					sum += s(u)*s(v)*dct[u*BLOCK_SIZE+v]*cos(PI_8*(i+0.5f)*u)*cos(PI_8*(j+0.5f)*v);
				}
			}
			values[i*BLOCK_SIZE+j] = round(sum);
		}
	}	
}

void dct(int* values, float* dct){
	float sum;

	for (int u = 0; u<BLOCK_SIZE; u++){
		for (int v = 0; v < BLOCK_SIZE; v++){
			sum = 0;
			for (int i = 0; i < BLOCK_SIZE; i++){
				for (int j = 0; j< BLOCK_SIZE; j++){
					sum += s(u)*s(v)*values[i*BLOCK_SIZE+j]*cos(PI_8*(i+0.5f)*u)*cos(PI_8*(j+0.5f)*v);
				}
			}
			dct[u*BLOCK_SIZE+v] = sum;
		}
	}
}


void dequantize(int* qdct, float factor, float* dct){
	float* matrix = yQuantizeMatrix;

	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			dct[i*8+j] = qdct[i*8+j]*(matrix[i*8+j]*factor);
		}
	}

}

void quantize(float* dct, float factor, int* qdct){
	float* matrix = yQuantizeMatrix;

	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			qdct[i*8+j] = round(dct[i*8+j]/(matrix[i*8+j]*factor));
		}
	}
}


void shift(int* block, int offset){


	for (int i = 0; i < BLOCK_SIZE*BLOCK_SIZE; i++){
		block[i] += offset;
	}
}

void print(int* block){

	for (int i = 0; i < BLOCK_SIZE*BLOCK_SIZE; i++){
		printf("%4d", block[i]);
		if ((i+1)%BLOCK_SIZE==0)
			printf("\n");
	}
}

void print(float* block){

	for (int i = 0; i < BLOCK_SIZE*BLOCK_SIZE; i++){
		printf("%8.2f ", block[i]);
		if ((i+1)%BLOCK_SIZE==0)
			printf("\n");
	}
}