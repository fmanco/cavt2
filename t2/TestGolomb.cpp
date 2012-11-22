#include <cstdio>
#include "Golomb.h"
#include "BitStream.h"

int main( int argc, char** argv ){

	char* file = (char*)"file"; //BACK OFF

	BitStream bs = BitStream(file, BitStream::WRITE);


	bs.open();
	Golomb::encode(15, -29, bs);
	Golomb::encode(16, 34, bs);
	bs.close();


	BitStream bs1 = BitStream(file, BitStream::READ);

	bs1.open();
	printf("%d\n", Golomb::decode(15, bs1));
	printf("%d\n", Golomb::decode(16, bs1));
	bs1.close();

	return 0;
}