#pragma once




class CSvrListGridCtrl: public CBCGPGridCtrl
{
public:
	CSvrListGridCtrl( )
	{

	}
	virtual ~CSvrListGridCtrl( )
	{

	}

	virtual void SetRowHeight ()
	{
		CBCGPGridCtrl::SetRowHeight ();
		m_nRowHeight += 8;
		m_nLargeRowHeight += 8;
	}

	CBCGPGridRow* CreateNewRow ()
	{
		CBCGPGridRow* pRow = CreateRow (GetColumnCount());

		for (int i = 0; i < GetColumnCount ();i++)
		{
			pRow->GetItem (i)->AllowEdit (FALSE);
		}

		return pRow;
	}
public:
	void AddEmptyRow ()
	{
		AddRow (CreateNewRow (), TRUE);
	}
};

// CSvrListView 视图
class CSvrListView : public CView
{
	DECLARE_DYNCREATE(CSvrListView)

protected:
	CSvrListView();           // 动态创建所使用的受保护的构造函数
	virtual ~CSvrListView();
public:
	CSvrListGridCtrl m_wndGrid;

public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkGrid(NMHDR* pNMHDR, LRESULT* pResult);
protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};


