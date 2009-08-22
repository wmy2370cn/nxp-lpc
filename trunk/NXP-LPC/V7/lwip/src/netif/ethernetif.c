/**
 * @file
 * Ethernet Interface Skeleton
 *
 */

/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/*
 * This file is a skeleton for developing Ethernet network interface
 * drivers for lwIP. Add code to the low_level functions and do a
 * search-and-replace for the word "ethernetif" to replace it with
 * something that better describes your network interface.
 */

#include "lwip/opt.h"
 
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ppp_oe.h"

#include "lwip/tcpip.h"
 
#include "netif/ethernetif.h"


struct eth_tx_msg
{
	struct netif 	*netif;
	struct pbuf 	*buf;
};

#define  MAX_RX_CNT 10
static  OS_EVENT *pRxMsgQueue = NULL;
void *RxMsgQeueTbl[MAX_RX_CNT];

#define  MAX_TX_CNT 10
static  OS_EVENT *pTxMsgQueue = NULL;
void *TxMsgQeueTbl[MAX_TX_CNT];

#define LWIP_ETHTHREAD_STACKSIZE	256
#define ETHERNETIF_RX_THREAD_PREORITY	10
#define ETHERNETIF_TX_THREAD_PREORITY	11

//static struct rt_thread eth_rx_thread;
static OS_STK eth_rx_thread_stack[LWIP_ETHTHREAD_STACKSIZE];
static OS_STK eth_tx_thread_stack[LWIP_ETHTHREAD_STACKSIZE];

static 	struct netif lpc_netif;

/* the interface provided to LwIP */
err_t eth_init(struct netif *netif)
{
	return ERR_OK;
}


err_t eth_input(struct pbuf *p, struct netif *inp)
{
	struct eth_hdr *ethhdr;

	if(p != RT_NULL)
	{
#ifdef LINK_STATS
		LINK_STATS_INC(link.recv);
#endif /* LINK_STATS */

		ethhdr = p->payload;

		switch(htons(ethhdr->type))
		{
		case ETHTYPE_IP:
			etharp_ip_input(inp, p);
			pbuf_header(p, -(s16_t) sizeof(struct eth_hdr));
			tcpip_input(p, inp);
			break;

		case ETHTYPE_ARP:
			etharp_arp_input(inp, (struct eth_addr *)inp->hwaddr, p);
			break;

		default:
			pbuf_free(p);
			p = RT_NULL;
			break;
		}
	}

	return ERR_OK;
}
/*********************************************************************************************************
** 函数名称: ethernetif_output
** 函数名称: ethernetif_output
**
** 功能描述：   当要发送一个IP信息包时，该函数由TCP/IP协议栈调用。该函数通过调用ethernetif_linkoutput()
               函数来完成实际的信息包发送。
**
** 输　入:  struct netif * netif  ，其指定IP信息包将被发送的哪一个网卡接口上
** 输　入:  struct pbuf * p         其保存着将要发送的IP信息包
** 输　入:  struct ip_addr * ipaddr  ，其指定IP信息包的目的IP地址
**          
** 输　出:   err_t               ERR_RTE:不能路由到目的地址（没有外网网关）
**                               ERR_BUF:无法为以太网头腾出空间
                                 etharp_query()函数或netif->linkoutput()函数的返回值
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年8月22日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
err_t ethernetif_output(struct netif *netif, struct pbuf *p, struct ip_addr *ipaddr)
{
	return etharp_output(netif, p, ipaddr);
}
/*********************************************************************************************************
** 函数名称: ethernetif_linkoutput
** 函数名称: ethernetif_linkoutput
**
** 功能描述：  完成实际的信息包发送
**
** 输　入:  struct netif * netif  ，其指定IP信息包将被发送的哪一个网卡接口上
** 输　入:  struct pbuf * p        ，其保存着将要发送的IP信息包     
**          
** 输　出:   err_t                 0: 发送成功   -1: 发送失败
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年8月22日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
err_t ethernetif_linkoutput(struct netif *netif, struct pbuf *p)
{
	struct eth_tx_msg msg;
	struct eth_device* enetif;
	INT8U ret = OS_NO_ERR;

	enetif = (struct eth_device*)netif->state;

	/* send a message to eth tx thread */
	msg.netif = netif;
	msg.buf   = p;
//	rt_mb_send(&eth_tx_thread_mb, (rt_uint32_t) &msg);
	ret =  OSQPost( pTxMsgQueue, &msg );

	/* waiting for ack */
//	rt_sem_take(&(enetif->tx_ack), RT_WAITING_FOREVER);
	OSSemPend(  enetif->tx_ack ,0, &ret);

	if(ret == OS_NO_ERR)
		return ERR_OK;

	return ERR_OK;
}

