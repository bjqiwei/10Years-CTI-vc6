// DJTc08a32.cpp: implementation of the CDJTc08a32 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DJTc08a32.h"
#include "CalculateExpress.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDJTc08a32::CDJTc08a32()
{
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



	m_pNormalScriptInfo = NULL;
	m_nNormalScriptNum	= 0;//普通脚本文件数量
	m_lSeting = TC08_PlayDTMF( TC08_PlayDTMF(g_pD08a->GetVoiceCardKey()) );//SetNum
	m_pwndProg = NULL;


}

CDJTc08a32::~CDJTc08a32()
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

	if( 1 == m_nFax )
	{
		DJFax_DisableCard();
	}

	if(m_bInitVoiceCard )
	{
		//如果初始化语音卡成功
		TC08_ExitVoiceSys();
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


bool CDJTc08a32::TC08_InitSystem(CListCtrl	*plstChannel,
								CListBox	*plistSystem,
								CListCtrl	*plistScript,
								CListBox	*pCommList,
								CListCtrl	*plistTCPIP,
								CListCtrl	*plstConfer,
								CListCtrl	*plstFax
								)
{


	CString strMSG("");
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

	if( !TC08_LoadSysScript()) return false;
	if( !TC08_LoadNormalScript()) return false;
	m_pwndProg->StepIt();//进度条显示


	long DriverOpenFlag = 0;
	DriverOpenFlag = LoadDRV ( );
	if ( DriverOpenFlag )
	{
		return false;
	}

	
	PostMessage( m_hWnd , WM_MY_FRIEND_MSG, 0, NewReadPass( 0 ));
	m_nTotalChannel = CheckValidCh();

	if ( EnableCard( m_nTotalChannel, 1024*32 ) != (long)0)
	{
		FreeDRV();
		return false;
	}

	/*if( 1 == m_nFax )
	{
		//系统有传真卡
		//注意在使用数字卡或模拟卡时，注意
		if ( DJFax_DriverReady(2048) ) 
		{
			DisableCard();
			FreeDRV();
			m_pPubCObj->PC_ShowSysMsg(0,"Init fax Driver Fail");
			return FALSE;
		}
		m_nTotalFaxLine = DJFax_GetTotalFaxChnl();
		m_pPubCObj->m_plstFax->DeleteAllItems();
		for( int i = 0; i < m_nTotalFaxLine; i++ )
		{
			strMSG.Format("%d",i);
			m_pPubCObj->m_plstFax->InsertItem(i,strMSG );
			m_pPubCObj->m_plstFax->SetItemText(i,1,"Free");
			m_pPubCObj->m_plstFax->SetItemText(i,2,"0");
			m_pPubCObj->m_plstFax->SetItemText(i,3,"");

		}
	}*/

	int r = DConf_EnableConfCard();
	if ( r != 0 ) 
	{
		DisableCard();
		FreeDRV();
		m_pPubCObj->PC_ShowSysMsg( 0, "Enable conference board Fail" );
		return FALSE;
	}
	
	Sig_Init(0);
	SetPackRate ( PACK_64KBPS );

	CChiefIVRDoc doc;
	CString strfile = "";
	strfile = m_strPath + "\\CFCHDJ.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		DisableCard();
		FreeDRV();
		return false;
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
			if( !TC08_InitChannel( pObj , m ) )
			{
				break;
			}
		}
		m_pwndProg->StepIt();//进度条显示
	}
	

	
	//会议组显示，东进模拟卡会议组数只能最大为MAX_GROUP_NUM；
	//每组会议最多只能有MAX_CH_PER_GROUP个资源
	for ( int i = 0; i < MAX_GROUP_NUM; i ++ )
	{
		str.Format("%d",i);
		m_pPubCObj->m_plstConfer->InsertItem(i,str);
		str.Format("%d",MAX_CH_PER_GROUP);

	}

	return true;
}

void CDJTc08a32::TC08_ExitVoiceSys()
{

	for( int i = 0; i <  m_nTotalChannel; i++ )
	{
		switch( m_pChannelInfo[i].m_nChanelType )
		{
		case CF_CHTYPE_USER:
			TC08_ResetUserChannel(i);
			break;
		case CF_CHTYPE_TRUNK:
			TC08_ResetTrunkChannel(i);
			break;
		case CF_CHTYPE_EMPTY:
			break;
		case CF_CHTYPE_RECORD:
			break;
		}

	}
	DisableCard();
	DConf_DisableConfCard();
	FreeDRV();



	return;
}


void CDJTc08a32::TC08_DoWork(int nIndex )
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_EMPTY == pChnlObj->m_nChanelType ) return;
	if( CF_CHTYPE_NOKNOW == pChnlObj->m_nChanelType ) return;

	CNodeObj *pNodeObj = pChnlObj->GetCurrentNodeObj();
	if( NULL == pNodeObj ) return;

	//处理处于Hold状态的通道
	switch( pChnlObj->m_nState )
	{
	case CHANNEL_STATE_HOLD:
	case CHANNEL_STATE_TRANSFER:
		if( "" != pChnlObj->m_strPlayFile )
		{
			if( TC08_MyCheckPlayEnd(nIndex) )
			{
				
				TC08_MyStartPlayFile( nIndex,pChnlObj->m_strPlayFile);
			}
		}
		TC08_MyHangUpDetect( nIndex );//挂机检测
		break;
	default:
		break;
	}

	if( pChnlObj->m_bRecordLoop )
	{
		if( 0 == pChnlObj->m_nRecordType )//录音类型：0普通 1会议
		{
			if( TC08_MyCheckRecordEnd( nIndex ) )
			{
				TC08_MyStopRecordFile( nIndex );
				
			}
		}
		else
		{
			if( TC08_MyCheckRecordEnd( nIndex ) )
			{
				TC08_MyStopRecordFile( nIndex );
				
			}
		}

	}

	if( pChnlObj->m_bPlayLoop )
	{
		if( 0 == pChnlObj->m_nPlayType )//录音类型：0普通 1会议
		{
			if( "" != pChnlObj->m_strPlayFile )
			{
				if( TC08_MyCheckPlayEnd(nIndex) )
				{
					
					TC08_MyStartPlayFile( nIndex,pChnlObj->m_strPlayFile,TRUE,0);
				}
			}
		}
		else
		{
			if( "" != pChnlObj->m_strPlayFile )
			{
				if( TC08_MyCheckPlayEnd(nIndex) )
				{

					TC08_MyStartPlayFile( nIndex,pChnlObj->m_strPlayFile,TRUE,1);
				}
			}
		}
	}




	switch( pNodeObj->m_nType )	
	{
	case NODETYPE_START:
		TC08_MoveNext_Node( nIndex, NEXTTYPE_NEXT );//查找该结点分支指向的的结点
		break;
	case NODETYPE_WAITCALL:
		TC08_WaitCall( nIndex );
		break;
	case NODETYPE_START_PLAY:
		TC08_PlayStart( nIndex );
		break;
	case NODETYPE_CHECK_DIAL:
		TC08_DialCheck( nIndex );
		break;
	case NODETYPE_RELEASECALL:
		TC08_ReleaseCall( nIndex );
		break;
	case NODETYPE_CALLHOLD:
		TC08_CallHold( nIndex );
		break;
	case NODETYPE_CHECK_TRANSFER:
		TC08_Transfer( nIndex );
		break;
	case NODETYPE_START_RECORD:
		TC08_StartRecord( nIndex );
		break;
	case NODETYPE_SETVARIABLE:
		m_pPubCObj->PC_SetVariable( nIndex,pChnlObj);
		break;
	case NODETYPE_COMPARE:
		m_pPubCObj->PC_Compare( nIndex,pChnlObj );
		break;
	case NODETYPE_ENTER_CONFERENCE:
		TC08_ConferenceEnter(nIndex);
		break;
	case NODETYPE_LEAVE_CONFERENCE:
		TC08_ConferenceLeave(nIndex);
		break;
	case NODETYPE_FAXRECIVE:
		TC08_FaxRecive( nIndex );
		break;
	case NODETYPE_FAXSEND:
		TC08_FaxSend( nIndex );
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
		TC08_DTMFSend( nIndex );
		break;
	case NODETYPE_RECIVE_DTMF:
		TC08_DTMFRecive( nIndex );
		m_pPubCObj->PC_ShowChannelDTMF( nIndex,pChnlObj->m_strDTMFRecived);
		break;
	case NODETYPE_EVENT_CHECK:
		TC08_EventCheck( nIndex );
		return;
		break;
	case NODETYPE_DATABASE_BROWSE:
		m_pPubCObj->PC_DBBrowse( nIndex,pChnlObj );
		break;
	case NODETYPE_DTMF_MENU:
		TC08_DTMFMenu( nIndex );
		m_pPubCObj->PC_ShowChannelDTMF( nIndex,pChnlObj->m_strDTMFRecived);
		break;
	case NODETYPE_CALLLOGREQUEST:
		TC08_CallLogRequest( nIndex );
		break;
	case NODETYPE_NODE_END:
		TC08_NodeEnd( nIndex );
		break;
	case NODETYPE_SWITCH:
		TC08_Switch( nIndex );	
		break;
	case NODETYPE_ANSWERCALL:
		TC08_AnswerCall( nIndex );	
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
		TC08_EndCallEventCheckBlock( nIndex );
		break;
	case NODETYPE_ENDCALLEVENT_END:
		TC08_EndCallEventEndBlock( nIndex );
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
		TC08_Custom(  nIndex );
		break;
	default:
		break;

	}

	//挂机检测
	TC08_MyHangUpDetect( nIndex );

	return;
}

void CDJTc08a32::TC08_MyHangUpDetect(int nIndex )
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return;
	}
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_TRUNK == pChnlObj->m_nChanelType  ){
	

		if( CHANNEL_STATE_FREE == pChnlObj->m_nState   ) 
			return;
		if( CHANNEL_STATE_DIAL == pChnlObj->m_nState   ) 
			return;
		/********************************************/
		//Sig_CheckBusy(nChnl)不能同Sig_CheckDial同时使用
		/********************************************/
		if ( Sig_CheckBusy(nIndex) )
		{
			pChnlObj->m_nHangUpType = 1;//设置主动挂机
			TC08_ResetTrunkChannel( nIndex );
			TC08_MyHangUp(nIndex);
			
		}
		return;
	}
	else if( CF_CHTYPE_USER == pChnlObj->m_nChanelType || CHANNEL_STATE_MONITORING ==  pChnlObj->m_nState ){
		if( CHANNEL_STATE_FREE == pChnlObj->m_nState )  return;
		if( CHANNEL_STATE_RINGING == pChnlObj->m_nState) return;
		//内线挂机和拍插簧检测
		int nResult = HangUpDetect(nIndex);
		if( HANG_UP_FLAG_TRUE == nResult )//挂机
		{
			pChnlObj->m_nHangUpType = 1;//设置主动挂机
			m_pPubCObj->PC_CTI_HangUpMsg( nIndex, pChnlObj );
			TC08_ResetUserChannel( nIndex );
			return;
		}
	
	}
	else if( CF_CHTYPE_RECORD == pChnlObj->m_nChanelType ){
		if( CHANNEL_STATE_FREE == pChnlObj->m_nState ) return;
		if( CHANNEL_STATE_RINGING == pChnlObj->m_nState) return;
		if( 0 == DRec_OffHookDetect(nIndex) ) 
			TC08_ResetUserChannel( nIndex );

	}

	return;
}
void CDJTc08a32::TC08_WaitCall(int nIndex )
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nType = pChnlObj->m_nChanelType;
	if( CF_CHTYPE_USER == nType || CF_CHTYPE_EMPTY == nType ) 
	{
		strMSG.Format("Error: WaitCall are only used by the trunk channel; ChIndex = %d ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	CWaitCallObj *pObj = (CWaitCallObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CWaitCallObj) ) )
	{
		strMSG.Format("ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	if( CF_CHTYPE_TRUNK == nType )
		TC08_WaitCall_Trunk( nIndex, pObj );
	else if( CF_CHTYPE_RECORD == nType )
		TC08_WaitCall_Record( nIndex, pObj );

	return;
}
void CDJTc08a32::TC08_WaitCall_Trunk(int nIndex, CWaitCallObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nStep = pChnlObj->m_nStep;
	switch (nStep)
	{
	case 0:
		if(RingDetect( nIndex )) 
		{
			if( pObj->m_bClearDTMF ){
				TC08_MyInitDtmfBuf( nIndex );
			}
			ResetCallerIDBuffer( nIndex );//必须调用它，才能接收主叫
			pChnlObj->m_strCallerID	= "";
			pChnlObj->m_strCalledID	= "";
			pChnlObj->m_nRings	= 1;
			pChnlObj->m_nStep	= 1;
			
			m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing" );
			pChnlObj->m_nState		= CHANNEL_STATE_RINGING;
			pChnlObj->m_nEventType	=  EVENTTYPE_RINGING;
			m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
			m_pPubCObj->PC_StartTimer( pChnlObj );
		}
		break;
	case 1:
		{
            bool bOffHook = false;
			char szCallerID[128];
			memset( szCallerID, 0 , sizeof(szCallerID) );
			pChnlObj->m_strCallerID	= "";
			pChnlObj->m_strCalledID	= "";
			if(RingDetect(nIndex)) 
			{
				pChnlObj->m_nRings += 1;
				if( pChnlObj->m_nRings > pObj->m_nRings )
				bOffHook=true;
			}
            int a = GetCallerIDStr ( nIndex , szCallerID );
            if( a == 3 ){
                bOffHook=true;
			}
            else if( a == 4 ){
                bOffHook=true;
			}
            if(bOffHook)
            {
				//if( strlen( szCallerID ) > 7 )
				//{
					pChnlObj->m_strCallerID = &(szCallerID[8]) ;
					m_pPubCObj->PC_ShowChannelCallerID(nIndex,pChnlObj->m_strCallerID);
					if( pObj->m_bRecCallerID )
						m_pPubCObj->PC_SaveValuesToVar(pChnlObj,pChnlObj->m_strCallerID, pObj->m_CallerID.m_nKey );
					
				//}
				pChnlObj->m_nStep	= 2;
				return;

            }
            if( m_pPubCObj->PC_ElapseTime(pChnlObj) > 15  )//每个回铃音为5秒
			{
				TC08_ResetTrunkChannel( nIndex );
				TC08_MyHangUp(nIndex);
			}
		}
		break;
	case 2:
		{
			if(RingDetect(nIndex)) 
			{
				TC08_MyOFFHook(nIndex);
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
				pChnlObj->m_nEventType =  EVENTTYPE_OFFHOOK;
				pChnlObj->m_nState =  CHANNEL_STATE_CONNECT;
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
            else if( m_pPubCObj->PC_ElapseTime(pChnlObj) > 15  )//每个回铃音为5秒
			{
				TC08_ResetTrunkChannel( nIndex );
				TC08_MyHangUp(nIndex);
			}
		}
	default:
		break;
	}
	return;
}

void CDJTc08a32::TC08_WaitCall_Record(int nIndex, CWaitCallObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int		r = 0;
	int nStep = pChnlObj->m_nStep;
	switch (nStep)
	{
	case 0:
		r = RingDetect(nIndex);
		if( 0 == r ) 
		{
			pChnlObj->m_strCallerID	= "";
			pChnlObj->m_strCalledID	= "";
			pChnlObj->m_nCallType			= CHANNEL_CALLTYPE_COMEIN;
			pChnlObj->m_nRings				= 1;
			pChnlObj->m_nStep				= 1;
			pChnlObj->m_nState				= CHANNEL_STATE_RINGING;
			pChnlObj->m_nEventType			=  EVENTTYPE_RINGING;
			pChnlObj->m_strtBeginTime		= (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
			m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
			pChnlObj->m_strRingTime = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
			m_pPubCObj->PC_StartTimer( pChnlObj);
		}

		break;
	case 1:
		r = DRec_OffHookDetect( nIndex );
		if ( r == 1 )
		{	// 监视的通道摘机，收主叫号码
			GetCallerIDStr ( nIndex, (LPTSTR)(LPCTSTR)pChnlObj->m_strCallerID );
			if( pObj->m_bRecCallerID )
				m_pPubCObj->PC_SaveValuesToVar( nIndex, pChnlObj, pChnlObj->m_strCallerID, pObj->m_CallerID.m_nKey );
			if( pObj->m_bRecCalledID )
				m_pPubCObj->PC_SaveValuesToVar( nIndex,pChnlObj,  pChnlObj->m_strCalledID, pObj->m_CalledID.m_nKey );
			if( pObj->m_bClearDTMF ) 
			{
				
				TC08_MyInitDtmfBuf( nIndex );
			}
			
			pChnlObj->m_nEventType =  EVENTTYPE_OFFHOOK;
			pChnlObj->m_nState		=  CHANNEL_STATE_CONNECT;
			pChnlObj->m_nStep		= 2;
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
			return;
		}
		else
		{
			if( m_pPubCObj->PC_ElapseTime(pChnlObj) > 30 )
			{
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}
		}
		break;
	case 2:
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		break;
	default:
		break;
	}
	return;
}

void CDJTc08a32::TC08_ResetTrunkChannel(int nIndex )
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nLinkIndex = pChnlObj->m_nLinkCh;
	
	TC08_MyStopRecordFile(nIndex);
	TC08_MyStopPlay(nIndex);
	TC08_MyConferLeave( nIndex, pChnlObj->m_nConfNO );
	if( -1 < nLinkIndex && nLinkIndex < m_nTotalChannel  )
	{
		if( CHANNEL_STATE_HOLD != pChnlObj->m_nState && CHANNEL_STATE_MONITORING !=  pChnlObj->m_nState )
		{
			TC08_MyClearLink( nIndex, nLinkIndex );
			if( CF_CHTYPE_TRUNK == m_pChannelInfo[nLinkIndex].m_nChanelType )
			{
				TC08_MyHangUp(nLinkIndex);
				m_pChannelInfo[nLinkIndex].m_nEventType = EVENTTYPE_HANGUP;
			}
			if( CF_CHTYPE_USER == m_pChannelInfo[nLinkIndex].m_nChanelType )
			{
				TC08_MyHangUp(nLinkIndex);
			}
			TC08_MyConferLeave( nLinkIndex, m_pChannelInfo[nLinkIndex].m_nConfNO );
			m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Free" );
			m_pChannelInfo[nLinkIndex].MoveCallEndBlock_Node();

		}


	}

	if( 1 == m_nFax )
	{
		int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
		if( -1 != nFaxChnl )
		{
			DJFax_StopFax(nFaxChnl);
			DJFax_ClearLink(nFaxChnl,nIndex);
			m_pFaxChannel[nFaxChnl].m_nCurrPage		= 0;
			m_pFaxChannel[nFaxChnl].m_nStatus			= 0;
			m_pFaxChannel[nFaxChnl].m_nTotalPage		= 0;
			m_pFaxChannel[nFaxChnl].m_strFaxFileName = "";
			m_pPubCObj->PC_ShowFaxUpdate(nFaxChnl,&(m_pFaxChannel[nFaxChnl]));
		}
	}

	TC08_MyHangUp(nIndex);
	m_pPubCObj->PC_ShowChannelState( nIndex, "Free" );
	m_pPubCObj->PC_ShowConnectToChnl( nIndex, -1 );
	pChnlObj->MoveCallEndBlock_Node();
	return;
}

void CDJTc08a32::TC08_ResetUserChannel( int nIndex )
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nLinkIndex = pChnlObj->m_nLinkCh;
	int nTransferCh = pChnlObj->m_nTransferCh;
	
	TC08_MyStopRecordFile(nIndex);
	TC08_MyStopPlay(nIndex);
	TC08_MyConferLeave( nIndex, pChnlObj->m_nConfNO );
	if( -1 < nLinkIndex && nLinkIndex < m_nTotalChannel  )
	{
		if( CHANNEL_STATE_HOLD != pChnlObj->m_nState && CHANNEL_STATE_MONITORING !=  pChnlObj->m_nState )
		{
			TC08_MyClearLink( nIndex, nLinkIndex );
			if( CF_CHTYPE_TRUNK == m_pChannelInfo[nLinkIndex].m_nChanelType )
			{
				TC08_MyHangUp(nLinkIndex);
				m_pChannelInfo[nLinkIndex].m_nEventType = EVENTTYPE_HANGUP;
				
			}
			else if( CF_CHTYPE_USER == m_pChannelInfo[nLinkIndex].m_nChanelType )
			{
				TC08_MyHangUp(nLinkIndex);
			}
			TC08_MyConferLeave( nLinkIndex, m_pChannelInfo[nLinkIndex].m_nConfNO );
			m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Free" );
			m_pChannelInfo[nLinkIndex].MoveCallEndBlock_Node();
		}

	}

	if( 1 == m_nFax )
	{
		int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
		if( -1 != nFaxChnl )
		{
			DJFax_StopFax(nFaxChnl);
			DJFax_ClearLink(nFaxChnl,nIndex);
			m_pFaxChannel[nFaxChnl].m_nCurrPage		= 0;
			m_pFaxChannel[nFaxChnl].m_nStatus			= 0;
			m_pFaxChannel[nFaxChnl].m_nTotalPage		= 0;
			m_pFaxChannel[nFaxChnl].m_strFaxFileName = "";
			m_pPubCObj->PC_ShowFaxUpdate(nFaxChnl,&(m_pFaxChannel[nFaxChnl]));
		}
	}

	if( -1 < nTransferCh && nTransferCh < m_nTotalChannel  )
	{
		FeedPower( nTransferCh );
		m_pPubCObj->PC_ShowChannelState( nTransferCh, "Free" );
		m_pChannelInfo[nTransferCh].MoveCallEndBlock_Node();
		if( CF_CHTYPE_USER == m_pChannelInfo[nTransferCh].m_nChanelType )
			m_pPubCObj->PC_CTI_HangUpMsg( nTransferCh, &m_pChannelInfo[nTransferCh] );
	}


	FeedPower( nIndex );
	TC08_MyInitDtmfBuf( nIndex );
	m_pPubCObj->PC_ShowChannelState( nIndex, " Free");
	m_pPubCObj->PC_ShowConnectToChnl( nIndex, -1 );
	m_pPubCObj->PC_CTI_HangUpMsg( nIndex, pChnlObj );
	pChnlObj->MoveCallEndBlock_Node();
	
	return;
}

BOOL CDJTc08a32::TC08_PlayExpressVox(int nIndex, CString strExpress )
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
			char *pszFileName = (LPTSTR)(LPCTSTR)strFileName;
			if( !AddIndexPlayFile( nIndex , pszFileName ) )
			{
				return FALSE;
			}
		}

	}

	return TRUE;
}

