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


/*

int main(int argc, char** argv){
	uint frames = 0;
	float sumPNSR_Y = 0.0f;
	float sumPNSR_U = 0.0f;
	float sumPNSR_V = 0.0f;

	if( argc < 2 ) {
		fprintf( stderr, "Usage: YuvCmp filename\n or YuvCmp filename filename" );
		return 1;
	}

	YUV orig(argv[1]);
	
	if (argc == 2) {
		
		YuvResize resize = YuvResize(2, true);
		YUV reduced = resize.prepareCopy(orig, YuvResize::REDUCE);
		YUV expanded = resize.prepareCopy(reduced, YuvResize::EXPAND);

		//chained conversion
		while (orig.readFrame()!=-1){
			
			resize.reduce(orig, reduced);
			resize.expand(reduced, expanded);
			
			expanded.displayFrame();
			
			sumPNSR_Y += calcPSNR(orig, expanded, 0);
			sumPNSR_U += calcPSNR(orig, expanded, 1);
			sumPNSR_V += calcPSNR(orig, expanded, 2);
			
			frames++;
		}
	} else {
		YUV orig2(argv[2]);
		while (orig.readFrame()!=-1 && orig2.readFrame()!=-1){
			
			
			sumPNSR_Y += calcPSNR(orig, orig2, 0);
			sumPNSR_U += calcPSNR(orig, orig2, 1);
			sumPNSR_V += calcPSNR(orig, orig2, 2);
			
			frames++;
		}
	}
	
	printf("Y: %f, U:%f, V:%f\n", sumPNSR_Y/(float)frames, sumPNSR_U/(float)frames, sumPNSR_V/(float)frames);
	
	return 0;

*/

// EOF =========================================================================
