#if !defined(AFX_SELFDEFINESPAGE_H__7C4BDDC0_2BFD_4B57_A265_6CAD4816B93F__INCLUDED_)
#define AFX_SELFDEFINESPAGE_H__7C4BDDC0_2BFD_4B57_A265_6CAD4816B93F__INCLUDED_

#include "ListBoxExBuddy.h"	// Added by ClassView
#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelfDefinesPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelfDefinesPage dialog
class CDrawDoc;
class CSelfDefinesPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CSelfDefinesPage)
public:
	CListBoxExBuddy m_ActionListBoxExBuddy;
	CListBoxExBuddy m_EventListBoxExBuddy;
// Construction
public:

	LRESULT OnActionNew( WPARAM wParam,LPARAM lParam);
	LRESULT OnActionEdit( WPARAM wParam,LPARAM lParam);
	LRESULT OnActionDel( WPARAM wParam,LPARAM lParam);
	LRESULT OnActionUP( WPARAM wParam,LPARAM lParam);
	LRESULT OnActionDown( WPARAM wParam,LPARAM lParam);

	LRESULT OnEventNew( WPARAM wParam,LPARAM lParam);
	LRESULT OnEventEdit( WPARAM wParam,LPARAM lParam);
	LRESULT OnEventDel( WPARAM wParam,LPARAM lParam);
	LRESULT OnEventUP( WPARAM wParam,LPARAM lParam);
	LRESULT OnEventDown( WPARAM wParam,LPARAM lParam);

	void InitPage(CCustomObj *pNodeObj, CDrawDoc *pDoc);
	CSelfDefinesPage();
	~CSelfDefinesPage();

// Dialog Data
	//{{AFX_DATA(CSelfDefinesPage)
	enum { IDD = IDD_PAGE_SELFDEFINENODE_SPC };
	CListCtrl	m_lstEvent;
	CListCtrl	m_lstAction;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSelfDefinesPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSelfDefinesPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListEvent(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListAction(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LRESULT OnNew( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEdit( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDel( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUP( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDown( WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CCustomObj * m_pNodeObj;
	CDrawDoc * m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELFDEFINESPAGE_H__7C4BDDC0_2BFD_4B57_A265_6CAD4816B93F__INCLUDED_)
