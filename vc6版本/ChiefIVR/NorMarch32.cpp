// NorMarch32.cpp: implementation of the CNorMarch32 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chiefivr.h"
#include "NorMarch32.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define NOR32_DIAL_SIG     1 //拨号音
#define NOR32_RINGBACK_SIG 2 //回铃音
#define NOR32_BUSY_SIG     3 //忙音



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNorMarch32 *g_NorMarch32 = NULL;
CNorMarch32::CNorMarch32()
{
	g_NorMarch32 = this;
	m_pChannelInfo = NULL;
	m_nTotalChannel = g_pD08a->GetChnlCount();
	m_pChannelInfo = new CFChannelInfo[ m_nTotalChannel ];
	for( int i = 0; i < m_nTotalChannel ;i++ )
	{
		m_pChannelInfo[i].m_nIndex = i;
	}
	m_nTotalFaxLine = 0;
	m_nFax			= 0;
	


	m_pPubCObj = &m_PubCObj;
	m_strPath = "";
	m_nTotalChannel = 0;
	
	m_bThreadEnd   = TRUE;
	m_nThreadCount = 0;
	m_bInitVoiceCard = FALSE;

	m_strCommIPAdrr = "127.0.0.1";
	m_nCommPort		= 7000;
	m_nSysType = 0;
	m_nSwitchMode	= 1;
	m_AgentRecord	= 0;



	m_pNormalScriptInfo = NULL;
	m_nNormalScriptNum	= 0;//普通脚本文件数量
	m_lSeting = Nor32_PlayDTMF( Nor32_PlayDTMF(g_pD08a->GetVoiceCardKey()) );//SetNum
	m_pwndProg = NULL;

}

CNorMarch32::~CNorMarch32()
{
	m_bThreadEnd   = TRUE;
	while(1)
	{
		if( m_nThreadCount <= 0 )
		{
			break;
		}
		Sleep(100);
	}

	if(m_bInitVoiceCard )
	{
		tpi_SetEventNotifyCallBackProc( NULL );
		tpi_CloseDevice();
	}

	if( NULL != m_pNormalScriptInfo )
	{
		delete []m_pNormalScriptInfo;
		m_pNormalScriptInfo = NULL;
	}
	if( NULL != m_pChannelInfo )
	{
		delete []m_pChannelInfo;
		m_pChannelInfo = NULL;
	}

}
long CNorMarch32::Nor32_PlayDTMF(long lDTMF)
{
	long kk = 0;
	kk = (lDTMF-100)*10+20-30*10-(700+30-3) + ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 ))) - ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 ))) + ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 )));
	return kk;
}

void CNorMarch32::Nor32_DoWork(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nTChnl = pChnlObj->m_nChannel;
	if( CF_CHTYPE_EMPTY == pChnlObj->m_nChanelType ) return;
	if( CF_CHTYPE_NOKNOW == pChnlObj->m_nChanelType ) return;
	
	CNodeObj *pNodeObj = pChnlObj->GetCurrentNodeObj();
	if( NULL == pNodeObj ) return;


	switch( pNodeObj->m_nType )	
	{
	case NODETYPE_START:
		{
			pChnlObj->m_strCallerID	= "";
			pChnlObj->m_strCalledID	= "";

			m_pPubCObj->PC_ShowChannelCallerID(nIndex,pChnlObj->m_strCallerID);
			m_pPubCObj->PC_ShowChannelCalledID(nIndex,pChnlObj->m_strCalledID);
			Nor32_MoveNext_Node( nIndex, NEXTTYPE_NEXT );//查找该结点分支指向的的结点
		}
		break;
	case NODETYPE_WAITCALL:
		Nor32_WaitCall( nIndex );
		break;
	case NODETYPE_START_PLAY:
		Nor32_PlayStart( nIndex );
		break;
	case NODETYPE_CHECK_DIAL:
		Nor32_DialCheck( nIndex );
		break;
	case NODETYPE_RELEASECALL:
		Nor32_ReleaseCall( nIndex );
		break;
	case NODETYPE_CALLHOLD:
		Nor32_CallHold( nIndex );
		break;
	case NODETYPE_CHECK_TRANSFER:
		Nor32_Transfer( nIndex );
		break;
	case NODETYPE_START_RECORD:
		Nor32_StartRecord( nIndex );
		break;
	case NODETYPE_SETVARIABLE:
		m_pPubCObj->PC_SetVariable( nIndex,pChnlObj);
		break;
	case NODETYPE_COMPARE:
		m_pPubCObj->PC_Compare( nIndex,pChnlObj );
		break;
	case NODETYPE_ENTER_CONFERENCE:
		Nor32_ConferenceEnter(nIndex);
		break;
	case NODETYPE_LEAVE_CONFERENCE:
		Nor32_ConferenceLeave( nIndex );
		break;
	case NODETYPE_FAXRECIVE:
		Nor32_FaxRecive( nIndex );
		break;
	case NODETYPE_FAXSEND:
		Nor32_FaxSend( nIndex );
		break;
	case NODETYPE_CONFERENCE_BUILD:
		break;
	case NODETYPE_CONFERENCE_REQUEST:
		break;
	case NODETYPE_CONFERENCE_DELETE:
		break;
	case NODETYPE_GET_CONFERENCE_MEMBER:
		break;
	case NODETYPE_OPEN_DATABASE:
		m_pPubCObj->PC_OpenDatabase( nIndex,pChnlObj  );
		break;
	case NODETYPE_CLOSE_DATABASE:
		m_pPubCObj->PC_CloseDatabase( nIndex ,pChnlObj );
		break;
	case NODETYPE_EXCUTE_SQL:
		m_pPubCObj->PC_ExcuteSQL( nIndex ,pChnlObj );
		break;
	case NODETYPE_DLL_LOAD:
		m_pPubCObj->PC_DLLLoad( nIndex ,pChnlObj );
		break;
	case NODETYPE_DLL_UNLOAD:
		m_pPubCObj->PC_DLLUnLoad( nIndex ,pChnlObj );
		break;
	case NODETYPE_DLL_CALL:
		m_pPubCObj->PC_DLLCall( nIndex ,pChnlObj );
		break;
	case NODETYPE_SEND_DTMFCODE:
		Nor32_DTMFSend( nIndex );
		break;
	case NODETYPE_RECIVE_DTMF:
		Nor32_DtmfRecive( nIndex );
		m_pPubCObj->PC_ShowChannelDTMF( nIndex,pChnlObj->m_strDTMFRecived);
		break;
	case NODETYPE_EVENT_CHECK:
		Nor32_EventCheck( nIndex );
		return;
		break;
	case NODETYPE_DATABASE_BROWSE:
		m_pPubCObj->PC_DBBrowse( nIndex,pChnlObj );
		break;
	case NODETYPE_DTMF_MENU:
		Nor32_DTMFMenu( nIndex );
		m_pPubCObj->PC_ShowChannelDTMF( nIndex,pChnlObj->m_strDTMFRecived);
		break;
	case NODETYPE_CALLLOGREQUEST:
		//Nor32_CallLogRequest( nChnl );
		break;
	case NODETYPE_NODE_END:
		Nor32_NodeEnd( nIndex );
		break;
	case NODETYPE_SWITCH:
		Nor32_Switch( nIndex );	
		break;
	case NODETYPE_ANSWERCALL:
		Nor32_AnswerCall( nIndex );	
		break;
	case NODETYPE_BLOCK:
		m_pPubCObj->PC_Block( nIndex,pChnlObj );
		break;
	case NODETYPE_ENTER:
		m_pPubCObj->PC_Enter( nIndex ,pChnlObj );
		break;
	case NODETYPE_RETURN:
		m_pPubCObj->PC_Return( nIndex ,pChnlObj);
		break;
	case NODETYPE_STOREPRO:
		m_pPubCObj->PC_StoredProcedures( nIndex ,pChnlObj );
		break;
	case NODETYPE_CHANGEVOICE:
		break;
	case NODETYPE_ENDCALL_BLOCK:
		break;
	case NODETYPE_ENDCALLEVENT_CHECK:
		Nor32_EndCallEventCheckBlock( nIndex );
		break;
	case NODETYPE_ENDCALLEVENT_END:
		Nor32_EndCallEventEndBlock( nIndex );
		break;
	case NODETYPE_FUNCTION:
		m_pPubCObj->PC_Function( nIndex,pChnlObj ,m_nTotalChannel, &m_pChannelInfo[0] );
		break;
	case NODETYPE_EXPRESS:
		m_pPubCObj->PC_Express( nIndex,pChnlObj);
		break;
	case NODETYPE_GOTO:
		m_pPubCObj->PC_GoTo( nIndex,pChnlObj );
		break;
	case NODETYPE_ROUTE_CALL:
		break;
	case NODETYPE_TRACE_INFO:
		m_pPubCObj->PC_TraceInfo( nIndex,pChnlObj );
		break;
	case NODETYPE_MOUDLE:
		m_pPubCObj->PC_Moudle( nIndex,pChnlObj );
		break;
	case NODETYPE_MOUDLE_BEGIN:
		m_pPubCObj->PC_MoveNext_Node( nIndex, pChnlObj, NEXTTYPE_NEXT );
		break;
	case NODETYPE_MOUDLE_END:
		m_pPubCObj->PC_MoudleEnd( nIndex,pChnlObj );
		break;
	case NODETYPE_CUSTOM:
		Nor32_Custom( nIndex );
		break;
	default:
		break;

	}

	//挂机检测


	return;
}

BOOL CNorMarch32::Nor32_InitChannel(CCFChnlSetObj *pChSetObj, int nChnlIndex)
{
	CString strFile = pChSetObj->m_strScriptFile;
	if( NULL == pChSetObj ) return FALSE;
	if( nChnlIndex < 0 || nChnlIndex >=  m_nTotalChannel ) return FALSE;
	if( "" == strFile ) return FALSE;
	int nChnl = pChSetObj->m_nIndex;//取得通道号
	CFChannelInfo *pChnlObj		= &( m_pChannelInfo[nChnlIndex] );
	pChnlObj->m_nChannel		= nChnl;
	pChnlObj->m_nChanelType		= pChSetObj->m_nType;
	pChnlObj->m_strScriptFile	= strFile;
	pChnlObj->m_lCardKey		= Nor32_PlayDTMF(pChSetObj->m_lCardKey);

	
	pChnlObj->m_strLocalDN = pChSetObj->m_strLocalDN;
	Nor32_MyHangUp( nChnlIndex );


	
	
	CNormalScriptInfo *pNSInfo = NULL;
	for( int i = 0; i < m_nNormalScriptNum; i++ )
	{
		if( strFile == m_pNormalScriptInfo[i].m_strScriptFile )
		{
			pNSInfo = &(m_pNormalScriptInfo[i]);
			break;
		}

	}

	if( NULL == pNSInfo )
		return FALSE;

	
	pChnlObj->RemoveALLObjects();
	POSITION pos = pNSInfo->m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CNodeObj *pObj = pNSInfo->m_objects.GetNext(pos);
		if( NULL == pObj ) continue;
		pNSInfo->InitNodeObjList( &(pChnlObj->m_objects), pObj );
		m_pwndProg->StepIt();//进度条显示
	}//end while
	
	pos = pChnlObj->m_objects.GetHeadPosition();
	pChnlObj->m_pCurrentNodeObj = pChnlObj->m_objects.GetAt( pos );

	pos = pNSInfo->m_RecordsetVariList.GetHeadPosition();
	while (pos != NULL)
	{
		CRecordsetVar *pRsObj = pNSInfo->m_RecordsetVariList.GetNext(pos);
		if( NULL != pRsObj )
		{
			
			CNewRecordsetVar *pNewObj = new CNewRecordsetVar(*pRsObj);
			pChnlObj->m_NewRecordsetVariList.AddTail(pNewObj);
		}

	}

	pos = pNSInfo->m_DBConnVarList.GetHeadPosition();
	while (pos != NULL)
	{
		CDBConnVar *pDBObj = pNSInfo->m_DBConnVarList.GetNext(pos);
		if( NULL != pDBObj )
		{
			CNewDBConnVar *pNewObj = new CNewDBConnVar( *pDBObj );
			pChnlObj->m_NewDBConnVarList.AddTail(pNewObj);

			
		}

	}
	pos = pNSInfo->m_NormalVarList.GetHeadPosition();
	while (pos != NULL)
	{
		CNormalVar *pNewObj = new CNormalVar;
		*pNewObj = *( pNSInfo->m_NormalVarList.GetNext(pos) );
		if( NULL != pNewObj )
			pChnlObj->m_NormalVarList.AddTail(pNewObj);

	}
	pos = pNSInfo->m_DLLVariableList.GetHeadPosition();
	while (pos != NULL)
	{
		CDLLVariable *pNewObj = new CDLLVariable;
		*pNewObj = *( pNSInfo->m_DLLVariableList.GetNext(pos) );
		if( NULL != pNewObj )
			pChnlObj->m_DLLVariableList.AddTail(pNewObj);

	}

	

	return TRUE;
}


BOOL CNorMarch32::Nor32_MoveNext_Node(int nIndex, CNextNode *pNextObj)
{
	
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	return m_pPubCObj->PC_MoveNext_Node( nIndex, pChnlObj, pNextObj );
	
}
BOOL CNorMarch32::Nor32_MoveNext_Node(int nIndex, int nNextType)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	return m_pPubCObj->PC_MoveNext_Node( nIndex, pChnlObj, nNextType );
}


CNodeObj * CNorMarch32::Nor32_MoveNextObj(int nNextType, CNodeObj *pCurObj, CCFDocument *pDoc)
{
	if( NULL == pCurObj || pDoc == NULL )  return NULL;
	CNextNode *pNextObj = pCurObj->FindNextNodeObj( nNextType );
	if( NULL == pNextObj ) return NULL;
	
	POSITION pos = pDoc->m_objects.GetHeadPosition();
	while (pos != NULL)
	{
		CNodeObj *pObj = pDoc->m_objects.GetNext(pos);
		if( NULL == pObj ) break;
		if( pObj->m_nKey == pNextObj->m_nNodeObjKey )
		{
			return pObj;
		}
	}//end while

	return NULL;
}

void CALLBACK Nor32_EventCallBack(EPhonicEvent eventType, int channelType, int channelID, int lParam1, int lParam2)
{
	if( g_NorMarch32 )
	{
		g_NorMarch32->Nor32_OnEventNotify(
					eventType,
					channelType,	
					channelID, 
					lParam1,
					lParam2);
	}
	return;
}

void CNorMarch32::Nor32_OnEventNotify(EPhonicEvent eventType, int channelType, int channelID, int lParam1, int lParam2)
{
	int nIndex = -1;
	if( 5 == channelType )//是会议
	{
		if( eventPlayEnd == eventType )
		{
			nIndex = Nor32_GetIndexByGroup( channelID, eventType );
			if( nIndex < 0 )
			{
				tpi_StopPlay( channelType,  channelID );
			}
			else
			{
				if( m_pChannelInfo[nIndex].m_bPlayLoop )
				{
					Nor32_MyStartPlayFile( nIndex,m_pChannelInfo[nIndex].m_strPlayFile,channelID,TRUE,1);
				}
				else
				{
					Nor32_MyStopConfPlay( channelID );
				}
			}
		}
		else if( eventRecordEnd == eventType )
		{
			nIndex = Nor32_GetIndexByGroup( channelID, eventType );
			if( nIndex < 0 )
			{
				tpi_StopRecord( channelType,  channelID );
			}
			else
			{
				if( m_pChannelInfo[nIndex].m_bRecordLoop )
				{
					Nor32_MyStartPlayFile( nIndex,m_pChannelInfo[nIndex].m_strPlayFile,channelID,TRUE,1);
				}
				else
				{
					Nor32_MyStopConfRecord( channelID );
				}
			}
		}

	}
	else
	{
		nIndex = Nor32_GetCifLogicChnl( channelID, channelType );
	}

	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}
	switch(eventType)
	{
	case eventState:
		{
			//Nor32_OnEventState( nIndex, (int)lParam1);
		}
		break;
	case eventDeviceTimer:
		{
			//pChannel->OnDeviceTimer();		
			
		}
		break;
	case eventIdle:
		{				
			//pChannel->OnIdle();				
			
		}
		break;
	case eventCallIn:
		{
			if(  CH_USER  == channelType )
			{
				Nor32_OnUserOffHook( nIndex );
			}
			else
			{
				Nor32_OnTrunkCallIn( nIndex, (char *)lParam1, (char *)lParam2 );
			}
		}
		break;
	case eventAnswer:
		{
			if(  CH_USER  == channelType )
			{
				Nor32_OnUserOffHook(  nIndex );
			}
			Nor32_Switch_Judge(  nIndex,TRUE );//检查Switch结果
			Nor32_Transfer_Judge( nIndex ,TRUE);
			Nor32_Dial_Judge( nIndex ,TRUE);
			Nor32_Cus_OutCallResult( nIndex,TRUE );
		}
		break;
	case eventCallOutFinish:
		break;
	case eventCallFail:
		{
			//pChannel->OnCallFail( lParam1);
			Nor32_MyHangUp( nIndex );
			Nor32_Switch_Judge(  nIndex,  FALSE );//检查Switch结果
			Nor32_Transfer_Judge( nIndex,FALSE );
			Nor32_Dial_Judge( nIndex ,FALSE);
			Nor32_Cus_OutCallResult( nIndex,FALSE );
		}
		break;
	case eventHangup:
		{
			Nor32_ResetChannel( nIndex );	
		}
		break;
	case eventDTMF:	
		{
			Nor32_OnReciveDtmf( nIndex, ( char )lParam1 );
		}
		break;
	case eventPlayEnd:
		{
			Nor32_OnPlayEnd( nIndex );
		}
		break;
	case eventRecordEnd:
		{
			Nor32_OnRecordEnd( nIndex );
		}
		break;
	case eventFaxError:
		{
			Nor32_OnFaxError( nIndex, (long)lParam1);	
		}
		break;
	case eventOneFaxPageOver:
		{
			Nor32_OnOneFaxPageOver( nIndex, (int)lParam1 );
		}
		break;
	case eventFlash:
		{
			Nor32_OnFlash( nIndex );		
		}
		break;
	case eventRing:
		{
			Nor32_OnRing( nIndex );		
		}
		break;
	case eventVoice:
		break;
	case eventSilence:
		break;
	case eventHanging:
		{
			Nor32_ResetChannel(  nIndex );		
		}
		break;
	case eventDetectedFaxTone:  
		break;//检测到传真音,设置到处32kbps 	 
	case eventFaxToneMissing:
		break;//检测到传真音,设置到处32kbps 	 				
	default:
		break;

	}

	return;
}



BOOL CNorMarch32::Nor32_InitSystem(CListCtrl *plstChannel, CListBox *plistSystem, CListCtrl *plistScript, CListBox *pCommList, CListCtrl *plistTCPIP, CListCtrl *plstConfer, CListCtrl *plstFax)
{
	m_pPubCObj->m_plistTCPIP	= plistTCPIP;
	m_pPubCObj->m_pCommList		= pCommList;
	m_pPubCObj->m_plistScript	= plistScript;
	m_pPubCObj->m_plstChannel	= plstChannel;
	m_pPubCObj->m_plistSystem	= plistSystem;
	m_pPubCObj->m_plstConfer	= plstConfer;
	m_pPubCObj->m_plstFax		= plstFax;
	m_pPubCObj->m_pwndProg		= m_pwndProg;
	m_pPubCObj->PC_OpenLogFile();
	m_pwndProg->StepIt();//进度条显示
	m_pPubCObj->PC_InitSystemVar();

	if( !Nor32_LoadSysScript()) return false;
	if( !Nor32_LoadNormalScript()) return false;
	m_pwndProg->StepIt();//进度条显示


	tpi_SetEventNotifyCallBackProc( Nor32_EventCallBack );
	if( tpi_OpenDevice() < 0 )
	{
		m_pPubCObj->PC_ShowSysMsg(0,"Load Driver Fail");
		tpi_SetEventNotifyCallBackProc( NULL );
		return  FALSE;
	}

	
	PostMessage( m_hWnd , WM_MY_FRIEND_MSG, 0, tpi_GetCardPass( 0 ));
	m_nTotalChannel = tpi_GetLineCount();


	
	CChiefIVRDoc doc;
	CString strfile = "";
	strfile = m_strPath + "\\CFCHDJ.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		tpi_SetEventNotifyCallBackProc( NULL );
		tpi_CloseDevice();
		return FALSE;
	}
	
	m_nTotalChannel = doc.m_nTotalChannel;
	
	CString str = "";
	POSITION pos = doc.m_ChSetListObj.GetHeadPosition();
	
	int m= -1;
	while( pos )
	{
		CCFChnlSetObj *pObj = doc.m_ChSetListObj.GetNext(pos);
		if( NULL != pObj )
		{
			m++;
			if( !Nor32_InitChannel( pObj , m ) )
			{
				break;
			}
		}
		m_pwndProg->StepIt();//进度条显示
	}
	

	
	return TRUE;
}

/////////////////////////////////////////////////////////
//通过TMS卡语音设备通道类型，取得CIF定义的的逻辑通道类型
/////////////////////////////////////////////////////////
inline int CNorMarch32::Nor32_GetCifChnlType(int nDevChnlType)
{
	if( CH_EMPTY == nDevChnlType) //空线路类型
		return CF_CHTYPE_EMPTY;
	else if(  CH_ANALOG_TRUNK    == nDevChnlType)  //模拟中继外线(普通市话线),接电信局
		return CF_CHTYPE_TRUNK;
	else if(  CH_USER  == nDevChnlType)  //模拟中继内线(座席线)，接电话机
		return CF_CHTYPE_USER;
	else if(  CH_HIGH_IMPEDANCE  == nDevChnlType)  //模拟中继高阻线(监控线)，与电话线/电话机相并联
		return CF_CHTYPE_RECORD;
	else
		return CF_CHTYPE_EMPTY;

}

