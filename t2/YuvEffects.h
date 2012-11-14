//==============================================================================
// YuvEffects.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __YUVEFFECTS_H__
#define __YUVEFFECTS_H__


//==============================================================================

#include "base.h"
#include "YuvFrame.h"


//==============================================================================
class YuvEffects {
public:
	YuvEffects  ( void );
	~YuvEffects (  );

public:
	void apply ( YuvFrame& frame );
	void apply ( const YuvFrame& in, YuvFrame& out );

	void enableColor2BW  ( void );
	void disableColor2BW ( void );

	void enableInvertColors  ( void );
	void disableInvertColors ( void );

	void enableChangeLuminance  ( double factor );
	void disableChangeLuminance ( void );

	void enableResize  ( uint factor, bool improved );
	void disableResize ( void );


private:
	void convertToBW ( YuvFrame& frame );
	void convertToBW ( const YuvFrame& in, YuvFrame& out );

	void invertColors ( YuvFrame& frame );
	void invertColors ( const YuvFrame& in, YuvFrame& out );

	void changeLuminance ( YuvFrame& frame );
	void changeLuminance ( const YuvFrame& in, YuvFrame& out );

	void resize ( YuvFrame& frame );
	void resize ( const YuvFrame& in, YuvFrame& out );
};


//==============================================================================
#endif /* __YUVEFFECTS_H__ */
