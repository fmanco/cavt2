//==============================================================================
// YuvFrame.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "YuvFrame.h"
#include <cstdlib>
#include <cstring>


//==============================================================================

YuvFrame::YuvFrame ( uint _nRows, uint _nCols )
	: nRows(_nRows), nCols(_nCols)
{
	yBuff     = NULL;

	buff_444  = NULL;
	yBuff_444 = NULL;
	uBuff_444 = NULL;
	vBuff_444 = NULL;

	buff_422  = NULL;
	yBuff_422 = NULL;
	uBuff_422 = NULL;
	vBuff_422 = NULL;

	buff_420  = NULL;
	yBuff_420 = NULL;
	uBuff_420 = NULL;
	vBuff_420 = NULL;

	sync_buff_444 = false;
	sync_buff_422 = false;
	sync_buff_420 = false;
}

YuvFrame::YuvFrame ( const YuvFrame& obj )
{
	nRows = obj.nRows;
	nCols = obj.nCols;

	yBuff     = NULL;

	buff_444  = NULL;
	yBuff_444 = NULL;
	uBuff_444 = NULL;
	vBuff_444 = NULL;

	buff_422  = NULL;
	yBuff_422 = NULL;
	uBuff_422 = NULL;
	vBuff_422 = NULL;

	buff_420  = NULL;
	yBuff_420 = NULL;
	uBuff_420 = NULL;
	vBuff_420 = NULL;

	sync_buff_444 = obj.sync_buff_444;
	sync_buff_422 = obj.sync_buff_422;
	sync_buff_420 = obj.sync_buff_420;


	if (sync_buff_444) {
		aloc_444();

		std::memcpy(uBuff_444, obj.uBuff_444, nRows * nCols * sizeof(uchar));
		std::memcpy(vBuff_444, obj.vBuff_444, nRows * nCols * sizeof(uchar));
	}

	if (sync_buff_422) {
		aloc_422();

		std::memcpy(uBuff_422, obj.uBuff_422, nRows * (nCols / 2) * sizeof(uchar));
		std::memcpy(vBuff_422, obj.vBuff_422, nRows * (nCols / 2) * sizeof(uchar));
	}

	if (sync_buff_420) {
		aloc_420();

		std::memcpy(uBuff_420, obj.uBuff_420, (nRows / 2) * (nCols / 2) * sizeof(uchar));
		std::memcpy(vBuff_420, obj.vBuff_420, (nRows / 2) * (nCols / 2) * sizeof(uchar));
	}

	if (yBuff) {
		std::memcpy(yBuff, obj.yBuff, nRows * nCols * sizeof(uchar));
	}
}

YuvFrame::~YuvFrame (  )
{
	if (buff_444)
		delete[] buff_444;

	if (buff_422)
		delete[] buff_422;

	if (buff_420)
		delete[] buff_420;
}


//==============================================================================

void YuvFrame::getYBlock ( Block &b, uint r, uint c )
{
	read_444();

	getBlock(b, r, c, yBuff_444);
}

void YuvFrame::getUBlock ( Block &b, uint r, uint c )
{
	read_444();

	getBlock(b, r, c, uBuff_444);
}

void YuvFrame::getVBlock ( Block &b, uint r, uint c )
{
	read_444();

	getBlock(b, r, c, vBuff_444);
}

void YuvFrame::putYBlock ( const Block &b, uint r, uint c )
{
	write_444();

	putBlock(b, r, c, yBuff_444);
}

void YuvFrame::putUBlock ( const Block &b, uint r, uint c )
{
	write_444();

	putBlock(b, r, c, uBuff_444);
}

void YuvFrame::putVBlock ( const Block &b, uint r, uint c )
{
	write_444();

	putBlock(b, r, c, vBuff_444);
}


//==============================================================================

