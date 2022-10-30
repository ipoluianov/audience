#ifndef _DISPLAY_H_
#define _DISPLAY_H_

class Display
{
public:
	Display() {}
	virtual ~Display() {}

	void init();
	void process();
private:

	void sendCommand(char ch);
	void sendData(char ch);
	void setOutput(char data);

};

#endif // _DISPLAY_H_
