//==============================================================================
// Golomb.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __GOLOMB_H__
#define __GOLOMB_H__


//==============================================================================

#include <base.h>
#include "BitStream.h"

//==============================================================================

class Golomb{

	int static Golomb::decode(uint m, BitStream& bs);
	int static Golomb::encode(uint m, int value, BitStream& bs);

};


//==============================================================================
#endif /* __GOLOMB_H__ */








