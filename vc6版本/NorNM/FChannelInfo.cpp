// FChannelInfo.cpp: implementation of the CFChannelInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <afxmt.h>
#include "CIF5188.h"

#include "PublicCardObj.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern CPublicCardObj *g_pPubObj;




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFChannelInfo::CFChannelInfo()
{

	m_nChannel = -1;
	m_nChangeVoiceChnl = -1;
	m_nConfNO = -1;
	m_nConfAction = CIF_CONF_NULL;
	m_nLinkCh = -1;
	m_nLinkType = CF_CHSWITCH_DISCONNECT;
	m_nIndex = -1;
	m_nFaxChnl = -1;
	m_nTransferCh = -1;
	m_nHoldChnl = -1;
	m_nInvitConfChnl= -1;
	m_nState = CHANNEL_STATE_FREE;
	m_nStep = 0;
	m_lTimeElapse = 0;
	m_nRings = 0;
	m_nChanelType = 0;
	m_lCardKey = 0;
	m_nRecordLength = 0;
	m_nHandleConferChnl = -1;
	m_nHandleConferNo = -1;
	m_pCurrentNodeObj = NULL;
	m_strCallerID	= "";
	m_strCalledID	= "";
	m_strLocalDN	= "";

	
	m_strPlayFile = "";
	m_strState = "";
	m_strDTMFRecived = "";
	m_strScriptFile = "";
	m_strRecordFile = "";
	m_strReciveFaxFile = "";
	m_strSendFaxFile = "";

	m_strtBeginTime = "";
	m_strtEndTime = "";
	m_nEventType = EVENTTYPE_NOTHING;
	m_nCallType =  -1;
	m_pCurDBConn = NULL;
	m_pCurRecset = NULL;
	m_pCurStoreProObj = NULL;
	m_bExcuteSQL = FALSE;//执行SQL语句是否完成
	m_bRecord	 = FALSE;
	m_nLinkType = CF_CHSWITCH_CONNECT;
	
	m_nDialType = 0;//拨号类型 0:呼叫坐席
	m_strDialTel = "";//呼叫号码
	m_nDialChIndex = -1;
	m_nThreadCount = 0;

	m_nPickUpType = 0;//取回保持的类型
	m_strPickUpTel = "";//取回保持的主叫号码
	m_nActiveType = 0;//取回活动呼叫的类型
	m_strActiveTel = "";//取回活动呼叫的号码

	m_plistSystem = NULL;
	m_nDialChIndex = -1;
	m_nPlayGroup = -1;
	m_nRecordGroup = -1;
	m_bEventCheckPlay = FALSE;
	m_strRingTime = "";
	m_nHangUpType = 0;

	m_nLineInit = 0;
	m_bDBExcuteEnd = TRUE;

	m_nDtmfLen = -1;
	m_strDtmfEnd = "";//接收DTMF的结束按键
	m_nSaveDtmfKey = -1;
	m_nTimeOut = 30;


	m_bRecordLoop = FALSE;
	m_bPlayLoop = FALSE;

	m_nRecordType = 0;//录音类型：0普通 1会议
	m_nPlayType = 0;//放音类型：0普通 1会议

	m_Threadhandle = NULL;
	m_strInboundCallPK = "";
	m_strOutboundCallPK = "";

	m_nCallInType = 0;

	m_strAgentPwd = "";
	m_strAgentID = "";
	m_nGroup = "";
	m_strLoginTime = "";
	m_strLogOutTime = "";
	m_strReadyTime = "";
	m_strNotReadyTime = "";
	m_strAgentName = "";
	m_strGroupName = "";


}

CFChannelInfo::~CFChannelInfo()
{
	RemoveALLObjects();

}

//取得变量值
bool CFChannelInfo::GetNorVarValues(CString &strValues, int nVarKey)
{
	CNormalVarList *pVarList = NULL;
	if( NULL !=m_pCurrentNodeObj )
	{
		if( NODETYPE_MOUDLE == m_pCurrentNodeObj->m_nType )
		{
			CNewModuleObj *pModule = (CNewModuleObj *)m_pCurrentNodeObj;
			pVarList = &(pModule->m_NormalVarList);
		}
		else
		{
			pVarList = &(m_NormalVarList);
		}
	}
	else
	{
		pVarList = &(m_NormalVarList);
	}
	
	POSITION pos = pVarList->GetHeadPosition();
	while(pos)
	{
		CNormalVar * pVar = pVarList->GetNext(pos);
		if( pVar != NULL )
		{
			//if( g_pD08a->CompareVarKey( nVarKey,pVar->m_nKey) )
			if( nVarKey == pVar->m_nKey )
			{
				strValues = pVar->m_strVariable ;
				return true;
			}
		}
	}
	
	strValues = "";
	return false;
}

void CFChannelInfo::RemoveALLObjects()
{
	while(1)
	{
		if( NULL == m_Threadhandle ){
			break;
		}

		DWORD   dwExitCode = 0;
		GetExitCodeThread( m_Threadhandle, &dwExitCode );
		if( STILL_ACTIVE == dwExitCode )
		{
			Sleep(100);
			continue;
		}
		else
		{
			break;
		}
	}
	while ( m_objects.GetHeadPosition() )
	{
		delete m_objects.RemoveHead();
	}
	
	while ( m_NewRecordsetVariList.GetHeadPosition() )
	{
		delete m_NewRecordsetVariList.RemoveHead();
	}
	
	while ( m_NewDBConnVarList.GetHeadPosition() )
	{
		delete m_NewDBConnVarList.RemoveHead();
	}
	
	while ( m_NormalVarList.GetHeadPosition() )
	{
		delete m_NormalVarList.RemoveHead();
	}

	while ( m_DLLVariableList.GetHeadPosition() )
	{
		delete m_DLLVariableList.RemoveHead();
	}

	m_nThreadCount = 0;
	return;
}


