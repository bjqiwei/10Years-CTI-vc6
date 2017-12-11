#if !defined(AFX_CONFERPLAYPAGE_H__1606E704_A35D_40E2_9E10_F56BEB4B3133__INCLUDED_)
#define AFX_CONFERPLAYPAGE_H__1606E704_A35D_40E2_9E10_F56BEB4B3133__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConferPlayPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConferPlayPage dialog

class CConferPlayPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CConferPlayPage)

// Construction
public:
	CConferPlayPage();
	~CConferPlayPage();

// Dialog Data
	//{{AFX_DATA(CConferPlayPage)
	enum { IDD = IDD_PAGE_CONF_PLAY_SPC };
	CString	m_strConferNO;
	CString	m_strFileExpress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConferPlayPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConferPlayPage)
	afx_msg void OnButtonFileexpress();
	afx_msg void OnButtonConferno();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFERPLAYPAGE_H__1606E704_A35D_40E2_9E10_F56BEB4B3133__INCLUDED_)
