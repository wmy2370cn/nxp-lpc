

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

}
void CCommMsgContainer::AddCommMsg( CCommMsg & msg )
{

}
bool CCommMsgContainer::GetCommMsg(  CCommMsg & msg )
{

	return true;
}

void CCommMsgContainer::FreeMem()
{

}