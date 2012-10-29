
//includes


class YuvVideo{
	
public:
	YuvVideo(char* filename); //read
	YuvVideo(char* filename, cols, rows, fps, type); //write
	

	YuvFrame getCurrentFrame();
	
	int readFrame();
	int appendFrame();
	void rewind();
	void setSubsampling(factor);

private:
	
	rows cols etc...
	FILE* fp;
	unsigned char* buffer;

	YuvFrame currentFrame; //currentFrame buffer is the same as the video buffer
}