/* ethernetif APIs */
INT8U eth_device_init(struct eth_device* dev, const char* name)
{
	struct netif* netif = &lpc_netif;
	
	memset(&lpc_netif, 0, sizeof(struct netif));
#if 0
	netif = (struct netif*) rt_malloc (sizeof(struct netif));
	if (netif == RT_NULL)
	{
		rt_kprintf("malloc netif failed\n");
		return -RT_ERROR;
	}
	rt_memset(netif, 0, sizeof(struct netif));
#endif
	/* set netif */
 	dev->netif = netif;
	/* register to rt-thread device manager */
//	rt_device_register(&(dev->parent), name, RT_DEVICE_FLAG_RDWR);
//	dev->parent.type = RT_Device_Class_NetIf;
//	rt_sem_init(&(dev->tx_ack), name, 0, RT_IPC_FLAG_FIFO);
	dev->tx_ack =  OSSemCreate( 0 );

	/* set name */
	netif->name[0] = name[0];
	netif->name[1] = name[1];

	/* set hw address to 6 */
	netif->hwaddr_len	= 6;
	/* maximum transfer unit */
	netif->mtu			= ETHERNET_MTU;
	/* broadcast capability */
	netif->flags		= NETIF_FLAG_BROADCAST;

	/* get hardware address */
 // 	rt_device_control(&(dev->parent), NIOCTL_GADDR, netif->hwaddr);
	SetHWAddr( netif->hwaddr );
 
	/* set output */
	netif->output		= ethernetif_output;
	netif->linkoutput	= ethernetif_linkoutput;

	/* add netif to lwip */
	if (netif_add(netif, IP_ADDR_ANY, IP_ADDR_BROADCAST, IP_ADDR_ANY, dev,	eth_init, eth_input) == RT_NULL)
	{
		/* failed, unregister device and free netif */
//		rt_device_unregister(&(dev->parent));
//		rt_free(netif);
		return OS_FALSE;
	}

	netif_set_default(netif);

	return OS_TRUE;
}

/* ethernet buffer */
void eth_tx_thread_entry(void* parameter)
{
	struct eth_tx_msg* msg;
	INT8U err = 0;

	while (1)
	{
		msg = OSQPend( pTxMsgQueue , 0,&err);
		if ( err == OS_NO_ERR)
		{
			struct eth_device* enetif;

			RT_ASSERT(msg->netif != RT_NULL);
			RT_ASSERT(msg->buf   != RT_NULL);
			enetif = (struct eth_device*)msg->netif->state;
			if (enetif != RT_NULL)
			{
				/* call driver's interface */
				if (enetif->eth_tx(enetif, msg->buf) != OS_TRUE)
			 	{
			//		rt_kprintf("transmit eth packet failed\n");
			 	}
			}
			/* send ack */
		//	rt_sem_release(&(enetif->tx_ack));
			OSSemPost(enetif->tx_ack );
		}
	}
}

/* ethernet buffer */
void eth_rx_thread_entry(void* parameter)
{
	struct eth_device* device;
	INT8U err = 0;

	while (1)
	{
		device = OSQPend( pRxMsgQueue , 0,&err);
		if ( err == OS_NO_ERR)
		{
			struct pbuf *p;
			/* receive all of buffer */
			while (1)
			{
				p = device->eth_rx(device);
				if (p != RT_NULL)
				{
					/* notify to upper layer */
					eth_input(p, device->netif);
				}
				else break;
			}
		}
	}
}
 

u8_t eth_device_ready(struct eth_device* dev)
{
	/* post message to ethernet thread */
	INT8U err = OS_NO_ERR;
	err = OSQPost(pRxMsgQueue,dev);
	if (err == OS_NO_ERR)
		return OS_TRUE;
	
	return OS_FALSE;
}

/*********************************************************************************************************
** 函数名称: eth_system_device_init
** 函数名称: eth_system_device_init
**
** 功能描述：  初始化网卡收发任务以及相关的信号量等
**
**          
** 输　出:   u8_t
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年8月22日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
INT8U eth_system_device_init()
{ 
	/* init rx thread */
	/* init mailbox and create ethernet thread */
	pRxMsgQueue = OSQCreate( &RxMsgQeueTbl[0],MAX_RX_CNT  );
	//启动任务
	OSTaskCreate (eth_rx_thread_entry, (void *)0, 	&eth_rx_thread_stack[LWIP_ETHTHREAD_STACKSIZE-1], ETHERNETIF_RX_THREAD_PREORITY);


	pTxMsgQueue = OSQCreate( &TxMsgQeueTbl[0],MAX_TX_CNT  );
	//启动任务
	OSTaskCreate (eth_tx_thread_entry, (void *)0, 	&eth_tx_thread_stack[LWIP_ETHTHREAD_STACKSIZE-1], ETHERNETIF_TX_THREAD_PREORITY);

	return OS_TRUE;
}


 

