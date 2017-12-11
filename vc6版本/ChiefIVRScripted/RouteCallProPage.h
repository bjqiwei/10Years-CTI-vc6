#if !defined(AFX_ROUTECALLPROPAGE_H__D03A949C_B7BC_40C9_B84C_0B7601A7D005__INCLUDED_)
#define AFX_ROUTECALLPROPAGE_H__D03A949C_B7BC_40C9_B84C_0B7601A7D005__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RouteCallProPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRouteCallProPage dialog
class CDrawDoc;
class CRouteCallProPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CRouteCallProPage)

// Construction
public:
	CRouteCallProPage();
	~CRouteCallProPage();

// Dialog Data
	//{{AFX_DATA(CRouteCallProPage)
	enum { IDD = IDD_PAGE_ROUTECALL };
	CString	m_strTelNum;
	CString	m_strWaitFile;
	int		m_nWaitOption;
	CString	m_strEndCh;
	CString	m_strConnFile;
	CString	m_strBeginCH;
	int		m_nConnOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CRouteCallProPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRouteCallProPage)
	afx_msg void OnBtnTel();
	afx_msg void OnBtnBeginCh();
	afx_msg void OnBtnEndCh();
	afx_msg void OnBtnWaitfile();
	afx_msg void OnBtnConnfile();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void InitPage(CRouteCallObj *pNodeObj, CDrawDoc *pDoc);
private:
	CRouteCallObj * m_pNodeObj;
	CDrawDoc * m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROUTECALLPROPAGE_H__D03A949C_B7BC_40C9_B84C_0B7601A7D005__INCLUDED_)
