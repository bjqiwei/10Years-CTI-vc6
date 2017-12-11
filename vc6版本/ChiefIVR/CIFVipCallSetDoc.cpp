// CIFVipCallSetDoc.cpp : implementation file
//

#include "stdafx.h"
#include "cfchh.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCIFVipCallSetDoc

IMPLEMENT_DYNCREATE(CCIFVipCallSetDoc, CDocument)

CCIFVipCallSetDoc::CCIFVipCallSetDoc()
{
}

BOOL CCIFVipCallSetDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CCIFVipCallSetDoc::~CCIFVipCallSetDoc()
{
	while( m_VipCallSetObjList.GetHeadPosition() )
	{
		delete m_VipCallSetObjList.RemoveHead();
	}
}


BEGIN_MESSAGE_MAP(CCIFVipCallSetDoc, CDocument)
	//{{AFX_MSG_MAP(CCIFVipCallSetDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCIFVipCallSetDoc diagnostics

#ifdef _DEBUG
void CCIFVipCallSetDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCIFVipCallSetDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCIFVipCallSetDoc serialization

void CCIFVipCallSetDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		m_VipCallSetObjList.Serialize(ar);
	}
	else
	{
		m_VipCallSetObjList.Serialize(ar);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCIFVipCallSetDoc commands

BOOL CCIFVipCallSetDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (IsModified())
		TRACE0("Warning: OnOpenDocument replaces an unsaved document.\n");

	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
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
		return FALSE;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // start off with unmodified

	return TRUE;
}
