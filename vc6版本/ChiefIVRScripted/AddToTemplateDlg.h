#if !defined(AFX_ADDTOTEMPLATEDLG_H__D484217C_0E79_4685_8F2C_6DD01FC5926B__INCLUDED_)
#define AFX_ADDTOTEMPLATEDLG_H__D484217C_0E79_4685_8F2C_6DD01FC5926B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddToTemplateDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddToTemplateDlg dialog

class CAddToTemplateDlg : public CDialog
{
// Construction
public:
	int m_nIco;
	CString m_strExpress;
	CAddToTemplateDlg(CWnd* pParent = NULL);   // standard constructor
	CImageList m_ImageList;
// Dialog Data
	//{{AFX_DATA(CAddToTemplateDlg)
	enum { IDD = IDD_DIALOG_TEMPLATE };
	CEdit	m_ctrExpress;
	CComboBoxEx	m_cbexICO;
	CString	m_strName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddToTemplateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddToTemplateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelendcancelComboboxexIco();
	afx_msg void OnSelendokComboboxexIco();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDTOTEMPLATEDLG_H__D484217C_0E79_4685_8F2C_6DD01FC5926B__INCLUDED_)
