#if !defined(AFX_FUNCTIONPAGE_H__5C9D8151_28B8_4EF3_849B_695B89893986__INCLUDED_)
#define AFX_FUNCTIONPAGE_H__5C9D8151_28B8_4EF3_849B_695B89893986__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FunctionPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFunctionPage dialog
class CDrawDoc;
class CFunctionPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CFunctionPage)

// Construction
public:
	void InitPage(CFunctionObj *pObj, CDrawDoc *pDoc);
	CString m_strFunction;
	CFunctionPage();
	~CFunctionPage();

// Dialog Data
	//{{AFX_DATA(CFunctionPage)
	enum { IDD = IDD_PAGE_FUNCTION };
	CEdit	m_ctrFunction;
	CString	m_strReturnValues;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFunctionPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFunctionPage)
	afx_msg void OnButtonFunction();
	afx_msg void OnButtonReturn();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CFunctionObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNCTIONPAGE_H__5C9D8151_28B8_4EF3_849B_695B89893986__INCLUDED_)
