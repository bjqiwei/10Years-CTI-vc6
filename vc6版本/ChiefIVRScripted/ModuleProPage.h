#if !defined(AFX_MODULEPROPAGE_H__AEDC458B_B2E8_40E0_AE82_3D567677E5E5__INCLUDED_)
#define AFX_MODULEPROPAGE_H__AEDC458B_B2E8_40E0_AE82_3D567677E5E5__INCLUDED_


#include "..//include//NodeObj.h"
#include "ListBoxExBuddy.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModuleProPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModuleProPage dialog
class CDrawDoc;
class CModuleProPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CModuleProPage)

// Construction
public:

	void InitPage( CModuleObj *pObj, CDrawDoc *pDoc);
	CModuleProPage();
	~CModuleProPage();

// Dialog Data
	//{{AFX_DATA(CModuleProPage)
	enum { IDD = IDD_PAGE_MODULE };
	CListCtrl	m_lstModuleEnd;
	CListCtrl	m_ctrParamLst;
	CString	m_strModuleFile;
	CString	m_strVariable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CModuleProPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CModuleProPage)
	afx_msg void OnButtonModulfile();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonVar();
	afx_msg void OnClickListParameter(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnEdit( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUP( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDown( WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CModuleObj * m_pNodeObj;
	CListBoxExBuddy m_ListBoxExBuddy;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODULEPROPAGE_H__AEDC458B_B2E8_40E0_AE82_3D567677E5E5__INCLUDED_)
