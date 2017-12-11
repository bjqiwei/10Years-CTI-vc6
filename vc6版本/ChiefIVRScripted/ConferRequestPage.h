#if !defined(AFX_CONFERREQUESTPAGE_H__7BEECFD9_F5F6_4C21_99BF_233C40262DDD__INCLUDED_)
#define AFX_CONFERREQUESTPAGE_H__7BEECFD9_F5F6_4C21_99BF_233C40262DDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConferRequestPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConferRequestPage dialog
class CDrawDoc;
class CConferRequestPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CConferRequestPage)

// Construction
public:
	void InitPage(CConferRequestObj *pObj, CDrawDoc *pDoc);
	CConferRequestPage();
	~CConferRequestPage();

// Dialog Data
	//{{AFX_DATA(CConferRequestPage)
	enum { IDD = IDD_PAGE_CONFER_REQUEST };
	int		m_nDialType;
	int		m_nAction;
	CString	m_strAimTel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConferRequestPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConferRequestPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONDialTel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CConferRequestObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFERREQUESTPAGE_H__7BEECFD9_F5F6_4C21_99BF_233C40262DDD__INCLUDED_)
