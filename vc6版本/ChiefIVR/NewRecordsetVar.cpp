// NewRecordsetVar.cpp: implementation of the CNewRecordsetVar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CIF5188.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNewRecordsetVar::CNewRecordsetVar()
{
	m_pRecordset = NULL;
	m_pRecordset.CreateInstance("ADODB.Recordset");
	m_pCommand	= NULL;
	m_pCommand.CreateInstance("ADODB.Command");
}

CNewRecordsetVar::~CNewRecordsetVar()
{
	//m_pRecordset->Release();
}
CNewRecordsetVar::CNewRecordsetVar( const CNewRecordsetVar &s )
:CRecordsetVar(s)
{
	m_pRecordset = s.m_pRecordset;
	m_pCommand = s.m_pCommand;
}
CNewRecordsetVar::CNewRecordsetVar( const CRecordsetVar &s )
:CRecordsetVar(s)
{
	m_pRecordset = NULL;
	m_pRecordset.CreateInstance("ADODB.Recordset");
	m_pCommand	= NULL;
	m_pCommand.CreateInstance("ADODB.Command");
}
CNewRecordsetVar & CNewRecordsetVar::operator =( const CNewRecordsetVar &s )
{
	if(this == &s)
		return *this;

	CRecordsetVar::operator = (s);

	m_pRecordset		= s.m_pRecordset ;
	m_pCommand = s.m_pCommand;

	return *this;
}

void CNewRecordsetVar::PrintComError(CString &ErrStr , _com_error &e)
{
   _bstr_t bstrSource(e.Source());
   _bstr_t bstrDescription(e.Description());
   ErrStr.Format("[Code = %08lx][Code meaning = %s][Source = %s][Description = %s]",
	   e.Error(),e.ErrorMessage(),(LPCSTR) bstrSource,(LPCSTR) bstrDescription );
   return;
}
