// CustomTemplate.cpp: implementation of the CCustomTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CustomTemplate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CCustomTemplateDoc, CDocument)
CCustomTemplateDoc::CCustomTemplateDoc()
{
	m_nBaseKey = 9000;
}

CCustomTemplateDoc::~CCustomTemplateDoc()
{

	while( m_ObjLst.GetHeadPosition() )
	{
		delete m_ObjLst.RemoveHead();
	}
}

void CCustomTemplateDoc::Serialize(CArchive &ar)
{
	if (ar.IsStoring())
	{
		m_ObjLst.Serialize(ar);
		
		ar << m_nBaseKey;
	}
	else
	{
		m_ObjLst.Serialize(ar);
		ar >> m_nBaseKey;
	}
}

BOOL CCustomTemplateDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (IsModified())
		TRACE0("Warning: Open Document replaces an unsaved document.\n");
	CString strError("");
	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		strError.Format("it is error that open the %s",lpszPathName);
		AfxMessageBox(strError);
		return FALSE;
	}

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			Serialize(loadArchive);     // load me
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		DeleteContents();   // remove failed contents

		TRY
		{
			ReportSaveLoadException(lpszPathName, e,
				FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		}
		END_TRY
		//DELETE_EXCEPTION(e);
		return FALSE;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // start off with unmodified

	return TRUE;
}
