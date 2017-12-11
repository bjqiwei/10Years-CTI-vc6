#if !defined(AFX_ADDNORMALVARIABLE_H__438E32DC_559E_47C0_ABE6_A591179A24B5__INCLUDED_)
#define AFX_ADDNORMALVARIABLE_H__438E32DC_559E_47C0_ABE6_A591179A24B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddNormalVariable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddNormalVariable dialog

class CAddNormalVariable : public CDialog
{
// Construction
public:

	CAddNormalVariable(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddNormalVariable)
	enum { IDD = IDD_DIALOG_ADDNORMALVAR };
	CListCtrl	m_varList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddNormalVariable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddNormalVariable)
	afx_msg void OnButtonNew();
	afx_msg void OnButtonEdit();
	afx_msg void OnButtonDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDNORMALVARIABLE_H__438E32DC_559E_47C0_ABE6_A591179A24B5__INCLUDED_)
