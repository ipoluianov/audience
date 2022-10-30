#include "system.h"
#include "elapsedtimer.h"

void ElapsedTimer::reset()
{
	time_ = app.sysTickValue();
}

unsigned long long ElapsedTimer::elapsed()
{
	unsigned long long now = app.sysTickValue();
	return now - time_;
}