/////////////////////////////////////////////////////////
//通过CIF定义的的逻辑通道类型,取得TMS卡语音设备通道类型
/////////////////////////////////////////////////////////
inline int CNorMarch32::Nor32_GetDevChnlType(int nCifChnlType)
{
	if( CF_CHTYPE_EMPTY == nCifChnlType) //空线路类型
		return CH_EMPTY;
	else if( CF_CHTYPE_TRUNK == nCifChnlType)  //模拟中继外线(普通市话线),接电信局
		return CH_ANALOG_TRUNK  ;
	else if( CF_CHTYPE_USER == nCifChnlType)  //模拟中继内线(座席线)，接电话机
		return  CH_USER;
	else if( CF_CHTYPE_RECORD == nCifChnlType)  //模拟中继高阻线(监控线)，与电话线/电话机相并联
		return CH_HIGH_IMPEDANCE  ;
	else
		return CH_EMPTY;
}


/////////////////////////////////////////////////////////
//通过TMS卡语音设备的通道号以及通道类型，取得CIF定义的的逻辑
//通道号
/////////////////////////////////////////////////////////
int CNorMarch32::Nor32_GetCifLogicChnl(int nDevChnlIndex, int nDevChnlType)
{
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( nDevChnlIndex == m_pChannelInfo[i].m_nChannel )
		{
			if( m_pChannelInfo[i].m_nChanelType == Nor32_GetCifChnlType(nDevChnlType) ) 
			{
				return i;
			}
		}
	}

	return -1;
}
BOOL CNorMarch32::Nor32_LoadSysScript(void)
{
	CCIFSysScriptDoc doc;
	CString strfile = "";
	strfile = m_strPath + "\\CFTMS.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		return FALSE;
	}

	m_lSystem = Nor32_PlayDTMF(doc.m_lCardKey);
	m_pPubCObj->PC_LoadSysScript( &(doc) );


	return TRUE;
}

BOOL CNorMarch32::Nor32_LoadNormalScript(void)
{
	CCIFSysScriptDoc doc;
	CString strfile = "";
	strfile = m_strPath + "\\CFSynWay.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		return FALSE ;
	}
	
	m_lNormal =  Nor32_PlayDTMF(doc.m_lCardKey);
	if( NULL != m_pNormalScriptInfo )
		delete []m_pNormalScriptInfo;

	CString str = "";
	POSITION pos = doc.m_SysScriptListObj.GetHeadPosition();
	m_nNormalScriptNum = doc.m_SysScriptListObj.GetCount();
	if( m_nNormalScriptNum > 0 )
	{
		m_pNormalScriptInfo = new CNormalScriptInfo[m_nNormalScriptNum];
		int i = 0;
		while( pos )
		{
			CCFSysScriptSetObj *pObj = doc.m_SysScriptListObj.GetNext(pos);
			if( NULL != pObj )
			{
				m_pNormalScriptInfo[i].m_strScriptFile = pObj->m_strScriptFile;
				m_pNormalScriptInfo[i].InitNormalScript(  pObj->m_strScriptFile );
				i++;
			}
		}
	}
	


	return TRUE;
}
void CNorMarch32::Nor32_OnOneFaxPageOver(int nIndex,int nPage)
{
	if( 0 == nPage ){
		return;
	}

	CString strMSG = "";
	Nor32_MyFaxStop( nIndex );
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CNodeObj *pNodeObj = pChnlObj->GetCurrentNodeObj();
	if( NULL == pNodeObj ){
		return;
	}
	
	if( NODETYPE_FAXRECIVE == pNodeObj->m_nType )
	{
		CFaxReciveObj *pObj = (CFaxReciveObj*)pChnlObj->GetCurrentNodeObj();
		if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CFaxReciveObj) ) )
		{
			strMSG.Format("Recive Fax error : ChIndex = %d   Error = Object is not valid", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
			return ;  
		}

		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		return;
	}
	else if( NODETYPE_FAXSEND == pNodeObj->m_nType )
	{
		CFaxSendObj *pObj = (CFaxSendObj*)pChnlObj->GetCurrentNodeObj();
		if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CFaxSendObj) ) )
		{
			strMSG.Format("Send Fax error : ChIndex = %d   Error = Object is not valid", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
			return ;  
		}

		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		return;
	}

	return;

}

void CNorMarch32::Nor32_OnFaxError(int nIndex, long cause)
{
	Nor32_MyFaxStop( nIndex );
	
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CNodeObj *pNodeObj = pChnlObj->GetCurrentNodeObj();
	if( NULL == pNodeObj ){
		return;
	}
	
	CString strMSG = "";

	if( NODETYPE_FAXRECIVE == pNodeObj->m_nType )
	{
		CFaxReciveObj *pObj = (CFaxReciveObj*)pChnlObj->GetCurrentNodeObj();
		if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CFaxReciveObj) ) )
		{
			strMSG.Format("Recive Fax error : ChIndex = %d   Error = Object is not valid", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
			return ;  
		}

		strMSG.Format("%s: ChIndex = %d    RcvFaxFile error ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	else if( NODETYPE_FAXSEND == pNodeObj->m_nType )
	{
		CFaxSendObj *pObj = (CFaxSendObj*)pChnlObj->GetCurrentNodeObj();
		if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CFaxSendObj) ) )
		{
			strMSG.Format("Send Fax error : ChIndex = %d   Error = Object is not valid", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
			return ;  
		}

		strMSG.Format("%s: ChIndex = %d Send Fax error",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	return;

}

void CNorMarch32::Nor32_OnTrunkCallIn(int nIndex, char *pszCallerID, char *pszCalledID)
{
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);

	pChnlObj->m_strInboundCallPK.Format("%s%d", (CTime::GetCurrentTime()).Format("%Y%m%d%H%M%S") , nIndex );
	pChnlObj->m_strCallerID= pszCallerID ;
	pChnlObj->m_strCalledID= pszCalledID ;
	pChnlObj->m_nCallType = CHANNEL_CALLTYPE_IN;

	m_pPubCObj->PC_ShowChannelCallerID(nIndex,pszCallerID);
	m_pPubCObj->PC_ShowChannelCalledID(nIndex,pszCalledID);


	CNodeObj *pNodeObj = pChnlObj->GetCurrentNodeObj();
	if( NULL == pNodeObj ){
		return;
	}
	
	if( NODETYPE_WAITCALL == pNodeObj->m_nType )
	{

		CWaitCallObj *pObj = (CWaitCallObj*)pNodeObj;
		if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CWaitCallObj) ) )
		{
			strMSG.Format("ChIndex = %d   Error = Object is not valid", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return ;   
		}
		

		if( pObj->m_bRecCallerID )
			m_pPubCObj->PC_SaveValuesToVar(pChnlObj,  pChnlObj->m_strCallerID, pObj->m_CallerID.m_nKey );
		if( pObj->m_bRecCalledID )
			m_pPubCObj->PC_SaveValuesToVar(pChnlObj,  pChnlObj->m_strCalledID, pObj->m_CalledID.m_nKey );
		if( pObj->m_bClearDTMF ) 
			pChnlObj->m_strDTMFRecived	= "" ;

		if( pObj->m_bAnswerCall )
		{
			Nor32_MyOFFHook( nIndex);
		}

		m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
		pChnlObj->m_nState =  CHANNEL_STATE_CONNECT;
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		return;
	}


	return;
}


void CNorMarch32::Nor32_OnEventState( int nIndex, int nState)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	switch(nState)
	{
	case STATE_IDLE:
		break;
	case STATE_IN_CALLING:
		break;
	case STATE_IN_RINGING:
		break;
	case STATE_IN_TALK:
		break;
	case STATE_IN_HANGUP:
		break;
	case STATE_IN_RELEASE:
		break;
	case STATE_OUT_CALLING:
		break;
	case STATE_OUT_RINGING:
		break;
	case STATE_OUT_TALK:
		break;
	case STATE_OUT_HANGUP:
		break;
	case STATE_OUT_RELEASE:
		break;
	case STATE_DISABLE:
		break;
	default:
		break;
	}
}

void CNorMarch32::Nor32_OnReciveDtmf(int nIndex, char dtmf)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	pChnlObj->m_nEventType =  EVENTTYPE_RECDTMF;
	if( strlen(pChnlObj->m_strDTMFRecived) > 100 ){
		//防止恶意按键
		pChnlObj->m_strDTMFRecived = "";
	}
	pChnlObj->m_strDTMFRecived += dtmf;
	m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlObj->m_strDTMFRecived  );

	return;
}

void CNorMarch32::Nor32_OnUserOffHook(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	pChnlObj->m_nEventType =  EVENTTYPE_OFFHOOK;
	
	m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");

	return;
}

void CNorMarch32::Nor32_OnFlash(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	pChnlObj->m_nEventType =  EVENTTYPE_HOOKFLASH;
	m_pPubCObj->PC_ShowChannelState( nIndex, "Flash");
	return;
}

void CNorMarch32::Nor32_OnRing(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	pChnlObj->m_strRingTime = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
	pChnlObj->m_nEventType =  EVENTTYPE_RINGING;
	pChnlObj->m_nState =  CHANNEL_STATE_RINGING;
	pChnlObj->m_nRings += 1;
	m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");

	return;
}

void CNorMarch32::Nor32_ResetChannel(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nDevChnlType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int nLinkIndex = pChnlInfo->m_nLinkCh;
	if( -1 < nLinkIndex && nLinkIndex < m_nTotalChannel  )
	{
		if( CHANNEL_STATE_HOLD != pChnlInfo->m_nState && CHANNEL_STATE_MONITORING !=  pChnlInfo->m_nState )
		{
			CFChannelInfo *pLinkChnlInfo = &(m_pChannelInfo[nLinkIndex]);
			Nor32_MyHangUp( nLinkIndex  );
			pLinkChnlInfo->MoveCallEndBlock_Node();

		}
	}

	int nTransferIndex = pChnlInfo->m_nTransferCh;
	if( -1 < nTransferIndex && nTransferIndex < m_nTotalChannel  )
	{

		CFChannelInfo *pLinkChnlInfo = &(m_pChannelInfo[nTransferIndex]);
		Nor32_MyHangUp( nTransferIndex );
		pLinkChnlInfo->MoveCallEndBlock_Node();
		
	}
	

	Nor32_MyHangUp( nIndex );
	pChnlInfo->MoveCallEndBlock_Node();

	

	return;
}


void CNorMarch32::Nor32_MyHangUp(int nIndex )
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;
	Nor32_MyPlaySignal(nIndex,NOR32_BUSY_SIG);
	tpi_ExitConference( channelType, channelID );
	Nor32_MyStopPlay( nIndex );
	Nor32_MyStopRecord( nIndex );
	tpi_BreakTalk( channelType, channelID );
	tpi_BreakListen( channelType, channelID );
	tpi_faxStopFax( channelType, channelID );
	tpi_Hangup( channelType, channelID, 0);

	tpi_EnableDtmfPredetect(channelType,channelID,1);


	if(  CH_USER  == channelType ){
		tpi_PlayTone( channelType, channelID, 450.0, 30000, 180,"350,350");
	}
	m_pPubCObj->PC_ShowChannelConferenceNO( nIndex, -1 );
	m_pPubCObj->PC_ShowChannelState( nIndex, "Free");
	m_pPubCObj->PC_ShowConnectToChnl( nIndex, -1 );
	m_pPubCObj->PC_ShowChannelCallerID(nIndex,"");
	m_pPubCObj->PC_ShowChannelCalledID(nIndex,"");


	if( CF_CHTYPE_USER == pChnlInfo->m_nChanelType )
	{
		m_pPubCObj->PC_CTI_HangUpMsg( nIndex, pChnlInfo );
	}

	return;
}

void CNorMarch32::Nor32_EventCheck(int nIndex)
{
	CString strMSG = "";
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CEventCheckObj *pObj = (CEventCheckObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CEventCheckObj) ) )
	{
		strMSG.Format("EventCheck error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}

	
	if( 0 == pChnlInfo->m_nStep )
	{
		pChnlInfo->m_nStep = 1;
		m_pPubCObj->PC_StartTimer(pChnlInfo);
		if(  pObj->m_bClearDTMF )
		{
			pChnlInfo->m_strDTMFRecived = "";
			if( EVENTTYPE_RECDTMF == pChnlInfo->m_nEventType )
			{
				pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
			}

		}
	}

	if(  pObj->m_bElapseTime )
	{
		if( m_pPubCObj->PC_ElapseTime(pChnlInfo)  >=  pObj->m_nElapseTime )
		{
			//到了指定的时间，将发生时钟事件
			m_pPubCObj->PC_StartTimer(pChnlInfo);
			pChnlInfo->m_nEventType		=  EVENTTYPE_ELAPSE_TIME;
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ELAPSE_TIME);
			return;
		}
	}

    if( Nor32_EventCheck_EventMsg( nIndex, pObj ) )//如果检测到事件消息，直接退出
		return;

	if( pObj->m_bNothing )
	{
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NOTHING);
	}

	return;
}

BOOL CNorMarch32::Nor32_EventCheck_EventMsg(int nIndex, CEventCheckObj *pObj)
{
	CString strMSG = "";
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	switch( pChnlInfo->m_nEventType )
	{
	case EVENTTYPE_OFFHOOK:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( CF_CHTYPE_USER == pChnlInfo->m_nChanelType )
		{

			if( CHANNEL_STATE_FREE == pChnlInfo->m_nState )
				Nor32_MyPlaySignal( nIndex, NOR32_DIAL_SIG );
			else
				Nor32_MyStopPlay( nIndex );

			pChnlInfo->m_nState	   = CHANNEL_STATE_CONNECT;
			strMSG.Format("[Send OffHook DN:%s]",pChnlInfo->m_strLocalDN);
			m_pPubCObj->PC_ShowCommMsg(strMSG);
			CTI_OffHookMsg( atoi(pChnlInfo->m_strLocalDN) );
		}
		if( pObj->m_bOffHook )
		{
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_OFFHOOK);
			return TRUE;
		}
		break;
	case EVENTTYPE_HANGUP:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;

		m_pPubCObj->PC_ShowChannelState( nIndex, "Free" );
		if( pObj->m_bHangUp )
		{
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_HANGUP);
			return TRUE;
		}
		else
		{
			//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
			pChnlInfo->MoveCallEndBlock_Node();
		}
		break;
	case EVENTTYPE_HOOKFLASH:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bHookFlash )
		{
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_HOOKFLASH);
			return TRUE;
		}
		break;
	case EVENTTYPE_RECDTMF:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bRecDTMF )
		{
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_RECDTMF);
			return TRUE;
		}
		break;
	case EVENTTYPE_RECFAXTONE:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bRecFaxTone )
		{
			
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_RECFAXTONE);
			return TRUE;
		}
		break;


	case EVENTTYPE_CONFDESTROYED:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bConfDestroyed )
		{
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_CONFDESTROYED);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_LOGIN_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentLoginMsg )
		{
			
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_LOGIN_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_OUT_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentLogOutMsg )
		{
			
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_OUT_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_BUSY_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentBusyMsg )
		{
			
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_BUSY_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_INSERVE_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentInserveMsg )
		{
			
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_INSERVE_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_ANSWER_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentAnswerMsg )
		{
			
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_ANSWER_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_MAKECALL_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentMakeCallMsg )
		{
			
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_MAKECALL_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_TRANSFER_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentTransferMsg )
		{
			
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_TRANSFER_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_CONFER_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentConferMsg )
		{
			
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_CONFER_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_HANGUP_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentHangUpMsg )
		{
			
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_HANGUP_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_SENDDATA_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentSendDataMsg )
		{
			
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_SENDDATA_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_SENDFAX_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentSendFaxMsg )
		{
			
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_SENDFAX_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_RINGING:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bRinging )
		{
			
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_RINGING);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_HOLD_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentHold )
		{
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_HOLD_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_PICKUPHOLD_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentPickupHold )
		{
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_PICKUPHOLD_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_MONITOR_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentMonitor )
		{
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_MONITOR_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_DISMONITOR_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentDisMonitor )
		{
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_DISMONITOR_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_BUILDCONFER_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentBuildConfer )
		{
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_BUILDCONFER_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_GETACTIVECALL:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentGetActiveCall )
		{
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_GETACTIVECALL);
			return TRUE;
		}
		break;
	default:
		break;
	
	}

	return FALSE;
}



void CNorMarch32::Nor32_NodeEnd(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	//Nor32_ReleaseCall( nIndex );
	pChnlObj->MoveCallEndBlock_Node();

	return;
}

void CNorMarch32::Nor32_PlayStart(int nIndex)
{
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	short int code = -1;
	CPlayStartObj *pObj = (CPlayStartObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CPlayStartObj) ) )
	{
		strMSG.Format("ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}


	int nStep = pChnlObj->m_nStep;
	switch(nStep)
	{
	case 0:
		{
			if( pObj->m_bClearDTMF ) 
			{
				pChnlObj->m_strDTMFRecived = "";
			}
			
			Nor32_MyStopPlay(nIndex);
			pChnlObj->m_nStep = 1;
		}
		break;
	case 1:
		{
			CNodeDataList *pDadaList = &(pObj->m_PlayList);
			int nCount = pDadaList->GetCount();
			if( nCount <= 0 || nCount > 30 ){
				strMSG.Format("%s: ChIndex = %d  The  count of play list <= 0 or > 30",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MyStopPlay(nIndex);
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

			int nGroupNo = -1;
			int nPlayWay = pObj->m_nPlayWay ;
			if( 1 == nPlayWay )//会议放音
			{
				int nVarKey = pObj->GetVarKey( pObj->m_strConfNO  );
				if( nVarKey < 0 )
				{
					nGroupNo = atoi(pObj->m_strConfNO );
				}
				else
				{
					if( !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strMSG, pObj->GetVarKey( pObj->m_strConfNO ) ) )
					{
						strMSG.Format("%s: ChIndex = %d    Play error ",  pObj->m_strCaption, nIndex);
						m_pPubCObj->PC_ShowSysMsg( nIndex,strMSG );
						Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
					nGroupNo = atoi(strMSG);
				}
				
				if( -1 == nGroupNo )
				{
					nGroupNo = Nor32_MyGetConferenceNo( nIndex );
				}
				else
				{
					Nor32_MyStopConfPlay( nGroupNo );
				}
			}

			CPlayFileList PlayfileList[30];
			int i = 0;
			POSITION pos = pDadaList->GetHeadPosition( );		
			//下面开始初始化数据
			CNodeData *pData = NULL;
			while( pos )
			{
				pData = pDadaList->GetNext(pos);
				if( NULL == pData ) continue;
				if( 0 == pData->m_nDataType )//是文件
				{
					m_pPubCObj->PC_GetExpressValues(pChnlObj, strMSG, pData->m_strData);
					PlayfileList[i].m_nIndex = i;
					PlayfileList[i].m_strPlayFile = strMSG;
					i++;
				}
				else
				{
					///////////////////////////////////////////////////////
					CString strNewExpress("");
					if( !m_pPubCObj->PC_GetExpressValues( pChnlObj,strNewExpress,pData->m_strData) )
					{
						strMSG.Format("%s: ChIndex = %d    Play Express Error :%s ",  pObj->m_strCaption, nIndex, pData->m_strData );
						m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
						Nor32_MyStopPlay(nIndex);
						Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
					int nStrLen = strNewExpress.GetLength();
					if( nStrLen <= 0 ) 
					{
						strMSG.Format("%s: ChIndex = %d    Play Express Error :%s ",  pObj->m_strCaption, nIndex, pData->m_strData );
						m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
						Nor32_MyStopPlay(nIndex);
						Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
					
					
					CString strPaht = m_strPath+"\\SysVoiceFle\\";
					CString strFileName("");
					for( int m = 0; m< nStrLen; m++ )
					{
						strFileName = "";
						if( '0' == strNewExpress.GetAt(m) )
						{
							strFileName = strPaht + "D0";
						}
						else if( '1' == strNewExpress.GetAt(m) )
						{
							strFileName = strPaht + "D1";
						}
						else if( '2' == strNewExpress.GetAt(m) )
						{
							strFileName = strPaht + "D2";
						}
						else if( '3' == strNewExpress.GetAt(m) )
						{
							strFileName = strPaht + "D3";
						}
						else if( '4' == strNewExpress.GetAt(m) )
						{
							strFileName = strPaht + "D4";
						}
						else if( '5' == strNewExpress.GetAt(m) )
						{
							strFileName = strPaht + "D5";
						}
						else if( '6' == strNewExpress.GetAt(m) )
						{
							strFileName = strPaht + "D6";
						}
						else if( '7' == strNewExpress.GetAt(m) )
						{
							strFileName = strPaht + "D7";
						}
						else if( '8' == strNewExpress.GetAt(m) )
						{
							strFileName = strPaht + "D8";
						}
						else if( '9' == strNewExpress.GetAt(m) )
						{
							strFileName = strPaht + "D9";
						}
						else if( '.' == strNewExpress.GetAt(m) )
						{
							strFileName = strPaht + "D15";
						}

						if( "" != strFileName )
						{
							PlayfileList[i].m_nIndex = i;
							PlayfileList[i].m_strPlayFile = strFileName;
							i++;
						}

					}
				}
			}//end while


			for( i = 0; i < 30 ; i++ )
			{
				if( -1 != PlayfileList[i].m_nIndex )
				{
					if( !Nor32_MyStartPlayFile( nIndex , PlayfileList[i].m_strPlayFile,nGroupNo,FALSE,nPlayWay ) )
					{
						strMSG.Format("%s: ChIndex = %d    AddIndexPlayFile error  %s",  pObj->m_strCaption, nIndex,PlayfileList[i].m_strPlayFile);
						m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
						Nor32_MyStopPlay(nIndex);
						Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						
						return;
					}
				}

			}
			pChnlObj->m_nStep = 2;
		}
		break;
	case 2:
		{
			if( strlen( pChnlObj->m_strDTMFRecived ) > 0 )
			{
				if( pObj->m_bPlayStop )
				{
					Nor32_MyStopPlay( nIndex );
					if( pObj->m_bRecDTMF )
					{
						int nVarKey = pObj->GetVarKey( pObj->m_strSaveDTMF );
						BOOL bResult =m_pPubCObj->PC_SaveValuesToVar( nIndex, pChnlObj, pChnlObj->m_strDTMFRecived, nVarKey );
						if( !bResult )
						{
							strMSG.Format("%s Save DTMF to Var Error ;ChIndex = %d",pObj->m_strCaption, nIndex);
							m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
							Nor32_MyStopPlay(nIndex);
							Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
							return;
						}
					}
					Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );	
				}
			}
		}
		break;
	default:
		break;
	}//end switch

	return;
}

BOOL CNorMarch32::Nor32_MyStopConfPlay(int nGroup)
{
	if( nGroup < 0 ){
		return FALSE;
	}
	int nIndex = Nor32_GetIndexByGroup( nGroup, eventPlayEnd );
	tpi_StopPlay( 5,  nGroup );
	if( 0 > nIndex || nIndex >= m_nTotalChannel  )
	{
		m_pChannelInfo[nIndex].m_bPlayLoop = FALSE;
		m_pChannelInfo[nIndex].m_nPlayType = -1;
		m_pChannelInfo[nIndex].m_nPlayGroup = -1;
	}
	return TRUE;
}

void CNorMarch32::Nor32_MyStopPlay(int nIndex )
{
	//对通道来说，要么再控制会议放音，要么在控制通道放音
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}

	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;

	if( 1 == pChnlInfo->m_nPlayType )
	{
		tpi_StopPlay( 5,  pChnlInfo->m_nPlayGroup );
	}
	else
	{
		tpi_StopPlay( channelType,  channelID );
	}

	pChnlInfo->m_bPlayLoop = FALSE;
	pChnlInfo->m_nPlayType = -1;
	pChnlInfo->m_nPlayGroup = -1;
	return;
}

BOOL CNorMarch32::Nor32_MyStopConfRecord(int nGroup)
{
	if( nGroup < 0 ){
		return FALSE;
	}
	tpi_StopRecord( 5,  nGroup );
	int nIndex = Nor32_GetIndexByGroup( nGroup, eventRecordEnd);
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		m_pChannelInfo[nIndex].m_bRecordLoop = FALSE;
		m_pChannelInfo[nIndex].m_nRecordType = -1;
		m_pChannelInfo[nIndex].m_nRecordLength = 0;
		m_pChannelInfo[nIndex].m_nRecordGroup = -1;
	}
	return TRUE;
}
void CNorMarch32::Nor32_MyStopRecord(int nIndex  )
{
	//对通道来说，要么再控制会议录音，要么在控制通道录音
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}


	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;
	if(  1 == pChnlInfo->m_nRecordType )
	{
		tpi_StopRecord( 5,  pChnlInfo->m_nRecordGroup );
	}
	else
	{
		tpi_StopRecord( channelType,  channelID );
	}


	pChnlInfo->m_bRecordLoop = FALSE;
	pChnlInfo->m_nRecordType = -1;
	pChnlInfo->m_nRecordLength = 0;
	pChnlInfo->m_nRecordGroup = -1;

	return;
}
BOOL CNorMarch32::Nor32_MyFaxReceive(int nIndex, const char *fileName)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return FALSE;
	}
	if( NULL == fileName ){
		return FALSE;
	}
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;
	if( 0 == tpi_faxReceiveFax( channelType,  channelID,  fileName) ){
		return TRUE;
	}
	else{

		return FALSE;
	}
}

