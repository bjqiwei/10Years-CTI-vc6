#if !defined(AFX_DLLCALLPAGESPC_H__26042D1B_67C9_4163_8F28_2CC1D28AC6A7__INCLUDED_)
#define AFX_DLLCALLPAGESPC_H__26042D1B_67C9_4163_8F28_2CC1D28AC6A7__INCLUDED_

#include "ListBoxExBuddy.h"	// Added by ClassView
#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLLCallPageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLLCallPageSpc dialog
class CDrawDoc;
class CDLLCallPageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CDLLCallPageSpc)

// Construction
public:
	CListBoxExBuddy m_ListBoxExBuddy;
	void InitPage(CDLLCallObj *pNodeObj, CDrawDoc *pDoc);
	CDLLCallPageSpc();
	~CDLLCallPageSpc();

// Dialog Data
	//{{AFX_DATA(CDLLCallPageSpc)
	enum { IDD = IDD_PAGE_DLLCALL_SPC };
	CComboBox	m_cobDLL;
	CListCtrl	m_lstParam;
	CString	m_strFunction;
	int		m_nParmType;
	CString	m_strParmVar;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDLLCallPageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDLLCallPageSpc)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonVar();
	afx_msg void OnClickListParameter(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnNew( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEdit( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDel( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUP( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDown( WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CString m_strDLL;
	CDLLCallObj * m_pNodeObj;
	CDrawDoc *m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLCALLPAGESPC_H__26042D1B_67C9_4163_8F28_2CC1D28AC6A7__INCLUDED_)
