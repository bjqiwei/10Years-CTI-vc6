#if !defined(AFX_EDITCOMPARELISTDLG_H__8A52E425_7992_4E29_9698_0A6E66E0CE30__INCLUDED_)
#define AFX_EDITCOMPARELISTDLG_H__8A52E425_7992_4E29_9698_0A6E66E0CE30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditCompareListDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditCompareListDlg dialog
class CDrawDoc;
class CEditCompareListDlg : public CDialog
{
// Construction
public:
	int m_nType;
	CString m_strExpression;
	CEditCompareListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditCompareListDlg)
	enum { IDD = IDD_EDITCOMPARE_DEFINE_DLG };
	CComboBox	m_cobType;
	CEdit	m_edtExpression;
	CString	m_strDscription;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditCompareListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditCompareListDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CDrawDoc * m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITCOMPARELISTDLG_H__8A52E425_7992_4E29_9698_0A6E66E0CE30__INCLUDED_)
