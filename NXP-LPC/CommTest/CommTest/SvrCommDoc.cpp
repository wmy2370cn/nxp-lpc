// PingTestDoc.cpp : 实现文件
//

#include "stdafx.h"
#include "CommTest.h"
#include "SvrCommDoc.h"


// CSvrCommDoc

IMPLEMENT_DYNCREATE(CSvrCommDoc, CDocument)

CSvrCommDoc::CSvrCommDoc()
{
 
}

BOOL CSvrCommDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CSvrCommDoc::~CSvrCommDoc()
{
}


BEGIN_MESSAGE_MAP(CSvrCommDoc, CDocument)
END_MESSAGE_MAP()


// CSvrCommDoc 诊断

#ifdef _DEBUG
void CSvrCommDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CSvrCommDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CSvrCommDoc 序列化

void CSvrCommDoc::Serialize(CArchive& ar)
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
#endif


// CSvrCommDoc 命令
 void CSvrCommDoc::StartTask( )
{
	 
}

void CSvrCommDoc::StopTask( )
{
	 
}


void CSvrCommDoc::OnCloseDocument()
{
	// TODO: 在此添加专用代码和/或调用基类
	StopTask();

	CDocument::OnCloseDocument();
}
