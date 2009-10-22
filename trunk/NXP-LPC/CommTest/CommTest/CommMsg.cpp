

#include "stdafx.h"
#include "CommMsg.h"



CCommMsg::CCommMsg( )
{
	m_nMsgType = 0;
}

CCommMsg::CCommMsg( MSG_TYPE MsgType)
{
	m_nMsgType = MsgType;
}


CCommMsg::~CCommMsg()
{

}

CCommMsg CCommMsg::operator = (const CCommMsg &Src)
{ 
	m_nMsgType = Src.m_nMsgType;  // 消息类型
 	return *this;
}










CCommMsgContainer::CCommMsgContainer( )
{

}

CCommMsgContainer::~CCommMsgContainer( )
{
	m_arrCommMsg.clear();
}

void CCommMsgContainer::AddCommMsg( CCommMsg & msg )
{
	CSingleLock lock (&m_Mutex);
	lock.Lock();
	if (m_arrCommMsg.size() >= MAX_COMM_MSG_CNT)
	{
		lock.Unlock();
		return;
	}

	m_arrCommMsg.push_back(msg);
	lock.Unlock();
}

bool CCommMsgContainer::GetCommMsg(  CCommMsg & msg )
{
	CSingleLock lock (&m_Mutex);
	lock.Lock();
	if (m_arrCommMsg.empty())
	{
		lock.Unlock();
		return false;
	}
	CCommMsg  ret_msg = m_arrCommMsg.front();
	m_arrCommMsg.pop_front();

	msg = ret_msg;
	return true;
}

void CCommMsgContainer::FreeMem()
{

}






CPingMsg::CPingMsg( )
{
	m_dwRTT = 0;             //Round Trip time in Milliseconds
	m_nStatus = 0;       //通 为 0；
	m_nIdx = 0;
	m_nTaskId = 0;
	GetSystemTime(&m_stRequest);
	GetSystemTime(&m_stReply);

	memset(&m_Address,0,sizeof(m_Address));
}

CPingMsg::~CPingMsg()
{

}

CPingMsg CPingMsg::operator = (const CPingMsg &Src)
{ 
//	m_nMsgType = Src.m_nMsgType;  // 消息类型
	m_dwRTT = Src.m_dwRTT;
	m_nStatus = Src.m_nStatus;
	m_Address = Src.m_Address;
	m_nIdx = Src.m_nIdx;
	m_nTaskId = Src.m_nTaskId;

	memcpy_s( &m_stRequest,sizeof(SYSTEMTIME),& Src.m_stRequest,sizeof(SYSTEMTIME) );
	memcpy_s( &m_stReply,sizeof(SYSTEMTIME),& Src.m_stReply,sizeof(SYSTEMTIME) );

	return *this;
}
CPingMsgContainer::CPingMsgContainer( )
{
	m_nPingCnt = 0;
	m_nLostCnt = 0;
	m_nReplyCnt = 0;
}
CPingMsgContainer::~CPingMsgContainer( )
{
	m_arrPingMsg.clear();
}

void CPingMsgContainer::AddPingMsg(CPingMsg &msg)
{	
	GetSystemTime(& msg.m_stReply);
	m_nPingCnt ++;

	CSingleLock lock (&m_Mutex);
	lock.Lock();
	if (m_arrPingMsg.size() >= MAX_COMM_MSG_CNT)
	{
		lock.Unlock();
		return;
	}
	msg.m_nIdx = m_nPingCnt;
	if (msg.m_nStatus)
	{
		m_nLostCnt++;
	}
	else
	{
		m_nReplyCnt ++;
	}
	m_arrPingMsg.push_back(msg);
	lock.Unlock();
}

bool  CPingMsgContainer::GetPingMsg(  CPingMsg & msg )
{
	CSingleLock lock (&m_Mutex);
	lock.Lock();
	if (m_arrPingMsg.empty())
	{
		lock.Unlock();
		return false;
	}
	CPingMsg  ret_msg = m_arrPingMsg.front();
	m_arrPingMsg.pop_front();
	msg = ret_msg;
	return true;
}

