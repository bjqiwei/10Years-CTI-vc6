// ChiefIVRScripted.h : main header file for the CHIEFIVRSCRIPTED application
//

#if !defined(AFX_CHIEFIVRSCRIPTED_H__083301EC_DD7C_454A_8A8A_AACCB68FC98B__INCLUDED_)
#define AFX_CHIEFIVRSCRIPTED_H__083301EC_DD7C_454A_8A8A_AACCB68FC98B__INCLUDED_

#include "MyDocManager.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "MyDocManager.h"
#include "MainFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CChiefIVRScriptedApp:
// See ChiefIVRScripted.cpp for the implementation of this class
//

class CChiefIVRScriptedApp : public CWinApp
{
public:
	void AddDocTemplate(CDocTemplate* pTemplate);

	
	CChiefIVRScriptedApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChiefIVRScriptedApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CChiefIVRScriptedApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHIEFIVRSCRIPTED_H__083301EC_DD7C_454A_8A8A_AACCB68FC98B__INCLUDED_)
