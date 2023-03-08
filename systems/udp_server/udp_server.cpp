#include "udp_server.h"
#include "system.h"
#include "utils/sha256.h"

err_t send_msg_to_dest();

char diagnosticsData[256];

void udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p,const  struct ip4_addr *addr, short unsigned int port)
{
	for (int i = 0; i < 256; i++)
		diagnosticsData[i] = 0;

	struct Sha_256 sha_256;
	uint8_t hash[32];
	sha_256_init(&sha_256, hash);
	sha_256_write(&sha_256, p->payload, p->len);
	sha_256_close(&sha_256);

	diagnosticsData[0] = 0xAA;
	diagnosticsData[1] = 0x55;
	*((uint32_t*)(diagnosticsData + 2)) = HAL_GetTick();
	memcpy(diagnosticsData + 2 + 4, hash, 32);

    if (p != NULL) {
        /* send received packet back to sender */
        char * frameData = (char *) p->payload;
        int frameLen = p->len;

        //udp_sendto(pcb, p, addr, port);

        struct pbuf * p1 = pbuf_alloc(PBUF_TRANSPORT, 256, PBUF_RAM);
        if (p1 != NULL)
		{
			pbuf_take(p1, (char*)diagnosticsData, 256);
			//udp_send(pcb, p1);
			//p1->flags = 2;
			//p1->ref = 1;
			p1->if_idx = 1;

			udp_sendto(pcb, p1, addr, port);
			pbuf_free(p1);
		}

        app.dac()->addFrame(((char *) p->payload) + 5 , ((char *) p->payload)[1], (p->len - 5));

        /* free the pbuf */
        pbuf_free(p);
    }
}

err_t send_msg_to_dest() {
    struct pbuf *p;
	uint8_t data[100]={0};


	  data[0] = 'T';
	  data[1] = 'C';
	  data[2] = '=';
	  data[3] = '0';
	  data[4] = 0xD;


    //sprintf((char*)data, "sending udp client message");

    /* allocate pbuf from pool*/
    //p = pbuf_alloc(PBUF_TRANSPORT,strlen((char*)data), PBUF_POOL);
	p = pbuf_alloc(PBUF_TRANSPORT, 5, PBUF_POOL);

    if (p != NULL)
    {
        /* copy data to pbuf */
        pbuf_take(p, (char*)data, strlen((char*)data));

        /* send udp data */
        //udp_send(upcb, p);

        /* free pbuf */
        pbuf_free(p);
        return ERR_OK;
    }
    return ERR_MEM;
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
