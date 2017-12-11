#if !defined(AFX_BBBROWSESPC_H__D46E639F_A093_4BF2_A2AD_5A8C691A1C98__INCLUDED_)
#define AFX_BBBROWSESPC_H__D46E639F_A093_4BF2_A2AD_5A8C691A1C98__INCLUDED_

#include "..//include//NodeObj.h"
#include "ListBoxExBuddy.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BBBrowseSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBBBrowseSpc dialog
class CDrawDoc;
class CBBBrowseSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CBBBrowseSpc)

// Construction
public:
	CListBoxExBuddy m_ListBoxExBuddy;
	CString m_strRecordset;
	void InitPage( CDBBrowseObj * pObj ,CDrawDoc *pDoc );
	CBBBrowseSpc();
	~CBBBrowseSpc();

// Dialog Data
	//{{AFX_DATA(CBBBrowseSpc)
	enum { IDD = IDD_PAGE_DBBROWSE_SPC };
	CListCtrl	m_lstFieldValues;
	CComboBox	m_cobRecordset;
	int		m_nBrowseType;
	CString	m_strField;
	CString	m_strVar;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CBBBrowseSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CBBBrowseSpc)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelect();
	afx_msg void OnClickListFieldvalues(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnNew( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEdit( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDel( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUP( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDown( WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CDBBrowseObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BBBROWSESPC_H__D46E639F_A093_4BF2_A2AD_5A8C691A1C98__INCLUDED_)