int CNorMarch32::Nor32_MyAddPromptFromFile(LPCTSTR fileName)
{

	return 0;
}

void CNorMarch32::Nor32_MyFaxStop(int nIndex)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;

	tpi_faxStopFax( channelType,  channelID );

	return;
}

int CNorMarch32::Nor32_MyGetConferenceNo(int nIndex)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return -1;
	}
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;
	return tpi_GetChannelConferenceHandle( channelType,  channelID) ;
}

BOOL CNorMarch32::Nor32_MyListenConference(int nIndex, int nGroup)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return FALSE;
	}
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;
	tpi_StopPlay( channelType,  channelID );
	if( 0 == tpi_ListenConference( channelType,  channelID,0,nGroup ) )
	{
		pChnlInfo->m_nConfNO = nGroup;
		m_pPubCObj->PC_ShowChannelConferenceNO( nIndex, nGroup );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CNorMarch32::Nor32_MyJoinConference(int nIndex, int nGroup)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return FALSE;
	}

	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;
	tpi_StopPlay( channelType,  channelID );
	if( 0 == tpi_JoinConference( channelType,  channelID,0,nGroup,0,0 ) )
	{
		pChnlInfo->m_nConfNO = nGroup;
		m_pPubCObj->PC_ShowChannelConferenceNO( nIndex, nGroup );
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CNorMarch32::Nor32_MyFaxSend(int nIndex, const char *fileName)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return FALSE;
	}
	if( NULL == fileName ){
		return FALSE;
	}
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;

	if( 0 == tpi_faxSendFax( channelType,  channelID,  fileName) ){
		return TRUE;
	}
	else{

		return FALSE;
	}
}
BOOL CNorMarch32::Nor32_MySendDtmfStr(int nIndex, const char *dtmfString)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return FALSE;
	}
	if( NULL == dtmfString ){
		return FALSE;
	}
	if( "" == dtmfString ){
		return FALSE;
	}
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;

	
	if( 0 == tpi_SendDtmf( channelType, channelID, dtmfString, 60, 60, 0 ) ){
		return TRUE;
	}
	else{

		return FALSE;
	}
}


BOOL CNorMarch32::Nor32_MyStartRecordFile(  int nIndex, CString strFile, BOOL bRecordLoop,int nRecordType,long lRecordLen, int nGroup )
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return FALSE;
	}
	if( "" == strFile ){
		return FALSE;
	}
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;
	
	char *pszFileName = (LPTSTR)(LPCTSTR)strFile;
	if( 1 == nRecordType )//会议录音
	{
		if( nGroup < 0 )
		{
			nGroup = Nor32_MyGetConferenceNo(  nIndex );
			if( nGroup < 0 ){
				return FALSE;
			}
		}
		if( 0 == tpi_RecordFile( 5, nGroup, pszFileName, -1, 8000L*6000L*lRecordLen ) )
		{
			pChnlInfo->m_strRecordFile = strFile;
			pChnlInfo->m_bRecordLoop = bRecordLoop;
			pChnlInfo->m_nRecordType = nRecordType;
			pChnlInfo->m_nRecordLength = lRecordLen;
			pChnlInfo->m_nRecordGroup = nGroup;
			return TRUE;
		}
		else{

			return FALSE;
		}
	}
	else
	{
		if( 0 == tpi_RecordFile( channelType, channelID, pszFileName, -1, 8000L*6000L*lRecordLen ) )
		{
			pChnlInfo->m_strRecordFile = strFile;
			pChnlInfo->m_bRecordLoop = bRecordLoop;
			pChnlInfo->m_nRecordType = nRecordType;
			pChnlInfo->m_nRecordLength = lRecordLen;
			return TRUE;
		}
		else{

			return FALSE;
		}
	}


}

BOOL CNorMarch32::Nor32_MyStartPlayFile(  int nIndex, CString strFile, int nConNo, BOOL bPlayLoop,int nPlayType/*放音类型：0普通 1会议*/ )
{
	if( "" == strFile ) return FALSE;
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return FALSE;
	}
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;
	
	char *pszFileName = (LPTSTR)(LPCTSTR)strFile;
	if( 1 == nPlayType )//会议放音
	{
		if( nConNo < 0 )
		{
			nConNo = Nor32_MyGetConferenceNo(  nIndex );
			if( nConNo < 0 ){
				return FALSE;
			}
		}
		if( 0 == tpi_PlayFile( 5, nConNo, pszFileName, 0, 0 ) )
		{
			pChnlInfo->m_strPlayFile = strFile;
			pChnlInfo->m_bPlayLoop = bPlayLoop;
			pChnlInfo->m_nPlayType = nPlayType;
			pChnlInfo->m_nPlayGroup = nConNo;
			return TRUE;
		}
		else{

			return FALSE;
		}
	}
	else//通道放音
	{
		if( CHANNEL_STATE_FREE == pChnlInfo->m_nState ){
			return FALSE;
		}
		
		nConNo = Nor32_MyGetConferenceNo(  nIndex );
		if( nConNo >= 0 )
		{
			if( 0 == tpi_PlayFile( 5, nConNo, pszFileName, 0, 0 ) )
			{
				pChnlInfo->m_strPlayFile = strFile;
				pChnlInfo->m_bPlayLoop = bPlayLoop;
				pChnlInfo->m_nPlayType = nPlayType;
				pChnlInfo->m_nPlayGroup = nConNo;
				return TRUE;
			}
			else{

				return FALSE;
			}
		}
		else
		{
			if( 0 == tpi_PlayFile( channelType, channelID, pszFileName, 0, 0 ) )
			{
				pChnlInfo->m_strPlayFile = strFile;
				pChnlInfo->m_bPlayLoop = bPlayLoop;
				pChnlInfo->m_nPlayType = nPlayType;
				return TRUE;
			}
			else{

				return FALSE;
			}
		}

	}

}

BOOL CNorMarch32::Nor32_PlayExpressVox(int nIndex, CString strExpress)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CString strNewExpress("");
	if( !m_pPubCObj->PC_GetExpressValues( pChnlObj,strNewExpress,strExpress) )
	{
		return FALSE;
	}
	int nStrLen = strNewExpress.GetLength();
	if( nStrLen <= 0 ) 
	{
		return FALSE;
	}
	
	
	CString strPaht = m_strPath+"\\SysVoiceFle\\";
	CString strFileName("");
	for( int i = 0; i< nStrLen; i++ )
	{
		strFileName = "";
		if( '0' == strNewExpress.GetAt(i) )
		{
			strFileName = strPaht + "D0";
		}
		else if( '1' == strNewExpress.GetAt(i) )
		{
			strFileName = strPaht + "D1";
		}
		else if( '2' == strNewExpress.GetAt(i) )
		{
			strFileName = strPaht + "D2";
		}
		else if( '3' == strNewExpress.GetAt(i) )
		{
			strFileName = strPaht + "D3";
		}
		else if( '4' == strNewExpress.GetAt(i) )
		{
			strFileName = strPaht + "D4";
		}
		else if( '5' == strNewExpress.GetAt(i) )
		{
			strFileName = strPaht + "D5";
		}
		else if( '6' == strNewExpress.GetAt(i) )
		{
			strFileName = strPaht + "D6";
		}
		else if( '7' == strNewExpress.GetAt(i) )
		{
			strFileName = strPaht + "D7";
		}
		else if( '8' == strNewExpress.GetAt(i) )
		{
			strFileName = strPaht + "D8";
		}
		else if( '9' == strNewExpress.GetAt(i) )
		{
			strFileName = strPaht + "D9";
		}
		else if( '.' == strNewExpress.GetAt(i) )
		{
			strFileName = strPaht + "D15";
		}

		if( "" != strFileName )
		{
			if( !Nor32_MyStartPlayFile( nIndex , strFileName ) )
			{
				return FALSE;
			}
		}

	}

	return TRUE;
}

void CNorMarch32::Nor32_OnPlayEnd(int nIndex)
{
	Nor32_MyStopPlay( nIndex );
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;

	CNodeObj *pNodeObj = pChnlInfo->GetCurrentNodeObj();
	if( NULL == pNodeObj )
	{
		return;
	}
	
	if( NODETYPE_START_PLAY == pNodeObj->m_nType )
	{
		Nor32_PlayEndCheck( nIndex, (CPlayStartObj *)pNodeObj ,pChnlInfo  );
		return;
	}
	else if( NODETYPE_SEND_DTMFCODE == pNodeObj->m_nType )
	{
		Nor32_DtmfSendEndCheck( nIndex, (CDTMFSendObj *)pNodeObj );
		return;
	}
	else if( NODETYPE_CUSTOM == pNodeObj->m_nType )
	{
		Nor32_Cus_StartPlayEnd(  nIndex );
		return;
	}

	//处理处于Hold状态的通道
	switch( pChnlInfo->m_nState )
	{
	case CHANNEL_STATE_HOLD:
	case CHANNEL_STATE_TRANSFER:
		if( "" != pChnlInfo->m_strPlayFile )
		{
			Nor32_MyStartPlayFile( nIndex,pChnlInfo->m_strPlayFile);
			return;
		}
		break;
	default:
		break;
	}

	if( pChnlInfo->m_bRecordLoop )
	{
		if( 0 == pChnlInfo->m_nRecordType )//录音类型：0普通 1会议
		{
			Nor32_MyStartRecordFile( nIndex,pChnlInfo->m_strRecordFile,TRUE);
		}

	}

	if( pChnlInfo->m_bPlayLoop )
	{
		if( 0 == pChnlInfo->m_nPlayType )//录音类型：0普通 1会议
		{
			Nor32_MyStartPlayFile( nIndex,pChnlInfo->m_strPlayFile,-1,TRUE);
		}
	}
	return;
}

void CNorMarch32::Nor32_DtmfSendEndCheck(int nIndex, CDTMFSendObj *pObj )
{
	CString strMSG = "";
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDTMFSendObj) ) )
	{
		strMSG.Format("DTMF Send Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	
	Nor32_MyStopPlay( nIndex );
	Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );

	return;
}
void CNorMarch32::Nor32_PlayEndCheck( int nIndex, CPlayStartObj *pObj,CFChannelInfo *pChnlInfo  )
{
	CString strMSG = "";
	if( pChnlInfo->m_nStep <= 1 )
	{
		return;
	}

    if( pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CPlayStartObj) ) )
	{
		strMSG.Format("PlayEndCheck ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	if( pObj->m_bRecDTMF )
	{
		int nVarKey = pObj->GetVarKey( pObj->m_strSaveDTMF );
		BOOL bResult =m_pPubCObj->PC_SaveValuesToVar( nIndex, pChnlInfo, pChnlInfo->m_strDTMFRecived, nVarKey );
		if( !bResult )
		{
			strMSG.Format("%s Save DTMF to Var Error ;ChIndex = %d",pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			Nor32_MyStopPlay(nIndex);
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
	}
	Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );

	return;
}

void CNorMarch32::Nor32_DTMFMenu(int nIndex)
{
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CDTMFMenuObj *pObj = (CDTMFMenuObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDTMFMenuObj) ) )
	{
		strMSG.Format("DTMFMenu Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	int nStep = pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			/*********************/
			//Nor32_FaxKey(nIndex);
			/********************/
			if( pObj->m_bClearDTMF )
			{
				pChnlObj->m_strDTMFRecived = "";
			}
			pChnlObj->m_nState  = CHANNEL_STATE_CONNECT ; 
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
			pChnlObj->m_nStep = 1;
			m_pPubCObj->PC_StartTimer( pChnlObj );
			
		}
		break;
	case 1:
		{
			if( strlen(pChnlObj->m_strDTMFRecived) > 0 )
			{
				pChnlObj->m_nEventType = EVENTTYPE_RECDTMF;
				m_pPubCObj->PC_ShowChannelDTMF( nIndex,pChnlObj->m_strDTMFRecived);
				Nor32_MyStopPlay( nIndex );
			}

			if( m_pPubCObj->PC_ElapseTime( pChnlObj ) > pObj->m_nTimeOut )
			{
				Nor32_MyStopPlay( nIndex );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_DTMF_TIMEOUT );
				return;
			}


			if( "" == pChnlObj->m_strDTMFRecived )
				return;

			CString strCode =  pChnlObj->m_strDTMFRecived.Right(1) ;
			int nNextType = 0;
			if( "*" == strCode )
				nNextType = NEXTTYPE_DTMF_11;	
			else if( "#" == strCode )
				nNextType = NEXTTYPE_DTMF_10;
			else
				nNextType = atoi(strCode) + 60;
			if( nNextType >= 60 && nNextType <= 71 )
			{
				Nor32_MoveNext_Node( nIndex,  nNextType );
				Nor32_MyStopPlay( nIndex );
				return;
			}
		}
		break;
	default:
		break;
	}
	return;
}

