
// CommTestDoc.cpp : CClientCommDoc 类的实现
//

#include "stdafx.h"
#include "CommTest.h"

#include "ClientCommDoc.h"
#include "ClientSendView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientCommDoc

IMPLEMENT_DYNCREATE(CClientCommDoc, CDocument)

BEGIN_MESSAGE_MAP(CClientCommDoc, CDocument)
END_MESSAGE_MAP()


// CClientCommDoc 构造/析构

CClientCommDoc::CClientCommDoc()
{
	// TODO: 在此添加一次性构造代码
	m_bConnected = FALSE;
	m_ClientComm.m_pDocument = this;
}

CClientCommDoc::~CClientCommDoc()
{
}

BOOL CClientCommDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
 
	return TRUE;
}




// CClientCommDoc 序列化

void CClientCommDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CClientCommDoc 诊断

#ifdef _DEBUG
void CClientCommDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CClientCommDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CClientCommDoc 命令
BOOL CClientCommDoc::Connect( )
{
	BOOL bRet = m_ClientComm.Connect();

	//连接成功后，更新相关的界面
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		if (pView->IsKindOf( RUNTIME_CLASS(CClientSendView) ))
		{
		 	((CClientSendView*)pView)->OnConnected();
		}		 		 
	}

	return bRet;
}
void  CClientCommDoc::Disconnect( )
{
	m_ClientComm.StopTask();
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CView* pView = GetNextView(pos);
		ASSERT_VALID(pView);
		if (pView->IsKindOf( RUNTIME_CLASS(CClientSendView) ))
		{
			((CClientSendView*)pView)->OnDisconnected();
		}		 		 
	}
}

void CClientCommDoc::OnCloseDocument()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_ClientComm.StopTask();

	CDocument::OnCloseDocument();
}
