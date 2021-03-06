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
	: type(YUV444), nRows(_nRows), nCols(_nCols)
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

YuvFrame::YuvFrame ( Type _type, uint _nRows, uint _nCols )
	: type(_type), nRows(_nRows), nCols(_nCols)
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


YuvFrame::YuvFrame ( int _type, uint _nRows, uint _nCols )
	: nRows(_nRows), nCols(_nCols)
{
	switch(_type) {
		case 444:
		type = YuvFrame::YUV444;
		break;

		case 422:
		type = YuvFrame::YUV422;
		break;

		case 420:
		type = YuvFrame::YUV420;
		break;

		default:
		type = YuvFrame::YUV444;
		break;
	}

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
	type = obj.type;

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

YuvFrame::Type YuvFrame::getType ( void ) const
{
	return type;
}

void YuvFrame::setType ( Type _type )
{
	type = _type;
}

void YuvFrame::setType ( uint _type )
{
	switch(_type) {
		case 444:
		type = YuvFrame::YUV444;
		break;

		case 422:
		type = YuvFrame::YUV422;
		break;

		case 420:
		type = YuvFrame::YUV420;
		break;

		default:
		type = YuvFrame::YUV444;
		break;
	}
}

uint YuvFrame::getNRows ( void ) const
{
	return nRows;
}

uint YuvFrame::getNCols ( void ) const
{
	return nCols;
}


uint YuvFrame::getYRows ( void ) const
{
	return getYRows(type);
}

uint YuvFrame::getYCols ( void ) const
{
	return getYCols(type);
}

uint YuvFrame::getURows ( void ) const
{
	return getURows(type);
}

uint YuvFrame::getUCols ( void ) const
{
	return getUCols(type);
}

uint YuvFrame::getVRows ( void ) const
{
	return getVRows(type);
}

uint YuvFrame::getVCols ( void ) const
{
	return getVCols(type);
}


uint YuvFrame::getYRows ( YuvFrame::Type _type ) const
{
	return nRows;
}

uint YuvFrame::getYCols ( YuvFrame::Type _type ) const
{
	return nCols;
}

uint YuvFrame::getURows ( YuvFrame::Type _type ) const
{
	switch(_type) {
		case YUV444:
			return nRows;

		case YUV422:
			return nRows;

		case YUV420:
			return (nRows / 2);
	}

	return 0;
}

uint YuvFrame::getUCols ( YuvFrame::Type _type ) const
{
	switch(_type) {
		case YUV444:
			return nCols;

		case YUV422:
			return (nCols / 2);

		case YUV420:
			return (nCols / 2);
	}

	return 0;
}

uint YuvFrame::getVRows ( YuvFrame::Type _type ) const
{
	switch(_type) {
		case YUV444:
			return nRows;

		case YUV422:
			return nRows;

		case YUV420:
			return (nRows / 2);
	}

	return 0;
}

uint YuvFrame::getVCols ( YuvFrame::Type _type ) const
{
	switch(_type) {
		case YUV444:
			return nCols;

		case YUV422:
			return (nCols / 2);

		case YUV420:
			return (nCols / 2);
	}

	return 0;
}


//==============================================================================

void YuvFrame::getYBlock ( Block &b, uint r, uint c )
{
	switch (type) {
		case YUV444:
			read_444();
			getBlock(b, r, c, yBuff_444, nRows, nCols);
			break;

		case YUV422:
			read_422();
			getBlock(b, r, c, yBuff_422, nRows, nCols);
			break;

		case YUV420:
			read_420();
			getBlock(b, r, c, yBuff_420, nRows, nCols);
			break;
	}
}

void YuvFrame::getUBlock ( Block &b, uint r, uint c )
{
	switch (type) {
		case YUV444:
			read_444();
			getBlock(b, r, c, uBuff_444, nRows, nCols);
			break;

		case YUV422:
			read_422();
			getBlock(b, r, c, uBuff_422, nRows, nCols / 2);
			break;

		case YUV420:
			read_420();
			getBlock(b, r, c, uBuff_420, nRows / 2, nCols / 2);
			break;
	}
}

void YuvFrame::getVBlock ( Block &b, uint r, uint c )
{
	switch (type) {
		case YUV444:
			read_444();
			getBlock(b, r, c, vBuff_444, nRows, nCols);
			break;

		case YUV422:
			read_422();
			getBlock(b, r, c, vBuff_422, nRows, nCols / 2);
			break;

		case YUV420:
			read_420();
			getBlock(b, r, c, vBuff_420, nRows / 2, nCols / 2);
			break;
	}
}

void YuvFrame::putYBlock ( const Block &b, uint r, uint c )
{
	switch (type) {
		case YUV444:
			read_444();
			write_444();
			putBlock(yBuff_444, nRows, nCols, b, r, c);
			break;

		case YUV422:
			read_422();
			write_422();
			putBlock(yBuff_422, nRows, nCols, b, r, c);
			break;

		case YUV420:
			read_420();
			write_420();
			putBlock(yBuff_420, nRows, nCols, b, r, c);
			break;
	}
}

void YuvFrame::putUBlock ( const Block &b, uint r, uint c )
{
	switch (type) {
		case YUV444:
			read_444();
			write_444();
			putBlock(uBuff_444, nRows, nCols, b, r, c);
			break;

		case YUV422:
			read_422();
			write_422();
			putBlock(uBuff_422, nRows, nCols / 2, b, r, c);
			break;

		case YUV420:
			read_420();
			write_420();
			putBlock(uBuff_420, nRows / 2, nCols / 2, b, r, c);
			break;
	}
}

void YuvFrame::putVBlock ( const Block &b, uint r, uint c )
{
	switch (type) {
		case YUV444:
			read_444();
			write_444();
			putBlock(vBuff_444, nRows, nCols, b, r, c);
			break;

		case YUV422:
			read_422();
			write_422();
			putBlock(vBuff_422, nRows, nCols / 2, b, r, c);
			break;

		case YUV420:
			read_420();
			write_420();
			putBlock(vBuff_420, nRows / 2, nCols / 2, b, r, c);
			break;
	}
}


//==============================================================================

uchar YuvFrame::getYPixel ( uint r, uint c )
{
	switch (type) {
		case YUV444:
			if (r >= nRows || c >= nCols)
				return 0;

			read_444();
			return yBuff_444[(r * nCols) + c];

		case YUV422:
			if (r >= nRows || c >= nCols)
				return 0;

			read_422();
			return yBuff_422[(r * nCols) + c];

		case YUV420:
			if (r >= nRows || c >= nCols)
				return 0;

			read_420();
			return yBuff_420[(r * nCols) + c];
	}

	return 0; // The compiler is complaining. But this should never get here.
}

uchar YuvFrame::getUPixel ( uint r, uint c )
{
	switch (type) {
		case YUV444:
			if (r >= nRows || c >= nCols)
				return 0;

			read_444();
			return uBuff_444[(r * nCols) + c];

		case YUV422:
			if (r >= nRows || c >= (nCols / 2))
				return 0;

			read_422();
			return uBuff_422[(r * (nCols / 2)) + c];

		case YUV420:
			if (r >= (nRows / 2) || c >= (nCols / 2))
				return 0;

			read_420();
			return uBuff_420[(r * (nCols / 2)) + c];
	}

	return 0; // The compiler is complaining. But this should never get here.
}

uchar YuvFrame::getVPixel ( uint r, uint c )
{
	switch (type) {
		case YUV444:
			if (r >= nRows || c >= nCols)
				return 0;

			read_444();
			return vBuff_444[(r * nCols) + c];

		case YUV422:
			if (r >= nRows || c >= (nCols / 2))
				return 0;

			read_422();
			return vBuff_422[(r * (nCols / 2)) + c];

		case YUV420:
			if (r >= (nRows / 2) || c >= (nCols / 2))
				return 0;

			read_420();
			return vBuff_420[(r * (nCols / 2)) + c];
	}

	return 0; // The compiler is complaining. But this should never get here.
}

void YuvFrame::putYPixel ( uint r, uint c, uchar pixel )
{
	switch (type) {
		case YUV444:
			if (r >= nRows || c >= nCols)
				return;

			read_444();
			write_444();
			yBuff_444[(r * nCols) + c] = pixel;
			break;

		case YUV422:
			if (r >= nRows || c >= nCols)
				return;

			read_422();
			write_422();
			yBuff_422[(r * nCols) + c] = pixel;
			break;

		case YUV420:
			if (r >= nRows || c >= nCols)
				return;

			read_420();
			write_420();
			yBuff_420[(r * nCols) + c] = pixel;
			break;
	}
}

void YuvFrame::putUPixel ( uint r, uint c, uchar pixel )
{
	switch (type) {
		case YUV444:
			if (r >= nRows || c >= nCols)
				return;

			read_444();
			write_444();
			uBuff_444[(r * nCols) + c] = pixel;
			break;

		case YUV422:
			if (r >= nRows || c >= (nCols / 2))
				return;

			read_422();
			write_422();
			uBuff_422[(r * (nCols / 2)) + c] = pixel;
			break;

		case YUV420:
			if (r >= (nRows / 2) || c >= (nCols / 2))
				return;

			read_420();
			write_420();
			uBuff_420[(r * (nCols / 2)) + c] = pixel;
			break;
	}
}

void YuvFrame::putVPixel ( uint r, uint c, uchar pixel )
{
	switch (type) {
		case YUV444:
			if (r >= nRows || c >= nCols)
				return;

			read_444();
			write_444();
			vBuff_444[(r * nCols) + c] = pixel;
			break;

		case YUV422:
			if (r >= nRows || c >= (nCols / 2))
				return;

			read_422();
			write_422();
			vBuff_422[(r * (nCols / 2)) + c] = pixel;
			break;

		case YUV420:
			if (r >= (nRows / 2) || c >= (nCols / 2))
				return;

			read_420();
			write_420();
			vBuff_420[(r * (nCols / 2)) + c] = pixel;
			break;
	}
}


//==============================================================================

uchar* YuvFrame::get_read_yBuff ( void )
{
	return get_read_yBuff(type);
}

uchar* YuvFrame::get_read_uBuff ( void )
{
	return get_read_uBuff(type);
}

uchar* YuvFrame::get_read_vBuff ( void )
{
	return get_read_vBuff(type);
}

uchar* YuvFrame::get_read_yBuff ( YuvFrame::Type _type )
{
	switch (_type) {
		case YUV444:
			read_444();
			return yBuff_444;

		case YUV422:
			read_422();
			return yBuff_422;

		case YUV420:
			read_420();
			return yBuff_420;
	}

	return NULL;
}

uchar* YuvFrame::get_read_uBuff ( YuvFrame::Type _type )
{
	switch (_type) {
		case YUV444:
			read_444();
			return uBuff_444;

		case YUV422:
			read_422();
			return uBuff_422;

		case YUV420:
			read_420();
			return uBuff_420;
	}

	return NULL;
}

uchar* YuvFrame::get_read_vBuff ( YuvFrame::Type _type )
{
	switch (_type) {
		case YUV444:
			read_444();
			return vBuff_444;

		case YUV422:
			read_422();
			return vBuff_422;

		case YUV420:
			read_420();
			return vBuff_420;
	}

	return NULL;
}


uchar* YuvFrame::get_write_yBuff ( void )
{
	return get_write_yBuff(type);
}

uchar* YuvFrame::get_write_uBuff ( void )
{
	return get_write_uBuff(type);
}

uchar* YuvFrame::get_write_vBuff ( void )
{
	return get_write_vBuff(type);
}

uchar* YuvFrame::get_write_yBuff ( YuvFrame::Type _type )
{
	switch (_type) {
		case YUV444:
			write_444();
			return yBuff_444;

		case YUV422:
			write_422();
			return yBuff_422;

		case YUV420:
			write_420();
			return yBuff_420;
	}

	return NULL;
}

uchar* YuvFrame::get_write_uBuff ( YuvFrame::Type _type )
{
	switch (_type) {
		case YUV444:
			write_444();
			return uBuff_444;

		case YUV422:
			write_422();
			return uBuff_422;

		case YUV420:
			write_420();
			return uBuff_420;
	}

	return NULL;
}

uchar* YuvFrame::get_write_vBuff ( YuvFrame::Type _type )
{
	switch (_type) {
		case YUV444:
			write_444();
			return vBuff_444;

		case YUV422:
			write_422();
			return vBuff_422;

		case YUV420:
			write_420();
			return vBuff_420;
	}

	return NULL;
}


//==============================================================================

int YuvFrame::cmp (YuvFrame &oth)
{
	if ((getNRows() != oth.getNRows() ) || (getNCols() != oth.getNCols()))
		return -1;

	uint err   = 0;
	uint count = 0;

	uchar* tbuff; // this' buffer
	uchar* obuff; // other's buffer

	tbuff = get_read_yBuff();
	obuff = oth.get_read_yBuff(type);
	count += getYRows() * getYCols();

	for (uint i = 0; i < (getYRows() * getYCols()); i++) {
		err += abs(tbuff[i] - obuff[i]);
	}

	tbuff = get_read_uBuff();
	obuff = oth.get_read_uBuff(type);
	count += getURows() * getUCols();

	for (uint i = 0; i < (getURows() * getUCols()); i++) {
		err += abs(tbuff[i] - obuff[i]);
	}

	tbuff = get_read_vBuff();
	obuff = oth.get_read_vBuff(type);
	count += getVRows() * getVCols();

	for (uint i = 0; i < (getVRows() * getVCols()); i++) {
		err += abs(tbuff[i] - obuff[i]);
	}

	return (int)(err / count); // Hopefully no overflows, otherwise, well...
}

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

void YuvFrame::getBlock ( Block& blk , uint r, uint c, uchar* fBuff, uint fRows, uint fCols )
{
	uint br; // Block row index
	uint bc; // Block column index

	uint fr; // Frame row index
	uint fc; // Frame column index

	uint er = ((r + blk.nRows) > fRows ? (fRows) : (r + blk.nRows)); // Last frame row
	uint ec = ((c + blk.nCols) > fCols ? (fCols) : (c + blk.nCols)); // Last frame column

	for (br = 0, fr = r; fr < er; br++, fr++) {
		for (bc = 0, fc = c; fc < ec; bc++, fc++) {
			blk.buff[(br * blk.nCols) + bc] = fBuff[(fr * fCols) + fc];
		}

		for (; bc < blk.nCols; bc++) {              // Fill the remaining
			blk.buff[(br * blk.nCols) + bc] = 0;        //  block columns with zeros
		}                                         //  if there are any
	}

	for (; br < blk.nRows; br++) {                  // Fill the remaining
		for (bc = 0; bc < blk.nCols; bc++) {        //  block rows with zeros
			blk.buff[(br * blk.nCols) + bc] = 0;        //  if there are any
		}
	}
}

void YuvFrame::putBlock ( uchar* fBuff, uint fRows, uint fCols, const Block& blk, uint r, uint c )
{
	uint br; // Block row index
	uint bc; // Block column index

	uint fr; // Frame row index
	uint fc; // Frame column index

	uint er = ((r + blk.nRows) > fRows ? (fRows) : (r + blk.nRows)); // Last frame row
	uint ec = ((c + blk.nCols) > fCols ? (fCols) : (c + blk.nCols)); // Last frame column

	for (br = 0, fr = r; fr < er; br++, fr++) {
		for (bc = 0, fc = c; fc < ec; bc++, fc++) {
			fBuff[(fr * fCols) + fc] = blk.buff[(br * blk.nCols) + bc];
		}
	}
}


// EOF =========================================================================
