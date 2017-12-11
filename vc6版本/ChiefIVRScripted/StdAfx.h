// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__5D8C2D46_48B8_4365_88C8_0F0518B7C484__INCLUDED_)
#define AFX_STDAFX_H__5D8C2D46_48B8_4365_88C8_0F0518B7C484__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#include <float.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include "htmlhelp.h"

void AFXAPI MyDDV_CheckChars(CDataExchange* pDX, CString const& value, int nMaxChars,int nMinChars, CString strMessage);

int GetImageIndex(UINT nIco);
void SetNodeImageList( CImageList *pImageList );
UINT GetImageIcoID( int ImageIndex );

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__5D8C2D46_48B8_4365_88C8_0F0518B7C484__INCLUDED_)
