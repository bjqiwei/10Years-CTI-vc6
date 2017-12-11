#if !defined(AFX_CONFERLEAVEPAGESPC_H__BD1716EA_C805_484B_BDA9_3CC5F07B1D56__INCLUDED_)
#define AFX_CONFERLEAVEPAGESPC_H__BD1716EA_C805_484B_BDA9_3CC5F07B1D56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConferLeavePageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConferLeavePageSpc dialog
class CDrawDoc;
class CConferLeavePageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CConferLeavePageSpc)

// Construction
public:
	void InitPage(CLeaveConferObj *pObj, CDrawDoc *pDoc);
	CConferLeavePageSpc();
	~CConferLeavePageSpc();

// Dialog Data
	//{{AFX_DATA(CConferLeavePageSpc)
	enum { IDD = IDD_PAGE_CONF_LEAVE_SPC };
	CString	m_strConferenceNO;
	CString	m_strChnlIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConferLeavePageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConferLeavePageSpc)
	afx_msg void OnButtonSelect();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonChindex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CLeaveConferObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFERLEAVEPAGESPC_H__BD1716EA_C805_484B_BDA9_3CC5F07B1D56__INCLUDED_)