void CNorMarch32::Nor32_DtmfRecive(int nIndex)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CDTMFReciveObj *pObj = (CDTMFReciveObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDTMFReciveObj) ) )
	{
		strMSG.Format("DTMF Recive Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   // if pObject is not valid
	}

	m_pPubCObj->PC_ShowChannelDTMF( nIndex,pChnlObj->m_strDTMFRecived);

	bool bError		= true;
	short int code	= -1;
	int nStep		= pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			m_pPubCObj->PC_StartTimer(pChnlObj);
			if( pObj->m_bClearDTMF ) 
			{
				pChnlObj->m_strDTMFRecived = "";
			}
			pChnlObj->m_strState = "ReciveDTMF";
			int  len = strlen(pChnlObj->m_strDTMFRecived);
			if(  pObj->m_nDTMFType > 0 )//只接收数字键
			{
				pChnlObj->m_strDTMFRecived.Replace('*',NULL);
				pChnlObj->m_strDTMFRecived.Replace('#',NULL);
			}
			if( 0 == pObj->m_nEndCondition )
			{
				//通过判断接收按键的最大数量来结束收键
				if( len  >= pObj->m_nDTMFCount )
					pChnlObj->m_nStep = 2;
				else
					pChnlObj->m_nStep = 1;

			}
			else
			{
				//通过判断接收特定按键值来结束收键
				if( pChnlObj->m_strDTMFRecived.Right(1) == pObj->m_strDTMF )
				{
					pChnlObj->m_strDTMFRecived.Replace( pObj->m_strDTMF, NULL);
					pChnlObj->m_nStep = 2;
				}
				else
				{
					pChnlObj->m_nStep = 1;
				}
			}

			
		}
		break;
	case 1:
		{
			if( strlen( pChnlObj->m_strDTMFRecived ) > 0 )
			{
				Nor32_MyStopPlay( nIndex );
				if( 0 == pObj->m_nEndCondition )
				{
					//通过判断接收按键的最大数量来结束收键
					if(  pObj->m_nDTMFType > 0 )//只接收数字键
					{
						pChnlObj->m_strDTMFRecived.Replace("*",NULL);
						pChnlObj->m_strDTMFRecived.Replace("#",NULL);
					}
					
					int  len = strlen( pChnlObj->m_strDTMFRecived );
					if( len  >= pObj->m_nDTMFCount )
					{
						BOOL bResult = m_pPubCObj->PC_SaveValuesToVar( nIndex, pChnlObj, pChnlObj->m_strDTMFRecived, pObj->m_SaveDTMF.m_nKey );
						if( bResult )
						{
							Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
						}
						else
						{
							Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
							strMSG.Format("%s Save DTMF to Var Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
							m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
						}
						return;
					}

				}
				else
				{
					//通过判断接收特定按键值来结束收键
					if( pChnlObj->m_strDTMFRecived.Right(1) == pObj->m_strDTMF )
					{
						pChnlObj->m_strDTMFRecived.Replace(pObj->m_strDTMF,NULL);
						BOOL bResult = m_pPubCObj->PC_SaveValuesToVar( nIndex, pChnlObj, pChnlObj->m_strDTMFRecived, pObj->m_SaveDTMF.m_nKey );
						if( bResult )
						{
							Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
						}
						else
						{
							Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
							strMSG.Format("%s Save DTMF to Var Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
							m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
						}
						return;
					}
					else
					{
						if(  pObj->m_nDTMFType > 0 )//只接收数字键
						{
							pChnlObj->m_strDTMFRecived.Replace("*",NULL);
							pChnlObj->m_strDTMFRecived.Replace("#",NULL);
						}
					}
				}
				pChnlObj->m_nEventType = EVENTTYPE_RECDTMF;
				
			}
			long n = m_pPubCObj->PC_ElapseTime(pChnlObj);
			if( n >= pObj->m_nTimeOut )
			{
				m_pPubCObj->PC_SaveValuesToVar( nIndex, pChnlObj, pChnlObj->m_strDTMFRecived, pObj->m_SaveDTMF.m_nKey );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				strMSG.Format("%s TimeOut Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				return;
			}
		}
		break;
	case 2:
		{
			BOOL bResult = m_pPubCObj->PC_SaveValuesToVar(pChnlObj, pChnlObj->m_strDTMFRecived, pObj->m_SaveDTMF.m_nKey );
			if( bResult )
			{
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
			else
			{
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				strMSG.Format("%s Save DTMF to Var Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			}
			
		}
	default:
		break;
	}


	return;
}

void CNorMarch32::Nor32_EndCallEventCheckBlock(int nIndex)
{
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
    if (pChnlObj->GetCurrentNodeObj() == NULL )
	{
		strMSG.Format("EndCallEventCheckBlock Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		return ;   
	}
	
	Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	return;
}

void CNorMarch32::Nor32_EndCallEventEndBlock(int nIndex)
{
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
    if( pChnlObj->GetCurrentNodeObj() == NULL )
	{
		strMSG.Format("EndCallEventEndBlock Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		return ;   
	}


	m_pPubCObj->PC_ShowChannelState( nIndex, "Free");
	m_pPubCObj->PC_ShowConnectToChnl( nIndex, -1 );

	pChnlObj->MoveFirst_Node();

	return;
}

void CNorMarch32::Nor32_DTMFSend(int nIndex)
{
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CDTMFSendObj *pObj = (CDTMFSendObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDTMFSendObj) ) )
	{
		strMSG.Format("DTMF Send Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   // if pObject is not valid
	}
	
	int nSetup		= pChnlObj->m_nStep;
	int nSS7Chnl	= pChnlObj->m_nChannel;//取得通道号
	switch( nSetup )
	{
	case 0:
		{
			int nKey = pObj->GetVarKey( pObj->m_strDTMF );
			CString strNewExpress = "";
			if( nKey < 0 )
			{
				strNewExpress = pObj->m_strDTMF;//输入的是号码串，不是变量
			}
			else
			{
				if(  !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strNewExpress, nKey ) )
				{
					Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					strMSG.Format("%s Input DTMF Express  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					return;
				}
			}
			char *pszDTMF = (LPTSTR)(LPCTSTR)strNewExpress;
			if(!Nor32_MySendDtmfStr(nIndex, pszDTMF ) )
			{
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				strMSG.Format("%s DJTrk_SendDtmfStr  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				return;
			}
			pChnlObj->m_nStep = 1;
		}
		break;
	default:
		break;
	}

	return;
}



void CNorMarch32::Nor32_ConferenceEnter(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nConferNO = -1;
	CString strMSG = "";
	CEnterConferObj *pObj = (CEnterConferObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CEnterConferObj) ) )
	{
		strMSG.Format("EnterConference error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );

		return ;  
	}


	CString strConfNO = pObj->m_strConferNO;
	int nKey = pObj->GetVarKey( strConfNO  );
	if( nKey < 0 )
	{
		nConferNO = atoi(strConfNO );
	}
	else
	{
		if(  !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strConfNO, nKey ) )
		{
			strMSG.Format("%s Input Conference index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		nConferNO = atoi( strConfNO );
	}
	
	/*if(  nConferNO < 0 || nConferNO >= m_nTotalConf )
	{
		strMSG.Format("%s: ChIndex = %d    Group error ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}*/

	int nAimIndex = -1;
	CString strChnl = pObj->m_strChnlIndex;
	int nKey1 = pObj->GetVarKey( strChnl );
	if( nKey1 < 0 )
	{
		nAimIndex = atoi( strChnl);
	}
	else
	{
		if(  !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strChnl, nKey1 ) )
		{
			strMSG.Format("%s Input Channel index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		nAimIndex = atoi( strChnl );
	}

	if( -1 == nAimIndex )
	{
		//如果 nChnl = -1表示自己运行该节点的通道加入
		nAimIndex = nIndex;
	}

	if( nAimIndex < 0  || nAimIndex >= m_nTotalChannel  ) 
	{
		strMSG.Format("%s Enter Conference  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}


	if( Nor32_MyConferEnter( nAimIndex,  nConferNO, pObj->m_nAction ) )
	{
		CString strFileName = m_strPath+"\\SysVoiceFle\\confadd.pcm";
		Nor32_MyStartPlayFile( nIndex,strFileName, nConferNO,FALSE,1);
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		return;
	}
	else
	{
		strMSG.Format("%s Enter Conference  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		pChnlObj->m_nConfNO = -1;
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	return;
}

BOOL CNorMarch32::Nor32_MyConferEnter(int nIndex, int nConferIndex, int nEnterType)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return FALSE;
	}
	if(  nConferIndex < 0 )
		return FALSE;

	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;

	Nor32_MyStopPlay( nIndex );
	if( CF_CHSWITCH_MONITOR == nEnterType )
	{
		if(  0 != tpi_ListenConference( channelType, channelID, 0, nConferIndex ) )
		{
			return FALSE;
		}
		else
		{
			pChnlInfo->m_nConfNO = nConferIndex;
			m_pPubCObj->PC_ShowChannelConferenceNO(nIndex,nConferIndex);
			return TRUE;
		}
	}
	else
	{
		if(  0 != tpi_JoinConference( channelType, channelID,0,nConferIndex, 0, 0)  )
		{
			return FALSE;
		}
		else
		{
			pChnlInfo->m_nConfNO = nConferIndex;
			m_pPubCObj->PC_ShowChannelConferenceNO(nIndex,nConferIndex);
			return TRUE;
		}
	}
	return TRUE;
}

void CNorMarch32::Nor32_CallHold(int nIndex)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) 
	{
		strMSG.Format("CallHold error : CallHold are only used by the User channel,  ChIndex = %d ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	CCallHoldObj *pObj = (CCallHoldObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CCallHoldObj) ) )
	{
		strMSG.Format("CallHold error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	
	int nDN = atoi( pChnlObj->m_strLocalDN );
	//PickUp Hold
	if( 1 == pObj->m_nCallHold ) 
	{
		Nor32_CallHold_PickUp( nIndex, pObj );//判断是否是恢复通话
		pChnlObj->m_nHoldChnl = -1;
		return;
	}

	//如果是Hold,进行下面操作
	int nChnlLink = pChnlObj->m_nLinkCh;
	if( -1 >= nChnlLink || nChnlLink >= m_nTotalChannel  )
	{
		strMSG.Format("%s Channel index is error; ChIndex = %d ", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		CTI_HoldMsg( nDN, 1);
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;	
	}


	pChnlObj->m_strDTMFRecived = "";
	Nor32_MyStopPlay( nChnlLink );
	if( !Nor32_MyClearLink( nIndex, nChnlLink) )
	{
		if( pObj->m_bPlayVox ) Nor32_MyStopPlay( nChnlLink );
		Nor32_MySetLink( nIndex, nChnlLink, CF_CHSWITCH_CONNECT );
		strMSG.Format("CallHold error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		CTI_HoldMsg( nDN, 1);
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
	}
	if( pObj->m_bPlayVox ) //看是否播放音乐
	{
		if( !Nor32_MyStartPlayFile( nChnlLink,pObj->m_strPlayFile))
		{
			if( pObj->m_bPlayVox ) Nor32_MyStopPlay( nChnlLink );
			Nor32_MySetLink( nIndex, nChnlLink , CF_CHSWITCH_CONNECT );
			strMSG.Format("CallHold error : ChIndex = %d", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			CTI_HoldMsg( nDN, 1);
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		m_pChannelInfo[nChnlLink].m_strPlayFile = pObj->m_strPlayFile;
	}
	pChnlObj->m_nHoldChnl = nChnlLink;
	pChnlObj->m_nLinkCh = -1;
	m_pChannelInfo[nChnlLink].m_nState = CHANNEL_STATE_HOLD;
	m_pPubCObj->PC_ShowChannelState( nChnlLink, "Hold" );
	CTI_HoldMsg( nDN, 0);
	Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);

	return;
}

void CNorMarch32::Nor32_CallHold_PickUp(int nIndex, CCallHoldObj *pObj)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nDN = atoi( pChnlObj->m_strLocalDN );

	int nChnlLink = -1;
	if( 1 == pChnlObj->m_nPickUpType )
	{
		nChnlLink = Nor32_FindHoldChannel( nIndex ,pChnlObj->m_strPickUpTel);
	}
	else
	{
		nChnlLink = Nor32_FindHoldChannel( nIndex);
	}
	if( 0 > nChnlLink || nChnlLink >= m_nTotalChannel  )
	{
		strMSG.Format("PickUp Hold error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 1,"","" );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	m_pChannelInfo[nChnlLink].m_nState = CHANNEL_STATE_CONNECT;
	m_pPubCObj->PC_ShowChannelState( nChnlLink, "Busy");
	Nor32_MyStopPlay(nIndex);
	Nor32_MyStopPlay(nChnlLink);
	if( !Nor32_MySetLink( nIndex, nChnlLink , CF_CHSWITCH_CONNECT ) )
	{
		strMSG.Format("PickUp Hold error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 1,"","" );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	pChnlObj->m_strInboundCallPK = m_pChannelInfo[nChnlLink].m_strInboundCallPK ;
	pChnlObj->m_strCallerID = m_pChannelInfo[nChnlLink].m_strCallerID ;
	pChnlObj->m_strCalledID = m_pChannelInfo[nChnlLink].m_strCalledID ;

	pChnlObj->m_nLinkCh = nChnlLink;
	m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 0 ,m_pChannelInfo[nChnlLink].m_strCallerID ,m_pChannelInfo[nChnlLink].m_strCalledID);
	Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	return;
}
BOOL CNorMarch32::Nor32_MySetLink(int nIndex1, int nIndex2, int nSwitchType)
{
	if( 0 > nIndex1 || nIndex1 >= m_nTotalChannel  ){
		return FALSE;
	}
	if( 0 > nIndex2 || nIndex2 >= m_nTotalChannel  ){
		return FALSE;
	}
	CFChannelInfo *pChnlInfo1 = &(m_pChannelInfo[nIndex1]);
	int channelType1 = Nor32_GetDevChnlType( pChnlInfo1->m_nChanelType );
	int channelID1 = pChnlInfo1->m_nChannel ;

	CFChannelInfo *pChnlInfo2 = &(m_pChannelInfo[nIndex2]);
	int channelType2 = Nor32_GetDevChnlType( pChnlInfo2->m_nChanelType );
	int channelID2 = pChnlInfo2->m_nChannel ;
	Nor32_MyStopPlay( nIndex1 );
	Nor32_MyStopPlay( nIndex2 );

	if( CF_CHSWITCH_CONNECT == nSwitchType )
	{
		if(1 == m_nSwitchMode)//会议模式
		{
			int nGroup = Nor32_MyGetConferenceNo( nIndex1 );
			if( nGroup < 0 )
			{
				nGroup = tpi_CreateConference();
			}
			if( nGroup < 0 )
			{
				return FALSE;
			}
			Nor32_MyJoinConference( nIndex1, nGroup );
			Nor32_MyJoinConference( nIndex2, nGroup );

			m_pChannelInfo[nIndex1].m_nLinkType = nSwitchType;
			m_pChannelInfo[nIndex2].m_nLinkType = nSwitchType;
			m_pPubCObj->PC_ShowConnectToChnl(nIndex1,nIndex2);
			m_pPubCObj->PC_ShowConnectToChnl(nIndex2,nIndex1);
			return TRUE;
		}
		else
		{
			//普通模式
			if( 0 == tpi_TalkWith( channelType2, channelID2, channelType1, channelID1 ) )
			{
				//建立通话的双方,如果正在与其他通道通话，也会被拆除，如果正在某组会议内，也
				//会被退出。

				m_pChannelInfo[nIndex1].m_nLinkType = nSwitchType;
				m_pChannelInfo[nIndex2].m_nLinkType = nSwitchType;
				m_pPubCObj->PC_ShowConnectToChnl(nIndex1,nIndex2);
				m_pPubCObj->PC_ShowConnectToChnl(nIndex2,nIndex1);
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}

	}
	else if( CF_CHSWITCH_MONITOR == nSwitchType )
	{
		if(1 == m_nSwitchMode)//会议模式
		{
			int nGroup = Nor32_MyGetConferenceNo( nIndex2 );
			if( nGroup < 0 )
			{
				nGroup = tpi_CreateConference();
				if( nGroup < 0 )
				{
					return FALSE;
				}
				Nor32_MyJoinConference( nIndex2, nGroup );
			}
			
			Nor32_MyListenConference( nIndex1, nGroup );
			m_pChannelInfo[nIndex1].m_nLinkType = nSwitchType;
			m_pChannelInfo[nIndex2].m_nLinkType = nSwitchType;
			m_pPubCObj->PC_ShowConnectToChnl(nIndex1,nIndex2);
			m_pPubCObj->PC_ShowConnectToChnl(nIndex2,nIndex1);
			return TRUE;
		}
		else
		{
			if( 0 == tpi_ListenTo( channelType2, channelID2, channelType1, channelID1 ) )
			{
				//建立单向交换，不影响源通道，不改变源通道的任何状态，
				//但对于目的通道，如果正在与其他通道通话，
				//也会被拆除，如果正在某组会议内，也会被退出
				m_pChannelInfo[nIndex1].m_nLinkType = nSwitchType;
				m_pChannelInfo[nIndex2].m_nLinkType = nSwitchType;
				m_pPubCObj->PC_ShowConnectToChnl(nIndex1,nIndex2);
				m_pPubCObj->PC_ShowConnectToChnl(nIndex2,nIndex1);
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}

	}
	else if( CF_CHSWITCH_SPEAK == nSwitchType )
	{

		if( 0 == tpi_ListenTo( channelType1, channelID1, channelType2, channelID2 ) )
		{
			m_pChannelInfo[nIndex1].m_nLinkType = nSwitchType;
			m_pChannelInfo[nIndex2].m_nLinkType = nSwitchType;
			m_pPubCObj->PC_ShowConnectToChnl(nIndex1,nIndex2);
			m_pPubCObj->PC_ShowConnectToChnl(nIndex2,nIndex1);
			return TRUE;
		}
		else
		{
			return FALSE;
		}

	}

	return FALSE;

}
BOOL CNorMarch32::Nor32_MyClearLink(int nIndex1, int nIndex2)
{
	if( 0 > nIndex1 || nIndex1 >= m_nTotalChannel  ){
		return FALSE;
	}
	if( 0 > nIndex2 || nIndex2 >= m_nTotalChannel  ){
		return FALSE;
	}
	CFChannelInfo *pChnlInfo1 = &(m_pChannelInfo[nIndex1]);
	int channelType1 = Nor32_GetDevChnlType( pChnlInfo1->m_nChanelType );
	int channelID1 = pChnlInfo1->m_nChannel ;

	CFChannelInfo *pChnlInfo2 = &(m_pChannelInfo[nIndex2]);
	int channelType2 = Nor32_GetDevChnlType( pChnlInfo2->m_nChanelType );
	int channelID2 = pChnlInfo2->m_nChannel ;

	int nLinkResult = 0;
	int nSwitchType = m_pChannelInfo[nIndex1].m_nLinkType;
	if( CF_CHSWITCH_MONITOR == nSwitchType || CF_CHSWITCH_SPEAK == nSwitchType )
	{
		if(1 == m_nSwitchMode)//会议模式
		{
			Nor32_MyExitConference( nIndex1 );
		}
		else
		{
			tpi_BreakListen( channelType1, channelID1 );
			tpi_BreakListen( channelType2, channelID2 );
		}

		m_pChannelInfo[nIndex1].m_nLinkType = CF_CHSWITCH_DISCONNECT;
		m_pChannelInfo[nIndex2].m_nLinkType = CF_CHSWITCH_DISCONNECT;
		m_pPubCObj->PC_ShowConnectToChnl(nIndex1,-1);
		m_pPubCObj->PC_ShowConnectToChnl(nIndex2,-1);
		return TRUE;
	}
	else 
	{
		if(1 == m_nSwitchMode)//会议模式
		{
			Nor32_MyExitConference( nIndex1 );
		}
		else
		{
			tpi_BreakTalk( channelType1, channelID1 );
			tpi_BreakTalk( channelType2, channelID2 );
		}

		int n1 = tpi_GetChannelConferenceHandle( channelType1,  channelID1 );
		int n2 = tpi_GetChannelConferenceHandle( channelType1,  channelID1 );

		m_pChannelInfo[nIndex1].m_nLinkType = CF_CHSWITCH_DISCONNECT;
		m_pChannelInfo[nIndex2].m_nLinkType = CF_CHSWITCH_DISCONNECT;
		m_pPubCObj->PC_ShowConnectToChnl(nIndex1,-1);
		m_pPubCObj->PC_ShowConnectToChnl(nIndex2,-1);
		return TRUE;
	}

	return TRUE;
}

void CNorMarch32::Nor32_AnswerCall(int nIndex)
{
	Nor32_MyOFFHook( nIndex);
	Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
	return;
}

void CNorMarch32::Nor32_MyOFFHook(int nIndex)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nTChnl = pChnlObj->m_nChannel;
	int nChnlType = Nor32_GetDevChnlType( pChnlObj->m_nChanelType );
	tpi_Answer( nChnlType, nTChnl, 1);
	return;
}

void CNorMarch32::Nor32_WaitCall(int nIndex)
{
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nType = pChnlObj->m_nChanelType;
	if( CF_CHTYPE_USER == nType || CF_CHTYPE_EMPTY == nType ) 
	{
		strMSG.Format("Error: WaitCall are only used by the trunk channel; ChIndex = %d ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	CWaitCallObj *pObj = (CWaitCallObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CWaitCallObj) ) )
	{
		strMSG.Format("ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	int nStep = pChnlObj->m_nStep;
	switch (nStep)
	{
	case 0:
		{
			if( pObj->m_bClearDTMF ) 
				pChnlObj->m_strDTMFRecived	= "" ;
			pChnlObj->m_nRings	= 1;
			pChnlObj->m_nStep	= 1;
			
			m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing" );
			pChnlObj->m_nState		= CHANNEL_STATE_RINGING;
			m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
			m_pPubCObj->PC_StartTimer( pChnlObj );
		}
		break;
	case 1:
		{
			if( m_pPubCObj->PC_ElapseTime(pChnlObj) > 15  )//每个回铃音为5秒
			{
				Nor32_ResetChannel( nIndex );
			}
		}
	default:
		break;
	}
	return;

}

void CNorMarch32::Nor32_ReleaseCall( int nIndex)
{

	Nor32_ResetChannel( nIndex );
	return;
}


void CNorMarch32::Nor32_FaxSend(int nIndex)
{
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CFaxSendObj *pObj = (CFaxSendObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CFaxSendObj) ) )
	{
		strMSG.Format("Send Fax error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}

	if( 1 != m_nFax )
	{
		strMSG.Format("Send Fax error : ChIndex = %d   Error = The system not support FAx ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}


	int nStep	= pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			CString strFile("");
			BOOL bResult = m_pPubCObj->PC_GetExpressValues( pChnlObj, strFile, pObj->m_strFileName);
			if( !bResult  )
			{
				strMSG.Format("Fax Send Error; ChIndex = %d   Error = File name error", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;   	
			}

			strFile = pObj->m_strFolder + strFile;
			if( !Nor32_MyFaxSend( nIndex, (LPCTSTR)strFile ) )
			{
				strMSG.Format("Fax Send Error; ChIndex = %d ", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;   	
			}

			pChnlObj->m_nStep = 1;
		}
	default:
		break;
	}

	return;
}

void CNorMarch32::Nor32_FaxRecive(int nIndex)
{
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CFaxReciveObj *pObj = (CFaxReciveObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CFaxReciveObj) ) )
	{
		strMSG.Format("Recive Fax error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}
	if( 1 != m_nFax )
	{
		strMSG.Format("Recive Fax error : ChIndex = %d   Error = The system not support FAx ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}

	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	int nStep = pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{

			CString strFile("");
			BOOL bResult = m_pPubCObj->PC_GetExpressValues( pChnlObj, strFile, pObj->m_strFileName);
			if( !bResult || -1 == pObj->GetVarKey( pObj->m_strPage)  )
			{
				strMSG.Format("FaxRecive Error; ChIndex = %d   Error = File name error", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;   	
			}
			strFile = pObj->m_strFolder + strFile;
					
			if(!Nor32_MyFaxReceive( nIndex, (LPCTSTR)strFile  ))
			{
				strMSG.Format("%s: ChIndex = %d  Receive Fax error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

			pChnlObj->m_strReciveFaxFile = strFile;
			pChnlObj->m_nStep = 1;

		}
		break;
	default:
		break;

	}

	return;
}




void CNorMarch32::Nor32_StartRecord(int nIndex)
{
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CRecordObj *pObj = (CRecordObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CRecordObj) ) )
	{
		strMSG.Format("Record Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	int nStep = pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			pChnlObj->m_strDTMFRecived = "";
			Nor32_MyStopRecord(  nIndex );
			pChnlObj->m_nStep = 1;
		}
		break;
	case 1:
		{

			int nGroupNo = 0;
			int nVarKey = pObj->GetVarKey( pObj->m_strConferNO  );
			if( nVarKey < 0 )
			{
				nGroupNo = atoi(pObj->m_strConferNO );
			}
			else
			{
				if( !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strMSG, pObj->GetVarKey( pObj->m_strConferNO ) ) )
				{
					strMSG.Format("%s: ChIndex = %d    Play error ",  pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				nGroupNo = atoi(strMSG);
				Nor32_MyStopConfRecord( nGroupNo );
			}

			CString strFile("");
			BOOL bResult = m_pPubCObj->PC_GetExpressValues(pChnlObj, strFile, pObj->m_strFileName);
			if( !bResult )
			{
				strMSG.Format("Record Error; ChIndex = %d   Error = File name error", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;   	
			}
			strFile = pObj->m_strFolder + strFile;
			pChnlObj->m_strRecordFile = strFile;
			if( !Nor32_MyStartRecordFile( nIndex, strFile,FALSE, pObj->m_nRecordType,pObj->m_nFileLength,nGroupNo ) )
			{
				strMSG.Format("Record Error; ChIndex = %d   Error = Start Record error File=%s", nIndex,strFile );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;   	
			}
			if(  pObj->m_nFileLength <= 0 )
			{
				//录音直到挂机停止
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
			else
			{
				pChnlObj->m_nStep = 2;
			}

		}
		break;
	case 2:
		if( strlen(pChnlObj->m_strDTMFRecived) > 0 )
		{
			m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlObj->m_strDTMFRecived );
			if( pChnlObj->m_strDTMFRecived == pObj->m_strEndDTMF  )
			{
				Nor32_MyStopRecord( nIndex );
				//清除录音文件末尾的DTMF
				m_pPubCObj->PC_ClearVoxFileEndDTMF( pChnlObj->m_strRecordFile);
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
		}
		break;
	default:
		break;

	}

	return;

}



void CNorMarch32::Nor32_OnRecordEnd(int nIndex)
{
	Nor32_MyStopRecord( nIndex );
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;

	CNodeObj *pNodeObj = pChnlInfo->GetCurrentNodeObj();
	if( NULL == pNodeObj ){
		return;
	}
	m_pPubCObj->PC_ClearVoxFileEndDTMF( pChnlInfo->m_strRecordFile);
	if( NODETYPE_START_RECORD == pNodeObj->m_nType )
	{
		if( pChnlInfo->m_nStep < 2 ){
			return;
		}
		
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		return;
	}

	return;
}



void CNorMarch32::Nor32_Switch(int nIndex)
{
	CString strMSG = "";
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CSwitchObj *pObj = (CSwitchObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CSwitchObj) ) )
	{
		strMSG.Format("Switch Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	
	
	/***********************************/
	//TMS_TransferKey( nIndex );//加密设置
   /*************************************/
	
	
	switch( pObj->m_nSwitchType )
	{
		case CF_CHSWITCH_CONNECT://直接连接
		case CF_CHSWITCH_MONITOR://监听
		case CF_CHSWITCH_SPEAK://别人能听见，但听不见别人
		case CF_CHSWITCH_DISCONNECT://断开连接	
			if( 0 == pObj->m_nAimType )
				Nor32_Switch_Self( nIndex,  pObj );
			else
				Nor32_Switch_Aim( nIndex,  pObj );
			break;
		case CF_CHSWITCH_CALLAGENT://呼叫坐席，这时的通道号为分机号码
			if(  0 == pChnlInfo->m_nStep  )
			{
				if( 3 == pObj->m_nAimType )//呼叫空闲坐席
				{
					Nor32_Switch_CallFreeAgent( nIndex,  pObj );
				}
				else
				{
					Nor32_Switch_CallAgent( nIndex,  pObj );
				}
			}
			else
			{
				Nor32_Switch_TimeOut( nIndex,  pObj );
			}
			break;
		case CF_CHSWITCH_CALLCUSTOMER://呼叫客户
			if(  0 == pChnlInfo->m_nStep  )
				Nor32_Switch_CallCustomer( nIndex,  pObj );
			else
				Nor32_Switch_TimeOut( nIndex,  pObj );
			break;
		default:
			break;
	}
	


	return;
}
/********************************************************
与指定通道进行行交换
3.	当Switch Type是其他类型时，这时目标可能是外线通道号、
内线通道号或分机号码，完成目标通道的交换。
*********************************************************/
void CNorMarch32::Nor32_Switch_Aim(int nIndex, CSwitchObj *pObj)
{
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nVarKey = pObj->GetVarKey( pObj->m_strAimChnl );
	if(  nVarKey < 0 )
	{
		strMSG = pObj->m_strAimChnl;
	}
	else if( !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strMSG, nVarKey ) )
	{	
		strMSG.Format("%s Channel index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	int nAimIndex = atoi(strMSG);
	if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
	{
		strMSG.Format("%s Channel index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	int nSwitchType = pObj->m_nSwitchType;
	switch( nSwitchType )
	{
		case CF_CHSWITCH_CONNECT://直接连接
		case CF_CHSWITCH_MONITOR://监听
		case CF_CHSWITCH_SPEAK://别人能听见，但听不见别人
			if( Nor32_MySetLink(nIndex, nAimIndex, pObj->m_nSwitchType  ) )
			{
				pChnlObj->m_nLinkCh = nAimIndex;
				if( CF_CHSWITCH_MONITOR != pObj->m_nSwitchType)
				{
					m_pChannelInfo[nAimIndex].m_nLinkCh = nIndex;
					pChnlObj->m_nState = CHANNEL_STATE_CONNECT;
				}
				else
				{
					pChnlObj->m_nState = CHANNEL_STATE_MONITORING;
				}
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				return;
			}
			else
			{
				strMSG.Format("%s Link Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			break;
		case CF_CHSWITCH_DISCONNECT://断开连接	
			if( Nor32_MyClearLink(nIndex, nAimIndex ) )
			{
				pChnlObj->m_nLinkType = -1;
				pChnlObj->m_nLinkCh = -1;
				m_pChannelInfo[nAimIndex].m_nLinkType = -1;
				m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
				m_pPubCObj->PC_ShowConnectToChnl(nIndex,-1);
				m_pPubCObj->PC_ShowConnectToChnl(nAimIndex,-1);
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
			else
			{
				strMSG.Format("%s Channel index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}
			break;
		default:
			break;
	}
	return;	

}

void CNorMarch32::Nor32_Switch_Self(int nIndex, CSwitchObj *pObj)
{
	if( nIndex < 0 || nIndex >= m_nTotalChannel  ) return;
	CString strMSG = "";
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nAimIndex = m_pChannelInfo[nIndex].m_nLinkCh;
	if( nIndex < 0 || nIndex >= m_nTotalChannel  )
	{
		strMSG.Format("%s have not the Channel that link to self . index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	int nSwitchType = pObj->m_nSwitchType;
	switch( nSwitchType )
	{
		case CF_CHSWITCH_CONNECT://直接连接
		case CF_CHSWITCH_MONITOR://监听
		case CF_CHSWITCH_SPEAK://别人能听见，但听不见别人
			Nor32_MySetLink(nIndex, nAimIndex, pObj->m_nSwitchType  );
			break;
		case CF_CHSWITCH_DISCONNECT://断开连接	
			Nor32_MyClearLink(nIndex, nAimIndex );
			pChnlInfo->m_nLinkType = -1;
			pChnlInfo->m_nLinkCh = -1;
			m_pChannelInfo[nAimIndex].m_nLinkType = -1;
			m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
			m_pPubCObj->PC_ShowConnectToChnl(nIndex,-1);
			m_pPubCObj->PC_ShowConnectToChnl(nAimIndex,-1);
			break;
		default:
			break;
	}

	Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	//m_pPubCObj->PC_StartTimer( pChnlInfo );
	//pChnlInfo->m_nStep = 1;
	return;
}

void CNorMarch32::Nor32_Switch_CallAgent(int nIndex, CSwitchObj *pObj)
{
	CString strMSG = "";
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if(  pChnlInfo->m_nStep > 0 ) return;
	int nAimIndex	= pChnlInfo->m_nLinkCh ;
	int nChType		= pChnlInfo->m_nChanelType;

	CString strCallTel("");
	int nVarKey = pObj->GetVarKey( pObj->m_strAimChnl );
	if( nVarKey < 0 )
	{
		strCallTel = pObj->m_strAimChnl;
	}
	else if( !m_pPubCObj->PC_GetNorVarValues( pChnlInfo, strCallTel, nVarKey) )
	{
		strMSG.Format("%s not get aim telephone; ChIndex = %d ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1, "");
		}
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	nAimIndex = Nor32_FindFreeChannel( strCallTel, CF_CHTYPE_USER );
	if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
	{
		strMSG.Format("%s Channel index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1, "");
		}
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	CFChannelInfo *pAimChnlInfo = &(m_pChannelInfo[nAimIndex]);
	if( CF_CHTYPE_USER != pAimChnlInfo->m_nChanelType )
	{
		strMSG.Format("%s Aim Channel not is User channel, index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1, "");
		}
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	pChnlInfo->m_nLinkCh = nAimIndex;

	//呼叫分机空闲，对其振铃
	pAimChnlInfo->m_nLinkCh		= nIndex;
	if( CF_CHTYPE_USER == nChType)
	{
		Nor32_MyMakeCall( nAimIndex, "", (LPTSTR)(LPCTSTR)pChnlInfo->m_strLocalDN, (LPTSTR)(LPCTSTR)pAimChnlInfo->m_strLocalDN );
	}
	else
	{
		Nor32_MyMakeCall( nAimIndex, pChnlInfo->m_strInboundCallPK,(LPTSTR)(LPCTSTR)pChnlInfo->m_strCallerID, (LPTSTR)(LPCTSTR)pChnlInfo->m_strCalledID );
	}


	//switch的等待选项，一般只用于呼叫时用
	if( 0 == pObj->m_nWaitOption )//播放回铃音
	{
		Nor32_MyPlaySignal(nIndex, NOR32_RINGBACK_SIG);
	}
	else if( 2 == pObj->m_nWaitOption )//播放语音文件
	{
		
		Nor32_MyStartPlayFile( nIndex , pObj->m_strPlayFile);
	}

	m_pPubCObj->PC_StartTimer( pChnlInfo );
	pChnlInfo->m_nStep = 1;
	
	
	return;
}

void CNorMarch32::Nor32_Switch_CallFreeAgent(int nIndex, CSwitchObj *pObj)
{
	CString strMSG = "";
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if(  pChnlInfo->m_nStep > 0 ) return;
	int nAimIndex	= pChnlInfo->m_nLinkCh ;
	int nChType		= pChnlInfo->m_nChanelType;

	CString strCallTel("");
	int nVarKey = pObj->GetVarKey( pObj->m_strAimChnl );
	if( nVarKey < 0 )
	{
		strCallTel = pObj->m_strAimChnl;
	}
	else if( !m_pPubCObj->PC_GetNorVarValues( pChnlInfo, strCallTel, nVarKey) )
	{
		strMSG.Format("%s not get aim telephone; ChIndex = %d ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1, "");
		}
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	nAimIndex = Nor32_FindFreeChannel( strCallTel, CF_CHTYPE_USER );
	if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
	{
		strMSG.Format("%s Channel index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1, "");
		}
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	CFChannelInfo *pAimChnlInfo = &(m_pChannelInfo[nAimIndex]);
	if( CF_CHTYPE_USER != pAimChnlInfo->m_nChanelType )
	{
		strMSG.Format("%s Aim Channel not is User channel, index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1, "");
		}
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	pChnlInfo->m_nLinkCh = nAimIndex;

	//呼叫分机空闲，对其振铃
	pAimChnlInfo->m_nLinkCh		= nIndex;
	if( CF_CHTYPE_USER == nChType)
	{
		Nor32_MyMakeCall( nAimIndex, "",(LPTSTR)(LPCTSTR)pAimChnlInfo->m_strLocalDN, (LPTSTR)(LPCTSTR)pChnlInfo->m_strLocalDN );
	}
	else
	{
		Nor32_MyMakeCall( nAimIndex,pChnlInfo->m_strInboundCallPK, (LPTSTR)(LPCTSTR)pChnlInfo->m_strCalledID, (LPTSTR)(LPCTSTR)pChnlInfo->m_strCallerID );
	}


	//switch的等待选项，一般只用于呼叫时用
	if( 0 == pObj->m_nWaitOption )//播放回铃音
	{
		Nor32_MyPlaySignal(nIndex, NOR32_RINGBACK_SIG);
	}
	else if( 2 == pObj->m_nWaitOption )//播放语音文件
	{
		
		Nor32_MyStartPlayFile( nIndex , pObj->m_strPlayFile);
	}

	m_pPubCObj->PC_StartTimer( pChnlInfo );
	pChnlInfo->m_nStep = 1;
	
		
	return;
}

void CNorMarch32::Nor32_Switch_CallCustomer(int nIndex, CSwitchObj *pObj)
{
	CString strMSG = "";
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if(  pChnlInfo->m_nStep > 0 ) return;
	int nAimIndex	= pChnlInfo->m_nLinkCh ;
	CString strCallTel("");
	int nVarKey = pObj->GetVarKey( pObj->m_strAimChnl );
	if( nVarKey < 0 )
	{
		strCallTel = pObj->m_strAimChnl;
	}
	else if( !m_pPubCObj->PC_GetNorVarValues( pChnlInfo, strCallTel, nVarKey) )
	{
		strMSG.Format("%s not get aim telephone; ChIndex = %d ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		if( CF_CHTYPE_USER == pChnlInfo->m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(pChnlInfo->m_strLocalDN), 1, "");
		}
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}


	//取得外拨的外线通道
	if( pObj->m_nGetFreeChWay > 0 )
	{

		int nBeginCH = 0;
		int nEndCH = 0;
		CString strNewExpress("");
		if(  !m_pPubCObj->PC_GetExpressValues( pChnlInfo,  strNewExpress, pObj->m_strBeginCH ) )
		{
			strMSG.Format("%s Get Begin Channel error ; ChIndex = %d ",pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;

		}
		nBeginCH = atoi(strNewExpress);
		if(  !m_pPubCObj->PC_GetExpressValues( pChnlInfo,  strNewExpress, pObj->m_strEndCH ) )
		{
			strMSG.Format("%s Get End Channel error ; ChIndex = %d ",pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;

		}
		nEndCH = atoi(strNewExpress);
		nAimIndex = Nor32_FindFreeTrunkChnl( nBeginCH, nEndCH );
	}
	else
	{
		nAimIndex = Nor32_FindFreeTrunk();
	}
	if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
	{
		strMSG.Format("%s have not free channel ; ChIndex = %d ",pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		if( CF_CHTYPE_USER == pChnlInfo->m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(pChnlInfo->m_strLocalDN), 1, "");
		}
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	
	CFChannelInfo *pAimChnlInfo = &(m_pChannelInfo[nAimIndex]);

	if( CF_CHTYPE_USER == pChnlInfo->m_nChanelType )
	{
		pChnlInfo->m_strInboundCallPK.Format("%s%d", (CTime::GetCurrentTime()).Format("%Y%m%d%H%M%S") , nIndex );
		pChnlInfo->m_strCalledID = pAimChnlInfo->m_strLocalDN;
		pChnlInfo->m_strCallerID = strCallTel;
		
	}
	else
	{
		pChnlInfo->m_strCalledID = pAimChnlInfo->m_strLocalDN;
		pChnlInfo->m_strCallerID = strCallTel;
	}
	Nor32_MyMakeCall( nAimIndex,pChnlInfo->m_strInboundCallPK, (LPTSTR)(LPCTSTR)strCallTel, (LPTSTR)(LPCTSTR)pAimChnlInfo->m_strLocalDN  );
	pAimChnlInfo->m_nLinkCh = nIndex;
	pAimChnlInfo->m_strDialTel		= strCallTel;
	pAimChnlInfo->m_nDialType		= 1;//1外拨
	m_pPubCObj->PC_ShowChannelState( nAimIndex, "Dial" );


	//switch的等待选项，一般只用于呼叫时用
	if( 0 == pObj->m_nWaitOption )//播放回铃音
	{
		Nor32_MyPlaySignal(nIndex, NOR32_RINGBACK_SIG);
	}
	else if( 2 == pObj->m_nWaitOption )//播放语音文件
	{
		Nor32_MyStartPlayFile( nIndex , pObj->m_strPlayFile);
	}

	m_pPubCObj->PC_StartTimer( pChnlInfo );
	pChnlInfo->m_strDialTel		= strCallTel;
	pChnlInfo->m_nDialType		= 1;//1外拨
	pChnlInfo->m_nLinkCh = nAimIndex;
	pChnlInfo->m_nStep = 1;

	return;
}



BOOL CNorMarch32::Nor32_MyMakeCall(int nIndex, CString strCallPK, char *szCallerID, char *szCalledID)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return FALSE;
	}
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;

	m_pPubCObj->PC_StartTimer(pChnlInfo);
	int nResult = 0;
	if( CF_CHTYPE_USER == pChnlInfo->m_nChanelType  )
	{
		
		nResult = tpi_MakeCall( channelType, channelID, szCallerID, "FSK", 30000 );
	}
	else
	{
		
		nResult = tpi_MakeCallEx( channelType, channelID, szCalledID, szCallerID,100,80,180, 30000 );
	}
	if( 0 != nResult )
	{
		//失败
		return FALSE;
	}

	pChnlInfo->m_strDTMFRecived	= "";
	pChnlInfo->m_strInboundCallPK		= strCallPK;
	pChnlInfo->m_strCallerID	= szCallerID ;
	pChnlInfo->m_strCalledID	= szCalledID ;
	if( CF_CHTYPE_USER == pChnlInfo->m_nChanelType  )
	{
		//呼叫分机空闲，对其振铃
		pChnlInfo->m_nCallType = CHANNEL_CALLTYPE_IN;
		pChnlInfo->m_nState		= CHANNEL_STATE_RINGING;
		pChnlInfo->m_nEventType =  EVENTTYPE_RINGING;

		m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
		m_pPubCObj->PC_CTI_RingingMsg( atoi( pChnlInfo->m_strLocalDN), szCallerID, szCalledID );
	}
	else
	{
		pChnlInfo->m_nCallType = CHANNEL_CALLTYPE_CALLOUT;
		pChnlInfo->m_nState		= CHANNEL_STATE_DIAL;
		pChnlInfo->m_nEventType	=  EVENTTYPE_OFFHOOK;
		m_pPubCObj->PC_ShowChannelState( nIndex, "Dial" );
	}

	return TRUE;
}

int CNorMarch32::Nor32_FindFreeChannel(int nChnlType)
{
	static int nCurrentTrunkNo = 0 ;
	int i = 0;
	for( i = nCurrentTrunkNo ; i < m_nTotalChannel ; i++ ) 
	{
		if( nChnlType == m_pChannelInfo[i].m_nChanelType && CHANNEL_STATE_FREE == m_pChannelInfo[i].m_nState )
		{

			nCurrentTrunkNo = i+1 ;
			return i;

		}
	}
	
	for( i = 0 ; i < nCurrentTrunkNo ; i++ )
	{
		if( nChnlType == m_pChannelInfo[i].m_nChanelType && CHANNEL_STATE_FREE == m_pChannelInfo[i].m_nState )
		{

			nCurrentTrunkNo = i+1 ;
			return i;

		}
	}

	return -1 ;
}

int CNorMarch32::Nor32_FindFreeChannel(CString strLocalDN, int nChType)
{
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( nChType == m_pChannelInfo[i].m_nChanelType )
		{
			if( strLocalDN == m_pChannelInfo[i].m_strLocalDN )
				if( CHANNEL_STATE_FREE == m_pChannelInfo[i].m_nState )
					return i;
		}
	}
	return -1;
}
/**********************************************************
  NOR32_DIAL_SIG     1 //拨号音
  NOR32_RINGBACK_SIG 2 //回铃音
  NOR32_BUSY_SIG     3 //忙音
**********************************************************/
void CNorMarch32::Nor32_MyPlaySignal(int nIndex, int nSigType)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;
	switch( nSigType )
	{
	case NOR32_DIAL_SIG:
		tpi_PlayTone( channelType, channelID, 450.0, 30000, 180, "100");
		break;
	case NOR32_RINGBACK_SIG:
		tpi_PlayTone( channelType, channelID, 450.0, 60000, 180, "1000,4000");
		break;
	case NOR32_BUSY_SIG:
		tpi_PlayTone( channelType, channelID, 450.0, 30000, 180,"350,350");
		break;
	default:
		tpi_PlayTone( channelType, channelID, 450.0, 30000, 180,"350,350");
		break;
	}

	return;
}

int CNorMarch32::Nor32_FindFreeTrunkChnl(int nBeginCH, int nEndCH)
{
	if(  0 > nBeginCH || nBeginCH >= m_nTotalChannel )
	{
		m_pPubCObj->PC_ShowCommMsg( "The choice channel scope is invalid" );
		return -1;
	}

	if(  0 > nEndCH || nEndCH >= m_nTotalChannel )
	{
		m_pPubCObj->PC_ShowCommMsg( "The choice channel scope is invalid" );
		return -1;
	}

	static int nCurrentTrunkNo = 0 ;
	if( nCurrentTrunkNo < nBeginCH )
	{
		nCurrentTrunkNo = nBeginCH;
	}
	if( nCurrentTrunkNo >= nEndCH )
	{
		nCurrentTrunkNo = nBeginCH;
	}
	int i = 0;
	for( i = nCurrentTrunkNo ; i < nEndCH ; i++ ) 
	{
		switch( m_pChannelInfo[i].m_nChanelType )
		{
			case CF_CHTYPE_DTNO1://数字中继中国一号信令 
			case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
			case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
			case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
			case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
				if( CHANNEL_STATE_FREE == m_pChannelInfo[i].m_nState )
				{
					nCurrentTrunkNo = i+1;
					return i;
				}
				break;
			case CF_CHTYPE_NOKNOW://未知通道类型 
			case CF_CHTYPE_EMPTY://空通道 
			case CF_CHTYPE_MTP3://七号信令MTP传输层通道
			case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
			case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
			case CF_CHTYPE_VIP:
			default:
				break;
		}

	}
	

	return -1 ;
}

int CNorMarch32::Nor32_FindFreeTrunk()
{
	static int nCurrentTrunkNo = 0 ;
	int i = 0;
	for( i = nCurrentTrunkNo ; i < m_nTotalChannel ; i++ ) 
	{
		switch( m_pChannelInfo[i].m_nChanelType )
		{
			case CF_CHTYPE_DTNO1://数字中继中国一号信令 
			case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
			case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
			case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
			case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
			case CF_CHTYPE_MTP3://七号信令MTP传输层通道
			case CF_CHTYPE_VIP:
				if( CHANNEL_STATE_FREE == m_pChannelInfo[i].m_nState )
				{
					nCurrentTrunkNo = i+1 ;
					return i;
				}
				break;
			case CF_CHTYPE_NOKNOW://未知通道类型 
			case CF_CHTYPE_EMPTY://空通道 
			case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
			case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
			default:
				break;
		}

	}
	
	for( i = 0 ; i < nCurrentTrunkNo ; i++ )
	{
		switch( m_pChannelInfo[i].m_nChanelType )
		{
			case CF_CHTYPE_DTNO1://数字中继中国一号信令 
			case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
			case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
			case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
			case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
			case CF_CHTYPE_MTP3://七号信令MTP传输层通道
			case CF_CHTYPE_VIP:
				if( CHANNEL_STATE_FREE == m_pChannelInfo[i].m_nState )
				{
					nCurrentTrunkNo = i+1 ;
					return i;
				}
				break;
			case CF_CHTYPE_NOKNOW://未知通道类型 
			case CF_CHTYPE_EMPTY://空通道 
			case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
			case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
			default:
				break;
		}

	}

	return -1 ;
}

void CNorMarch32::Nor32_Switch_TimeOut(int nIndex, CSwitchObj *pObj)
{
	CString strMSG = "";
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nAimIndex	=  pChnlInfo->m_nLinkCh ;
	if( 0 > nAimIndex || nAimIndex >= m_nTotalChannel  ) return;
	long n = m_pPubCObj->PC_ElapseTime( pChnlInfo);
	if( n > pObj->m_nTimeOut )
	{
		//超时或失败
		m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
		Nor32_ResetChannel( nAimIndex );
		m_pChannelInfo[nIndex].m_nLinkCh = -1;
		Nor32_MyPlaySignal(  nIndex, NOR32_BUSY_SIG );
		strMSG.Format("%s Switch Time out fail ; ChIndex = %d ",pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );

		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1, "");
		}
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );

	}

	return;
}



void CNorMarch32::Nor32_Switch_Judge(int nIndex, BOOL bResult )
{
	int nLinkIndex = m_pChannelInfo[nIndex].m_nLinkCh;
	if( -1 >= nLinkIndex || nLinkIndex >= m_nTotalChannel  ) return;
	CNodeObj * pObj = m_pChannelInfo[nLinkIndex].GetCurrentNodeObj();
	if( NULL == pObj ) return;
	if( NODETYPE_SWITCH != pObj->m_nType ) return;
	Nor32_Switch_Result(  nLinkIndex,  bResult , pObj );//检查Switch结果
	return;
}

void CNorMarch32::Nor32_Switch_Result(int nIndex, BOOL bResult, CNodeObj *pNewObj)
{
	if( 0 == m_pChannelInfo[nIndex].m_nStep ) return;
	CString strMSG = "";
	CSwitchObj *pObj = (CSwitchObj*)pNewObj;
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CSwitchObj) ) )
	{
		strMSG.Format("Switch Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	int nAimIndex	= m_pChannelInfo[nIndex].m_nLinkCh ;
	if( bResult )
	{
		//连接成功
		Nor32_MySetLink( nIndex, nAimIndex, CF_CHSWITCH_CONNECT );
		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 0, "");
		}
		
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	}
	else
	{
		m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
		Nor32_ResetChannel( nAimIndex );
		m_pChannelInfo[nIndex].m_nLinkCh = -1;
		Nor32_MyPlaySignal(nIndex,NOR32_BUSY_SIG);
		strMSG.Format("%s Switch  fail; ChIndex = %d ",pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1,"");
		}
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
	}
	
	return;
}

void CNorMarch32::Nor32_Cus_OutCallResult(int nIndex, BOOL bResult )
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ){
		return;
	}
	CNodeObj * NewpObj = m_pChannelInfo[nIndex].GetCurrentNodeObj();
	if( NULL == NewpObj ) {
		return;
	}
	if( NODETYPE_CUSTOM != NewpObj->m_nType ) {
		return;
	}

	CString strMSG = "";
	CCustomObj *pObj = (CCustomObj*)NewpObj;
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CCustomObj) ) )
	{
		strMSG.Format("CustomObj Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	if( 9 != pObj->m_nFunType )//OutCall(   <Tel> )
	{
		return;
	}
	if(  pChnlInfo->m_nStep < 2 )
	{
		return;
	}
	if( bResult )//成功
	{
		m_pPubCObj->PC_Cus_MoveNextFun( pChnlInfo, pObj);
		return;
	}
	else//超时或失败
	{
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	return;
}

void CNorMarch32::Nor32_Custom(int nIndex)
{
	CString strMSG("");
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nType = pChnlInfo->m_nChanelType;
	CCustomObj *pObj = (CCustomObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CCustomObj) ) )
	{
		strMSG.Format("CustomObj Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	
	if( 0 == pChnlInfo->m_nStep )
	{
		pObj->MoveFirst();
		pChnlInfo->m_nStep = 1;
	}
	if( pObj->m_bEnd )
	{
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		return;
	}
	switch( pObj->m_nFunType )
	{
		case 0://SendFax(<File> )
			Nor32_Cus_SendFax(  nIndex,  pChnlInfo,  pObj );
			break;
		case 1://StartPlayFile(  <File> ; <Loop> )
			Nor32_Cus_StartPlayFile(  nIndex,  pChnlInfo,  pObj );
			break;
		case 2://StartRecordFile(  <File>  ; <Len> ; <Loop> )
			Nor32_Cus_StartRecordFile(  nIndex,  pChnlInfo,  pObj );
			break;
		case 3://GetAgentCall( <AgentDN> )
			Nor32_Cus_GetAgentCall(  nIndex,  pChnlInfo,  pObj );
			break;
		case 4://EnterConfer( <Chnl> ; <conference> )
			Nor32_Cus_EnterConfer(  nIndex,  pChnlInfo,  pObj );
			break;
		case 5://LeaveConfer( <Chnl> ; <conference> )
			Nor32_Cus_LeaveConfer(  nIndex,  pChnlInfo,  pObj );
			break;
		case 6://ReciveDtmf( <SaveTo> ; <Len> ; <EndDtmf> )
			Nor32_Cus_ReciveDtmf(  nIndex,  pChnlInfo,  pObj );
			break;
		case 7://ReciveFax( <File> )
			Nor32_Cus_ReciveFax(  nIndex,  pChnlInfo,  pObj );
			break;
		case 8://OffHook( <Chnl> )
			Nor32_Cus_OffHook(  nIndex,  pChnlInfo,  pObj );
			break;
		case 9://OutCall(   <Tel> )
			Nor32_Cus_OutCall(  nIndex,  pChnlInfo,  pObj );
			break;
		case 10://StopPlayFile( )
			Nor32_Cus_StopPlayFile(  nIndex,  pChnlInfo,  pObj );
			break;
		case 11://StopRecordFile()
			Nor32_Cus_StopRecordFile(  nIndex,  pChnlInfo,  pObj );
			break;
		case 12://HangUp( <Chnl> )
			Nor32_Cus_HangUp(  nIndex,  pChnlInfo,  pObj );
			break;
		case 13://ClearDtmf( <Chnl> )
			Nor32_Cus_ClearDtmf(  nIndex,  pChnlInfo,  pObj );
			break;
		case 14://GetActiveCall
			Nor32_Cus_GetAgentActiveCall( nIndex,  pChnlInfo,  pObj );
			break;
		case 15://AnswerCall
			Nor32_Cus_AnswerCall( nIndex,  pChnlInfo,  pObj );
			break;
		case 16://Hold
			Nor32_Cus_Hold( nIndex,  pChnlInfo,  pObj );
			break;
		case 17://PickUpHold
			Nor32_Cus_PickupHold( nIndex,  pChnlInfo,  pObj );
			break;
		case 18://TansferToIVR
			Nor32_Cus_TansferToIVR( nIndex,  pChnlInfo,  pObj );
			break;	
		case 19://Monitor
			Nor32_Cus_Monitor( nIndex,  pChnlInfo,  pObj );
			break;	
		case 20://DisMonitor
			Nor32_Cus_DisMonitor( nIndex,  pChnlInfo,  pObj );
			break;	
		case 21://JoinConfer
			Nor32_Cus_JoinConfer( nIndex,  pChnlInfo,  pObj );
			break;	

		default:
			break;
	}

	return;
}

void CNorMarch32::Nor32_Cus_ReciveFax(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}

void CNorMarch32::Nor32_Cus_SendFax(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}

void CNorMarch32::Nor32_Cus_OutCall(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom OutCall Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	
	switch( pChnlInfo->m_nStep )
	{
		case 1:
			{
				CString str = pObj->m_strFunName;
				str.Replace("OutCall",NULL);
				CString strTel("");
				if( !m_pPubCObj->PC_GetParameterValues( pChnlInfo,str,strTel ) )
				{
					strMSG.Format("%s  Custom OutCall is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}

				char *pszTel = (LPTSTR)(LPCTSTR)strTel;
				pChnlInfo->m_strInboundCallPK.Format("%s%d", (CTime::GetCurrentTime()).Format("%Y%m%d%H%M%S") , nIndex );
				if( !Nor32_MyMakeCall( nIndex, pChnlInfo->m_strInboundCallPK,pszTel,  (LPTSTR)(LPCTSTR)pChnlInfo->m_strLocalDN ) )
				{
					strMSG.Format("Custom OutCall Error; ChIndex = %d it is error that to outcall! %s", nIndex,pObj->m_strFunName );
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return ;   	
				}
				else
				{
					pChnlInfo->m_nStep = 2;
				}
			}
			break;
		default:
			break;
	}


	
	return;
}
void CNorMarch32::Nor32_Cus_OffHook(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom OffHook Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}

	CString str = pObj->m_strFunName;
	str.Replace("OffHook",NULL);
	CString strAmiChnl = "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlInfo,str,strAmiChnl) )
	{
		strMSG.Format("%s  Custom OffHook is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	int nAimChnl = atoi( strAmiChnl );
	Nor32_MyOFFHook( nAimChnl);

	m_pPubCObj->PC_Cus_MoveNextFun( pChnlInfo, pObj);
}
void CNorMarch32::Nor32_Cus_ReciveDtmf(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom ReciveDtmf Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	

	int nStep = pChnlInfo->m_nStep;
	switch( nStep )
	{
	case 1:
		{	
			pChnlInfo->m_nDtmfLen = -1;
			pChnlInfo->m_strDtmfEnd = "";//接收DTMF的结束按键

			CString str = pObj->m_strFunName;
			str.Replace("ReciveDtmf",NULL);
			CString strSaveTo = "";
			CString strLen = "";
			CString strEndDtmf = "";
			CString strTimeOut = "";
			if( !m_pPubCObj->PC_DivideString( str,";",strSaveTo,strLen,strEndDtmf,strTimeOut) )
			{
				strMSG.Format("%s Custom ReciveDtmf Error is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			pChnlInfo->m_nTimeOut = atoi(strTimeOut);
			pChnlInfo->m_nDtmfLen  = atoi(strLen);
			pChnlInfo->m_strDtmfEnd  = strEndDtmf;
			pChnlInfo->m_nSaveDtmfKey = pObj->GetVarKey(strSaveTo);
			if( -1 == pChnlInfo->m_nSaveDtmfKey  )
			{
				strMSG.Format("%s Custom ReciveDtmf ; not find saveto variable ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			
			m_pPubCObj->PC_StartTimer(pChnlInfo);
			pChnlInfo->m_strState = "ReciveDTMF";
			int  len = strlen(pChnlInfo->m_strDTMFRecived);
			if( 0 < pChnlInfo->m_nDtmfLen  )
			{
				//通过判断接收按键的最大数量来结束收键
				if(  len  >= pChnlInfo->m_nDtmfLen )
				{
					m_pPubCObj->PC_Cus_MoveNextFun( pChnlInfo, pObj);
					return;
				}
				else
				{
					pChnlInfo->m_nStep = 2;
					return;
				}

			}
			else
			{
				//通过判断接收特定按键值来结束收键
				if( pChnlInfo->m_strDTMFRecived.Right(1) == pChnlInfo->m_strDtmfEnd )
				{
					pChnlInfo->m_strDTMFRecived.Replace( pChnlInfo->m_strDtmfEnd, NULL );
					m_pPubCObj->PC_Cus_MoveNextFun( pChnlInfo, pObj);
					return;
				}
				else
				{
					pChnlInfo->m_nStep = 2;
					return;
				}
			}
		}
		break;
	case 2:
		{
			if( strlen( pChnlInfo->m_strDTMFRecived ) )
			{
				Nor32_MyStopPlay(nIndex);
				if( 0 < pChnlInfo->m_nDtmfLen  )
				{
					//通过判断接收按键的最大数量来结束收键
					int  len = strlen( pChnlInfo->m_strDTMFRecived );
					if( len  >= pChnlInfo->m_nDtmfLen )
					{
						m_pPubCObj->PC_SaveValuesToVar( nIndex, pChnlInfo, pChnlInfo->m_strDTMFRecived, pChnlInfo->m_nSaveDtmfKey );
						m_pPubCObj->PC_Cus_MoveNextFun( pChnlInfo, pObj);
						return;
					}

				}
				else
				{
					//通过判断接收特定按键值来结束收键
					if( pChnlInfo->m_strDTMFRecived.Right(1) == pChnlInfo->m_strDtmfEnd )
					{
						pChnlInfo->m_strDTMFRecived.Replace( pChnlInfo->m_strDtmfEnd, NULL );
						m_pPubCObj->PC_SaveValuesToVar( nIndex, pChnlInfo, pChnlInfo->m_strDTMFRecived, pChnlInfo->m_nSaveDtmfKey );
						m_pPubCObj->PC_Cus_MoveNextFun( pChnlInfo, pObj);
						return;
					}
				}
				pChnlInfo->m_nEventType = EVENTTYPE_RECDTMF;
				
			}
			long n = m_pPubCObj->PC_ElapseTime(pChnlInfo);
			if( n >= pChnlInfo->m_nTimeOut )
			{
				m_pPubCObj->PC_SaveValuesToVar( nIndex, pChnlInfo, pChnlInfo->m_strDTMFRecived, pChnlInfo->m_nSaveDtmfKey );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				strMSG.Format("%s TimeOut Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				return;
			}
		}
		break;
	default:
		break;
	}


	return;
}
void CNorMarch32::Nor32_Cus_LeaveConfer(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom StartRecordFile Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	

	CString str = pObj->m_strFunName;
	str.Replace("LeaveConfer",NULL);
	CString strAmiChnl = "";
	CString strConfNo	= "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strAmiChnl,strConfNo ) )
	{
		strMSG.Format("%s  Custom LeaveConfer is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}


	int nAmiIndex = atoi(strAmiChnl);
	if(  nAmiIndex < 0 )
	{
		nAmiIndex = nIndex;
	}
	pChnlObj->m_nHandleConferChnl = nAmiIndex;
	int nAmiConfNo = atoi(strConfNo);
	pChnlObj->m_nHandleConferNo = nAmiConfNo;

	CString strFileName = m_strPath+"\\SysVoiceFle\\Confexit.pcm";
	Nor32_MyStartPlayFile( nAmiIndex, strFileName );
	Nor32_MyExitConference(nAmiIndex );
	m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
	return;


}
void CNorMarch32::Nor32_Cus_EnterConfer(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom EnterConfer Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	

	CString str = pObj->m_strFunName;
	str.Replace("EnterConfer",NULL);
	CString strAmiChnl = "";
	CString strConfNo	= "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strAmiChnl,strConfNo ) )
	{
		strMSG.Format("%s  Custom EnterConfer is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}


	int nAmiIndex = atoi(strAmiChnl);
	if(  nAmiIndex < 0 )
	{
		nAmiIndex = nIndex;
	}
	pChnlObj->m_nHandleConferChnl = nAmiIndex;
	int nAmiConfNo = atoi(strConfNo);
	pChnlObj->m_nHandleConferNo = nAmiConfNo;
	if( Nor32_MyConferEnter( nAmiIndex,  nAmiIndex , CF_CHSWITCH_CONNECT ) )
	{
		CString strFileName = m_strPath+"\\SysVoiceFle\\confadd.pcm";
		if( Nor32_MyStartPlayFile( nAmiIndex, strFileName,FALSE,1) )
		{
			m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
			return;
		}
		else
		{
			strMSG.Format("%s Custom EnterConfer  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
	}
	else
	{
		strMSG.Format("%s Custom EnterConfer  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	return;

}

void CNorMarch32::Nor32_Cus_GetAgentActiveCall(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	int nDN = atoi( pChnlObj->m_strLocalDN );
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom GetActiveCall Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		
		m_pPubCObj->PC_CTI_GetActiveCallResponseMsg( nDN, 1,"","" );
		return ;  
	}
	
	CString str = pObj->m_strFunName;
	str.Replace("GetActiveCall",NULL);
	CString strType = "";
	CString strTel	= "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strType,strTel ) )
	{
		strMSG.Format("%s  Custom GetActiveCall is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_GetActiveCallResponseMsg( nDN, 1 ,"","");
		return;
	}
	
	int n = Nor32_FindActiveCall( nIndex, atoi(strType), strTel );
	if( 0 > n )
	{
		strMSG.Format("Custom GetActiveCall Error; ChIndex = %d it is error that to GetActiveCall! %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_GetActiveCallResponseMsg( nDN, 1 ,"","");
		return ;   	
	}

	int m = m_pChannelInfo[n].m_nLinkCh;
	if( 0 > m || m >= m_nTotalChannel  )
	{
		strMSG.Format("Custom GetActiveCall Error; ChIndex = %d have not active Call %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_GetActiveCallResponseMsg( nDN, 1 ,"","");
		return ; 
	}
	else
	{

		m_pChannelInfo[m].m_nLinkCh = -1;
		Nor32_ResetChannel( m );
		pChnlObj->m_nLinkCh = n;
		m_pChannelInfo[n].m_nLinkCh = nIndex;

		CNodeObj *pNodeObj = m_pChannelInfo[n].GetCurrentNodeObj();
		if( NULL != pNodeObj )
		{
			if( NODETYPE_SWITCH == pNodeObj->m_nType )
			{
				Nor32_MoveNext_Node( n,  NEXTTYPE_NEXT );
			}
		}

		Nor32_MySetLink( nIndex, n , CF_CHSWITCH_CONNECT);

		pChnlObj->m_strInboundCallPK = m_pChannelInfo[n].m_strInboundCallPK;
		pChnlObj->m_strCallerID = m_pChannelInfo[n].m_strCallerID;
		pChnlObj->m_strCalledID = m_pChannelInfo[n].m_strCalledID;

		m_pPubCObj->PC_CTI_GetActiveCallResponseMsg( nDN, 0 ,m_pChannelInfo[n].m_strCallerID,m_pChannelInfo[n].m_strCalledID);
		m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
	}
	return;
}

void CNorMarch32::Nor32_Cus_GetAgentCall(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom GetAgentCall Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	
	
	CString str = pObj->m_strFunName;
	str.Replace("GetAgentCall",NULL);
	CString strAgentDN("");
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strAgentDN ) )
	{
		strMSG.Format("%s  Custom GetAgentCall is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}



	if( !Nor32_Cus_GetAgentCall( nIndex, pChnlObj, strAgentDN ) )
	{
		strMSG.Format("Custom GetAgentCall Error; ChIndex = %d it is error that to GetAgentCall! %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   	
	}


	m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
}

void CNorMarch32::Nor32_Cus_StartRecordFile(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom StartRecordFile Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	

	CString str = pObj->m_strFunName;
	str.Replace("StartRecordFile",NULL);
	CString strFile	= "";
	CString strLen = "";
	CString strType = "";
	CString strLoop = "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strFile,strLen,strType,strLoop ) )
	{
		strMSG.Format("%s Custom StartRecordFile Error is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}



	long nLen = atol(strLen);
	if( nLen <= 0 )
	{
		nLen = 3600;
	}

	BOOL bLoop = FALSE;
	if( "0" == strLoop ){
		bLoop = FALSE;
	}
	else
	{
		bLoop = TRUE;
	}

	int nType= 0;
	if( "0" == strType )
	{
		nType = 0;
	}
	else
	{
		nType = 1;
	}
	
	if( !Nor32_MyStartRecordFile(nIndex, strFile, bLoop, nType,nLen ) )//PACK_64KBPS:
	{
		strMSG.Format("Custom StartRecordFile Error; ChIndex = %d   Error = Start Record error File=%s", nIndex,strFile );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   	
	}
	else
	{
		m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
	}


	return;
}


void CNorMarch32::Nor32_Cus_StartPlayEnd(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ){
		return;
	}
	CNodeObj * NewpObj = m_pChannelInfo[nIndex].GetCurrentNodeObj();
	if( NULL == NewpObj ) {
		return;
	}
	if( NODETYPE_CUSTOM != NewpObj->m_nType ) {
		return;
	}

	CString strMSG = "";
	CCustomObj *pObj = (CCustomObj*)NewpObj;
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CCustomObj) ) )
	{
		strMSG.Format("CustomObj Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	if( 1 != pObj->m_nFunType )//OutCall( <Chnl> ;  <Tel> )
	{
		return;
	}
	if(  pChnlInfo->m_nStep < 2 )
	{
		return;
	}

	m_pPubCObj->PC_Cus_MoveNextFun( pChnlInfo, pObj);
	
	
	return;
}

void CNorMarch32::Nor32_Cus_StartPlayFile(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom StartPlayFile  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	
	switch( pChnlObj->m_nStep )
	{
	case 1:
		{
			CString str = pObj->m_strFunName;
			str.Replace("StartPlayFile",NULL);
			CString strFile	= "";
			CString strType = "";
			CString strLoop = "";
			if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strFile,strType,strLoop ) )
			{
				strMSG.Format("%s  Custom StartPlayFile is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

			BOOL bLoop = FALSE;
			if( "0" == strLoop ){
				bLoop = FALSE;
			}
			else
			{
				bLoop = TRUE;
			}

			int nType= 0;
			if( "0" == strType )
			{
				nType = 0;
			}
			else
			{
				nType = 1;
			}

			if( !Nor32_MyStartPlayFile( nIndex, strFile,bLoop,nType) )//false
			{
				strMSG.Format("%s Custom StartPlayFile Error is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			if( bLoop )
			{
				m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
			}
			else
			{
				pChnlObj->m_nStep = 2;
			}
		}
		break;
	default:
		break;
	}

	return;
}

void CNorMarch32::Nor32_Cus_StopPlayFile( int nIndex,  CFChannelInfo *pChnlObj,  CCustomObj *pObj )
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom StopPlayFile Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	

	Nor32_MyStopPlay( nIndex  );
	m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);


	return;
}

void CNorMarch32::Nor32_Cus_StopRecordFile( int nIndex,  CFChannelInfo *pChnlObj,  CCustomObj *pObj )
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom StopRecordFile Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	
	Nor32_MyStopRecord( nIndex  );
	m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);

	return;
}

void CNorMarch32::Nor32_Cus_HangUp( int nIndex,  CFChannelInfo *pChnlObj,  CCustomObj *pObj )
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom HangUp Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	

	CString str = pObj->m_strFunName;
	str.Replace("HangUp",NULL);
	CString strAmiChnl = "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strAmiChnl) )
	{
		strMSG.Format("%s  Custom HangUp is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	int nAimChnl = atoi( strAmiChnl );
	Nor32_ResetChannel( nAimChnl  );
	m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
	
	return;
}
void CNorMarch32::Nor32_Cus_ClearDtmf( int nIndex,  CFChannelInfo *pChnlObj,  CCustomObj *pObj )
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom ClearDtmf Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	

	CString str = pObj->m_strFunName;
	str.Replace("ClearDtmf",NULL);
	CString strAmiChnl = "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strAmiChnl) )
	{
		strMSG.Format("%s  Custom ClearDtmf is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	int nAimChnl = atoi( strAmiChnl );
	Nor32_MyInitDtmfBuf( nAimChnl  );
	m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
	

	return;
}

BOOL CNorMarch32::Nor32_MyExitConference(int nIndex)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return FALSE;
	}
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int channelType = Nor32_GetDevChnlType( pChnlInfo->m_nChanelType );
	int channelID = pChnlInfo->m_nChannel ;
	pChnlInfo->m_nConfNO = -1;
	m_pPubCObj->PC_ShowChannelConferenceNO( nIndex, -1 );
	if( 0 == tpi_ExitConference( channelType,  channelID) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CNorMarch32::Nor32_MyReleaseConference(int nGroup)
{
	//函数会将所有会议成员退出会议,并释放相关资源.
	//调用了tpi_CreateConference()
    //一定要调用tpi_ReleaseConference()释放相关资源.
	if( 0 == tpi_ReleaseConference( nGroup ) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

int CNorMarch32::Nor32_GetIndexByGroup(int nGroup, int nEventType)
{
	if( eventPlayEnd == nEventType )
	{
		for( int i = 0; i < m_nTotalChannel; i++ )
		{
			if( nGroup == m_pChannelInfo[i].m_nPlayGroup )
			{
				return i;
			}
		}
	}
	else if( eventRecordEnd == nEventType )
	{
		for( int i = 0; i < m_nTotalChannel; i++ )
		{
			if( nGroup == m_pChannelInfo[i].m_nRecordGroup)
			{
				return i;
			}
		}
	}


	return -1;
}

void CNorMarch32::Nor32_Transfer(int nIndex)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) return;
	CTransferCheckObj *pObj = (CTransferCheckObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CTransferCheckObj) ) )
	{
		strMSG.Format("TransferStart Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	switch( pObj->m_nTransferType )
	{
	case 0://开始转移
		Nor32_Transfer_Start( nIndex ,pObj );
		break;
	case 1://取消转移
		Nor32_Transfer_Cancel( nIndex ,pObj );
		break;
	case 2://完成转移
		{
			switch( pObj->m_nTransferAim )
			{
			case 0:
				Nor32_Transfer_ToAgent( nIndex ,pObj );
				break;
			case 1:
				Nor32_Transfer_ToCustomer( nIndex  ,pObj );
				break;
			case 2:
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}

	return;
}

void CNorMarch32::Nor32_Transfer_Cancel(int nIndex, CTransferCheckObj *pObj)
{
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nLinkIndex =  pChnlObj->m_nLinkCh;
	if( nLinkIndex < 0 || nLinkIndex >= m_nTotalChannel  )
	{
		strMSG.Format("%s Have not active call ;ChIndex = %d", pObj->m_strCaption, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		m_pPubCObj->PC_CTI_CancelTransferResponseMsg( atoi(pChnlObj->m_strLocalDN), 1, "" );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	if( pObj->m_bClearDTMF )
	{
		pChnlObj->m_strDTMFRecived = "";
	}

	m_pChannelInfo[nLinkIndex].m_nState = CHANNEL_STATE_CONNECT;
	m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Busy");
	Nor32_MyStopPlay( nLinkIndex );
	Nor32_MyStopPlay( nIndex );
	if( !Nor32_MySetLink( nIndex, nLinkIndex , CF_CHSWITCH_CONNECT ) )
	{
		strMSG.Format("Cancel transfer error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_CancelTransferResponseMsg( atoi(pChnlObj->m_strLocalDN), 1, "" );
		return;
	}
	m_pPubCObj->PC_CTI_CancelTransferResponseMsg( atoi(pChnlObj->m_strLocalDN), 0, "" );
	Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );

	return;
}

void CNorMarch32::Nor32_Transfer_Start(int nIndex, CTransferCheckObj *pObj)
{
	CString strMSG = "";
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) return;
	int nLinkIndex =  pChnlObj->m_nLinkCh;
	int nDN = atoi( pChnlObj->m_strLocalDN );
	if( nLinkIndex <= -1 )
	{
		strMSG.Format("%s Have not active call ;ChIndex = %d", pObj->m_strCaption, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		CTI_TransferMsg( nDN, 1, "" );//执行错误
		return;
	}

	Nor32_MyClearLink(nIndex,nLinkIndex );
	if( 1 == pObj->m_nWaitOption )
	{
		//播放语音
		Nor32_MyStartPlayFile( nLinkIndex, pObj->m_strPlayFile);
		m_pChannelInfo[nLinkIndex].m_strPlayFile = pObj->m_strPlayFile;
	}
	if( pObj->m_bClearDTMF )
	{
		pChnlObj->m_strDTMFRecived = "";
	}
	Nor32_MyPlaySignal( nIndex, NOR32_DIAL_SIG );
	m_pChannelInfo[nLinkIndex].m_nState = CHANNEL_STATE_TRANSFER;
	m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Busy" );
	Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);


	return;
}

void CNorMarch32::Nor32_Transfer_ToCustomer(int nIndex, CTransferCheckObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) return;
	int nStep = pChnlObj->m_nStep;
	int nLinkIndex =  pChnlObj->m_nLinkCh;
	int nDN = atoi( pChnlObj->m_strLocalDN );
	CString strMSG = "";
	switch( nStep )
	{
	case 0:
		{
			if( 0 > nLinkIndex || nLinkIndex >= m_nTotalChannel  )
			{
				strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}
			if( pObj->m_bClearDTMF )
			{
				pChnlObj->m_strDTMFRecived = "";
			}
			pChnlObj->m_nStep = 1;
		}
		break;
	case 1:
		{
			//转移到外线通道
			CString strPhone	= "";
			//呼叫转移到外线时，目标号码必须从表达式获取
			strPhone = pObj->m_strExpression;
			int nKey = pObj->GetVarKey(strPhone);
			if(  !m_pPubCObj->PC_GetNorVarValues(pChnlObj,  strPhone, nKey ) )
			{
				strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}
			
			//已取得电话号码
			int nAimIndex = Nor32_FindFreeTrunk();
			if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
			{
				strMSG.Format("%s have not free channel ; ChIndex = %d ",pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}

			if( !Nor32_MyMakeCall( nAimIndex, "",(LPSTR)(LPCSTR)strPhone, (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN ) )
			{
				strMSG.Format("%s Call out fail ,GErrCode=%d ; ChIndex = %d ",pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}

			pChnlObj->m_nTransferCh	= nAimIndex;
			m_pChannelInfo[nAimIndex].m_nLinkCh = nIndex;
			Nor32_MyPlaySignal(nIndex, NOR32_RINGBACK_SIG);
			m_pPubCObj->PC_StartTimer(pChnlObj);
			pChnlObj->m_nStep = 2;
		}
		break;
	case 2:
		{
			int nAimIndex = pChnlObj->m_nTransferCh;
			//收到取消转移命令
			BOOL bCancel = FALSE;
			if( EVENTTYPE_AGENT_CANCEL_TRANSFER == pChnlObj->m_nEventType )
				bCancel = TRUE;
			else
			{
				//看是否收到取消转移的按键
				if( strlen( pChnlObj->m_strDTMFRecived ) > 0 )
				{
					m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlObj->m_strDTMFRecived );
					if(  pChnlObj->m_strDTMFRecived.Right(1) == pObj->m_strCancelDTMF  )
					{
						bCancel = TRUE;
					}
				}
			}
			if( bCancel )
			{
				//满足取消转移条件
				m_pChannelInfo[nAimIndex].m_nLinkCh	= -1;
				Nor32_ResetChannel( nAimIndex );
				pChnlObj->m_nTransferCh = -1;

				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_CANCEL_TRANSFER );//取消转移
			}
			return;
		
		}
		break;
	default:
		break;
	}

	return;
}

void CNorMarch32::Nor32_Transfer_ToAgent(int nIndex, CTransferCheckObj *pObj)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlInfo->m_nChanelType ) return;
	CString strMSG = "";
	int nStep = pChnlInfo->m_nStep;
	int nLinkIndex =  pChnlInfo->m_nLinkCh;
	int nDN = atoi( pChnlInfo->m_strLocalDN );
	int nAimIndex	= -1 ;
	switch( nStep )
	{
	case 0:
		{
			if( nLinkIndex < 0 || nLinkIndex >= m_nTotalChannel  )
			{
				strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}
			if( pObj->m_bClearDTMF )
			{
				pChnlInfo->m_strDTMFRecived = "";
			}

			//转移到内线通道
			CString strPhone	= pObj->m_strExpression;
			int nKey = pObj->GetVarKey(strPhone);
			if(  !m_pPubCObj->PC_GetNorVarValues(pChnlInfo,  strPhone, nKey ) )
			{
				strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}
	
			//已取得电话号码
			nAimIndex = Nor32_FindFreeChannel( strPhone, CF_CHTYPE_USER  );//通过分机号码查找该分机
			if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
			{
				//如果该通道不是内线，认为是错误的
				strMSG.Format("%s Channel index is error; ChIndex = %d ",  pObj->m_strCaption, nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}

			//呼叫分机空闲，对其振铃
			pChnlInfo->m_nTransferCh	= nAimIndex;
			m_pChannelInfo[nAimIndex].m_nLinkCh		= nIndex;
			Nor32_MyMakeCall( nAimIndex, pChnlInfo->m_strInboundCallPK,(LPTSTR)(LPCTSTR)pChnlInfo->m_strCallerID, (LPTSTR)(LPCTSTR)pChnlInfo->m_strCalledID );
			Nor32_MyPlaySignal(nIndex, NOR32_RINGBACK_SIG);
			m_pPubCObj->PC_ShowChannelState( nAimIndex, "Ringing");	
			pChnlInfo->m_nStep = 2;
			
		}
		break;
	case 2:
		{
			//检测内线结果
			nAimIndex = pChnlInfo->m_nTransferCh;
			//收到取消转移命令
			BOOL bCancel = FALSE;
			if( EVENTTYPE_AGENT_CANCEL_TRANSFER == pChnlInfo->m_nEventType )
				bCancel = TRUE;
			else
			{
				//看是否收到取消转移的按键
				if( strlen( pChnlInfo->m_strDTMFRecived ) > 0 )
				{
					m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlInfo->m_strDTMFRecived );
					if(  pChnlInfo->m_strDTMFRecived.Right(1) == pObj->m_strCancelDTMF  )
					{
						bCancel = TRUE;
					}
				}
			}
			if( bCancel )
			{
				//满足取消转移条件
				m_pChannelInfo[nAimIndex].m_nLinkCh	= -1;
				Nor32_ResetChannel( nAimIndex );
				pChnlInfo->m_nTransferCh = -1;

				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_CANCEL_TRANSFER );//取消转移
			}
			return;
		
			
		}
		break;
	default:
		break;
	}

	return;
}

void CNorMarch32::Nor32_Transfer_Judge(int nIndex, BOOL bResult )
{
	int nLinkIndex = m_pChannelInfo[nIndex].m_nLinkCh;
	if( -1 >= nLinkIndex || nLinkIndex >= m_nTotalChannel  ){
		return;
	}
	CNodeObj * pObj = m_pChannelInfo[nLinkIndex].GetCurrentNodeObj();
	if( NULL == pObj ) {
		return;
	}
	if( NODETYPE_CHECK_TRANSFER != pObj->m_nType ) {
		return;
	}
	Nor32_Transfer_Result(  nLinkIndex,  bResult , pObj );//检查Transfer结果
	return;
}

void CNorMarch32::Nor32_Transfer_Result(int nIndex, BOOL bResult, CNodeObj *pNewObj)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( 0 == pChnlInfo->m_nStep ){
		return;
	}

	CString strMSG = "";
	CTransferCheckObj *pObj = (CTransferCheckObj*)pNewObj;
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CTransferCheckObj) ) )
	{
		strMSG.Format("Transfer Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	int nDN = atoi( pChnlInfo->m_strLocalDN );
	int nAimIndex = pChnlInfo->m_nTransferCh;
	if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  ){
		pChnlInfo->m_nTransferCh = -1;
		strMSG.Format("%s TimeOut Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );//超时，没有应答
		CTI_TransferMsg( nDN, 1, "" );//执行错误
		return;
	}
	if( bResult )//转移成功
	{
		int nLinkIndex =  pChnlInfo->m_nLinkCh;
		if( -1 >= nLinkIndex || nLinkIndex >= m_nTotalChannel  ){
			pChnlInfo->m_nTransferCh = -1;
			strMSG.Format("%s TimeOut Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );//超时，没有应答
			CTI_TransferMsg( nDN, 1, "" );//执行错误
			return;
		}
		if( 1 == pObj->m_nWaitOption ){
			Nor32_MyStopPlay( nLinkIndex );
		}
		m_pChannelInfo[nAimIndex].m_nTransferCh		= -1;
		m_pChannelInfo[nAimIndex].m_nLinkCh			= nLinkIndex;

		Nor32_MySetLink( nLinkIndex, nAimIndex , CF_CHSWITCH_CONNECT );
		m_pChannelInfo[nLinkIndex].m_nState			= CHANNEL_STATE_CONNECT;
		m_pChannelInfo[nLinkIndex].m_nTransferCh	= -1;
		m_pChannelInfo[nLinkIndex].m_nLinkCh		= nAimIndex;

		pChnlInfo->m_nTransferCh		= -1;
		pChnlInfo->m_nLinkCh			= -1;
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		CTI_TransferMsg( nDN, 0, "" );
		return;
	}
	else
	{
		//超时或失败
		m_pChannelInfo[nAimIndex].m_nLinkCh	= -1;
		Nor32_ResetChannel( nAimIndex );
		pChnlInfo->m_nTransferCh = -1;
		strMSG.Format("%s TimeOut Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );//超时，没有应答
		CTI_TransferMsg( nDN, 1, "" );//执行错误

	}
	//检测结果

	
	
	return;
}



void CNorMarch32::Nor32_ConferenceLeave(int nIndex)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nDN = atoi( pChnlObj->m_strLocalDN );
	CLeaveConferObj *pObj = (CLeaveConferObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CLeaveConferObj) ) )
	{
		strMSG.Format("Leave Conference error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );

		return ;  
	}


	pChnlObj->m_nHandleConferNo = -1;
	int nKey = pObj->GetVarKey( pObj->m_strConferNO );
	CString strConfNO = "";
	if( nKey < 0 )
	{
		pChnlObj->m_nHandleConferNo = atoi(pObj->m_strConferNO);
	}
	else
	{
		if(  !m_pPubCObj->PC_GetNorVarValues( pChnlObj, strConfNO, nKey ) )
		{
			strMSG.Format("%s Leave Conference index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		pChnlObj->m_nHandleConferNo = atoi( strConfNO );
	}
	//取加入的通道号

	int nAimIndex = -1;
	CString strChnl = pObj->m_strChnlIndex;
	if( "-1" == strChnl )
	{
		//操作的会议通道号是自己
		pChnlObj->m_nHandleConferChnl = nIndex;
		nAimIndex = nIndex;
	}
	else
	{
		
		int nKey1 = pObj->GetVarKey( strChnl );
		if( nKey1 < 0 )
		{
			nAimIndex = atoi( strChnl);
		}
		else
		{
			if(  !m_pPubCObj->PC_GetNorVarValues( pChnlObj, strChnl, nKey1 ) )
			{
				strMSG.Format("%s Input Channel index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			nAimIndex = atoi( strChnl );
		}
		if( nAimIndex < 0  || nAimIndex >= m_nTotalChannel  ) 
		{
			strMSG.Format("%s Enter Conference  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		pChnlObj->m_nHandleConferChnl = nAimIndex;
		
	}
	//播放离开会议的声音
	CString strFileName = m_strPath+"\\SysVoiceFle\\Confexit.pcm";
	Nor32_MyReleaseConference( pChnlObj->m_nHandleConferNo );
	Nor32_MyStartPlayFile( nIndex,strFileName,pChnlObj->m_nHandleConferNo,FALSE,1);
	Nor32_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );

	return;
}


void CNorMarch32::Nor32_DialCheck(int nIndex)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nType = pChnlObj->m_nChanelType;
	if( CF_CHTYPE_RECORD == nType ) return;
	CDialCheckObj *pObj = (CDialCheckObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDialCheckObj) ) )
	{
		strMSG.Format("DialCheck Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		if( pObj->m_bError )
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	

	int nSetup = pChnlObj->m_nStep;
	int nDialChnl = pChnlObj->m_nDialChIndex;
	switch( nSetup )
	{
	case 0:
		{
			char szPhone[32] ;
			memset( szPhone, 0 , sizeof(szPhone) );
			if( CF_CHTYPE_USER != nType )
			{
				int nKey = pObj->GetVarKey( pObj->m_strPhone );
				CString strPhone = "";
				if( nKey < 0 )
				{
						strPhone = pObj->m_strPhone;//输入的是号码串，不是变量
				}
				else
				{
					if(  !m_pPubCObj->PC_GetNorVarValues( pChnlObj, strPhone, nKey ) )
					{
							strMSG.Format("%s Dial tel Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
							m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
							Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
							return;
					}
				}
				
				memmove( szPhone, strPhone, strlen(strPhone) >= 32 ? 31:strlen(strPhone) );
				if( strlen(szPhone) < 3 )
				{
						//号码长度小于3时，被认为号码错误
						strMSG.Format("%s  Phone Error; ChIndex = %d", pObj->m_strCaption, nIndex);
						m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
						Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
				}
			}
			if( !Nor32_MyMakeCall( nIndex,"",szPhone, (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN  ) )
			{
					strMSG.Format("%s  Make call Error; ChIndex = %d", pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
			}
			pChnlObj->m_nStep = 1;
		}
		break;
	default:
		break;
	}

	return;
}

void CNorMarch32::Nor32_Dial_Judge(int nIndex,BOOL bResult)
{
	int nLinkIndex = m_pChannelInfo[nIndex].m_nLinkCh;
	if( -1 >= nLinkIndex || nLinkIndex >= m_nTotalChannel  ){
		return;
	}
	CNodeObj * pObj = m_pChannelInfo[nLinkIndex].GetCurrentNodeObj();
	if( NULL == pObj ) {
		return;
	}
	if( NODETYPE_CHECK_DIAL != pObj->m_nType ) {
		return;
	}
	Nor32_Dila_Result(  nLinkIndex,  bResult , pObj );//检查结果
	return;
}

void CNorMarch32::Nor32_Dila_Result(int nIndex, BOOL bResult, CNodeObj *pNewObj)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( 0 == pChnlInfo->m_nStep ){
		return;
	}

	CString strMSG = "";
	CDialCheckObj *pObj = (CDialCheckObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDialCheckObj) ) )
	{
		strMSG.Format("DialCheck Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		if( pObj->m_bError )
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	if( bResult )//成功
	{
		pChnlInfo->m_strState ="Connect";
		pChnlInfo->m_nState	 = CHANNEL_STATE_CONNECT;
		m_pPubCObj->PC_ShowChannelState( nIndex, "Connect" );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_CONNECT );
		return;
	}
	else
	{
		//超时或失败
		pChnlInfo->m_nState			= CHANNEL_STATE_FREE;
		pChnlInfo->m_nEventType		=  EVENTTYPE_HANGUP;
		strMSG.Format("%s  Make call Error; ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;

	}
	
	return;
}

BOOL CNorMarch32::Nor32_Cus_GetAgentCall(int nIndex, CFChannelInfo *pChnlInfo, CString strDN)
{
	int m = -1;
	int n = -1;
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		n = m_pChannelInfo[i].m_nLinkCh;
		if( -1 != n )
		{
			if( strDN == m_pChannelInfo[i].m_strLocalDN && CF_CHTYPE_USER == m_pChannelInfo[i].m_nChanelType  )
			{
				m = i;
				break;
			}
		}
	}
	if( nIndex == m )
	{
		return FALSE;
	}
	
	if( -1 != m )
	{
		if( CHANNEL_STATE_FREE == m_pChannelInfo[m].m_nState )
		{
			return FALSE;
		}
		m_pChannelInfo[m].m_nLinkCh = -1;
		Nor32_ResetChannel( m );
		pChnlInfo->m_nLinkCh = n;
		m_pChannelInfo[n].m_nLinkCh = nIndex;
		Nor32_MySetLink( nIndex, n , CF_CHSWITCH_CONNECT);

		return TRUE;
	}
	else
	{
		return FALSE;
	}


}

void CNorMarch32::Nor32_MyInitDtmfBuf(int nIndex)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}
	m_pChannelInfo[nIndex].m_strDTMFRecived = "";
	return;
}


void CNorMarch32::Nor32_DealCTIMsg(CFC_MESSAGE_PACK *pMsg)
{
	
	int nMsgType = pMsg->nMsgType;
	int nDN = pMsg->nSMID;
	CString strMSG = "";
	strMSG.Format("%d",nDN);
	int nIndex = Nor32_FindUerChannel( strMSG);
	if( nMsgType != CFMTYPE_REG_COMM )
	{
		if(  0 > nIndex || nIndex >= m_nTotalChannel )
		{
			strMSG.Format("[MType:%d][Message Error From DN:%d]", nMsgType, nDN );
			m_pPubCObj->PC_ShowSysMsg(0,strMSG);	
			return;
		}
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	m_lComm = pMsg->lCardKey;

	switch( nMsgType )
	{
		case CFMTYPE_MONITOR://Monitoring
			{
				//通道操作
				CFMSG_MONITOR recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, pMsg->szMessageBody ,sizeof(CFMSG_MONITOR) );
				strMSG.Format("[DN:%d][Monitoring:%s]", pMsg->nSMID, recMsg.szTel);
				m_pPubCObj->PC_ShowCommMsg( strMSG );
				strMSG.Format("%s",recMsg.szTel);
				int nMonitorIndex = Nor32_FindUerChannel( strMSG );
				if( -1 != nMonitorIndex  )
				{
					//找到发送消息来的正确通道号码
					pChnlObj->m_strDialTel = recMsg.szTel ;
					pChnlObj->m_nEventType	= EVENTTYPE_AGENT_MONITOR_MSG;
					m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
				}
				else
				{
					CTI_MonitorMsg(  nDN, 1, "");//执行错误
				}
				strMSG.Format("[MType:Monitoring][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( strMSG );
			}
			break;
		case CFMTYPE_DMONITOR://Disable Monitoring
			{
				//通道操作
				CFMSG_MONITOR recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, pMsg->szMessageBody ,sizeof(CFMSG_MONITOR) );
				strMSG.Format("[DN:%d][DisMonitoring]", pMsg->nSMID);
				m_pPubCObj->PC_ShowCommMsg( strMSG );
				strMSG.Format("%s",recMsg.szTel);
				pChnlObj->m_nEventType	= EVENTTYPE_AGENT_DISMONITOR_MSG;
				CTI_DisMonitorMsg(  nDN, 0 );//执行正确

				strMSG.Format("[MType:DisMonitoring][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( strMSG );
			}
			break;
		case CFMTYPE_MCALL://Make Call
			{
				//通道操作
				CFMSG_MAKECALL recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, pMsg->szMessageBody ,sizeof(CFMSG_MAKECALL) );
				strMSG.Format("[DN:%d][MakeCall:%s][CallType:%d]", pMsg->nSMID, recMsg.szTel,recMsg.nCallType);
				m_pPubCObj->PC_ShowCommMsg( strMSG );

				//找到发送消息来的正确通道号码
				pChnlObj->m_nDialType = recMsg.nCallType;
				pChnlObj->m_strDialTel = recMsg.szTel;
				pChnlObj->m_nEventType	= EVENTTYPE_AGENT_MAKECALL_MSG;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");

				
				//MakeCall response message
				CTI_MakeCallMsg( nDN ,0 , "");//执行正确

				strMSG.Format("[MType:MakeCall][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( strMSG );
			}
			break;
		case CFMTYPE_TRANSFER://Transfer
			{
				//通道操作
				CFMSG_TRANSFER recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, pMsg->szMessageBody ,sizeof(CFMSG_TRANSFER) );
				strMSG.Format("[DN:%d][Transfer:%s]", pMsg->nSMID, recMsg.szTel);
				m_pPubCObj->PC_ShowCommMsg( strMSG );
				int nLinkIndex = pChnlObj->m_nLinkCh;
				if( -1 != nLinkIndex )
				{
					//找到发送消息来的正确通道号码
					pChnlObj->m_nDialType = recMsg.nCallType;
					pChnlObj->m_strDialTel = recMsg.szTel;
					pChnlObj->m_nEventType	= EVENTTYPE_AGENT_TRANSFER_MSG;
					m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
				}
				else
				{
					CTI_TransferMsg( nDN, 1, "" );//执行错误
					strMSG.Format("[MType:Transfer][TO DN:%d]", nDN  );
					m_pPubCObj->PC_ShowCommMsg( strMSG );
				}


			}
			break;
		case CFMTYPE_CANCEL_TRANSFER://Cancel transfer
			{
				//通道操作
				pChnlObj->m_nEventType	= EVENTTYPE_AGENT_CANCEL_TRANSFER;
				strMSG.Format("[MType:CancelTransfer][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( strMSG );

			}
			break;
		case CFMTYPE_ANSWER://Answer
			{
				//通道操作
				strMSG.Format("[MType:AnswerCall][From DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg(strMSG );

				//当用软电话AnswerCall时必须用
				int nChnl = pChnlObj->m_nChannel;
				Nor32_MyOFFHook( nIndex );
				pChnlObj->m_nEventType	= EVENTTYPE_OFFHOOK;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");

				CTI_AnswerCallMsg( nDN, 0 );//执行正确

				strMSG.Format("[MType:AnswerCall][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( strMSG );
			}
			break;
		case CFMTYPE_RELEASECALL://Release Call
			{

			}
			break;
		case CFMTYPE_HOLD://Hold Request
			{
				//通道操作
				CFMSG_HOLD recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, pMsg->szMessageBody ,sizeof(CFMSG_HOLD) );
				strMSG.Format("[Hold DN:%d]", pMsg->nSMID);
				m_pPubCObj->PC_ShowCommMsg( strMSG );
				int nLinkIndex = pChnlObj->m_nLinkCh;
				if( -1 != nLinkIndex )
				{
					//找到发送消息来的正确通道号码
					pChnlObj->m_nEventType	= EVENTTYPE_AGENT_HOLD_MSG;
					m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
				}
				else
					CTI_HoldMsg( nDN , 1 );//执行错误
				strMSG.Format("[MType:Hold][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( strMSG );
			}
			break;
		case CFMTYPE_PICKUPHOLD://Pickup Hold Request
			{
				//通道操作
				CFMSG_PICKUPHOLD recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, pMsg->szMessageBody ,sizeof(CFMSG_PICKUPHOLD) );
				strMSG.Format("[PickupHold DN:%d]", pMsg->nSMID);
				m_pPubCObj->PC_ShowCommMsg( strMSG );


				pChnlObj->m_nPickUpType = recMsg.nType;//取回保持的类型
				pChnlObj->m_strPickUpTel = recMsg.szTel;//取回保持的主叫号码

				int nLinkIndex = -1;
				if( 1 == pChnlObj->m_nPickUpType )
				{
					nLinkIndex = Nor32_FindHoldChannel( nIndex ,pChnlObj->m_strPickUpTel);
				}
				else
				{
					nLinkIndex = Nor32_FindHoldChannel( nIndex);
				}

				if( -1 != nLinkIndex )
				{
					//找到发送消息来的正确通道号码
					pChnlObj->m_nEventType	= EVENTTYPE_AGENT_PICKUPHOLD_MSG;
					m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
					
				}	
				else
					m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 1,"","" );//执行错误
				strMSG.Format("[MType:PickupHold][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( strMSG );
			}
			break;
		case CFMTYPE_SENDFAX://Send Fax Request
			{

			}
			break;
		case CFMTYPE_LOGIN://Set Feature Login
			{

			}
			break;
		case CFMTYPE_LOGOUT://Set Feature Logout
			{

			}
			break;
		case CFMTYPE_MSETBBUSY://Set Feature MSB
			{

			}
			break;
		case CFMTYPE_MSI://Set Feature MSI
			{

			}
			break;
		case CFMTYPE_SFNOTR://Set Feature Not Ready
			{

			}
			break;
		case CFMTYPE_SFR://Set Feature  Ready
			{

			}
			break;
		case CFMTYPE_REG_COMM://reg to Comm and response
			{
				m_pPubCObj->PC_ShowSysMsg(0,"Regist to communicate server success");
			}
			break;
		case CFMTYPE_NET_ABEND://Net abend message type
			{

			}
			break;
		case CFMTYPE_HANGUP://Net abend message type
			{
				//通道操作
				strMSG.Format("[MType:HangUp][From DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg(strMSG );
				Nor32_ResetChannel( nIndex );	
			}
			break;
		case CFMTYPE_INCONFER://In Conference
			{
				//通道操作
				CFMSG_IN_CONFER recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, pMsg->szMessageBody ,sizeof(CFMSG_IN_CONFER) );
				strMSG.Format("[DN:%d][In Conference:%s][CallType:4]", pMsg->nSMID, recMsg.szTel);
				m_pPubCObj->PC_ShowCommMsg( strMSG );

				//找到发送消息来的正确通道号码
				pChnlObj->m_nDialType = 4;//加入某分机所在的会议，此时直接设定类型为4
				pChnlObj->m_strDialTel.Format("%s", recMsg.szTel );
				pChnlObj->m_nEventType	= EVENTTYPE_AGENT_CONFER_MSG;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
			}
			break;
		case CFMTYPE_LCONFER://Leave Conference 
			{

			}
			break;
		case CFMTYPE_CONFER_CALL:
			{
				//通道操作
				CFMSG_CONFER_CALL recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, pMsg->szMessageBody ,sizeof(CFMSG_CONFER_CALL) );
				strMSG.Format("[DN:%d][ConferCall:%s][CallType:%d]", pMsg->nSMID, recMsg.szTel,recMsg.nCallType);
				m_pPubCObj->PC_ShowCommMsg( strMSG );

				//找到发送消息来的正确通道号码
				pChnlObj->m_nDialType = recMsg.nCallType;
				pChnlObj->m_strDialTel.Format("%s", recMsg.szTel );
				pChnlObj->m_nEventType	= EVENTTYPE_AGENT_CONFER_MSG;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");

			}
			break;
		case CFMTYPE_BUILD_CONFER:
			{
				//通道操作
				strMSG.Format("[MType:Build Conference][From DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg(strMSG );

				//找到发送消息来的正确通道号码
				pChnlObj->m_nEventType	= EVENTTYPE_AGENT_BUILDCONFER_MSG;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");

				strMSG.Format("[MType:Build Conference][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( strMSG );

			}
			break;
		case CFMTYPE_GETACTIVECALL:
			{
				//通道操作
				CFMSG_GETACTIVECALL recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, pMsg->szMessageBody ,sizeof(CFMSG_GETACTIVECALL) );
				strMSG.Format("[DN:%d][GetActiveCall:%s][CallType:%d]", pMsg->nSMID, recMsg.szTel,recMsg.nType);
				m_pPubCObj->PC_ShowCommMsg( strMSG );

				pChnlObj->m_nActiveType = recMsg.nType;//类型
				pChnlObj->m_strActiveTel = recMsg.szTel;//主叫号码
				pChnlObj->m_nEventType	= EVENTTYPE_AGENT_GETACTIVECALL;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
			}
			break;
		default:
			break;
	}


	return;
}

int CNorMarch32::Nor32_FindHoldChannel(int nIndex)
{
	int nChnlLink = -1;
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( nIndex == m_pChannelInfo[i].m_nLinkCh )
		{
			nChnlLink = i;
			break;
		}
	}

	return nChnlLink;
}

int CNorMarch32::Nor32_FindHoldChannel(int nIndex, CString strCallerID)
{
	if( "" == strCallerID ){
		return -1;
	}
	int nChnlLink = -1;
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( nIndex == m_pChannelInfo[i].m_nLinkCh && strCallerID == m_pChannelInfo[i].m_strCallerID )
		{
			nChnlLink = i;
			break;
		}
	}

	return nChnlLink;
}


int CNorMarch32::Nor32_FindUerChannel(CString strLocalDN)
{
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( strLocalDN == m_pChannelInfo[i].m_strLocalDN && CF_CHTYPE_USER == m_pChannelInfo[i].m_nChanelType )
		{
			return i;
		}
	}
	return -1;
}



int CNorMarch32::Nor32_FindActiveCall(int nIndex, int nType, CString strTel)
{

	if( 0 == nType )
	{//取得指定分机号码拥有的活动呼叫
		int m = -1;
		int n = -1;
		for( int i = 0; i < m_nTotalChannel; i++ )
		{
			n = m_pChannelInfo[i].m_nLinkCh;
			if( -1 != n )
			{
				if( strTel == m_pChannelInfo[i].m_strLocalDN && CF_CHTYPE_USER == m_pChannelInfo[i].m_nChanelType  )
				{
					m = i;
					break;
				}
			}
		}
		if( nIndex == m )
		{
			return -1;
		}
		
		if( -1 != m )
		{
			if( CHANNEL_STATE_FREE == m_pChannelInfo[m].m_nState )
			{
				return -1;
			}

			return n;
		}
		else
		{
			return -1;
		}
	}
	else
	{	
		//取得指定主叫号码的活动呼叫
		int m = -1;
		for( int i = 0; i < m_nTotalChannel; i++ )
		{
			if( strTel == m_pChannelInfo[i].m_strCallerID && CF_CHTYPE_USER != m_pChannelInfo[i].m_nChanelType  )
			{
				m = i;
				break;
			}
		}
		if( nIndex == m )
		{
			return -1;
		}
		
		if( -1 != m )
		{
			if( CHANNEL_STATE_FREE == m_pChannelInfo[m].m_nState )
			{
				return -1;
			}

			return m;
		}
		else
		{
			return -1;
		}
	}

	return -1;
}




void CNorMarch32::Nor32_Cus_AnswerCall(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom AnswerCall Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	

	CString str = pObj->m_strFunName;
	str.Replace("AnswerCall",NULL);
	CString strType = "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strType) )
	{
		strMSG.Format("%s  Custom AnswerCall is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}


	Nor32_MyOFFHook( nIndex);

	m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
	

	return;
}



void CNorMarch32::Nor32_Cus_Hold(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) 
	{
		strMSG.Format("Custom Hold error : CallHold are only used by the User channel,  ChIndex = %d ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom Hold Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	

	CString str = pObj->m_strFunName;
	str.Replace("Hold",NULL);
	CString strFile = "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strFile) )
	{
		strMSG.Format("%s  Custom Hold is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	int nDN = atoi( pChnlObj->m_strLocalDN );
	int nChnlLink = pChnlObj->m_nLinkCh;
	if( -1 >= nChnlLink || nChnlLink >= m_nTotalChannel  )
	{
		strMSG.Format("%s Custom Hold Channel index is error; ChIndex = %d ", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		CTI_HoldMsg( nDN, 1);
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;	
	}


	pChnlObj->m_strDTMFRecived = "";
	Nor32_MyStopPlay( nChnlLink );
	if( !Nor32_MyClearLink( nIndex, nChnlLink) )
	{
		Nor32_MyStopPlay( nChnlLink );
		Nor32_MySetLink( nIndex, nChnlLink, CF_CHSWITCH_CONNECT );
		strMSG.Format("Custom Hold error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		CTI_HoldMsg( nDN, 1);
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
	}
	if( "" != strFile ) //看是否播放音乐
	{
		if( !Nor32_MyStartPlayFile( nChnlLink,strFile))
		{
			Nor32_MySetLink( nIndex, nChnlLink , CF_CHSWITCH_CONNECT );
			strMSG.Format("Custom Hold error : ChIndex = %d", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			CTI_HoldMsg( nDN, 1);
			Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		m_pChannelInfo[nChnlLink].m_strPlayFile = strFile;
	}
	pChnlObj->m_nHoldChnl = nChnlLink;
	pChnlObj->m_nLinkCh = -1;
	m_pChannelInfo[nChnlLink].m_nState = CHANNEL_STATE_HOLD;
	m_pPubCObj->PC_ShowChannelState( nChnlLink, "Hold" );
	CTI_HoldMsg( nDN, 0);

	m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
	

	return;
}

void CNorMarch32::Nor32_Cus_PickupHold(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) 
	{
		strMSG.Format("Custom PickUpHold error : CallHold are only used by the User channel,  ChIndex = %d ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	int nDN = atoi( pChnlObj->m_strLocalDN );
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom PickUpHold Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 1,"","" );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	

	CString str = pObj->m_strFunName;
	str.Replace("PickUpHold",NULL);
	CString strType = "";
	CString strTel = "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strType,strTel) )
	{
		strMSG.Format("%s  Custom PickUpHold is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 1,"","" );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}


	
	int nChnlLink = -1;
	if( "0" == strType )
	{
		nChnlLink = Nor32_FindHoldChannel( nIndex);
		
	}
	else
	{
		nChnlLink = Nor32_FindHoldChannel( nIndex ,strTel);
	}
	if( 0 > nChnlLink || nChnlLink >= m_nTotalChannel  )
	{
		strMSG.Format("Custom PickUpHold error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 1,"","" );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	m_pChannelInfo[nChnlLink].m_nState = CHANNEL_STATE_CONNECT;
	m_pPubCObj->PC_ShowChannelState( nChnlLink, "Busy");
	Nor32_MyStopPlay(nIndex);
	Nor32_MyStopPlay(nChnlLink);
	if( !Nor32_MySetLink( nIndex, nChnlLink , CF_CHSWITCH_CONNECT ) )
	{
		strMSG.Format("Custom PickUpHold error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 1,"","" );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	pChnlObj->m_strCallerID = m_pChannelInfo[nChnlLink].m_strCallerID ;
	pChnlObj->m_strCalledID = m_pChannelInfo[nChnlLink].m_strCalledID ;

	pChnlObj->m_nLinkCh = nChnlLink;
	m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 0,m_pChannelInfo[nChnlLink].m_strCallerID ,m_pChannelInfo[nChnlLink].m_strCalledID );

	m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
	
	return;
}

void CNorMarch32::Nor32_Cus_TansferToIVR(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) 
	{
		strMSG.Format("Custom TansferToIVR error : CallHold are only used by the User channel,  ChIndex = %d ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	
	int nDN = atoi( pChnlObj->m_strLocalDN );
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom TansferToIVR Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		CTI_TransferMsg( nDN, 1, "" );//执行错误
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	

	CString str = pObj->m_strFunName;
	str.Replace("TransferToIVR",NULL);
	CString strNodeKey = "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strNodeKey) )
	{
		strMSG.Format("%s  Custom TansferToIVR is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		CTI_TransferMsg( nDN, 1, "" );//执行错误
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	
	int nChnlLink = pChnlObj->m_nLinkCh;
	if( -1 >= nChnlLink || nChnlLink >= m_nTotalChannel  )
	{
		strMSG.Format("%s Custom TansferToIVR Channel index is error; ChIndex = %d ", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		CTI_TransferMsg( nDN, 1, "" );//执行错误
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;	
	}


	Nor32_MyStopPlay( nChnlLink );
	if( !Nor32_MyClearLink( nIndex, nChnlLink) )
	{
		Nor32_MyStopPlay( nChnlLink );
		Nor32_MySetLink( nIndex, nChnlLink, CF_CHSWITCH_CONNECT );
		strMSG.Format("Custom Hold error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		CTI_TransferMsg( nDN, 1, "" );//执行错误
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
	}
	

	if( m_pPubCObj->PC_TransferToIVR( nChnlLink, atoi(strNodeKey),&(m_pChannelInfo[nChnlLink]) ) )
	{
		pChnlObj->m_nHoldChnl = nChnlLink;
		pChnlObj->m_nLinkCh = -1;
		m_pChannelInfo[nChnlLink].m_nLinkCh = -1;
		CTI_TransferMsg( nDN, 0, "" );

		m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
	}
	else
	{
		Nor32_MyStopPlay( nChnlLink );
		Nor32_MySetLink( nIndex, nChnlLink, CF_CHSWITCH_CONNECT );
		strMSG.Format("Custom Hold error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		CTI_TransferMsg( nDN, 1, "" );//执行错误
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
	}

	return;
}

void CNorMarch32::Nor32_Cus_DisMonitor(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	int nDN = atoi( pChnlObj->m_strLocalDN );
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom DisMonitor Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_DisMonitorResponseMsg( nDN, 1 );
		return ;  
	}

	Nor32_MyExitConference(  nIndex );
	m_pPubCObj->PC_CTI_DisMonitorResponseMsg( nDN, 0 );
	m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);

	return;
}

void CNorMarch32::Nor32_Cus_Monitor(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	int nDN = atoi( pChnlObj->m_strLocalDN );
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom Monitor Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_MonitorResponseMsg( nDN, 1 );
		return ;  
	}
	
	CString str = pObj->m_strFunName;
	str.Replace("Monitor",NULL);
	CString strDN	= "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strDN ) )
	{
		strMSG.Format("%s  Custom Monitor is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_MonitorResponseMsg( nDN, 1 );
		return;
	}
	
	int n = Nor32_FindUerChannel( strDN );
	if( 0 > n || n >= m_nTotalChannel  )
	{
		strMSG.Format("Custom Monitor Error; ChIndex = %d it is error that to GetActiveCall! %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_MonitorResponseMsg( nDN, 1 );
		return ;   	
	}

	int nGroup = Nor32_MyGetConferenceNo( n );
	if( nGroup < 0 )
	{
		strMSG.Format("%s Custom Monitor ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_MonitorResponseMsg( nDN, 1 );
		return;
	}
			
	if( Nor32_MyListenConference( nIndex, nGroup ) )
	{
		pChnlObj->m_nState = CHANNEL_STATE_MONITORING;
		m_pPubCObj->PC_CTI_MonitorResponseMsg( nDN, 0 );
		m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
		return;
	}
	else
	{
		strMSG.Format("%s Custom Monitor ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_MonitorResponseMsg( nDN, 1 );
		return;
	}
	return;
}

void CNorMarch32::Nor32_Cus_JoinConfer(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	int nDN = atoi( pChnlObj->m_strLocalDN );
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom JoinConfer Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_JoinConferenceResponseMsg( nDN, 1 );
		return ;  
	}
	
	CString str = pObj->m_strFunName;
	str.Replace("JoinConfer",NULL);
	CString strDN	= "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strDN ) )
	{
		strMSG.Format("%s  Custom JoinConfer is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_JoinConferenceResponseMsg( nDN, 1 );
		return;
	}
	
	int n = Nor32_FindUerChannel( strDN );
	if( 0 > n || n >= m_nTotalChannel  )
	{
		strMSG.Format("Custom JoinConfer Error; ChIndex = %d it is error that to GetActiveCall! %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_JoinConferenceResponseMsg( nDN, 1 );
		return ;   	
	}

	int nGroup = Nor32_MyGetConferenceNo( n );
	if( nGroup < 0 )
	{
		strMSG.Format("%s Custom JoinConfer ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_JoinConferenceResponseMsg( nDN, 1 );
		return;
	}
			
	if( Nor32_MyJoinConference( nIndex, nGroup ) )
	{
		pChnlObj->m_nState = CHANNEL_STATE_MONITORING;
		m_pPubCObj->PC_CTI_JoinConferenceResponseMsg( nDN, 0 );
		m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
		return;
	}
	else
	{
		strMSG.Format("%s Custom JoinConfer ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		Nor32_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_JoinConferenceResponseMsg( nDN, 1 );
		return;
	}
	return;
}
