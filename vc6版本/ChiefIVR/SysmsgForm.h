#if !defined(AFX_SYSMSGFORM_H__B6FB09D9_3D81_4691_9414_8D592C732473__INCLUDED_)
#define AFX_SYSMSGFORM_H__B6FB09D9_3D81_4691_9414_8D592C732473__INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysmsgForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysmsgForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "SystemErrorLogFile.h"	// Added by ClassView

class CSysmsgForm : public CFormView
{
protected:
	CSysmsgForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSysmsgForm)

// Form Data
public:
	//{{AFX_DATA(CSysmsgForm)
	enum { IDD = IDD_SYSMSG_FORM };
	CListBox	m_listSystem;
	//}}AFX_DATA

// Attributes
public:
	int m_InitialUpdate;
	int m_cy;
	int m_cx;
// Operations
public:	
	void ShowSysMsg(CString strMSG);
	void ShowSysMsg( int nChnl , CString strMSG );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysmsgForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSysmsgForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CSysmsgForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnSetfocusListSystem();
	afx_msg void OnSelchangeListSystem();
	afx_msg void OnSelcancelListSystem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSMSGFORM_H__B6FB09D9_3D81_4691_9414_8D592C732473__INCLUDED_)
