// MyMultiDocTemplate.h: interface for the CMyMultiDocTemplate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMULTIDOCTEMPLATE_H__B5FD9F4F_BBD2_4AC3_9968_97795A07C3A1__INCLUDED_)
#define AFX_MYMULTIDOCTEMPLATE_H__B5FD9F4F_BBD2_4AC3_9968_97795A07C3A1__INCLUDED_

#include "DrawDoc.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyMultiDocTemplate : public CMultiDocTemplate  
{
public:
	virtual CDrawDoc* OpenDocumentFile(LPCTSTR lpszPathName, BOOL bMakeVisible = TRUE,int nDocType=0);
	CMyMultiDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
	virtual ~CMyMultiDocTemplate();

};

#endif // !defined(AFX_MYMULTIDOCTEMPLATE_H__B5FD9F4F_BBD2_4AC3_9968_97795A07C3A1__INCLUDED_)