uchar YuvFrame::getYPixel ( uint r, uint c ) const
{
	if (r >= nRows || c >= nCols)
		return 0;

	if (sync_buff_444 | sync_buff_422 | sync_buff_420) {
		return yBuff[(r * nCols) + c];
	} else {
		return 0;
	}
}

uchar YuvFrame::getUPixel ( uint r, uint c ) const
{
	if (r >= nRows || c >= nCols)
		return 0;

	if (sync_buff_444) {
		return uBuff_444[(r * nCols) + c];
	} else if (sync_buff_422) {
		return uBuff_422[(r * (nCols / 2)) + (c / 2)];
	} else if (sync_buff_420) {
		return uBuff_420[((r / 2) * (nCols / 2)) + (c / 2)];
	} else {
		return 0;
	}
}

uchar YuvFrame::getVPixel ( uint r, uint c ) const
{
	if (r >= nRows || c >= nCols)
		return 0;

	if (sync_buff_444) {
		return vBuff_444[(r * nCols) + c];
	} else if (sync_buff_422) {
		return vBuff_422[(r * (nCols / 2)) + (c / 2)];
	} else if (sync_buff_420) {
		return vBuff_420[((r / 2) * (nCols / 2)) + (c / 2)];
	} else {
		return 0;
	}
}

void YuvFrame::putYPixel ( uint r, uint c, uchar pixel )
{
	if (r >= nRows || c >= nCols)
		return;

	if (sync_buff_444 || sync_buff_422 || sync_buff_420) {
		yBuff[(r * nCols) + c] = pixel;
	}
}

void YuvFrame::putUPixel ( uint r, uint c, uchar pixel )
{
	if (r >= nRows || c >= nCols)
		return;

	if (sync_buff_444) {
		uBuff_444[(r * nCols) + c] = pixel;
	} else if (sync_buff_422) {
		uBuff_422[(r * (nCols / 2)) + (c / 2)] = pixel;
	} else if (sync_buff_420) {
		uBuff_420[((r / 2) * (nCols / 2)) + (c / 2)] = pixel;
	}
}

void YuvFrame::putVPixel ( uint r, uint c, uchar pixel )
{
	if (r >= nRows || c >= nCols)
		return;

	if (sync_buff_444) {
		vBuff_444[(r * nCols) + c] = pixel;
	} else if (sync_buff_422) {
		vBuff_422[(r * (nCols / 2)) + (c / 2)] = pixel;
	} else if (sync_buff_420) {
		vBuff_420[((r / 2) * (nCols / 2)) + (c / 2)] = pixel;
	}
}


//==============================================================================

uchar* YuvFrame::get_read_yBuff_444 ( void ) { read_444(); return yBuff_444; }
uchar* YuvFrame::get_read_uBuff_444 ( void ) { read_444(); return uBuff_444; }
uchar* YuvFrame::get_read_vBuff_444 ( void ) { read_444(); return vBuff_444; }

uchar* YuvFrame::get_read_yBuff_422 ( void ) { read_422(); return yBuff_422; }
uchar* YuvFrame::get_read_uBuff_422 ( void ) { read_422(); return uBuff_422; }
uchar* YuvFrame::get_read_vBuff_422 ( void ) { read_422(); return vBuff_422; }

uchar* YuvFrame::get_read_yBuff_420 ( void ) { read_420(); return yBuff_420; }
uchar* YuvFrame::get_read_uBuff_420 ( void ) { read_420(); return uBuff_420; }
uchar* YuvFrame::get_read_vBuff_420 ( void ) { read_420(); return vBuff_420; }

uchar* YuvFrame::get_write_yBuff_444 ( void ) { write_444(); return yBuff_444; }
uchar* YuvFrame::get_write_uBuff_444 ( void ) { write_444(); return uBuff_444; }
uchar* YuvFrame::get_write_vBuff_444 ( void ) { write_444(); return vBuff_444; }

