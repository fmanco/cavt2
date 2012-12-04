//==============================================================================
// YuvAnalyse.cpp
//
// Authors : Filipe Manco <filipe.manco@ua.pt>
// Authors : Frederico Honório <fredericohonorio@ua.pt>
//==============================================================================

#include "base.h"
#include "YuvAnalyse.h"
#include "YuvFrame.h"
#include <math.h>


//==============================================================================

void YuvAnalyse::psnr(YuvFrame& a, YuvFrame& b, float* psnrY, float* psnrU, float* psnrV){

	float mseY, mseU, mseV;

	mse(a, b, &mseY, &mseU, &mseV);

	*psnrY = 10.0 * log10f((255.0*255.0)/mseY);
	*psnrU = 10.0 * log10f((255.0*255.0)/mseU);
	*psnrV = 10.0 * log10f((255.0*255.0)/mseV);
}

void YuvAnalyse::mse(YuvFrame& a, YuvFrame& b, float* mseY, float* mseU, float* mseV){

	uchar *aBufferY = a.get_read_yBuff();
	uchar *bBufferY = b.get_read_yBuff();
	uchar *aBufferU = a.get_read_uBuff();
	uchar *bBufferU = b.get_read_uBuff();
	uchar *aBufferV = a.get_read_vBuff();
	uchar *bBufferV = b.get_read_vBuff();

	int sizeY = a.getYCols()*a.getYRows();
	int sizeU = a.getUCols()*a.getURows();
	int sizeV = a.getVCols()*a.getVRows();
	
	int sumY = 0, sumU = 0, sumV = 0;

	for (int i = 0; i < sizeY; i++){
		sumY+=(aBufferY[i] - bBufferY[i]) * (aBufferY[i] - bBufferY[i]);
	}
	
	for (int i = 0; i < sizeU; i++){
		sumU+=(aBufferU[i] - bBufferU[i]) * (aBufferU[i] - bBufferU[i]);
	}

	for (int i = 0; i < sizeV; i++){
		sumV+=(aBufferV[i] - bBufferV[i]) * (aBufferV[i] - bBufferV[i]);
	}

	*mseY = sumY*(1.0/((float)sizeY));
	*mseU = sumU*(1.0/((float)sizeU));
	*mseV = sumV*(1.0/((float)sizeV));
}


void YuvAnalyse::entropy(YuvFrame& frame, float* entY, float* entU, float* entV){

	uchar *aBufferY = frame.get_read_yBuff();
	uchar *aBufferU = frame.get_read_uBuff();
	uchar *aBufferV = frame.get_read_vBuff();

	int sizeY = frame.getYCols()*frame.getYRows();
	int sizeU = frame.getUCols()*frame.getURows();
	int sizeV = frame.getVCols()*frame.getVRows();

	float entropyY=0.0;
	float entropyU=0.0;
	float entropyV=0.0;
	uint occurrenceY[255] = {0};
	uint occurrenceU[255] = {0};
	uint occurrenceV[255] = {0};
	float prob;
	int i;

	for(i = 0 ; i < sizeY; i++) {
		occurrenceY[aBufferY[i]]++;
	}	
	
	for(i = 0 ; i < sizeU; i++) {
		occurrenceU[aBufferU[i]]++;
	}	

	for(i = 0 ; i < sizeV; i++) {
		occurrenceV[aBufferV[i]]++;
	}	


	for (i = 0; i < 255; i++){
		if (occurrenceY[i]>0) {
			prob = ((float) occurrenceY[i])/(float)sizeY;
			entropyY += prob*log2f(prob);
		}

		if (occurrenceU[i]>0) {
			prob = ((float) occurrenceU[i])/(float)sizeU;
			entropyU += prob*log2f(prob);
		}

		if (occurrenceV[i]>0) {
			prob = ((float) occurrenceV[i])/(float)sizeV;
			entropyV += prob*log2f(prob);
		}
	}

	*entY = entropyY;
	*entU = entropyU;
	*entV = entropyV;
}
// EOF =========================================================================
