//==============================================================================
// Golomb.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "Golomb.h"
#include <math.h>
#include <cstdlib>

//==============================================================================
//encodes signed values by assigning negative values to odd numbers, and positive to even numbers


int Golomb::decode(uint m, BitStream& bs){
	int q = 0, r, value;
	int nbits = (int) floor(log2(m)) + 1;
	uchar bit;
	


	// while (bs.readBit()!=0){
	// 	q++;
	// }

	do {
		bs.readBit(&bit); //ugly
		if (bit == 0 )
			break;
		q++;
	} while(true);


	bs.readBits(nbits, &bit); //ugly
	r = (int) bit;
 
	value = q * m + r;
	if (value % 2 == 0)
		return value/2;
	else
		return -(value+1)/2;
}


void Golomb::encode(uint m, int value, BitStream& bs){
	int q, r, i;
	int nbits = (int) floor(log2(m)) + 1;

	if (value < 0 )
	{
		value = 2*abs(value) - 1;
	}
	else 
	{
		value *= 2;
	}

	q = value/m;
	r = value%m;
	

	// printf("value=%d m=%d q=%d r=%d nBits=%d\n", value, m, q, r, nbits);


	for(i=0; i<q;i++)
	{
		bs.writeBit(1);
	}

	bs.writeBit(0);
	bs.writeBits(r, nbits);
}


// EOF =========================================================================