uchar* YuvFrame::get_write_yBuff_422 ( void ) { write_422(); return yBuff_422; }
uchar* YuvFrame::get_write_uBuff_422 ( void ) { write_422(); return uBuff_422; }
uchar* YuvFrame::get_write_vBuff_422 ( void ) { write_422(); return vBuff_422; }

uchar* YuvFrame::get_write_yBuff_420 ( void ) { write_420(); return yBuff_420; }
uchar* YuvFrame::get_write_uBuff_420 ( void ) { write_420(); return uBuff_420; }
uchar* YuvFrame::get_write_vBuff_420 ( void ) { write_420(); return vBuff_420; }


//==============================================================================

void YuvFrame::read_444 ( void )
{
	aloc_444();

	if (!sync_buff_444) {
		if (sync_buff_422) {
			convert_422_444();
			sync_buff_444 = true;
		} else if (sync_buff_420) {
			convert_420_444();
			sync_buff_444 = true;
		}
	}
}

void YuvFrame::write_444 ( void )
{
	aloc_444();

	sync_buff_444 = true;
	sync_buff_422 = false;
	sync_buff_420 = false;
}

void YuvFrame::read_422 ( void )
{
	aloc_422();

	if (!sync_buff_422) {
		if (sync_buff_444) {
			convert_444_422();
			sync_buff_422 = true;
		} else if (sync_buff_420) {
			// TODO: convert from 420 to 422
			sync_buff_422 = true;
		}
	}
}

void YuvFrame::write_422 ( void )
{
	aloc_422();

	sync_buff_444 = false;
	sync_buff_422 = true;
	sync_buff_420 = false;
}

void YuvFrame::read_420 ( void )
{
	aloc_420();

	if (!sync_buff_420) {
		if (sync_buff_444) {
			convert_444_420();
			sync_buff_420 = true;
		} else if (sync_buff_422) {
			// TODO: convert from 422 to 420
			sync_buff_420 = true;
		}
	}
}

void YuvFrame::write_420 ( void )
{
	aloc_420();

	sync_buff_444 = false;
	sync_buff_422 = false;
	sync_buff_420 = true;
}


void YuvFrame::aloc_444 ( void )
{
	if (yBuff == NULL) {
		yBuff = new uchar[nRows * nCols];
	}

	if (buff_444 == NULL) {
		buff_444 = new uchar[2 * nRows * nCols];

		yBuff_444 = yBuff;
		uBuff_444 = buff_444;
		vBuff_444 = buff_444 + (nRows * nCols);
	}
}

void YuvFrame::aloc_422 ( void )
{
	if (yBuff == NULL) {
		yBuff = new uchar[nRows * nCols];
	}

	if (buff_422 == NULL) {
		buff_422 = new uchar[2 * nRows * (nCols / 2)];

		yBuff_422 = yBuff;
		uBuff_422 = buff_422;
		vBuff_422 = buff_422 + (nRows * (nCols / 2));
	}
}

void YuvFrame::aloc_420 ( void )
{
	if (yBuff == NULL) {
		yBuff = new uchar[nRows * nCols];
	}

	if (buff_420 == NULL) {
		buff_420 = new uchar[2 * (nRows / 2) * (nCols / 2)];

		yBuff_420 = yBuff;
		uBuff_420 = buff_420;
		vBuff_420 = buff_420 + ((nRows / 2) * (nCols / 2));
	}
}


void YuvFrame::convert_444_422 ( void )
{
	for (uint i = 0; i < (nRows * nCols) / 2; i++) {
		// yBuff is allways the same
		uBuff_422[i] = uBuff_444[i * 2];
		vBuff_422[i] = vBuff_444[i * 2];
	}
}

void YuvFrame::convert_444_420 ( void )
{
	for (uint l = 0, lr = 0; lr < nRows / 2; l += 2, lr++) {
		for (uint c = 0, cr = 0; cr < nCols / 2; c += 2, cr++) {
			uint pos    = c + (l * nCols);
			uint posRaw = cr + (lr * (nCols / 2));

			// yBuff is allways the same
			uBuff_420[posRaw] = uBuff_444[pos];
			vBuff_420[posRaw] = uBuff_444[pos];
		}
	}
}

