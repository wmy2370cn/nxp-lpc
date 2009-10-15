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
**描        述:    报文  采用PING PONG队列管理
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
#pragma once
#include <deque>


const unsigned int MAX_PACKET_LEN = 2048;
const unsigned int MAX_PACKET_CNT = 2048;

class CPacket
{
public:
	CPacket( );
	CPacket( unsigned char *pData,unsigned int nLen );
	~CPacket( );
public:
	void SetPacket( unsigned char *pData,unsigned int nLen);
	unsigned int m_nId;
	unsigned int GetPacket(CString &szPacket);
protected:
	unsigned char *m_pPacketData;
	unsigned int  m_nLen;
public:
	SYSTEMTIME m_stBuild; 
};


class CPacketContainer
{
public:
	CPacketContainer();
	~CPacketContainer();


public:
	 //task 往ping中推送数据，如果ping满了，则停止推送
	void PutData( unsigned char *pData,unsigned int nLen  );
	//view从ping中取数据
	bool GetData( CPacket ** ppPacket );

	//view调用删除数据
	bool RemoveData( CPacket *pPacket );

protected:
	void FreeMem( );

	CCriticalSection  m_Mutex;
	 //task 往ping中推送数据，如果ping满了，则停止推送
	std::deque  < CPacket * >   m_arrPingBuf; 

	//view从ping中取数据，然后移到pong中，如果pong满了，则开始删除pong的头
	std::deque  < CPacket * >   m_arrPongBuf;
	unsigned int m_nCurId;
};