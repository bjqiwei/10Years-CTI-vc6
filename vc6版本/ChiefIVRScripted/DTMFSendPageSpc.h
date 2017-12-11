#if !defined(AFX_DTMFSENDPAGESPC_H__18482E0A_02DD_4791_BD65_2A8F08C2C005__INCLUDED_)
#define AFX_DTMFSENDPAGESPC_H__18482E0A_02DD_4791_BD65_2A8F08C2C005__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DTMFSendPageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDTMFSendPageSpc dialog
class CDrawDoc;
class CDTMFSendPageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CDTMFSendPageSpc)

// Construction
public:
	void InitPage(CDTMFSendObj *pObj, CDrawDoc *pDoc);
	CDTMFSendPageSpc();
	~CDTMFSendPageSpc();

// Dialog Data
	//{{AFX_DATA(CDTMFSendPageSpc)
	enum { IDD = IDD_PAGE_DTMFSEND_SPC };
	CString	m_strExpress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDTMFSendPageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDTMFSendPageSpc)
	afx_msg void OnButtonGet();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CDTMFSendObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DTMFSENDPAGESPC_H__18482E0A_02DD_4791_BD65_2A8F08C2C005__INCLUDED_)
