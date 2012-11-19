//==============================================================================
// Golomb.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include <base.h>
#include "Golomb.h"


//==============================================================================



int static Golomb::decode(uint m, BitStream& bs){
	int q = 0; r,err;

	while (bs.readBit()!=0){
		q++;
	}

	r = bs.readNBits(log_2(n));
	e = q * m + c;
	if (c % 2 ==0)
		return e/2;
	else
		return -(e+1)/2;
}


// EOF =========================================================================
