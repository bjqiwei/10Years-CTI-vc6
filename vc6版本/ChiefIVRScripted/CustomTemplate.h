// CustomTemplate.h: interface for the CCustomTemplate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CUSTOMTEMPLATE_H__A67B84CF_71B1_4096_B37D_D863292FE583__INCLUDED_)
#define AFX_CUSTOMTEMPLATE_H__A67B84CF_71B1_4096_B37D_D863292FE583__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCustomTemplateDoc : public CDocument  
{
protected: // create from serialization only
	
	DECLARE_DYNCREATE(CCustomTemplateDoc)
public:
	CCustomTemplateDoc();
	virtual ~CCustomTemplateDoc();
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);
	CNodeObjList m_ObjLst;
	int m_nBaseKey;
};

#endif // !defined(AFX_CUSTOMTEMPLATE_H__A67B84CF_71B1_4096_B37D_D863292FE583__INCLUDED_)
