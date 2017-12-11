#if !defined(AFX_STOREPROCPAGE_H__62153EB3_578E_4CB4_9397_B7F7FF5D65CD__INCLUDED_)
#define AFX_STOREPROCPAGE_H__62153EB3_578E_4CB4_9397_B7F7FF5D65CD__INCLUDED_

#include "ListBoxExBuddy.h"	// Added by ClassView
#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StoreProcPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStoreProcPage dialog
class CDrawDoc;
class CStoreProcPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CStoreProcPage)

// Construction
public:
	CString m_strDB;
	CListBoxExBuddy m_ListBoxExBuddy;
	void InitPage( CStoreProObj *pNodeObj, CDrawDoc *pDoc);
	CStoreProcPage();
	~CStoreProcPage();

// Dialog Data
	//{{AFX_DATA(CStoreProcPage)
	enum { IDD = IDD_PPAGE_STOREPROC };
	CComboBox	m_cobDB;
	CListCtrl	m_lstParam;
	CString	m_strStoreProName;
	CString	m_strParameterVar;
	CString	m_strRSName;
	BOOL	m_bReturnRecordSet;
	CString	m_strReturn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStoreProcPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CStoreProcPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonParamvalue();
	afx_msg void OnClickListParameter(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonReturnvalue();
	//}}AFX_MSG
	afx_msg LRESULT OnNew( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEdit( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDel( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUP( WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDown( WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CStoreProObj * m_pNodeObj;
	CDrawDoc * m_pDoc;
	CRecordsetVar * m_pRecsetVar;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOREPROCPAGE_H__62153EB3_578E_4CB4_9397_B7F7FF5D65CD__INCLUDED_)