void CDJTc08a32::TC08_PlayStart(int nIndex )
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	short int code = -1;
	CPlayStartObj *pObj = (CPlayStartObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CPlayStartObj) ) )
	{
		strMSG.Format("ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	int nStep = pChnlObj->m_nStep;
	switch(nStep)
	{
	case 0:
		{
			if( pObj->m_bClearDTMF ) 
			{
				TC08_MyInitDtmfBuf( nIndex );
			}
			TC08_MyStopPlay(nIndex);
			CNodeDataList *pDadaList = &(pObj->m_PlayList);
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
					char *pszFileName = (LPTSTR)(LPCTSTR)strMSG;
					if( !AddIndexPlayFile( nIndex , pszFileName ) )
					{
						strMSG.Format("%s: ChIndex = %d    AddIndexPlayFile error  %s",  pObj->m_strCaption, nIndex,pszFileName);
						m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
						TC08_MyStopPlay(nIndex);
						TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
				}
				else
				{
					//表达式
					if(! TC08_PlayExpressVox( nIndex, pData->m_strData ) )
					{
						strMSG.Format("%s: ChIndex = %d    Play Express Error :%s ",  pObj->m_strCaption, nIndex, pData->m_strData );
						m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
						TC08_MyStopPlay(nIndex);
						TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
				}
			}//end while
			if( !StartIndexPlayFile( nIndex ) )
			{
				strMSG.Format("%s: ChIndex = %d    StartIndexPlayFile error",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MyStopPlay(nIndex);
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}
			/*********************/
			TC08_TCPIP(nIndex);
			/********************/
			pChnlObj->m_nStep = 1;
		}
		break;
	case 1:
		{
			if( TC08_GetDtmfCode( nIndex, pChnlObj->m_strDTMFRecived) )
			{
				if( pObj->m_bPlayStop )
				{
					TC08_MyStopPlay( nIndex );
					pChnlObj->m_nStep = 2;
				}
			}
			else if( CheckIndexPlayFile(nIndex)  )
			{
				TC08_MyStopPlay( nIndex );
				pChnlObj->m_nStep = 2;
			}
		}
		break;
	case 2:
		{	
			if( pObj->m_bRecDTMF )
			{
				int nVarKey = pObj->GetVarKey( pObj->m_strSaveDTMF );
				BOOL bResult =m_pPubCObj->PC_SaveValuesToVar( nIndex, pChnlObj, pChnlObj->m_strDTMFRecived, nVarKey );
				if( !bResult )
				{
					strMSG.Format("%s Save DTMF to Var Error ;ChIndex = %d",pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					TC08_MyStopPlay(nIndex);
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
			}

			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );	

		}
		break;
	default:
		{
			strMSG.Format("%s: ChIndex = %d    Play error ",  pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		}
		break;
	}//end switch

	return;
}

int CDJTc08a32::TC08_GetHoldChnl(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nLinkIndex = -1;
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( nIndex == pChnlInfo->m_nLinkCh )
		{
			nLinkIndex = i;
			break;
		}
	}
	return nLinkIndex;
}
char CDJTc08a32::TC08_GetCodeAscii(int code )
{
	switch(code)
	{
	case 0:		return 'D';		break;
	case 1:		return '1';		break;
	case 2:		return '2';		break;
	case 3:		return '3';		break;
	case 4:		return '4';		break;
	case 5:		return '5';		break;
	case 6:		return '6';		break;
	case 7:		return '7';		break;
	case 8:		return '8';		break;
	case 9:		return '9';		break;
	case 10:	return '0';		break;
	case 11:	return '*';		break;
	case 12:	return '#';		break;
	case 13:	return 'A';		break;
	case 14:	return 'B';		break;
	case 15:	return 'C';		break;
	default:	return -1;
	}
}

void CDJTc08a32::TC08_DTMFMenu(int nIndex )
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CDTMFMenuObj *pObj = (CDTMFMenuObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDTMFMenuObj) ) )
	{
		strMSG.Format("DTMFMenu Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	int nStep = pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			/*********************/
			TC08_FaxKey(nIndex);
			/********************/
			if( pObj->m_bClearDTMF )
			{
				TC08_MyInitDtmfBuf( nIndex );
				
			}
			pChnlObj->m_nState  = CHANNEL_STATE_CONNECT ; 
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
			pChnlObj->m_nStep = 1;
			m_pPubCObj->PC_StartTimer( pChnlObj );
			
		}
		break;
	case 1:
		{
			if( TC08_GetDtmfCode( nIndex, pChnlObj->m_strDTMFRecived ) )
			{
				pChnlObj->m_nEventType = EVENTTYPE_RECDTMF;
				m_pPubCObj->PC_ShowChannelDTMF( nIndex,pChnlObj->m_strDTMFRecived);
				StartPlaySignal( nIndex, SIG_STOP);
			}

			if( m_pPubCObj->PC_ElapseTime( pChnlObj ) > pObj->m_nTimeOut )
			{
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_DTMF_TIMEOUT );
				StartPlaySignal( nIndex,SIG_STOP);
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
				TC08_MoveNext_Node( nIndex,  nNextType );
				StartPlaySignal( nIndex ,SIG_STOP);
				return;
			}
		}
		break;
	default:
		break;
	}
	return;
}

void CDJTc08a32::TC08_ReleaseCall(int nIndex )
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_TRUNK == pChnlObj->m_nChanelType )
		TC08_ReleaseCall_Trunk(nIndex);
	else if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
		TC08_ReleaseCall_User(nIndex);
	TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	return;
}

void CDJTc08a32::TC08_CallLogRequest(int nIndex )
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CCallLogRequestObj *pObj = (CCallLogRequestObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CCallLogRequestObj) ) )
	{
		strMSG.Format("CallLogRequest ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	bool bError = true;
	if( 0 == pObj->m_nRequestType ) //Begin Save Call Log
	{
		pChnlObj->m_strtBeginTime		= (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
	}
	else
	{
		pChnlObj->m_strtEndTime		= (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
	}

	TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	return;
}

void CDJTc08a32::TC08_DTMFRecive(int nIndex )
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CDTMFReciveObj *pObj = (CDTMFReciveObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDTMFReciveObj) ) )
	{
		strMSG.Format("DTMF Recive Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
			/*********************/
			TC08_TCPIP(nIndex);
			/********************/
			m_pPubCObj->PC_StartTimer(pChnlObj);
			if( pObj->m_bClearDTMF ) 
			{
				TC08_MyInitDtmfBuf( nIndex );
				
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
			/*********************/
			TC08_FaxKey(nIndex);
			/********************/
			if( TC08_GetDtmfCode( nIndex, strMSG ) )
			{
				StartPlaySignal( nIndex,SIG_STOP);
				if( 0 == pObj->m_nEndCondition )
				{
					//通过判断接收按键的最大数量来结束收键
					if(  pObj->m_nDTMFType > 0 )//只接收数字键
					{
						if( "*" == strMSG )  return;
						if( "#" == strMSG )  return;
					}
					pChnlObj->m_strDTMFRecived += strMSG;
					int  len = strlen( pChnlObj->m_strDTMFRecived );
					if( len  >= pObj->m_nDTMFCount )
					{
						pChnlObj->m_nStep = 2;
						return;
					}

				}
				else
				{
					//通过判断接收特定按键值来结束收键
					if( strMSG == pObj->m_strDTMF )
					{
						pChnlObj->m_nStep = 2;
						return;
					}
					else
					{
						if(  pObj->m_nDTMFType > 0 )//只接收数字键
						{
							if( "*" == strMSG )  return;
							if( "#" == strMSG )  return;

						}
						pChnlObj->m_strDTMFRecived += strMSG;

					}
				}
				pChnlObj->m_nEventType = EVENTTYPE_RECDTMF;
				
			}
			long n = m_pPubCObj->PC_ElapseTime(pChnlObj);
			if( n >= pObj->m_nTimeOut )
			{
				m_pPubCObj->PC_SaveValuesToVar( nIndex, pChnlObj, pChnlObj->m_strDTMFRecived, pObj->m_SaveDTMF.m_nKey );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				strMSG.Format("%s TimeOut Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				return;
			}
		}
		break;
	case 2:
		{
			BOOL bResult = m_pPubCObj->PC_SaveValuesToVar( nIndex, pChnlObj, pChnlObj->m_strDTMFRecived, pObj->m_SaveDTMF.m_nKey );
			if( bResult )
			{
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
			else
			{
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				strMSG.Format("%s Save DTMF to Var Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			}
			
		}
	default:
		break;
	}


	return;
}


int CDJTc08a32::TC08_FindFreeTrunk(  )
{
	static int nCurrentTrunkNo = 0 ;
	int i = 0;
	int nType = 0;
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

void CDJTc08a32::TC08_DialCheck(int nIndex )
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
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
							TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
							return;
					}
				}
				
				memmove( szPhone, strPhone, strlen(strPhone) >= 32 ? 31:strlen(strPhone) );
				if( strlen(szPhone) < 3 )
				{
						//号码长度小于3时，被认为号码错误
						strMSG.Format("%s  Phone Error; ChIndex = %d", pObj->m_strCaption, nIndex);
						m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
						TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
				}
			}
			if( !TC08_MyMakeCall( nIndex,szPhone, (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN  ) )
			{
					strMSG.Format("%s  Make call Error; ChIndex = %d", pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
			}
			pChnlObj->m_nStep = 1;
		}
		break;
	case 1:
		{
			/*********************/
			TC08_TransferKey(nIndex);
			/********************/
			int nCallOutResult = TC08_MyMakeCall_Check( nIndex );
			if( 3 == nCallOutResult )//未得到结果
				return;//未得到结果
			else if( 0 == nCallOutResult )
			{
				//连接成功
				pChnlObj->m_strState ="Connect";
				pChnlObj->m_nState	 = CHANNEL_STATE_CONNECT;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Connect" );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_CONNECT );
				return;
			}
			else
			{
				//超时或失败
				pChnlObj->m_nState			= CHANNEL_STATE_FREE;
				pChnlObj->m_nEventType		=  EVENTTYPE_HANGUP;
				strMSG.Format("%s  Make call Error; ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

		}
		break;
	default:
		break;
	}

	return;
}

void CDJTc08a32::TC08_CallHold(int nIndex )
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);

	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) 
	{
		strMSG.Format("CallHold error : CallHold are only used by the User channel,  ChIndex = %d ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	CCallHoldObj *pObj = (CCallHoldObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CCallHoldObj) ) )
	{
		strMSG.Format("CallHold error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	
	int nDN = atoi( pChnlObj->m_strLocalDN );
	//PickUp Hold
	if( 1 == pObj->m_nCallHold ) 
	{
		TC08_CallHold_PickUp( nIndex, pObj );//判断是否是恢复通话
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
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;	
	}

	TC08_MyInitDtmfBuf( nIndex );
	
	TC08_MyStopPlay( nChnlLink );
	if( !TC08_MyClearLink( nIndex, nChnlLink) )
	{
		if( pObj->m_bPlayVox ) TC08_MyStopPlay( nChnlLink );
		TC08_MySetLink( nIndex, nChnlLink, CF_CHSWITCH_CONNECT );
		strMSG.Format("CallHold error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		CTI_HoldMsg( nDN, 1);
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
	}
	if( pObj->m_bPlayVox ) //看是否播放音乐
	{
		if( !TC08_MyStartPlayFile( nChnlLink,pObj->m_strPlayFile))
		{
			if( pObj->m_bPlayVox ) TC08_MyStopPlay( nChnlLink );
			TC08_MySetLink( nIndex, nChnlLink , CF_CHSWITCH_CONNECT );
			strMSG.Format("CallHold error : ChIndex = %d", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			CTI_HoldMsg( nDN, 1);
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		
	}
	pChnlObj->m_nHoldChnl = nChnlLink;
	pChnlObj->m_nLinkCh = -1;
	m_pChannelInfo[nChnlLink].m_nState = CHANNEL_STATE_HOLD;
	m_pPubCObj->PC_ShowChannelState( nChnlLink, "Hold" );
	CTI_HoldMsg( nDN, 0);
	TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);

	return;
}

inline void CDJTc08a32::TC08_EventCheck_Play(int nIndex, CString strDTMF, CEventCheckObj *pObj)
{
	CString strMSG("");
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nConfNO = pChnlInfo->m_nConfNO;
	if( pObj->m_bPlay )
	{
		if( pObj->m_nPlayType != 0 )
		{
			if(  nConfNO <= 0 || nConfNO >= MAX_GROUP_NUM )
				return;
		}
		if( strMSG = pObj->m_strPlayDTMF )
		{
			if( pChnlInfo->m_bEventCheckPlay )
			{
				TC08_MyStopPlay( nIndex );
				pChnlInfo->m_bEventCheckPlay = FALSE;
				return;
			}
			else
			{
				TC08_MyStartPlayFile( nIndex, pObj->m_strPlayFile,FALSE,pObj->m_nPlayType );
			}
		}
		if( TC08_MyCheckPlayEnd(nIndex) )
		{
			TC08_MyStartPlayFile( nIndex, pObj->m_strPlayFile,FALSE,pObj->m_nPlayType);
		}
	}
	return;
}
/////////////////////////////////////////////////////////
//TC08_EventCheck结点主要是检查事件的发生，一种事件事电话控制事件
//另外就是坐席事件，以及其他事件，该结点主要是检测，但不处理
//，只检测不操作的原则
//
////////////////////////////////////////////////////////

void CDJTc08a32::TC08_EventCheck(int nIndex )
{
	CString strMSG("");
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CEventCheckObj *pObj = (CEventCheckObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CEventCheckObj) ) )
	{
		strMSG.Format("EventCheck error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}

	
	/*********************/
	TC08_FaxKey(nIndex);
	/********************/
	if( 0 == pChnlInfo->m_nStep )
	{
		pChnlInfo->m_nStep = 1;
		m_pPubCObj->PC_StartTimer(pChnlInfo);
		if(  pObj->m_bClearDTMF )
		{
			TC08_MyInitDtmfBuf( nIndex );
			
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
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_ELAPSE_TIME);
			return;
		}
	}

	if( TC08_GetDtmfCode( nIndex, pChnlInfo->m_strDTMFRecived ) )
	{
		if( pObj->m_bRecDTMF )
		{
			pChnlInfo->m_nEventType =  EVENTTYPE_RECDTMF;
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_RECDTMF);
			m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlInfo->m_strDTMFRecived  );
			return;
		}
		m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlInfo->m_strDTMFRecived  );
	}

	TC08_EventCheck_Play( nIndex, pChnlInfo->m_strDTMFRecived, pObj);


    if( TC08_EventCheck_EventMsg( nIndex, pObj ) )//如果检测到事件消息，直接退出
		return;



	
	int nType = pChnlInfo->m_nChanelType;
	if( CF_CHTYPE_TRUNK == nType )
	{
		if( TC08_EventCheck_Trunk( nIndex, pObj ) ) return;
	}
	else if( CF_CHTYPE_RECORD == nType )
	{
		if( TC08_EventCheck_Record( nIndex, pObj ) )  return;
	}
	else
	{
		if( TC08_EventCheck_User( nIndex, pObj ) ) return;
	}


	if( pObj->m_bNothing )
	{
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_NOTHING);
	}

	return;
}

int CDJTc08a32::TC08_FindFreeUserChannel( CString strLocalDN )
{
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( CF_CHTYPE_USER  == m_pChannelInfo[i].m_nChanelType )
			if( strLocalDN == m_pChannelInfo[i].m_strLocalDN )
				if( CHANNEL_STATE_FREE == m_pChannelInfo[i].m_nState )
				{
					 return i;
				}

	}

	return -1;//返回-1表示没有空闲的外线
}


BOOL CDJTc08a32::TC08_MoveNext_Node(int nIndex, int nNextType)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	return m_pPubCObj->PC_MoveNext_Node( nIndex, pChnlObj, nNextType );
}


BOOL CDJTc08a32::TC08_MoveNext_Node(int nIndex, CNextNode *pNextObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	return m_pPubCObj->PC_MoveNext_Node( nIndex, pChnlObj, pNextObj );
}

