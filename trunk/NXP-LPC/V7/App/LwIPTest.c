/****************************************Copyright (c)**************************************************
**                                        
**                                      
**                                      
**
**                           
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:  
**创   建   人:  
**最后修改日期: 
**描        述:   
**备        注 :   
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人:  
** 版  本:  
** 日　期:  
** 描　述: 
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 描　述:  
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include <stdlib.h>
#include "includes.h"  
#include "bsp.h"
#include "api.h"
#include "sockets.h"

#define  MAX_CONN_CNT 3 //支持最多3路并发

struct client_node
{
	struct netconn *conn;
	INT32U second_cnt; //秒计数，在秒定时器中不断累加，如果收到数据就清0，30秒检查一次，如果发现值>30，则判定为没有数据，断开
};

struct netconn *g_Conn[MAX_CONN_CNT] ; 

void InitServer( )
{
	INT8U i = 0;

	for (i = 0 ; i < MAX_CONN_CNT; i++)
	{
		g_Conn[i] = NULL;
	}

}

void MainLwIpListen( )
{
	struct netconn *conn = NULL, *newconn = NULL;
	struct sockaddr_in sin;
	struct ip_addr naddr;
	/* 建立一个连接结构 */
	conn = netconn_new(NETCONN_TCP);
	/* 将连接绑定到一个本地任意IP地址的2000端口上 */
	netconn_bind(conn, NULL, 2000);

	while(1)
	{
		/* 阻塞直至得到一个进入的连接 */
		newconn = netconn_accept(conn);
		if (newconn)
		{
			/* 获取远程主机的IP地址和端口号 */
			netconn_peer(conn,  &naddr, &sin.sin_port);
	//		sin.sin_port = htons(sin.sin_port);
	//		sin.sin_addr.s_addr = naddr.addr;
			//检查是否合法


			//通知到另外一个任务
		}

		OSTimeDly(1000);
	}
}

void ClientMgr( )
{

}

void ConnHandler( struct netconn *conn )
{
	struct netbuf *buf = NULL;
	if (conn == NULL)
		return ;
	
	/* 接收数据直到其它主机关闭连接 */
	buf = netconn_recv(conn);
	if (buf)
	{
		//给这个链接打个时间戳，如果长时间没有数据收发，那么需要强制关闭

		//处理数据

	}
	else
	{
		netconn_delete(conn);
	}
// 	while((buf = netconn_recv(conn)) != NULL)
// 	{
// //		do_something(buf);
// 	}
}

