// ChiefIVR.h : main header file for the CHIEFIVR application
//

#if !defined(AFX_CHIEFIVR_H__B5B4A478_4DDC_40AE_BE4D_BD1411AF1A97__INCLUDED_)
#define AFX_CHIEFIVR_H__B5B4A478_4DDC_40AE_BE4D_BD1411AF1A97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CChiefIVRApp:
// See ChiefIVR.cpp for the implementation of this class
//

class CChiefIVRApp : public CWinApp
{
public:
	CString m_strPath;
	BOOL m_bNormalShow;
	CChiefIVRApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChiefIVRApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CChiefIVRApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHIEFIVR_H__B5B4A478_4DDC_40AE_BE4D_BD1411AF1A97__INCLUDED_)
