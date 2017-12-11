#if !defined(AFX_CCTRANSFERPAGE_H__BC5573BD_D6D0_4AB0_A881_ED23BE1181EF__INCLUDED_)
#define AFX_CCTRANSFERPAGE_H__BC5573BD_D6D0_4AB0_A881_ED23BE1181EF__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CcTransferPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCcTransferPage dialog
class CDrawDoc;
class CCcTransferPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCcTransferPage)

// Construction
public:
	void InitPage(CCCTransferObj *pNodeObj, CDrawDoc *pDoc);
	CCcTransferPage();
	~CCcTransferPage();

// Dialog Data
	//{{AFX_DATA(CCcTransferPage)
	enum { IDD = IDD_PAGE_CC_TRANSFER };
	int		m_nTransferType;
	UINT	m_nTimeOut;
	int		m_nWaitType;
	CString	m_strPlayFile;
	CString	m_strCompleteDTMF;
	CString	m_strCancelDTMF;
	CString	m_strAimDN;
	BOOL	m_bTimeOut;
	BOOL	m_bTalkWitDN;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCcTransferPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCcTransferPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAimdn();
	afx_msg void OnButtonPlayfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CCCTransferObj * m_pNodeObj;
	CDrawDoc * m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCTRANSFERPAGE_H__BC5573BD_D6D0_4AB0_A881_ED23BE1181EF__INCLUDED_)
