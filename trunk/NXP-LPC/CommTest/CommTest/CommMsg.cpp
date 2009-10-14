

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