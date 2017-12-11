// NormalScriptInfo.cpp: implementation of the CNormalScriptInfo class.
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

CNormalScriptInfo::CNormalScriptInfo()
{
	m_strScriptFile = "";
}

CNormalScriptInfo::~CNormalScriptInfo()
{
	while( m_objects.GetHeadPosition() )
	{
		delete m_objects.RemoveHead();
	}
	while ( m_RecordsetVariList.GetHeadPosition() )
	{
		delete m_RecordsetVariList.RemoveHead();
	}
	
	while ( m_DBConnVarList.GetHeadPosition() )
	{
		delete m_DBConnVarList.RemoveHead();
	}
	
	while ( m_NormalVarList.GetHeadPosition() )
	{
		delete m_NormalVarList.RemoveHead();
	}

	while ( m_DLLVariableList.GetHeadPosition() )
	{
		delete m_DLLVariableList.RemoveHead();
	}
}

void CNormalScriptInfo::InitNodeObjList(CNodeObjList *pObjList, CNodeObj *pObj)
{
	if( NULL == pObj || NULL == pObjList ) return;
	int nType = pObj->m_nType;
	if( NODETYPE_WAITCALL == nType )
	{
		CWaitCallObj * pNewObj = new CWaitCallObj;
		*pNewObj = *(CWaitCallObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_START_PLAY == nType )
	{
		CPlayStartObj * pNewObj = new CPlayStartObj;
		*pNewObj = *(CPlayStartObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_CHECK_DIAL == nType )
	{
		CDialCheckObj * pNewObj = new CDialCheckObj;
		*pNewObj = *(CDialCheckObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_RELEASECALL == nType )
	{
		CReleaseCallObj * pNewObj = new CReleaseCallObj;
		*pNewObj = *(CReleaseCallObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_CALLHOLD == nType )
	{
		CCallHoldObj * pNewObj = new CCallHoldObj;
		*pNewObj = *(CCallHoldObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if(  NODETYPE_START_RECORD == pObj->m_nType )
	{
		CRecordObj * pNewObj = new CRecordObj;
		*pNewObj = *(CRecordObj *)pObj;
		pObjList->AddTail(pNewObj);

	}
	else if( NODETYPE_CHECK_TRANSFER == nType )
	{
		CTransferCheckObj * pNewObj = new CTransferCheckObj;
		*pNewObj = *(CTransferCheckObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_OPEN_DATABASE == nType )
	{
		CDBOpenObj * pNewObj = new CDBOpenObj;
		*pNewObj = *(CDBOpenObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_CLOSE_DATABASE == nType )
	{
		CDBCloseObj * pNewObj = new CDBCloseObj;
		*pNewObj = *(CDBCloseObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_EXCUTE_SQL == nType )
	{
		CNewSQLExcuteObj * pNewObj = new CNewSQLExcuteObj( *(CSQLExcuteObj *)pObj );
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_DLL_LOAD == nType )
	{
		CDLLLoadObj * pNewObj = new CDLLLoadObj;
		*pNewObj = *(CDLLLoadObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_DLL_UNLOAD == nType )
	{
		CDLLUnLoadObj * pNewObj = new CDLLUnLoadObj;
		*pNewObj = *(CDLLUnLoadObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_DLL_CALL == nType )
	{
		CDLLCallObj * pNewObj = new CDLLCallObj;
		*pNewObj = *(CDLLCallObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_RECIVE_DTMF == nType )
	{
		CDTMFReciveObj * pNewObj = new CDTMFReciveObj;
		*pNewObj = *(CDTMFReciveObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if(  NODETYPE_SEND_DTMFCODE == pObj->m_nType )
	{
		CDTMFSendObj * pNewObj = new CDTMFSendObj;
		*pNewObj = *(CDTMFSendObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_DTMF_MENU == nType )
	{
		CDTMFMenuObj * pNewObj = new CDTMFMenuObj;
		*pNewObj = *(CDTMFMenuObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_EVENT_CHECK == nType )
	{
		CEventCheckObj * pNewObj = new CEventCheckObj;
		*pNewObj = *(CEventCheckObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_DATABASE_BROWSE == nType )
	{
		CDBBrowseObj * pNewObj = new CDBBrowseObj;
		*pNewObj = *(CDBBrowseObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_CALLLOGREQUEST == nType )
	{
		CCallLogRequestObj * pNewObj = new CCallLogRequestObj;
		*pNewObj = *(CCallLogRequestObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_SWITCH  == nType )
	{
		CSwitchObj * pNewObj = new CSwitchObj;
		*pNewObj = *(CSwitchObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_COMPARE  == nType )
	{
		CCompareObj * pNewObj = new CCompareObj;
		*pNewObj = *(CCompareObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if(  NODETYPE_ENTER_CONFERENCE == pObj->m_nType )
	{
		CEnterConferObj * pNewObj = new CEnterConferObj;
		*pNewObj = *(CEnterConferObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if(  NODETYPE_LEAVE_CONFERENCE == pObj->m_nType )
	{
		CLeaveConferObj * pNewObj = new CLeaveConferObj;
		*pNewObj = *(CLeaveConferObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if(  NODETYPE_FAXRECIVE == pObj->m_nType )
	{
		CFaxReciveObj * pNewObj =  new CFaxReciveObj;
		*pNewObj = *(CFaxReciveObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if(  NODETYPE_FAXSEND == pObj->m_nType )
	{
		CFaxSendObj * pNewObj =  new CFaxSendObj;
		*pNewObj = *(CFaxSendObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if(  NODETYPE_CONFERENCE_BUILD == pObj->m_nType )
	{
		CConferenceBulidObj * pNewObj =  new CConferenceBulidObj;
		*pNewObj = *(CConferenceBulidObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if(  NODETYPE_CONFERENCE_DELETE == pObj->m_nType )
	{
		CConferenceDeleteObj * pNewObj =  new CConferenceDeleteObj;
		*pNewObj = *(CConferenceDeleteObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if(  NODETYPE_GET_CONFERENCE_MEMBER == pObj->m_nType )
	{
		CGetConferMemberObj * pNewObj =  new CGetConferMemberObj;
		*pNewObj = *(CGetConferMemberObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if(  NODETYPE_CONFERENCE_REQUEST == pObj->m_nType )
	{
		CConferRequestObj * pNewObj =  new CConferRequestObj;
		*pNewObj = *(CConferRequestObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_BLOCK == pObj->m_nType )
	{
		CNewBlockObj * pNewObj =  new CNewBlockObj(*(CBlockObj *)pObj);
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_STOREPRO == pObj->m_nType )
	{
		CNewStoreProObj * pNewObj =  new CNewStoreProObj(*(CStoreProObj *)pObj);
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_CHANGEVOICE == pObj->m_nType )
	{
		CChangeVoiceObj * pNewObj =  new CChangeVoiceObj;
		*pNewObj = *(CChangeVoiceObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_ENDCALL_BLOCK == pObj->m_nType )
	{
		CNewEndCallBlockObj * pNewObj =  new CNewEndCallBlockObj(*(CEndCallBlockObj *)pObj);
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_EXPRESS == pObj->m_nType )
	{
		CExpressObj * pNewObj =  new CExpressObj;
		*pNewObj = *(CExpressObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_FUNCTION == pObj->m_nType )
	{
		CFunctionObj * pNewObj =  new CFunctionObj;
		*pNewObj = *(CFunctionObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_GOTO == pObj->m_nType )
	{
		CGoToObj * pNewObj =  new CGoToObj;
		*pNewObj = *(CGoToObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_TRACE_INFO == pObj->m_nType )
	{
		CTraceObj * pNewObj =  new CTraceObj;
		*pNewObj = *(CTraceObj *)pObj;
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_MOUDLE == pObj->m_nType )
	{
		CNewModuleObj * pNewObj =  new CNewModuleObj( *(CModuleObj *)pObj );
		pObjList->AddTail(pNewObj);
	}
	else if( NODETYPE_CUSTOM == pObj->m_nType )
	{
		CCustomObj * pNewObj =  new CCustomObj;
		*pNewObj = *(CCustomObj *)pObj;
		pNewObj->MoveFirst();
		pObjList->AddTail(pNewObj);
	}
	else
	{
		CNodeObj * pNewObj = new CNodeObj;
		*pNewObj = *pObj;
		pObjList->AddTail(pNewObj);
	}

	return;

}

BOOL CNormalScriptInfo::InitNormalScript(  CString strFile)
{
	CCFDocument doc;
	if( ! doc.OnOpenDocument( strFile ) ) 
	{
		CString strError("");
		strError.Format("It is fail that open the file %s",strFile );
		AfxMessageBox( strError );
		return FALSE;
	}
	
	
	POSITION pos = doc.m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CNodeObj *pObj = doc.m_objects.GetNext(pos);
		if( NULL == pObj ) continue;
		InitNodeObjList( &(m_objects), pObj );

	}//end while
	

	pos = doc.m_RecordsetVariList.GetHeadPosition();
	while (pos != NULL)
	{
		CRecordsetVar *pRsObj = doc.m_RecordsetVariList.GetNext(pos);
		if( NULL != pRsObj )
		{
			
			CNewRecordsetVar *pNewObj = new CNewRecordsetVar(*pRsObj);
			m_RecordsetVariList.AddTail(pNewObj);
		}

	}

	pos = doc.m_DBConnVarList.GetHeadPosition();
	while (pos != NULL)
	{
		CDBConnVar *pDBObj = doc.m_DBConnVarList.GetNext(pos);
		if( NULL != pDBObj )
		{
			CNewDBConnVar *pNewObj = new CNewDBConnVar(*pDBObj);
			m_DBConnVarList.AddTail(pNewObj);

			
		}

	}
	pos = doc.m_NormalVarList.GetHeadPosition();
	while (pos != NULL)
	{
		CNormalVar *pNewObj = new CNormalVar;
		*pNewObj = *( doc.m_NormalVarList.GetNext(pos) );
		if( NULL != pNewObj )
			m_NormalVarList.AddTail(pNewObj);

	}
	pos = doc.m_DLLVariableList.GetHeadPosition();
	while (pos != NULL)
	{
		CDLLVariable *pNewObj = new CDLLVariable;
		*pNewObj = *( doc.m_DLLVariableList.GetNext(pos) );
		if( NULL != pNewObj )
			m_DLLVariableList.AddTail(pNewObj);

	}



  
	return TRUE;

}