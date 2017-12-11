#if !defined(AFX_GETCONFERMEMBERPAGE_H__1BF68C57_53AC_48AB_9B62_ECE40DBADC3A__INCLUDED_)
#define AFX_GETCONFERMEMBERPAGE_H__1BF68C57_53AC_48AB_9B62_ECE40DBADC3A__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetConferMemberPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGetConferMemberPage dialog
class CDrawDoc;
class CGetConferMemberPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CGetConferMemberPage)

// Construction
public:
	void InitPage(CGetConferMemberObj *pObj, CDrawDoc *pDoc);
	CGetConferMemberPage();
	~CGetConferMemberPage();

// Dialog Data
	//{{AFX_DATA(CGetConferMemberPage)
	enum { IDD = IDD_PPAGE_GETCONFMEMBER };
	CString	m_strConferNO;
	CString	m_strMemberCount;
	CString	m_strListenNum;
	CString	m_strMaxMemberNum;
	CString	m_strMaxSpeakerNum;
	CString	m_strMaxSpeakingNum;
	CString	m_strSpeakerNum;
	CString	m_strSpeakingNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGetConferMemberPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGetConferMemberPage)
	afx_msg void OnButtonConfno();
	afx_msg void OnButtonConfmember();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonMaxSpeaking();
	afx_msg void OnButtonMaxSpeaker();
	afx_msg void OnButtonMaxMember();
	afx_msg void OnButtonSpeaking();
	afx_msg void OnButtonSpeaker();
	afx_msg void OnButtonListen();
	afx_msg void OnDoubleclickedButtonConfno();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CGetConferMemberObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETCONFERMEMBERPAGE_H__1BF68C57_53AC_48AB_9B62_ECE40DBADC3A__INCLUDED_)
