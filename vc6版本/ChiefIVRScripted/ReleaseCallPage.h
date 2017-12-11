#if !defined(AFX_RELEASECALLPAGE_H__79C92C0D_ECFF_47A1_ABE8_657A4968EAEF__INCLUDED_)
#define AFX_RELEASECALLPAGE_H__79C92C0D_ECFF_47A1_ABE8_657A4968EAEF__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReleaseCallPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReleaseCallPage dialog

class CReleaseCallPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CReleaseCallPage)

// Construction
public:
	void InitPage( CReleaseCallObj * pNodeObj );
	CReleaseCallPage();
	~CReleaseCallPage();

// Dialog Data
	//{{AFX_DATA(CReleaseCallPage)
	enum { IDD = IDD_PAGE_RELEASECALL_SPC };
	BOOL	m_bSaveDetail;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CReleaseCallPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CReleaseCallPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckSaveDetail();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CReleaseCallObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RELEASECALLPAGE_H__79C92C0D_ECFF_47A1_ABE8_657A4968EAEF__INCLUDED_)
