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

#if 1 /* don't build, this is only a skeleton, see previous comment */

#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/sys.h"
#include <lwip/stats.h>
#include <lwip/snmp.h>
#include "netif/etharp.h"
#include "netif/ppp_oe.h"

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
static HANDLER __hBlockOutput;
HANDLER hEthernetInput;
//* 接收线程堆栈
#define	T_ETHERNETIF_INPUT_STKSIZE		256
OS_STK T_ETHERNETIF_INPUT_STK[T_ETHERNETIF_INPUT_STKSIZE];

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
	INT8U		__u8OldPrio;
	INT8U		__u8Err;

  struct ethernetif *ethernetif = netif->state;
  
  /* set MAC hardware address length */
  netif->hwaddr_len = ETHARP_HWADDR_LEN;

  /* set MAC hardware address */
//  netif->hwaddr[0] = ;
//  ...
//  netif->hwaddr[5] = ;

	netif->hwaddr[0] = 0xBC;
	netif->hwaddr[1] = 0x20;
	netif->hwaddr[2] = 0x06;
	netif->hwaddr[3] = 0x09;
	netif->hwaddr[4] = 0x30;
	netif->hwaddr[5] = 0x11;

  /* maximum transfer unit */
  netif->mtu = 1500;
  
  /* device capabilities */
  /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
  netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;
//  	pstNetif->flags = NETIF_FLAG_BROADCAST;


	//* 初始化EMAC。EMACInit()函数包含查询状态位代码，并且这些查询代码并没有使用OSTimeDly()等函数主动释放
	//* CPU使用权。如果网线在当时并没有接触良好，则这个过程需要相当长的时间。为了避免阻塞其它低优先级任务的
	//* 正常运行，我们使用了uCOS提供的任务管理函数，先将其所在任务的优先级降低，等初始化完成之后再恢复其优
	//* 先级。
	__u8OldPrio = OSTCBCur->OSTCBPrio;
	__u8Err = OSTaskChangePrio(OS_PRIO_SELF, OS_TASK_IDLE_PRIO - 1);
	EMACInit();
	if(__u8Err == OS_NO_ERR)
		OSTaskChangePrio(OS_PRIO_SELF, __u8OldPrio);
	
	OSTaskCreate(ethernetif_input, netif, &T_ETHERNETIF_INPUT_STK[T_ETHERNETIF_INPUT_STKSIZE-1], 6);

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
low_level_output(struct netif *pstNetif, struct pbuf *pstPbuf)
{
 	INT8U	__u8Err;

	struct pbuf *__pstSendPbuf = pstPbuf;
	err_t __errReturn = ERR_OK;

	//* 阻塞对EMAC的访问，以避免不同的任务同时访问EMAC造成访问冲突的问题，最长等待时间是2秒
	OSMutexPend(__hBlockOutput, 200, &__u8Err);
	if(OS_NO_ERR == __u8Err)
	{
		for(; __pstSendPbuf!=NULL; __pstSendPbuf=__pstSendPbuf->next)
		{
			//* 发送pbuf中的数据，每次一个pbuf，如果__pstSendPbuf->next指针为空则表明已经到达pbuf链表的末尾
			if(!EMACSendPacket(__pstSendPbuf->payload, __pstSendPbuf->len, (__pstSendPbuf->next == NULL)))
			{
				__errReturn = ERR_RTE;
				break;
			}
		}

        OSMutexPost(__hBlockOutput);
	}
	else
		__errReturn = ERR_IF;

    return __errReturn;
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
 	struct pbuf     *__pstPbuf = NULL, *__pstCurPbuf;
	INT16U			__u16Len;
	
	//* 获取收到的信息包的长度
	__u16Len = GetInputPacketLen();
	if(__u16Len)
	{
		//* 从pbuf pool中获取一个pbuf链
		__pstPbuf = pbuf_alloc(PBUF_RAW, __u16Len, PBUF_POOL);
		if(__pstPbuf != NULL)
		{
			//* 复制数据
			for(__pstCurPbuf=__pstPbuf; __pstCurPbuf!=NULL; __pstCurPbuf=__pstCurPbuf->next)
				EMACReadPacket(__pstCurPbuf->payload, __pstCurPbuf->len, (__pstCurPbuf->next == NULL));
		}
		else;
	}
	
	return __pstPbuf; 
}

static err_t
ethernetif_output(struct netif *netif, struct pbuf *p,
      struct ip_addr *ipaddr)
{
  
 /* resolve hardware address, then send (or queue) packet */
  return etharp_output(netif, p, ipaddr);
 
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
ethernetif_input(struct netif *pReserved)
{
	struct ethernetif		*__pstEthernetif;
	struct pbuf				*__pstPbuf;
	struct eth_hdr			*__pstEthhdr;
	struct netif			*__pstNetif;
	INT8U					__u8RtnVal;
	
	__pstNetif = (struct netif*)pReserved;			

	while(OS_TRUE)
	{
		__pstEthernetif = (struct ethernetif*)__pstNetif->state;
		
		//* 从EMAC循环读取数据
		do{
			__pstPbuf = low_level_input(__pstNetif);
			if(__pstPbuf == NULL)
				OSSemPend(hEthernetInput, 10, &__u8RtnVal);
		}while(__pstPbuf == NULL);	
		
		__pstEthhdr = __pstPbuf->payload;
		
		switch(htons(__pstEthhdr->type))
		{
			case ETHTYPE_IP:
				//* 更新ARP表
				etharp_ip_input(__pstNetif, __pstPbuf);
				//* 跳过以太网头部字段
				pbuf_header(__pstPbuf, -sizeof(struct eth_hdr) );				 					
				
				//* 传递到网络层
				__pstNetif->input(__pstPbuf, __pstNetif);
				
				break;
		
			case ETHTYPE_ARP:
				//* 将__pstPbuf传递到ARP模块
				etharp_arp_input(__pstNetif, __pstEthernetif->ethaddr, __pstPbuf);
				break;
		
			default:
				pbuf_free(__pstPbuf);
				__pstPbuf = NULL;
				break;
		}	
	}
}

static void
arp_timer(void *arg)
{
  etharp_tmr();
  sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);
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
  INT8U	__u8Err;
  static struct ethernetif ethernetif;
  
  netif->state = &ethernetif;
  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;
  netif->output = ethernetif_output;
  netif->linkoutput = low_level_output;
  
  __hBlockOutput = OSMutexCreate(5, &__u8Err);
  
  ethernetif.ethaddr = (struct eth_addr *)&(netif->hwaddr[0]);
  
  low_level_init(netif);

  etharp_init();

  sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);

  return ERR_OK;
}				  
#endif /* 0 */
