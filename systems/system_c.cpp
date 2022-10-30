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


void system_dac1_dma_it(void)
{
	app.dac()->dmaIT1();
}

void system_dac2_dma_it(void)
{
	app.dac()->dmaIT2();
}
