//==============================================================================
// MonkeyCoder.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __MONKEYCODER_H__
#define __MONKEYCODER_H__


//==============================================================================

#include "base.h"
#include "Predictor.h"
#include "stdint.h"


//==============================================================================

class MonkeyCoder
{
public:
	MonkeyCoder  ( int _quant = 1 );
	~MonkeyCoder (  );

public:
	void encode ( const int16_t frame[2], int32_t diff[2] );
	void decode ( const int32_t diff[2], int16_t frame[2] );

private:
	Predictor p;
	int quant;

};


//==============================================================================
#endif /* __MONKEYCODER_H__ */
