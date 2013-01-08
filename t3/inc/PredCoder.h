//==============================================================================
// PredCoder.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __PREDCODER_H__
#define __PREDCODER_H__


//==============================================================================

#include "base.h"
#include "Predictor.h"
#include "stdint.h"


//==============================================================================

class PredCoder
{
public:
	PredCoder  ( int _quant = 1 );
	~PredCoder (  );

public:
	void encode ( const int16_t frame[2], int32_t diff[2] );
	void decode ( const int32_t diff[2], int16_t frame[2] );

private:
	Predictor p;
	int quant;

};


//==============================================================================
#endif /* __PREDCODER_H__ */
