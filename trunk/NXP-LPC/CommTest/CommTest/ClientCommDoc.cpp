
// CommTestDoc.cpp : CClientCommDoc 类的实现
//

#include "stdafx.h"
#include "CommTest.h"

#include "ClientCommDoc.h"

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
	return bRet;
}
