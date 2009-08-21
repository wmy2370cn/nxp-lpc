#pragma once
#ifndef __NETIF_ETHERNETIF_H__
#define __NETIF_ETHERNETIF_H__

#include "lwip/netif.h"

#define NIOCTL_GADDR		0x01
#define ETHERNET_MTU		1500




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
