// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__51B7C9A7_9DAA_4A6F_9593_EF3B186B7058__INCLUDED_)
#define AFX_MAINFRM_H__51B7C9A7_9DAA_4A6F_9593_EF3B186B7058__INCLUDED_

#include "trayicon.h"
#include "DJTc08a32.h"
#include "DJSS17.h"
#include "TMS8E240.h"
#include "NorMarch32.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CTrayIcon m_trayIcon;
	BOOL m_bShutdown;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CSplitterWnd m_wndSplitter1;
	CSplitterWnd m_wndSplitter2;
	CSplitterWnd m_wndSplitter3;
	CProgressCtrl m_wndProg;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysRun();
	afx_msg void OnSysStop();
	afx_msg void OnClose();
	afx_msg void OnUpdateSysRun(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSysStop(CCmdUI* pCmdUI);
	afx_msg void OnAddSysscript();
	afx_msg void OnUpdateAddSysscript(CCmdUI* pCmdUI);
	afx_msg void OnDelSysscript();
	afx_msg void OnUpdateDelSysscript(CCmdUI* pCmdUI);
	afx_msg void OnSaveSysscript();
	afx_msg void OnUpdateSaveSysscript(CCmdUI* pCmdUI);
	afx_msg void OnAppOpen();
	afx_msg void OnMenuRun();
	afx_msg void OnUpdateMenuRun(CCmdUI* pCmdUI);
	afx_msg void OnMenuStop();
	afx_msg void OnUpdateMenuStop(CCmdUI* pCmdUI);
	afx_msg void OnMenuExit();
	afx_msg void OnUpdateMenuExit(CCmdUI* pCmdUI);
	afx_msg void OnAppExit();
	afx_msg void OnAddNorscript();
	afx_msg void OnDelNorscript();
	afx_msg void OnSaveNorscript();
	afx_msg void OnAutoCheck();
	afx_msg void OnUpdateAutoCheck(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysHangup();
	afx_msg void OnUpdateSysHangup(CCmdUI* pCmdUI);
	afx_msg void OnMenuHangup();
	afx_msg void OnUpdateMenuHangup(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg LRESULT OnTrayNotification(WPARAM wp, LPARAM lp);
	afx_msg void OnFriendMessage(WPARAM wp, LPARAM lp);
	afx_msg void OnSystemLogMessage(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
private:
	
public:
	void TMS_Exit( void );
	void TMS_Init( void );

	void DJSS7_Exit( void );
	void DJSS7_Init( void );
	void SystemExit( void );
	void SystemRun( void );
	void DJTc08a32_Init( void );
	void DJTc08a32_Exit( void  );
	void ReadSystemSetFile();
public:
	CDJTc08a32 *m_pDJTc08a32;
	CDJSS17    *m_pDJSS17;
	CTMS8E240  *m_pTms;
	CNorMarch32 *m_pNor32;
	BOOL m_bRun;
	int m_nRunMode;
	CString m_strCommIPAdrr;
	unsigned int m_nCommPort;
	int m_nSaveSysLog;
public:
	int m_nSwitchMode;
	void Nor32_Exit( void );
	void Nor32_Init( void );
	void AutoRun( void );
	void SystemHangUp( void );
	void StepIt( void );
	int m_nSysType;
	int m_nConfCard;
	int m_nCardType;
	int m_nSwitchType;
	int m_AgentRecord;
	int m_nAnalogTrunk;//是否有模拟中继
	int m_nDigitalTrunk;//是否有数字中继以及类型

	//int m_nTrunkType;
	int m_nFax;
	int m_nChangeVoice;
	int m_nVIP;

	int m_nSS7_type; //SS7类型
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__51B7C9A7_9DAA_4A6F_9593_EF3B186B7058__INCLUDED_)
