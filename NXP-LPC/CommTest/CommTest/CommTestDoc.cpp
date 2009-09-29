
// CommTestDoc.cpp : CCommTestDoc 类的实现
//

#include "stdafx.h"
#include "CommTest.h"

#include "CommTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCommTestDoc

IMPLEMENT_DYNCREATE(CCommTestDoc, CDocument)

BEGIN_MESSAGE_MAP(CCommTestDoc, CDocument)
END_MESSAGE_MAP()


// CCommTestDoc 构造/析构

CCommTestDoc::CCommTestDoc()
{
	// TODO: 在此添加一次性构造代码

}

CCommTestDoc::~CCommTestDoc()
{
}

BOOL CCommTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CCommTestDoc 序列化

void CCommTestDoc::Serialize(CArchive& ar)
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


// CCommTestDoc 诊断

#ifdef _DEBUG
void CCommTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCommTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCommTestDoc 命令
