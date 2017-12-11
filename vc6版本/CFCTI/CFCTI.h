// CFCTI.h : main header file for the CFCTI DLL
//

#if !defined(AFX_CFCTI_H__60793C3E_BFC1_4A8C_BA0E_00E103AE04FF__INCLUDED_)
#define AFX_CFCTI_H__60793C3E_BFC1_4A8C_BA0E_00E103AE04FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCFCTIApp
// See CFCTI.cpp for the implementation of this class
//

class CCFCTIApp : public CWinApp
{
public:
	CCFCTIApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCFCTIApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCFCTIApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFCTI_H__60793C3E_BFC1_4A8C_BA0E_00E103AE04FF__INCLUDED_)
