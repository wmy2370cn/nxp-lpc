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
**
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
#include <string.h>

#include <rtthread.h>

#include <lwip/sys.h>
#include <lwip/api.h>
#include "lwip/ip_addr.h"

#include "applib.h"
#include "mb.h"
#include "MbxTcp.h"
#include "mbproto.h" 
#include "Func.h"


/* ----------------------- Defines ------------------------------------------*/

/* ----------------------- MBAP Header --------------------------------------*/
/*
 *
 * <------------------------ MODBUS TCP/IP ADU(1) ------------------------->
 *              <----------- MODBUS PDU (1') ---------------->
 *  +-----------+---------------+------------------------------------------+
 *  | TID | PID | Length | UID  |Code | Data                               |
 *  +-----------+---------------+------------------------------------------+
 *  |     |     |        |      |                                           
 * (2)   (3)   (4)      (5)    (6)                                          
 *
 * (2)  ... MB_TCP_TID          = 0 (Transaction Identifier - 2 Byte) 
 * (3)  ... MB_TCP_PID          = 2 (Protocol Identifier - 2 Byte)
 * (4)  ... MB_TCP_LEN          = 4 (Number of bytes - 2 Byte)
 * (5)  ... MB_TCP_UID          = 6 (Unit Identifier - 1 Byte)
 * (6)  ... MB_TCP_FUNC         = 7 (Modbus Function Code)
 *
 * (1)  ... Modbus TCP/IP Application Data Unit
 * (1') ... Modbus Protocol Data Unit
 */

#define MB_TCP_TID          0
#define MB_TCP_PID          2
#define MB_TCP_LEN          4
#define MB_TCP_UID          6
#define MB_TCP_FUNC         7

#define MB_TCP_PROTOCOL_ID  0   /* 0 = Modbus Protocol */

/* ----------------------- Defines ------------------------------------------*/
#define MB_TCP_PSEUDO_ADDRESS   255

#define MB_TCP_DEFAULT_PORT 502 /* TCP listening port. */


MBErrorCode MBTCPReceive( UCHAR * pucRcvAddress, UCHAR ** ppucFrame, USHORT * pusLength )
{
    MBErrorCode    eStatus = MB_EIO;
    UCHAR          *pucMBTCPFrame;
    USHORT          usLength;
    USHORT          usPID;

//    if( xMBTCPPortGetRequest( &pucMBTCPFrame, &usLength ) != FALSE )
	if(0)
    {
        usPID = pucMBTCPFrame[MB_TCP_PID] << 8U;
        usPID |= pucMBTCPFrame[MB_TCP_PID + 1];

        if( usPID == MB_TCP_PROTOCOL_ID )
        {
            *ppucFrame = &pucMBTCPFrame[MB_TCP_FUNC];
            *pusLength = usLength - MB_TCP_FUNC;
            eStatus = MB_ENOERR;

            /* Modbus TCP does not use any addresses. Fake the source address such
             * that the processing part deals with this frame.
             */
            *pucRcvAddress = MB_TCP_PSEUDO_ADDRESS;
        }
    }
    else
    {
        eStatus = MB_EIO;
    }
    return eStatus;
}

MBErrorCode MBTCPSend( UCHAR _unused, const UCHAR * pucFrame, USHORT usLength )
{
    MBErrorCode    eStatus = MB_ENOERR;
    UCHAR          *pucMBTCPFrame = ( UCHAR * ) pucFrame - MB_TCP_FUNC;
    USHORT          usTCPLength = usLength + MB_TCP_FUNC;

    /* The MBAP header is already initialized because the caller calls this
     * function with the buffer returned by the previous call. Therefore we 
     * only have to update the length in the header. Note that the length 
     * header includes the size of the Modbus PDU and the UID Byte. Therefore 
     * the length is usLength plus one.
     */
    pucMBTCPFrame[MB_TCP_LEN] = ( usLength + 1 ) >> 8U;
    pucMBTCPFrame[MB_TCP_LEN + 1] = ( usLength + 1 ) & 0xFF;
//    if( xMBTCPPortSendResponse( pucMBTCPFrame, usTCPLength ) == FALSE )
    {
        eStatus = MB_EIO;
    }
    return eStatus;
}

#define  MBX_TCP_TASK_STK_SIZE 512
#define  MBX_TCP_TASK_PRIO 18
//最多支持3路在线
#define  MBX_TCP_CLIENT_CNT 3

struct MBX_TCP
{
	INT32U TaskStk[MBX_TCP_TASK_STK_SIZE];
	INT8U  Priority;
	INT8U  UsedFlag;

	//对端的IP地址以及端口号
	struct ip_addr Addr;
	INT16U Port;

	struct netconn  *pConn;

};

typedef struct MBX_TCP CMbxTcp;

CMbxTcp g_arrMbxTcp[MBX_TCP_CLIENT_CNT];

/*********************************************************************************************************
** 函数名称: InitMbxTcp
** 函数名称: InitMbxTcp
**
** 功能描述：  
**
** 输　入:  void
**          
** 输　出:   void
**         
** 全局变量:  
** 调用模块: 无
**
** 作　者:  LiJin
** 日　期:  2009年11月20日
** 备  注:  
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 备  注: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static void InitMbxTcp( void )
{
	INT8U i = 0;
	for ( i = 0 ; i < MBX_TCP_CLIENT_CNT ; i++)
	{
		memset( &g_arrMbxTcp[i].TaskStk ,0,sizeof( g_arrMbxTcp[i].TaskStk ) );
	}


}

static INT8U AddNewClient( struct netconn  *pConn )
{

	return TRUE;
}

void MbxTcpListen(void *pData )
{
	struct netconn *conn = NULL, *newconn = NULL;
	INT16U sin_port = 0;
	struct ip_addr naddr;
	/* 建立一个连接结构 */
	conn = netconn_new(NETCONN_TCP);
	/* 将连接绑定到一个本地任意IP地址的2000端口上 */
	netconn_bind(conn, NULL, MB_TCP_DEFAULT_PORT);
	/* Tell connection to go into listening mode. */
	netconn_listen(conn);

	while(1)
	{
		/* 阻塞直至得到一个进入的连接 */
		newconn = netconn_accept(conn);
		if (newconn)
		{
			/* 获取远程主机的IP地址和端口号 */
			netconn_peer(newconn,  &naddr, &sin_port);
			//		sin.sin_port = htons(sin.sin_port);
			//		sin.sin_addr.s_addr = naddr.addr;
			//检查是否合法


			//通知到另外一个任务
		}

	}
}


void InitMbxTcpTask( )
{

}