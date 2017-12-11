// NodeData.cpp: implementation of the CNodeData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NodeObj.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CNodeData, CObject, 0)

CNodeData::CNodeData()
{
	m_strData = "";//数据结点保存数据
	m_strName = "";//数据结点名称
	m_nDataType = 0;
	m_nKey = -1;

}

CNodeData::CNodeData( const CNodeData &s )
{
	m_strData = s.m_strData ;
	m_strName = s.m_strName ;
	m_nDataType = s.m_nDataType ;
	m_nKey = s.m_nKey ;

}

CNodeData & CNodeData::operator =( const CNodeData &s )
{
	if(this == &s)
		return *this;

	m_strData = s.m_strData ;
	m_strName = s.m_strName ;
	m_nDataType = s.m_nDataType ;
	m_nKey = s.m_nKey ;


	return *this;

}

CNodeData::~CNodeData()
{
	m_strData = "";//数据结点保存数据
	m_strName = "";//数据结点名称
	m_nDataType = 0;
	m_nKey = -1;

}

void CNodeData::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);

	if( ar.IsStoring() )//保存数据
	{
		ar << m_nDataType;
		ar << m_nKey;
		ar << m_strData;
		ar << m_strName;

	}
	else//读数据
	{
		ar >> m_nDataType;
		ar >> m_nKey;
		ar >> m_strData;
		ar >> m_strName;

	}


	return;

}
