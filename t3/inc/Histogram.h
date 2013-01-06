//==============================================================================
// Histogram.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__


//==============================================================================

#include "base.h"
#include <string>
#include <stdint.h>

//==============================================================================

class Histogram
{
public:
	Histogram  ( void );
	~Histogram (  );

public:
	void inc ( int16_t val );
	uint get ( int16_t val );

public:
	bool write_csv ( std::string file );

private:
	uint hist[65536];
};


//==============================================================================
#endif /* __HISTOGRAM_H__ */
