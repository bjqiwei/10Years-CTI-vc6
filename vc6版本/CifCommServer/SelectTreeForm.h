#if !defined(AFX_SELECTTREEFORM_H__F7D524B3_B39D_481C_9A12_6C41C53B1474__INCLUDED_)
#define AFX_SELECTTREEFORM_H__F7D524B3_B39D_481C_9A12_6C41C53B1474__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectTreeForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectTreeForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CSelectTreeForm : public CFormView
{
protected:
	CSelectTreeForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSelectTreeForm)

// Form Data
public:
	//{{AFX_DATA(CSelectTreeForm)
	enum { IDD = IDD_TREE_FORM };
	CTreeCtrl	m_selectTree;
	//}}AFX_DATA
public:
	int m_InitialUpdate;
	int m_cy;
	int m_cx;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectTreeForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSelectTreeForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CSelectTreeForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkTreeSelect(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTTREEFORM_H__F7D524B3_B39D_481C_9A12_6C41C53B1474__INCLUDED_)
