/*
 * File      : app.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://openlab.rt-thread.com/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2008-12-11     xuxinming    the first version
 */

#include <rtthread.h>

#ifdef RT_USING_LWIP
#include <lwip/sys.h>
#include <lwip/api.h>
#endif
/**
 * @addtogroup LPC2478
 */
/*@{*/

  
void thread_tcpecho(void *parameter)
{
	struct netconn *conn, *newconn;
	err_t err;

	/* Create a new connection identifier. */
	conn = netconn_new(NETCONN_TCP);

	/* Bind connection to well known port number 7. */
	netconn_bind(conn, NULL, 7);

	/* Tell connection to go into listening mode. */
	netconn_listen(conn);

	while(1)
	{
		/* Grab new connection. */
		newconn = netconn_accept(conn);
		/* Process the new connection. */
		if(newconn != NULL)
		{
			struct netbuf *buf;
			void *data;
			rt_uint16_t len;

			while((buf = netconn_recv(newconn)) != NULL)
			{
				do
				{
					netbuf_data(buf, &data, &len);
					err = netconn_write(newconn, data, len, NETCONN_COPY);
					if(err != ERR_OK){}
				}
				while(netbuf_next(buf) >= 0);
				netbuf_delete(buf);
			}
			/* Close connection and discard connection identifier. */
			netconn_delete(newconn);
		}
	}
}


char echo_thread_stack[512];
struct rt_thread echo_thread; 

int echo_thread_init()
{  
	rt_thread_init(&echo_thread,"echo_thread",thread_tcpecho, RT_NULL,	&echo_thread_stack[0], sizeof(echo_thread_stack),
		22, 10);

	rt_thread_startup(&echo_thread);

	return 0;
}
/*@}*/
