#ifndef _TELNET_SERVER_H_
#define _TELNET_SERVER_H_

#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"

#include "telnet_connection.h"

class TelnetServer
{
public:
	void start();
	void stop();
	virtual ~TelnetServer() { }

	static TelnetServer & get();

	void process();

private:
	TelnetServer();


	struct tcp_pcb *serverPcb_ = nullptr;

	static err_t callback_accept(void *arg, struct tcp_pcb *newpcb, err_t err);
	static err_t callback_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
	static void callback_error(void *arg, err_t err);
	static err_t callback_poll(void *arg, struct tcp_pcb *tpcb);
	static err_t callback_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);

	static void closeConnection(struct tcp_pcb *tpcb, struct TelnetConnection *es);

	// Connections management
	static const int COUNT_OF_ALLOWED_CONNECTIONS = 3;
	TelnetConnection conns[COUNT_OF_ALLOWED_CONNECTIONS];
	bool conns_busy[COUNT_OF_ALLOWED_CONNECTIONS];
	TelnetConnection * getConnection();
	void freeConnection(TelnetConnection * conn);
};

#endif // _TELNET_SERVER_H_
