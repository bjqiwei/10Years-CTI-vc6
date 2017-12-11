// NewSQLExcuteObj.cpp: implementation of the CNewSQLExcuteObj class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chiefivr.h"
#include "CIF5188.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CNewSQLExcuteObj, CSQLExcuteObj, 0)
CNewSQLExcuteObj::CNewSQLExcuteObj()
{
	m_pDB = NULL;
	m_pRS = NULL;
	m_str = "";
	m_strError="";
}

CNewSQLExcuteObj::~CNewSQLExcuteObj()
{

}
CNewSQLExcuteObj::CNewSQLExcuteObj( CPoint point )
	: CSQLExcuteObj(point)
{
	m_pDB = NULL;
	m_pRS = NULL;
	m_str = "";
	m_strError="";

}
void CNewSQLExcuteObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CSQLExcuteObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
		m_pDB->Serialize( ar );
		m_pRS->Serialize( ar );
		ar << m_str;
		ar << m_strError;
	}
	else//读数据
	{

		m_pDB->Serialize( ar );
		m_pRS->Serialize( ar );
		ar >> m_str;
		ar >> m_strError;

	}


}

CNewSQLExcuteObj::CNewSQLExcuteObj( const CSQLExcuteObj &s )
:CSQLExcuteObj(s)
{
	m_pDB = NULL;
	m_pRS = NULL;
	m_str = "";
	m_strError = "";

}

CNewSQLExcuteObj::CNewSQLExcuteObj( const CNewSQLExcuteObj &s )
	:CSQLExcuteObj(s)
{

	m_pDB = s.m_pDB;
	m_pRS = s.m_pRS;
	m_str = s.m_str;
	m_strError = s.m_strError;
}
CNewSQLExcuteObj & CNewSQLExcuteObj::operator =( const CNewSQLExcuteObj &s )
{
	if(this == &s)
		return *this;

	CNewSQLExcuteObj::operator = (s);
	m_pDB = s.m_pDB;
	m_pRS = s.m_pRS;
	m_str = s.m_str;
	m_strError = s.m_strError;

	return *this;
	
}
