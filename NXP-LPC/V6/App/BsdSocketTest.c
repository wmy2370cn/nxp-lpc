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

#ifndef INVALID_SOCKET
#define INVALID_SOCKET  (int)(-1)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR            (-1)
#endif


#define  MAX_CONN_CNT 3 //支持最多3路并发
#define USERPORT  9002

struct client_node
{
	INT32S  sock_id;
	INT32U  second_cnt; //秒计数，在秒定时器中不断累加，如果收到数据就清0，30秒检查一次，如果发现值>30，则判定为没有数据，断开
};

struct client_node g_Conn[MAX_CONN_CNT];

void InitServer( )
{
	INT8U i = 0;

	for (i = 0 ; i < MAX_CONN_CNT; i++)
	{
	//	g_Conn[i] = NULL;
	}
}


static  void  ListenTask (void *p_arg)
{
	INT32S newsocket,server;
	struct sockaddr_in dst_addr,client_addr;
	int namelen = sizeof(client_addr);
	while(1)
	{
		server = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
		if (server == INVALID_SOCKET)
		{
			OSTimeDlyHMSM(0,0,1,0);
			continue;
		}

		dst_addr.sin_family = AF_INET;
		dst_addr.sin_port = htons(USERPORT);
		dst_addr.sin_addr.s_addr=htonl(INADDR_ANY);
		if (bind(server,(struct sockaddr *)&dst_addr,sizeof(dst_addr))==SOCKET_ERROR)
		{
			//	str_print("服务器端口绑定失败、请正确配置IP地址并连接网络",2);
			close(server);
			OSTimeDlyHMSM(0,0,1,0);

			continue; 
		}

		if (listen(server,5) == SOCKET_ERROR)
		{
			close(server);
			OSTimeDlyHMSM(0,0,1,0);
			continue;
		}

		while(1)
		{
			struct fd_set fdread;
			struct timeval tv;
			int ret = 0;

			OSTimeDlyHMSM(0,0,0,300);
	 		tv.tv_sec = 0;
			tv.tv_usec = 1;
			FD_ZERO(&fdread);
			FD_SET(server, &fdread);
			ret = select(0, &fdread, NULL, NULL, &tv);
			if (ret <= 0)
				continue;
			newsocket = accept(server,(struct sockaddr*)&client_addr,&namelen);
			if (newsocket == INVALID_SOCKET)
			{
				close(newsocket);
				OSTimeDlyHMSM(0,0,0,300);
				continue;
			}
			//连接进来了，查找是否有重复的节点，然后添加进来
			


		}
	}

}

static  void  ProcessTask (void *p_arg)
{
	struct client_node *pCilent = (struct client_node *)p_arg;

	INT32S sock;
	struct fd_set fdread;
	struct timeval tv;

	if (p_arg == NULL)
		return;

	while(1)
	{
		if (pCilent->sock_id == 0 || pCilent->sock_id == INVALID_SOCKET)
		{
			OSTimeDlyHMSM(0,0,1,0);
			continue;
		}

		sock = pCilent->sock_id

		tv.tv_sec = 2;
		tv.tv_usec = 0;





	}



}
