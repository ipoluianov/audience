#ifndef _DAC_H_
#define _DAC_H_

#include "../utils/stream/stream.h"

class Dac
{
public:
	Dac() {}
	virtual ~Dac() {}

	void addData(char * data, int size);

	void init();
	void process();

	void setChannel(int index, unsigned int code);

	void timer1();
	void dmaIT1();
	void dmaIT2();

	int currentDataSize();

	bool busy_ = false;
private:
	//static const int FRAMES_COUNT = 100;
	//Frame frames_[FRAMES_COUNT];

	static const int bufferSizeSamples_ = 1000;

	int inputIndex_ = 0;
	int outputIndex_ = 0;

	int channelA_[bufferSizeSamples_];
	int channelB_[bufferSizeSamples_];

	//Frame * currentFrame_ = nullptr;
	//int playingPoint_ = 0;

	//int rateCounter_ = 0;
	//int rateDiv_ = 0;

	//signed short genBuffer_[8192];

	void sendBytes8DMA(unsigned char * bytes);
	void sendBytes8(unsigned char * bytes);
	void sendBytes1DMA(unsigned char * bytes);
	void sendBytes1(unsigned char * bytes);

};

#endif // _DAC_H_
