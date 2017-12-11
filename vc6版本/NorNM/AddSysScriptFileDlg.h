#if !defined(AFX_ADDSYSSCRIPTFILEDLG_H__AEA6A811_A7D1_4C46_9F28_8CB651AD468B__INCLUDED_)
#define AFX_ADDSYSSCRIPTFILEDLG_H__AEA6A811_A7D1_4C46_9F28_8CB651AD468B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddSysScriptFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddSysScriptFileDlg dialog

class CAddSysScriptFileDlg : public CDialog
{
// Construction
public:
	CAddSysScriptFileDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddSysScriptFileDlg)
	enum { IDD = IDD_SYSSCRIPT_DLG };
	CString	m_strFile;
	CString	m_strRemark;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddSysScriptFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddSysScriptFileDlg)
	afx_msg void OnButtonFile();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDSYSSCRIPTFILEDLG_H__AEA6A811_A7D1_4C46_9F28_8CB651AD468B__INCLUDED_)
