//*------------------------------------------------------------------------------------------------
//* 文件名				: LwIPEntry.c
//* 功能描述			: LwIP入口函数库
//* 作者    			: 焦海波
//* 版本				: 0.1
//* 建立日期、时间		: 2007/06/24
//* 修改日期、时间	    : 
//* 修改原因			: 
//*------------------------------------------------------------------------------------------------
//*------------------------------------------ 头文件 ----------------------------------------------
#include <stdlib.h>
#include	"ucos_ii.h"
#include	"../include/lwip/netif.h"
#include	"../include/lwip/tcpip.h"
//*------------------------------------ 变量、数据类型宏定义 --------------------------------------
//* IP地址
#define 	IP_ADDR_0			192
#define 	IP_ADDR_1			168
#define 	IP_ADDR_2			10
#define 	IP_ADDR_3			72

//* 网关地址
#define		GATEWAY_ADDR_0 		192
#define		GATEWAY_ADDR_1 		168
#define		GATEWAY_ADDR_2 		10	   
#define		GATEWAY_ADDR_3 		1

//* 掩码地址
#define		NET_MASK_0			255
#define		NET_MASK_1			255
#define		NET_MASK_2			255
#define		NET_MASK_3			0
//*================================================================================================
//*　　　　　　　　　　　　　　　　　　　　　函　数　区
//*================================================================================================
//*------------------------------------------------------------------------------------------------
//* 函数名称 : __ilvInitLwIP
//* 功能描述 : 完成LwIP最基本的初始化工作
//* 入口参数 : 无
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
__inline void __ilvInitLwIP(void)
{
	sys_init();
	
	mem_init();
	
	memp_init();
	
	pbuf_init();
	
//	raw_init();
	
	tcpip_init(NULL, NULL);
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : __ilvSetLwIP
//* 功能描述 : 设置LwIP，包括添加配置网络接口、建立接收任务等工作
//* 入口参数 : 无
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
__inline void __ilvSetLwIP(void)
{
	extern err_t ethernetif_init(struct netif *stNetif);

	struct ip_addr __stIpAddr, __stNetMask, __stGateway;
	static struct netif __stEMACNetif;
	
	//* 初始化缺省网络接口(即习惯意义上的网卡)及网络接口链表(即netif结构体链表)，根据
	//* 函数说明，它必须率先被调用
	netif_init();
	
	//* 建立并配置EMAC接口
	IP4_ADDR(&__stIpAddr, IP_ADDR_0, IP_ADDR_1, IP_ADDR_2, IP_ADDR_3);
			
	IP4_ADDR(&__stNetMask, NET_MASK_0, NET_MASK_1, NET_MASK_2, NET_MASK_3);
	IP4_ADDR(&__stGateway, GATEWAY_ADDR_0, GATEWAY_ADDR_1, GATEWAY_ADDR_2, GATEWAY_ADDR_3);
	
	netif_add(&__stEMACNetif, &__stIpAddr, &__stNetMask, &__stGateway, NULL, ethernetif_init, tcpip_input);
	
	netif_set_default(&__stEMACNetif);
	netif_set_up(&__stEMACNetif);
}
//*------------------------------------------------------------------------------------------------
//* 函数名称 : T_LwIPEntry
//* 功能描述 : LwIP入口任务
//* 入口参数 : <pvArg>[in/out] 保留参数
//* 出口参数 : 无
//*------------------------------------------------------------------------------------------------
void T_LwIPEntry(void * pvArg)
{
	struct netconn  *__pstConn, *__pstNewConn;
	struct netbuf	*__pstNetbuf;

	//* 初始化LwIP
	__ilvInitLwIP();
	
	//* 设置LwIP，包括添加配置网络接口、建立接收任务等工作
	__ilvSetLwIP();
	
	__pstConn = netconn_new(NETCONN_TCP);
	netconn_bind(__pstConn, NULL, 80);
	netconn_listen(__pstConn);
	
	while(OS_TRUE)
	{
		__pstNewConn = netconn_accept(__pstConn);
		
		if(__pstNewConn != NULL)
		{			
			__pstNetbuf = netconn_recv(__pstNewConn);
			if(__pstNetbuf != NULL)
			{
				netconn_write(__pstNewConn, "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n", 44, NETCONN_COPY);
				netconn_write(__pstNewConn, "<body><h1>这是LWIP TCP测试！</h1></body>", 40, NETCONN_COPY);
				
				netbuf_delete(__pstNetbuf);	
			}
			
			netconn_close(__pstNewConn);
			while(netconn_delete(__pstNewConn) != ERR_OK)
				OSTimeDlyHMSM(0, 0, 1, 0);
		}
	}
}