#include <binary_server/binary_server.h>


BinaryServer::BinaryServer()
{
	for (int i = 0; i < COUNT_OF_ALLOWED_CONNECTIONS; i++)
		conns_busy[i] = false;
}

BinaryServer & BinaryServer::get()
{
	static BinaryServer instance;
	return instance;
}

void BinaryServer::start()
{
	serverPcb_ = tcp_new();

	if (serverPcb_ != NULL)
	{
		err_t err;
		err = tcp_bind(serverPcb_, IP_ADDR_ANY, 1753); // binary port = 1753
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

void BinaryServer::stop()
{
}

void BinaryServer::process()
{
	for (int i = 0; i < COUNT_OF_ALLOWED_CONNECTIONS; i++)
	{
		if (conns_busy[i])
		{
			conns[i].process();
		}
	}
}

err_t BinaryServer::callback_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	err_t ret_err;
	struct BinaryConnection *conn;

	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);

	// set priority for the newly accepted tcp connection newpcb
	tcp_setprio(newpcb, TCP_PRIO_MAX);

	// allocate structure es to maintain tcp connection informations
	conn = get().getConnection();
	if (conn != NULL)
	{
		conn->start(newpcb);
		conn->state = BinaryConnection::ES_ACCEPTED;

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

err_t BinaryServer::callback_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	struct BinaryConnection *conn;
	err_t ret_err;

	LWIP_ASSERT("arg != NULL", arg != NULL);

	conn = (struct BinaryConnection *)arg;

	// if we receive an empty tcp frame from client => close connection
	if (p == NULL)
	{
		// remote host closed connection
		conn->state = BinaryConnection::ES_CLOSING;
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
	else if(conn->state == BinaryConnection::ES_ACCEPTED)
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

void BinaryServer::callback_error(void *arg, err_t err)
{
	  struct BinaryConnection *es;

	  LWIP_UNUSED_ARG(err);

	  es = (struct BinaryConnection *)arg;
	  if (es != NULL)
	  {
		  get().freeConnection(es);
	  }
}

err_t BinaryServer::callback_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t ret_err;
	ret_err = ERR_OK;
	return ret_err;
}

err_t BinaryServer::callback_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	  return ERR_OK;
}

void BinaryServer::closeConnection(struct tcp_pcb *tpcb, struct BinaryConnection *es)
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

BinaryConnection * BinaryServer::getConnection()
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

void BinaryServer::freeConnection(BinaryConnection * conn)
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
