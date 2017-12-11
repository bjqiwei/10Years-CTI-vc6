#if !defined(AFX_SETVARIABLEPROPAGE_H__4037117C_8790_4146_A9AA_8D0B65472249__INCLUDED_)
#define AFX_SETVARIABLEPROPAGE_H__4037117C_8790_4146_A9AA_8D0B65472249__INCLUDED_

#include "..//include//NodeObj.h"
#include "ListBoxExBuddy.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetVariableProPage.h : header file
//

class CDrawDoc;
/////////////////////////////////////////////////////////////////////////////
// CSetVariableProPage dialog

class CSetVariableProPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSetVariableProPage)

// Construction
public:
	CListBoxExBuddy m_ListBoxExBuddy;
	BOOL JudgeNameValidity( CString strVarName );


	void InitPage( CNodeObj * pNodeObj, CDrawDoc *pDoc, int *pnBaseVarKey );

	CSetVariableProPage();
	~CSetVariableProPage();
private:
// Dialog Data
	//{{AFX_DATA(CSetVariableProPage)
	enum { IDD = IDD_PAGE_SETVARIABLE_SPC };
	CListCtrl	m_varList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSetVariableProPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSetVariableProPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemclickListVar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListVar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListVar(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnNew( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEdit( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDel( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUP( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDown( WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	CDrawDoc *m_pDoc;
	CNodeObj * m_pNodeObj;
	int *m_pnBaseVarKey;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETVARIABLEPROPAGE_H__4037117C_8790_4146_A9AA_8D0B65472249__INCLUDED_)
