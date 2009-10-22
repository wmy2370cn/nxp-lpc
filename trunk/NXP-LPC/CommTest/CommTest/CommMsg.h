


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

const int MAX_COMM_MSG_CNT = 1024;

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


class CPingMsg
{
public:
	CPingMsg( );
	~CPingMsg( );
	CPingMsg operator= (const CPingMsg &Src);
public:
 
protected:
public:
	in_addr	 m_Address;              //The IP address of the replier
	unsigned long m_dwRTT;             //Round Trip time in Milliseconds
	unsigned int m_nStatus;       //Í¨ Îª 0£»

	SYSTEMTIME m_stRequest; 
	SYSTEMTIME m_stReply; 
	unsigned int m_nTaskId;

	unsigned int m_nIdx;
};

class CPingMsgContainer
{
public:
	CPingMsgContainer( );
	~CPingMsgContainer( );
public:
	void AddPingMsg( CPingMsg & msg );
	bool GetPingMsg(  CPingMsg & msg );
protected:
	void FreeMem( );

	CCriticalSection  m_Mutex;
	std::deque  < CPingMsg >   m_arrPingMsg;
public:
	unsigned long m_nPingCnt;
	unsigned long m_nLostCnt;
	unsigned long m_nReplyCnt;

	void ResetCnt( )
	{
		m_nPingCnt = 0;
		m_nLostCnt = 0;
		m_nReplyCnt = 0;
	}
};