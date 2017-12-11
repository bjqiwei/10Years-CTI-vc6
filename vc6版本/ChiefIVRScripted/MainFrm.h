// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__6A9598F1_53D6_4C12_ACB4_ECF792AD9D08__INCLUDED_)
#define AFX_MAINFRM_H__6A9598F1_53D6_4C12_ACB4_ECF792AD9D08__INCLUDED_


#include "..//include//CFTabCtrlBar.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:
	void DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ShowCustomerMenu( void );
	BOOL CreateBottomBar();
	BOOL CreateWorkspace( void );
	CCFTabCtrlBar m_wndWorkspace;
	CCFTabCtrlBar m_wndBottomBar;
	BOOL CreateStatusBar();
	BOOL CreateToolBar();
	BOOL CreateToolBox( void );
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	CDialogBar      m_wndDlgBar;

	CToolBar    m_wndScrtipBar;
	CToolBar    m_wndBuildViewBar;

	CImageList m_LTabImages;
	CImageList m_ImageList;
	CImageList m_BTabImages;
	CImageList m_BImageList;

// Generated message map functions
protected:

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnToolbarDropDown(NMTOOLBAR* pnmh, LRESULT* plRes);
	afx_msg void OnUpdateViewToolboxBar(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg void OnDeleteNode();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__6A9598F1_53D6_4C12_ACB4_ECF792AD9D08__INCLUDED_)
