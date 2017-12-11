#if !defined(AFX_CONFERDELETEPAGE_H__9CAB1BF8_FD79_436E_B37E_1F9FEA76F73E__INCLUDED_)
#define AFX_CONFERDELETEPAGE_H__9CAB1BF8_FD79_436E_B37E_1F9FEA76F73E__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConferDeletePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConferDeletePage dialog
class CDrawDoc;
class CConferDeletePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CConferDeletePage)

// Construction
public:
	void EnableMyWindow( void );
	void InitPage(CConferenceDeleteObj *pObj, CDrawDoc *pDoc);
	CConferDeletePage();
	~CConferDeletePage();

// Dialog Data
	//{{AFX_DATA(CConferDeletePage)
	enum { IDD = IDD_PAGE_CONFER_DELETE };
	int		m_nDeleteType;
	CString	m_strConferNO;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConferDeletePage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConferDeletePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonVar();
	afx_msg void OnRadioDeletetype1();
	afx_msg void OnRadioDeletetype2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CConferenceDeleteObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFERDELETEPAGE_H__9CAB1BF8_FD79_436E_B37E_1F9FEA76F73E__INCLUDED_)
