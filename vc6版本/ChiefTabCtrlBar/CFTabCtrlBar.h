// CFTabCtrlBar.h: interface for the CCFTabCtrlBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CFTABCTRLBAR_H__7B428BC9_AEA3_4E05_BB0F_9469888167C0__INCLUDED_)
#define AFX_CFTABCTRLBAR_H__7B428BC9_AEA3_4E05_BB0F_9469888167C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "CFControlBar.h"


typedef struct
{
	CWnd *pWnd ;
	char szLabel[32];
}TCB_ITEM;


/////////////////////////////////////////////////////////////////////////////
// CCFTabCtrlBar tab control bar

class AFX_EXT_CLASS CCFTabCtrlBar : public CCFControlBar
{
// Construction
public:
	CCFTabCtrlBar();

// Attributes
public:
	CTabCtrl m_tabctrl;
	CToolTipCtrl* m_ToolTip;

protected:
	int m_nActiveTab;
	CView* m_pActiveView;
	CList <TCB_ITEM *,TCB_ITEM *> m_views;

// Operations
public:
	void SetActiveView(int nNewTab);
	void SetActiveView(CRuntimeClass *pViewClass);
	CImageList* SetTabImageList(CImageList *pImageList);
	BOOL ModifyTabStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags);

// Overrides
public:
    // ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCFTabCtrlBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	CView* GetActiveView();
	CView* GetView(int nView);
	CView* GetView(CRuntimeClass *pViewClass);
	BOOL AddView(LPCTSTR lpszLabel, CRuntimeClass *pViewClass, CCreateContext *pContext = NULL);
	void RemoveView(int nView);
	virtual ~CCFTabCtrlBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CCFTabCtrlBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CFTABCTRLBAR_H__7B428BC9_AEA3_4E05_BB0F_9469888167C0__INCLUDED_)
