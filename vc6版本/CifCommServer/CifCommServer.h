// CifCommServer.h : main header file for the CIFCOMMSERVER application
//

#if !defined(AFX_CIFCOMMSERVER_H__9FAA5B44_1997_4AB5_9C83_38F5B30724C0__INCLUDED_)
#define AFX_CIFCOMMSERVER_H__9FAA5B44_1997_4AB5_9C83_38F5B30724C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCifCommServerApp:
// See CifCommServer.cpp for the implementation of this class
//

class CCifCommServerApp : public CWinApp
{
public:
	CCifCommServerApp();
	BOOL m_bNormalShow;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCifCommServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCifCommServerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CIFCOMMSERVER_H__9FAA5B44_1997_4AB5_9C83_38F5B30724C0__INCLUDED_)
