//#include <rtthread.h>
//#ifdef RT_USING_FINSH
//#include <finsh.h>
//#endif

#include "lwip/debug.h"
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/sys.h"
#include "lwip/stats.h"
#include "lwip/tcpip.h"
#include "lwip/ip_addr.h"
#include "lwip/dhcp.h"

#include "netif/ethernetif.h"
#include "netif/etharp.h"

/*
 * lwip system initial entry
 */
void lwip_sys_init()
{
	struct ip_addr ipaddr, netmask, gw;

#if LWIP_DHCP
	rt_uint32_t mscnt = 0;

	IP4_ADDR(&gw, 0,0,0,0);
	IP4_ADDR(&ipaddr, 0,0,0,0);
	IP4_ADDR(&netmask, 0,0,0,0);
#else
//	IP4_ADDR(&ipaddr, RT_LWIP_IPADDR0, RT_LWIP_IPADDR1, RT_LWIP_IPADDR2, RT_LWIP_IPADDR3);
//	IP4_ADDR(&gw, RT_LWIP_GWADDR0, RT_LWIP_GWADDR1, RT_LWIP_GWADDR2, RT_LWIP_GWADDR3);
//	IP4_ADDR(&netmask, RT_LWIP_MSKADDR0, RT_LWIP_MSKADDR1, RT_LWIP_MSKADDR2, RT_LWIP_MSKADDR3);
	IP4_ADDR(&gw, 192,9,200,1);
	IP4_ADDR(&ipaddr, 200,9,200,128);
	IP4_ADDR(&netmask, 255,255,255,0);

#endif
  	tcpip_init(NULL, NULL);

	netif_set_addr(netif_default, &ipaddr, &netmask, &gw);
	netif_set_up(netif_default);

#if LWIP_DHCP
	/* use DHCP client */
	dhcp_start(netif_default);

    while (1) 
	{
        OSTimeDly(DHCP_FINE_TIMER_MSECS);

        dhcp_fine_tmr();
        mscnt += DHCP_FINE_TIMER_MSECS;
        if (mscnt >= DHCP_COARSE_TIMER_SECS*1000)
        {
            dhcp_coarse_tmr();
            mscnt = 0;
        }
    }
#endif

 
}
