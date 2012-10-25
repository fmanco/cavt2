#ifndef _BITSTREAM_H_
#define _BITSTREAM_H_


class Bitsream {

public:
	enum type {READ, WRITE};
public:
	Bitsream(char* filename, type type);

public:

	void writeBit(unsigned short bit);
	void writeBits(unsigned short, int nBits);
	
	unsigned short readBit(int pos);
	unsigned short readBits(int pos, int len);
	
	void flush();
	void close();

private:
	FILE *fp;
	unsigned char* buffer;
	unsigned int pos;
	type type;

};

#endif