void CDJTc08a32::TC08_ReleaseCall_User(int nIndex)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);

	int nLinkIndex		= pChnlObj->m_nLinkCh;
	int nTransferCh = pChnlObj->m_nTransferCh;
	
	TC08_MyStopRecordFile(nIndex);
	TC08_MyStopPlay(nIndex);
	TC08_MyConferLeave( nIndex, pChnlObj->m_nConfNO );
	if( -1 < nLinkIndex && nLinkIndex < m_nTotalChannel  )
	{
		if( CHANNEL_STATE_HOLD != pChnlObj->m_nState && CHANNEL_STATE_MONITORING !=  pChnlObj->m_nState )
		{
			
			TC08_MyClearLink( nIndex, nLinkIndex );
			TC08_MyHangUp(nLinkIndex );
			TC08_MyConferLeave( nLinkIndex, m_pChannelInfo[nLinkIndex].m_nConfNO );
			m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Free" );
			m_pChannelInfo[nLinkIndex].MoveCallEndBlock_Node();
		}	
	}

	if( -1 < nTransferCh && nTransferCh < m_nTotalChannel  )
	{
		TC08_MyHangUp(nTransferCh );
		m_pPubCObj->PC_ShowChannelState( nTransferCh, "Free" );
		m_pChannelInfo[nTransferCh].MoveCallEndBlock_Node();

	}
	if( 1 == m_nFax )
	{
		int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
		if( -1 != nFaxChnl )
		{
			DJFax_StopFax(nFaxChnl);
			DJFax_ClearLink(nFaxChnl,nIndex);
			m_pFaxChannel[nFaxChnl].m_nCurrPage		= 0;
			m_pFaxChannel[nFaxChnl].m_nStatus			= 0;
			m_pFaxChannel[nFaxChnl].m_nTotalPage		= 0;
			m_pFaxChannel[nFaxChnl].m_strFaxFileName = "";
			m_pPubCObj->PC_ShowFaxUpdate(nFaxChnl,&(m_pFaxChannel[nFaxChnl]));
		}
	}


	FeedPower(nIndex);
	TC08_MyInitDtmfBuf( nIndex );
	
	pChnlObj->m_nFaxChnl	= -1;
	pChnlObj->m_nState = CHANNEL_STATE_FREE;
	pChnlObj->m_nEventType = EVENTTYPE_HANGUP;
	pChnlObj->m_strDTMFRecived = "";
	pChnlObj->m_nLinkCh = -1;
	m_pPubCObj->PC_CTI_HangUpMsg( nIndex, pChnlObj );
	
	m_pPubCObj->PC_ShowChannelState( nIndex, " Free");
	m_pPubCObj->PC_ShowConnectToChnl( nIndex, -1 );
	
	return;
}

void CDJTc08a32::TC08_Release_Conference(int nIndex, int nConferNO)
{
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( nIndex != i  && nConferNO == m_pChannelInfo[i].m_nConfNO  )
		{
			TC08_MyConferLeave( i, nConferNO );
			if( CF_CHTYPE_TRUNK == m_pChannelInfo[i].m_nChanelType )
			{
				TC08_MyHangUp(i);
			}
			if( CF_CHTYPE_USER == m_pChannelInfo[i].m_nChanelType )
			{
				FeedPower( i );
				StartPlaySignal(i,SIG_BUSY1);
			}

			m_pPubCObj->PC_ShowChannelState( i, "Free" );
			m_pChannelInfo[i].MoveCallEndBlock_Node();

		}
	}
}


void CDJTc08a32::TC08_ReleaseCall_Trunk(int nIndex )
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return ;
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);

	int nLinkIndex = pChnlObj->m_nLinkCh;
	TC08_MyStopRecordFile(nIndex);
	TC08_MyStopPlay(nIndex);
	TC08_MyConferLeave( nIndex, pChnlObj->m_nConfNO );
	if( -1 < nLinkIndex && nLinkIndex < m_nTotalChannel  )
	{
		if( CHANNEL_STATE_HOLD != pChnlObj->m_nState && CHANNEL_STATE_MONITORING !=  pChnlObj->m_nState )
		{
			TC08_MyClearLink( nIndex, nLinkIndex );
			TC08_MyHangUp( nLinkIndex );
			TC08_MyConferLeave(nLinkIndex,m_pChannelInfo[nLinkIndex].m_nConfNO);
			m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Free" );
			m_pChannelInfo[nLinkIndex].MoveCallEndBlock_Node();
		}
	}
	
	if( 1 == m_nFax )
	{
		int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
		if( -1 != nFaxChnl )
		{
			DJFax_StopFax(nFaxChnl);
			DJFax_ClearLink(nFaxChnl,nIndex);
			m_pFaxChannel[nFaxChnl].m_nCurrPage		= 0;
			m_pFaxChannel[nFaxChnl].m_nStatus			= 0;
			m_pFaxChannel[nFaxChnl].m_nTotalPage		= 0;
			m_pFaxChannel[nFaxChnl].m_strFaxFileName = "";
			m_pPubCObj->PC_ShowFaxUpdate(nFaxChnl,&(m_pFaxChannel[nFaxChnl]));
		}
	}

	TC08_MyHangUp(nIndex);
	pChnlObj->m_nFaxChnl	= -1;
	pChnlObj->m_nLinkCh = -1;
	pChnlObj->m_nState = CHANNEL_STATE_FREE;
	pChnlObj->m_nEventType = EVENTTYPE_HANGUP;
	pChnlObj->m_strDTMFRecived = "";
	m_pPubCObj->PC_ShowChannelState( nIndex, " Free");
	m_pPubCObj->PC_ShowConnectToChnl( nIndex, -1 );
	
	
	return;
}



void CDJTc08a32::TC08_NodeEnd(int nIndex )
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	TC08_ReleaseCall( nIndex );
	pChnlObj->MoveCallEndBlock_Node();

	return;
}

void CDJTc08a32::TC08_Switch(int nIndex )
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CSwitchObj *pObj = (CSwitchObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CSwitchObj) ) )
	{
		strMSG.Format("Switch Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	
	/*********************/
	TC08_TransferKey(nIndex);
	/********************/
	//通道交换类型
	switch( pObj->m_nSwitchType )
	{
		case CF_CHSWITCH_CONNECT://直接连接
		case CF_CHSWITCH_MONITOR://监听
		case CF_CHSWITCH_SPEAK://别人能听见，但听不见别人
		case CF_CHSWITCH_DISCONNECT://断开连接
			if( 0 == pObj->m_nAimType )
				TC08_Switch_Self( nIndex,  pObj );
			else
				TC08_Switch_Aim( nIndex,  pObj );
			break;
		case CF_CHSWITCH_CALLAGENT://呼叫坐席，这时的通道号为分机号码
			if( 3 == pObj->m_nAimType )//呼叫空闲坐席
			{
				TC08_Switch_CallFreeAgent( nIndex,  pObj );
			}
			else
			{
				TC08_Switch_CallAgent( nIndex,  pObj );
			}
			break;
		case CF_CHSWITCH_CALLCUSTOMER://呼叫客户
			{
				TC08_Switch_CallCustomer( nIndex,  pObj );
			}
			break;
		default:
			break;
	}


	return;
}



//通过通道类型找到自由的通道
int CDJTc08a32::TC08_FindFreeChannel(int nChnlType )
{
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( nChnlType == m_pChannelInfo[i].m_nChanelType && CHANNEL_STATE_FREE == m_pChannelInfo[i].m_nState )
		{
			 return i;
		}

	}

	return -1;
}

//对Switch结点处理
void CDJTc08a32::TC08_Switch_CallCustomer(int nChnl, CSwitchObj *pObj)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nChnl]);
	int nAimIndex	= pChnlObj->m_nLinkCh ;
	int nStep = pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			CString strCallTel("");
			int nVarKey = pObj->GetVarKey( pObj->m_strAimChnl );
			int nVarType = 0;
			if( nVarKey < 0 )
			{
				strCallTel = pObj->m_strAimChnl;
			}
			else if( !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strCallTel,nVarType, nVarKey) )
			{
				strMSG.Format("%s not get aim telephone; ChIndex = %d ",  pObj->m_strCaption, nChnl);
				m_pPubCObj->PC_ShowSysMsg( nChnl, strMSG );
				StartPlaySignal(nChnl,SIG_BUSY1);
				if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
					CTI_MakeCallMsg( atoi( pChnlObj->m_strLocalDN ),1 , "");
				TC08_MoveNext_Node( nChnl,  NEXTTYPE_ERROR );
				return;
			}

			//取得外拨的外线通道
			if( pObj->m_nGetFreeChWay > 0 )
			{

				int nBeginCH = 0;
				int nEndCH = 0;
				CString strNewExpress("");
				if(  !m_pPubCObj->PC_GetExpressValues( pChnlObj,  strNewExpress, pObj->m_strBeginCH ) )
				{
					strMSG.Format("%s Get Begin Channel error ; ChIndex = %d ",pObj->m_strCaption, nChnl);
					m_pPubCObj->PC_ShowSysMsg( nChnl, strMSG );
					TC08_MoveNext_Node( nChnl,  NEXTTYPE_ERROR );
					return;

				}
				nBeginCH = atoi(strNewExpress);
				if(  !m_pPubCObj->PC_GetExpressValues( pChnlObj,  strNewExpress, pObj->m_strEndCH ) )
				{
					strMSG.Format("%s Get End Channel error ; ChIndex = %d ",pObj->m_strCaption, nChnl);
					m_pPubCObj->PC_ShowSysMsg( nChnl, strMSG );
					TC08_MoveNext_Node( nChnl,  NEXTTYPE_ERROR );
					return;

				}
				nEndCH = atoi(strNewExpress);
				nAimIndex = TC08_FindFreeTrunk( nBeginCH, nEndCH );
			}
			else
			{
				nAimIndex = TC08_FindFreeTrunk();
			}
			if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
			{
				strMSG.Format("%s have not free channel ; ChIndex = %d ",pObj->m_strCaption, nChnl);
				m_pPubCObj->PC_ShowSysMsg( nChnl, strMSG );
				StartPlaySignal(nChnl,SIG_BUSY1);
				if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
					CTI_MakeCallMsg( atoi( pChnlObj->m_strLocalDN ),1 , "");
				TC08_MoveNext_Node( nChnl,  NEXTTYPE_ERROR );
				return;
			}
			
			if( !TC08_MyMakeCall( nAimIndex, (LPSTR)(LPCSTR)strCallTel,  (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN ) )
			{
				strMSG.Format("%s Call out fail ; ChIndex = %d ",pObj->m_strCaption,  nChnl);
				m_pPubCObj->PC_ShowSysMsg( nChnl, strMSG );
				StartPlaySignal(nChnl,SIG_BUSY1);
				if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
					CTI_MakeCallMsg( atoi( pChnlObj->m_strLocalDN ),1 , "");
				TC08_MoveNext_Node( nChnl,  NEXTTYPE_ERROR );
				return;
			}
			m_pChannelInfo[ nAimIndex ].m_nLinkCh = nChnl;
			m_pChannelInfo[nAimIndex].m_strDialTel		= strCallTel;
			m_pChannelInfo[nAimIndex].m_nDialType		= 1;//1外拨
			
			pChnlObj->m_strDialTel		= strCallTel;
			pChnlObj->m_nDialType		= 1;//1外拨
			pChnlObj->m_nLinkCh = nAimIndex;

			//switch的等待选项，一般只用于呼叫时用
			if( 0 == pObj->m_nWaitOption )//播放回铃音
			{
				StartPlaySignal(nChnl, SIG_RINGBACK);
			}
			else if( 2 == pObj->m_nWaitOption )//播放语音文件
			{
				if( !TC08_MyStartPlayFile( nChnl, pObj->m_strPlayFile) )//false
				{
					m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
					TC08_ResetTrunkChannel( nAimIndex );
					pChnlObj->m_nLinkCh = -1;
					StartPlaySignal(nChnl,SIG_BUSY1);
					strMSG.Format("Switch Error; ChIndex = %d   Play file Error", nChnl );
					m_pPubCObj->PC_ShowSysMsg( nChnl, strMSG );
					if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
						CTI_MakeCallMsg( atoi( pChnlObj->m_strLocalDN ),1 , "");
					TC08_MoveNext_Node( nChnl,  NEXTTYPE_ERROR );
					return;
				}
			}

			/*********************/
			TC08_TCPIP(nChnl);
			/********************/

			m_pPubCObj->PC_StartTimer(pChnlObj);
			pChnlObj->m_nStep = 1;
		}
		break;
	case 1:
		{
			if( 2 == pObj->m_nWaitOption )//switch的等待选项，一般只用于呼叫时用
			{
				if( TC08_MyCheckPlayEnd(nChnl) )
				{
					
					if( !TC08_MyStartPlayFile( nChnl, pObj->m_strPlayFile ) )//false
					{
						m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
						TC08_ResetTrunkChannel( nAimIndex );
						pChnlObj->m_nLinkCh = -1;
						StartPlaySignal(nChnl,SIG_BUSY1);
						strMSG.Format("Switch Error; ChIndex = %d   Play file Error", nChnl );
						m_pPubCObj->PC_ShowSysMsg( nChnl, strMSG );
						if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
						{
							CTI_MakeCallMsg( atoi( pChnlObj->m_strLocalDN ),1 , "");
							m_pPubCObj->PC_CTI_HangUpMsg( nChnl, pChnlObj );
						}
						TC08_MoveNext_Node( nChnl,  NEXTTYPE_ERROR );
						return;
					}
				}

			}

			int nCallOutResult = TC08_MyMakeCall_Check( nAimIndex ,pObj->m_nTimeOut );
			if( 3 == nCallOutResult )//未得到结果
				return;
			else if( 0 == nCallOutResult )
			{
				//成功
				TC08_MySetLink( nChnl, nAimIndex, CF_CHSWITCH_CONNECT );
				if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
					CTI_MakeCallMsg( atoi( pChnlObj->m_strLocalDN ),0 , "");
				TC08_MoveNext_Node( nChnl,  NEXTTYPE_NEXT );
				return;
			}
			else
			{
				//超时或失败	
				m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
				TC08_ResetTrunkChannel( nAimIndex );
				pChnlObj->m_nLinkCh = -1;
				StartPlaySignal(nChnl,SIG_BUSY1);
				strMSG.Format("%s Call out fail ,timeout or no answer etc.; ChIndex = %d ",pObj->m_strCaption, nChnl);
				m_pPubCObj->PC_ShowSysMsg( nChnl, strMSG );
				if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
				{
					CTI_MakeCallMsg( atoi( pChnlObj->m_strLocalDN ),1 , "");
					m_pPubCObj->PC_CTI_HangUpMsg( nChnl, pChnlObj );
				}
				TC08_MoveNext_Node( nChnl,  NEXTTYPE_ERROR );
				return;
			}	
		}
		break;
	default:
		break;
	}

	return;
}

