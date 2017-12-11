// NewEndCallBlockObj.cpp: implementation of the CNewEndCallBlockObj class.
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
IMPLEMENT_SERIAL(CNewEndCallBlockObj, CNodeObj, 0)
CNewEndCallBlockObj::CNewEndCallBlockObj()
{

}

CNewEndCallBlockObj::~CNewEndCallBlockObj()
{
	while ( m_ChildObjLst.GetHeadPosition() )
	{
		delete m_ChildObjLst.RemoveHead();
	}
}
CNewEndCallBlockObj::CNewEndCallBlockObj( const CNewEndCallBlockObj &s )
	:CNodeObj(s)
{
	NewAssignsObjList( s.m_ChildObjLst,&m_ChildObjLst,0);
}
CNewEndCallBlockObj::CNewEndCallBlockObj( const CEndCallBlockObj &s )
	:CNodeObj(s)
{
	NewAssignsObjList( s.m_ChildObjLst,&m_ChildObjLst,1);
}
CNewEndCallBlockObj & CNewEndCallBlockObj::operator =( const CNewEndCallBlockObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	NewAssignsObjList( s.m_ChildObjLst,&m_ChildObjLst,0);

	return *this;
	
}

void CNewEndCallBlockObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{

		m_ChildObjLst.Serialize(ar);
	}
	else//读数据
	{

		m_ChildObjLst.Serialize(ar);
	}
	
	return;

}
CNodeObj * CNewEndCallBlockObj::FindNodeObj(int nKey, int &nParentKey)
{
	CNodeObjList *pObjLst = &(m_ChildObjLst);
	POSITION pos = pObjLst->GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj = pObjLst->GetNext(pos);
		if( pObj == NULL ) continue;
		if( nKey == pObj->m_nKey )
		{
			nParentKey = m_nKey;
			return pObj;
		}

	}

	return NULL;
}
/////////////////////////////////////
//查找属于Block的节点
//
//////////////////////////////////////
CNodeObj * CNewEndCallBlockObj::FindObj(int nKey)
{
	CNodeObjList *pObjLst = &(m_ChildObjLst);
	POSITION pos = pObjLst->GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj = pObjLst->GetNext(pos);
		if( pObj == NULL ) continue;
		if( nKey == pObj->m_nKey )
		{
			return pObj;
		}

	}

	return NULL;
}

CNodeObj * CNewEndCallBlockObj::GetEndCallEventCheckObj()
{
	CNodeObjList *pObjLst = &(m_ChildObjLst);
	POSITION pos = pObjLst->GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj = pObjLst->GetNext(pos);
		if( pObj == NULL ) continue;
		if( NODETYPE_ENDCALLEVENT_CHECK == pObj->m_nType )
		{
			return pObj;
		}

	}

	return NULL;
}