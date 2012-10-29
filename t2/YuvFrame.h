

class YuvFrame{
	
	YuvFrame(...) //buffer is the same as parent video
	
	
public:
	YuvBlock getBlock(x,y, nC, nR, type);//este?
	void getBlock(x,y, nC, nR, type, YuvBlock(*|&) block); //ou este? assim podemos usar sempre o mesmo block
	void setBlock(x,y, nC, nR, type, YuvBlock block);
	uint getYPixel(x,y);
	uint getUPixel(x,y);
	uint getVPixel(x,y);
	
	void toYUV444(unsigned char* buffer);
	dumb toRGB(); //HOW!?! Isto é que não sei,
	//é o player o responsavel por ter ambos os buffers (444 e RGB)?
	//podemos por metodos estáticos para as conversões que o player chama
	// e dá os buffers
	
	
private:
	uint nCols, nRows, type;
	unsigned char* buffer;
	unsigned char* yBuffer;
	unsigned char* uBuffer;
	unsigned char* vBuffer;
	
	
	
	
	


}
