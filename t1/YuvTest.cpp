#include "YUV.h"

int main(int argc, char** argv) {
	YUV src(argv[1]);
	
	YUV dst((char*)"output.yuv", src.getNRows(), src.getNCols(), src.getFps(), src.getType());

	while(src.readFrame() == 0) {
		src.buffCopy(dst);
		dst.appendFrame();
	}
}
