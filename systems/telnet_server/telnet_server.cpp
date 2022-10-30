#include "telnet_server.h"


TelnetServer::TelnetServer()
{
	for (int i = 0; i < COUNT_OF_ALLOWED_CONNECTIONS; i++)
		conns_busy[i] = false;
}

TelnetServer & TelnetServer::get()
{
	static TelnetServer instance;
	return instance;
}

void TelnetServer::start()
{
	serverPcb_ = tcp_new();

	if (serverPcb_ != NULL)
	{
		err_t err;
		err = tcp_bind(serverPcb_, IP_ADDR_ANY, 23); // telnet port = 32
		if (err == ERR_OK)
		{
			serverPcb_ = tcp_listen(serverPcb_);
			tcp_accept(serverPcb_, callback_accept);
		}
		else
		{
			memp_free(MEMP_TCP_PCB, serverPcb_);
		}
	}
}

void TelnetServer::stop()
{
}

void TelnetServer::process()
{
	for (int i = 0; i < COUNT_OF_ALLOWED_CONNECTIONS; i++)
	{
		if (conns_busy[i])
		{
			conns[i].process();
		}
	}
}

err_t TelnetServer::callback_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	err_t ret_err;
	struct TelnetConnection *conn;

	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);

	// set priority for the newly accepted tcp connection newpcb
	tcp_setprio(newpcb, TCP_PRIO_MIN);

	// allocate structure es to maintain tcp connection informations
	conn = get().getConnection();
	if (conn != NULL)
	{
		conn->start(newpcb);
		conn->state = TelnetConnection::ES_ACCEPTED;

		// pass newly allocated es structure as argument to newpcb
		tcp_arg(newpcb, conn);
		// initialize lwip tcp_recv callback function for newpcb
		tcp_recv(newpcb, callback_recv);
		// initialize lwip tcp_err callback function for newpcb
		tcp_err(newpcb, callback_error);
		// initialize lwip tcp_poll callback function for newpcb
		tcp_poll(newpcb, callback_poll, 0);
		// initialize LwIP tcp_sent callback function
		tcp_sent(newpcb, callback_sent);

		ret_err = ERR_OK;
	}
	else
	{
		// close tcp connection
		closeConnection(newpcb, conn);
		// return memory error
		ret_err = ERR_MEM;
	}
	return ret_err;
}

err_t TelnetServer::callback_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	struct TelnetConnection *conn;
	err_t ret_err;

	LWIP_ASSERT("arg != NULL", arg != NULL);

	conn = (struct TelnetConnection *)arg;

	// if we receive an empty tcp frame from client => close connection
	if (p == NULL)
	{
		// remote host closed connection
		conn->state = TelnetConnection::ES_CLOSING;
		// we're done sending, close connection
		closeConnection(tpcb, conn);
		ret_err = ERR_OK;
	}
	else if(err != ERR_OK) // else : a non empty frame was received from client but for some reason err != ERR_OK
	{
		// free received pbuf
		if (p != NULL)
		{
			pbuf_free(p);
		}
		ret_err = err;
	}
	else if(conn->state == TelnetConnection::ES_ACCEPTED)
	{
		struct pbuf * current_buf = p;

		while (current_buf != 0)
		{
			conn->input(static_cast<char *>(current_buf->payload), current_buf->len);
			current_buf = current_buf->next;
		}

		tcp_recved(tpcb, p->tot_len);
		pbuf_free(p);

		ret_err = ERR_OK;
	}
	else
	{
		tcp_recved(tpcb, p->tot_len);
		pbuf_free(p);
		ret_err = ERR_OK;
	}
	return ret_err;
}

void TelnetServer::callback_error(void *arg, err_t err)
{
	  struct TelnetConnection *es;

	  LWIP_UNUSED_ARG(err);

	  es = (struct TelnetConnection *)arg;
	  if (es != NULL)
	  {
		  get().freeConnection(es);
	  }
}

err_t TelnetServer::callback_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t ret_err;
	ret_err = ERR_OK;
	return ret_err;
}

err_t TelnetServer::callback_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	  return ERR_OK;
}

void TelnetServer::closeConnection(struct tcp_pcb *tpcb, struct TelnetConnection *es)
{
	  /* remove all callbacks */
	  tcp_arg(tpcb, NULL);
	  tcp_sent(tpcb, NULL);
	  tcp_recv(tpcb, NULL);
	  tcp_err(tpcb, NULL);
	  tcp_poll(tpcb, NULL, 0);

	  /* delete es structure */
	  if (es != NULL)
	  {
	    get().freeConnection(es);
	  }

	  /* close tcp connection */
	  tcp_close(tpcb);
}

TelnetConnection * TelnetServer::getConnection()
{
	for (int i = 0; i < COUNT_OF_ALLOWED_CONNECTIONS; i++)
	{
		if (!conns_busy[i])
		{
			conns_busy[i] = true;
			return &conns[i];
		}
	}
	return nullptr;
}

void TelnetServer::freeConnection(TelnetConnection * conn)
{
	for (int i = 0; i < COUNT_OF_ALLOWED_CONNECTIONS; i++)
	{
		if (conns_busy[i] && &conns[i] == conn)
		{
			conns[i].stop();
			conns_busy[i] = false;
			return;
		}
	}
}
