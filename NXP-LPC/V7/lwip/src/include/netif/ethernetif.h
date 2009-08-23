#pragma once
#ifndef __NETIF_ETHERNETIF_H__
#define __NETIF_ETHERNETIF_H__

#include "lwip/netif.h"

#define NIOCTL_GADDR		0x01
#define ETHERNET_MTU		1500

#define  NET_IF_ADDR_SIZE                                  6    /* 48-bit MAC/net addr size.                            */
#define  NET_IF_ADDR_SIZE_MAC                            NET_IF_ADDR_SIZE

#define  NET_IF_ADDR_BROADCAST                0xFFFFFFFFFFFF
#define  NET_IF_ADDR_BROADCAST_xx                       0xFF    /* ALL broadcast addr octet vals identical.             */
#define  NET_IF_ADDR_BROADCAST_00                       0xFF
#define  NET_IF_ADDR_BROADCAST_01                       0xFF
#define  NET_IF_ADDR_BROADCAST_02                       0xFF
#define  NET_IF_ADDR_BROADCAST_03                       0xFF
#define  NET_IF_ADDR_BROADCAST_04                       0xFF
#define  NET_IF_ADDR_BROADCAST_05                       0xFF

#define  NET_IF_MTU_ETHER                               1500
#define  NET_IF_MTU_IEEE_802                            1492

#define  NET_IF_HDR_SIZE_ETHER                            14    /* See Note #2.                                         */

#define  NET_IF_HDR_SIZE_MAX                NET_IF_HDR_SIZE_ETHER


/* See Note #1.                                         */
#define  NET_IF_MTU_MIN                         (DEF_MIN(NET_IF_MTU_ETHER, NET_IF_MTU_IEEE_802))
#define  NET_IF_MTU_MAX                         (DEF_MAX(NET_IF_MTU_ETHER, NET_IF_MTU_IEEE_802))
#define  NET_IF_MTU                                      NET_IF_MTU_ETHER

#define  NET_IF_FRAME_CRC_SIZE                             4


#define  NET_IF_FRAME_MIN_CRC_SIZE                        64    /* See Note #2.                                         */
#define  NET_IF_FRAME_MIN_SIZE                          (NET_IF_FRAME_MIN_CRC_SIZE - NET_IF_FRAME_CRC_SIZE)

#define  NET_IF_FRAME_MAX_SIZE                          (NET_IF_MTU                + NET_IF_HDR_SIZE_MAX  )
#define  NET_IF_FRAME_MAX_CRC_SIZE                      (NET_IF_FRAME_MAX_SIZE     + NET_IF_FRAME_CRC_SIZE)



typedef struct eth_device* eth_device_t;
struct eth_device
{ 
	struct eth_addr *ethaddr;
	struct netif *netif;
 	OS_EVENT  *tx_ack;
 
	/* eth device interface */
 	struct pbuf* (*eth_rx)(eth_device_t dev);
	INT8U (*eth_tx)(eth_device_t dev, struct pbuf* p);
};

INT8U eth_device_init(struct eth_device* dev, const char* name);
INT8U eth_system_device_init(void);
INT8U eth_device_ready(struct eth_device* dev);
 

#endif /* __NETIF_ETHERNETIF_H__ */
