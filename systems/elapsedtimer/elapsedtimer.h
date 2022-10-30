#ifndef _ELAPSED_TIMER_H_
#define _ELAPSED_TIMER_H_

class ElapsedTimer
{
public:
	ElapsedTimer() { }
	virtual ~ElapsedTimer() { }

	void reset();
	unsigned long long elapsed();
private:
	unsigned long long time_ = 0;
};

#endif // _ELAPSED_TIMER_H_
