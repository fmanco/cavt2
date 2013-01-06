//==============================================================================
// TestHist.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "Histogram.h"


//==============================================================================

int main ( int argc, char** argv )
{
	Histogram c;

	c.inc(-10);
	c.inc(  0);
	c.inc(  0);
	c.inc( 10);

	c.write_csv("TestHist.csv");

	return 0;
}


// EOF =========================================================================
