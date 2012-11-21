//==============================================================================
// Golomb.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __GOLOMB_H__
#define __GOLOMB_H__


//==============================================================================

#include "base.h"
#include "BitStream.h"

//==============================================================================

class Golomb{

public:
	int static decode(uint m, BitStream& bs);
	void static encode(uint m, int value, BitStream& bs);

};


//==============================================================================
#endif /* __GOLOMB_H__ */








