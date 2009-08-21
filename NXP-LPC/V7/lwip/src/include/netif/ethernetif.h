#pragma once
#ifndef __NETIF_ETHERNETIF_H__
#define __NETIF_ETHERNETIF_H__

#include "lwip/netif.h"

#define NIOCTL_GADDR		0x01
#define ETHERNET_MTU		1500




typedef struct eth_device* eth_device_t;
struct eth_device
{
	/* inherit from rt_device */
//	struct rt_device parent;

	struct eth_addr *ethaddr;
	struct netif *netif;
 	OS_EVENT  *tx_ack;

	/* common device interface */
// 	u8_t  (*init)	(eth_device_t dev);
// 	u8_t  (*open)	(eth_device_t dev, INT16U oflag);
// 	u8_t  (*close)	(eth_device_t dev);
// 	u8_t (*read)	(eth_device_t dev, INT16U pos, void* buffer, INT16U size);
// 	u8_t (*write)	(eth_device_t dev, INT16U pos, const void* buffer, INT16U size);
// 	u8_t  (*control)(eth_device_t dev, INT8U cmd, void *args);

	/* eth device interface */
 	struct pbuf* (*eth_rx)(eth_device_t dev);
	u8_t (*eth_tx)(eth_device_t dev, struct pbuf* p);
};

INT8U eth_device_init(struct eth_device* dev, const char* name);
INT8U eth_system_device_init(void);
INT8U eth_device_ready(struct eth_device* dev);
#if 0
struct eth_device
{
	/* inherit from rt_device */
	struct rt_device parent;

	struct eth_addr *ethaddr;
	struct netif *netif;
	struct rt_semaphore tx_ack;
	
	/* eth device interface */
	struct pbuf* (*eth_rx)(rt_device_t dev);
	INT8U (*eth_tx)(rt_device_t dev, struct pbuf* p);
};

INT8U eth_system_device_init(void);
INT8U eth_device_ready(struct eth_device* dev);

INT8U eth_device_init(struct eth_device* dev, const char* name);

INT8U eth_system_device_init(void);

#endif

#endif /* __NETIF_ETHERNETIF_H__ */
