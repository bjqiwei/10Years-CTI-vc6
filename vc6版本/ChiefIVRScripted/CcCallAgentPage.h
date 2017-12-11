#if !defined(AFX_CCCALLAGENTPAGE_H__21A36185_B954_412E_A2D2_179BC507CBE9__INCLUDED_)
#define AFX_CCCALLAGENTPAGE_H__21A36185_B954_412E_A2D2_179BC507CBE9__INCLUDED_

#include "ListBoxExBuddy.h"	// Added by ClassView
#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CcCallAgentPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCcCallAgentPage dialog
class CDrawDoc;
class CCcCallAgentPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCcCallAgentPage)

// Construction
public:
	CString GetPlayStr( int nPlayType );
	int GetPlayType( CString strPlayType );
	CCcCallAgentPage();
	~CCcCallAgentPage();
	CListBoxExBuddy m_ListBoxExBuddy;
// Dialog Data
	//{{AFX_DATA(CCcCallAgentPage)
	enum { IDD = IDD_PAGE_CC_CALLAGENT };
	CEdit	m_edtFileExpress;
	CListCtrl	m_lstPlay;
	UINT	m_nTimeOut;
	int		m_nWaitType;
	CString	m_strWaitPlayFile;
	int		m_nPlayType;
	CString	m_strAimDN;
	BOOL	m_bIsPlayBeforeTalk;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCcCallAgentPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCcCallAgentPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAimdn();
	afx_msg void OnButtonFile();
	afx_msg void OnButtonExpress();
	afx_msg void OnClickListPlay(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnNew( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEdit( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDel( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUP( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDown( WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CCCCallAgentObj * m_pNodeObj;
	CDrawDoc * m_pDoc;
public:
	void InitPage( CCCCallAgentObj *pNodeObj, CDrawDoc * pDoc );
	CNodeDataList m_PlayList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCCALLAGENTPAGE_H__21A36185_B954_412E_A2D2_179BC507CBE9__INCLUDED_)
