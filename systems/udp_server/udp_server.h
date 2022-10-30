#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_

#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"


class UdpServer
{
public:
	UdpServer() {}
	virtual ~UdpServer() {}

	void init();
	void process();
private:
};

#endif // _UDP_SERVER_H_
