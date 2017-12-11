// CFComm.h : main header file for the CFCOMM DLL
//

#if !defined(AFX_CFCOMM_H__7164CA0A_6D35_4CB1_8F6C_97D2E161FF7E__INCLUDED_)
#define AFX_CFCOMM_H__7164CA0A_6D35_4CB1_8F6C_97D2E161FF7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCFCommApp
// See CFComm.cpp for the implementation of this class
//

class CCFCommApp : public CWinApp
{
public:
	CCFCommApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCFCommApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCFCommApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CFCOMM_H__7164CA0A_6D35_4CB1_8F6C_97D2E161FF7E__INCLUDED_)
