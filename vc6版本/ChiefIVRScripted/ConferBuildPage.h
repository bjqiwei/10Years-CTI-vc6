#if !defined(AFX_CONFERBUILDPAGE_H__19A004E7_A9B6_4BB6_9B58_7DCBA2000082__INCLUDED_)
#define AFX_CONFERBUILDPAGE_H__19A004E7_A9B6_4BB6_9B58_7DCBA2000082__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConferBuildPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConferBuildPage dialog
class CDrawDoc;
class CConferBuildPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CConferBuildPage)

// Construction
public:
	void EnableMyWindow( void );
	void InitPage(CConferenceBulidObj *pObj, CDrawDoc *pDoc);
	CConferBuildPage();
	~CConferBuildPage();

// Dialog Data
	//{{AFX_DATA(CConferBuildPage)
	enum { IDD = IDD_PAGE_CONFER_BULID };
	int		m_nBuildType;
	CString	m_strConferNO;
	CString	m_strMaxMember;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConferBuildPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConferBuildPage)
	afx_msg void OnButtonVar();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonMaxmembervar();
	afx_msg void OnRadioBuildtype1();
	afx_msg void OnRadioBuildtype2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CConferenceBulidObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFERBUILDPAGE_H__19A004E7_A9B6_4BB6_9B58_7DCBA2000082__INCLUDED_)
