#if !defined(AFX_NODEREMARKFORMVIEW_H__D8290C09_18E8_4C89_9518_170063E7BD18__INCLUDED_)
#define AFX_NODEREMARKFORMVIEW_H__D8290C09_18E8_4C89_9518_170063E7BD18__INCLUDED_

#include "DrawDoc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NodeRemarkFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNodeRemarkFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "DrawView.h"	// Added by ClassView

class CNodeRemarkFormView : public CFormView
{
protected:
	CNodeRemarkFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CNodeRemarkFormView)

// Form Data
public:
	//{{AFX_DATA(CNodeRemarkFormView)
	enum { IDD = IDD_NODEREMARK_FORMVIEW };
	CEdit	m_edtRemark;
	//}}AFX_DATA
protected:
	int m_InitialUpdate;
	int m_cy;
	int m_cx;
// Attributes
public:

// Operations
public:
	CDrawView * m_pDrawView;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNodeRemarkFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CNodeRemarkFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CNodeRemarkFormView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChangeEditRmark();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODEREMARKFORMVIEW_H__D8290C09_18E8_4C89_9518_170063E7BD18__INCLUDED_)
