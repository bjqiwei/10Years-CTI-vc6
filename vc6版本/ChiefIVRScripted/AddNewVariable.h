#if !defined(AFX_ADDNEWVARIABLE_H__A64918E2_3D92_4749_B0FE_B49781DFBE9E__INCLUDED_)
#define AFX_ADDNEWVARIABLE_H__A64918E2_3D92_4749_B0FE_B49781DFBE9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddNewVariable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddNewVariable dialog

class CAddNewVariable : public CDialog
{
// Construction
public:
	CString m_strType;
	CAddNewVariable(CWnd* pParent = NULL);   // standard constructor
	bool CheckNameIsAvail( CString strName );
// Dialog Data
	//{{AFX_DATA(CAddNewVariable)
	enum { IDD = IDD_DIALOG_ADDVAR };
	CComboBox	m_cobType;
	CString	m_strName;
	CString	m_strValues;
	CString	m_strDescription;
	//}}AFX_DATA

		
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddNewVariable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddNewVariable)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditValues();
	afx_msg void OnButtonOk();
	afx_msg void OnChangeEditName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	BOOL bIsNumber(CString str);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDNEWVARIABLE_H__A64918E2_3D92_4749_B0FE_B49781DFBE9E__INCLUDED_)
