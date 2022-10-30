#ifndef _DAC_H_
#define _DAC_H_

#include "../utils/stream/stream.h"

class Dac
{
public:
	Dac() {}
	virtual ~Dac() {}

	struct Frame
	{
		int channelA[512];
		int channelB[512];
		int size = 0;
	};

	void addFrame(char * data, int bits, int count);


	void init();
	void process();
	void setChannel(int index, unsigned int code);

	void timer1();
	void dmaIT1();
	void dmaIT2();

	void generateSin();
	void generateMiander();

	bool busy_ = false;
private:
	static const int FRAMES_COUNT = 100;
	Frame frames_[FRAMES_COUNT];
	int inputIndex_ = 0;
	int outputIndex_ = 0;

	Frame * currentFrame_ = nullptr;
	int playingPoint_ = 0;

	int rateCounter_ = 0;
	int rateDiv_ = 0;

	signed short genBuffer_[8192];

	void sendBytes8DMA(unsigned char * bytes);
	void sendBytes8(unsigned char * bytes);
	void sendBytes1DMA(unsigned char * bytes);
	void sendBytes1(unsigned char * bytes);

};

#endif // _DAC_H_
