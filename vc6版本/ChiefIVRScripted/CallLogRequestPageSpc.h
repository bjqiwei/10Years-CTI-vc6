#if !defined(AFX_CALLLOGREQUESTPAGESPC_H__26501FCF_AD49_4EF1_9634_D8B91E4B4701__INCLUDED_)
#define AFX_CALLLOGREQUESTPAGESPC_H__26501FCF_AD49_4EF1_9634_D8B91E4B4701__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallLogRequestPageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCallLogRequestPageSpc dialog
class CDrawDoc;
class CCallLogRequestPageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CCallLogRequestPageSpc)

// Construction
public:
	void InitPage(CCallLogRequestObj *pObj, CDrawDoc *pDoc);
	CCallLogRequestPageSpc();
	~CCallLogRequestPageSpc();

// Dialog Data
	//{{AFX_DATA(CCallLogRequestPageSpc)
	enum { IDD = IDD_PAGE_CALLLOGREQUEST_SPC };
	int		m_nRequestType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCallLogRequestPageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCallLogRequestPageSpc)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CCallLogRequestObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLLOGREQUESTPAGESPC_H__26501FCF_AD49_4EF1_9634_D8B91E4B4701__INCLUDED_)
