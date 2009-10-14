


#pragma once

#include <deque>

typedef enum enmMsgType
{
	MSG_NULL = 0,
	MSG_SEND_DATA
}MSG_TYPE;

class CCommMsg
{
public:
	CCommMsg( );
	CCommMsg( MSG_TYPE MsgType);

	~CCommMsg( );
	CCommMsg operator= (const CCommMsg &Src);
public:
	unsigned int m_nMsgType;  
};

const int MAX_COMM_MSG_CNT = 512;

class CCommMsgContainer
{
public:
	CCommMsgContainer( );
	~CCommMsgContainer ();
public:
	void AddCommMsg( CCommMsg & msg );
	bool GetCommMsg(  CCommMsg & msg );
protected:
	void FreeMem( );

	CCriticalSection  m_Mutex;
	std::deque  < CCommMsg >   m_arrCommMsg;

};