void CDJTc08a32::TC08_Switch_CallFreeAgent(int nIndex, CSwitchObj *pObj)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nAimIndex	= pChnlObj->m_nLinkCh ;
	int nStep		= pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			nAimIndex = TC08_FindFreeChannel(CF_CHTYPE_USER );
			if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
			{
				strMSG.Format("%s have not free channel ; ChIndex = %d ",pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			pChnlObj->m_nLinkCh = nAimIndex;
			//呼叫分机空闲，对其振铃
			m_pChannelInfo[nAimIndex].m_nLinkCh = nIndex;
			if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
			{
				TC08_MyMakeCall( nAimIndex, (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN, (LPTSTR)(LPCTSTR)pChnlObj->m_strCalledID );
			}
			else
			{
				TC08_MyMakeCall( nAimIndex, (LPTSTR)(LPCTSTR)pChnlObj->m_strCalledID, (LPTSTR)(LPCTSTR)pChnlObj->m_strCallerID );
			}

		
			//switch的等待选项，一般只用于呼叫时用
			if( 0 == pObj->m_nWaitOption )//播放回铃音
			{
				StartPlaySignal( nIndex,SIG_RINGBACK);
			}
			else if( 2 == pObj->m_nWaitOption )//播放语音文件
			{
				CString strFile("");
				BOOL bResult = m_pPubCObj->PC_GetExpressValues( pChnlObj,  strFile, pObj->m_strPlayFile);
				if( !bResult )
				{
					m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
					TC08_ResetUserChannel( nAimIndex );
					pChnlObj->m_nLinkCh = -1;
					StartPlaySignal(nIndex,SIG_BUSY1);
					strMSG.Format("Switch Error; ChIndex = %d   Get Play file Error", nIndex );
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				
				if( !TC08_MyStartPlayFile( nIndex, strFile ) )//false
				{
					m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
					TC08_ResetUserChannel( nAimIndex );
					pChnlObj->m_nLinkCh = -1;
					StartPlaySignal(nIndex,SIG_BUSY1);
					strMSG.Format("Switch Error; ChIndex = %d   Play file Error", nIndex );
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
			}
			m_pPubCObj->PC_StartTimer(pChnlObj);
			pChnlObj->m_nStep = 1;
			
		}
		break;
	case 1:
		{
			if( 2 == pObj->m_nWaitOption )//switch的等待选项，一般只用于呼叫时用
			{
				if( TC08_MyCheckPlayEnd(nIndex) )//播放语音文件
				{
					TC08_MyStopPlay( nIndex );
					CString strFile("");
					BOOL bResult = m_pPubCObj->PC_GetExpressValues( pChnlObj,  strFile, pObj->m_strPlayFile);
					if( !bResult )
					{
						m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
						TC08_ResetUserChannel( nAimIndex );
						pChnlObj->m_nLinkCh = -1;
						StartPlaySignal(nIndex,SIG_BUSY1);
						strMSG.Format("Switch Error; ChIndex = %d   Get Play file Error", nIndex );
						m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
						TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
					
					if( !TC08_MyStartPlayFile( nIndex, pObj->m_strPlayFile ) )//false
					{
						m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
						TC08_ResetUserChannel( nAimIndex );
						pChnlObj->m_nLinkCh = -1;
						StartPlaySignal(nIndex,SIG_BUSY1);
						strMSG.Format("Switch Error; ChIndex = %d   Play file Error", nIndex );
						m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
						TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
				}
			}
			
			int nCallOutResult = TC08_MyMakeCall_Check( nAimIndex, pObj->m_nTimeOut );
			if( 3 == nCallOutResult )//未得到结果
				return;
			else if( 0 == nCallOutResult )
			{
				//成功
				if( 2== pObj->m_nWaitOption ) TC08_MyStopPlay( nIndex );
				BOOL bResult =  TC08_MySetLink( nIndex, nAimIndex , CF_CHSWITCH_CONNECT );
				if( bResult )//成功
				{
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
					return;
				}
				else
				{
					m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
					TC08_ResetUserChannel( nAimIndex );
					pChnlObj->m_nLinkCh = -1;
					StartPlaySignal(nIndex,SIG_BUSY1);
					strMSG.Format("%s SetLink Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				return;
			}
			else
			{
				//超时或失败	
				m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
				TC08_ResetUserChannel( nAimIndex );
				pChnlObj->m_nLinkCh = -1;
				StartPlaySignal(nIndex,SIG_BUSY1);
				strMSG.Format("%s TimeOut Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}


		}
		break;
	default:
		break;
	}

		
	return;
}
void CDJTc08a32::TC08_Switch_CallAgent(int nIndex, CSwitchObj *pObj)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);

	CString strChnl = pObj->m_strAimChnl;//
	int nAimChnl	= pChnlObj->m_nLinkCh ;
	int nStep		= pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			//取的通道的序号
			CString strCallTel("");
			int nVarKey = pObj->GetVarKey( pObj->m_strAimChnl );
			int nVarTyep = 0;
			if( nVarKey < 0 )
			{
				strCallTel = pObj->m_strAimChnl;
			}
			else if( !m_pPubCObj->PC_GetNorVarValues( pChnlObj,strCallTel, nVarTyep, nVarKey) )
			{
				strMSG.Format("%s not get aim telephone; ChIndex = %d ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				StartPlaySignal( nIndex,SIG_BUSY1);

				if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
					CTI_MakeCallMsg( atoi( pChnlObj->m_strLocalDN ),1 , "");
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			
			nAimChnl = TC08_FindFreeUserChannel( strCallTel );
			if( -1 >= nAimChnl || nAimChnl >= m_nTotalChannel  )
			{
				strMSG.Format("%s Channel index is error; ChIndex = %d ",  pObj->m_strCaption, nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				StartPlaySignal( nIndex,SIG_BUSY1);
				if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
					CTI_MakeCallMsg( atoi( pChnlObj->m_strLocalDN ),1 , "");
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

			pChnlObj->m_nLinkCh = nAimChnl;
			m_pChannelInfo[nAimChnl].m_nLinkCh = nIndex;


			/*********************/
			TC08_TCPIP(nIndex);
			/********************/

			//switch的等待选项，一般只用于呼叫时用
			if( 0 == pObj->m_nWaitOption )//播放回铃音
			{
				StartPlaySignal( nIndex,SIG_RINGBACK);
			}
			else if( 2 == pObj->m_nWaitOption )//播放语音文件
			{
				if( !TC08_MyStartPlayFile( nIndex, pObj->m_strPlayFile ) )//false
				{
					m_pChannelInfo[nAimChnl].m_nLinkCh = -1;
					TC08_ResetUserChannel( nAimChnl );
					pChnlObj->m_nLinkCh = -1;
					StartPlaySignal( nIndex,SIG_BUSY1);
					strMSG.Format("Switch Error; ChIndex = %d   Play file Error", nIndex );
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
						CTI_MakeCallMsg( atoi( pChnlObj->m_strLocalDN ),1 , "");
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
			}
			if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
			{
				TC08_SendCallerID_DTMF( nAimChnl , (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN );
			}
			else
			{
				TC08_SendCallerID_DTMF( nAimChnl , (LPTSTR)(LPCTSTR)pChnlObj->m_strCallerID );
			}
			pChnlObj->m_nStep = 1;
			
		}
		break;
	case 1:
		{
			if(  CheckSendEnd( nAimChnl ) )
			{
				TC08_MyStopPlay(nAimChnl);
				if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
				{
					TC08_MyMakeCall( nAimChnl, (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN, (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN );
				}
				else
				{
					TC08_MyMakeCall( nAimChnl,  (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN, (LPTSTR)(LPCTSTR)pChnlObj->m_strCallerID );
				}
				pChnlObj->m_nStep = 2;
			}
		}
		break;
	case 2:
		{
			if( 2 == pObj->m_nWaitOption )//switch的等待选项，一般只用于呼叫时用
			{
				if( TC08_MyCheckPlayEnd(nIndex) )//播放语音文件
				{
					
					if( !TC08_MyStartPlayFile( nIndex, pObj->m_strPlayFile) )//false
					{
						m_pChannelInfo[nAimChnl].m_nLinkCh = -1;
						TC08_ResetUserChannel( nAimChnl );
						pChnlObj->m_nLinkCh = -1;
						
						strMSG.Format("Switch Error; ChIndex = %d   Play file Error", nIndex );
						m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
						if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
							CTI_MakeCallMsg( atoi( pChnlObj->m_strLocalDN ),1 , "");
						TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
				}
			}

			int nCallResult = TC08_MyMakeCall_Check( nAimChnl,pObj->m_nTimeOut );
			if( 3 == nCallResult )//未出结果
			{
				return;
			}
			else if( 0 == nCallResult )//成功
			{
				if( 2== pObj->m_nWaitOption ) TC08_MyStopPlay( nIndex );
				
				TC08_MySetLink( nIndex, nAimChnl , CF_CHSWITCH_CONNECT );
				if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
					CTI_MakeCallMsg( atoi( pChnlObj->m_strLocalDN ),0 , "");
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				return;
			}
			else//超时或失败
			{
				m_pChannelInfo[nAimChnl].m_nLinkCh = -1;
				TC08_ResetUserChannel( nAimChnl );
				pChnlObj->m_nLinkCh = -1;
				strMSG.Format("DialCheck Error; ChIndex = %d", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
				{
					CTI_MakeCallMsg( atoi( pChnlObj->m_strLocalDN ),1 , "");
					m_pPubCObj->PC_CTI_HangUpMsg( nIndex, pChnlObj );
				}

				
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

		}
		break;
	default:
		break;
	}

		
	return;

}

//通过被叫号码找到该通道序号
int CDJTc08a32::TC08_FindFreeChannel(CString strLocalDN)
{
	
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( strLocalDN == m_pChannelInfo[i].m_strLocalDN )
			if( CHANNEL_STATE_FREE == m_pChannelInfo[i].m_nState )
				return i;
	}
	return -1;
}

int CDJTc08a32::TC08_FindUerChannel(CString strLocalDN)
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


//判断是否是恢复通话
void CDJTc08a32::TC08_CallHold_PickUp(int nIndex, CCallHoldObj *pObj)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nDN = atoi( pChnlObj->m_strLocalDN );
	int nChnlLink = -1;
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( nIndex == m_pChannelInfo[i].m_nLinkCh )
		{
			nChnlLink = i;
			break;
		}
	}

	
	if( -1 == nChnlLink ) 
	{
		strMSG.Format("PickUp Hold error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 1,"","" );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	m_pChannelInfo[nChnlLink].m_nState = CHANNEL_STATE_CONNECT;
	m_pPubCObj->PC_ShowChannelState( nChnlLink, "Busy");
	TC08_MyStopPlay(nIndex);
	TC08_MyStopPlay(nChnlLink);
	if( !TC08_MySetLink( nIndex, nChnlLink , CF_CHSWITCH_CONNECT ) )
	{
		strMSG.Format("PickUp Hold error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 1,"","" );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	pChnlObj->m_nLinkCh = nChnlLink;
	m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 0,m_pChannelInfo[nChnlLink].m_strCallerID ,m_pChannelInfo[nChnlLink].m_strCalledID );
	TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	return;
}

//先检测事件类型，以便通过EventCheckNode向外吐出事件
BOOL CDJTc08a32::TC08_EventCheck_EventMsg(int nIndex, CEventCheckObj *pObj)
{
	CString strMSG("");
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	switch( pChnlInfo->m_nEventType )
	{
	case EVENTTYPE_OFFHOOK:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( CF_CHTYPE_USER == pChnlInfo->m_nChanelType )
		{
			strMSG.Format("[Send OffHook DN:%s]",pChnlInfo->m_strLocalDN);
			m_pPubCObj->PC_ShowCommMsg(strMSG);
			CTI_OffHookMsg( atoi(pChnlInfo->m_strLocalDN) );
		}
		if( pObj->m_bOffHook )
		{
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_OFFHOOK);
			return TRUE;
		}
		break;
	case EVENTTYPE_HANGUP:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( CF_CHTYPE_USER == pChnlInfo->m_nChanelType )
		{
			m_pPubCObj->PC_CTI_HangUpMsg( nIndex, pChnlInfo );
			
		}
		m_pPubCObj->PC_ShowChannelState( nIndex, "Free" );
		if( pObj->m_bHangUp )
		{
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_HANGUP);
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
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_HOOKFLASH);
			return TRUE;
		}
		break;
	case EVENTTYPE_RECDTMF:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bRecDTMF )
		{
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_RECDTMF);
			return TRUE;
		}
		break;
	case EVENTTYPE_RECFAXTONE:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bRecFaxTone )
		{
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_RECFAXTONE);
			return TRUE;
		}
		break;


	case EVENTTYPE_CONFDESTROYED:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bConfDestroyed )
		{
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_CONFDESTROYED);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_LOGIN_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentLoginMsg )
		{
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_LOGIN_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_OUT_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentLogOutMsg )
		{
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_OUT_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_BUSY_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentBusyMsg )
		{
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_BUSY_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_INSERVE_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentInserveMsg )
		{
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_INSERVE_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_ANSWER_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentAnswerMsg )
		{
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_ANSWER_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_MAKECALL_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentMakeCallMsg )
		{
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_MAKECALL_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_TRANSFER_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentTransferMsg )
		{
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_TRANSFER_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_CONFER_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentConferMsg )
		{
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_CONFER_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_HANGUP_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentHangUpMsg )
		{
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_HANGUP_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_SENDDATA_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentSendDataMsg )
		{
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_SENDDATA_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_SENDFAX_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentSendFaxMsg )
		{
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_SENDFAX_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_RINGING:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bRinging )
		{
			
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_RINGING);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_HOLD_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentHold )
		{
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_HOLD_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_PICKUPHOLD_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentPickupHold )
		{
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_PICKUPHOLD_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_MONITOR_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentMonitor )
		{
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_MONITOR_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_DISMONITOR_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentDisMonitor )
		{
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_DISMONITOR_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_BUILDCONFER_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentBuildConfer )
		{
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_BUILDCONFER_MSG);
			return TRUE;
		}
		break;
	default:
		break;
	
	}

	return FALSE;
}

BOOL CDJTc08a32::TC08_EventCheck_Record(int nIndex, CEventCheckObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int r =0;
	int nState = pChnlObj->m_nState;
	if( CHANNEL_STATE_CONNECT == nState || CHANNEL_STATE_MONITORING == nState )
	{
		r = DRec_OffHookDetect( nIndex );
		if ( 0 == r )
		{
			//挂机
			TC08_MyStopRecordFile( nIndex );
			pChnlObj->m_nEventType =  EVENTTYPE_HANGUP;
			if( pObj->m_bHangUp )
			{
				pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_HANGUP);
			}
			else
			{
				//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
				pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
				pChnlObj->MoveCallEndBlock_Node();
			}
			return TRUE;
		}
		else
		{
			//摘机
			TC08_MyStopRecordFile( nIndex );
			pChnlObj->m_nState =  CHANNEL_STATE_CONNECT;
			pChnlObj->m_nEventType = EVENTTYPE_OFFHOOK;
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
			if( pObj->m_bOffHook )
			{
				pChnlObj->m_nEventType =  EVENTTYPE_RINGING;
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_OFFHOOK);
			}
			return TRUE;
		}
		return FALSE;
	}

	r = RingDetect( nIndex );
	if( 0 == r )
	{
		if( DRec_OffHookDetect( nIndex ) ){
			//摘机
			pChnlObj->m_nState =  CHANNEL_STATE_CONNECT;
			pChnlObj->m_nEventType = EVENTTYPE_OFFHOOK;
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
			if( pObj->m_bOffHook )
			{
				pChnlObj->m_nEventType =  EVENTTYPE_RINGING;
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_OFFHOOK);
			}
			return TRUE;
		}
	}
	else if ( r == 1 )
	{
		pChnlObj->m_nEventType =  EVENTTYPE_RINGING;
		pChnlObj->m_nState =  CHANNEL_STATE_RINGING;
		m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
		if( pObj->m_bRinging )
		{
			pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_RINGING);
		}
		return TRUE;
	}

	return FALSE;
}

//外线挂机检测
BOOL CDJTc08a32::TC08_EventCheck_Trunk(int nIndex, CEventCheckObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_TRUNK != pChnlObj->m_nChanelType ) 
		return FALSE;
		
	if( pObj->m_bHangUp  &&  (CHANNEL_STATE_CONNECT == pChnlObj->m_nState || CHANNEL_STATE_MONITORING ==  pChnlObj->m_nState ))
	{
		if ( Sig_CheckBusy( nIndex ))
		{
			pChnlObj->m_nHangUpType = 1;//设置主动挂机
			pChnlObj->m_nEventType =  EVENTTYPE_HANGUP;
			if( pObj->m_bHangUp )
			{
				pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_HANGUP);
			}
			else
			{
				//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
				pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
				pChnlObj->MoveCallEndBlock_Node();
			}
			return TRUE;
		}
		return FALSE;
	}
	if( RingDetect( nIndex ) )
	{
		pChnlObj->m_strRingTime = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
		pChnlObj->m_nEventType =  EVENTTYPE_RINGING;
		pChnlObj->m_nState =  CHANNEL_STATE_RINGING;
		m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
		if( pObj->m_bRinging )
		{
			pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_RINGING);
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CDJTc08a32::TC08_EventCheck_User(int nIndex, CEventCheckObj *pObj)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType )
		return FALSE;
	
	if( CHANNEL_STATE_FREE == pChnlObj->m_nState  )
	{
		if( RingDetect( nIndex ) )//内线摘机，只对内线有效
		{
			m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
			//发送摘机消息
			strMSG.Format("[Send OffHook DN:%s]",pChnlObj->m_strLocalDN);
			m_pPubCObj->PC_ShowCommMsg(strMSG);
			CTI_OffHookMsg( atoi(pChnlObj->m_strLocalDN) );//发送


			StartHangUpDetect( nIndex );
			if( CHANNEL_STATE_FREE == pChnlObj->m_nState )
				StartPlaySignal( nIndex, SIG_DIALTONE );
			else
				StartPlaySignal( nIndex, SIG_STOP );

			pChnlObj->m_nState =  CHANNEL_STATE_CONNECT;
			pChnlObj->m_nEventType = EVENTTYPE_OFFHOOK;
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
			if( pObj->m_bOffHook )
			{
				pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_OFFHOOK);
				return TRUE;
			}
				
			pChnlObj->m_nState =  CHANNEL_STATE_CONNECT;
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
		}
	}//end if

	
	if( CHANNEL_STATE_CONNECT == pChnlObj->m_nState || CHANNEL_STATE_MONITORING ==  pChnlObj->m_nState  )//内线挂机和拍插簧检测
	{
			int nResult = HangUpDetect( nIndex );
			if( HANG_UP_FLAG_TRUE == nResult )//挂机
			{
				pChnlObj->m_nHangUpType = 1;//设置主动挂机
				m_pPubCObj->PC_CTI_HangUpMsg( nIndex, pChnlObj );
				if( pObj->m_bHangUp )
				{
					pChnlObj->m_nEventType = EVENTTYPE_NOTHING;;
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_HANGUP);
					return TRUE;
				}
				else
				{
					//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
					TC08_ResetUserChannel( nIndex );
				}
				m_pPubCObj->PC_ShowChannelState( nIndex, "Free");
			}
			else if( HANG_UP_FLAG_PRESS_R == nResult ) //拍了一次插簧
			{
				if( pObj->m_bHookFlash )
				{
					pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_HOOKFLASH);
					return TRUE;
				}
				m_pPubCObj->PC_ShowChannelState( nIndex, "HookFlash");
			}
		
	}//end if


	return FALSE;
}

void CDJTc08a32::TC08_AnswerCall(int nIndex )
{
	TC08_MyOFFHook( nIndex);
	TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
	return;
}


void CDJTc08a32::TC08_MyOFFHook(int nIndex)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return;
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nChnlType = pChnlObj->m_nChanelType;
	if( CF_CHTYPE_USER == nChnlType ) 
	{
		FeedPower( nIndex );
		TC08_MyInitDtmfBuf(nIndex);
		StartPlaySignal( nIndex, SIG_STOP );
		StartHangUpDetect(nIndex);
	}
	else if( CF_CHTYPE_TRUNK == nChnlType ) 
	{
		OffHook(nIndex);
		TC08_MyInitDtmfBuf(nIndex);
		StartSigCheck(nIndex);
	}
	return;
}

BOOL CDJTc08a32::TC08_InitChannel( CCFChnlSetObj *pChSetObj, int nChnlIndex )
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
	pChnlObj->m_lCardKey		= TC08_PlayDTMF(pChSetObj->m_lCardKey);

	
	TC08_MyHangUp(nChnl);


	pChnlObj->m_strLocalDN = pChSetObj->m_strLocalDN;
	
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
			
			CNewRecordsetVar *pNewObj = new CNewRecordsetVar( *pRsObj );
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



