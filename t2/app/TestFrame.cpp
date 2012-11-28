//==============================================================================
// TestFrame.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "YuvFrame.h"


//==============================================================================

int main ( int argc, char** argv )
{
	YuvFrame a(20, 20);

	uchar* buff = a.get_write_yBuff_444();
	buff[10] = 1;
	buff[11] = 2;

	a.get_read_yBuff_420();
	a.get_write_yBuff_420();

	YuvFrame b = a;

	printf("%p %p\n%d %d %d\n", &a, &b,
		b.getYPixel(0, 9), b.getYPixel(0, 10), b.getYPixel(0, 11));
}


// EOF =========================================================================