#if 0
/* Define those to better describe your network interface. */
#define IFNAME0 'e'
#define IFNAME1 'n'

/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
struct ethernetif {
  struct eth_addr *ethaddr;
  /* Add whatever per-interface state that is needed here. */
};

/* Forward declarations. */
static void  ethernetif_input(struct netif *netif);

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void
low_level_init(struct netif *netif)
{
  struct ethernetif *ethernetif = netif->state;
  
  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set MAC hardware address */
  netif->hwaddr[0] = ;
  ...
  netif->hwaddr[5] = ;

  /* maximum transfer unit */
  netif->mtu = 1500;
  
  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
 
  /* Do whatever else is needed to initialize interface. */  
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */

static err_t
low_level_output(struct netif *netif, struct pbuf *p)
{
  struct ethernetif *ethernetif = netif->state;
  struct pbuf *q;

  initiate transfer();
  
#if ETH_PAD_SIZE
  pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

  for(q = p; q != NULL; q = q->next) {
    /* Send the data from the pbuf to the interface, one pbuf at a
       time. The size of the data in each pbuf is kept in the ->len
       variable. */
    send data from(q->payload, q->len);
  }

  signal that packet should be sent();

#if ETH_PAD_SIZE
  pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif
  
  LINK_STATS_INC(link.xmit);

  return ERR_OK;
}

/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *
low_level_input(struct netif *netif)
{
  struct ethernetif *ethernetif = netif->state;
  struct pbuf *p, *q;
  u16_t len;

  /* Obtain the size of the packet and put it into the "len"
     variable. */
  len = ;

#if ETH_PAD_SIZE
  len += ETH_PAD_SIZE; /* allow room for Ethernet padding */
#endif

  /* We allocate a pbuf chain of pbufs from the pool. */
  p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
  
  if (p != NULL) {

#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE); /* drop the padding word */
#endif

    /* We iterate over the pbuf chain until we have read the entire
     * packet into the pbuf. */
    for(q = p; q != NULL; q = q->next) {
      /* Read enough bytes to fill this pbuf in the chain. The
       * available data in the pbuf is given by the q->len
       * variable. */
      read data into(q->payload, q->len);
    }
    acknowledge that packet has been read();

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE); /* reclaim the padding word */
#endif

    LINK_STATS_INC(link.recv);
  } else {
    drop packet();
    LINK_STATS_INC(link.memerr);
    LINK_STATS_INC(link.drop);
  }

  return p;  
}

/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
static void
ethernetif_input(struct netif *netif)
{
  struct ethernetif *ethernetif;
  struct eth_hdr *ethhdr;
  struct pbuf *p;

  ethernetif = netif->state;

  /* move received packet into a new pbuf */
  p = low_level_input(netif);
  /* no packet could be read, silently ignore this */
  if (p == NULL) return;
  /* points to packet payload, which starts with an Ethernet header */
  ethhdr = p->payload;

  switch (htons(ethhdr->type)) {
  /* IP or ARP packet? */
  case ETHTYPE_IP:
  case ETHTYPE_ARP:
#if PPPOE_SUPPORT
  /* PPPoE packet? */
  case ETHTYPE_PPPOEDISC:
  case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
    /* full packet send to tcpip_thread to process */
    if (netif->input(p, netif)!=ERR_OK)
     { LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_input: IP input error\n"));
       pbuf_free(p);
       p = NULL;
     }
    break;

  default:
    pbuf_free(p);
    p = NULL;
    break;
  }
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t
ethernetif_init(struct netif *netif)
{
  struct ethernetif *ethernetif;

  LWIP_ASSERT("netif != NULL", (netif != NULL));
    
  ethernetif = mem_malloc(sizeof(struct ethernetif));
  if (ethernetif == NULL) {
    LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
    return ERR_MEM;
  }

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

  /*
   * Initialize the snmp variables and counters inside the struct netif.
   * The last argument should be replaced with your link speed, in units
   * of bits per second.
   */
  NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_YOUR_NETIF_IN_BPS);

  netif->state = ethernetif;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  /* We directly use etharp_output() here to save a function call.
   * You can instead declare your own function an call etharp_output()
   * from it if you have to do some checks before sending (e.g. if link
   * is available...) */
  netif->output = etharp_output;
  netif->linkoutput = low_level_output;
  
  ethernetif->ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);
  
  /* initialize the hardware */
  low_level_init(netif);

  return ERR_OK;
}

#endif
