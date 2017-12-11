// CifCommServerDoc.cpp : implementation of the CCifCommServerDoc class
//

#include "stdafx.h"
#include "CifCommServer.h"

#include "CifCommServerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCifCommServerDoc

IMPLEMENT_DYNCREATE(CCifCommServerDoc, CDocument)

BEGIN_MESSAGE_MAP(CCifCommServerDoc, CDocument)
	//{{AFX_MSG_MAP(CCifCommServerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCifCommServerDoc construction/destruction

CCifCommServerDoc::CCifCommServerDoc()
{
	// TODO: add one-time construction code here

}

CCifCommServerDoc::~CCifCommServerDoc()
{
}

BOOL CCifCommServerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCifCommServerDoc serialization

void CCifCommServerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCifCommServerDoc diagnostics

#ifdef _DEBUG
void CCifCommServerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCifCommServerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCifCommServerDoc commands
