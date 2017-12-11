// CFVariable.cpp: implementation of the CCFVariable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NodeObj.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// CNormalVar
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CNormalVar, CObject, 0)
CNormalVar::CNormalVar()
{
	m_nVarDataType	= DATATYPE_STRING ;
	m_strName = _T("");
	m_nKey = 0;
	m_strVariable = _T("");
	m_strDescription= _T("");
}
CNormalVar::CNormalVar( const CNormalVar &s )
{
	m_strName		= s.m_strName;
	m_nVarDataType	= s.m_nVarDataType;
	m_nKey			= s.m_nKey ;
	m_strVariable = s.m_strVariable ;
	m_strDescription= s.m_strDescription ;
}

CNormalVar & CNormalVar::operator =( const CNormalVar &s )
{
	if(this == &s)
		return *this;
	m_strName		= s.m_strName;
	m_nVarDataType	= s.m_nVarDataType;
	m_nKey			= s.m_nKey ;
	m_strVariable = s.m_strVariable ;
	m_strDescription= s.m_strDescription ;
	return *this;
}

CNormalVar::~CNormalVar()
{

}

void CNormalVar::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if (ar.IsStoring())//保存数据
	{
		ar << m_strName;
		ar << m_nVarDataType;
		ar << m_nKey;
		ar << m_strVariable;
		ar << m_strDescription ;

	}
	else//读数据
	{
		ar >> m_strName;
		ar >> m_nVarDataType;
		ar >> m_nKey;
		ar >> m_strVariable;
		ar >> m_strDescription ;

	}


}
inline int	CNormalVar::GetDataType( CString strDataType ) 
{
	int nType = 0;

	if( strDataType =="Integer" )
		nType = DATATYPE_INTEGER;
	else if( strDataType =="Double" )
		nType = DATATYPE_FLOAT;
	else if( strDataType =="String" )
		nType = DATATYPE_STRING;
	else if( strDataType =="Date" )
		nType = DATATYPE_DATE;
	else if( strDataType =="Time" )
		nType = DATATYPE_TIME;
	else if( strDataType =="DateTime" )
		nType = DATATYPE_DATATIME;

	return nType;
}
inline CString	CNormalVar::GetDataTypeName( int nType ) 
{

	CString str="";
	switch(nType)
	{

	case DATATYPE_INTEGER:
		str="Integer";
		break;
	case DATATYPE_FLOAT:
		str="Double";
		break;
	case DATATYPE_STRING:
		str="String";
		break;
	case DATATYPE_DATE:
		str="Date";
		break;
	case DATATYPE_TIME:
		str="Time";
		break;
	case DATATYPE_DATATIME:
		str="DateTime";
		break;
	default:
		break;
	}

	return str;
}



//////////////////////////////////////////////////////////////////////
// CParameter
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CParameter, CObject, 0)
CParameter::CParameter()
{

	m_nKey = -1;
	m_strName = _T("");
	m_nVarKey = -1;
	m_strVariable = _T("");
	m_strDescription= _T("");
	m_strValues=_T("");
}
CParameter::CParameter( const CParameter &s )
{
	m_nKey	= s.m_nKey;
	m_strName	= s.m_strName;
	m_nVarKey	= s.m_nVarKey;
	m_strVariable = s.m_strVariable ;
	m_strDescription= s.m_strDescription ;
	m_strValues= s.m_strValues ;
}

CParameter & CParameter::operator =( const CParameter &s )
{
	if(this == &s)
		return *this;
	m_nKey	= s.m_nKey;
	m_strName		= s.m_strName;
	m_nVarKey	= s.m_nVarKey;
	m_strVariable = s.m_strVariable ;
	m_strDescription= s.m_strDescription ;
	m_strValues= s.m_strValues ;
	return *this;
}

CParameter::~CParameter()
{

}

void CParameter::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if (ar.IsStoring())//保存数据
	{
		ar << m_nKey;
		ar << m_strName;
		ar << m_nVarKey;
		ar << m_strVariable;
		ar << m_strDescription ;
		ar << m_strValues;

	}
	else//读数据
	{
		ar >> m_nKey;
		ar >> m_strName;
		ar >> m_nVarKey;
		ar >> m_strVariable;
		ar >> m_strDescription ;
		ar >> m_strValues;
	}


}

