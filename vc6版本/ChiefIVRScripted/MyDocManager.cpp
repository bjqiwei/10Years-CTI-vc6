// MyDocManager.cpp: implementation of the CMyDocManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "MyDocManager.h"
#include "NewScriptTypeSelect.h"
#include "DrawDoc.h"
#include "MyMultiDocTemplate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyDocManager::CMyDocManager()
{
	
}

CMyDocManager::~CMyDocManager()
{

}

void CMyDocManager::OnFileNew()
{

	if (m_templateList.IsEmpty())
	{
		TRACE0("Error: no document templates registered with CWinApp.\n");
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return;
	}
	int nDocType = 0;
	CMyMultiDocTemplate* pTemplate = (CMyMultiDocTemplate*)m_templateList.GetHead();
	if (m_templateList.GetCount() > 1)
	{
		CNewScriptTypeSelect dlg;
		int nID = dlg.DoModal();
		if (nID == IDOK)
		{
			nDocType = dlg.m_nDocType;
			if( 0 == nDocType )
				pTemplate = (CMyMultiDocTemplate*)m_templateList.GetHead();
			else
			{
				pTemplate = (CMyMultiDocTemplate*)m_templateList.GetTail();
			}
			
		}
		else
			return;     // none - cancel operation
	}

	ASSERT(pTemplate != NULL);
	//ASSERT_KINDOF(CDocTemplate, pTemplate);

	CDrawDoc *pDoc = (CDrawDoc *)pTemplate->OpenDocumentFile(NULL,TRUE,nDocType);
	
}


