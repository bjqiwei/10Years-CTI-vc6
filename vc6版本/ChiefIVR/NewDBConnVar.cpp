// NewDBConnVar.cpp: implementation of the CNewDBConnVar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxmt.h>
#include "CIF5188.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


extern CCriticalSection g_Section;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNewDBConnVar::CNewDBConnVar()
{
	m_pConnection = NULL;
	m_bConnect = FALSE;
	m_pConnection.CreateInstance("ADODB.Connection");
	m_pConnection->ConnectionTimeout = 5;
	m_pRecordset = NULL;
	m_pRecordset.CreateInstance("ADODB.Recordset");
	m_pCommand	= NULL;
	m_pCommand.CreateInstance("ADODB.Command");

}

CNewDBConnVar::~CNewDBConnVar()
{
	CString strError("");
	CloseDatabase(strError);
	m_pConnection = NULL;

}

BOOL CNewDBConnVar::CloseDatabase( CString &strError )
{
	try
	{
		if( m_pConnection )
		if( m_pConnection->GetState() != adStateClosed )
		{
			m_pConnection->Close();	
		}
		m_bConnect = FALSE;
		return TRUE;

	}
	catch(_com_error e)///²¶×½Òì³£
	{
		strError.Format("%s",(LPTSTR)e.Description());
		return FALSE;
	} 

}

BOOL CNewDBConnVar::OpenDataBase( CString strConn, CString &strError )
{
	try
	{
		m_strConn = strConn;
		if( "" == strConn ) 
		{
			strError = "not set Connection string";
			m_bConnect = FALSE;
			return FALSE;
		}
		if( m_pConnection )
		{
			if( adStateClosed == m_pConnection->GetState() )
			{
				//m_pConnection.CreateInstance("ADODB.Connection");
				//("driver={SQL Server};Server=laojiang;DATABASE=pbx;UID=sa;PWD=123","","",adModeUnknown);
				m_pConnection->Open( (_bstr_t)strConn,"","",adModeUnknown);
			}
			m_bConnect = TRUE;
			return TRUE;
		}
		else
		{
			m_bConnect = FALSE;
			return FALSE ;
		}

	}
	
	catch(_com_error e)///²¶×½Òì³£
	{
		strError.Format("%s",(LPTSTR)e.Description());
		m_bConnect = FALSE;
		return FALSE;
	} 
	
	
}

CNewDBConnVar::CNewDBConnVar( const CNewDBConnVar &s )
				:CDBConnVar(s)
{
	m_pConnection		= s.m_pConnection ;
	m_bConnect = s.m_bConnect ;

	m_pRecordset = NULL;
	m_pRecordset.CreateInstance("ADODB.Recordset");
	m_pCommand	= NULL;
	m_pCommand.CreateInstance("ADODB.Command");

}
CNewDBConnVar::CNewDBConnVar( const CDBConnVar &s )
				:CDBConnVar(s)
{

	m_bConnect = FALSE;
	m_pConnection = NULL;
	m_pConnection.CreateInstance("ADODB.Connection");
	m_pConnection->ConnectionTimeout = 5;
	m_pRecordset = NULL;
	m_pRecordset.CreateInstance("ADODB.Recordset");
	m_pCommand	= NULL;
	m_pCommand.CreateInstance("ADODB.Command");
	
}

CNewDBConnVar & CNewDBConnVar::operator =( const CNewDBConnVar &s )
{
	if(this == &s)
		return *this;

	CDBConnVar::operator = (s);

	m_pConnection		= s.m_pConnection ;
	m_bConnect = s.m_bConnect ;

	m_pRecordset = NULL;
	m_pRecordset.CreateInstance("ADODB.Recordset");
	m_pCommand	= NULL;
	m_pCommand.CreateInstance("ADODB.Command");
	


	return *this;

}


BOOL CNewDBConnVar::ExcuteSQL( CNewRecordsetVar *RS, CString strSQL, CString &strError )
{
	_variant_t RecordsAffected;
	try
	{
		g_Section.Lock();
		RS->m_pRecordset = m_pConnection->Execute( (_bstr_t)strSQL,&RecordsAffected,adCmdText);
		g_Section.Unlock();
	}
	catch(_com_error e )
	{
		strError.Format("%s",(LPTSTR)e.Description());
		g_Section.Unlock();
		return FALSE;
	}

	return TRUE;
}

BOOL CNewDBConnVar::ExcuteSQL(CString strSQL, CString &strError)
{
	_variant_t RecordsAffected;
	try
	{
		g_Section.Lock();
		m_pConnection->Execute( (_bstr_t)strSQL,&RecordsAffected,adCmdText);
		g_Section.Unlock();
	}
	catch(_com_error e )
	{
		strError.Format("%s",(LPTSTR)e.Description());
		g_Section.Unlock();
		return FALSE;
	}

	return TRUE;
}

void CNewDBConnVar::PrintComError(CString &ErrStr, _com_error &e)
{
   _bstr_t bstrSource(e.Source());
   _bstr_t bstrDescription(e.Description());
   ErrStr.Format("[Code = %08lx][Code meaning = %s][Source = %s][Description = %s]",
	   e.Error(),e.ErrorMessage(),(LPCSTR) bstrSource,(LPCSTR) bstrDescription );
   return;
}

void CNewDBConnVar::PrintProviderError(CString &ErrStr, _ConnectionPtr pConnection)
{
	if( NULL == pConnection ) return;
   // Print Provider Errors from Connection object.
   // pErr is a record object in the Connection's Error collection.
   ErrorPtr pErr = NULL;
   CString strTemp = "";
   if ( (pConnection->Errors->Count) > 0 ) {
      long nCount = pConnection->Errors->Count;
      // Collection ranges from 0 to nCount -1.
      for ( long i = 0 ; i < nCount ; i++ ) {
         pErr = pConnection->Errors->GetItem(i);
         strTemp.Format("Error number: %x\t%s\n", pErr->Number, (LPCSTR) pErr->Description);
      }
   }


   ErrStr = strTemp;
}
