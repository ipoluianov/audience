#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "main.h"

#include "dac/dac.h"

#include "elapsedtimer/elapsedtimer.h"
#include "telnet_server/telnet_server.h"
#include "binary_server/binary_server.h"
#include "binary_server/binary_server_api.h"
#include "udp_server/udp_server.h"
#include "display/display.h"

class System
{
public:
	System();
	virtual ~System() { }

	void init();
	void process();
	void systick();
	unsigned long long sysTickValue();

	void timer1();
	Dac * dac() { return &dac_; }
	BinaryServerApi * api() { return &api_; }
	Display * display() { return &display_; }

	UdpServer * udpServer() { return &udpServer_; }
private:
	Dac dac_;
	Display display_;
	BinaryServerApi api_;
	UdpServer udpServer_;
	unsigned long long systickvalue_ = 0;
};

extern System app;

#endif // _SYSTEM_H_
