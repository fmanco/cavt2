//==============================================================================
// YuvAnalyse.h
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#ifndef __YUVANALYSE_H__
#define __YUVANALYSE_H__


//==============================================================================

#include "base.h"
#include "YuvFrame.h"


//==============================================================================

class YuvAnalyse {
public:
	void static psnr(YuvFrame& a, YuvFrame& b, float* psnrY, float* psnrU, float* psnrV);
	void static entropy(YuvFrame& frame, float* entY, float* entU, float* entV);
private:
	void static mse(YuvFrame& a, YuvFrame& b, float* mseY, float* mseU, float* mseV);
};
//==============================================================================
#endif /* __YUVANALYSE_H__ */
