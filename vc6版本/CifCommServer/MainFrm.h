// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__2063F395_BD95_4C6F_B228_2A6390AFBDC2__INCLUDED_)
#define AFX_MAINFRM_H__2063F395_BD95_4C6F_B228_2A6390AFBDC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "trayicon.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitterWnd m_wndSplitter1;
	CSplitterWnd m_wndSplitter2;
	CSplitterWnd m_wndSplitter3;
public:

public:
	BOOL m_bShutdown;
	CTrayIcon m_trayIcon;
	BOOL m_bSysRun;
	int m_nRunMode;
	unsigned int  m_nPort;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ReadConfig();
	void SysRun();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAppOpen();
	afx_msg void OnUpdateAppOpen(CCmdUI* pCmdUI);
	afx_msg void OnMenuRun();
	afx_msg void OnUpdateMenuRun(CCmdUI* pCmdUI);
	afx_msg void OnMenuStop();
	afx_msg void OnUpdateMenuStop(CCmdUI* pCmdUI);
	afx_msg void OnMenuExit();
	afx_msg void OnUpdateMenuExit(CCmdUI* pCmdUI);
	afx_msg void OnFileRun();
	afx_msg void OnUpdateFileRun(CCmdUI* pCmdUI);
	afx_msg void OnFileStop();
	afx_msg void OnUpdateFileStop(CCmdUI* pCmdUI);
	afx_msg void OnAppExit();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LRESULT OnTrayNotification(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__2063F395_BD95_4C6F_B228_2A6390AFBDC2__INCLUDED_)
