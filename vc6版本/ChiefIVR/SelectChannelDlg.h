#if !defined(AFX_SELECTCHANNELDLG_H__D0098E32_69D2_4A80_8270_65F8D25F4ACA__INCLUDED_)
#define AFX_SELECTCHANNELDLG_H__D0098E32_69D2_4A80_8270_65F8D25F4ACA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectChannelDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectChannelDlg dialog

class CSelectChannelDlg : public CDialog
{
// Construction
public:
	CSelectChannelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectChannelDlg)
	enum { IDD = IDD_DIALOG_SELCETCHANNEL };
	int		m_nChannelType;
	UINT	m_nBeginNo;
	UINT	m_nEndNo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectChannelDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectChannelDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTCHANNELDLG_H__D0098E32_69D2_4A80_8270_65F8D25F4ACA__INCLUDED_)
