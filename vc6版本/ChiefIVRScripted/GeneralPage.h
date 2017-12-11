#if !defined(AFX_GENERALPAGE_H__B5261451_605B_46EE_B628_2C4191896CE0__INCLUDED_)
#define AFX_GENERALPAGE_H__B5261451_605B_46EE_B628_2C4191896CE0__INCLUDED_


#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GeneralPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage dialog
class CDrawDoc;
class CGeneralPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CGeneralPage)

// Construction
public:
	bool CheckNameIsAvail( CString strName );
	CString m_strPanelMsg;
	void InitPage( CNodeObj *pObj, CDrawDoc *pDoc  );
	CGeneralPage();
	~CGeneralPage();
private:
// Dialog Data
	//{{AFX_DATA(CGeneralPage)
	enum { IDD = IDD_PAGE_GENERAL };
	CEdit	m_edtPanelMsg;
	CString	m_strName;
	CString	m_strDescription;
	CString	m_strKey;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CGeneralPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CGeneralPage)
	afx_msg void OnChangeEditName();
	afx_msg void OnButtonMsg();
	afx_msg void OnChangeEditPanelmsg();
	afx_msg void OnSetfocusEditPanelmsg();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CNodeObj * m_pNodeObj;
	CDrawDoc *m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERALPAGE_H__B5261451_605B_46EE_B628_2C4191896CE0__INCLUDED_)
