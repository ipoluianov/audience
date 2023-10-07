#include "system_c.h"
#include "system.h"

System app;

void system_init(void)
{
	app.init();
}

void system_process(void)
{
	app.process();
}

void system_systick(void)
{
	app.systick();
}

void system_tim1(void)
{
}

void system_timer1(void)
{
	app.timer1();
}

void system_add_data(char * data, int size)
{
	app.dac()->addData(data, size);
}

