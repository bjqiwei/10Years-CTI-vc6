#if !defined(AFX_WAITCALLSPECIFICPROPAGE_H__2D2D42F0_D910_4A6E_BB0C_C50E5BBB4A45__INCLUDED_)
#define AFX_WAITCALLSPECIFICPROPAGE_H__2D2D42F0_D910_4A6E_BB0C_C50E5BBB4A45__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaitCallSpecificProPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWaitCallSpecificProPage dialog
class CDrawDoc;
class CWaitCallSpecificProPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CWaitCallSpecificProPage)

// Construction
public:
	void EnableMyWindow( void );
	void InitPage( CWaitCallObj * pObj ,CDrawDoc *pDoc );

	CWaitCallSpecificProPage();
	~CWaitCallSpecificProPage();
private:
// Dialog Data
	//{{AFX_DATA(CWaitCallSpecificProPage)
	enum { IDD = IDD_PAGE_WAITCALL_SPC };
	UINT	m_nRings;
	BOOL	m_bClearDtmf;
	BOOL	m_bReciveCallerID;
	BOOL	m_bReciveCalledID;
	CString	m_strSaveCalled;
	CString	m_strSaveCallerID;
	BOOL	m_bAnswerCall;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWaitCallSpecificProPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWaitCallSpecificProPage)
	afx_msg void OnButtonSavecalled();
	afx_msg void OnButtonSavecallerid();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckCdtmf();
	afx_msg void OnCheckRcallerid();
	afx_msg void OnCheckRcalledid();
	afx_msg void OnCheckAnswercall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDrawDoc * m_pDoc;
	CWaitCallObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAITCALLSPECIFICPROPAGE_H__2D2D42F0_D910_4A6E_BB0C_C50E5BBB4A45__INCLUDED_)
