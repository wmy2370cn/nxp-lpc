
// CommTestDoc.h : CClientCommDoc 类的接口
//


#pragma once
#include "ClientComm.h"

const int MAX_SEND_LEN = 2048;

class CClientCommDoc : public CDocument
{
protected: // 仅从序列化创建
	CClientCommDoc();
	DECLARE_DYNCREATE(CClientCommDoc)

// 属性
public:
	int  m_nProtocolType; //协议类型 0 TCP 1 UDP
	DWORD m_dwDestIp;
	unsigned int m_nDestPort;
	unsigned int m_nLocalPort; //0 表示随机端口 非0为指定端口

	CClientSocket  m_ClientSocket;

	CMutex  m_nSendTxtMutex;

	//原始的，UNICODE编码
	CString m_szRawSendTxt;

 

#ifdef UNICODE
	
#endif // UNICODE


// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CClientCommDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};


