

class YuvReader{ //o YuvShow usa isto, passa a apenas ler input se ouver e chamar o mêtodo display daqui
	
public:
	YuvReader(YuvVideo);
	YuvReader(char* filename); //dps até podemos usar isto para ler de uma bitstream com o intra e inter enconding
	
	int readFrame(); //só chama o readFrame do vídeo
	void displayFrame();
	
	void setResize(YuvResize);
	void unsetResize();
	
	void setEffetcts(YuvEffects);
	void unsetEffetcts();
	
private:

	buffer444?
	bufferRGB?
	
	IplImage *img;
	YuvVideo vid;
	YuvFrame currentFrame;//=vid.getCurrentFrame();
	
	
	
};
