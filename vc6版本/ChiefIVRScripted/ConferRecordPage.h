#if !defined(AFX_CONFERRECORDPAGE_H__37926250_3465_465B_A5DE_539600390DBA__INCLUDED_)
#define AFX_CONFERRECORDPAGE_H__37926250_3465_465B_A5DE_539600390DBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConferRecordPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConferRecordPage dialog

class CConferRecordPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CConferRecordPage)

// Construction
public:
	CConferRecordPage();
	~CConferRecordPage();

// Dialog Data
	//{{AFX_DATA(CConferRecordPage)
	enum { IDD = IDD_PAGE_CONF_RECORD_SPC };
	CString	m_strConferNO;
	CString	m_strFileExpress;
	UINT	m_nFileLength;
	CString	m_strFolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConferRecordPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConferRecordPage)
	afx_msg void OnButtonFile();
	afx_msg void OnButtonConferno();
	afx_msg void OnButtonFolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFERRECORDPAGE_H__37926250_3465_465B_A5DE_539600390DBA__INCLUDED_)
