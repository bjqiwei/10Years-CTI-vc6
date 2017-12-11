#if !defined(AFX_EXPRESSPAGE_H__F4C9EF29_E75C_4736_8E98_07F27F95BE12__INCLUDED_)
#define AFX_EXPRESSPAGE_H__F4C9EF29_E75C_4736_8E98_07F27F95BE12__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExpressPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExpressPage dialog
class CDrawDoc;
class CExpressPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CExpressPage)

// Construction
public:
	void InitPage(CExpressObj *pObj, CDrawDoc *pDoc);
	CString m_strExpress;
	CExpressPage();
	~CExpressPage();

// Dialog Data
	//{{AFX_DATA(CExpressPage)
	enum { IDD = IDD_PAGE_EXPRESS };
	CEdit	m_ctrExpress;
	CString	m_strExpressValues;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CExpressPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CExpressPage)
	afx_msg void OnButtonExpress();
	afx_msg void OnButtonExpressvalues();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CExpressObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPRESSPAGE_H__F4C9EF29_E75C_4736_8E98_07F27F95BE12__INCLUDED_)
