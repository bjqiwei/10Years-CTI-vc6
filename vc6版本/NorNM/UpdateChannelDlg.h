#if !defined(AFX_UPDATECHANNELDLG_H__9FDCF516_4DB6_4FB8_9D79_02A19B8BE90A__INCLUDED_)
#define AFX_UPDATECHANNELDLG_H__9FDCF516_4DB6_4FB8_9D79_02A19B8BE90A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UpdateChannelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUpdateChannelDlg dialog

class CUpdateChannelDlg : public CDialog
{
// Construction
public:
	CUpdateChannelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUpdateChannelDlg)
	enum { IDD = IDD_DIALOG_UPDATECHNL };
	BOOL	m_bUpdateCalledID;
	BOOL	m_bUpdateScriptFile;
	BOOL	m_bUpdateType;
	int		m_nChnlType;
	CString	m_strCalledID;
	CString	m_strScriptFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpdateChannelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUpdateChannelDlg)
	afx_msg void OnButtonFile();
	afx_msg void OnCheckUpdateType();
	afx_msg void OnCheckUpdateCalledid();
	afx_msg void OnCheckUpdateScriptfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPDATECHANNELDLG_H__9FDCF516_4DB6_4FB8_9D79_02A19B8BE90A__INCLUDED_)
