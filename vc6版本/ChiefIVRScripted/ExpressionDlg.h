#if !defined(AFX_EXPRESSIONDLG_H__C0EC9F12_1D1B_4BFF_A3CE_469A8C9643F3__INCLUDED_)
#define AFX_EXPRESSIONDLG_H__C0EC9F12_1D1B_4BFF_A3CE_469A8C9643F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExpressionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExpressionDlg dialog

class CExpressionDlg : public CDialog
{
// Construction
public:
	CString m_strExpress;
	CExpressionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CExpressionDlg)
	enum { IDD = IDD_DIALOG_EXPRESS };
	CTreeCtrl	m_treeSelect;
	CEdit	m_edtExpress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExpressionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExpressionDlg)
	afx_msg void OnButtonXiaoyudengyu();
	afx_msg void OnButtonXiaoyu();
	afx_msg void OnButtonStrjia();
	afx_msg void OnButtonJian();
	afx_msg void OnButtonJia();
	afx_msg void OnButtonDengyu();
	afx_msg void OnButtonDayudendyu();
	afx_msg void OnButtonDayu();
	afx_msg void OnButtonChu();
	afx_msg void OnButtonCheng();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPRESSIONDLG_H__C0EC9F12_1D1B_4BFF_A3CE_469A8C9643F3__INCLUDED_)
