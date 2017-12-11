#if !defined(AFX_SYSMSGFORM_H__22949B80_589B_4601_AD9A_331C845FDCCA__INCLUDED_)
#define AFX_SYSMSGFORM_H__22949B80_589B_4601_AD9A_331C845FDCCA__INCLUDED_

#include "CFListBox.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysMsgForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysMsgForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "SystemErrorLogFile.h"	// Added by ClassView

class CSysMsgForm : public CFormView
{
protected:
	CSysMsgForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSysMsgForm)

// Form Data
public:
	//{{AFX_DATA(CSysMsgForm)
	enum { IDD = IDD_SYSMSG_FORM };
	CMyListBox	m_lstSysMsg;
	//}}AFX_DATA
public:
	int m_InitialUpdate;
	int m_cy;
	int m_cx;
// Attributes
public:
	int m_nIsSaveSysLog;
// Operations
public:
	void CreateSysLogFile();
	void WriteStringToLogFile(LPCTSTR lpsz);
	CSystemErrorLogFile m_SystemErrorLogFile;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysMsgForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSysMsgForm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CSysMsgForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSMSGFORM_H__22949B80_589B_4601_AD9A_331C845FDCCA__INCLUDED_)
