#ifndef _BINARY_SERVER_H_
#define _BINARY_SERVER_H_

#include <binary_server/binary_connection.h>
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"


class BinaryServer
{
public:
	void start();
	void stop();
	virtual ~BinaryServer() { }

	static BinaryServer & get();

	void process();

private:
	BinaryServer();

	struct tcp_pcb *serverPcb_ = nullptr;

	static err_t callback_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
	static err_t callback_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
	static void callback_error(void *arg, err_t err);
	static err_t callback_poll(void *arg, struct tcp_pcb *tpcb);
	static err_t callback_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);

	static void closeConnection(struct tcp_pcb *tpcb, struct BinaryConnection *es);

	// Connections management
	static const int COUNT_OF_ALLOWED_CONNECTIONS = 3;
	BinaryConnection conns[COUNT_OF_ALLOWED_CONNECTIONS];
	bool conns_busy[COUNT_OF_ALLOWED_CONNECTIONS];
	BinaryConnection * getConnection();
	void freeConnection(BinaryConnection * conn);
};

#endif // _BINARY_SERVER_H_
