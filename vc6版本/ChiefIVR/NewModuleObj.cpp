// NewModuleObj.cpp: implementation of the CNewModuleObj class.
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
 void NewAssignsObjList( const CNodeObjList &s , CNodeObjList *d,int nAssigns )
{

	POSITION  pos = s.GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj = s.GetNext(pos);
		if( pObj == NULL ) continue;
		int nType = pObj->m_nType;
		if( NODETYPE_WAITCALL == nType )
		{
			CWaitCallObj * pNewObj = new CWaitCallObj;
			*pNewObj = *(CWaitCallObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_START_PLAY == nType )
		{
			CPlayStartObj * pNewObj = new CPlayStartObj;
			*pNewObj = *(CPlayStartObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_CHECK_DIAL == nType )
		{
			CDialCheckObj * pNewObj = new CDialCheckObj;
			*pNewObj = *(CDialCheckObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_RELEASECALL == nType )
		{
			CReleaseCallObj * pNewObj = new CReleaseCallObj;
			*pNewObj = *(CReleaseCallObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_CALLHOLD == nType )
		{
			CCallHoldObj * pNewObj = new CCallHoldObj;
			*pNewObj = *(CCallHoldObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_START_RECORD == pObj->m_nType )
		{
			CRecordObj * pNewObj = new CRecordObj;
			*pNewObj = *(CRecordObj *)pObj;
			d->AddTail(pNewObj);

		}
		else if( NODETYPE_CHECK_TRANSFER == nType )
		{
			CTransferCheckObj * pNewObj = new CTransferCheckObj;
			*pNewObj = *(CTransferCheckObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_OPEN_DATABASE == nType )
		{
			CDBOpenObj * pNewObj = new CDBOpenObj;
			*pNewObj = *(CDBOpenObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_CLOSE_DATABASE == nType )
		{
			CDBCloseObj * pNewObj = new CDBCloseObj;
			*pNewObj = *(CDBCloseObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_EXCUTE_SQL == nType )
		{
			if( 0 == nAssigns )
			{
				CNewSQLExcuteObj * pNewObj = new CNewSQLExcuteObj;
				*pNewObj = *(CNewSQLExcuteObj *)pObj;
				d->AddTail(pNewObj);
			}
			else
			{
				CNewSQLExcuteObj * pNewObj = new CNewSQLExcuteObj( *(CSQLExcuteObj *)pObj );
				d->AddTail(pNewObj);
			}
		}
		else if( NODETYPE_DLL_LOAD == nType )
		{
			CDLLLoadObj * pNewObj = new CDLLLoadObj;
			*pNewObj = *(CDLLLoadObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_DLL_UNLOAD == nType )
		{
			CDLLUnLoadObj * pNewObj = new CDLLUnLoadObj;
			*pNewObj = *(CDLLUnLoadObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_DLL_CALL == nType )
		{
			CDLLCallObj * pNewObj = new CDLLCallObj;
			*pNewObj = *(CDLLCallObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_RECIVE_DTMF == nType )
		{
			CDTMFReciveObj * pNewObj = new CDTMFReciveObj;
			*pNewObj = *(CDTMFReciveObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_SEND_DTMFCODE == pObj->m_nType )
		{
			CDTMFSendObj * pNewObj = new CDTMFSendObj;
			*pNewObj = *(CDTMFSendObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_DTMF_MENU == nType )
		{
			CDTMFMenuObj * pNewObj = new CDTMFMenuObj;
			*pNewObj = *(CDTMFMenuObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_EVENT_CHECK == nType )
		{
			CEventCheckObj * pNewObj = new CEventCheckObj;
			*pNewObj = *(CEventCheckObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_DATABASE_BROWSE == nType )
		{
			CDBBrowseObj * pNewObj = new CDBBrowseObj;
			*pNewObj = *(CDBBrowseObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_CALLLOGREQUEST == nType )
		{
			CCallLogRequestObj * pNewObj = new CCallLogRequestObj;
			*pNewObj = *(CCallLogRequestObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_SWITCH  == nType )
		{
			CSwitchObj * pNewObj = new CSwitchObj;
			*pNewObj = *(CSwitchObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_COMPARE  == nType )
		{
			CCompareObj * pNewObj = new CCompareObj;
			*pNewObj = *(CCompareObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_ENTER_CONFERENCE == pObj->m_nType )
		{
			CEnterConferObj * pNewObj = new CEnterConferObj;
			*pNewObj = *(CEnterConferObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_LEAVE_CONFERENCE == pObj->m_nType )
		{
			CLeaveConferObj * pNewObj = new CLeaveConferObj;
			*pNewObj = *(CLeaveConferObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_FAXRECIVE == pObj->m_nType )
		{
			CFaxReciveObj * pNewObj =  new CFaxReciveObj;
			*pNewObj = *(CFaxReciveObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_FAXSEND == pObj->m_nType )
		{
			CFaxSendObj * pNewObj =  new CFaxSendObj;
			*pNewObj = *(CFaxSendObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_CONFERENCE_BUILD == pObj->m_nType )
		{
			CConferenceBulidObj * pNewObj =  new CConferenceBulidObj;
			*pNewObj = *(CConferenceBulidObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_CONFERENCE_DELETE == pObj->m_nType )
		{
			CConferenceDeleteObj * pNewObj =  new CConferenceDeleteObj;
			*pNewObj = *(CConferenceDeleteObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_GET_CONFERENCE_MEMBER == pObj->m_nType )
		{
			CGetConferMemberObj * pNewObj =  new CGetConferMemberObj;
			*pNewObj = *(CGetConferMemberObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if(  NODETYPE_CONFERENCE_REQUEST == pObj->m_nType )
		{
			CConferRequestObj * pNewObj =  new CConferRequestObj;
			*pNewObj = *(CConferRequestObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_BLOCK == pObj->m_nType )
		{
			if( 0 == nAssigns )
			{
				CNewBlockObj * pNewObj =  new CNewBlockObj;
				*pNewObj = *(CNewBlockObj *)pObj;
				d->AddTail(pNewObj);
			}
			else
			{
				CNewBlockObj * pNewObj =  new CNewBlockObj( *(CBlockObj *)pObj );
				d->AddTail(pNewObj);
			}
		}
		else if( NODETYPE_STOREPRO == pObj->m_nType )
		{
			if( 0 == nAssigns )
			{
				CNewStoreProObj * pNewObj =  new CNewStoreProObj;
				*pNewObj = *(CNewStoreProObj *)pObj;
				d->AddTail(pNewObj);
			}
			else
			{
				CNewStoreProObj * pNewObj =  new CNewStoreProObj(*(CStoreProObj *)pObj);
				d->AddTail(pNewObj);
			}
		}
		else if( NODETYPE_CHANGEVOICE == pObj->m_nType )
		{
			CChangeVoiceObj * pNewObj =  new CChangeVoiceObj;
			*pNewObj = *(CChangeVoiceObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_ENDCALL_BLOCK == pObj->m_nType )
		{
			if( 0 == nAssigns )
			{
				CNewEndCallBlockObj * pNewObj =  new CNewEndCallBlockObj;
				*pNewObj = *(CNewEndCallBlockObj *)pObj;
				d->AddTail(pNewObj);
			}
			else
			{
				CNewEndCallBlockObj * pNewObj =  new CNewEndCallBlockObj(*(CEndCallBlockObj *)pObj);
				d->AddTail(pNewObj);
			}
		}
		else if( NODETYPE_EXPRESS == pObj->m_nType )
		{
			CExpressObj * pNewObj =  new CExpressObj;
			*pNewObj = *(CExpressObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_FUNCTION == pObj->m_nType )
		{
			CFunctionObj * pNewObj =  new CFunctionObj;
			*pNewObj = *(CFunctionObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_GOTO == pObj->m_nType )
		{
			CGoToObj * pNewObj =  new CGoToObj;
			*pNewObj = *(CGoToObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_TRACE_INFO == pObj->m_nType )
		{
			CTraceObj * pNewObj =  new CTraceObj;
			*pNewObj = *(CTraceObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_MOUDLE == pObj->m_nType )
		{
			if( 0 == nAssigns )
			{
				CNewModuleObj * pNewObj =  new CNewModuleObj;
				*pNewObj = *(CNewModuleObj *)pObj;
				d->AddTail(pNewObj);
			}
			else
			{
				CNewModuleObj * pNewObj =  new CNewModuleObj( *(CModuleObj *)pObj );
				d->AddTail(pNewObj);
			}
		}
		else if( NODETYPE_CUSTOM == pObj->m_nType )
		{
			CCustomObj * pNewObj =  new CCustomObj;
			*pNewObj = *(CCustomObj *)pObj;
			pNewObj->MoveFirst();
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_CC_CALLAGENT == pObj->m_nType )
		{
			CCCCallAgentObj * pNewObj =  new CCCCallAgentObj;
			*pNewObj = *(CCCCallAgentObj *)pObj;
			d->AddTail(pNewObj);
		}
		else if( NODETYPE_CC_TRANSFER == pObj->m_nType )
		{
			CCCTransferObj * pNewObj =  new CCCTransferObj;
			*pNewObj = *(CCCTransferObj *)pObj;
			d->AddTail(pNewObj);

		}
		else
		{
			CNodeObj * pNewObj = new CNodeObj;
			*pNewObj = *pObj;
			d->AddTail(pNewObj);
		}

	}
	
	return;
}

IMPLEMENT_SERIAL(CNewModuleObj, CNodeObj, 0)
CNewModuleObj::CNewModuleObj()
{
	m_pCurObj = NULL;
	m_pCurDBConn = NULL;
	m_pCurRecset = NULL;
	m_pCurStoreProObj = NULL;
}

CNewModuleObj::~CNewModuleObj()
{
	while( m_ParameterLst.GetHeadPosition() )
	{
		delete m_ParameterLst.RemoveHead();
	}
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
}

CNewModuleObj::CNewModuleObj( const CModuleObj &s )
	:CNodeObj(s)
{
	m_pCurDBConn = NULL;
	m_pCurRecset = NULL;
	m_pCurStoreProObj = NULL;
	m_pCurObj = s.m_objects.GetHead();
	m_strFile = s.m_strFile;

	POSITION pos = s.m_ParameterLst.GetHeadPosition();
	while( pos )
	{
		CParameter *pParmObj = s.m_ParameterLst.GetNext(pos);
		if( pParmObj != NULL )
		{
			CParameter *pNewObj = new CParameter;
			*pNewObj = *pParmObj;
			m_ParameterLst.AddTail( pNewObj );
		}
	}

	pos = s.m_DBConnVarList.GetHeadPosition();
	while (pos != NULL)
	{
		CDBConnVar *pDBObj = s.m_DBConnVarList.GetNext(pos);
		if( NULL != pDBObj )
		{
			CNewDBConnVar *pNewObj = new CNewDBConnVar(*pDBObj);
			m_DBConnVarList.AddTail(pNewObj);
		}
	}


	pos = s.m_DLLVariableList.GetHeadPosition();
	while (pos)
	{
		CDLLVariable * pNewObj = new CDLLVariable;
		*pNewObj = *( s.m_DLLVariableList.GetNext(pos) );
		m_DLLVariableList.AddTail(pNewObj);
	}

	pos = s.m_NormalVarList.GetHeadPosition();
	while (pos)
	{
		CNormalVar * pNewObj = new CNormalVar;
		*pNewObj = *( s.m_NormalVarList.GetNext(pos) );
		m_NormalVarList.AddTail(pNewObj);
	}

	pos = s.m_RecordsetVariList.GetHeadPosition();
	while (pos != NULL)
	{
		CRecordsetVar *pRsObj = s.m_RecordsetVariList.GetNext(pos);
		if( NULL != pRsObj )
		{
			
			CNewRecordsetVar *pNewObj = new CNewRecordsetVar(*pRsObj);
			m_RecordsetVariList.AddTail(pNewObj);
		}

	}

	NewAssignsObjList( s.m_objects, &(m_objects),1);
	
}
void CNewModuleObj::Serialize(CArchive &ar)
{
	ASSERT_VALID(this);
	CNodeObj::Serialize( ar );

	if (ar.IsStoring())//保存数据
	{
	}
	else//读数据
	{
	}
	
	return;

}
CNewModuleObj::CNewModuleObj( const CNewModuleObj &s )
	:CNodeObj(s)
{
	m_pCurDBConn = s.m_pCurDBConn;
	m_pCurRecset = s.m_pCurRecset;
	m_pCurStoreProObj = s.m_pCurStoreProObj;
	m_strFile = s.m_strFile;
	m_pCurObj = s.m_pCurObj;
	POSITION pos = s.m_ParameterLst.GetHeadPosition();
	while( pos )
	{
		CParameter *pObj = s.m_ParameterLst.GetNext(pos);
		if( pObj != NULL )
		{
			CParameter *pNewObj = new CParameter;
			*pNewObj = *pObj;
			m_ParameterLst.AddTail( pNewObj );
		}
	}
	pos = s.m_DBConnVarList.GetHeadPosition();
	while (pos)
	{
		CNewDBConnVar * pNewObj = new CNewDBConnVar;
		*pNewObj = *( s.m_DBConnVarList.GetNext(pos) );
		m_DBConnVarList.AddTail(pNewObj);
	}

	pos = s.m_DLLVariableList.GetHeadPosition();
	while (pos)
	{
		CDLLVariable * pNewObj = new CDLLVariable;
		*pNewObj = *( s.m_DLLVariableList.GetNext(pos) );
		m_DLLVariableList.AddTail(pNewObj);
	}

	pos = s.m_NormalVarList.GetHeadPosition();
	while (pos)
	{
		CNormalVar * pNewObj = new CNormalVar;
		*pNewObj = *( s.m_NormalVarList.GetNext(pos) );
		m_NormalVarList.AddTail(pNewObj);
	}

	pos = s.m_RecordsetVariList.GetHeadPosition();
	while (pos)
	{
		CNewRecordsetVar * pNewObj = new CNewRecordsetVar;
		*pNewObj = *( s.m_RecordsetVariList.GetNext(pos) );
		m_RecordsetVariList.AddTail(pNewObj);
	}
	
	NewAssignsObjList( s.m_objects,&m_objects,0);
	
}
CNewModuleObj & CNewModuleObj::operator =( const CNewModuleObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);
	m_pCurDBConn = s.m_pCurDBConn;
	m_pCurRecset = s.m_pCurRecset;
	m_pCurStoreProObj = s.m_pCurStoreProObj;
	m_pCurObj = s.m_pCurObj;
	m_strFile = s.m_strFile;


	POSITION pos = s.m_ParameterLst.GetHeadPosition();
	while( pos )
	{
		CParameter *pObj = s.m_ParameterLst.GetNext(pos);
		if( pObj != NULL )
		{
			CParameter *pNewObj = new CParameter;
			*pNewObj = *pObj;
			m_ParameterLst.AddTail( pNewObj );
		}
	}

	pos = s.m_DBConnVarList.GetHeadPosition();
	while (pos)
	{
		CNewDBConnVar * pNewObj = new CNewDBConnVar;
		*pNewObj = *( s.m_DBConnVarList.GetNext(pos) );
		m_DBConnVarList.AddTail(pNewObj);
	}

	pos = s.m_DLLVariableList.GetHeadPosition();
	while (pos)
	{
		CDLLVariable * pNewObj = new CDLLVariable;
		*pNewObj = *( s.m_DLLVariableList.GetNext(pos) );
		m_DLLVariableList.AddTail(pNewObj);
	}

	pos = s.m_NormalVarList.GetHeadPosition();
	while (pos)
	{
		CNormalVar * pNewObj = new CNormalVar;
		*pNewObj = *( s.m_NormalVarList.GetNext(pos) );
		m_NormalVarList.AddTail(pNewObj);
	}

	pos = s.m_RecordsetVariList.GetHeadPosition();
	while (pos)
	{
		CNewRecordsetVar * pNewObj = new CNewRecordsetVar;
		*pNewObj = *( s.m_RecordsetVariList.GetNext(pos) );
		m_RecordsetVariList.AddTail(pNewObj);
	}

	NewAssignsObjList( s.m_objects, &m_objects,0);


	return *this;
	
}

//////////////////////////////////////////////////////////////////////
// CNewBlockObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CNewBlockObj, CNodeObj, 0)
CNewBlockObj::CNewBlockObj()
{

}

CNewBlockObj::~CNewBlockObj()
{
	while ( m_ChildObjLst.GetHeadPosition() )
	{
		delete m_ChildObjLst.RemoveHead();
	}
}
CNewBlockObj::CNewBlockObj( const CNewBlockObj &s )
	:CNodeObj(s)
{
	NewAssignsObjList( s.m_ChildObjLst,&m_ChildObjLst,0);
}
CNewBlockObj::CNewBlockObj( const CBlockObj &s )
	:CNodeObj(s)
{
	NewAssignsObjList( s.m_ChildObjLst,&m_ChildObjLst,1);
}
CNewBlockObj & CNewBlockObj::operator =( const CNewBlockObj &s )
{
	if(this == &s)
		return *this;

	CNodeObj::operator = (s);

	NewAssignsObjList( s.m_ChildObjLst,&m_ChildObjLst,0);

	return *this;
	
}
void CNewBlockObj::Serialize(CArchive &ar)
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
CNodeObj * CNewBlockObj::FindNodeObj(int nKey, int &nParentKey)
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
		else
		{
			if(  NODETYPE_BLOCK == pObj->m_nType )
			{
				CNewBlockObj *pBlockObj = (CNewBlockObj *)pObj;
				CNodeObj *pObj1 = pBlockObj->FindNodeObj( nKey,nParentKey);//递归调用
				if( NULL != pObj1 )
				{
					return pObj1;
				}
			}
		}

	}

	return NULL;
}

/////////////////////////////////////
//查找属于Block的节点
//
//////////////////////////////////////
CNodeObj * CNewBlockObj::FindObj(int nKey)
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
		else
		{
			if(  NODETYPE_BLOCK == pObj->m_nType )
			{
				CNewBlockObj *pBlockObj = (CNewBlockObj *)pObj;
				CNodeObj *pObj1 = pBlockObj->FindObj( nKey);//递归调用
				if( NULL != pObj1 )
					return pObj1;
			}
		}

	}

	return NULL;
}