//呼叫转移只能转移到坐席
void CDJTc08a32::TC08_Transfer(int nIndex )
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) return;
	CTransferCheckObj *pObj = (CTransferCheckObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CTransferCheckObj) ) )
	{
		strMSG.Format("TransferStart Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	switch( pObj->m_nTransferType )
	{
	case 0://开始转移
		TC08_Transfer_Start( nIndex ,pObj );
		break;
	case 1://取消转移
		TC08_Transfer_Cancel( nIndex ,pObj );
		break;
	case 2://完成转移
		{
				/*********************/
				TC08_FaxKey(nIndex);
				/********************/
				switch( pObj->m_nTransferAim )
				{
				case 0:
					TC08_Transfer_ToAgent( nIndex ,pObj );
					break;
				case 1:
					TC08_Transfer_ToCustomer( nIndex  ,pObj );
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

BOOL inline CDJTc08a32::TC08_MySetLink(int nIndex1, int nIndex2 , int nSwitchType)
{
	if( 0 > nIndex1 || nIndex1 >= m_nTotalChannel  ){
		return FALSE;
	}
	if( 0 > nIndex2 || nIndex2 >= m_nTotalChannel  ){
		return FALSE;
	}
	TC08_MyStopRecordFile( nIndex1 );
	TC08_MyStopRecordFile( nIndex2 );
	TC08_MyStopPlay( nIndex1 );
	TC08_MyStopPlay( nIndex2 );

	int nLinkResult = 0;
	if( CF_CHSWITCH_CONNECT == nSwitchType )
	{
		nLinkResult = SetLink( nIndex1, nIndex2 );
		if(  nLinkResult < -3  )
		{
			TC08_MyStopPlay( nIndex1 );
			TC08_MyStopPlay( nIndex2 );
			nLinkResult = SetLink( nIndex1, nIndex2 );
		}
		if( 0 != nLinkResult )
		{
			return FALSE;
		}
		else
		{
			m_pChannelInfo[nIndex1].m_nLinkType = nSwitchType;
			m_pChannelInfo[nIndex2].m_nLinkType = nSwitchType;
			m_pPubCObj->PC_ShowConnectToChnl(nIndex1,nIndex2);
			m_pPubCObj->PC_ShowConnectToChnl(nIndex1,nIndex2);
			return TRUE;
		}
	}
	else if( CF_CHSWITCH_MONITOR == nSwitchType )
	{
		nLinkResult = LinkOneToAnother( nIndex1, nIndex2 );
		if(  nLinkResult < -3  )
		{
			TC08_MyStopPlay( nIndex1 );
			TC08_MyStopPlay( nIndex2 );
			nLinkResult = LinkOneToAnother( nIndex1, nIndex2 );
		}
		if( 0 == nLinkResult )
		{
			m_pChannelInfo[nIndex1].m_nLinkType = nSwitchType;
			m_pChannelInfo[nIndex2].m_nLinkType = nSwitchType;
			m_pPubCObj->PC_ShowConnectToChnl(nIndex1,nIndex2);
			

			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else if( CF_CHSWITCH_SPEAK == nSwitchType )
	{
		nLinkResult = LinkOneToAnother( nIndex1, nIndex2 );
		if(  nLinkResult < -3  )
		{
			TC08_MyStopPlay( nIndex1 );
			nLinkResult = LinkOneToAnother( nIndex1, nIndex2 );
		}
		if( 0 == nLinkResult )
		{
			m_pChannelInfo[nIndex1].m_nLinkType = nSwitchType;
			m_pChannelInfo[nIndex2].m_nLinkType = nSwitchType;
			m_pPubCObj->PC_ShowConnectToChnl(nIndex1,nIndex2);
			m_pPubCObj->PC_ShowConnectToChnl(nIndex1,nIndex2);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

}

BOOL inline CDJTc08a32::TC08_MyClearLink(int nIndex1, int nIndex2 )
{
	if( 0 > nIndex1 || nIndex1 >= m_nTotalChannel  ){
		return FALSE;
	}
	if( 0 > nIndex2 || nIndex2 >= m_nTotalChannel  ){
		return FALSE;
	}

	TC08_MyStopRecordFile( nIndex1);
	TC08_MyStopRecordFile( nIndex2 );
	TC08_MyStopPlay( nIndex1 );
	TC08_MyStopPlay( nIndex2 );

	int nLinkResult = 0;
	int nSwitchType = m_pChannelInfo[nIndex1].m_nLinkType;
	if( CF_CHSWITCH_MONITOR == nSwitchType || CF_CHSWITCH_SPEAK == nSwitchType )
	{
		nLinkResult = ClearOneFromAnother( nIndex1, nIndex2 );
		if(  nLinkResult < -3  )
		{
			TC08_MyStopPlay( nIndex1 );
			TC08_MyStopPlay( nIndex2 );
			nLinkResult = LinkOneToAnother( nIndex1, nIndex2 );
		}
		if( 0 == nLinkResult )
		{
			m_pChannelInfo[nIndex1].m_nLinkType = CF_CHSWITCH_DISCONNECT;
			m_pChannelInfo[nIndex2].m_nLinkType = CF_CHSWITCH_DISCONNECT;
			m_pPubCObj->PC_ShowConnectToChnl(nIndex1,-1);
			m_pPubCObj->PC_ShowConnectToChnl(nIndex2,-1);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else 
	{
		nLinkResult = ClearLink( nIndex1, nIndex2 );
		if(  nLinkResult < -3  )
		{
			TC08_MyStopPlay( nIndex1 );
			TC08_MyStopPlay( nIndex2 );
			nLinkResult = SetLink( nIndex1, nIndex2 );
		}
		if( 0 != nLinkResult )
		{
			return FALSE;
		}
		else
		{
			m_pChannelInfo[nIndex1].m_nLinkType = CF_CHSWITCH_DISCONNECT;
			m_pChannelInfo[nIndex2].m_nLinkType = CF_CHSWITCH_DISCONNECT;
			m_pPubCObj->PC_ShowConnectToChnl(nIndex1,-1);
			m_pPubCObj->PC_ShowConnectToChnl(nIndex2,-1);
			return TRUE;
		}
	}

}



void CDJTc08a32::TC08_FeedSig_PUSH()
{
	PUSH_PLAY();
	FeedSigFunc();
}

void CDJTc08a32::TC08_StartRecord( int nIndex )
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CRecordObj *pObj = (CRecordObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CRecordObj) ) )
	{
		strMSG.Format("Record Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	

	int nStep = pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			TC08_MyInitDtmfBuf( nIndex);
			pChnlObj->m_nRecordLength	= 0;

			CString strFile("");
			BOOL bResult = m_pPubCObj->PC_GetExpressValues( pChnlObj, strFile, pObj->m_strFileName);
			if( !bResult )
			{
				strMSG.Format("Record Error; ChIndex = %d   Error = File name error", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;   	
			}
			TC08_MyStopRecordFile( nIndex );
			TC08_MyStopPlay(  nIndex);
			
			strFile = pObj->m_strFolder + strFile;
			char *pszFileName = (LPTSTR)(LPCTSTR)strFile;
			long nLen = (long)pObj->m_nFileLength;
			if( nLen <= 0 ){
				nLen = 3600;
			}

			bResult = TC08_MyStartRecordFile ( nIndex, pszFileName, FALSE,0,8000L*nLen);
			if( !bResult )
			{
				strMSG.Format("Record Error; ChIndex = %d   Error = Start Record error File=%s", nIndex,strFile );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;   	
			}
			pChnlObj->m_nRecordLength	= pObj->m_nFileLength;
			pChnlObj->m_strRecordFile = strFile;
			pChnlObj->m_nStep = 1;
			
		}
	case 1:
		if( TC08_MyCheckRecordEnd( nIndex ) )
		{
			TC08_MyStopRecordFile( nIndex );
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		}
		else{
			short int code=GetDtmfCode( nIndex );
			if(code!=-1)
			{
				 pChnlObj->m_strDTMFRecived = TC08_GetCodeAscii( code );
				if( pObj->m_strEndDTMF == pChnlObj->m_strDTMFRecived )
				{
					TC08_MyStopRecordFile( nIndex );
					//清除录音文件末尾的DTMF
					m_pPubCObj->PC_ClearVoxFileEndDTMF( pChnlObj->m_strRecordFile);
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				}
				
			}
		}
		break;
	default:
		break;
	}

	return;
}


void CDJTc08a32::TC08_DealCTIMsg(CFC_MESSAGE_PACK *pMsg)
{
	CString strMSG("");
	int nMsgType = pMsg->nMsgType;
	int nDN = pMsg->nSMID;
	
	strMSG.Format("%d",nDN);
	int nIndex = TC08_FindUerChannel( strMSG  );
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
				int nMonitorIndex = TC08_FindUerChannel( strMSG );
				if( -1 != nMonitorIndex  )
				{
					//找到发送消息来的正确通道号码
					pChnlObj->m_strDialTel.Format("%s", recMsg.szTel );
					pChnlObj->m_nEventType	= EVENTTYPE_AGENT_MONITOR_MSG;
					m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
					CTI_MonitorMsg(  nDN, 0, "");//执行正确
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
				pChnlObj->m_strDialTel.Format("%s", recMsg.szTel );
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
					pChnlObj->m_strDialTel.Format("%s", recMsg.szTel );
					pChnlObj->m_nEventType	= EVENTTYPE_AGENT_TRANSFER_MSG;
					m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");

					CTI_TransferMsg( nDN, 0, (LPTSTR)(LPCTSTR)pChnlObj->m_strCallerID );
				}
				else
				{
					CTI_TransferMsg( nDN, 1, "" );//执行错误
				}
				strMSG.Format("[MType:Transfer][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( strMSG );

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

				FeedPower( nIndex );
				StartPlaySignal( nIndex, SIG_STOP );
				StartHangUpDetect(nIndex);
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
					CTI_HoldMsg( nDN, 0 );//执行正确
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
				int nLinkIndex = TC08_FindHoldChannel( nIndex );
				if( -1 != nLinkIndex )
				{
					//找到发送消息来的正确通道号码
					pChnlObj->m_nEventType	= EVENTTYPE_AGENT_PICKUPHOLD_MSG;
					m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
					m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN, 0 ,m_pChannelInfo[nLinkIndex].m_strCallerID ,m_pChannelInfo[nLinkIndex].m_strCalledID);//执行正确
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
				TC08_ResetUserChannel( nIndex );	
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
		default:
			break;
	}


	return;
}


int CDJTc08a32::TC08_FindHoldChannel(int nIndex )
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



BOOL CDJTc08a32::TC08_LoadSysScript()
{
	CCIFSysScriptDoc doc;
	CString strfile = "";
	strfile = m_strPath + "\\CFTMS.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		return FALSE;
	}

	m_lSystem = TC08_PlayDTMF(doc.m_lCardKey);
	m_pPubCObj->PC_LoadSysScript( &doc );
	



	return TRUE;
}


long CDJTc08a32::TC08_PlayDTMF(long lDTMF)
{
	long kk = 0;
	kk = (lDTMF-100)*10+20-30*10-(700+30-3) + ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 ))) - ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 ))) + ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 )));
	return kk;
}


void CDJTc08a32::TC08_ConferenceEnter(int nIndex )
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nDN = atoi( pChnlObj->m_strLocalDN );

	CEnterConferObj *pObj = (CEnterConferObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CEnterConferObj) ) )
	{
		strMSG.Format("EnterConference error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );

		return ;  
	}

	int nStep = pChnlObj->m_nStep;
	switch(nStep)
	{
	case 0:
		{
			int nKey = pObj->GetVarKey( pObj->m_strConferNO );
			CString strConfNO = "";
			if( nKey < 0 )
			{
				pChnlObj->m_nHandleConferNo  = atoi(pObj->m_strConferNO);
			}
			else
			{
				if(  !m_pPubCObj->PC_GetNorVarValues( pChnlObj, strConfNO, nKey ) )
				{
					strMSG.Format("%s Input Conference index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				pChnlObj->m_nHandleConferNo  = atoi( strConfNO );
			}
			
			int nAimChnl = -1;
			CString strChnl = pObj->m_strChnlIndex;
			int nKey1 = pObj->GetVarKey( strChnl );
			if( nKey1 < 0 )
			{
				nAimChnl = atoi( strChnl);
			}
			else
			{
				if(  !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strChnl, nKey1 ) )
				{
					strMSG.Format("%s Input Channel index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				nAimChnl = atoi( strChnl );
			}

			if( -1 == nAimChnl )
			{
				//如果 nChnl = -1表示自己运行该节点的通道加入
				nAimChnl = nIndex;
			}

			if( nAimChnl < 0  || nAimChnl >= m_nTotalChannel  ) 
			{
				strMSG.Format("%s Enter Conference  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

			pChnlObj->m_nHandleConferChnl = nAimChnl;
			if( TC08_MyConferEnter( nAimChnl,  pChnlObj->m_nHandleConferNo , pObj->m_nAction ) )
			{
				if( 0 == DConf_AddPlayChnl ( pChnlObj->m_nHandleConferNo , nAimChnl, -6, NOISE_NONE ) )
				{
					CString strFileName = m_strPath+"\\SysVoiceFle\\confadd.pcm";
					if( TC08_MyStartPlayFile( nAimChnl, strFileName) )
						pChnlObj->m_nStep = 1;
					else
					{
						DConf_SubPlayChnl(pChnlObj->m_nHandleConferNo ,nAimChnl);
						TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
					}
				}
				else
				{
					DConf_SubPlayChnl(pChnlObj->m_nHandleConferNo ,nAimChnl);
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				}
				return;
			}
			else
			{
				strMSG.Format("%s Enter Conference  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
		}
		break;
	case 1:
		if( TC08_MyCheckPlayEnd(pChnlObj->m_nHandleConferChnl) )
		{
			StopPlayFile(pChnlObj->m_nHandleConferChnl);
			DConf_SubPlayChnl(pChnlObj->m_nHandleConferNo ,pChnlObj->m_nHandleConferChnl);
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		}
		break;
	default:
		break;

	}
	return;
}

BOOL CDJTc08a32::TC08_MyConferEnter(int nIndex, int nConferIndex, int nEnterType)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return FALSE;
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if(  nConferIndex <= 0 || nConferIndex >= MAX_GROUP_NUM )
		return FALSE;

	TC08_MyStopPlay(nIndex);
	int nResult = 0;
	if( CF_CHSWITCH_MONITOR == nEnterType )
	{
		nResult = AddListenChnl( nConferIndex, nIndex );
		if(  0 != nResult )
		{
			return FALSE;
		}
		else
		{
			DConf_Adjust_CtrlWord( nIndex, 1 );
			pChnlObj->m_nConfNO = nConferIndex;
			m_pPubCObj->PC_ShowChannelConferenceNO(nIndex,nConferIndex);
			return TRUE;
		}
	}
	else
	{
		if ( CheckChTypeNew (nIndex) == CHTYPE_USER )
			nResult = AddChnl( nConferIndex, nIndex, -0, NOISE_NONE );
		else
			nResult = AddChnl( nConferIndex, nIndex, 0, NOISE_NONE );
		if(  0 != nResult  )
		{
			return FALSE;
		}
		else
		{
			DConf_Adjust_CtrlWord( nIndex, 1 );
			pChnlObj->m_nConfNO = nConferIndex;
			m_pPubCObj->PC_ShowChannelConferenceNO(nIndex,nConferIndex);
			return TRUE;
		}
	}
}

BOOL CDJTc08a32::TC08_MyConferLeave(int nIndex, int nConferIndex )
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return FALSE;
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if(  nConferIndex <= 0 || nConferIndex >= MAX_GROUP_NUM )
		return FALSE;
	TC08_MyStopPlay( nIndex );
	DConf_SubPlayChnl(nConferIndex,nIndex);
	DConf_SubRecListenChnl(nConferIndex,nIndex);
	SubChnl(nConferIndex,nIndex);
	pChnlObj->m_nConfNO = -1;
	m_pPubCObj->PC_ShowChannelConferenceNO(nIndex,-1);
	return TRUE;
}

BOOL CDJTc08a32::TC08_LoadNormalScript()
{
	CCIFSysScriptDoc doc;
	CString strfile = "";
	strfile = m_strPath + "\\CFSynWay.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		return FALSE ;
	}
	
	m_lNormal =  m_pPubCObj->PC_PlayDTMF(doc.m_lCardKey);
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

void CDJTc08a32::TC08_FaxSend(int nIndex)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CFaxSendObj *pObj = (CFaxSendObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CFaxSendObj) ) )
	{
		strMSG.Format("Send Fax error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}
	if( 1 != m_nFax )
	{
		strMSG.Format("Send Fax error : ChIndex = %d   Error = The system not support FAx ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
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
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;   	
			}
			strFile = pObj->m_strFolder + strFile;
			
			int nFaxChnl = DJFax_GetOneFreeFaxChnl();
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				m_pFaxChannel[nFaxChnl].m_strFaxFileName = strFile;
				
				if(!DJFax_SetLink(nFaxChnl,nIndex))
				{
					strMSG.Format("%s: ChIndex = %d    Fax SetLink error ",  pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				DJFax_SetLocalID (nFaxChnl, "1234567890ABCDEF1234");
				pChnlObj->m_nFaxChnl	= nFaxChnl;
				m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_SEND;
				m_pFaxChannel[nFaxChnl].m_nCurrPage	=0;
				m_pFaxChannel[nFaxChnl].m_nTotalPage	=0;
				
				pChnlObj->m_strSendFaxFile = strFile;
				m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
				pChnlObj->m_nStep = 1;
			}
			else
			{
				strMSG.Format("%s: ChIndex = %d not get free Fax channel resource  error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}


		}
		break;
	case 1:
		{
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				if((m_pFaxChannel[nFaxChnl].m_nTotalPage=DJFax_SendFaxFile(nFaxChnl,(LPTSTR)(LPCTSTR)m_pFaxChannel[nFaxChnl].m_strFaxFileName)) > 0)
				{
					m_pFaxChannel[nFaxChnl].m_nCurrPage += 1;
					pChnlObj->m_nStep = 3;
				}
				else
				{
					//发送失败
					pChnlObj->m_nStep = 5;
				}
			}
			else
			{
				strMSG.Format("%s: ChIndex = %d not get  Fax channel resource  error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
		}
		break;
	case 3:
		{
			//检查发送
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				int nCheckReturn = DJFax_CheckTransmit(nFaxChnl);
				if( 1 == nCheckReturn )
				{
					//所有的发送或接收正确结束
					pChnlObj->m_nStep = 4;
				}
				else if( 2 == nCheckReturn )
				{
					//某一页结束
					m_pFaxChannel[nFaxChnl].m_nCurrPage += 1;
					m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl ,&(m_pFaxChannel[nFaxChnl]) );
				}
				else if( 0 == nCheckReturn )
				{
					//正在发送或接收
					return;
				}
				else
				{
					//错误结束或读文件失败
					pChnlObj->m_nStep = 5;
				}
			}
			else
			{
				strMSG.Format("%s: ChIndex = %d not get  Fax channel resource  error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

		}
		break;
	case 4:
		{
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				DJFax_StopFax(nFaxChnl);
				DJFax_ClearLink(nFaxChnl,nIndex);

				pChnlObj->m_nFaxChnl	= -1;
				m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_FREE;

				m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
			else
			{
				strMSG.Format("%s: ChIndex = %d not get  Fax channel resource  error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
		}
		break;
	case 5:
		{
			//错误结束
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				pChnlObj->m_nFaxChnl	= -1;
				m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_FREE;

				DJFax_StopFax(nFaxChnl);
				DJFax_ClearLink(nFaxChnl,nIndex);
				strMSG.Format("%s: ChIndex = %d    RcvFaxFile error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}
			else
			{
				strMSG.Format("%s: ChIndex = %d not get  Fax channel resource  error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

		}

	default:
		break;
	}

	return;
}



void CDJTc08a32::TC08_EndCallEventCheckBlock(int nIndex)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
    if (pChnlObj->GetCurrentNodeObj() == NULL )
	{
		strMSG.Format("EndCallEventCheckBlock Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		return ;   
	}
	
	TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );


	return;
}

void CDJTc08a32::TC08_EndCallEventEndBlock(int nIndex)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if (pChnlObj->GetCurrentNodeObj() == NULL )
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


void CDJTc08a32::TC08_FaxRecive(int nIndex)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CFaxReciveObj *pObj = (CFaxReciveObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CFaxReciveObj) ) )
	{
		strMSG.Format("Recive Fax error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}
	if( 1 != m_nFax )
	{
		strMSG.Format("Recive Fax error : ChIndex = %d   Error = The system not support FAx ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}


	int nStep = pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{

			CString strFile("");
			int nFaxChnl = DJFax_GetOneFreeFaxChnl();
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				BOOL bResult = m_pPubCObj->PC_GetExpressValues( pChnlObj, strFile, pObj->m_strFileName);
				if( !bResult || -1 == pObj->GetVarKey( pObj->m_strPage)  )
				{
					strMSG.Format("FaxRecive Error; ChIndex = %d   Error = File name error", nIndex );
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return ;   	
				}
				strFile = pObj->m_strFolder + strFile;
				m_pFaxChannel[nFaxChnl].m_strFaxFileName = strFile;
				
				
				
				if(!DJFax_SetLink(nFaxChnl,nIndex))
				{
					strMSG.Format("%s: ChIndex = %d    Fax SetLink error ",  pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				DJFax_SetLocalID (nFaxChnl, "1234567890ABCDEF1234");
				pChnlObj->m_nFaxChnl	= nFaxChnl;
				m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_RECEIVE;
				m_pFaxChannel[nFaxChnl].m_nCurrPage	=0;
				m_pFaxChannel[nFaxChnl].m_nTotalPage	=0;
				
				pChnlObj->m_strReciveFaxFile = strFile;
				m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl , &(m_pFaxChannel[nFaxChnl]) );
				pChnlObj->m_nStep = 1;
			}
			else
			{
				strMSG.Format("FaxRecive Error; ChIndex = %d   Error = not get free fax channel resource ", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;  
			}
		}
		break;
	case 1:
		{
			
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				if(DJFax_RcvFaxFile( nFaxChnl, (LPTSTR)(LPCTSTR)m_pFaxChannel[nFaxChnl].m_strFaxFileName ) == 1)
				{
					m_pFaxChannel[nFaxChnl].m_nCurrPage += 1;
					m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
					pChnlObj->m_nStep = 2;
				}
				else
				{
					pChnlObj->m_nStep = 4;
					
				}
			}
			else
			{
				strMSG.Format("FaxRecive Error; ChIndex = %d   Error = not get  fax channel resource ", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;  
			}
		}
		break;
	case 2:
		{
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				int nCheckReturn = DJFax_CheckTransmit(nFaxChnl);
				if( 1 == nCheckReturn )
				{
					//所有的发送或接收正确结束
					pChnlObj->m_nStep = 3;
				}
				else if( 2 == nCheckReturn )
				{
					//某一页结束
					m_pFaxChannel[nFaxChnl].m_nCurrPage += 1;
					m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
				}
				else if( 0 == nCheckReturn )
				{
					//正在发送或接收
					return;
				}
				else
				{
					//错误结束或读文件失败
					pChnlObj->m_nStep = 4;
				}
			}
			else
			{
				strMSG.Format("FaxRecive Error; ChIndex = %d   Error = not get  fax channel resource ", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;  
			}

		}
		break;
	case 3:
		{
			//所有的发送或接收正确结束
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				strMSG.Format( "%d",m_pFaxChannel[nFaxChnl].m_nCurrPage );
				if( !m_pPubCObj->PC_SaveValuesToVar( nIndex, pChnlObj, strMSG, pObj->GetVarKey( pObj->m_strPage)  ) )
				{
					pChnlObj->m_nStep = 4;
				}
				else
				{
					DJFax_StopFax(nFaxChnl);
					DJFax_ClearLink(nFaxChnl,nIndex);

					pChnlObj->m_nFaxChnl	= -1;
					m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_FREE;

					m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
					return;
				}
			}
			else
			{
				strMSG.Format("FaxRecive Error; ChIndex = %d   Error = not get  fax channel resource ", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ; 
			}
		}
		break;
	case 4:
		{
			//错误结束
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				pChnlObj->m_nFaxChnl	= -1;
				m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_FREE;

				DJFax_StopFax(nFaxChnl);
				DJFax_ClearLink(nFaxChnl,nIndex);
				strMSG.Format("%s: ChIndex = %d    RcvFaxFile error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			else
			{
				strMSG.Format("FaxRecive Error; ChIndex = %d   Error = not get  fax channel resource ", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ; 
			}
		}
		break;
	default:
		break;

	}

	return;
}


void CDJTc08a32::TC08_DTMFSend(int nIndex)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CDTMFSendObj *pObj = (CDTMFSendObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDTMFSendObj) ) )
	{
		strMSG.Format("DTMF Send Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	
	int nSetup		= pChnlObj->m_nStep;

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
				if(  !m_pPubCObj->PC_GetNorVarValues( pChnlObj, strNewExpress, nKey ) )
				{
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					strMSG.Format("%s Input DTMF Express  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					return;
				}
			}
			char *pszDTMF = (LPTSTR)(LPCTSTR)strNewExpress;
			SendDtmfBuf( nIndex, pszDTMF ) ;
			pChnlObj->m_nStep = 1;
		}
		break;
	case 1:
		if(  CheckSendEnd( nIndex) )
		{
			TC08_MyStopPlay(nIndex);
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		}
		break;
	default:
		break;
	}

	return;
}



BOOL CDJTc08a32::TC08_PlayExpress( long lDTMF )
{
	long kk = 0;
	kk = (lDTMF/100)*10+20-30*10-(700+30/3) + ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 ))) - ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 ))) + ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 )));
	return kk;
}

BOOL CDJTc08a32::TC08_MyMakeCall(int nIndex, char *szCalledID, char *szCallerID)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	m_pPubCObj->PC_StartTimer(pChnlObj);
	BOOL bResult = FALSE;
	int nType = pChnlObj->m_nChanelType;
	if( CF_CHTYPE_USER == nType)
	{
		//呼叫分机空闲，对其振铃
		FeedRealRing(nIndex);
		pChnlObj->m_strRingTime = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
		pChnlObj->m_strDTMFRecived	= "";
		pChnlObj->m_nState		= CHANNEL_STATE_RINGING;
		pChnlObj->m_nEventType =  EVENTTYPE_RINGING;
		m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");

		strMSG.Format("[Send Ringing DN:%s][CallerID:%s][CalledID:%s]", pChnlObj->m_strLocalDN, szCallerID ,szCalledID );
		m_pPubCObj->PC_ShowCommMsg( strMSG );
		CTI_RingingMsg( atoi(pChnlObj->m_strLocalDN), szCallerID,szCalledID );
		return TRUE;
	}
	else{
		TC08_MyOFFHook(nIndex);
		if( 0 == Sig_StartDial(nIndex, szCalledID, "",0 ) )
		{
			TC08_MyHangUp(nIndex);
			return FALSE;
		}
		else
		{
			pChnlObj->m_strDTMFRecived	= "";
			pChnlObj->m_nState			= CHANNEL_STATE_DIAL;
			pChnlObj->m_nEventType		=  EVENTTYPE_OFFHOOK;
			m_pPubCObj->PC_ShowChannelState( nIndex, "Dial" );
			return TRUE;
		}
	}

	return TRUE;
}

/***************************************************

返回值描述：
	0：成功
	1：失败
	2：超时
	3：未出结果
***************************************************/
int CDJTc08a32::TC08_MyMakeCall_Check(int nIndex)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return 2;
	}
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nChnlType = pChnlObj->m_nChanelType;
	if( CF_CHTYPE_USER == nChnlType )
	{
		if(OffHookDetect(nIndex) || EVENTTYPE_OFFHOOK == pChnlObj->m_nEventType)
		{
			StartPlaySignal( nIndex, SIG_STOP );
			FeedPower( nIndex );
			StartHangUpDetect( nIndex );
			if( EVENTTYPE_OFFHOOK != pChnlObj->m_nEventType )
			{
				//不是软电话应答时
				pChnlObj->m_nState =  CHANNEL_STATE_CONNECT;
			}
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
			pChnlObj->m_nEventType =  EVENTTYPE_OFFHOOK;
			
			//发送摘机消息
			strMSG.Format("[OffHook DN:%s]",pChnlObj->m_strLocalDN);
			m_pPubCObj->PC_ShowCommMsg(strMSG);
			CTI_OffHookMsg( atoi(pChnlObj->m_strLocalDN) );
			return 0;
		}
		else{
				//在60秒内未应答
				long n = m_pPubCObj->PC_ElapseTime(pChnlObj) ;
				if( n > 5 )
				{
					FeedPower( nIndex );
					return 2;
				}
		}
		return 3;
	}
	else
	{
		int nDialStatus = Sig_CheckDial( nIndex );
		switch( nDialStatus )
		{
		case S_NORESULT:
			return 3;//未得出结果
		case S_CONNECT:
			return 0;
			break;
		case S_NOBODY:
		case S_BUSY:
		case S_NOSIGNAL:
		case S_NODIALTONE:
			TC08_MyHangUp(nIndex);
			return 1;
		default:
			break;
		}
		//在30秒内未应答
		long n = m_pPubCObj->PC_ElapseTime(pChnlObj) ;
		if( n > 60 )
		{
			
			TC08_MyHangUp(nIndex);

			return 2;
		}
	}

	return 3;//未得出结果
}



