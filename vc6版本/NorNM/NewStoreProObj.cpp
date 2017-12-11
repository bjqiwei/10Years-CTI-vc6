// NewStoreProObj.cpp: implementation of the CNewStoreProObj class.
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
IMPLEMENT_SERIAL(CNewStoreProObj, CStoreProObj, 0)
CNewStoreProObj::CNewStoreProObj()
{
	m_pDB = NULL;
	m_pRS = NULL;
	m_str = "";
	m_strError="";
	m_pChnlInfo = NULL;
}

CNewStoreProObj::~CNewStoreProObj()
{

}
CNewStoreProObj::CNewStoreProObj( CPoint point )
	: CStoreProObj(point)
{
	m_pDB = NULL;
	m_pRS = NULL;
	m_str = "";
	m_strError="";
	m_pChnlInfo = NULL;

}
void CNewStoreProObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNewStoreProObj::Serialize( ar );

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

CNewStoreProObj::CNewStoreProObj( const CStoreProObj &s )
:CStoreProObj(s)
{
	m_pDB = NULL;
	m_pRS = NULL;
	m_str = "";
	m_strError = "";
	m_pChnlInfo = NULL;
}

CNewStoreProObj::CNewStoreProObj( const CNewStoreProObj &s )
	:CStoreProObj(s)
{

	m_pDB = s.m_pDB;
	m_pRS = s.m_pRS;
	m_str = s.m_str;
	m_strError = s.m_strError;
	m_pChnlInfo = s.m_pChnlInfo;
}
CNewStoreProObj & CNewStoreProObj::operator =( const CNewStoreProObj &s )
{
	if(this == &s)
		return *this;

	CNewStoreProObj::operator = (s);
	m_pDB = s.m_pDB;
	m_pRS = s.m_pRS;
	m_str = s.m_str;
	m_strError = s.m_strError;
	m_pChnlInfo = s.m_pChnlInfo;
	return *this;
	
}