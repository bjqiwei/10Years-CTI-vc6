#if !defined(AFX_CONFERENCEENTERPAGESPC_H__F0800A87_FC7C_430F_933E_A1B76987886A__INCLUDED_)
#define AFX_CONFERENCEENTERPAGESPC_H__F0800A87_FC7C_430F_933E_A1B76987886A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConferenceEnterPageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConferenceEnterPageSpc dialog
class CDrawDoc;
class CConferenceEnterPageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CConferenceEnterPageSpc)

// Construction
public:
	void InitPage(CEnterConferObj *pObj, CDrawDoc *pDoc);
	CConferenceEnterPageSpc();
	~CConferenceEnterPageSpc();

// Dialog Data
	//{{AFX_DATA(CConferenceEnterPageSpc)
	enum { IDD = IDD_PAGE_CONF_ENTER_SPC };
	int		m_nAction;
	CString	m_strConferenceNO;
	CString	m_strChnlIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConferenceEnterPageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConferenceEnterPageSpc)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelect();
	afx_msg void OnButtonChindex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CEnterConferObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFERENCEENTERPAGESPC_H__F0800A87_FC7C_430F_933E_A1B76987886A__INCLUDED_)