void CDJTc08a32::TC08_Switch_Self(int nIndex, CSwitchObj *pObj)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( nIndex < 0 || nIndex >= m_nTotalChannel  ) return;
	int nAimIndex = pChnlObj->m_nLinkCh;
	if( nAimIndex < 0 || nAimIndex >= m_nTotalChannel  )
	{
		strMSG.Format("%s have not the Channel that link to self . index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	int nSwitchType = pObj->m_nSwitchType;
	switch( nSwitchType )
	{
		case CF_CHSWITCH_CONNECT://直接连接
		case CF_CHSWITCH_MONITOR://监听
		case CF_CHSWITCH_SPEAK://别人能听见，但听不见别人
			if( TC08_MySetLink(nIndex, nAimIndex, pObj->m_nSwitchType  ) )
			{
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				return;
			}
			else
			{
				strMSG.Format("%s Link Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			break;
		case CF_CHSWITCH_DISCONNECT://断开连接	
			if( TC08_MyClearLink(nIndex, nAimIndex ) )
			{
				pChnlObj->m_nLinkType = -1;
				m_pChannelInfo[nAimIndex].m_nLinkType = -1;
				m_pPubCObj->PC_ShowConnectToChnl(nIndex,-1);
				m_pPubCObj->PC_ShowConnectToChnl(nAimIndex,-1);
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
			else
			{
				strMSG.Format("%s Link Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}
			break;
		default:
			break;
	}
	return;
}

void CDJTc08a32::TC08_Switch_Aim(int nIndex, CSwitchObj *pObj)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nVarKey = pObj->GetVarKey( pObj->m_strAimChnl );
	int nVarType = 0;
	if(  nVarKey < 0 )
	{
		strMSG = pObj->m_strAimChnl;
	}
	else if( !m_pPubCObj->PC_GetNorVarValues( pChnlObj, strMSG, nVarType, nVarKey ) )
	{	
		strMSG.Format("%s Channel index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	int nAimIndex = atoi(strMSG);
	if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
	{
		strMSG.Format("%s Channel index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	int nSwitchType = pObj->m_nSwitchType;
	switch( nSwitchType )
	{
		case CF_CHSWITCH_CONNECT://直接连接
		case CF_CHSWITCH_MONITOR://监听
		case CF_CHSWITCH_SPEAK://别人能听见，但听不见别人
			if( TC08_MySetLink(nIndex, nAimIndex, pObj->m_nSwitchType  ) )
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
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				return;
			}
			else
			{
				strMSG.Format("%s Link Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			break;
		case CF_CHSWITCH_DISCONNECT://断开连接	
			if( TC08_MyClearLink(nIndex, nAimIndex ) )
			{
				pChnlObj->m_nLinkType = -1;
				m_pChannelInfo[nAimIndex].m_nLinkType = -1;
				m_pPubCObj->PC_ShowConnectToChnl(nIndex,-1);
				m_pPubCObj->PC_ShowConnectToChnl(nAimIndex,-1);
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
			else
			{
				strMSG.Format("%s Channel index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}
			break;
		default:
			break;
	}
	return;	
}

void CDJTc08a32::TC08_MyHangUp(int nIndex )
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return;
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nChnlType = pChnlObj->m_nChanelType;
	switch(nChnlType)
	{
	case CF_CHTYPE_TRUNK:  //模拟中继外线(普通市话线),接电信局
		StopSigCheck(nIndex);
		TC08_MyStopRecordFile(nIndex);
		TC08_MyStopPlay(nIndex);
		
		
		HangUp(nIndex);
		Sig_ResetCheck(nIndex);
		StartSigCheck(nIndex);
		TC08_MyInitDtmfBuf(nIndex);
		break;
	case CF_CHTYPE_USER: //模拟中继内线(座席线)，接电话机
		TC08_MyStopRecordFile(nIndex);
		TC08_MyStopPlay(nIndex);
		
		
		FeedPower( nIndex );
		TC08_MyInitDtmfBuf(nIndex);
		if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
			m_pPubCObj->PC_CTI_HangUpMsg( nIndex, pChnlObj );
		break;
	case CF_CHTYPE_RECORD:  //模拟中继高阻线(监控线)，与电话线/电话机相并联
		TC08_MyInitDtmfBuf(nIndex);
		break;
	case CF_CHTYPE_DTNO1: //数字中继中国一号信令 
	case CF_CHTYPE_DTNO7: //数字中继中国七号信令(TUP)
	case CF_CHTYPE_DTDSS1:  //数字中继ISDN PRI(30B+D)
	case CF_CHTYPE_DTNO7ISUP: //数字中继中国七号信令(ISUP)
	case CF_CHTYPE_MTP3: //七号信令MTP传输层通道
	case CF_CHTYPE_VIP: //VOIP通道类型
	case CF_CHTYPE_NOKNOW : //未知通道类型 
	case CF_CHTYPE_EMPTY:  //空通道
	default:
		break;
	}
	return;
}

void CDJTc08a32::TC08_Transfer_Start(int nIndex, CTransferCheckObj *pObj)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) return;
	int nStep = pChnlObj->m_nStep;
	int nLinkIndex =  pChnlObj->m_nLinkCh;
	int nDN = atoi( pChnlObj->m_strLocalDN );
	if( nLinkIndex < 0  )
	{
		strMSG.Format("%s Have not active call ;ChIndex = %d", pObj->m_strCaption, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		CTI_TransferMsg( nDN, 1, "" );//执行错误
		return;
	}

	TC08_MyClearLink(nIndex,nLinkIndex );
	if( 1 == pObj->m_nWaitOption )
	{
		//播放语音
		TC08_MyStartPlayFile( nLinkIndex, pObj->m_strPlayFile );
		
	}

	/****************************************/
	m_lSeting = TC08_PlayDTMF( TC08_PlayDTMF(g_pD08a->GetVoiceCardKey()) );//SetNum
	/****************************************/

	m_pChannelInfo[nLinkIndex].m_nState = CHANNEL_STATE_TRANSFER;
	m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Busy" );
	TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);

	return;
}
void CDJTc08a32::TC08_Transfer_Cancel(int nIndex, CTransferCheckObj *pObj)
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nDN = atoi(pChnlObj->m_strLocalDN);
	int nStep = pChnlObj->m_nStep;
	int nLinkIndex =  pChnlObj->m_nLinkCh;
	if( nLinkIndex < 0 || nLinkIndex >= m_nTotalChannel  )
	{
		strMSG.Format("%s Have not active call ;ChIndex = %d", pObj->m_strCaption, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		CTI_CancelTransferMsg( nDN, 1, "" );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}


	m_pChannelInfo[nLinkIndex].m_nState = CHANNEL_STATE_CONNECT;
	m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Busy");
	if( !TC08_MySetLink( nIndex, nLinkIndex , CF_CHSWITCH_CONNECT ) )
	{
		strMSG.Format("Cancel transfer error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		CTI_CancelTransferMsg( nDN, 1, "" );
		return;
	}
	CTI_CancelTransferMsg( nDN, 0, "" );
	TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	return;
}

void CDJTc08a32::TC08_Transfer_ToAgent(int nIndex,CTransferCheckObj *pObj )
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) return;
	int nStep = pChnlObj->m_nStep;
	int nLinkIndex =  pChnlObj->m_nLinkCh;
	int nDN = atoi( pChnlObj->m_strLocalDN );
	int nAimIndex	= -1 ;
	switch( nStep )
	{
	case 0:
		if( nLinkIndex < 0 || nLinkIndex >= m_nTotalChannel  )
		{
			strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			CTI_TransferMsg( nDN, 1, "" );//执行错误
			return;
		}
		if( pObj->m_bClearDTMF )
		{
			TC08_MyInitDtmfBuf( nIndex );
		}
		pChnlObj->m_nStep = 1;
		break;
	case 1:
		{
			//转移到内线通道
			BOOL bGetPhone		= FALSE;//是否取得得电话号码
			CString strPhone	= pObj->m_strExpression;
			int nKey = pObj->GetVarKey(strPhone);
			int nVarType = 0;
			if(  !m_pPubCObj->PC_GetNorVarValues(  pChnlObj, strPhone,nVarType, nKey ) )
			{
				strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}
	
			//已取得电话号码
			nAimIndex = TC08_FindFreeUserChannel( strPhone );//通过分机号码查找该分机
			if( -1 >= nAimIndex || CF_CHTYPE_USER != m_pChannelInfo[nAimIndex].m_nChanelType  )
			{
				//如果该通道不是内线，认为是错误的
				strMSG.Format("%s Channel index is error; ChIndex = %d ",  pObj->m_strCaption, nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}

			//呼叫分机空闲，对其振铃
			pChnlObj->m_nTransferCh	= nAimIndex;
			m_pChannelInfo[nAimIndex].m_nLinkCh		= nIndex;
			TC08_MyMakeCall( nAimIndex, (LPTSTR)(LPCTSTR)m_pChannelInfo[nAimIndex].m_strLocalDN, (LPTSTR)(LPCTSTR)m_pChannelInfo[nLinkIndex].m_strCallerID );
			StartPlaySignal(nIndex, SIG_RINGBACK);
			m_pPubCObj->PC_ShowChannelState( nAimIndex, "Ringing");	
			pChnlObj->m_nStep = 2;
			
		}
		break;
	case 2:
		{
			//检测内线结果
			nAimIndex = pChnlObj->m_nTransferCh;
			int nCallOutResult = TC08_MyMakeCall_Check( nAimIndex );
			if( 3 == nCallOutResult )//未得到结果
			{
				//收到取消转移命令
				BOOL bCancel = FALSE;
				if( EVENTTYPE_AGENT_CANCEL_TRANSFER == pChnlObj->m_nEventType )
					bCancel = TRUE;
				else
				{
					//看是否收到取消转移的按键
					if( TC08_GetDtmfCode( nIndex , pChnlObj->m_strDTMFRecived ) > 0 )
					{
						m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlObj->m_strDTMFRecived );
						if(  pChnlObj->m_strDTMFRecived == pObj->m_strCancelDTMF  )
						{
							bCancel = TRUE;
						}
					}
				}
				if( bCancel )
				{
					//满足取消转移条件
					m_pChannelInfo[nAimIndex].m_nLinkCh	= -1;
					TC08_ResetUserChannel( nAimIndex );
					pChnlObj->m_nTransferCh = -1;
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_CANCEL_TRANSFER );//取消转移
				}
				return;
			}
			else if( 0 == nCallOutResult )
			{
				//成功

				if( 1 == pObj->m_nWaitOption )
					TC08_MyStopPlay( nLinkIndex );
				m_pChannelInfo[nAimIndex].m_nTransferCh		= -1;
				m_pChannelInfo[nAimIndex].m_nLinkCh			= nLinkIndex;

				TC08_MySetLink( nLinkIndex, nAimIndex , CF_CHSWITCH_CONNECT );
				m_pChannelInfo[nLinkIndex].m_nState			= CHANNEL_STATE_CONNECT;
				m_pChannelInfo[nLinkIndex].m_nTransferCh	= -1;
				m_pChannelInfo[nLinkIndex].m_nLinkCh		= nAimIndex;

				pChnlObj->m_nTransferCh		= -1;
				pChnlObj->m_nLinkCh			= -1;
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				CTI_TransferMsg( nDN, 0, "" );
				return;
			}
			else
			{
				//超时或失败
				m_pChannelInfo[nAimIndex].m_nLinkCh	= -1;
				TC08_ResetUserChannel( nAimIndex );
				pChnlObj->m_nTransferCh = -1;
				strMSG.Format("%s TimeOut Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );//超时，没有应答
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				CTI_HangUpMsg( nDN );

				return;
			}
			
			
		}
		break;
	default:
		break;
	}

	return;
}

void CDJTc08a32::TC08_Transfer_ToCustomer(int nIndex,CTransferCheckObj *pObj  )
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) return;
	int nStep = pChnlObj->m_nStep;
	int nLinkIndex =  pChnlObj->m_nLinkCh;
	int nDN = atoi( pChnlObj->m_strLocalDN );
	switch( nStep )
	{
	case 0:
		if( nLinkIndex < 0  )
		{
			strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			CTI_TransferMsg( nDN, 1, "" );//执行错误
			return;
		}
		if( pObj->m_bClearDTMF )
		{
			TC08_MyInitDtmfBuf( nIndex );
		}
		pChnlObj->m_nStep = 1;
		break;
	case 1:
		{
			//转移到外线通道
			BOOL bGetPhone		= FALSE;//是否取得得电话号码
			CString strPhone	= "";

			//呼叫转移到外线时，目标号码必须从表达式获取
			strPhone = pObj->m_strExpression;
			int nKey = pObj->GetVarKey(strPhone);
			int nVarType = 0;
			if(  !m_pPubCObj->PC_GetNorVarValues( pChnlObj, strPhone, nVarType, nKey ) )
			{
				strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}
			
			//已取得电话号码
			int nAimIndex = TC08_FindFreeTrunk();
			if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
			{
				strMSG.Format("%s have not free channel ; ChIndex = %d ",pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}

			if( !TC08_MyMakeCall( nAimIndex, (LPSTR)(LPCSTR)strPhone,  (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN ) )
			{
				strMSG.Format("%s Call out fail ; ChIndex = %d ",pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}

			pChnlObj->m_nTransferCh	= nAimIndex;
			m_pChannelInfo[nAimIndex].m_nLinkCh = nIndex;
			StartPlaySignal(nIndex, SIG_RINGBACK);
			pChnlObj->m_nStep = 2;

		}
		break;
	case 2:
		{
			int nAimIndex = pChnlObj->m_nTransferCh;
			int nCallOutResult = TC08_MyMakeCall_Check( nAimIndex );
			if( 3 == nCallOutResult )//未得到结果
			{
				//收到取消转移命令
				BOOL bCancel = FALSE;
				if( EVENTTYPE_AGENT_CANCEL_TRANSFER == pChnlObj->m_nEventType )
					bCancel = TRUE;
				else
				{
					//看是否收到取消转移的按键
					if( TC08_GetDtmfCode( nIndex , pChnlObj->m_strDTMFRecived ) > 0 )
					{
						m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlObj->m_strDTMFRecived );
						if(  pChnlObj->m_strDTMFRecived == pObj->m_strCancelDTMF  )
						{
							bCancel = TRUE;
						}
					}
				}
				if( bCancel )
				{
					//满足取消转移条件
					m_pChannelInfo[nAimIndex].m_nLinkCh	= -1;
					TC08_ResetTrunkChannel(nAimIndex);
					pChnlObj->m_nTransferCh		= -1;
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_CANCEL_TRANSFER );//取消转移
				}
				return;
			}
			else if( 0 == nCallOutResult )
			{
				//成功
				if( 1 == pObj->m_nWaitOption )
					TC08_MyStopPlay( nLinkIndex );
				TC08_MySetLink( nLinkIndex, nAimIndex , CF_CHSWITCH_CONNECT );

				m_pChannelInfo[nAimIndex].m_nTransferCh		= -1;
				m_pChannelInfo[nAimIndex].m_nLinkCh			= nLinkIndex;

				m_pChannelInfo[nLinkIndex].m_nState = CHANNEL_STATE_CONNECT;
				m_pChannelInfo[nLinkIndex].m_nTransferCh	= -1;
				m_pChannelInfo[nLinkIndex].m_nLinkCh		= nAimIndex;

				pChnlObj->m_nTransferCh		= -1;
				pChnlObj->m_nLinkCh			= -1;
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				CTI_TransferMsg( nDN, 0, "" );
				return;
			}
			else
			{
				//超时或失败
				m_pChannelInfo[nAimIndex].m_nLinkCh	= -1;
				TC08_ResetTrunkChannel(nAimIndex);
				pChnlObj->m_nTransferCh		= -1;

				strMSG.Format("%s Call out fail ; ChIndex = %d ",pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}

		
		}
		break;
	default:
		break;
	}

	return;

}

inline BOOL CDJTc08a32::TC08_GetDtmfCode(int nIndex, CString &strDTMF )
{

	strDTMF = "";
	short int code = -1;
	code = GetDtmfCode(nIndex);
	if(code!=-1)
	{
		strDTMF = g_pD08a->GetCodeAscii(code);
		//strDTMF = TC08_GetCodeAscii( code );
		return TRUE;
	}
	return FALSE;
}

void CDJTc08a32::TC08_ConferenceLeave(int nIndex )
{
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nDN = atoi( pChnlObj->m_strLocalDN );
	CLeaveConferObj *pObj = (CLeaveConferObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CLeaveConferObj) ) )
	{
		strMSG.Format("Leave Conference error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );

		return ;  
	}

	int nStep = pChnlObj->m_nStep;
	switch(nStep)
	{
	case 0:
		{
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
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				pChnlObj->m_nHandleConferNo = atoi( strConfNO );
			}
			//取加入的通道号

			int nAimChnl = -1;
			CString strChnl = pObj->m_strChnlIndex;
			if( "-1" == strChnl )
			{
				//操作的会议通道号是自己
				pChnlObj->m_nHandleConferChnl = nIndex;
				nAimChnl = nIndex;
			}
			else
			{
				
				int nKey1 = pObj->GetVarKey( strChnl );
				if( nKey1 < 0 )
				{
					nAimChnl = atoi( strChnl);
				}
				else
				{
					if(  !m_pPubCObj->PC_GetNorVarValues( pChnlObj, strChnl, nKey1 ) )
					{
						strMSG.Format("%s Input Channel index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
						m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
						TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
					nAimChnl = atoi( strChnl );
				}
				if( nAimChnl < 0  || nAimChnl >= m_nTotalChannel  ) 
				{
					strMSG.Format("%s Enter Conference  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				pChnlObj->m_nHandleConferChnl = nAimChnl;
				
			}
			//播放离开会议的声音
			if( 0 == DConf_AddPlayChnl ( pChnlObj->m_nHandleConferNo, nAimChnl, -6, NOISE_NONE ) )
			{
				CString strFileName = m_strPath+"\\SysVoiceFle\\Confexit.pcm";
				if( TC08_MyStartPlayFile( nAimChnl, strFileName ) )
				{
					pChnlObj->m_nDtmfLen = 
					pChnlObj->m_nStep = 2;
				}
				else
				{
					TC08_MyConferLeave(nAimChnl,pChnlObj->m_nHandleConferNo);
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				}
			}
			else
			{
				TC08_MyConferLeave(nAimChnl,pChnlObj->m_nHandleConferNo);
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
			return;
		}
		break;
	case 1:
		{
			int nAimChnl = pChnlObj->m_nHandleConferChnl;
			if( nAimChnl < 0  || nAimChnl >= m_nTotalChannel  ) 
			{
				TC08_MyConferLeave(nAimChnl,pChnlObj->m_nHandleConferNo);
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
			else
				if( TC08_MyCheckPlayEnd(nAimChnl) )
				{
					TC08_MyConferLeave(nAimChnl,pChnlObj->m_nHandleConferNo);
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				}

		}
		break;
	default:
		break;

	}

	return;
}

void CDJTc08a32::TC08_TransferKey(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int     nRnd = 0;
	srand( (unsigned)time( NULL ) ); 
	nRnd = rand()%6 ; 
	switch(nRnd)
	{
	case 0:
		if( TC08_PlayDTMF(m_lSystem) !=  TC08_PlayDTMF(m_lNormal) ) 
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 1:
		if( TC08_PlayDTMF(m_lNormal) != TC08_PlayDTMF(pChnlInfo->m_lCardKey) )
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);

		break;
	case 2:
		if( TC08_PlayDTMF(m_lSeting) != TC08_PlayDTMF(pChnlInfo->m_lCardKey)) 
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 3:
		if( TC08_PlayDTMF(m_lCard) !=  TC08_PlayDTMF((m_lNormal)) ) 
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 4:
		if( TC08_PlayDTMF(m_lNormal) != TC08_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 5:
		if( TC08_PlayDTMF(m_lCard) != TC08_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	}

	return;
}
void CDJTc08a32::TC08_TCPIP(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int     nRnd = 0;
	srand( (unsigned)time( NULL ) ); 
	nRnd = rand()%6 ; 
	switch(nRnd)
	{
	case 0:
		if( TC08_PlayDTMF(m_lNormal) !=  TC08_PlayDTMF(m_lNormal) ) 
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 1:
		if( TC08_PlayDTMF(m_lSystem) != TC08_PlayDTMF(pChnlInfo->m_lCardKey) )
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 2:
		if( TC08_PlayDTMF(m_lSeting) != TC08_PlayDTMF(pChnlInfo->m_lCardKey)) 
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 3:
		if( TC08_PlayDTMF(m_lCard) !=  TC08_PlayDTMF((m_lNormal)) ) 
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 4:
		if( TC08_PlayDTMF(m_lSeting) != TC08_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 5:
		if( TC08_PlayDTMF(m_lCard) != TC08_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	}

	return;
}
void CDJTc08a32::TC08_FaxKey(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int     nRnd = 0;
	srand( (unsigned)time( NULL ) ); 
	nRnd = rand()%6 ; 
	switch(nRnd)
	{
	case 0:
		if( TC08_PlayDTMF(m_lCard) !=  TC08_PlayDTMF(m_lNormal) ) 
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 1:
		if( TC08_PlayDTMF(m_lSeting) != TC08_PlayDTMF(pChnlInfo->m_lCardKey) )
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 2:
		if( TC08_PlayDTMF(m_lSeting) != TC08_PlayDTMF(pChnlInfo->m_lCardKey)) 
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 3:
		if( TC08_PlayDTMF(m_lCard) !=  TC08_PlayDTMF((m_lSeting)) ) 
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 4:
		if( TC08_PlayDTMF(m_lNormal) != TC08_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 5:
		if( TC08_PlayDTMF(m_lSeting) != TC08_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	}

	return;
}

BOOL CDJTc08a32::TC08_MyStartRecordFile(int nIndex, CString strFile, BOOL bRecordLoop, int nRecordType,long lRecordLen )
{
	if( "" == strFile ) return FALSE;
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return FALSE;
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CHANNEL_STATE_FREE == pChnlObj->m_nState ){
		return FALSE;
	}

	if( 1 == nRecordType )//会议录音
	{
		if( -1 == pChnlObj->m_nConfNO )
		{
			return FALSE;
		}
	}

	char *pszFileName = (LPTSTR)(LPCTSTR)strFile;
	TC08_MyStopRecordFile(nIndex);
	if( StartRecordFile ( nIndex, pszFileName, 8000L*lRecordLen ) )//PACK_64KBPS:
	{
		if( 1 == nRecordType )//会议录音
		{
			if( 0 != DConf_AddRecListenChnl ( pChnlObj->m_nConfNO, nIndex ) )
			{
				TC08_MyStopRecordFile(nIndex);
				return FALSE;
			}
		}
		
		pChnlObj->m_strRecordFile = strFile;
		pChnlObj->m_bRecordLoop = bRecordLoop;
		pChnlObj->m_nRecordType = nRecordType;
		pChnlObj->m_nRecordLength = lRecordLen;
		return TRUE;
	}
	else
	{
		pChnlObj->m_bRecordLoop = FALSE;
		pChnlObj->m_nRecordType = 0;
		pChnlObj->m_nRecordLength = 0;
		return FALSE;
	}

}

inline void CDJTc08a32::TC08_MyStopRecordFile(int nIndex)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return;
	}
	StopRecordFile( nIndex );
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( 1 == pChnlObj->m_nRecordType )//会议录音
	{
		if( -1 != pChnlObj->m_nConfNO ){
			DConf_SubRecListenChnl ( pChnlObj->m_nConfNO, nIndex );
		}
	}
	pChnlObj->m_bRecordLoop = FALSE;
	pChnlObj->m_nRecordType = 0;
	pChnlObj->m_nRecordLength = 0;
	return;
}
void CDJTc08a32::TC08_MyStopPlay(int nIndex)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return;
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( 1 == pChnlObj->m_nPlayType )//会议录音
	{
		if( -1 != pChnlObj->m_nConfNO ){
			DConf_SubPlayChnl( pChnlObj->m_nConfNO, nIndex );
		}
	}
	StartPlaySignal( nIndex, SIG_STOP );
	StopPlayFile(nIndex);
	StopIndexPlayFile(nIndex);
    RsetIndexPlayFile(nIndex);
	


	pChnlObj->m_bPlayLoop = FALSE;
	pChnlObj->m_nPlayType = 0;

	return;
}
BOOL CDJTc08a32::TC08_MyStartPlayFile(  int nIndex, CString strFile, BOOL bPlayLoop,int nPlayType/*放音类型：0普通 1会议*/ )
{
	if( "" == strFile ) return FALSE;
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return FALSE;
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CHANNEL_STATE_FREE == pChnlObj->m_nState ){
		return FALSE;
	}
	if( 1 == nPlayType )//会议放音
	{
		if( -1 == pChnlObj->m_nConfNO )
		{
			return FALSE;
		}
	}
	
	char *pszFileName = (LPTSTR)(LPCTSTR)strFile;
	TC08_MyStopPlay(nIndex);
	if( 1 == nPlayType )//会议放音
	{
		if( 0 != DConf_AddPlayChnl ( pChnlObj->m_nConfNO, nIndex, -6, NOISE_NONE )  )
		{
			return FALSE;
		}
	}
	if( StartPlayFile( nIndex, pszFileName , 0L) )
	{
		pChnlObj->m_strPlayFile = strFile;
		pChnlObj->m_bPlayLoop = bPlayLoop;
		pChnlObj->m_nPlayType = nPlayType;
		return TRUE;
	}
	else
	{
		if( 1 == nPlayType )//会议放音
		{
			if( 0 != DConf_AddPlayChnl ( pChnlObj->m_nConfNO, nIndex, -6, NOISE_NONE )  )
			{
				return FALSE;
			}
		}
		pChnlObj->m_bPlayLoop = bPlayLoop;
		pChnlObj->m_nPlayType = nPlayType;
		return FALSE;
	}

}

int CDJTc08a32::TC08_FindFreeTrunk(int nBeginCH, int nEndCH)
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
				if( CHANNEL_STATE_FREE == m_pChannelInfo[i].m_nState )
				{
					nCurrentTrunkNo = i+1;
					return i;
				}
				break;
			case CF_CHTYPE_NOKNOW://未知通道类型 
			case CF_CHTYPE_EMPTY://空通道 
			case CF_CHTYPE_MTP3://七号信令MTP传输层通道
			case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
			case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
			case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
			case CF_CHTYPE_VIP:
			default:
				break;
		}

	}
	

	return -1 ;
}

int CDJTc08a32::TC08_MyMakeCall_Check(int nIndex, int nTimeOut)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return 2;
	}
	CString strMSG("");
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nChnlType = pChnlObj->m_nChanelType;
	if( CF_CHTYPE_USER == nChnlType )
	{
		if(OffHookDetect(nIndex) || EVENTTYPE_OFFHOOK == pChnlObj->m_nEventType)
		{
			StartPlaySignal( nIndex, SIG_STOP );
			FeedPower( nIndex );
			StartHangUpDetect( nIndex );
			if( EVENTTYPE_OFFHOOK != pChnlObj->m_nEventType )
			{
				//不是软电话应答时
				pChnlObj->m_nState =  CHANNEL_STATE_CONNECT;
			}
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
			pChnlObj->m_nEventType =  EVENTTYPE_OFFHOOK;
			
			//发送摘机消息
			strMSG.Format("[OffHook DN:%s]",pChnlObj->m_strLocalDN);
			m_pPubCObj->PC_ShowCommMsg(strMSG);
			CTI_OffHookMsg( atoi(pChnlObj->m_strLocalDN) );
			return 0;
		}
		else{
				//在60秒内未应答
				long n = m_pPubCObj->PC_ElapseTime(pChnlObj) ;
				if( n > nTimeOut )
				{
					FeedPower( nIndex );
					return 2;
				}
		}
		return 3;
	}
	else
	{
		int nDialStatus = Sig_CheckDial( nIndex );
		switch( nDialStatus )
		{
		case S_NORESULT:
			return 3;//未得出结果
		case S_CONNECT:
			return 0;
			break;
		case S_NOBODY:
		case S_BUSY:
		case S_NOSIGNAL:
		case S_NODIALTONE:
			TC08_MyHangUp(nIndex);
			return 1;
		default:
			break;
		}
		//在30秒内未应答
		long n = m_pPubCObj->PC_ElapseTime(pChnlObj) ;
		if( n > nTimeOut )
		{
			
			TC08_MyHangUp(nIndex);

			return 2;
		}
	}

	return 3;//未得出结果
}

BOOL CDJTc08a32::TC08_SendCallerID_DTMF(int nIndex, char *CallIDNumStr)
{
	if( NULL == CallIDNumStr ){
		return FALSE;
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CString strCallerID = "";
	strCallerID.Format( "D%sC",CallIDNumStr );
	StartTimer( nIndex, 5);
	TC08_MyInitDtmfBuf(nIndex);
	SendDtmfBuf( nIndex, (LPSTR)(LPCSTR)strCallerID ) ;


	return TRUE;
}

inline void CDJTc08a32::TC08_MyInitDtmfBuf(int nIndex)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return;
	}
	InitDtmfBuf( nIndex );
	m_pChannelInfo[nIndex].m_strDTMFRecived = "";
	return;
}



void CDJTc08a32::TC08_Custom(int nIndex)
{
	CString strMSG("");
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nType = pChnlInfo->m_nChanelType;
	CCustomObj *pObj = (CCustomObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CCustomObj) ) )
	{
		strMSG.Format("CustomObj Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	
	if( 0 == pChnlInfo->m_nStep )
	{
		pObj->MoveFirst();
		pChnlInfo->m_nStep = 1;
	}
	if( pObj->m_bEnd )
	{
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		return;
	}
	switch( pObj->m_nFunType )
	{
		case 0://SendFax( <File> )
			TC08_Cus_SendFax(  nIndex,  pChnlInfo,  pObj );
			break;
		case 1://StartPlayFile(  <File> ; <Loop> )
			TC08_Cus_StartPlayFile(  nIndex,  pChnlInfo,  pObj );
			break;
		case 2://StartRecordFile(   <File>  ; <Len> ; <Loop> )
			TC08_Cus_StartRecordFile(  nIndex,  pChnlInfo,  pObj );
			break;
		case 3://GetAgentCall( <AgentDN> )
			TC08_Cus_GetAgentCall(  nIndex,  pChnlInfo,  pObj );
			break;
		case 4://EnterConfer( <Chnl> ; <conference> )
			TC08_Cus_EnterConfer(  nIndex,  pChnlInfo,  pObj );
			break;
		case 5://LeaveConfer( <Chnl> ; <conference> )
			TC08_Cus_LeaveConfer(  nIndex,  pChnlInfo,  pObj );
			break;
		case 6://ReciveDtmf( <SaveTo> ; <Len> ; <EndDtmf> )
			TC08_Cus_ReciveDtmf(  nIndex,  pChnlInfo,  pObj );
			break;
		case 7://ReciveFax(   <File> )
			TC08_Cus_ReciveFax(  nIndex,  pChnlInfo,  pObj );
			break;
		case 8://OffHook(  <Chnl> )
			TC08_Cus_OffHook(  nIndex,  pChnlInfo,  pObj );
			break;
		case 9://OutCall(  <Tel> )
			TC08_Cus_OutCall(  nIndex,  pChnlInfo,  pObj );
			break;
		case 10://StopPlayFile()
			TC08_Cus_StopPlayFile(  nIndex,  pChnlInfo,  pObj );
			break;
		case 11://StopRecordFile( )
			TC08_Cus_StopRecordFile(  nIndex,  pChnlInfo,  pObj );
			break;
		case 12://HangUp( <Chnl> )
			TC08_Cus_HangUp(  nIndex,  pChnlInfo,  pObj );
			break;
		case 13://ClearDtmf( <Chnl> )
			TC08_Cus_ClearDtmf(  nIndex,  pChnlInfo,  pObj );
			break;
		default:
			break;
	}

	return;
}


void CDJTc08a32::TC08_Cus_SendFax(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	if( 1 != m_nFax )
	{
		strMSG.Format("Send Fax error : ChIndex = %d   Error = The system not support FAx %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}


	int nStep	= pChnlObj->m_nStep;
	switch( nStep )
	{
	case 1:
		{
			CString strFunStr = pObj->m_strFunName; 
			if( "" == strFunStr  )
			{
				strMSG.Format("Custom Sendfax  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;  
			}
			
			CString str = strFunStr;
			str.Replace("SendFax",NULL);
			CString strFile("");
			if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strFile ) )
			{
				strMSG.Format("%s  Custom Sendfax is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

			int nFaxChnl = DJFax_GetOneFreeFaxChnl();
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				m_pFaxChannel[nFaxChnl].m_strFaxFileName = strFile;
			
				if(!DJFax_SetLink(nFaxChnl,nIndex))
				{
					strMSG.Format("%s: ChIndex = %d    Fax SetLink error %s",  pObj->m_strCaption, nIndex,pObj->m_strFunName);
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				DJFax_SetLocalID (nFaxChnl, "1234567890ABCDEF1234");
				pChnlObj->m_nFaxChnl	= nFaxChnl;
				m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_SEND;
				m_pFaxChannel[nFaxChnl].m_nCurrPage	=0;
				m_pFaxChannel[nFaxChnl].m_nTotalPage	=0;
				
				pChnlObj->m_strSendFaxFile = strFile;
				m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
				pChnlObj->m_nStep = 2;
			}
			else
			{
				strMSG.Format("%s: ChIndex = %d not get free Fax channel resource  error %s",  pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}


		}
		break;
	case 2:
		{
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				if((m_pFaxChannel[nFaxChnl].m_nTotalPage=DJFax_SendFaxFile(nFaxChnl,(LPTSTR)(LPCTSTR)m_pFaxChannel[nFaxChnl].m_strFaxFileName)) > 0)
				{
					m_pFaxChannel[nFaxChnl].m_nCurrPage += 1;
					pChnlObj->m_nStep = 3;
				}
				else
				{
					//发送失败
					pChnlObj->m_nStep = 6;
				}
			}
			else
			{
				strMSG.Format("%s: ChIndex = %d not get  Fax channel resource  error ! %s ",  pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
		}
		break;
	case 3:
		{
			//检查发送
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				int nCheckReturn = DJFax_CheckTransmit(nFaxChnl);
				if( 1 == nCheckReturn )
				{
					//所有的发送或接收正确结束
					pChnlObj->m_nStep = 4;
				}
				else if( 2 == nCheckReturn )
				{
					//某一页结束
					m_pFaxChannel[nFaxChnl].m_nCurrPage += 1;
					m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl ,&(m_pFaxChannel[nFaxChnl]) );
				}
				else if( 0 == nCheckReturn )
				{
					//正在发送或接收
					return;
				}
				else
				{
					//错误结束或读文件失败
					pChnlObj->m_nStep = 5;
				}
			}
			else
			{
				strMSG.Format("%s: ChIndex = %d not get  Fax channel resource  error . %s",  pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

		}
		break;
	case 4:
		{
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				DJFax_StopFax(nFaxChnl);
				DJFax_ClearLink(nFaxChnl,nIndex);

				pChnlObj->m_nFaxChnl	= -1;
				m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_FREE;

				m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
				m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
			}
			else
			{
				strMSG.Format("%s: ChIndex = %d not get  Fax channel resource  error ;%s",  pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
		}
		break;
	case 5:
		{
			//错误结束
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				pChnlObj->m_nFaxChnl	= -1;
				m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_FREE;

				DJFax_StopFax(nFaxChnl);
				DJFax_ClearLink(nFaxChnl,nIndex);
				strMSG.Format("%s: ChIndex = %d    RcvFaxFile error ;%s",  pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}
			else
			{
				strMSG.Format("%s: ChIndex = %d not get  Fax channel resource  error; %s ",  pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

		}

	default:
		break;
	}

	return;
}

void CDJTc08a32::TC08_Cus_ReciveFax(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	
	if( 1 != m_nFax )
	{
		strMSG.Format("Custom Recive Fax error : ChIndex = %d   Error = The system not support FAx ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}


	int nStep = pChnlObj->m_nStep;
	switch( nStep )
	{
	case 1:
		{

			CString strFunStr = pObj->m_strFunName; 
			if( "" == strFunStr  )
			{
				strMSG.Format("Custom Recive Fax  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;  
			}
			
			CString str = strFunStr;
			str.Replace("ReciveFax",NULL);
			CString strFile("");
			if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strFile ) )
			{
				strMSG.Format("%s  Custom Recive Fax is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}


			
			int nFaxChnl = DJFax_GetOneFreeFaxChnl();
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				m_pFaxChannel[nFaxChnl].m_strFaxFileName = strFile;
				
			
				if(!DJFax_SetLink(nFaxChnl,nIndex))
				{
					strMSG.Format("%s: ChIndex = %d  Custom   Fax SetLink error ;%s",  pObj->m_strCaption, nIndex,pObj->m_strFunName );
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				DJFax_SetLocalID (nFaxChnl, "1234567890ABCDEF1234");
				pChnlObj->m_nFaxChnl	= nFaxChnl;
				m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_RECEIVE;
				m_pFaxChannel[nFaxChnl].m_nCurrPage	=0;
				m_pFaxChannel[nFaxChnl].m_nTotalPage	=0;
				
				pChnlObj->m_strReciveFaxFile = strFile;
				m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl , &(m_pFaxChannel[nFaxChnl]) );
				pChnlObj->m_nStep = 2;
			}
			else
			{
				strMSG.Format("Custom FaxRecive Error; ChIndex = %d   Error = not get free fax channel resource %s", nIndex,pObj->m_strFunName  );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;  
			}
		}
		break;
	case 2:
		{
			
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				if(DJFax_RcvFaxFile( nFaxChnl, (LPTSTR)(LPCTSTR)m_pFaxChannel[nFaxChnl].m_strFaxFileName ) == 1)
				{
					m_pFaxChannel[nFaxChnl].m_nCurrPage += 1;
					m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
					pChnlObj->m_nStep = 3;
				}
				else
				{
					pChnlObj->m_nStep = 5;
					
				}
			}
			else
			{
				strMSG.Format("Custom FaxRecive Error; ChIndex = %d   Error = not get  fax channel resource %s ", nIndex,pObj->m_strFunName  );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;  
			}
		}
		break;
	case 3:
		{
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				int nCheckReturn = DJFax_CheckTransmit(nFaxChnl);
				if( 1 == nCheckReturn )
				{
					//所有的发送或接收正确结束
					pChnlObj->m_nStep = 4;
				}
				else if( 2 == nCheckReturn )
				{
					//某一页结束
					m_pFaxChannel[nFaxChnl].m_nCurrPage += 1;
					m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
				}
				else if( 0 == nCheckReturn )
				{
					//正在发送或接收
					return;
				}
				else
				{
					//错误结束或读文件失败
					pChnlObj->m_nStep = 5;
				}
			}
			else
			{
				strMSG.Format("Custom FaxRecive Error; ChIndex = %d   Error = not get  fax channel resource ;%s", nIndex,pObj->m_strFunName  );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;  
			}

		}
		break;
	case 4:
		{
			//所有的发送或接收正确结束
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				DJFax_StopFax(nFaxChnl);
				DJFax_ClearLink(nFaxChnl,nIndex);

				pChnlObj->m_nFaxChnl	= -1;
				m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_FREE;

				m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
				m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
				return;
			}
			else
			{
				strMSG.Format("Custom FaxRecive Error; ChIndex = %d   Error = not get  fax channel resource %s", nIndex,pObj->m_strFunName  );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ; 
			}
		}
		break;
	case 5:
		{
			//错误结束
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nIndex);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				pChnlObj->m_nFaxChnl	= -1;
				m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_FREE;

				DJFax_StopFax(nFaxChnl);
				DJFax_ClearLink(nFaxChnl,nIndex);
				strMSG.Format("%s: ChIndex = %d  Custom   RcvFaxFile error ;%s",  pObj->m_strCaption, nIndex,pObj->m_strFunName );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			else
			{
				strMSG.Format("Custom FaxRecive Error; ChIndex = %d   Error = not get  fax channel resource %s ", nIndex,pObj->m_strFunName  );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ; 
			}
		}
		break;
	default:
		break;

	}

	return;
}

void CDJTc08a32::TC08_Cus_OutCall(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom OutCall Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}

				char *pszTel = (LPTSTR)(LPCTSTR)strTel;
				if( !TC08_MyMakeCall( nIndex, pszTel,  (LPTSTR)(LPCTSTR)pChnlInfo->m_strLocalDN ) )
				{
					strMSG.Format("Custom OutCall Error; ChIndex = %d it is error that to outcall! %s", nIndex,pObj->m_strFunName );
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return ;   	
				}
				else
				{
					pChnlInfo->m_nStep = 2;
				}
			}
			break;
		case 2:
			{
				int nCallResult = TC08_MyMakeCall_Check( nIndex );
				if( 3 == nCallResult )//未出结果
				{
					return;
				}
				else if( 0 == nCallResult )//成功
				{
					m_pPubCObj->PC_Cus_MoveNextFun( pChnlInfo, pObj);
					return;
				}
				else//超时或失败
				{
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
			}
			break;
		default:
			break;
	}


	
	return;
}
void CDJTc08a32::TC08_Cus_OffHook(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom OffHook Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}

	CString str = pObj->m_strFunName;
	str.Replace("OffHook",NULL);
	CString strAmiChnl = "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlInfo,str,strAmiChnl) )
	{
		strMSG.Format("%s  Custom OffHook is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	int nAimChnl = atoi( strAmiChnl );
	TC08_MyOFFHook( nAimChnl);

	m_pPubCObj->PC_Cus_MoveNextFun( pChnlInfo, pObj);
}
void CDJTc08a32::TC08_Cus_ReciveDtmf(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom ReciveDtmf Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
			if( TC08_GetDtmfCode( nIndex, strMSG ) )
			{
				StartPlaySignal( nIndex,SIG_STOP);
				if( 0 < pChnlInfo->m_nDtmfLen  )
				{
					//通过判断接收按键的最大数量来结束收键
					pChnlInfo->m_strDTMFRecived += strMSG;
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
					if( strMSG == pChnlInfo->m_strDtmfEnd )
					{
						m_pPubCObj->PC_SaveValuesToVar( nIndex, pChnlInfo, pChnlInfo->m_strDTMFRecived, pChnlInfo->m_nSaveDtmfKey );
						m_pPubCObj->PC_Cus_MoveNextFun( pChnlInfo, pObj);
						return;
					}
					else
					{
						pChnlInfo->m_strDTMFRecived += strMSG;

					}
				}
				pChnlInfo->m_nEventType = EVENTTYPE_RECDTMF;
				
			}
			long n = m_pPubCObj->PC_ElapseTime(pChnlInfo);
			if( n >= pChnlInfo->m_nTimeOut )
			{
				m_pPubCObj->PC_SaveValuesToVar( nIndex, pChnlInfo, pChnlInfo->m_strDTMFRecived, pChnlInfo->m_nSaveDtmfKey );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
void CDJTc08a32::TC08_Cus_LeaveConfer(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom StartRecordFile Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	
	int nStep = pChnlObj->m_nStep;
	switch(nStep)
	{
	case 1:
		{

			CString str = pObj->m_strFunName;
			str.Replace("LeaveConfer",NULL);
			CString strAmiChnl = "";
			CString strConfNo	= "";
			if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strAmiChnl,strConfNo ) )
			{
				strMSG.Format("%s  Custom LeaveConfer is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}


			int nAmiIndex = atoi(strAmiChnl);
			if(  nAmiIndex < 0 )
			{
				nAmiIndex = nIndex;
			}
			int nAmiConfNo = atoi(strConfNo);

			CString strFileName = m_strPath+"\\SysVoiceFle\\Confexit.pcm";
			if( TC08_MyStartPlayFile( nAmiIndex, strFileName ) )
			{
				TC08_MyConferLeave(nAmiIndex,nAmiConfNo);
				m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
			}
			else
			{
				TC08_MyConferLeave(nAmiIndex,nAmiConfNo);
				m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
			}
			return;
		}
		break;
	default:
		break;
	}

	m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);

	return;
}
void CDJTc08a32::TC08_Cus_EnterConfer(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom EnterConfer Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	
	int nStep = pChnlObj->m_nStep;
	switch(nStep)
	{
	case 1:
		{

			CString str = pObj->m_strFunName;
			str.Replace("EnterConfer",NULL);
			CString strAmiChnl = "";
			CString strConfNo	= "";
			if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strAmiChnl,strConfNo ) )
			{
				strMSG.Format("%s  Custom EnterConfer is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}


			int nAmiIndex = atoi(strAmiChnl);
			if(  nAmiIndex < 0 )
			{
				nAmiIndex = nIndex;
			}

			int nAmiConfNo = atoi(strConfNo);
			if( TC08_MyConferEnter( nAmiIndex,  nAmiConfNo , CF_CHSWITCH_CONNECT ) )
			{
				CString strFileName = m_strPath+"\\SysVoiceFle\\confadd.pcm";
				if( TC08_MyStartPlayFile( nAmiIndex, strFileName,FALSE,1) )
				{
					m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
					return;
				}
				else
				{
					strMSG.Format("%s Custom EnterConfer  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
					TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
			}
			else
			{
				strMSG.Format("%s Custom EnterConfer  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			return;
		}
		break;
	default:
		break;
	}

	return;


}
void CDJTc08a32::TC08_Cus_GetAgentCall(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom GetAgentCall Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	
	
	CString str = pObj->m_strFunName;
	str.Replace("GetAgentCall",NULL);
	CString strAgentDN("");
	if( !m_pPubCObj->PC_GetParameterValues( pChnlInfo,str,strAgentDN ) )
	{
		strMSG.Format("%s  Custom OutCall is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}



	if( !TC08_Cus_GetAgentCall( nIndex, pChnlInfo, strAgentDN ) )
	{
		strMSG.Format("Custom GetAgentCall Error; ChIndex = %d it is error that to GetAgentCall! %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   	
	}


	m_pPubCObj->PC_Cus_MoveNextFun( pChnlInfo, pObj);

}


void CDJTc08a32::TC08_Cus_StartRecordFile(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom StartRecordFile Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	
	switch( pChnlObj->m_nStep )
	{
	case 1:
		{
			CString str = pObj->m_strFunName;
			str.Replace("StartRecordFile",NULL);
			CString strAmiChnl = "";
			CString strFile	= "";
			CString strLen = "";
			CString strType = "";
			CString strLoop = "";
			if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strAmiChnl,strFile,strLen,strType,strLoop ) )
			{
				strMSG.Format("%s Custom StartRecordFile Error is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}


			int nAmiIndex = atoi(strAmiChnl);
			if(  nAmiIndex < 0 )
			{
				nAmiIndex = nIndex;
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
			
			if( !TC08_MyStartRecordFile(nIndex, strFile, bLoop, nType,nLen ) )//PACK_64KBPS:
			{
				strMSG.Format("Custom StartRecordFile Error; ChIndex = %d   Error = Start Record error File=%s", nIndex,strFile );
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;   	
			}
			else
			{
				if( bLoop )
				{
					m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
				}
				else
				{
					pChnlObj->m_nStep = 2;
				}
			}
		}
		break;
	case 2:
		{
			if( TC08_MyCheckRecordEnd( nIndex ) )
			{
				TC08_MyStopRecordFile( nIndex );
				m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
			}
		}
	}
	

	return;
}
void CDJTc08a32::TC08_Cus_StartPlayFile(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj)
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom StartPlayFile  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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

			if( !TC08_MyStartPlayFile( nIndex, strFile,bLoop,nType) )//false
			{
				strMSG.Format("%s Custom StartPlayFile Error is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
				m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
				TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
	case 2:
		{
			if( TC08_MyCheckPlayEnd( nIndex ) )
			{
				TC08_MyStopPlay( nIndex );
				m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
			}
		}
		break;
	default:
		break;
	}
	

	

	return;
}

BOOL CDJTc08a32::TC08_Cus_GetAgentCall(int nIndex, CFChannelInfo *pChnlInfo, CString strDN)
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
		TC08_ResetUserChannel( m );
		pChnlInfo->m_nLinkCh = n;
		m_pChannelInfo[n].m_nLinkCh = nIndex;
		TC08_MySetLink( nIndex, n , CF_CHSWITCH_CONNECT);

		return TRUE;
	}
	else
	{
		return FALSE;
	}


}


void CDJTc08a32::TC08_Cus_StopPlayFile( int nIndex,  CFChannelInfo *pChnlObj,  CCustomObj *pObj )
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom StopPlayFile Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	

	CString str = pObj->m_strFunName;
	str.Replace("StopPlayFile",NULL);
	CString strAmiChnl = "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strAmiChnl) )
	{
		strMSG.Format("%s Custom StopPlayFile Error is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	
	int nAmiIndex = atoi( strAmiChnl );
	
	TC08_MyStopPlay( nAmiIndex  );
	m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);


	return;
}

void CDJTc08a32::TC08_Cus_StopRecordFile( int nIndex,  CFChannelInfo *pChnlObj,  CCustomObj *pObj )
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom StopRecordFile Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	

	CString str = pObj->m_strFunName;
	str.Replace("StopRecordFile",NULL);
	CString strAmiChnl = "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strAmiChnl) )
	{
		strMSG.Format("%s Custom StopRecordFile Error is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	
	int nAmiIndex = atoi( strAmiChnl );
	TC08_MyStopRecordFile( nAmiIndex  );
	m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);

	return;

}

void CDJTc08a32::TC08_Cus_HangUp( int nIndex,  CFChannelInfo *pChnlObj,  CCustomObj *pObj )
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom HangUp Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	

	CString str = pObj->m_strFunName;
	str.Replace("HangUp",NULL);
	CString strAmiChnl = "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strAmiChnl) )
	{
		strMSG.Format("%s  Custom HangUp is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	int nAimChnl = atoi( strAmiChnl );
	TC08_ReleaseCall( nAimChnl  );
	m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
	
	return;
}

void CDJTc08a32::TC08_Cus_ClearDtmf( int nIndex,  CFChannelInfo *pChnlObj,  CCustomObj *pObj )
{
	CString strMSG("");
	if( "" == pObj->m_strFunName  )
	{
		strMSG.Format("Custom ClearDtmf Error  error : ChIndex = %d %s", nIndex,pObj->m_strFunName );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	

	CString str = pObj->m_strFunName;
	str.Replace("ClearDtmf",NULL);
	CString strAmiChnl = "";
	if( !m_pPubCObj->PC_GetParameterValues( pChnlObj,str,strAmiChnl) )
	{
		strMSG.Format("%s  Custom ClearDtmf is error ;ChIndex = %d %s", pObj->m_strCaption, nIndex,pObj->m_strFunName);
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		TC08_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	int nAimChnl = atoi( strAmiChnl );
	TC08_MyInitDtmfBuf( nAimChnl  );
	m_pPubCObj->PC_Cus_MoveNextFun( pChnlObj, pObj);
	

	return;
}



BOOL CDJTc08a32::TC08_MyCheckPlayEnd(int nIndex)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return TRUE;
	}
	return CheckPlayEnd( nIndex );
}

BOOL CDJTc08a32::TC08_MyCheckRecordEnd(int nIndex)
{
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ){
		return TRUE;
	}
	return CheckRecordEnd( nIndex );
}
