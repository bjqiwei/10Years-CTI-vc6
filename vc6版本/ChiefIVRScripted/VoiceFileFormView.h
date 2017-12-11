#if !defined(AFX_VOICEFILEFORMVIEW_H__98241E89_CAC0_4BAA_993F_65FC33EB4D9D__INCLUDED_)
#define AFX_VOICEFILEFORMVIEW_H__98241E89_CAC0_4BAA_993F_65FC33EB4D9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VoiceFileFormView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVoiceFileFormView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CVoiceFileFormView : public CFormView
{
protected:
	CVoiceFileFormView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CVoiceFileFormView)

// Form Data
public:
	//{{AFX_DATA(CVoiceFileFormView)
	enum { IDD = IDD_VOXFILE_FORMVIEW };
	CListCtrl	m_voxfileList;
	//}}AFX_DATA
protected:
	int m_InitialUpdate;
	int m_cy;
	int m_cx;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVoiceFileFormView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CVoiceFileFormView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CVoiceFileFormView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VOICEFILEFORMVIEW_H__98241E89_CAC0_4BAA_993F_65FC33EB4D9D__INCLUDED_)