//////////////////////////////////////////////////////////////////////
// CRecordsetVar Class
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CRecordsetVar, CObject, 0)
CRecordsetVar::CRecordsetVar()
{
	m_strName = _T("");
	m_nVarDataType = DATATYPE_DBRECORDSET;
	m_nKey = 0;
	m_strDataBase= _T("");
	m_strSQL= _T("");
	m_nSQLType = 0;

}
CRecordsetVar::CRecordsetVar( const CRecordsetVar &s )
{
	m_strName		= s.m_strName;
	m_nVarDataType	= s.m_nVarDataType;
	m_nKey			= s.m_nKey ;
	m_strDataBase	= s.m_strDataBase ;
	m_strSQL		= s.m_strSQL ;
	m_nSQLType		= s.m_nSQLType ;
}

CRecordsetVar & CRecordsetVar::operator =( const CRecordsetVar &s )
{
	if(this == &s)
		return *this;
	m_strName		= s.m_strName;
	m_nVarDataType	= s.m_nVarDataType;
	m_nKey			= s.m_nKey ;
	m_strDataBase	= s.m_strDataBase ;
	m_strSQL		= s.m_strSQL ;
	m_nSQLType		= s.m_nSQLType ;

	return *this;
}

CRecordsetVar::~CRecordsetVar()
{

}
void CRecordsetVar::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if (ar.IsStoring())//保存数据
	{
		ar << m_strName;
		ar << m_nVarDataType;
		ar << m_nKey;
		ar << m_strDataBase;
		ar << m_strSQL;
		ar << m_nSQLType;
		
	}
	else//读数据
	{
		ar >> m_strName;
		ar >> m_nVarDataType;
		ar >> m_nKey;
		ar >> m_strDataBase;
		ar >> m_strSQL;
		ar >> m_nSQLType;

	}


}



//////////////////////////////////////////////////////////////////////
// CSystemVar Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CSystemVar, CObject, 0)
CSystemVar::CSystemVar()
{
	m_strName = _T("");
	m_nVarDataType = 0;
	m_nKey = 0;
}
CSystemVar::CSystemVar( const CSystemVar &s )
{
	m_strName = s.m_strName;
	m_nVarDataType = s.m_nVarDataType;
	m_nKey = s.m_nKey ;
}

CSystemVar & CSystemVar::operator =( const CSystemVar &s )
{
	if(this == &s)
		return *this;
	m_strName		= s.m_strName;
	m_nVarDataType	= s.m_nVarDataType;
	m_nKey			= s.m_nKey ;

	return *this;

}

CSystemVar::~CSystemVar()
{

}
void CSystemVar::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if (ar.IsStoring())//保存数据
	{
		ar << m_strName;
		ar << m_nKey;
	}
	else//读数据
	{
		ar >> m_strName;
		ar >> m_nKey;
	}


}

//////////////////////////////////////////////////////////////////////
// CDLLVariable Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CDLLVariable, CObject, 0)
CDLLVariable::CDLLVariable()
{
	m_hDLL = NULL;
	m_nVarDataType		= DATATYPE_DLL ;
	m_strName			= _T("");
	m_nKey				= 0;
	m_strDLLFileName	= _T("");

}
CDLLVariable::CDLLVariable( const CDLLVariable &s )
{
	m_hDLL				= s.m_hDLL;
	m_nVarDataType		= s.m_nVarDataType;
	m_strName			= s.m_strName;
	m_nKey				= s.m_nKey;
	m_strDLLFileName	= s.m_strDLLFileName;
}

CDLLVariable & CDLLVariable::operator =( const CDLLVariable &s )
{
	if(this == &s)
		return *this;

	m_hDLL				= s.m_hDLL;
	m_nVarDataType		= s.m_nVarDataType;
	m_strName			= s.m_strName;
	m_nKey				= s.m_nKey;
	m_strDLLFileName	= s.m_strDLLFileName;


	return *this;
}

CDLLVariable::~CDLLVariable()
{
	if( NULL != m_hDLL )
	{
		FreeLibrary(m_hDLL);
		m_hDLL = NULL;
	}
}
void CDLLVariable::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if (ar.IsStoring())//保存数据
	{
		
		ar << m_nVarDataType;
		ar << m_strName;
		ar << m_nKey;
		ar << m_strDLLFileName;


	}
	else//读数据
	{

		ar >> m_nVarDataType;
		ar >> m_strName;
		ar >> m_nKey;
		ar >> m_strDLLFileName;

	}

	return;
}