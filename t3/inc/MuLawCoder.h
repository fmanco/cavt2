//==============================================================================
// MuLawCoder.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __MULAWCODER_H__
#define __MULAWCODER_H__


//==============================================================================

#include "base.h"
#include "stdint.h"


//==============================================================================

class MuLawCoder
{
public:
	MuLawCoder  ( void );
	~MuLawCoder (  );

public:
	uint8_t static encode ( int16_t val );
	int16_t static decode ( uint8_t val );

};


//==============================================================================
#endif /* __MULAWCODER_H__ */
