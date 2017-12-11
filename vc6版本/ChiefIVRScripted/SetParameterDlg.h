#if !defined(AFX_SETPARAMETERDLG_H__39200BDD_45CC_4EF6_9943_5816C1A654E4__INCLUDED_)
#define AFX_SETPARAMETERDLG_H__39200BDD_45CC_4EF6_9943_5816C1A654E4__INCLUDED_

#include "ListBoxExBuddy.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetParameterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetParameterDlg dialog
class CDrawDoc;
class CSetParameterDlg : public CDialog
{
// Construction
public:
	CSetParameterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetParameterDlg)
	enum { IDD = IDD_MODULE_PARAM };
	CListCtrl	m_ctrParamLst;
	CString	m_strName;
	CString	m_strDescription;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetParameterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSetParameterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListParameter(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	afx_msg LRESULT OnNew( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEdit( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDel( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUP( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDown( WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	BOOL ParamIsExit( CString strName );
	void Init( CDrawDoc *pDoc );
	CListBoxExBuddy m_ListBoxExBuddy;
	CDrawDoc *m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETPARAMETERDLG_H__39200BDD_45CC_4EF6_9943_5816C1A654E4__INCLUDED_)