BOOL CFChannelInfo::MoveNext_Node(CNextNode *pNextObj)
{
	if( NULL == pNextObj ) return FALSE;
	if( NULL == m_pCurrentNodeObj ) return FALSE;
	CNodeObj *pObj  = FindNodeObj( pNextObj->m_nNodeObjKey );
	if( NULL != pObj )
	{
		if( NODETYPE_MOUDLE == m_pCurrentNodeObj->m_nType )
		{
			CNewModuleObj *pModule = (CNewModuleObj *)m_pCurrentNodeObj;
			CNodeObj *pCurObj = pModule->m_pCurObj;
			if( NULL == pCurObj )
			{
				return FALSE;
			}
			else
			{

				if( NODETYPE_MOUDLE_END == pCurObj->m_nType )
				{
					m_pCurrentNodeObj = pObj;
				}
				else
				{
					pModule->m_pCurObj = pCurObj;
				}
				
			}
		}
		else
		{
			m_pCurrentNodeObj = pObj;
			if( NODETYPE_MOUDLE == m_pCurrentNodeObj->m_nType )
			{
				CNewModuleObj *pModule = (CNewModuleObj *)m_pCurrentNodeObj;
				pModule->m_pCurObj = pModule->m_objects.GetHead();
				CParameterList *pParamList = &(pModule->m_ParameterLst);
				POSITION pos = pParamList->GetHeadPosition();
				while(pos)
				{
					CParameter * pParam = pParamList->GetNext(pos);
					if( pParam != NULL )
					{
						int nKey = pModule->GetVarKey( pParam->m_strVariable );
						if( nKey >= 100 && nKey < 4000 )
						{
							InPutVarValuesToParam( pParam, nKey );
						}
					}
				}

				
			}

		}
		m_nStep = 0;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//Start Node 结点向下找到指向的下一个结点
BOOL CFChannelInfo::MoveNext_Node( int nNextType )
{
	CNextNode *pNextObj = NULL;
	if( NULL == m_pCurrentNodeObj ) return FALSE;
	if( NODETYPE_MOUDLE == m_pCurrentNodeObj->m_nType )
	{
		CNewModuleObj *pModule = (CNewModuleObj *)m_pCurrentNodeObj;
		CNodeObj *pCurObj = pModule->m_pCurObj;
		if( NULL == pCurObj )
		{
			return FALSE;
		}
		else
		{
			if( NODETYPE_MOUDLE_END == pCurObj->m_nType )
			{
				pNextObj = pModule->FindNextNodeObj( nNextType );
			}
			else
			{
				pNextObj = pCurObj->FindNextNodeObj( nNextType );
			}
			
			if( NULL == pNextObj ) return FALSE;
			CNodeObj *pObj  = FindNodeObj( pNextObj->m_nNodeObjKey );
			if( NULL != pObj )
			{
				if( NODETYPE_MOUDLE_END == pCurObj->m_nType )
				{
					m_pCurrentNodeObj = pObj;
				}
				else
				{
					pModule->m_pCurObj = pObj;
				}
				m_nStep = 0;
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
	}
	else
	{
		pNextObj = m_pCurrentNodeObj->FindNextNodeObj( nNextType );
		if( NULL == pNextObj ) return FALSE;
		CNodeObj *pObj  = FindNodeObj( pNextObj->m_nNodeObjKey );
		if( NULL != pObj )
		{
			m_pCurrentNodeObj = pObj;
			if( NODETYPE_MOUDLE == m_pCurrentNodeObj->m_nType )
			{
				CNewModuleObj *pModule = (CNewModuleObj *)m_pCurrentNodeObj;
				pModule->m_pCurObj = pModule->m_objects.GetHead();
				CParameterList *pParamList = &(pModule->m_ParameterLst);
				POSITION pos = pParamList->GetHeadPosition();
				while(pos)
				{
					CParameter * pParam = pParamList->GetNext(pos);
					if( pParam != NULL )
					{
						int nKey = pModule->GetVarKey( pParam->m_strVariable );
						if( nKey >= 100 && nKey < 4000 )
						{
							InPutVarValuesToParam( pParam, nKey );
						}
					}
				}


			}
			m_nStep = 0;
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}


}

CNodeObj * CFChannelInfo::FindNodeObj(int nKey)
{
	if( NULL == m_pCurrentNodeObj ) return NULL;
	CNodeObjList *pObjLst = NULL;
	if( NODETYPE_MOUDLE == m_pCurrentNodeObj->m_nType )
	{
		CNewModuleObj *pModule = (CNewModuleObj *)m_pCurrentNodeObj;
		CNodeObj *pCurObj = pModule->m_pCurObj;
		if( NULL == pCurObj )
		{
			return NULL;
		}
		else
		{
			if( NODETYPE_MOUDLE_END == pCurObj->m_nType )
			{
				pObjLst = &(m_objects);
			}
			else
			{
				pObjLst = &(pModule->m_objects);
			}
		}
	}
	else
	{
		pObjLst = &(m_objects);
	}

	POSITION pos = pObjLst->GetTailPosition();
	while (pos)
	{
		CNodeObj* pObj = pObjLst->GetPrev(pos);
		if( nKey == pObj->m_nKey  )
		{
			return pObj;
		}
		else
		{
			if(  NODETYPE_BLOCK == pObj->m_nType )
			{
				CNewBlockObj *pObj1 = (CNewBlockObj*)pObj;
				CNodeObj* pObj2 = pObj1->FindObj(nKey);
				if( NULL != pObj2 )
					return pObj2;
			}
			else if(  NODETYPE_ENDCALL_BLOCK == pObj->m_nType )
			{
				CNewEndCallBlockObj *pObj1 = (CNewEndCallBlockObj*)pObj;
				CNodeObj* pObj2 = pObj1->FindObj(nKey);
				if( NULL != pObj2 )
					return pObj2;
			}
		}
	}

	return NULL;

}



bool CFChannelInfo::SaveToNorVar(CString strValues, int nVarKey)
{
	if( NULL == m_pCurrentNodeObj ) return false;
	CNormalVarList *pVarList = NULL;
	if( NODETYPE_MOUDLE == m_pCurrentNodeObj->m_nType )
	{
		CNewModuleObj *pModule = (CNewModuleObj *)m_pCurrentNodeObj;
		pVarList = &(pModule->m_NormalVarList);
	}
	else
	{
		pVarList = &(m_NormalVarList);
	}

	POSITION pos = pVarList->GetHeadPosition();
	while(pos)
	{
		CNormalVar * pVar = pVarList->GetNext(pos);
		if( pVar != NULL )
		{
			//if( g_pD08a->CompareVarKey( nVarKey,pVar->m_nKey) )
			if( nVarKey == pVar->m_nKey )
			{
				pVar->m_strVariable = strValues;
				return true;
			}
		}
	}

	return false;
}

void CFChannelInfo::MoveFirst_Node()
{
	m_nChangeVoiceChnl	= -1;
	m_nConfNO			= -1;
	m_nConfAction		= CIF_CONF_NULL;
	m_nLinkCh		= -1;
	m_nLinkType = CF_CHSWITCH_DISCONNECT;
	m_nFaxChnl		= -1;
	m_nHoldChnl		= -1;
	m_nInvitConfChnl= -1;
	m_nState		= CHANNEL_STATE_FREE;
	m_nStep			= 0;
	m_lTimeElapse	= 0;
	m_nRings		= 0;
	m_nRecordLength = 0;
	m_nPlayGroup = -1;
	m_nRecordGroup = -1;
	m_nHandleConferChnl = -1;

	m_strPlayFile		= "";
	m_strDTMFRecived	= "";
	m_strRecordFile		= "";
	m_strReciveFaxFile	= "";
	m_strSendFaxFile	= "";

	m_strtBeginTime		= "";
	m_strtEndTime		= "";
	m_nEventType		= EVENTTYPE_NOTHING;
	m_nCallType			= -1;
	m_nLinkType			= CF_CHSWITCH_CONNECT;
	
	m_nDialType			= 0;//拨号类型 0:呼叫坐席
	m_strDialTel		= "";//呼叫号码
	m_nDialChIndex		= -1;
	m_strRingTime       = "";
	m_nHangUpType		= 0;

	m_nPickUpType = 0;//取回保持的类型
	m_strPickUpTel = "";//取回保持的主叫号码
	m_nActiveType = 0;//取回活动呼叫的类型
	m_strActiveTel = "";//取回活动呼叫的号码

	InitNormalVar();//初始化通道变量
	m_pCurrentNodeObj = m_objects.GetHead();


	m_bRecordLoop = FALSE;
	m_bPlayLoop = FALSE;

	m_nRecordType = 0;//录音类型：0普通 1会议
	m_nPlayType = 0;//放音类型：0普通 1会议

	m_strInboundCallPK = "";
	m_strOutboundCallPK = "";
	m_nCallInType = 0;
	return;
}

void CFChannelInfo::MoveCallEndBlock_Node()
{
	m_nState = CHANNEL_STATE_FREE;//在进入CallEndBlock节点前需要将通道状态设置为自由状态
	m_strtEndTime = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
	POSITION pos  = m_objects.GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj = m_objects.GetNext(pos);
		if( NULL != pObj )
		{
			if( NODETYPE_ENDCALL_BLOCK == pObj->m_nType )
			{
				CNewEndCallBlockObj *pEndCallObj = (CNewEndCallBlockObj*)pObj;
				m_pCurrentNodeObj = pEndCallObj->GetEndCallEventCheckObj();
				return;
				
			}

		}

	}

	return;
}

CNewDBConnVar * CFChannelInfo::FindDBConnectVar( int nKey)
{
	CNewDBConnVarList *pConnLst = NULL;
	if( NULL !=m_pCurrentNodeObj )
	{
		if( NODETYPE_MOUDLE == m_pCurrentNodeObj->m_nType )
		{
			CNewModuleObj *pModule = (CNewModuleObj *)m_pCurrentNodeObj;
			pConnLst = &(pModule->m_DBConnVarList);
		}
		else
		{
			pConnLst = &(m_NewDBConnVarList);
		}
	}
	else
	{
		pConnLst = &(m_NewDBConnVarList);
	}

	
	POSITION pos = pConnLst->GetHeadPosition();
	while(pos)
	{
		CNewDBConnVar * pVar = pConnLst->GetNext(pos);
		if( pVar != NULL )
		{
			if( nKey == pVar->m_nKey )
			{
				return pVar ;
			}
		}
	}

	return NULL;
}


CNewRecordsetVar * CFChannelInfo::FindRecordsetVar(int nKey)
{
	CNewRecordsetVarList *pRecLst = NULL;
	if( NULL !=m_pCurrentNodeObj )
	{
		if( NODETYPE_MOUDLE == m_pCurrentNodeObj->m_nType )
		{
			CNewModuleObj *pModule = (CNewModuleObj *)m_pCurrentNodeObj;
			pRecLst = &(pModule->m_RecordsetVariList);
		}
		else
		{
			pRecLst = &(m_NewRecordsetVariList);
		}
	}
	else
	{
		pRecLst = &(m_NewRecordsetVariList);
	}


	POSITION pos = pRecLst->GetHeadPosition();
	while(pos)
	{
		CNewRecordsetVar * pVar = pRecLst->GetNext(pos);
		if( pVar != NULL )
		{
			if( nKey == pVar->m_nKey )
			{
				return pVar;
			}
		}
	}

	return NULL;
}

void CFChannelInfo::ExcuteSQL()
{
	
	//AfxBeginThread( ExcuteSQL_Progress, this, NULL );
}



BOOL CFChannelInfo::GetSQLExpressValues(CString &strNewExpress, CString strOldExpress )
{
	int nPos1	= -1;
	int nPos2	= -1;
	int nPos3	= -1;
	int nVarKey = -1;
	int nVarType = -1;
	BOOL bResult = TRUE;
	CString strVar("");
	CString strVarValues("");
	
	strNewExpress = "";
	while(1)
	{
		nPos1 = strOldExpress.Find( '|', nPos3+1 );
		if( -1 == nPos1 )
		{
			//取得后面的常量串
			strVarValues = strOldExpress.Right( strlen(strOldExpress) - nPos3-1 );
			strNewExpress = strNewExpress + strVarValues;
			break;
		}
		strNewExpress =  strNewExpress + strOldExpress.Mid( nPos3+1, nPos1 - nPos3-1 );
		nPos2 = strOldExpress.Find( ':', nPos1+1 );
		nPos3 = strOldExpress.Find( '|', nPos1+1 );
		strVar = strOldExpress.Mid(nPos1+2,nPos2-nPos1-2);
		nVarKey = atoi(strVar);
		if( 0 == nVarKey ) 
		{
			bResult = FALSE;
			break;
		}
		if( !g_pPubObj->PC_GetNorVarValues( this,strVarValues, nVarType, nVarKey ) )
		{
			//没有找到该变量
			bResult = FALSE;
			break;
		}

		if( DATATYPE_INTEGER == nVarType )//interger
		{
			strNewExpress = strNewExpress + strVarValues;
		}
		else if( DATATYPE_FLOAT == nVarType )//Double
		{
			strNewExpress = strNewExpress + strVarValues;
		}
		else//其他类型,都做字符串来处理
		{
			strNewExpress = strNewExpress + "'"+ strVarValues + "'";
		}

	}//end while

	if( FALSE == bResult && "" == strNewExpress ) 
	{
		//里面没有变量
		strNewExpress = strOldExpress;
	}


	return bResult;
}

BOOL CFChannelInfo::GetNorVarValues(CString &strValues, int &nVarType, int nVarKey)
{

	CNormalVarList *pVarList = NULL;
	if( NULL !=m_pCurrentNodeObj )
	{
		if( NODETYPE_MOUDLE == m_pCurrentNodeObj->m_nType )
		{
			CNewModuleObj *pModule = (CNewModuleObj *)m_pCurrentNodeObj;
			pVarList = &(pModule->m_NormalVarList);
		}
		else
		{
			pVarList = &(m_NormalVarList);
		}
	}
	else
	{
		pVarList = &(m_NormalVarList);
	}

	
	POSITION pos = pVarList->GetHeadPosition();
	while(pos)
	{
		CNormalVar * pVar = pVarList->GetNext(pos);
		if( pVar != NULL )
		{
			//if( g_pD08a->CompareVarKey( nVarKey,pVar->m_nKey) )
			if( nVarKey == pVar->m_nKey )
			{
				strValues = pVar->m_strVariable ;
				nVarType = pVar->m_nVarDataType;
				return true;
			}
		}
	}
	
	nVarType = -1;
	strValues = "";
	return false;
}

BOOL CFChannelInfo::GetContantValues(CString &strValues, int nVarKey)
{
	if( 1 == nVarKey )
	{
		strValues = (CTime::GetCurrentTime()).Format("%H:%M:%S");
		return TRUE;
	}
	else if( 2 == nVarKey )
	{
		strValues = (CTime::GetCurrentTime()).Format("%Y-%m-%d");
		return TRUE;
	}
	else if( 3 == nVarKey )
	{
		strValues = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
		return TRUE;
	}
	else if( 4 == nVarKey )
	{
		strValues.Format("%d", m_nChannel );
		return TRUE;
	}
	else if( 5 == nVarKey )
	{
		strValues.Format("%d", this->m_nChanelType );
		return TRUE;
	}
	else if( 6 == nVarKey )
	{
		strValues = m_strScriptFile;
		return TRUE;
	}
	else if( 7 == nVarKey )
	{
		strValues = m_strDTMFRecived;
		return TRUE;
	}
	else if( 8 == nVarKey )
	{
		strValues.Format("%d", m_nConfNO );
		return TRUE;
	}
	else if( 9 == nVarKey )
	{
		strValues.Format("%d", this->m_nLinkCh );
		return TRUE;
	}
	else if( 11 == nVarKey )
	{
		strValues = m_strtBeginTime;
		return TRUE;
	}
	else if( 12 == nVarKey )
	{
		strValues = this->m_strtEndTime;
		return TRUE;
	}
	else if( 13 == nVarKey )
	{
		strValues = this->m_strCallerID;
		return TRUE;
	}
	else if( 14 == nVarKey )
	{
		strValues = this->m_strCalledID;
		return TRUE;
	}
	else if( 15 == nVarKey )
	{
		strValues = (CTime::GetCurrentTime()).Format("%H%M%S");
		return TRUE;
	}
	else if( 16 == nVarKey )
	{
		strValues = (CTime::GetCurrentTime()).Format("%Y%m%d");
		return TRUE;
	}
	else if( 17 == nVarKey )
	{
		strValues = (CTime::GetCurrentTime()).Format("%Y%m%d%H%M%S");
		return TRUE;
	}
	else if( 18 == nVarKey )
	{
		strValues.Format("%d",  m_nHoldChnl );
		return TRUE;
	}
	else if( 19 == nVarKey )
	{
		strValues.Format("%d",  m_nDialType );
		return TRUE;
	}
	else if( 20 == nVarKey )
	{
		strValues = m_strDialTel;
		return TRUE;
	}
	else if( 21 == nVarKey )
	{
		strValues = m_strRecordFile;
		return TRUE;
	}
	else if( 22 == nVarKey )
	{
		strValues = m_strReciveFaxFile;
		return TRUE;
	}
	else if( 23 == nVarKey )
	{
		strValues = m_strSendFaxFile;
		return TRUE;
	}
	else if( 24 == nVarKey )
	{
		strValues.Format("%d", m_nDialChIndex );
		return TRUE;
	}
	else if( 25 == nVarKey )
	{
		strValues = m_strLocalDN;
		return TRUE;
	}
	else if( 26 == nVarKey )
	{
		strValues = m_strRingTime;
		return TRUE;
	}
	else if( 27 == nVarKey )
	{
		strValues.Format("%d", m_nHangUpType);
		return TRUE;
	}
	else if( 28 == nVarKey )
	{
		strValues.Format("%d", m_nPickUpType);
		return TRUE;
	}
	else if( 29 == nVarKey )
	{
		strValues =  m_strPickUpTel;
		return TRUE;
	}
	else if( 30 == nVarKey )
	{
		strValues.Format("%d", m_nActiveType);
		return TRUE;
	}
	else if( 31 == nVarKey )
	{
		strValues = m_strActiveTel;
		return TRUE;
	}
	else if( 32 == nVarKey )
	{
		strValues = m_strInboundCallPK;
		return TRUE;
	}
	else if( 33 == nVarKey )
	{
		strValues.Format("%d", m_nCallInType);
		return TRUE;
	}
	else if( 34 == nVarKey )
	{
		strValues = m_strOutboundCallPK;
		return TRUE;
	}
	else if( 35 == nVarKey )
	{
		strValues.Format("%d", m_nCallType);
		return TRUE;
	}
	else if( 36 == nVarKey )
	{
		strValues = m_strAgentPwd;
		return TRUE;
	}
	else if( 37 == nVarKey )
	{
		strValues = m_strAgentID;
		return TRUE;
	}
	else if( 38 == nVarKey )
	{
		strValues.Format("%d", m_nGroup);
		return TRUE;
	}
	else if( 39 == nVarKey )
	{
		strValues = m_strLoginTime;
		return TRUE;
	}
	else if( 40 == nVarKey )
	{
		strValues = m_strLogOutTime;
		return TRUE;
	}
	else if( 41 == nVarKey )
	{
		strValues = m_strReadyTime;
		return TRUE;
	}
	else if( 42 == nVarKey )
	{
		strValues = m_strNotReadyTime;
		return TRUE;
	}
	else if( 43 == nVarKey )
	{
		strValues = m_strAgentName;
		return TRUE;
	}
	else if( 44 == nVarKey )
	{
		strValues = m_strGroupName;
		return TRUE;
	}

	else
		return FALSE;
	
}



void CFChannelInfo::ShowSysMsg(CString strMSG)
{
	if (m_plistSystem == NULL || !m_plistSystem->IsKindOf( RUNTIME_CLASS(CListBox) ) )
		return;
	if( !IsWindow( m_plistSystem->GetSafeHwnd() ) ) return;
	strMSG = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S") + "  " + strMSG;
	int n_line = m_plistSystem->GetCount();

	m_plistSystem->InsertString(n_line, strMSG);
	m_plistSystem->SetCurSel(n_line);
	g_pPubObj->PC_WriteSysLog( strMSG );
	return;
}

CDLLVariable * CFChannelInfo::FindDLLVariable(int nKey)
{
	CFDLLVariableList *pDllLst = NULL;
	if( NULL !=m_pCurrentNodeObj )
	{
		if( NODETYPE_MOUDLE == m_pCurrentNodeObj->m_nType )
		{
			CNewModuleObj *pModule = (CNewModuleObj *)m_pCurrentNodeObj;
			pDllLst = &(pModule->m_DLLVariableList);
		}
		else
		{
			pDllLst = &(m_DLLVariableList);
		}
	}
	else
	{
		pDllLst = &(m_DLLVariableList);
	}

	
	POSITION pos = pDllLst->GetHeadPosition();
	while(pos)
	{
		CDLLVariable * pVar = pDllLst->GetNext(pos);
		if( pVar != NULL )
		{
			if( nKey == pVar->m_nKey )
			{
				return pVar ;
			}
		}
	}

	return NULL;
}

void CFChannelInfo::ExcuteStoredPro()
{
	//AfxBeginThread( ExcuteStorePro_Progress, this, NULL );
}

BOOL CFChannelInfo::GetConstAndVarValues(CString &strValues, int nVarKey)
{
	if( nVarKey < 0 ) return FALSE;
	if( nVarKey <= 50 )
	{
		//常量
		return GetContantValues( strValues, nVarKey );
	}
	else if( nVarKey > 50 && nVarKey < 100 )
	{
		//系统变量
		return FALSE;

	}
	else
	{
		//普通变量
		return GetNorVarValues( strValues, nVarKey );
	}
}



BOOL CFChannelInfo::GetContantValues(CString &strValues, int &nVarType, int nVarKey)
{
	if( 1 == nVarKey )
	{
		strValues = (CTime::GetCurrentTime()).Format("%H:%M:%S");
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 2 == nVarKey )
	{
		strValues = (CTime::GetCurrentTime()).Format("%Y-%m-%d");
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 3 == nVarKey )
	{
		strValues = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 4 == nVarKey )
	{
		strValues.Format("%d", m_nChannel );
		nVarType = DATATYPE_INTEGER;
		return TRUE;
	}
	else if( 5 == nVarKey )
	{
		strValues.Format("%d", this->m_nChanelType );
		nVarType = DATATYPE_INTEGER;
		return TRUE;
	}
	else if( 6 == nVarKey )
	{
		strValues = m_strScriptFile;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 7 == nVarKey )
	{
		strValues = m_strDTMFRecived;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 8 == nVarKey )
	{
		strValues.Format("%d", m_nConfNO );
		nVarType = DATATYPE_INTEGER;
		return TRUE;
	}
	else if( 9 == nVarKey )
	{
		strValues.Format("%d", this->m_nLinkCh );
		nVarType = DATATYPE_INTEGER;
		return TRUE;
	}
	else if( 11 == nVarKey )
	{
		strValues = m_strtBeginTime;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 12 == nVarKey )
	{
		strValues = this->m_strtEndTime;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 13 == nVarKey )
	{
		strValues = this->m_strCallerID;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 14 == nVarKey )
	{
		strValues = this->m_strCalledID;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 15 == nVarKey )
	{
		strValues = (CTime::GetCurrentTime()).Format("%H%M%S");
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 16 == nVarKey )
	{
		strValues = (CTime::GetCurrentTime()).Format("%Y%m%d");
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 17 == nVarKey )
	{
		strValues = (CTime::GetCurrentTime()).Format("%Y%m%d%H%M%S");
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 18 == nVarKey )
	{
		strValues.Format("%d",  m_nHoldChnl );
		nVarType = DATATYPE_INTEGER;
		return TRUE;
	}
	else if( 19 == nVarKey )
	{
		strValues.Format("%d",  m_nDialType );
		nVarType = DATATYPE_INTEGER;
		return TRUE;
	}
	else if( 20 == nVarKey )
	{
		strValues = m_strDialTel;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 21 == nVarKey )
	{
		strValues = m_strRecordFile;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 22 == nVarKey )
	{
		strValues = m_strReciveFaxFile;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 23 == nVarKey )
	{
		strValues = m_strSendFaxFile;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 24 == nVarKey )
	{
		strValues.Format("%d", m_nDialChIndex );
		nVarType = DATATYPE_INTEGER;
		return TRUE;
	}
	else if( 25 == nVarKey )
	{
		strValues = m_strLocalDN;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 26 == nVarKey )
	{
		strValues = m_strRingTime;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 27 == nVarKey )
	{
		strValues.Format("%d", m_nHangUpType );
		nVarType = DATATYPE_INTEGER;
		return TRUE;
	}
	else if( 28 == nVarKey )
	{
		strValues.Format("%d", m_nPickUpType);
		nVarType = DATATYPE_INTEGER;
		return TRUE;
	}
	else if( 29 == nVarKey )
	{
		strValues =  m_strPickUpTel;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 30 == nVarKey )
	{
		strValues.Format("%d", m_nActiveType);
		nVarType = DATATYPE_INTEGER;
		return TRUE;
	}
	else if( 31 == nVarKey )
	{
		strValues = m_strActiveTel;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 32 == nVarKey )
	{
		strValues = m_strInboundCallPK;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 33 == nVarKey )
	{
		strValues.Format("%d", m_nCallInType);
		nVarType = DATATYPE_INTEGER;
		return TRUE;
	}
	else if( 34 == nVarKey )
	{
		strValues = m_strOutboundCallPK;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 35 == nVarKey )
	{
		strValues.Format("%d", m_nCallType);
		nVarType = DATATYPE_INTEGER;
		return TRUE;
	}
	else if( 36 == nVarKey )
	{
		strValues = m_strAgentPwd;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 37 == nVarKey )
	{
		strValues = m_strAgentID;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 38 == nVarKey )
	{
		strValues.Format("%d", m_nGroup);
		nVarType = DATATYPE_INTEGER;
		return TRUE;
	}
	else if( 39 == nVarKey )
	{
		strValues = m_strLoginTime;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 40 == nVarKey )
	{
		strValues = m_strLogOutTime;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 41 == nVarKey )
	{
		strValues = m_strReadyTime;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 42 == nVarKey )
	{
		strValues = m_strNotReadyTime;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 43 == nVarKey )
	{
		strValues = m_strAgentName;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}
	else if( 44 == nVarKey )
	{
		strValues = m_strGroupName;
		nVarType = DATATYPE_STRING;
		return TRUE;
	}

	else
	{
		strValues = "";
		nVarType	= -1;
		return FALSE;
	}
}


//将通道级变量初始话
void CFChannelInfo::InitNormalVar()
{
	CNormalVarList *pVarList = &(m_NormalVarList);
	POSITION pos = pVarList->GetHeadPosition();
	while(pos)
	{
		CNormalVar * pVar = pVarList->GetNext(pos);
		if( pVar != NULL )
		{
			if( DATATYPE_STRING == pVar->m_nVarDataType )
				pVar->m_strVariable = "";
			else
				pVar->m_strVariable = "0";
		}
	}

	return;
}



void CFChannelInfo::PrintADOVersion( _ConnectionPtr pConnection )
{
	if( NULL == pConnection ) return;
	CString strTemp("");
	strTemp.Format("ADO Version   : %s",(LPCSTR) pConnection->Version);
	ShowSysMsg(strTemp);
	strTemp.Format("DBMS Name   : %s\n\n",(LPCSTR) (_bstr_t) 
		pConnection->Properties->GetItem("DBMS Name")->Value);
	ShowSysMsg(strTemp);
	strTemp.Format("DBMS Version   : %s\n\n",(LPCSTR) (_bstr_t)
		pConnection->Properties->GetItem("DBMS Version")->Value);
	ShowSysMsg(strTemp);
	strTemp.Format("OLE DB Version   : %s\n\n",(LPCSTR) (_bstr_t) 
		pConnection->Properties->GetItem("OLE DB Version")->Value);
	ShowSysMsg(strTemp);
	strTemp.Format("Provider Name   : %s\n\n",(LPCSTR) (_bstr_t) 
		pConnection->Properties->GetItem("Provider Name")->Value);
	ShowSysMsg(strTemp);
	strTemp.Format("Provider Version   : %s\n\n",(LPCSTR) (_bstr_t) 
		pConnection->Properties->GetItem("Provider Version")->Value);
	ShowSysMsg(strTemp);
	strTemp.Format("Driver Name   : %s\n\n",(LPCSTR) (_bstr_t) 
		pConnection->Properties->GetItem("Driver Name")->Value);
	ShowSysMsg(strTemp);
	strTemp.Format("Driver Version   : %s\n\n",(LPCSTR) (_bstr_t) 
		pConnection->Properties->GetItem("Driver Version")->Value);
	ShowSysMsg(strTemp);
	strTemp.Format("Driver ODBC Version   : %s\n\n",(LPCSTR) (_bstr_t) 
		pConnection->Properties->GetItem("Driver ODBC Version")->Value);
	ShowSysMsg(strTemp);

   return;
}

void CFChannelInfo::PrintComError(_com_error &e)
{
	CString ErrStr="";
   _bstr_t bstrSource(e.Source());
   _bstr_t bstrDescription(e.Description());
   ErrStr.Format("[Code = %08lx][Code meaning = %s][Source = %s][Description = %s]",
	   e.Error(),e.ErrorMessage(),(LPCSTR) bstrSource,(LPCSTR) bstrDescription );
   ShowSysMsg(ErrStr);
   return;
}
void CFChannelInfo::PrintProviderError( _ConnectionPtr pConnection)
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
		 ShowSysMsg(strTemp);
      }
   }
   return;
}

void CFChannelInfo::PrintADOConnectString(_ConnectionPtr pConnection)
{
	if( NULL == pConnection ) return;
	CString strState("");
	ShowSysMsg( (LPCSTR)pConnection->ConnectionString );

	int nState = pConnection->State;
   switch( nState ) {
   case adStateClosed://默认，指示对象是关闭的
      strState = "ADO State:adStateClosed";
      break;
   case adStateOpen://指示对象是打开的
      strState = "ADO State:adStateOpen";
      break;
   case adStateConnecting://指示 Recordset 对象正在连接
      strState = "ADO State:adStateConnecting";
      break;
   case adStateExecuting://指示 Recordset 对象正在执行命令。 
      strState = "ADO State:adStateExecuting";
      break;
   case adStateFetching://指示 Recordset 对象的行正在被读取
      strState = "ADO State:adStateFetching";
      break;
   default:
	  strState = "ADO State:NoKnow";
      break;
   }


	ShowSysMsg( strState );
	return;
}

CNodeObj * CFChannelInfo::GetCurrentNodeObj()
{
	if( NODETYPE_MOUDLE == m_pCurrentNodeObj->m_nType )
	{
		CNewModuleObj *pModule = (CNewModuleObj *)m_pCurrentNodeObj;
		return pModule->m_pCurObj;
	}
	else
	{
		return m_pCurrentNodeObj;
	}
}

void CFChannelInfo::SetCurrentNodeObj(CNodeObj *pObj)
{
	if( g_pD08a->CompareVarKey(NODETYPE_MOUDLE, m_pCurrentNodeObj->m_nType) )
	//if( NODETYPE_MOUDLE == m_pCurrentNodeObj->m_nType )
	{
		CNewModuleObj *pModule = (CNewModuleObj *)m_pCurrentNodeObj;
		pModule->m_pCurObj = pObj;
	}
	else
	{
		m_pCurrentNodeObj = pObj;
	}
}

//////////////////////////////////////////////////////////////////////
// CDBParameters Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDBParameters::CDBParameters()
{
	m_pRecordset = NULL;
	m_pRecordset.CreateInstance("ADODB.Recordset");
	m_pCommand	= NULL;
	m_pCommand.CreateInstance("ADODB.Command");
	m_pConnection = NULL;
	m_pConnection.CreateInstance("ADODB.Connection");
	m_pConnection->ConnectionTimeout = 5;
	strSQL = "";
	strError = "";
}

CDBParameters::~CDBParameters()
{

}
CDBParameters::CDBParameters( const CDBParameters &s )
{
	m_pRecordset = s.m_pRecordset;
	m_pCommand = s.m_pCommand;
	m_pConnection = s.m_pConnection;
	strSQL = s.strSQL;

}

CDBParameters & CDBParameters::operator =( const CDBParameters &s )
{
	if(this == &s)
		return *this;

	m_pRecordset = s.m_pRecordset;
	m_pCommand = s.m_pCommand;
	m_pConnection = s.m_pConnection;
	strSQL = s.strSQL;

	return *this;
}

BOOL CFChannelInfo::GetModuleParamValues(CString &strValues, int nVarKey)
{
	if( NULL == m_pCurrentNodeObj ) return FALSE;
	CParameterList *pParamList = NULL;
	if( NODETYPE_MOUDLE == m_pCurrentNodeObj->m_nType )
	{
		CNewModuleObj *pModule = (CNewModuleObj *)m_pCurrentNodeObj;
		pParamList = &(pModule->m_ParameterLst);
	}
	else
	{
		return FALSE;//当此时不是在运行模块脚本时，访问参数变量认为错误
	}


	
	POSITION pos = pParamList->GetHeadPosition();
	while(pos)
	{
		CParameter * pParam = pParamList->GetNext(pos);
		if( pParam != NULL )
		{
			
			if( nVarKey == pParam->m_nKey )
			{
				strValues = pParam->m_strValues; ;
				return TRUE;
			}
		}
	}
	
	strValues = "";
	return FALSE;
}

BOOL CFChannelInfo::SaveToModuleParam(CString strValues, int nVarKey)
{
	if( NULL == m_pCurrentNodeObj ) return FALSE;
	CParameterList *pParamList = NULL;
	if( NODETYPE_MOUDLE == m_pCurrentNodeObj->m_nType )
	{
		CNewModuleObj *pModule = (CNewModuleObj *)m_pCurrentNodeObj;
		pParamList = &(pModule->m_ParameterLst);
	}
	else
	{
		return FALSE;//当此时不是在运行模块脚本时，访问参数变量认为错误
	}

	POSITION pos = pParamList->GetHeadPosition();
	while(pos)
	{
		CParameter * pParam = pParamList->GetNext(pos);
		if( pParam != NULL )
		{
			//if( g_pD08a->CompareVarKey( nVarKey,pVar->m_nKey) )
			if( nVarKey == pParam->m_nKey )
			{
				pParam->m_strValues = strValues;
				int nKey = m_pCurrentNodeObj->GetVarKey( pParam->m_strVariable );
				if( nKey >= 100 && nKey < 4000 )
				{
					OutParamValuesToVariable( strValues, nKey );//改变参数值的时候，同时也改变对应变量的值
				}
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CFChannelInfo::OutParamValuesToVariable(CString strValues, int nVarKey)
{
	CNormalVarList *pVarList = &(m_NormalVarList);
	POSITION pos = pVarList->GetHeadPosition();
	while(pos)
	{
		CNormalVar * pVar = pVarList->GetNext(pos);
		if( pVar != NULL )
		{
			//if( g_pD08a->CompareVarKey( nVarKey,pVar->m_nKey) )
			if( nVarKey == pVar->m_nKey )
			{
				pVar->m_strVariable = strValues;
				return true;
			}
		}
	}

	return false;
}

BOOL CFChannelInfo::InPutVarValuesToParam( CParameter * pParam, int nVarKey)
{
	CNormalVarList *pVarList = &(m_NormalVarList);
	POSITION pos = pVarList->GetHeadPosition();
	while(pos)
	{
		CNormalVar * pVar = pVarList->GetNext(pos);
		if( pVar != NULL )
		{
			//if( g_pD08a->CompareVarKey( nVarKey,pVar->m_nKey) )
			if( nVarKey == pVar->m_nKey )
			{
				pParam->m_strValues =  pVar->m_strVariable;
				return TRUE;
			}
		}
	}

	return FALSE;
}
