/****************************************Copyright (c)**************************************************
**                                江苏科技股份有限公司
**                                     事业部
**                                         
**
**                                 http://www.xxx.com.cn
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:  
**创   建   人:  
**最后修改日期: 
**描        述:  
**              
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人:  
** 版  本: v1.00
** 日　期:  
** 描　述: 原始版本
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人:  
** 日　期:  
** 描　述:  
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

#pragma  once
#include <deque>

// Determines the size of the first bytes who tells you how big the message are. (pakage heap)  
const int MAX_PACKAGE_SIZE = 1024;

class CSvrCommPacket
{
public:
	CSvrCommPacket( );
	~CSvrCommPacket( );
public:
	// Used with overlapped.. 
	OVERLAPPED			m_ol;

	int GetOperation()
	{
		return m_nOperation;
	}
	void SetOperation(int op)
	{
		m_nOperation = op;
	}
	// returns A pointer to the Buffer 
	unsigned char * GetBuffer()
	{
		return m_Buffer;
	}
	WSABUF * GetWSABuffer()
	{
		return  const_cast<WSABUF*>(&m_wsabuf);
	}
	// Setup Different Types in buffer. 
	void SetupWrite();
	void SetupRead();
	void SetupZeroByteRead();

	void EmptyUsed();
	UINT Use(UINT nSize);
	UINT GetUsed();
	//UINT GetSize();
	BOOL IsValid();

	BOOL Flush(UINT nBytesToRemove);

	// Write different types of variabels to the buffer
	BOOL AddData(const BYTE * pData,UINT nSize);
	BOOL AddData(const char * pData,UINT nSize);
	BOOL AddData(BYTE data);
	BOOL AddData(UINT data);
	BOOL AddData(unsigned short data);

	int m_nSeqNum;
protected:
	void Init();	


protected:
	// The Type of Operation. 
	int		m_nOperation;


	// number of bytes that are Used. 
	unsigned int    m_nUsed;
	// The buffer. 
	unsigned char  m_Buffer[MAX_PACKAGE_SIZE];
	//Holds the Buffer Used for send/receive
	WSABUF m_wsabuf;
};