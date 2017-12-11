#if !defined(AFX_ACTIVEMSGFORM_H__5325B99B_C05F_40C5_A40C_606EB9DE1693__INCLUDED_)
#define AFX_ACTIVEMSGFORM_H__5325B99B_C05F_40C5_A40C_606EB9DE1693__INCLUDED_

#include "CFListBox.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ActiveMsgForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CActiveMsgForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif



class CActiveMsgForm : public CFormView
{
protected:
	CActiveMsgForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CActiveMsgForm)

// Form Data
public:
	//{{AFX_DATA(CActiveMsgForm)
	enum { IDD = IDD_ACTIVEMSG_FORM };
	CMyListBox	m_lstSocket;
	CMyListBox	m_lstIVR;
	CMyListBox	m_lstCTI;
	CMyListBox	m_lstAgent;
	CMyListBox	m_lstAcd;
	CString	m_strTitle;
	//}}AFX_DATA
public:
	int m_InitialUpdate;
	int m_cy;
	int m_cx;
// Attributes
public:

// Operations
public:
	void ShowList(CString strItem );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CActiveMsgForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CActiveMsgForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CActiveMsgForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACTIVEMSGFORM_H__5325B99B_C05F_40C5_A40C_606EB9DE1693__INCLUDED_)
