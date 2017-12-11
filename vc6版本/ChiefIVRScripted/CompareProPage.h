#if !defined(AFX_COMPAREPROPAGE_H__FD4C2BDA_C99C_4453_A432_AC160DFCE61F__INCLUDED_)
#define AFX_COMPAREPROPAGE_H__FD4C2BDA_C99C_4453_A432_AC160DFCE61F__INCLUDED_

#include "..//include//NodeObj.h"
#include "ListBoxExBuddy.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CompareProPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCompareProPage dialog
class CDrawDoc;
class CCompareProPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCompareProPage)

// Construction
public:
	CListBoxExBuddy m_ListBoxExBuddy;
	RemoveNextNode();
	void InitPage( CCompareObj *pObj, CDrawDoc *pDoc);
	CCompareProPage();
	~CCompareProPage();

// Dialog Data
	//{{AFX_DATA(CCompareProPage)
	enum { IDD = IDD_PAGE_COMPARE };
	CListCtrl	m_listVar;
	CString	m_strVariable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCompareProPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCompareProPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonVariable();
	afx_msg void OnItemclickListVar(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListVar(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnNew( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEdit( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDel( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUP( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDown( WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	CString GetTestTypeName( int nType );
	CDrawDoc * m_pDoc;
	CCompareObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMPAREPROPAGE_H__FD4C2BDA_C99C_4453_A432_AC160DFCE61F__INCLUDED_)
