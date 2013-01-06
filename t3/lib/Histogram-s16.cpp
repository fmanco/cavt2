//==============================================================================
// Histogram-s16.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "Histogram.h"
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <limits.h>

//==============================================================================

Histogram::Histogram  ( void )
{
	for (uint i = 0; i < 65536; i++) {
		hist[i] = 0;
	}
}

Histogram::~Histogram (  )
{

}


//==============================================================================

void Histogram::inc ( int16_t val )
{
	hist[static_cast<uint16_t>(val)]++;
}

uint Histogram::get ( int16_t val )
{
	return hist[static_cast<uint16_t>(val)];
}

bool Histogram::write_csv ( std::string file )
{
	std::ofstream f(file.c_str());

	if (!f.is_open())
		return false;

	f << "val, count" << std::endl;

	for (int16_t i = SHRT_MIN; i < SHRT_MAX; i++ ) {
		f << i << ", " << hist[static_cast<uint16_t>(i)] << std::endl;
	}

	f.close();

	return true;
}


// EOF =========================================================================
