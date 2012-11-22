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


int Golomb::decode(uint m, int *value, BitStream& bs){
	int q = 0, r, v;
	int nbits = (int) floor(log2(m)) + 1;
	int err;
	uchar bit;
	

	do {
		if ((err=bs.readBit(&bit)) != 0)
			return err;

		if (bit == 0)
			break;
		q++;
	} while(true);


	printf("hey!\n");

	if ((err = bs.readBits(nbits, &bit)) != 0) 
		return err;

	r = (int) bit;
 
	v = q * m + r;
	if (v % 2 == 0)
		*value =  v/2;
	else
		*value = -(v+1)/2;

	return 0;
}


int Golomb::encode(uint m, int value, BitStream& bs){
	int q, r, i;
	int nbits = (int) floor(log2(m)) + 1;
	int err;

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
		if ((err = bs.writeBit(1))!= 0)
			return err;
	}

	
	if ((err = bs.writeBit(0))!= 0)
		return err;

	if ((err = bs.writeBits(r, nbits))!= 0){
		return err;
	}

	return 0;
}


// EOF =========================================================================