void YuvFrame::convert_422_444 ( void )
{
	for (uint i = 0; i < nRows * nCols; i+=2) {
		// yBuff is allways the same
		uBuff_444[i + 1] = uBuff_444[i] = uBuff_422[i / 2];
		vBuff_444[i + 1] = vBuff_444[i] = vBuff_422[i / 2];
	}
}

void YuvFrame::convert_422_420 ( void )
{
	for (uint l = 0, lr = 0; lr < nRows / 2; l += 2, lr++) {
		for (uint c = 0; c < nCols / 2; c++) {
			uint pos    = c + (l * nCols);
			uint posRaw = c + (lr * (nCols / 2));

			// yBuff is allways the same
			uBuff_422[posRaw] = uBuff_420[pos];
			vBuff_422[posRaw] = uBuff_420[pos];
		}
	}
}

void YuvFrame::convert_420_444 ( void )
{
	for (uint r = 0; r < nRows; r++) {
		for (uint c = 0; c < nCols; c++) {
			uBuff_444[c + (r * nCols)] = uBuff_420[(c / 2) + ((r / 2) * (nCols / 2))];
			vBuff_444[c + (r * nCols)] = vBuff_420[(c / 2) + ((r / 2) * (nCols / 2))];
		}
	}
}

void YuvFrame::convert_420_422 ( void )
{
	for (uint r = 0; r < nRows; r++) {
		for (uint c = 0; c < nCols / 2; c++) {
			uBuff_422[c + (r * nCols)] = uBuff_420[(c / 2) + ((r / 2) * (nCols / 2))];
			vBuff_422[c + (r * nCols)] = vBuff_420[(c / 2) + ((r / 2) * (nCols / 2))];
		}
	}
}

void YuvFrame::getBlock ( Block& b , uint r, uint c, uchar* frameBuff )
{
	uint  bRows = b.getNRows();
	uint  bCols = b.getNCols();
	uchar* buff = b.getBuff();

	uint br; // Buffer row index
	uint bc; // Buffer column index
	uint fr; // Frame row index
	uint fc; // Frame column index
	uint er = ((r + bRows) > nRows ? (nRows) : (r + bRows)); // Last frame row
	uint ec = ((c + bCols) > nCols ? (nCols) : (c + bCols)); // Last frame column

	for (br = 0, fr = r; fr < er; br++, fr++) {
		for (bc = 0, fc = c; fc < ec; bc++, fc++) {
			buff[(br * bCols) + bc] = frameBuff[(fr * nCols) + fc];
		}

		for (; bc < bCols; bc++) {
			buff[(br * bCols) + bc] = 0;
		}
	}

	for (; br < bRows; br++) {
		for (bc = 0; bc < bCols; bc++) {
			buff[(br * bCols) + bc] = 0;
		}
	}
}

void YuvFrame::putBlock ( const Block& b , uint r, uint c, uchar* frameBuff )
{
	uint  bRows = b.getNRows();
	uint  bCols = b.getNCols();
	uchar* buff = b.getBuff();

	uint br; // Buffer row index
	uint bc; // Buffer column index
	uint fr; // Frame row index
	uint fc; // Frame column index
	uint er = ((r + bRows) > nRows ? (nRows) : (r + bRows)); // Last frame row
	uint ec = ((c + bCols) > nCols ? (nCols) : (c + bCols)); // Last frame column

	for (br = 0, fr = r; fr < er; br++, fr++) {
		for (bc = 0, fc = c; fc < ec; bc++, fc++) {
			frameBuff[(fr * nCols) + fc] = buff[(br * bCols) + bc];
		}
	}
}


// EOF =========================================================================
