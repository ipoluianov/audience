#include "udp_server.h"
#include "system.h"

void udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,const  struct ip4_addr *addr, short unsigned int port)
{
    if (p != NULL) {
        /* send received packet back to sender */
        udp_sendto(pcb, p, addr, port);
        app.dac()->addFrame(((char *) p->payload) + 5 , ((char *) p->payload)[1], (p->len - 5));

        /* free the pbuf */
        pbuf_free(p);
    }
}

void UdpServer::init()
{
	struct udp_pcb * pcb;

	/* get new pcb */
	pcb = udp_new();
	if (pcb == NULL) {
		LWIP_DEBUGF(UDP_DEBUG, ("udp_new failed!\n"));
		return;
	}

	/* bind to any IP address on port 7 */
	if (udp_bind(pcb, IP_ADDR_ANY, 7) != ERR_OK) {
		LWIP_DEBUGF(UDP_DEBUG, ("udp_bind failed!\n"));
		return;
	}

	/* set udp_echo_recv() as callback function
	   for received packets */
	udp_recv(pcb, udp_echo_recv, NULL);
}

void UdpServer::process()
{
}
