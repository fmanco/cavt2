
#include "BitStream.h"

int main ( int argc, char** argv) {
	uchar bit = 0;

	BitStream str((char*) "file", BitStream::WRITE);

	printf("%d\n", str.open());

	for(int i = 0; i < 23; i++) {
			printf("%d\n", str.writeBit((bit = ~bit)));
	}

	printf("%d\n", str.close());
}
