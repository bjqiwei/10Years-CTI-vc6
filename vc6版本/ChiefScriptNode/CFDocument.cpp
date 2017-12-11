// CFDocument.cpp : implementation file
//

#include "stdafx.h"
#include "NodeObj.h"

/////////////////////////////////////////////////////////////////////////////
// CCFDocument
////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CCFDocument, CDocument)

CCFDocument::CCFDocument()
{
	m_nBaseKey = 100;
	m_nVarBaseKey = 100;

}

BOOL CCFDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CCFDocument::~CCFDocument()
{

	while( m_objects.GetHeadPosition() )
	{
		delete m_objects.RemoveHead();
	}

	while( m_DBConnVarList.GetHeadPosition() )
	{
		delete m_DBConnVarList.RemoveHead();
	}

	while( m_RecordsetVariList.GetHeadPosition() )
	{
		delete m_RecordsetVariList.RemoveHead();
	}

	while( m_NormalVarList.GetHeadPosition() )
	{
		delete m_NormalVarList.RemoveHead();
	}

	while( m_DLLVariableList.GetHeadPosition() )
	{
		delete m_DLLVariableList.RemoveHead();
	}
	while( m_ParameterLst.GetHeadPosition() )
	{
		delete m_ParameterLst.RemoveHead();
	}

	return;
}


BEGIN_MESSAGE_MAP(CCFDocument, CDocument)
	//{{AFX_MSG_MAP(CCFDocument)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCFDocument diagnostics

#ifdef _DEBUG
void CCFDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void CCFDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCFDocument serialization

void CCFDocument::Serialize(CArchive& ar)
{

	if (ar.IsStoring())
	{
		m_objects.Serialize(ar);
		m_RecordsetVariList.Serialize(ar);
		m_DBConnVarList.Serialize(ar);
		m_NormalVarList.Serialize(ar);
		m_DLLVariableList.Serialize(ar);
		ar << m_nBaseKey;
		ar << m_nVarBaseKey;
		ar << m_nDocType;
		m_ParameterLst.Serialize(ar);

	}
	else
	{
		m_objects.Serialize(ar);
		m_RecordsetVariList.Serialize(ar);
		m_DBConnVarList.Serialize(ar);
		m_NormalVarList.Serialize(ar);
		m_DLLVariableList.Serialize(ar);
		ar >> m_nBaseKey;
		ar >> m_nVarBaseKey;
		ar >> m_nDocType;
		m_ParameterLst.Serialize(ar);

	}


}

/////////////////////////////////////////////////////////////////////////////
// CCFDocument commands
