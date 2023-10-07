#include "system.h"
#include "lwip.h"

System::System()
{
}

void System::init()
{
	dac_.init();

	//TelnetServer::get().start();
	//BinaryServer::get().start();

	udpServer_.init();
	//display_.init();

	//scan_.start();
}

void System::process()
{
  //MX_LWIP_Process();

  dac_.process();
  //display_.process();

  //TelnetServer::get().process();
  //BinaryServer::get().process();

  //udpServer_.process();
}

void System::systick()
{
	//__disable_irq();
	systickvalue_++;
	//__enable_irq();
}

unsigned long long System::sysTickValue()
{
	unsigned long long result;
	//__disable_irq();
	result = systickvalue_;
	//__enable_irq();
	return result;
}

void System::timer1()
{
	dac_.timer1();
}
