// MyDocManager.h: interface for the CMyDocManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYDOCMANAGER_H__F26EF717_57CD_4887_A7B7_14B0C03EFA32__INCLUDED_)
#define AFX_MYDOCMANAGER_H__F26EF717_57CD_4887_A7B7_14B0C03EFA32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyDocManager : public CDocManager  
{
public:
	virtual void OnFileNew();
	CMyDocManager();
	virtual ~CMyDocManager();

};

#endif // !defined(AFX_MYDOCMANAGER_H__F26EF717_57CD_4887_A7B7_14B0C03EFA32__INCLUDED_)
