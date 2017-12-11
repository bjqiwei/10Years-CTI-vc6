#if !defined(AFX_SELECTCONSTANTVARDLD_H__3489A275_0CFA_4B67_987E_8D5930B5D525__INCLUDED_)
#define AFX_SELECTCONSTANTVARDLD_H__3489A275_0CFA_4B67_987E_8D5930B5D525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectConstantVarDld.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectConstantVarDlg dialog

class CSelectConstantVarDlg : public CDialog
{
// Construction
public:
	void InitVarialeList();
	CString m_strVarName;
	CSelectConstantVarDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectConstantVarDlg)
	enum { IDD = IDD_SELECT_CONSTANTVAR };
	CListCtrl	m_varList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectConstantVarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectConstantVarDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTCONSTANTVARDLD_H__3489A275_0CFA_4B67_987E_8D5930B5D525__INCLUDED_)
