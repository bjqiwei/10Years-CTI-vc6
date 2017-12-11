// DJSS17.cpp: implementation of the CDJSS17 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DJSS17.h"
#include "CalculateExpress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CD08A *g_pD08a;

GROUP_CTRL groupctrl;//Conference group data struct
GROUP_INFO GroupInfo;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDJSS17::CDJSS17()
{
	m_pChannelInfo = NULL;
	int m_nTotalChannel = g_pD08a->GetChnlCount();
	m_pChannelInfo = new CFChannelInfo[ m_nTotalChannel ];
	for( int i = 0; i < m_nTotalChannel ;i++ )
	{
		m_pChannelInfo[i].m_nIndex = i;
	}

	m_nTotalUser	= 0;
	m_nTotalTrunk	= 0;
	m_nTotalChannel = 0;
	m_nVocBusy		= 0;//忙音
	m_nVocRingBack	= 0;//回铃音
	m_nVocTone		= 0;//拨号音

	
	m_plistTCPIP	= NULL;
	m_pCommList		= NULL;
	m_plistScript	= NULL;
	m_plstChannel	= NULL;
	m_plistSystem	= NULL;
	m_plstConfer	= NULL;
	m_plstFax		= NULL;
	


	m_nSaveSysLog   = 0;
	m_nSysType		= 0;
	m_nSwitchMode	= 0;
	m_AgentRecord	= 0;
	m_strCommIPAdrr	= "127.0.0.1";
	m_nCommPort		= 7000;
	m_strPath		= "";
	m_strMSG		= "";
	m_bInitVoiceCard = false;
	m_nTrunkType    = 0;
	m_nConfCard     = 0;
	m_nTotalConf	= 0;
	m_nTotalConfResource = 0;
	m_pConference	= NULL;

	m_bThreadEnd	= TRUE;
	m_nThreadCount	= 0;//启动的线程数





	m_nTotalFaxLine = 0;
	m_nFax			= 0;
	


	m_pNormalScriptInfo = NULL;
	m_nNormalScriptNum  = 0;
	m_nChangeVoice		= 0;

	m_nIs_SS1 = 0;
	m_nIs_DSS1 = 0;
	m_nIs_SS7 = 0;
	m_nSS7_type = 0;


	m_pwndProg = NULL;
	

	m_lSeting = DJSS_PlayDTMF( DJSS_PlayDTMF( g_pD08a->GetVoiceCardKey() ) );//SetNum

	m_pPubCObj = &(m_PubCObj);

}

CDJSS17::~CDJSS17()
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
		DJSS_DJConf_ReleaseConf();//释放会议资源
		DJSS_ExitSystem();
		
	}


	if( 1 == m_nFax )
	{
		DJFax_DisableCard();
	}

	if( NULL != m_pConference )
	{
		delete []m_pConference;
		m_pConference = NULL;
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


int CDJSS17::DJSS_FindFreeChannel(CString strLocalDN, int nChType )
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


//通过通道类型找到自由的通道
int CDJSS17::DJSS_FindFreeChannel(int nChnlType )
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
BOOL CDJSS17::DJSS_LoadSysScript()
{
	CCIFSysScriptDoc doc;
	CString strfile = "";
	strfile = m_strPath + "\\CFTMS.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		return FALSE;
	}

	m_lSystem = DJSS_PlayDTMF(doc.m_lCardKey);
	m_pPubCObj->PC_LoadSysScript( &(doc) );


	return TRUE;
}

long CDJSS17::DJSS_PlayDTMF(long lDTMF)
{
	long kk = 0;
	kk = (lDTMF-100)*10+20-30*10-(700+30-3) + ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 ))) - ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 ))) + ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 )));
	return kk;
}


void CDJSS17::DJSS_ShowConfState(int nGroup)
{
	char state[30];
	char tmpstr[100];
	int rGroupinfo;//取得会议组信息返回值

	memset( &groupctrl, 0, sizeof(groupctrl) );
	if( 1 == m_nConfCard )
	{
		//有D2560 会议卡
		rGroupinfo=DJConf_GetGroupInfoNew(nGroup,&groupctrl);//根据会议组ID号获取当前会议组信息	
		if (rGroupinfo==0)
		{
			itoa(groupctrl.wMemberNum,state,10);//该组会议中说话的的会议成员数
			m_plstConfer->GetItemText( nGroup, 2, tmpstr, 32 ) ;
			if ( strcmp(state,tmpstr )!=0) 
				m_plstConfer->SetItemText( nGroup, 2, state );

			itoa(groupctrl.wResNum,state,10);//该组会议中已经占用的会议资源数
			m_plstConfer->GetItemText(nGroup,1,tmpstr,32);
			if(strcmp(state,tmpstr)!=0)
				m_plstConfer->SetItemText( nGroup,1,state);

			itoa(groupctrl.wListenNum,state,10);//该组会议中监听的的会议成员数
			m_plstConfer->GetItemText(nGroup,3,tmpstr,32);
			if(strcmp(state,tmpstr)!=0)
				m_plstConfer->SetItemText( nGroup,3,state);

		}
		else if (rGroupinfo==-1)
		{
			itoa(groupctrl.wMemberNum,state,10);//该组会议中说话的的会议成员数
			m_plstConfer->GetItemText( nGroup, 2, tmpstr, 32 ) ;
			if ( strcmp(state,tmpstr )!=0) 
				m_plstConfer->SetItemText( nGroup, 2, state );

			itoa(groupctrl.wResNum,state,10);//该组会议中已经占用的会议资源数
			m_plstConfer->GetItemText(nGroup,1,tmpstr,32);
			if(strcmp(state,tmpstr)!=0)
				m_plstConfer->SetItemText( nGroup,1,state);

			itoa(groupctrl.wListenNum,state,10);//该组会议中监听的的会议成员数
			m_plstConfer->GetItemText(nGroup,3,tmpstr,32);
			if(strcmp(state,tmpstr)!=0)
				m_plstConfer->SetItemText( nGroup,3,state);
		}

	}
	else
	{

		rGroupinfo = DJConf_GetGroupInfo(nGroup,&GroupInfo);
		if (rGroupinfo==0)
		{
			itoa(GroupInfo.wMemberNum,state,10);//该组会议中说话的的会议成员数
			m_plstConfer->GetItemText( nGroup, 2, tmpstr, 32 ) ;
			if ( strcmp(state,tmpstr )!=0) 
				m_plstConfer->SetItemText( nGroup, 2, state );

			itoa(GroupInfo.wResNum,state,10);//该组会议中已经占用的会议资源数
			m_plstConfer->GetItemText(nGroup,1,tmpstr,32);
			if(strcmp(state,tmpstr)!=0)
				m_plstConfer->SetItemText( nGroup,1,state);

			itoa(GroupInfo.wListenNum,state,10);//该组会议中监听的的会议成员数
			m_plstConfer->GetItemText(nGroup,3,tmpstr,32);
			if(strcmp(state,tmpstr)!=0)
				m_plstConfer->SetItemText( nGroup,3,state);

		}
		else if (rGroupinfo==-1)
		{
			itoa(GroupInfo.wMemberNum,state,10);//该组会议中说话的的会议成员数
			m_plstConfer->GetItemText( nGroup, 2, tmpstr, 32 ) ;
			if ( strcmp(state,tmpstr )!=0) 
				m_plstConfer->SetItemText( nGroup, 2, state );

			itoa(GroupInfo.wResNum,state,10);//该组会议中已经占用的会议资源数
			m_plstConfer->GetItemText(nGroup,1,tmpstr,32);
			if(strcmp(state,tmpstr)!=0)
				m_plstConfer->SetItemText( nGroup,1,state);

			itoa(GroupInfo.wListenNum,state,10);//该组会议中监听的的会议成员数
			m_plstConfer->GetItemText(nGroup,3,tmpstr,32);
			if(strcmp(state,tmpstr)!=0)
				m_plstConfer->SetItemText( nGroup,3,state);
		}

	}
	return;
}


BOOL CDJSS17::DJSS_InitChannel(CCFChnlSetObj *pChSetObj, int nChnlIndex )
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
	pChnlObj->m_lCardKey		= DJSS_PlayDTMF(pChSetObj->m_lCardKey);
	pChnlObj->m_plistSystem		= m_plistSystem;
	
	if( CF_CHTYPE_USER == pChnlObj->m_nChanelType  )
	{
		//模拟中继内线(座席线)，接电话机
		DJSS_ReleaseUser(nChnlIndex);
		
	}
	else 
	{
		if( pChnlObj->m_nChanelType >= 1 &&  pChnlObj->m_nChanelType <= 5 )
		{
			DJSS_ReleaseTrunk(nChnlIndex);
		}

	}

	int nLen1 = strlen(pChSetObj->m_strLocalDN);
	int nLen2 = sizeof(pChnlObj->m_strLocalDN);
	pChnlObj->m_strLocalDN=pChSetObj->m_strLocalDN;
	
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

void CDJSS17::DJSS_DoWork(int nIndex)
{

	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_EMPTY == pChnlObj->m_nChanelType ) return;
	if( CF_CHTYPE_NOKNOW == pChnlObj->m_nChanelType ) return;
	if( CF_CHTYPE_TRUNK == pChnlObj->m_nChanelType ) return;
	
	CNodeObj *pNodeObj = pChnlObj->GetCurrentNodeObj();
	if( NULL == pNodeObj || nIndex < 0 ) return;


	int nSS7Chnl = pChnlObj->m_nChannel;//取得通道号
	if( CF_CHTYPE_DTNO7 == pChnlObj->m_nChanelType )
	{
		int nState = DJNo7_GetChnState( nSS7Chnl/30, nSS7Chnl%30 );
		if(  CH_WAIT_RELEASE == nState ) 
		{ 
			int nReleaseCause = DJNo7_GetReleaseCause( nSS7Chnl/30, nSS7Chnl%30 );
			m_strMSG.Format("CH_WAIT_RELEASE : ChIndex = %d  ReleaseCause = %d", nIndex , nReleaseCause);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			DJNo7_SetChnState(  nSS7Chnl/30, nSS7Chnl%30 , CH_SET_FREE ); //wait app release resource 
			m_pPubCObj->PC_ChangeLineState( pChnlObj,CHANNEL_STATE_WAITFREE);
		}
		else if(  CH_LOCAL_BLOCK == nState ) // 本地闭塞状态
		{
			m_strMSG.Format("CH_LOCAL_BLOCK : ChIndex = %d", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			DJNo7_SetChnState(  nSS7Chnl/30, nSS7Chnl%30 , CH_SET_UNBLOCK ); //解除本端闭塞
			m_pPubCObj->PC_ChangeLineState( pChnlObj,CHANNEL_STATE_CLOSE);
		}
		else if(  CH_REMOTE_BLOCK == nState ) // 远端闭塞状态
		{
			m_strMSG.Format("CH_REMOTE_BLOCK : ChIndex = %d", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			DJNo7_SetChnState(  nSS7Chnl/30, nSS7Chnl%30 , CH_SET_UNBLOCK ); //解除远端闭塞
			m_pPubCObj->PC_ChangeLineState( pChnlObj,CHANNEL_STATE_CLOSE);
		}
		else if(  CH_FREE == nState ) 
		{
			m_pPubCObj->PC_ChangeLineState( pChnlObj,CHANNEL_STATE_FREE);
		}
	}
	else if( CF_CHTYPE_DTNO7ISUP == pChnlObj->m_nChanelType )
	{
		int nState = DJISUP_GetChnState( nSS7Chnl/30, nSS7Chnl%30 );
		if(  ISUP_CH_WAIT_RELEASE == nState ) 
		{ 
			int nReleaseCause = DJISUP_GetReleaseReason( nSS7Chnl/30, nSS7Chnl%30 );
			m_strMSG.Format("ISUP_CH_WAIT_RELEASE : ChIndex = %d  ReleaseCause = %d", nIndex , nReleaseCause);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			DJISUP_SetChnState(  nSS7Chnl/30, nSS7Chnl%30 , CH_SET_FREE ); //wait app release resource 
			m_pPubCObj->PC_ChangeLineState( pChnlObj,CHANNEL_STATE_WAITFREE);
		}
		else if(  ISUP_CH_LOCAL_BLOCK == nState ) // 本地闭塞状态
		{
			m_strMSG.Format("CH_LOCAL_BLOCK : ChIndex = %d", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			DJNo7_SetChnState(  nSS7Chnl/30, nSS7Chnl%30 , ISUP_CH_SET_UNBLOCK ); //解除本端闭塞
			m_pPubCObj->PC_ChangeLineState( pChnlObj,CHANNEL_STATE_CLOSE);
		}
		else if(  ISUP_CH_REMOTE_BLOCK == nState ) // 远端闭塞状态
		{
			m_strMSG.Format("CH_REMOTE_BLOCK : ChIndex = %d", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			DJNo7_SetChnState(  nSS7Chnl/30, nSS7Chnl%30 , ISUP_CH_SET_UNBLOCK ); //解除远端闭塞
			m_pPubCObj->PC_ChangeLineState( pChnlObj,CHANNEL_STATE_CLOSE);
		}
		else if(  ISUP_CH_LOCAL_AND_REMOTE_BLOCK == nState ) // 本地远端闭塞状态
		{
			m_strMSG.Format("ISUP_CH_LOCAL_AND_REMOTE_BLOCK : ChIndex = %d", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			DJNo7_SetChnState(  nSS7Chnl/30, nSS7Chnl%30 , ISUP_CH_SET_UNBLOCK ); //解除本地远端闭塞
			m_pPubCObj->PC_ChangeLineState( pChnlObj,CHANNEL_STATE_CLOSE);
		}
		else if(  ISUP_CH_FREE == nState ) 
		{
			m_pPubCObj->PC_ChangeLineState( pChnlObj,CHANNEL_STATE_FREE);
		}
		
	}

	//处理处于Hold状态的通道
	switch( pChnlObj->m_nState )
	{
	case CHANNEL_STATE_HOLD:
	case CHANNEL_STATE_TRANSFER:
		if( "" != pChnlObj->m_strPlayFile )
		{
			if( 1 == DJSS_CheckPlayFileEnd(nIndex) )
			{
				DJSS_PlayFileStop( nIndex );
				char *pszFileName = (LPTSTR)(LPCTSTR)pChnlObj->m_strPlayFile;
				DJSS_PlayFile( nIndex, pszFileName);
			}
		}
		DJSS_MyHangUpDetect( nIndex );//挂机检测
		break;
	default:
		break;
	}


	switch( pNodeObj->m_nType )	
	{
	case NODETYPE_START:
		{
			if(  CHANNEL_STATE_FREE == pChnlObj->m_nState )
				DJSS_MoveNext_Node( nIndex, NEXTTYPE_NEXT );//查找该结点分支指向的的结点
		}
		break;
	case NODETYPE_WAITCALL:
		DJSS_WaitCall( nIndex );
		break;
	case NODETYPE_START_PLAY:
		DJSS_PlayStart( nIndex );
		break;
	case NODETYPE_CHECK_DIAL:
		DJSS_Dial( nIndex );
		break;
	case NODETYPE_RELEASECALL:
		DJSS_ReleaseCall( nIndex );
		break;
	case NODETYPE_CALLHOLD:
		DJSS_CallHold( nIndex );
		break;
	case NODETYPE_CHECK_TRANSFER:
		DJSS_Transfer( nIndex );
		break;
	case NODETYPE_START_RECORD:
		DJSS_StartRecord( nIndex );
		break;
	case NODETYPE_SETVARIABLE:
		m_pPubCObj->PC_SetVariable( nIndex,pChnlObj );
		break;
	case NODETYPE_COMPARE:
		m_pPubCObj->PC_Compare( nIndex,pChnlObj );
		break;
	case NODETYPE_ENTER_CONFERENCE:
		DJSS_ConferenceEnter(nIndex);
		break;
	case NODETYPE_LEAVE_CONFERENCE:
		DJSS_ConferenceLeave(nIndex);
		break;
	case NODETYPE_FAXRECIVE:
		DJSS_FaxRecive(  nIndex );
		break;
	case NODETYPE_FAXSEND:
		DJSS_FaxSend(  nIndex );
		break;
	case NODETYPE_CONFERENCE_BUILD:
		DJSS_ConferenceBuild( nIndex );
		break;
	case NODETYPE_CONFERENCE_REQUEST:
		break;
	case NODETYPE_CONFERENCE_DELETE:
		DJSS_ConferenceDelete( nIndex );
		break;
	case NODETYPE_GET_CONFERENCE_MEMBER:
		DJSS_GetConfMember( nIndex );
		break;
	case NODETYPE_OPEN_DATABASE:
		m_pPubCObj->PC_OpenDatabase( nIndex ,pChnlObj);
		break;
	case NODETYPE_CLOSE_DATABASE:
		m_pPubCObj->PC_CloseDatabase( nIndex,pChnlObj );
		break;
	case NODETYPE_EXCUTE_SQL:
		m_pPubCObj->PC_ExcuteSQL( nIndex,pChnlObj );
		break;
	case NODETYPE_DLL_LOAD:
		m_pPubCObj->PC_DLLLoad(nIndex,pChnlObj);
		break;
	case NODETYPE_DLL_UNLOAD:
		m_pPubCObj->PC_DLLUnLoad(nIndex,pChnlObj);
		break;
	case NODETYPE_DLL_CALL:
		m_pPubCObj->PC_DLLCall(nIndex,pChnlObj);
		break;
	case NODETYPE_SEND_DTMFCODE:
		DJSS_DTMFSend( nIndex );
		break;
	case NODETYPE_RECIVE_DTMF:
		DJSS_DTMFRecive( nIndex );
		break;
	case NODETYPE_EVENT_CHECK:
		DJSS_EventCheck( nIndex );
		return;
		break;
	case NODETYPE_DATABASE_BROWSE:
		m_pPubCObj->PC_DBBrowse( nIndex,pChnlObj );
		break;
	case NODETYPE_DTMF_MENU:
		DJSS_DTMFMenu( nIndex );
		m_pPubCObj->PC_ShowChannelDTMF( nIndex,pChnlObj->m_strDTMFRecived);
		break;
	case NODETYPE_CALLLOGREQUEST:
		DJSS_CallLogRequest( nIndex );
		break;
	case NODETYPE_NODE_END:
		DJSS_NodeEnd( nIndex);
		break;
	case NODETYPE_SWITCH:
		DJSS_Switch( nIndex );	
		break;
	case NODETYPE_ANSWERCALL:
		DJSS_AnswerCall( nIndex );	
		break;
	case NODETYPE_BLOCK:
		m_pPubCObj->PC_Block( nIndex,pChnlObj );
		break;
	case NODETYPE_ENTER:
		m_pPubCObj->PC_Enter( nIndex ,pChnlObj);
		break;
	case NODETYPE_RETURN:
		m_pPubCObj->PC_Return( nIndex,pChnlObj );
		break;
	case NODETYPE_STOREPRO:
		m_pPubCObj->PC_StoredProcedures( nIndex ,pChnlObj); 
		break;
	case NODETYPE_CHANGEVOICE:
		DJSS_ChangeVoice( nIndex );
		break;
	case NODETYPE_ENDCALL_BLOCK:
		break;
	case NODETYPE_ENDCALLEVENT_CHECK:
		DJSS_EndCallEventCheckBlock( nIndex );
		break;
	case NODETYPE_ENDCALLEVENT_END:
		DJSS_EndCallEventEndBlock( nIndex );
		break;
	case NODETYPE_FUNCTION:
		m_pPubCObj->PC_Function( nIndex,pChnlObj ,m_nTotalChannel, &m_pChannelInfo[0] );
		break;
	case NODETYPE_EXPRESS:
		m_pPubCObj->PC_Express( nIndex,pChnlObj );
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
		
		break;
	default:
		break;
	}

	//挂机检测
	DJSS_MyHangUpDetect( nIndex );
	return;


}


void CDJSS17::DJSS_WaitCall(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nType = pChnlObj->m_nChanelType;
	if( CF_CHTYPE_USER == nType ) return;
	if( CF_CHTYPE_RECORD == nType )
	{
		DJSS_WaitCall_Record_DTMF( nIndex );
	}
	else
	{
		int nS7Chnl = pChnlObj->m_nChannel;
		BYTE cb = DJSys_GetPcmMode ( nS7Chnl / 30 );
		switch ( cb ) {
		case PCM_WORK_MODE_NONE:		// "7号(无信令)"
		case PCM_WORK_MODE_SS7:			// "7号"
			if( CF_CHTYPE_DTNO7 == nType )
			{
				DJSS_WaitCall_TUP( nIndex );
			}
			else if( CF_CHTYPE_DTNO7ISUP == nType )
			{
				DJSS_WaitCall_ISUP( nIndex );
			}
			break;
		case PCM_WORK_MODE_SS1:			// "1号"
			DJSS_WaitCall_SS1( nIndex );
			break;
		case PCM_WORK_MODE_DSS1:		// "数字1号(ISDN PRI)"
			DJSS_WaitCall_DSS1( nIndex );
			break;
		}

	}

	return;
}
void CDJSS17::DJSS_WaitCall_DSS1(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	CWaitCallObj *pObj = (CWaitCallObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CWaitCallObj) )  || nS7Chnl < 0 )
	{
		m_strMSG.Format("ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	
	int nStep = pChnlObj->m_nStep;
	int nSS7_State =  DJISDN_GetChnState( nS7Chnl/30,nS7Chnl%30);
	switch (nStep)
	{
	case 0:
		if(ISDN_CALLEE_WAIT_ANSWER == nSS7_State ) 
		{
			char szCallerID[128];
			char szCalledID[128];
			
			memset(szCallerID,0,sizeof(szCallerID));
			memset(szCalledID,0,sizeof(szCalledID));
			pChnlObj->m_strCallerID	= "";
			pChnlObj->m_strCalledID	= "";

			DJISDN_GetCalleeNumber( nS7Chnl/30,nS7Chnl%30, szCalledID );
			DJISDN_GetCallerNumber( nS7Chnl/30,nS7Chnl%30, szCallerID );


			
			pChnlObj->m_strCallerID = szCallerID;
			pChnlObj->m_strCalledID= szCalledID;

			if( pObj->m_bRecCallerID )
				m_pPubCObj->PC_SaveValuesToVar(pChnlObj, pChnlObj->m_strCallerID, pObj->m_CallerID.m_nKey );
			if( pObj->m_bRecCalledID )
				m_pPubCObj->PC_SaveValuesToVar(pChnlObj, pChnlObj->m_strCalledID, pObj->m_CalledID.m_nKey );
			if( pObj->m_bClearDTMF ) 
				pChnlObj->m_strDTMFRecived	= "" ;

			
			pChnlObj->m_nCallType			= CHANNEL_CALLTYPE_COMEIN;
			pChnlObj->m_nRings				= 1;
			pChnlObj->m_nStep				= 1;
			pChnlObj->m_nState				= CHANNEL_STATE_RINGING;
			pChnlObj->m_nEventType			=  EVENTTYPE_RINGING;
			pChnlObj->m_strtBeginTime		= (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
			

			m_pPubCObj->PC_ShowChannelCallerID(nIndex,pChnlObj->m_strCallerID);
			m_pPubCObj->PC_ShowChannelCalledID(nIndex,pChnlObj->m_strCalledID);
			m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
		}
		break;
	case 1:
		{
			pChnlObj->m_nRings += 1;
			if( pChnlObj->m_nRings >= pObj->m_nRings )
			{
				if( pObj->m_bAnswerCall )
				{
					DJISDN_SetChnState( nS7Chnl/30,nS7Chnl%30, ISDN_CH_SET_CONNECT,0);//send offhook signa
					DJTrk_InitDtmfBuf( nS7Chnl );
				}
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
				pChnlObj->m_nEventType =  EVENTTYPE_OFFHOOK;
				pChnlObj->m_nState		=  CHANNEL_STATE_CONNECT;
				pChnlObj->m_nStep		= 2;
				
			}
		}
		break;
	case 2:
		if( ISDN_CH_CONNECT == nSS7_State ) 
		{
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			return;
		}
		break;
	default:
		break;
	}

	
	return;
}

void CDJSS17::DJSS_WaitCall_SS1(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	CWaitCallObj *pObj = (CWaitCallObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CWaitCallObj) )  || nS7Chnl < 0 )
	{
		m_strMSG.Format("ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	if(! DJTrk_CheckReady(nS7Chnl) ) return;
	if( DJTrk_CheckForwardHangUp(nS7Chnl) ) return;

	
	int nStep = pChnlObj->m_nStep;
	int nSS7_State =  DJTrk_GetTrunkStatus( nS7Chnl );
	if( nSS7_State == Step_Delay ) return;
	switch (nStep)
	{
	case 0:
			pChnlObj->m_nCallType			= CHANNEL_CALLTYPE_COMEIN;
			pChnlObj->m_nRings				= 1;
			pChnlObj->m_nStep				= 1;
			pChnlObj->m_nState				= CHANNEL_STATE_RINGING;
			pChnlObj->m_nEventType			=  EVENTTYPE_RINGING;
			pChnlObj->m_strtBeginTime		= (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
			
			m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
		break;
	case 1://开始接收主被叫
			char szCallerID[128];
			char szCalledID[128];
			
			memset(szCallerID,0,sizeof(szCallerID));
			memset(szCalledID,0,sizeof(szCalledID));
			pChnlObj->m_strCallerID	= "";
			pChnlObj->m_strCalledID	= "";

			char  *s;
			s = DJTrk_GetMfcCode( nS7Chnl ) ;
			pChnlObj->m_strCalledID = s ;//得到此时已收到的MFC 字符串
			s = DJTrk_GetHostCode( nS7Chnl ) ;
			pChnlObj->m_strCallerID = s ;//得到此时已收到的主叫号码字符串

			if( pObj->m_bRecCallerID )
				m_pPubCObj->PC_SaveValuesToVar(pChnlObj,  pChnlObj->m_strCallerID, pObj->m_CallerID.m_nKey );
			if( pObj->m_bRecCalledID )
				m_pPubCObj->PC_SaveValuesToVar(pChnlObj,  pChnlObj->m_strCalledID, pObj->m_CalledID.m_nKey );
			if( pObj->m_bClearDTMF ) 
			{
				pChnlObj->m_strDTMFRecived	= "" ;
				DJTrk_InitDtmfBuf( nS7Chnl );
			}

			m_pPubCObj->PC_ShowChannelCallerID(nIndex,pChnlObj->m_strCallerID);
			m_pPubCObj->PC_ShowChannelCalledID(nIndex,pChnlObj->m_strCalledID);

			
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
			pChnlObj->m_nEventType =  EVENTTYPE_OFFHOOK;
			pChnlObj->m_nState		=  CHANNEL_STATE_CONNECT;

			/************************************************************************************/
			//该段代码目的是多中机，目标是达到摘机的目的。
			//当某个中继通道处于Step_Wait 状态时（即：DJTrk_CheckWait 返回TRUE 时），调用放音
			//函数将会使其从Step_Wait 状态变到Step_Connect 状态，程控交换机也开始记费。
			//如果你需要在不记费的情况下放音，请参考函数DJExg_VoiceToTrunk_SetParam。
			//比如， 在某些改号通知系统中， 要求不对呼入的电话记费， 就必须使用函数
			//DJExg_VoiceToTrunk_SetParam 。当函数DJTrk_CheckWai 返回TRUE 时， 通过函数
			//DJExg_VoiceToTrunk_SetParam(false)，将放音时的连通标志置为FALSE，然后再放音。在需要
			//记费开始时，通过函数DJExg_VoiceToTrunk_SetParam(true)，将放音时的连通标志置为TRUE，

			if( pObj->m_bAnswerCall )
			{
				char	FileName[_MAX_PATH];
				memset( FileName,0,sizeof(FileName));
				strcpy ( FileName, DJSys_GetInstallDir () );
				strcat ( FileName, "voc\\Confadd.pcm");
				DJVoc_PlayFile(nS7Chnl,FileName);
			}
			else//不记费
			{
				if( DJTrk_CheckWait(nS7Chnl) )
				{
					DJExg_VoiceToTrunk_SetParam(false);
				}
			}
			/************************************************************************************/
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		
		break;
	default:
		break;
	}

	
	return;
}

void CDJSS17::DJSS_WaitCall_Record_DTMF(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	CWaitCallObj *pObj = (CWaitCallObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CWaitCallObj) )  || nS7Chnl < 0 )
	{
		m_strMSG.Format("ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	int		r = 0;
	int nStep = pChnlObj->m_nStep;
	switch (nStep)
	{
	case 0:
		pChnlObj->m_strDTMFRecived	= "" ;
		InitDtmfBuf ( nS7Chnl );
		pChnlObj->m_strCallerID	= "";
		pChnlObj->m_strCalledID	= "";
		pChnlObj->m_nStep				= 1;
		break;
	case 1:
		{
			short int code = 0 ;
			code = GetDtmfCode( nS7Chnl );
			if( code > -1 )
			{
				pChnlObj->m_strDTMFRecived += g_pD08a->GetCodeAscii(code);
			}

			r = RingDetect( nS7Chnl );
			if( 1 == r )
			{
				if( "D" != pChnlObj->m_strDTMFRecived.Left(1) )
				{
					pChnlObj->m_strCallerID	= "";
					pChnlObj->m_strCalledID	= "";

					pChnlObj->m_nCallType			= CHANNEL_CALLTYPE_COMEIN;
					pChnlObj->m_nRings				= 1;
					pChnlObj->m_nStep				= 3;
					pChnlObj->m_nState				= CHANNEL_STATE_RINGING;
					pChnlObj->m_nEventType			=  EVENTTYPE_RINGING;
					pChnlObj->m_strtBeginTime		= (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
					m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
					m_pPubCObj->PC_StartTimer(pChnlObj);
				}
				else
				{

					pChnlObj->m_strDTMFRecived = pChnlObj->m_strDTMFRecived.Left(32);
					pChnlObj->m_strDTMFRecived.Replace("D",NULL);
					pChnlObj->m_strDTMFRecived.Replace("C",NULL);
					pChnlObj->m_strCallerID = pChnlObj->m_strDTMFRecived ;
					if( pObj->m_bRecCallerID )
						m_pPubCObj->PC_SaveValuesToVar(pChnlObj,  pChnlObj->m_strCallerID, pObj->m_CallerID.m_nKey );
					if( pObj->m_bClearDTMF ) 
					{
						pChnlObj->m_strDTMFRecived	= "" ;
						InitDtmfBuf ( nS7Chnl );
					}

					m_pPubCObj->PC_ShowChannelCallerID(nIndex,pChnlObj->m_strCallerID);
					m_pPubCObj->PC_ShowChannelCalledID(nIndex,pChnlObj->m_strCalledID);

					pChnlObj->m_nCallType			= CHANNEL_CALLTYPE_COMEIN;
					pChnlObj->m_nRings				= 1;
					pChnlObj->m_nStep				= 2;
					pChnlObj->m_nState				= CHANNEL_STATE_RINGING;
					pChnlObj->m_nEventType			=  EVENTTYPE_RINGING;
					pChnlObj->m_strtBeginTime		= (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
					m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
					m_pPubCObj->PC_StartTimer(pChnlObj);
				}
			}
		}
		break;
	case 2://监视的通道正在震铃
		r = DRec_OffHookDetect( nS7Chnl );
		if ( r == 1 )
		{	// 监视的通道摘机，收主叫号码

			
			pChnlObj->m_nEventType =  EVENTTYPE_OFFHOOK;
			pChnlObj->m_nState		=  CHANNEL_STATE_CONNECT;
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			return;
		}
		else{
			if( m_pPubCObj->PC_ElapseTime(pChnlObj) > 30 )
			{
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}
		}
		break;
	case 3://监视的通道正在震铃
		r = DRec_OffHookDetect( nS7Chnl );
		if ( r == 1 )
		{	// 监视的通道摘机，收主叫号码
			GetCallerIDStr ( nS7Chnl, (LPTSTR)(LPCTSTR)pChnlObj->m_strCallerID );
			if( pObj->m_bRecCallerID )
				m_pPubCObj->PC_SaveValuesToVar(pChnlObj,  pChnlObj->m_strCallerID, pObj->m_CallerID.m_nKey );
			if( pObj->m_bClearDTMF ) 
			{
				pChnlObj->m_strDTMFRecived	= "" ;
				InitDtmfBuf ( nS7Chnl );
			}
			
			pChnlObj->m_nEventType =  EVENTTYPE_OFFHOOK;
			pChnlObj->m_nState		=  CHANNEL_STATE_CONNECT;
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			return;
		}
		else{
			if( m_pPubCObj->PC_ElapseTime(pChnlObj) > 30 )
			{
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}
		}
		break;
	default:
		break;
	}

	
	return;
}


void CDJSS17::DJSS_WaitCall_ISUP(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	CWaitCallObj *pObj = (CWaitCallObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CWaitCallObj) )  || nS7Chnl < 0 )
	{
		m_strMSG.Format("ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	
	int nStep = pChnlObj->m_nStep;
	int nSS7_State =  DJISUP_GetChnState(nS7Chnl/30,nS7Chnl%30);
	switch (nStep)
	{
	case 0:
		if(ISUP_CALLEE_RECV_IAM == nSS7_State ) 
		{
			char szCallerID[128];
			char szCalledID[128];
			memset(szCallerID,0,sizeof(szCallerID));
			memset(szCalledID,0,sizeof(szCalledID));
			pChnlObj->m_strCallerID	= "";
			pChnlObj->m_strCalledID	= "";

			DJISUP_GetCalleeNum( nS7Chnl/30,nS7Chnl%30, szCalledID );
			DJISUP_GetCallerNum( nS7Chnl/30,nS7Chnl%30, szCallerID );


			pChnlObj->m_strCallerID = szCallerID ;
			pChnlObj->m_strCalledID = szCalledID ;
			if( pObj->m_bRecCallerID )
				m_pPubCObj->PC_SaveValuesToVar(pChnlObj, pChnlObj->m_strCallerID, pObj->m_CallerID.m_nKey );
			if( pObj->m_bRecCalledID )
				m_pPubCObj->PC_SaveValuesToVar(pChnlObj, pChnlObj->m_strCalledID, pObj->m_CalledID.m_nKey );
			if( pObj->m_bClearDTMF ) 
				pChnlObj->m_strDTMFRecived	= "" ;

			
			pChnlObj->m_nCallType			= CHANNEL_CALLTYPE_COMEIN;
			pChnlObj->m_nRings				= 1;
			pChnlObj->m_nStep				= 1;
			pChnlObj->m_nState				= CHANNEL_STATE_RINGING;
			pChnlObj->m_nEventType			=  EVENTTYPE_RINGING;
			
			

			m_pPubCObj->PC_ShowChannelCallerID(nIndex,pChnlObj->m_strCallerID);
			m_pPubCObj->PC_ShowChannelCalledID(nIndex,pChnlObj->m_strCalledID);
			m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
		}
		break;
	case 1:
		{
			pChnlObj->m_nRings += 1;
			if( pChnlObj->m_nRings >= pObj->m_nRings )
			{
				DJTrk_InitDtmfBuf( nS7Chnl );
				if( pObj->m_bAnswerCall )
				{
					//应答计费,记录开始计费时间
					DJISUP_SetChnState( nS7Chnl/30,nS7Chnl%30, ISUP_CALLEE_SET_ACM,CI_CHARGE );//send ack signal
					DJISUP_SetChnState( nS7Chnl/30,nS7Chnl%30, ISUP_CALLEE_SET_ANM );//send offhook signal
					pChnlObj->m_strtBeginTime		= (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
				}

				
				pChnlObj->m_nEventType =  EVENTTYPE_OFFHOOK;
				pChnlObj->m_nState		=  CHANNEL_STATE_CONNECT;
				pChnlObj->m_nStep		= 2;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
			}
		}
		break;
	case 2:
		if(ISUP_CALLEE_CONNECT == nSS7_State ) 
		{
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			return;
		}
	default:
		break;
	}

	
	return;
}

void CDJSS17::DJSS_WaitCall_TUP(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	CWaitCallObj *pObj = (CWaitCallObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CWaitCallObj) )  || nS7Chnl < 0 )
	{
		m_strMSG.Format("ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}


	int nStep = pChnlObj->m_nStep;
	int nSS7_State = DJNo7_GetChnState(nS7Chnl/30,nS7Chnl%30);
	switch (nStep)
	{
	case 0:
		if(CALLEE_RECV_IAM == nSS7_State ) 
		{

			char szCallerID[128];
			char szCalledID[128];
			memset(szCallerID,0,sizeof(szCallerID));
			memset(szCalledID,0,sizeof(szCalledID));
			pChnlObj->m_strCallerID	= "";
			pChnlObj->m_strCalledID	= "";

			DJNo7_GetCalleeNum( nS7Chnl/30,nS7Chnl%30, szCalledID );
			DJNo7_GetCallerNum( nS7Chnl/30,nS7Chnl%30, szCallerID );


			pChnlObj->m_strCallerID=szCallerID ;
			pChnlObj->m_strCalledID=szCalledID ;
			if( pObj->m_bRecCallerID )
				m_pPubCObj->PC_SaveValuesToVar(pChnlObj,  pChnlObj->m_strCallerID, pObj->m_CallerID.m_nKey );
			if( pObj->m_bRecCalledID )
				m_pPubCObj->PC_SaveValuesToVar(pChnlObj,  pChnlObj->m_strCalledID, pObj->m_CalledID.m_nKey );
			if( pObj->m_bClearDTMF ) 
				pChnlObj->m_strDTMFRecived	= "" ;

			
			pChnlObj->m_nCallType			= CHANNEL_CALLTYPE_COMEIN;
			pChnlObj->m_nRings				= 1;
			pChnlObj->m_nStep				= 1;
			pChnlObj->m_nState				= CHANNEL_STATE_RINGING;
			pChnlObj->m_nEventType			=  EVENTTYPE_RINGING;
			
			

			m_pPubCObj->PC_ShowChannelCallerID(nIndex,pChnlObj->m_strCallerID);
			m_pPubCObj->PC_ShowChannelCalledID(nIndex,pChnlObj->m_strCalledID);
			m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
		}
		break;
	case 1:
		pChnlObj->m_nRings += 1;
		if( pChnlObj->m_nRings >= pObj->m_nRings  )
		{
			DJTrk_InitDtmfBuf( nS7Chnl );
			if( pObj->m_bAnswerCall )
			{
				//应答，计费，记录开始计费时间
				pChnlObj->m_strtBeginTime		= (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
				DJNo7_SetChnStateA( nS7Chnl/30,nS7Chnl%30, CALLEE_SET_ACM, ACM_ADDR_CHARGE + ACM_CALLEE_IDLE );//send ack signal
				DJNo7_SetChnStateA( nS7Chnl/30,nS7Chnl%30, CALLEE_SET_ANC, CSM_CHARGE );//send offhook signal
			
				pChnlObj->m_nEventType =  EVENTTYPE_OFFHOOK;
				pChnlObj->m_nState		=  CHANNEL_STATE_CONNECT;
				pChnlObj->m_nStep		= 2;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
			}
			else
			{
				
				pChnlObj->m_nState		=  CHANNEL_STATE_CONNECT;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );

			}

		}
		break;
	case 2:
		if(  CALLEE_CONNECT == nSS7_State )
		{
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			return;
		}
		break;
	default:
		break;
	}

	
	return;
}
void CDJSS17::DJSS_MyHangUpDetect_DSS1(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;
	if( CHANNEL_STATE_FREE == pChnlObj->m_nState   ) 
		return;
	if( CHANNEL_STATE_DIAL == pChnlObj->m_nState   ) 
		return;
	if( CH_FREE == DJISDN_GetChnState( nS7Chnl/30, nS7Chnl%30 ) )
	{
			CNodeObj  *pObj = pChnlObj->GetCurrentNodeObj();
			if( NULL != pObj )
			{
				if( NODETYPE_FAXRECIVE == pObj->m_nType )
				{
					int FaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
					if ( FaxChnl != -1 ) 
					{
						DJFax_StopFax(FaxChnl);
						DJFax_ClearLink(FaxChnl,nS7Chnl);
					}
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				}
				else if( NODETYPE_FAXSEND == pObj->m_nType )
				{
					int FaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
					if ( FaxChnl != -1 ) 
					{
						DJFax_StopFax(FaxChnl);
						DJFax_ClearLink(FaxChnl,nS7Chnl);
					}
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				}

			}
			else
			{

				DJSS_ResetTrunk(nIndex);
			}
	}
	return;
}

void CDJSS17::DJSS_MyHangUpDetect_SS1(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;
	if( CHANNEL_STATE_FREE == pChnlObj->m_nState   ) 
		return;
	if( CHANNEL_STATE_DIAL == pChnlObj->m_nState   ) 
		return;

	if ( DJTrk_GetTrunkStatus(nS7Chnl) == Step_Delay )
		return;
	if( DJTrk_CheckForwardHangUp(nS7Chnl) == 1 ) 
	{
		DJSS_ResetTrunk(nIndex);
	}
	return;
}

void CDJSS17::DJSS_MyHangUpDetect_SS7(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;
	if( CHANNEL_STATE_FREE == pChnlObj->m_nState   ) 
		return;
	if( CHANNEL_STATE_DIAL == pChnlObj->m_nState   ) 
		return;


	if( CF_CHTYPE_DTNO7 == pChnlObj->m_nChanelType  )
	{
		int result =  DJNo7_GetChnState( nS7Chnl/30, nS7Chnl%30 );
		if( CH_FREE == result )
		{
			m_strMSG.Format("HangUpDetect_SS7 : ChIndex = %d ", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );

			CNodeObj  *pObj = pChnlObj->GetCurrentNodeObj();
			if( NULL != pObj )
			{
				if( NODETYPE_FAXRECIVE == pObj->m_nType )
				{
					int FaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
					if ( FaxChnl != -1 ) 
					{
						DJFax_StopFax(FaxChnl);
						DJFax_ClearLink(FaxChnl,nS7Chnl);
					}
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				}
				else if( NODETYPE_FAXSEND == pObj->m_nType )
				{
					int FaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
					if ( FaxChnl != -1 ) 
					{
						DJFax_StopFax(FaxChnl);
						DJFax_ClearLink(FaxChnl,nS7Chnl);
					}
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				}

			}
			else
			{
				DJSS_ResetTrunk(nIndex);
			}
		}
	}
	else if( CF_CHTYPE_DTNO7ISUP == pChnlObj->m_nChanelType )
	{
		int result =  DJISUP_GetChnState( nS7Chnl/30, nS7Chnl%30 );
		if( ISUP_CH_FREE == result )
		{
			
			CNodeObj  *pObj = pChnlObj->GetCurrentNodeObj();
			if( NULL != pObj )
			{
				if( NODETYPE_FAXRECIVE == pObj->m_nType )
				{
					int FaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
					if ( FaxChnl != -1 ) 
					{
						DJFax_StopFax(FaxChnl);
						DJFax_ClearLink(FaxChnl,nS7Chnl);
					}
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				}
				else if( NODETYPE_FAXSEND == pObj->m_nType )
				{
					int FaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
					if ( FaxChnl != -1 ) 
					{
						DJFax_StopFax(FaxChnl);
						DJFax_ClearLink(FaxChnl,nS7Chnl);
					}
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				}

			}
			else


			DJSS_ResetTrunk(nIndex);
		}
	}

	return;
}

void CDJSS17::DJSS_MyHangUpDetect_User(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;
	if( CHANNEL_STATE_MONITORING ==  pChnlObj->m_nState ) return;
	if( CHANNEL_STATE_FREE == pChnlObj->m_nState ) return;
	if( CHANNEL_STATE_RINGING == pChnlObj->m_nState) return;
	//内线挂机和拍插簧检测
	int nResult = HangUpDetect(nS7Chnl);
	if( HANG_UP_FLAG_TRUE == nResult )//挂机
	{
		pChnlObj->m_nHangUpType = 1;//设置主动挂机
		m_pPubCObj->PC_CTI_HangUpMsg( nIndex, pChnlObj );
		DJSS_ResetUser( nIndex );	
	}
	return;
}
void CDJSS17::DJSS_MyHangUpDetect_Record(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;
	if( CHANNEL_STATE_FREE == pChnlObj->m_nState ) return;
	if( CHANNEL_STATE_RINGING == pChnlObj->m_nState) return;
	if( 0 == DRec_OffHookDetect(nS7Chnl) ) DJSS_ResetUser( nIndex );
	return;
}
void CDJSS17::DJSS_MyHangUpDetect(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if(  CF_CHTYPE_USER == pChnlObj->m_nChanelType )
	{
		DJSS_MyHangUpDetect_User( nIndex );
	}
	else if( CF_CHTYPE_RECORD == pChnlObj->m_nChanelType )
	{
		DJSS_MyHangUpDetect_Record( nIndex );
	}
	else
	{
		int nS7Chnl = pChnlObj->m_nChannel;
		BYTE cb = DJSys_GetPcmMode ( nS7Chnl / 30 );
		switch ( cb ) {
		case PCM_WORK_MODE_NONE:		// "7号(无信令)"
		case PCM_WORK_MODE_SS7:			// "7号"
			DJSS_MyHangUpDetect_SS7( nIndex );
			break;
		case PCM_WORK_MODE_SS1:			// "1号"
			DJSS_MyHangUpDetect_SS1( nIndex );
			break;
		case PCM_WORK_MODE_DSS1:		// "数字1号(ISDN PRI)"
			DJSS_MyHangUpDetect_DSS1( nIndex );
			break;
		}

	}

	
	return;
}

//////////////////////////////////////////////////
//中继通道拆线处理
//
//
//////////////////////////////////////////////////
void CDJSS17::DJSS_ResetTrunk(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  )  return ;
	
	m_pPubCObj->PC_Bfx2Tiff( pChnlObj->m_strReciveFaxFile );


	DJSS_ReleaseTrunk(nIndex);
	int nState = pChnlObj->m_nState;
	int nLinkIndex = pChnlObj->m_nLinkCh;
	if( -1 < nLinkIndex && nLinkIndex < m_nTotalChannel  )
	{
		if( CHANNEL_STATE_MONITORING !=  nState && CHANNEL_STATE_HOLD !=  nState )
		{
			if(  CF_CHTYPE_USER == m_pChannelInfo[nLinkIndex].m_nChanelType  ) 
			{
				DJSS_ReleaseUser( nLinkIndex );
			}
			else 
			{
				DJSS_ReleaseTrunk(nLinkIndex);
			}
		
			m_pChannelInfo[nLinkIndex].MoveCallEndBlock_Node();
			m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Free" );
		}	
	}

	pChnlObj->MoveCallEndBlock_Node();
	m_pPubCObj->PC_ShowChannelState( nIndex, "Free" );
	m_pPubCObj->PC_ShowConnectToChnl( nIndex, -1 );
	

	return;
}

void CDJSS17::DJSS_ResetUser(int nIndex)
{
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  )  return ;
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	m_pPubCObj->PC_Bfx2Tiff( pChnlObj->m_strReciveFaxFile );


	DJSS_ReleaseUser( nIndex );
	int nState = pChnlObj->m_nState;
	int nLinkIndex = pChnlObj->m_nLinkCh;
	int nTransferIndex = pChnlObj->m_nTransferCh;
	if( -1 < nLinkIndex && nLinkIndex < m_nTotalChannel  )
	{
		if( CHANNEL_STATE_MONITORING !=  nState && CHANNEL_STATE_HOLD !=  nState )
		{
			if(  CF_CHTYPE_USER == m_pChannelInfo[nLinkIndex].m_nChanelType ) 
			{
				DJSS_ReleaseUser( nLinkIndex );
			}
			else 
			{
				DJSS_ReleaseTrunk(nLinkIndex);
				
			}
			
			m_pChannelInfo[nLinkIndex].MoveCallEndBlock_Node();
			m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Free" );
			
		}

	}

	
	if( -1 < nTransferIndex && nTransferIndex < m_nTotalChannel  )
	{
		DJSS_ReleaseUser( nTransferIndex );
		m_pChannelInfo[nTransferIndex].MoveCallEndBlock_Node();
		m_pPubCObj->PC_ShowChannelState( nTransferIndex, "Free" );
		m_pPubCObj->PC_ShowConnectToChnl( nTransferIndex, -1 );
		
	}

	pChnlObj->MoveCallEndBlock_Node();

	m_pPubCObj->PC_ShowChannelState( nIndex, " Free");
	m_pPubCObj->PC_ShowConnectToChnl( nIndex, -1 );
	

	return;
}


void CDJSS17::DJSS_HangUp_SS1(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl	= pChnlObj->m_nChannel;//取得通道号
	DJTrk_BackwardHangUp( nS7Chnl );
	return;
}

void CDJSS17::DJSS_HangUp_DSS1(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl	= pChnlObj->m_nChannel;//取得通道号
	int nState =DJISDN_GetChnState(nS7Chnl/30,nS7Chnl%30);
	if( ISDN_CH_FREE == nState) return;
	if( ISDN_CH_WAIT_APP_FREE == nState)
	{
		 DJISDN_SetChnState(nS7Chnl/30,nS7Chnl%30 , ISDN_CH_SET_FREE, 0);
	}
	else
	{
		 DJISDN_SetChnState(nS7Chnl/30,nS7Chnl%30 , ISDN_CH_SET_DISCONNECT );
	}

	return;
}

void CDJSS17::DJSS_HangUp_SS7(int nIndex)
{

	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl	= pChnlObj->m_nChannel;//取得通道号
	if( CF_CHTYPE_DTNO7ISUP == pChnlObj->m_nChanelType )
	{
		//不管是呼入呼出，主动挂机都发ISUP_CH_SET_REL
		int nState = DJISUP_GetChnState( nS7Chnl/30, nS7Chnl%30 ) ;
		if( ISUP_CH_FREE == nState ) return;
		if( ISUP_CH_WAIT_RELEASE  == nState)
		{
			DJISUP_SetChnState(nS7Chnl/30,nS7Chnl%30, ISUP_CH_SET_FREE );
		}
		else
		{
			DJISUP_SetChnState(nS7Chnl/30,nS7Chnl%30, ISUP_CH_SET_REL );
			pChnlObj->m_nHangUpType = 1;//设置主动挂机
		}
	}
	else
	{
		int nState = DJNo7_GetChnState( nS7Chnl/30, nS7Chnl%30 );
		if( CH_FREE == nState ) return;
		if( CH_WAIT_RELEASE == nState )
		{
			DJNo7_SetChnState( nS7Chnl/30,nS7Chnl%30, CH_SET_FREE );
		}
		else
		{
			if( CHANNEL_CALLTYPE_CALLOUT == pChnlObj->m_nCallType )
			{
				// 去话连通状态
				DJNo7_SetChnState(nS7Chnl/30,nS7Chnl%30, CALLER_SET_CLF );//呼出时主动挂机要用CALLER_SET_CLF
				pChnlObj->m_nHangUpType = 1;//设置主动挂机
				
			}
			else
			{
				DJNo7_SetChnState(nS7Chnl/30,nS7Chnl%30, CALLEE_SET_CBK );//对方呼入时主动挂机要用CALLEE_SET_CBK
				pChnlObj->m_nHangUpType = 1;//设置主动挂机
			}
		}
	}
	return;
}

////////////////////////////////////////////////
//释放中继通道的相关资源
//
///////////////////////////////////////////////
void CDJSS17::DJSS_ReleaseTrunk(int nIndex)
{
	if( nIndex == -1 ) return ;
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl	= pChnlObj->m_nChannel;//取得通道号
	int nLinkIndex	= pChnlObj->m_nLinkCh;//取得通道号
	if( nLinkIndex > -1 ) DJSS_MyClearLink(nIndex, nLinkIndex );
		
	DJSS_StopRecord(nIndex);
	DJTrk_InitDtmfBuf( nS7Chnl );

	if( pChnlObj->m_nConfNO > -1 )
	{
		DJSS_DJConf_LeaveConf( nIndex, pChnlObj->m_nConfNO );
		
	}
	if( 1 == m_nFax )//有传真卡
	{
		int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
		if( -1 != nFaxChnl )
		{
			DJFax_StopFax(nFaxChnl);
			DJFax_ClearLink(nFaxChnl,nS7Chnl);
			m_pFaxChannel[nFaxChnl].m_nCurrPage		= 0;
			m_pFaxChannel[nFaxChnl].m_nStatus			= 0;
			m_pFaxChannel[nFaxChnl].m_nTotalPage		= 0;
			m_pFaxChannel[nFaxChnl].m_strFaxFileName = "";
			m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
		}
	}

	DJSS_HangUpTrunk( nIndex );


	m_nVocBusy		= DJVoc_SFVC_ForPlay_New(0, false);
	m_nVocRingBack	= DJVoc_SFVC_ForPlay_New(0, false);
	m_nVocTone		= DJVoc_SFVC_ForPlay_New(0, false);
	DJVoc_LoopPlayPromptFile( m_nVocBusy, "BUSY" ) ;
	DJVoc_LoopPlayPromptFile( m_nVocRingBack, "RING" ) ;
	DJVoc_LoopPlayPromptFile( m_nVocTone, "TONE" ) ;

	return;
}

void CDJSS17::DJSS_ReleaseUser(int nIndex)
{

	if( nIndex == -1 ) return ;
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl	= pChnlObj->m_nChannel;//取得通道号
	int nLinkIndex	= pChnlObj->m_nLinkCh;//取得通道号
	int nType = pChnlObj->m_nChanelType;
	DJSS_StartPlaySignal(nIndex,SIG_STOP);
	DJSS_StopRecord(nIndex);
	if( nLinkIndex > -1 ) DJSS_MyClearLink(nIndex, nLinkIndex );
	
	if( pChnlObj->m_nConfNO > -1 )
	{
		DJSS_DJConf_LeaveConf( nIndex, pChnlObj->m_nConfNO );

	}
	if( 1 == m_nFax )
	{
		int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
		if( -1 != nFaxChnl )
		{
			DJFax_StopFax(nFaxChnl);
			DJFax_ClearLink(nFaxChnl,nS7Chnl);
			m_pFaxChannel[nFaxChnl].m_nCurrPage		= 0;
			m_pFaxChannel[nFaxChnl].m_nStatus			= 0;
			m_pFaxChannel[nFaxChnl].m_nTotalPage		= 0;
			m_pFaxChannel[nFaxChnl].m_strFaxFileName = "";
			m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
		}
	}

	if( CF_CHTYPE_RECORD != nType ) {
		FeedPower(nS7Chnl);
	}
	InitDtmfBuf(nS7Chnl);
	
	return;
	
}
void CDJSS17::DJSS_NodeEnd(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if(  pChnlObj->m_nChanelType >= 6 &&  pChnlObj->m_nChanelType <= 8 ) 
	{
		DJSS_ResetUser( nIndex );
	}
	else 
	{
		m_pPubCObj->PC_ShowSysMsg( nIndex, "DJSS_NodeEnd" );
		DJSS_ResetTrunk(nIndex);
	}
	

	return;
}

void CDJSS17::DJSS_EventCheck(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CEventCheckObj *pObj = (CEventCheckObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CEventCheckObj) ) )
	{
		m_strMSG.Format("EventCheck error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}


	if( 0 == pChnlObj->m_nStep )
	{
		pChnlObj->m_nStep = 1;
		m_pPubCObj->PC_StartTimer(pChnlObj);
		if(  pObj->m_bClearDTMF )
		{
			DJSS_InitDtmfBuf(nIndex);
			pChnlObj->m_strDTMFRecived = "";
			if( EVENTTYPE_RECDTMF == pChnlObj->m_nEventType )
			{
				pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
			}
		}
	}

	if(  pObj->m_bElapseTime )
	{
		if( m_pPubCObj->PC_ElapseTime(pChnlObj) >=  pObj->m_nElapseTime )
		{
			//到了指定的时间，将发生时钟事件
			m_pPubCObj->PC_StartTimer(pChnlObj);
			pChnlObj->m_nEventType		=  EVENTTYPE_ELAPSE_TIME;
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ELAPSE_TIME);
			return;
		}
	}

    if( DJSS_EventCheck_EventMsg( nIndex, pObj ) )//如果检测到事件消息，直接退出
		return;
	if( DJSS_GetDtmfCode( nIndex , pChnlObj->m_strDTMFRecived ) > 0 )
	{
		m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlObj->m_strDTMFRecived );
		pChnlObj->m_nEventType		=  EVENTTYPE_RECDTMF;
		if( pObj->m_bRecDTMF )
		{
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_RECDTMF);
			return;
		}
		
	}
	
	int nChnlType = pChnlObj->m_nChanelType;
	switch(nChnlType)
	{
	case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
	case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
	case CF_CHTYPE_MTP3://七号信令MTP传输层通道
		DJSS_EventCheck_SS7( nIndex, pObj );
		break;
	case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
		break;
	case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
		DJSS_EventCheck_User( nIndex, pObj );
		break;
	case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
		DJSS_EventCheck_User( nIndex, pObj );
		break;
	case CF_CHTYPE_DTNO1://数字中继中国一号信令 
		DJSS_EventCheck_SS1( nIndex, pObj );
		break;
	case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
		DJSS_EventCheck_DSS1( nIndex, pObj );
		break;
	case CF_CHTYPE_VIP://VOIP通道类型
	case CF_CHTYPE_NOKNOW://未知通道类型 
	case CF_CHTYPE_EMPTY://空通道 
	default:
		break;
	}
	
	if( pObj->m_bNothing )
	{
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NOTHING);
	}

	return;
}

BOOL CDJSS17::DJSS_EventCheck_EventMsg(int nIndex, CEventCheckObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	switch( pChnlObj->m_nEventType )
	{
	case EVENTTYPE_OFFHOOK:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
		{
			m_strMSG.Format("[Send OffHook DN:%s]",pChnlObj->m_strLocalDN);
			m_pPubCObj->PC_ShowCommMsg(m_strMSG);
			CTI_OffHookMsg( atoi(pChnlObj->m_strLocalDN) );
		}
		if( pObj->m_bOffHook )
		{
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_OFFHOOK);
			return TRUE;
		}
		break;
	case EVENTTYPE_HANGUP:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
		{
			m_pPubCObj->PC_CTI_HangUpMsg( nIndex, pChnlObj );
			
		}
		m_pPubCObj->PC_ShowChannelState( nIndex, "Free" );
		if( pObj->m_bHangUp )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_HANGUP);
			return TRUE;
		}
		else
		{
			//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
			pChnlObj->MoveCallEndBlock_Node();
		}
		break;
	case EVENTTYPE_HOOKFLASH:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bHookFlash )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_HOOKFLASH);
			return TRUE;
		}
		break;
	case EVENTTYPE_RECDTMF:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bRecDTMF )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_RECDTMF);
			return TRUE;
		}
		break;
	case EVENTTYPE_RECFAXTONE:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bRecFaxTone )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_RECFAXTONE);
			return TRUE;
		}
		break;

	case EVENTTYPE_CONFDESTROYED:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bConfDestroyed )
		{
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_CONFDESTROYED);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_LOGIN_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentLoginMsg )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_LOGIN_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_OUT_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentLogOutMsg )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_OUT_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_BUSY_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentBusyMsg )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_BUSY_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_INSERVE_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentInserveMsg )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_INSERVE_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_ANSWER_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentAnswerMsg )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_ANSWER_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_MAKECALL_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentMakeCallMsg )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_MAKECALL_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_TRANSFER_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentTransferMsg )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_TRANSFER_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_CONFER_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentConferMsg )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_CONFER_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_HANGUP_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentHangUpMsg )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_HANGUP_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_SENDDATA_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentSendDataMsg )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_SENDDATA_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_SENDFAX_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentSendFaxMsg )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_SENDFAX_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_RINGING:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bRinging )
		{
			
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_RINGING);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_HOLD_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentHold )
		{
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_HOLD_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_PICKUPHOLD_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentPickupHold )
		{
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_PICKUPHOLD_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_MONITOR_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentMonitor )
		{
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_MONITOR_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_DISMONITOR_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentDisMonitor )
		{
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_DISMONITOR_MSG);
			return TRUE;
		}
		break;
	case EVENTTYPE_AGENT_BUILDCONFER_MSG:
		pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentBuildConfer )
		{
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_BUILDCONFER_MSG);
			return TRUE;
		}
		break;
	default:
		break;
	}

	return FALSE;
}

void CDJSS17::DJSS_EventCheck_SS1(int nIndex, CEventCheckObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	if(! DJTrk_CheckReady( nS7Chnl ) ) return;
	if( DJTrk_CheckForwardHangUp( nS7Chnl ) )
	{
		//if( pObj->m_bHangUp  &&  (CHANNEL_STATE_CONNECT == pChnlObj->m_nState || CHANNEL_STATE_MONITORING ==  pChnlObj->m_nState ))
		if( pObj->m_bHangUp )
		{
			DJSS_ReleaseTrunk( nIndex );
			pChnlObj->m_nEventType =  EVENTTYPE_HANGUP;
			if( pObj->m_bHangUp )
			{
				pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_HANGUP);
				return;
			}
			else
			{
				//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
				pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
				pChnlObj->MoveCallEndBlock_Node();
			}
		}
		return;
	}


	//振铃检测
	if( DJTrk_GetTrunkStatus( nS7Chnl) != Step_Delay ) 
	{
		if(  CHANNEL_STATE_FREE != pChnlObj->m_nState ) return;
		pChnlObj->m_nCallType	= CHANNEL_CALLTYPE_COMEIN;
		pChnlObj->m_nEventType =  EVENTTYPE_RINGING;
		pChnlObj->m_nState		=  CHANNEL_STATE_RINGING;
		pChnlObj->m_strRingTime = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
		m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
		if( pObj->m_bRinging )
		{
			pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_RINGING);
		}
		return ;
	}

	return ;
}

void CDJSS17::DJSS_EventCheck_DSS1(int nIndex, CEventCheckObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	int result = DJISDN_GetChnState( nS7Chnl/30, nS7Chnl%30 );

	if( pObj->m_bHangUp )
	{
		if( ISDN_CH_WAIT_APP_FREE == result )
		{
			DJSS_ReleaseTrunk( nIndex );
			DJISDN_SetChnState(nS7Chnl/30,nS7Chnl%30, ISDN_CH_SET_FREE ,0); 
			pChnlObj->m_nEventType =  EVENTTYPE_HANGUP;
			if( pObj->m_bHangUp )
			{
				pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_HANGUP);
			}
			else
			{
				//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
				pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
				pChnlObj->MoveCallEndBlock_Node();
			}
		}
		return ;
	}

	//振铃检测
	if(ISDN_CALLEE_WAIT_ANSWER == result ) 
	{
		pChnlObj->m_nCallType	= CHANNEL_CALLTYPE_COMEIN;
		pChnlObj->m_nEventType =  EVENTTYPE_RINGING;
		pChnlObj->m_nState		=  CHANNEL_STATE_RINGING;
		pChnlObj->m_strRingTime = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
		m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
		if( pObj->m_bRinging )
		{
			pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_RINGING);
		}
		return ;
	}

	return ;
}

void CDJSS17::DJSS_EventCheck_SS7(int nIndex, CEventCheckObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	int result = 0;
	if( CF_CHTYPE_DTNO7ISUP == pChnlObj->m_nChanelType )
	{
		result = DJISUP_GetChnState( nS7Chnl/30, nS7Chnl%30 );
	}
	else
	{
		result = DJNo7_GetChnState( nS7Chnl/30, nS7Chnl%30 );
	}

	if( pObj->m_bHangUp )
	{
		if( CF_CHTYPE_DTNO7ISUP == pChnlObj->m_nChanelType )
		{
			if( ISUP_CH_FREE == result )
			{
				DJSS_ReleaseTrunk( nIndex );
				
				pChnlObj->m_nEventType =  EVENTTYPE_HANGUP;
				if( pObj->m_bHangUp )
				{
					pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_HANGUP);
				}
				else
				{
					//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
					pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
					pChnlObj->MoveCallEndBlock_Node();
				}
			}
			return ;
		}
		else if( CF_CHTYPE_DTNO7 == pChnlObj->m_nChanelType ) 
		{
			if( CH_FREE == result )
			{
				m_pPubCObj->PC_ShowSysMsg( nIndex, "DJSS_EventCheck_SS7" );
				DJSS_ReleaseTrunk( nIndex );
				
				
				
				pChnlObj->m_nEventType =  EVENTTYPE_HANGUP;
				if( pObj->m_bHangUp )
				{
					pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_HANGUP);
					return;
				}
				else
				{
					//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
					pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
					pChnlObj->MoveCallEndBlock_Node();
					return;
				}
			}
			return ;
		}
	}

	//振铃检测
	if( CF_CHTYPE_DTNO7ISUP == pChnlObj->m_nChanelType )
	{
		if(  CHANNEL_STATE_FREE != pChnlObj->m_nState ) return;
		if(ISUP_CALLEE_RECV_IAM == result ) 
		{
			pChnlObj->m_nCallType	= CHANNEL_CALLTYPE_COMEIN;
			pChnlObj->m_nEventType =  EVENTTYPE_RINGING;
			pChnlObj->m_nState		=  CHANNEL_STATE_RINGING;
			pChnlObj->m_strRingTime = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
			m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
			if( pObj->m_bRinging )
			{
				pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_RINGING);
			}
			return ;
		}
	}
	else
	{
		if( CALLEE_RECV_IAM == result ) 
		{
			pChnlObj->m_nCallType	= CHANNEL_CALLTYPE_COMEIN;
			pChnlObj->m_nEventType =  EVENTTYPE_RINGING;
			pChnlObj->m_nState		=  CHANNEL_STATE_RINGING;
			pChnlObj->m_strRingTime = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
			m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
			if( pObj->m_bRinging )
			{
				pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_RINGING);
			}
			return ;
		
		}
	}


	return ;
}



void CDJSS17::DJSS_EventCheck_User(int nIndex, CEventCheckObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	int nChnlType = pChnlObj->m_nChanelType;
	if( CHANNEL_STATE_FREE == pChnlObj->m_nState  )
	{
		if( RingDetect(nS7Chnl) )//内线摘机，录音，外线振铃
		{
			
			if( CF_CHTYPE_USER == nChnlType )
			{
				m_strMSG.Format("[Send OffHook DN:%s]",pChnlObj->m_strLocalDN);
				m_pPubCObj->PC_ShowCommMsg(m_strMSG);//发送摘机消息
				CTI_OffHookMsg( atoi(pChnlObj->m_strLocalDN) );//发送

		
				StartHangUpDetect(nS7Chnl);
				if( CHANNEL_STATE_FREE == pChnlObj->m_nState )
					DJSS_StartPlaySignal( nIndex, SIG_DIALTONE );
				else
					DJSS_StartPlaySignal( nIndex, SIG_STOP );

				pChnlObj->m_nState =  CHANNEL_STATE_CONNECT;
				pChnlObj->m_nEventType = EVENTTYPE_OFFHOOK;
				pChnlObj->m_strRingTime = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
				pChnlObj->m_strtBeginTime = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");				
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
				if( pObj->m_bOffHook )
				{
					pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_OFFHOOK);
					return ;
				}
					
				pChnlObj->m_nState =  CHANNEL_STATE_CONNECT;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
			}
			else
			{
				pChnlObj->m_nEventType =  EVENTTYPE_RINGING;
				pChnlObj->m_nState =  CHANNEL_STATE_RINGING;
				pChnlObj->m_strRingTime = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
				pChnlObj->m_strtBeginTime = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");								
				m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
				if( pObj->m_bRinging )
				{
					pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_RINGING);
					return ;
				}
				
			}
		}
	}//end if

	
	if( CHANNEL_STATE_CONNECT == pChnlObj->m_nState || CHANNEL_STATE_MONITORING ==  pChnlObj->m_nState  )//内线挂机和拍插簧检测
	{
		if( CF_CHTYPE_USER == nChnlType )
		{
			int nResult = HangUpDetect(nS7Chnl);
			if( HANG_UP_FLAG_TRUE == nResult )//挂机
			{
				pChnlObj->m_nHangUpType = 1;//设置主动挂机
				m_pPubCObj->PC_CTI_HangUpMsg( nIndex, pChnlObj );
				if( pObj->m_bHangUp )
				{
					pChnlObj->m_nState = CHANNEL_STATE_FREE;
					pChnlObj->m_nEventType = EVENTTYPE_NOTHING;;
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_HANGUP);
					return ;
				}
				else
				{
					//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
					DJSS_ResetUser( nIndex );
				}
				m_pPubCObj->PC_ShowChannelState( nIndex, "Free");
			}
			else if( HANG_UP_FLAG_PRESS_R == nResult ) //拍了一次插簧
			{
				if( pObj->m_bHookFlash )
				{
					pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_HOOKFLASH);
				}
				m_pPubCObj->PC_ShowChannelState( nIndex, "HookFlash");
				return;
			}
		}
		else
		{
			if( CF_CHTYPE_RECORD != nChnlType ) return;
			if( 0 != DRec_OffHookDetect(nS7Chnl) ) return;
			if( pObj->m_bHangUp )
			{
				pChnlObj->m_nEventType = EVENTTYPE_NOTHING;;
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_HANGUP);
				return ;
			}
			else
			{
				//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
				DJSS_ResetUser( nIndex );
			}
			m_pPubCObj->PC_ShowChannelState( nIndex, "Free");

		}
		
	}//end if


	return ;

}

void CDJSS17::DJSS_PlayStartUser(int nIndex,  CPlayStartObj* pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	int nStep	= pChnlObj->m_nStep;
	switch(nStep)
	{
	case 0:
		{
			if( pObj->m_bClearDTMF ) 
			{
				DJSS_InitDtmfBuf(nIndex);
				pChnlObj->m_strDTMFRecived = "";
			}
			
			DJSS_StopPlay( nIndex );
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
					m_pPubCObj->PC_GetExpressValues(pChnlObj, m_strMSG, pData->m_strData);
					char *pszFileName = (LPTSTR)(LPCTSTR)m_strMSG;
					if( !AddIndexPlayFile( nS7Chnl , pszFileName ) )
					{
						m_strMSG.Format("%s: ChIndex = %d    Play error ",  pObj->m_strCaption, nIndex);
						m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
						DJSS_StopPlay( nIndex );
						DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
				}
				else
				{
					//表达式
					if(! DJSS_PlayExpressVox( nIndex, pData->m_strData , -1 ) )
					{
						m_strMSG.Format("%s: ChIndex = %d    Play Express Error :%s ",  pObj->m_strCaption, nIndex, pData->m_strData );
						m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
						DJSS_StopPlay( nIndex );
						DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
				}
			}//end while
			if( !StartIndexPlayFile( nS7Chnl ) )
			{
				m_strMSG.Format("%s: ChIndex = %d    Play error ",  pObj->m_strCaption, nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_StopPlay( nIndex );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			pChnlObj->m_nStep = 1;
		}
		break;
	case 1:
		{
			if( DJSS_GetDtmfCode( nIndex , pChnlObj->m_strDTMFRecived ) > 0 )
			{
				m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlObj->m_strDTMFRecived );
				if( pObj->m_bPlayStop )
				{
					DJSS_StopPlay(nIndex);
					pChnlObj->m_nStep = 2;
				}
			}
			else if( CheckIndexPlayFile(nS7Chnl)  )
			{
				DJSS_StopPlay(nIndex);
				pChnlObj->m_nStep = 2;
			}

		}
		break;
	case 2:
		{	
			if( pObj->m_bRecDTMF )
			{
				int nVarKey = pObj->GetVarKey( pObj->m_strSaveDTMF );
				BOOL bResult = m_pPubCObj->PC_SaveValuesToVar(pChnlObj, pChnlObj->m_strDTMFRecived, nVarKey );
				if( !bResult )
				{

					DJSS_StopPlay(nIndex);
					m_strMSG.Format("%s Save DTMF to Var Error ;ChIndex = %d",pObj->m_strCaption, nIndex );
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
			}
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			return;
		}
		break;
	default:
		{
			m_strMSG.Format("%s: ChIndex = %d    Play error ",  pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		break;
	}//end switch

	return;
}

void CDJSS17::DJSS_PlayStartTrunk(int nIndex,  CPlayStartObj* pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	int nStep	= pChnlObj->m_nStep;
	switch(nStep)
	{
	case 0:
		{
			if( pObj->m_bClearDTMF ) 
			{
				DJSS_InitDtmfBuf(nIndex);
				pChnlObj->m_strDTMFRecived = "";
			}

			DJSS_StopPlay(nIndex);
			
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
					m_pPubCObj->PC_GetExpressValues(pChnlObj, m_strMSG, pData->m_strData);
					char *pszFileName = (LPTSTR)(LPCTSTR)m_strMSG;
					if( !DJVoc_AddIndexPlayFile( nS7Chnl , pszFileName ) )
					{
						DJSS_StopPlay(nIndex);
						m_strMSG.Format("%s: ChIndex = %d    Play error ",  pObj->m_strCaption, nIndex);
						m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
						DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
				}
				else
				{
					//表达式
					if(! DJSS_PlayExpressVox( nIndex, pData->m_strData,-1 ) )
					{

						DJSS_StopPlay(nIndex);
						m_strMSG.Format("%s: ChIndex = %d    Play Express Error :%s ",  pObj->m_strCaption, nIndex, pData->m_strData );
						m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
						DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
				}
			}//end while
			if( !DJVoc_StartIndexPlayFile( nS7Chnl ) )
			{

				DJSS_StopPlay(nIndex);
				m_strMSG.Format("%s: ChIndex = %d    Play Express Error  ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			pChnlObj->m_nStep = 1;
		}
		break;
	case 1:
		{
			if( DJSS_GetDtmfCode( nIndex , pChnlObj->m_strDTMFRecived ) > 0 )
			{
				m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlObj->m_strDTMFRecived );
				if( pObj->m_bPlayStop )
				{
					DJSS_StopPlay(nIndex);
					pChnlObj->m_nStep = 2;
				}

			}
			else if( DJVoc_CheckIndexPlayEnd(nS7Chnl)  )
			{
				DJSS_StopPlay(nIndex);
				pChnlObj->m_nStep = 2;
			}


		}
		break;
	case 2:
		{	
			if( pObj->m_bRecDTMF )
			{
				int nVarKey = pObj->GetVarKey( pObj->m_strSaveDTMF );
				BOOL bResult = m_pPubCObj->PC_SaveValuesToVar(pChnlObj, pChnlObj->m_strDTMFRecived, nVarKey );
				if( !bResult )
				{

					DJSS_StopPlay(nIndex);
					m_strMSG.Format("%s Save DTMF to Var Error ;ChIndex = %d",pObj->m_strCaption, nIndex );
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
			}
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			return;
		}
		break;
	default:
		{
			m_strMSG.Format("%s: ChIndex = %d    Play error ",  pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		break;
	}//end switch
	
	return;
}

//当nGroupNo=-1时表示是普通放音，大于-1时是会议放音
BOOL CDJSS17::DJSS_PlayExpressVox(int nIndex, CString strExpress, int nGroupNo )
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	CString strNewExpress("");
	if( !m_pPubCObj->PC_GetExpressValues(pChnlObj,strNewExpress,strExpress) )
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
			if( -1 == nGroupNo )//普通放音
			{
				char *pszFileName = (LPTSTR)(LPCTSTR)strFileName;
				if(  pChnlObj->m_nChanelType >= 6 &&  pChnlObj->m_nChanelType <= 8 ) 
				{
					if( !AddIndexPlayFile( nS7Chnl , pszFileName ) )
					{
						return FALSE;
					}
				}
				else
				{
					if( !DJVoc_AddIndexPlayFile( nS7Chnl , pszFileName ) )
					{
						return FALSE;
					}
				}
			}
			else
			{
				char *pszFileName = (LPTSTR)(LPCTSTR)strFileName;
				return DJConf_AddIndexPlayFile (  nGroupNo , pszFileName );
			}
		}

	}

	return TRUE;
}
void CDJSS17::DJSS_PlayStart(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CPlayStartObj *pObj = (CPlayStartObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CPlayStartObj) ) )
	{
		m_strMSG.Format("ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	DJSS_FaxKey( nIndex );//加密

	if( 0 == pObj->m_nPlayWay )
	{
		if(  pChnlObj->m_nChanelType >= 6 &&  pChnlObj->m_nChanelType <= 8 ) 
		{
			DJSS_PlayStartUser( nIndex,pObj );
		}
		else
		{
			DJSS_PlayStartTrunk( nIndex,pObj);
		}
	}
	else//会议放音
	{
		DJSS_PlayStart_Confer( nIndex,  pObj );
	}


	return;
}


void CDJSS17::DJSS_PlayStart_Confer(int nIndex, CPlayStartObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	static int nGroupNo = 0;
	int nStep = pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			int nVarKey = pObj->GetVarKey( pObj->m_strConfNO  );
			if( nVarKey < 0 )
			{
				nGroupNo = atoi(pObj->m_strConfNO );
			}
			else
			{
				if( !m_pPubCObj->PC_GetNorVarValues(pChnlObj, m_strMSG, pObj->GetVarKey( pObj->m_strConfNO ) ) )
				{
					m_strMSG.Format("%s: ChIndex = %d    Play error ",  pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				nGroupNo = atoi(m_strMSG);
			}

			if(  nGroupNo < 0 || nGroupNo >= m_nTotalConf )
			{
				m_strMSG.Format("%s: ChIndex = %d    Group error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

			DJConf_StopIndexPlayFile( nGroupNo );
			DJConf_InitIndexPlayFile( nGroupNo );
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
					m_pPubCObj->PC_GetExpressValues(pChnlObj, m_strMSG, pData->m_strData);
					char *pszFileName = (LPTSTR)(LPCTSTR)m_strMSG;
					if( !DJConf_AddIndexPlayFile( nGroupNo , pszFileName ) )
					{
						DJConf_StopIndexPlayFile( nGroupNo );
						m_strMSG.Format("%s: ChIndex = %d    Add Play error ",  pObj->m_strCaption, nIndex);
						m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
						DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
				}
				else
				{
					//表达式
					if(! DJSS_PlayExpressVox( nIndex, pData->m_strData,nGroupNo ) )
					{

						DJConf_StopIndexPlayFile( nGroupNo );
						m_strMSG.Format("%s: ChIndex = %d    Play Express Error :%s ",  pObj->m_strCaption, nIndex, pData->m_strData );
						m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
						DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
				}
			}//end while
			if( DJConf_StartIndexPlayFile( nGroupNo ) )
			{
				DJConf_StopIndexPlayFile( nGroupNo );
				m_strMSG.Format("%s: ChIndex = %d   Start Play  Error code=%d  ",  pObj->m_strCaption, nIndex, DJSys_GetErrCode() );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

			pChnlObj->m_nStep = 1;
		}
		break;
	case 1:
		if( DJConf_CheckIndexPlayEnd( nGroupNo ) )
		{
			DJConf_StopIndexPlayFile( nGroupNo );
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );	
		}
		break;
	default:
		break;
	}
	
	return;
}
void CDJSS17::DJSS_InitDtmfBuf(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	if(  pChnlObj->m_nChanelType >= 6 &&  pChnlObj->m_nChanelType <= 8 ) 
	{
		InitDtmfBuf( nS7Chnl );
	}
	else
	{
		DJTrk_InitDtmfBuf(nS7Chnl);
	}
	
	return;
}


//接收DTMF码，然后删除
int CDJSS17::DJSS_GetDtmfCode(int nIndex ,CString &strDtmf)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	strDtmf = "";
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	int nCodeNum = 0;
	if(  pChnlObj->m_nChanelType >= 6 &&  pChnlObj->m_nChanelType <= 8 ) 
	{
		short int code = 0 ;
		int i = 0;
		while( code > -1 )
		{
			code = GetDtmfCode( nS7Chnl );
			if( code > -1 )
			{
				strDtmf += g_pD08a->GetCodeAscii(code);
				//strDtmf += DJSS_GetCodeAscii(code);
				i++;
			}
		}
		nCodeNum = i;

	}
	else
	{
		 nCodeNum = DJTrk_GetReciveDtmfNum(nS7Chnl);
		 if( nCodeNum > 0 )
		 {
			 strDtmf.Format("%s", DJTrk_GetDtmfCode(nS7Chnl) );
			 DJTrk_InitDtmfBuf(nS7Chnl);
		 }
	}

	return nCodeNum;
}

//nGroupNo=-1时表示非会议录音
BOOL CDJSS17::DJSS_MyRecordFile(int nIndex, char *pszfilename,DWORD dRecordLen, int nGroupNo)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	if( -1 == nGroupNo ){
		if(  pChnlObj->m_nChanelType >= 6 &&  pChnlObj->m_nChanelType <= 8 ) 
		{
			return StartRecordFile ( nS7Chnl, pszfilename, 8000L*3600L );	
		}
		else
		{
		
			return DJVoc_RecordFile( nS7Chnl, pszfilename , 8000L*3600L );
		}
	}
	else{
		if( 0 > DJConf_RecordFileNew( nGroupNo, pszfilename, 0L,  dRecordLen ) )
			return TRUE;
		else
			return FALSE;

	}

}

BOOL CDJSS17::DJSS_CheckRecordFileEnd(int nIndex,int nGroupNo)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	int nType = pChnlObj->m_nChanelType;
	if( -1 != nGroupNo ) return FALSE;

	switch(nType)
	{
	case CF_CHTYPE_NOKNOW://    -1 //未知通道类型 
		break;
	case CF_CHTYPE_EMPTY://     0  //空通道 
		break;
	case CF_CHTYPE_DTNO1://     1  //数字中继中国一号信令 
	case CF_CHTYPE_DTNO7 ://    2  //数字中继中国七号信令(TUP)
	case CF_CHTYPE_DTDSS1://    3  //数字中继ISDN PRI(30B+D)
	case CF_CHTYPE_DTNO7ISUP:// 4  //数字中继中国七号信令(ISUP)
	case CF_CHTYPE_MTP3://		5  //七号信令MTP传输层通道
		{
			int vid = DJTrk_GetTrunkRecordID( nS7Chnl );
			if( DJVoc_CheckVoiceEnd(vid) )
			{
				//DJVoc_StopRecordFile(nS7Chnl);
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		break;
	case CF_CHTYPE_TRUNK://     6  //模拟中继外线(普通市话线),接电信局
	case CF_CHTYPE_USER://		7  //模拟中继内线(座席线)，接电话机
	case CF_CHTYPE_RECORD://    8  //模拟中继高阻线(监控线)，与电话线/电话机相并联
		{
			if( 1 == CheckRecordEnd( nS7Chnl ) )
			{
				StopRecordFile(nS7Chnl);
				return TRUE;
			}
			else
				return FALSE;

		}
		break;
	case CF_CHTYPE_VIP://	    9  //VOIP通道类型
		break;
	default:
		break;
	}

	return FALSE;
}

BOOL CDJSS17::DJSS_PlayFile(int nIndex, char *pszfilename)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	if(  pChnlObj->m_nChanelType >= 6 &&  pChnlObj->m_nChanelType <= 8 ) 
	{
		return StartPlayFile( nS7Chnl, pszfilename , 0L);
	}
	else
	{
		return DJVoc_PlayFile(nS7Chnl, pszfilename);
	}


}

BOOL CDJSS17::DJSS_CheckPlayFileEnd(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	int nType = pChnlObj->m_nChanelType;
	switch(nType)
	{
	case CF_CHTYPE_NOKNOW://    -1 //未知通道类型 
	case CF_CHTYPE_EMPTY://     0  //空通道 
		return TRUE;
	case CF_CHTYPE_DTNO1://     1  //数字中继中国一号信令 
	case CF_CHTYPE_DTNO7 ://    2  //数字中继中国七号信令(TUP)
	case CF_CHTYPE_DTDSS1://    3  //数字中继ISDN PRI(30B+D)
	case CF_CHTYPE_DTNO7ISUP:// 4  //数字中继中国七号信令(ISUP)
	case CF_CHTYPE_MTP3://		5  //七号信令MTP传输层通道
		{
			int vid = DJTrk_GetTrunkPlayID( nS7Chnl );
			if( vid >= 0   )
			{
				return DJVoc_CheckVoiceEnd(vid);
			}
			else
			{
				return TRUE;
			}
		}
	case CF_CHTYPE_TRUNK://     6  //模拟中继外线(普通市话线),接电信局
	case CF_CHTYPE_USER://		7  //模拟中继内线(座席线)，接电话机
	case CF_CHTYPE_RECORD://    8  //模拟中继高阻线(监控线)，与电话线/电话机相并联
		return CheckPlayEnd( nS7Chnl );
	case CF_CHTYPE_VIP://	    9  //VOIP通道类型
		break;
	default:
		break;
	}
	return TRUE;
}

void CDJSS17::DJSS_PlayFileStop(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	if(  pChnlObj->m_nChanelType >= 6 &&  pChnlObj->m_nChanelType <= 8 ) 
	{
		StopPlayFile ( nS7Chnl );
		StopIndexPlayFile(nS7Chnl);
		RsetIndexPlayFile(nS7Chnl);
	}
	else
	{
		DJVoc_StopPlayFile( nS7Chnl );
		DJVoc_StopIndexPlayFile( nS7Chnl );
		DJVoc_InitIndexPlayFile( nS7Chnl );
	}

	return;
}



CNodeObj * CDJSS17::DJSS_MoveNextObj(int nNextType, CNodeObj *pCurObj, CCFDocument *pDoc)
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

BOOL CDJSS17::DJSS_MoveNext_Node(int nIndex, CNextNode *pNextObj)
{
	
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	return m_pPubCObj->PC_MoveNext_Node( nIndex, pChnlObj, pNextObj );
	
}
BOOL CDJSS17::DJSS_MoveNext_Node(int nIndex, int nNextType)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	return m_pPubCObj->PC_MoveNext_Node( nIndex, pChnlObj, nNextType );
}


void CDJSS17::DJSS_CallLogRequest(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CCallLogRequestObj *pObj = (CCallLogRequestObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CCallLogRequestObj) ) )
	{
		m_strMSG.Format("CallLogRequest ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}


	if( 0 == pObj->m_nRequestType ) //Begin Save Call Log
	{
		pChnlObj->m_strtBeginTime		= (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
	}
	else
	{
		pChnlObj->m_strtEndTime		= (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
	}

	DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	return;

	
}
/////////////////////////////////////////
//接收公网传真机发来的传真
//
//
/////////////////////////////////////////
void CDJSS17::DJSS_FaxRecive(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CFaxReciveObj *pObj = (CFaxReciveObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CFaxReciveObj) ) )
	{
		m_strMSG.Format("Recive Fax error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}
	if( 1 != m_nFax )
	{
		m_strMSG.Format("Recive Fax error : ChIndex = %d   Error = The system not support FAx ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}

	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	int nStep = pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{

			CString strFile("");
			int nFaxChnl = DJFax_GetOneFreeFaxChnl();
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				BOOL bResult = m_pPubCObj->PC_GetExpressValues(pChnlObj, strFile, pObj->m_strFileName);
				if( !bResult || -1 == pObj->GetVarKey( pObj->m_strPage)  )
				{
					m_strMSG.Format("FaxRecive Error; ChIndex = %d   Error = File name error", nIndex );
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return ;   	
				}
				strFile = pObj->m_strFolder + strFile;
				m_pFaxChannel[nFaxChnl].m_strFaxFileName = strFile;
				
				
				
				if(!DJFax_SetLink(nFaxChnl,nS7Chnl))
				{
					m_strMSG.Format("%s: ChIndex = %d    Fax SetLink error ",  pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				DJFax_SetLocalID (nFaxChnl, "1234567890ABCDEF1234");
				pChnlObj->m_nFaxChnl	= nFaxChnl;
				m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_RECEIVE;
				m_pFaxChannel[nFaxChnl].m_nCurrPage	=0;
				m_pFaxChannel[nFaxChnl].m_nTotalPage	=0;
				
				pChnlObj->m_strReciveFaxFile = strFile;
				m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
				pChnlObj->m_nStep = 1;
			}
			else
			{
				m_strMSG.Format("FaxRecive Error; ChIndex = %d   Error = not get free fax channel resource ", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;  
			}
		}
		break;
	case 1:
		{
			
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
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
				m_strMSG.Format("FaxRecive Error; ChIndex = %d   Error = not get  fax channel resource ", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;  
			}
		}
		break;
	case 2:
		{
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
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
				m_strMSG.Format("FaxRecive Error; ChIndex = %d   Error = not get  fax channel resource ", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;  
			}

		}
		break;
	case 3:
		{
			//所有的发送或接收正确结束
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				m_strMSG.Format( "%d",m_pFaxChannel[nFaxChnl].m_nCurrPage );
				if( !m_pPubCObj->PC_SaveValuesToVar(pChnlObj,  m_strMSG, pObj->GetVarKey( pObj->m_strPage)  ) )
				{
					pChnlObj->m_nStep = 4;
				}
				else
				{
					DJFax_StopFax(nFaxChnl);
					DJFax_ClearLink(nFaxChnl,nS7Chnl);

					pChnlObj->m_nFaxChnl	= -1;
					m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_FREE;

					m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
					return;
				}
			}
			else
			{
				m_strMSG.Format("FaxRecive Error; ChIndex = %d   Error = not get  fax channel resource ", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ; 
			}
		}
		break;
	case 4:
		{
			//错误结束
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				pChnlObj->m_nFaxChnl	= -1;
				m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_FREE;

				DJFax_StopFax(nFaxChnl);
				DJFax_ClearLink(nFaxChnl,nS7Chnl);
				m_strMSG.Format("%s: ChIndex = %d    RcvFaxFile error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			else
			{
				m_strMSG.Format("FaxRecive Error; ChIndex = %d   Error = not get  fax channel resource ", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ; 
			}
		}
		break;
	default:
		break;

	}

	return;
}


///////////////////////////////////
//发送传真到传真机上
//
//////////////////////////////////
void CDJSS17::DJSS_FaxSend(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CFaxSendObj *pObj = (CFaxSendObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CFaxSendObj) ) )
	{
		m_strMSG.Format("Send Fax error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}
	if( 1 != m_nFax )
	{
		m_strMSG.Format("Send Fax error : ChIndex = %d   Error = The system not support FAx ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}

	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	int nStep	= pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			CString strFile("");
			BOOL bResult = m_pPubCObj->PC_GetExpressValues(pChnlObj, strFile, pObj->m_strFileName);
			if( !bResult  )
			{
				m_strMSG.Format("Fax Send Error; ChIndex = %d   Error = File name error", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;   	
			}
			strFile = pObj->m_strFolder + strFile;
			if( !m_pPubCObj->PC_Tiff2Bfx( strFile ) )
			{
				m_strMSG.Format("Tiff[%s] to Bfx Error; ChIndex = %d   Error = File name error", strFile, nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;   
			}

			int nFaxChnl = DJFax_GetOneFreeFaxChnl();
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				
				m_pFaxChannel[nFaxChnl].m_strFaxFileName = strFile;
				
				if(!DJFax_SetLink(nFaxChnl,nS7Chnl))
				{
					m_strMSG.Format("%s: ChIndex = %d    Fax SetLink error ",  pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
				m_strMSG.Format("%s: ChIndex = %d not get free Fax channel resource  error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}


		}
		break;
	case 1:
		{
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
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
				m_strMSG.Format("%s: ChIndex = %d not get  Fax channel resource  error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
		}
		break;
	case 3:
		{
			//检查发送
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
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
				m_strMSG.Format("%s: ChIndex = %d not get  Fax channel resource  error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

		}
		break;
	case 4:
		{
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				DJFax_StopFax(nFaxChnl);
				DJFax_ClearLink(nFaxChnl,nS7Chnl);

				pChnlObj->m_nFaxChnl	= -1;
				m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_FREE;

				m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				return;
			}
			else
			{
				m_strMSG.Format("%s: ChIndex = %d not get  Fax channel resource  error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
		}
		break;
	case 5:
		{

			//错误结束
			int nFaxChnl = DJFax_GetFaxChnlOfVoiceChnl(nS7Chnl);
			if( nFaxChnl >= 0 && nFaxChnl < m_nTotalFaxLine )
			{
				pChnlObj->m_nFaxChnl	= -1;
				m_pFaxChannel[nFaxChnl].m_nStatus		= CIF_FAX_FREE;

				DJFax_StopFax(nFaxChnl);
				DJFax_ClearLink(nFaxChnl,nS7Chnl);
				m_strMSG.Format("%s: ChIndex = %d    SendFaxFile error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				m_pPubCObj->PC_ShowFaxUpdate( nFaxChnl, &(m_pFaxChannel[nFaxChnl]) );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			else
			{
				m_strMSG.Format("%s: ChIndex = %d not get  Fax channel resource  error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

		}

	default:
		break;
	}

	return;
}

/***************************************************************
	内线使用该节点是内线通道时，直接对该内线通道振领，检测摘机
	即可；外线需要使用本通道，根据给的号码进行呼叫，检测呼叫结果
	即可。如果是录音通道，不能使用该节点。
***************************************************************/
void CDJSS17::DJSS_Dial(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nType = pChnlObj->m_nChanelType;
	if( CF_CHTYPE_RECORD == nType ) return;
	CDialCheckObj *pObj = (CDialCheckObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDialCheckObj) ) )
	{
		m_strMSG.Format("DialCheck Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		if( pObj->m_bError )
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	
	
	int nSetup		= pChnlObj->m_nStep;
	int nSS7Chnl	= pChnlObj->m_nChannel;//取得通道号
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
					if(  !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strPhone, nKey ) )
					{
						m_strMSG.Format("%s Dial tel Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
						m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
						if( pObj->m_bError )
							DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
				}
				
				memmove( szPhone, strPhone, strlen(strPhone) >= 32 ? 31:strlen(strPhone) );
				if( strlen(szPhone) < 3 )
				{
					//号码长度小于3时，被认为号码错误
					m_strMSG.Format("%s  Phone Error; ChIndex = %d", pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					if( pObj->m_bError )
						DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
			}
			if( !DJSS_MyMakeCall( nIndex, szPhone, (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN ) )
			{
				
				m_strMSG.Format("%s Dial fail ,GErrCode=%d ; ChIndex = %d ",pObj->m_strCaption, DJSys_GetErrCode(), nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			pChnlObj->m_nStep	= 1;
			
		}
		break;
	case 1:
		{
			int nCallOutResult = DJSS_MyMakeCall_Check( nIndex );
			if( 3 == nCallOutResult )//未得到结果
				return;
			else if( 0 == nCallOutResult ){
				//成功
				pChnlObj->m_nState			= CHANNEL_STATE_CONNECT;
				pChnlObj->m_strState ="Connect";
				m_pPubCObj->PC_ShowChannelState( nIndex, "Connect" );
				if( pObj->m_bConnect )
				{
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_CONNECT );
				}
				else
				{
					//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
					pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
					pChnlObj->MoveCallEndBlock_Node();
				}
				return;
			}
			else{
				//超时或失败
				pChnlObj->m_strDTMFRecived	= "";
				pChnlObj->m_nState			= CHANNEL_STATE_FREE;
				pChnlObj->m_nStep			= 0;
				pChnlObj->m_nEventType		=  EVENTTYPE_HANGUP;
				pChnlObj->m_nLinkCh			= -1;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Free" );
				
				DJNo7_SetChnState(nSS7Chnl/30,nSS7Chnl%30, CALLER_SET_CLF ) ;
				if( pObj->m_bBusy)
				{
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_BUSY);
					return;
				}
				else if( pObj->m_bNoAnswer)
				{
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				else
				{
					//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
					pChnlObj->m_nEventType = EVENTTYPE_NOTHING;
					pChnlObj->MoveCallEndBlock_Node();
					return;
				}
			}

		}
		
		break;
	case 2:
		break;
	}

	return;

}


void CDJSS17::DJSS_ReleaseCall(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if(  pChnlObj->m_nChanelType >= 6 &&  pChnlObj->m_nChanelType <= 8 ) 
		DJSS_ResetUser(nIndex);
	else
		DJSS_ResetTrunk(nIndex);
	m_pPubCObj->PC_ShowSysMsg( nIndex, "DJSS_ReleaseCall" );
	DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	return;
}

void CDJSS17::DJSS_CallHold(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) 
	{
		m_strMSG.Format("CallHold error : CallHold are only used by the User channel,  ChIndex = %d ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	CCallHoldObj *pObj = (CCallHoldObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CCallHoldObj) ) )
	{
		m_strMSG.Format("CallHold error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	
	//PickUp Hold

	if( g_pD08a->CompareVarKey( 1,pObj->m_nCallHold) )
	//if( 1 == pObj->m_nCallHold ) 
	{
		DJSS_CallHold_PickUp( nIndex, pObj );//判断是否是恢复通话
		pChnlObj->m_nHoldChnl = -1;
		return;
	}

	//如果是Hold,进行下面操作
	int nIndexLink = pChnlObj->m_nLinkCh;
	if( -1 >= nIndexLink || nIndexLink >= m_nTotalChannel  )
	{
		m_strMSG.Format("%s have not active call; ChIndex = %d ", pObj->m_strCaption, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;	
	}


	DJSS_InitDtmfBuf( nIndex );
	pChnlObj->m_strDTMFRecived = "";
	if( !DJSS_MyClearLink( nIndex, nIndexLink) )
	{
		DJSS_MySetLink( nIndex, nIndexLink, CF_CHSWITCH_CONNECT );
		m_strMSG.Format("CallHold error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	if( pObj->m_bPlayVox ) //看是否播放音乐
	{
		char *pszFileName = (LPTSTR)(LPCTSTR)pObj->m_strPlayFile;
		if( !DJSS_PlayFile( nIndexLink, pszFileName ) )
		{
			if( pObj->m_bPlayVox ) DJSS_PlayFileStop( nIndexLink );
			DJSS_MySetLink( nIndex, nIndexLink , CF_CHSWITCH_CONNECT );
			m_strMSG.Format("CallHold error : ChIndex = %d", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		m_pChannelInfo[nIndexLink].m_strPlayFile = pObj->m_strPlayFile;
	}
	pChnlObj->m_nHoldChnl = nIndexLink;
	pChnlObj->m_nLinkCh = -1;
	m_pChannelInfo[nIndexLink].m_nState = CHANNEL_STATE_HOLD;
	m_pPubCObj->PC_ShowChannelState( nIndexLink, "Hold" );
	DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);

  

	return;
}

void CDJSS17::DJSS_CallHold_PickUp(int nIndex, CCallHoldObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nLinkIndex = -1;
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( nIndex == m_pChannelInfo[i].m_nLinkCh )
		{
			nLinkIndex = i;
			break;
		}
	}

	int nDN = atoi( pChnlObj->m_strLocalDN );
	if( -1 >= nLinkIndex || nLinkIndex >= m_nTotalChannel  )
	{
		m_strMSG.Format("have not PickUp Hold active Call  : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 1,"","" );//执行错误
		return;
	}
	m_pChannelInfo[nLinkIndex].m_nState = CHANNEL_STATE_CONNECT;
	m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Busy");
	DJSS_PlayFileStop( nLinkIndex );
	DJSS_PlayFileStop( nIndex );
	if( !DJSS_MySetLink( nIndex, nLinkIndex , CF_CHSWITCH_CONNECT ) )
	{
		m_strMSG.Format("PickUp Hold error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 1,"","" );
		return;
		
	}
	pChnlObj->m_nLinkCh = nLinkIndex;
	DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 1,m_pChannelInfo[nLinkIndex].m_strCallerID,m_pChannelInfo[nLinkIndex].m_strCalledID );

	return;
}

BOOL CDJSS17::DJSS_MySetLink(int nIndex1, int nIndex2, int nSwitchType)
{
	DJSS_StartPlaySignal(nIndex1,SIG_STOP);
	DJSS_StopPlay( nIndex1 );
	DJSS_StartPlaySignal(nIndex2,SIG_STOP);
	DJSS_StopPlay( nIndex2 );
	DJSS_MyClearLink( nIndex1, nIndex2 );//在连接之前必须清除原来的连接

	int nChnlType1   = m_pChannelInfo[nIndex1].m_nChanelType;
	switch( nChnlType1 )
	{
	case CF_CHTYPE_DTNO1://数字中继中国一号信令 
	case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
	case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
	case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
	case CF_CHTYPE_MTP3://七号信令MTP传输层通道
		return DJSS_MySetLink_Trunk(nIndex1, nIndex2, nSwitchType );
		break;
	case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
	case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
	case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
		return DJSS_MySetLink_User(nIndex1, nIndex2, nSwitchType );
		break;
	case CF_CHTYPE_VIP://VOIP通道类型
	case CF_CHTYPE_NOKNOW://未知通道类型 
	case CF_CHTYPE_EMPTY://空通道 
	default:
		break;
	}

	return FALSE;

}

BOOL CDJSS17::DJSS_MyClearLink(int nIndex1, int nIndex2)
{

	DJSS_StopPlay(nIndex1 );
	DJSS_StopPlay(nIndex2 );
	int nS7Chnl1	= m_pChannelInfo[nIndex1].m_nChannel;//取得通道号
	int nS7Chnl2	= m_pChannelInfo[nIndex2].m_nChannel;//取得通道号
	int nChnlType1   = m_pChannelInfo[nIndex1].m_nChanelType;
	int nChnlType2  = m_pChannelInfo[nIndex2].m_nChanelType;
	switch( nChnlType1 )
	{
	case CF_CHTYPE_DTNO1://数字中继中国一号信令 
	case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
	case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
	case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
	case CF_CHTYPE_MTP3://七号信令MTP传输层通道
		if(  nChnlType2 >=6 && nChnlType2 <= 8  ) 
		{
			DJExg_ClearLinkTrunkAndUserByUser(nS7Chnl2);
		}
		else
		{
			DJExg_ClearLinkTrunkAndTrunk( nS7Chnl1 );
		}
		break;
	case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
	case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
	case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
		if(  nChnlType2 >=6 && nChnlType2 <= 8  ) 
		{
			ClearLink( nS7Chnl1, nS7Chnl2  );
			ClearOneFromAnother( nS7Chnl1, nS7Chnl2 );
		}
		else
		{
			DJExg_ClearLinkTrunkAndUserByUser( nS7Chnl1 );
			DJExg_ClearListenUserFromTrunk( nS7Chnl1 );
		}
		break;
	case CF_CHTYPE_VIP://VOIP通道类型
	case CF_CHTYPE_NOKNOW://未知通道类型 
	case CF_CHTYPE_EMPTY://空通道 
	default:
		return FALSE;
		break;
	}

	return TRUE;
}

BOOL CDJSS17::DJSS_MySetLink_User(int nIndex1, int nIndex2, int nSwitchType)
{

	int nChnl1 = m_pChannelInfo[nIndex1].m_nChannel;
	int nChnl2 = m_pChannelInfo[nIndex2].m_nChannel;
	int nChnlType2   = m_pChannelInfo[nIndex2].m_nChanelType;
	if( CF_CHSWITCH_CONNECT == nSwitchType )
	{
		switch( nChnlType2 )
		{
		case CF_CHTYPE_DTNO1://数字中继中国一号信令 
		case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
		case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
		case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
		case CF_CHTYPE_MTP3://七号信令MTP传输层通道
			if( 1 == DJExg_SetLinkTrunkAndUser(nChnl2,nChnl1) )
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
			break;
		case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
		case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
		case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
			if( 0 == SetLink(nChnl1,nChnl2) )
			{
				m_pChannelInfo[nIndex1].m_nLinkType = nSwitchType;
				m_pChannelInfo[nIndex2].m_nLinkType = nSwitchType;
				m_pPubCObj->PC_ShowConnectToChnl(nIndex1,nIndex2);
				m_pPubCObj->PC_ShowConnectToChnl(nIndex2,nIndex1);
				return TRUE;
			}
			else
				return FALSE;
			break;
		case CF_CHTYPE_VIP://VOIP通道类型
		case CF_CHTYPE_NOKNOW://未知通道类型 
		case CF_CHTYPE_EMPTY://空通道 
		default:
			return FALSE;
		}

	}
	else if( CF_CHSWITCH_MONITOR == nSwitchType )
	{
		switch( nChnlType2 )
		{
		case CF_CHTYPE_DTNO1://数字中继中国一号信令 
		case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
		case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
		case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
		case CF_CHTYPE_MTP3://七号信令MTP传输层通道
			if( 1 == DJExg_SetListenUserToTrunk(nChnl1,nChnl2) )
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
			break;
		case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
		case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
		case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
			if( 0 == LinkOneToAnother(nChnl1,nChnl2) )
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
			break;
		case CF_CHTYPE_VIP://VOIP通道类型
		case CF_CHTYPE_NOKNOW://未知通道类型 
		case CF_CHTYPE_EMPTY://空通道 
		default:
			return FALSE;
			break;
		}

	}

	return FALSE;
}

BOOL CDJSS17::DJSS_MySetLink_Trunk(int nIndex1, int nIndex2, int nSwitchType)
{

	int nLinkResult = 0;
	int nChnl1 = m_pChannelInfo[nIndex1].m_nChannel;
	int nChnl2 = m_pChannelInfo[nIndex2].m_nChannel;
	int nChnlType2   = m_pChannelInfo[nIndex2].m_nChanelType;
	if( CF_CHSWITCH_CONNECT == nSwitchType )
	{
		switch( nChnlType2 )
		{
		case CF_CHTYPE_DTNO1://数字中继中国一号信令 
		case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
		case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
		case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
		case CF_CHTYPE_MTP3://七号信令MTP传输层通道
			nLinkResult = DJExg_SetLinkTrunkAndTrunk(nChnl1,nChnl2);
			break;
		case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
		case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
		case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
			nLinkResult = DJExg_SetLinkTrunkAndUser(nChnl1,nChnl2);
			break;
		case CF_CHTYPE_VIP://VOIP通道类型
		case CF_CHTYPE_NOKNOW://未知通道类型 
		case CF_CHTYPE_EMPTY://空通道 
		default:
			nLinkResult = 0;
			break;
		}
		if( 0 == nLinkResult )
		{
			return FALSE;
		}
		else
		{
			m_pChannelInfo[nIndex1].m_nLinkType = nSwitchType;
			m_pChannelInfo[nIndex2].m_nLinkType = nSwitchType;
			m_pPubCObj->PC_ShowConnectToChnl(nIndex1,nIndex2);
			m_pPubCObj->PC_ShowConnectToChnl(nIndex2,nIndex1);
			return TRUE;
		}
	}
	else if( CF_CHSWITCH_MONITOR == nSwitchType )
	{

		switch( nChnlType2 )
		{
		case CF_CHTYPE_DTNO1://数字中继中国一号信令 
		case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
		case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
		case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
		case CF_CHTYPE_MTP3://七号信令MTP传输层通道
			nLinkResult = DJExg_SetListenTrunkToUser(nChnl1,nChnl2);
			break;
		case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
		case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
		case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
			nLinkResult = DJExg_SetListenTrunkToTrunk(nChnl1,nChnl2);
			break;
		case CF_CHTYPE_VIP://VOIP通道类型
		case CF_CHTYPE_NOKNOW://未知通道类型 
		case CF_CHTYPE_EMPTY://空通道 
		default:
			nLinkResult = 0;
			break;
		}
		if( 0 == nLinkResult )
		{
			return FALSE;
		}
		else
		{
			m_pChannelInfo[nIndex1].m_nLinkType = nSwitchType;
			m_pChannelInfo[nIndex2].m_nLinkType = nSwitchType;
			m_pPubCObj->PC_ShowConnectToChnl(nIndex1,nIndex2);
			m_pPubCObj->PC_ShowConnectToChnl(nIndex2,nIndex1);
			return TRUE;
		}
	}
	else
	{
		return FALSE;
	}

}

void CDJSS17::DJSS_StopRecord(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nSS7Chnl		= pChnlObj->m_nChannel;//取得通道号
	int voiceID = -1;
	int nChnlType   = pChnlObj->m_nChanelType;
	switch( nChnlType )
	{
	case CF_CHTYPE_DTNO1://数字中继中国一号信令 
	case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
	case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
	case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
	case CF_CHTYPE_MTP3://七号信令MTP传输层通道
		{
			DJVoc_StopRecordFile ( nSS7Chnl );
			voiceID = DJTrk_GetTrunkRecordID(nSS7Chnl);
			if(voiceID != -1)
			{
				DJExg_ClearLinkRecordVoiceFromTrunk(nSS7Chnl);
				HANDLE handle = (HANDLE)DJVoc_GetVoiceHandle(voiceID);
				DJVoc_VoiceStop( voiceID );
				if( INVALID_HANDLE_VALUE != handle )
					CloseHandle(handle);
			}
			
		}
		break;
	case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
	case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
	case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
		StopRecordFile ( nSS7Chnl );
		break;
	case CF_CHTYPE_VIP://VOIP通道类型
	case CF_CHTYPE_NOKNOW://未知通道类型 
	case CF_CHTYPE_EMPTY://空通道 
	default:
		break;
	}

	return;
}

void CDJSS17::DJSS_StopPlay(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int voiceID = -1;
	int nSS7Chnl		= pChnlObj->m_nChannel;//取得通道号
	int nChnlType   = pChnlObj->m_nChanelType;
	switch( nChnlType )
	{
	case CF_CHTYPE_DTNO1://数字中继中国一号信令 
	case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
	case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
	case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
	case CF_CHTYPE_MTP3://七号信令MTP传输层通道
		DJVoc_StopPlayFile ( nSS7Chnl );
		voiceID = DJTrk_GetTrunkPlayID( nSS7Chnl );
		if( voiceID >= 0 )
		{
			DJVoc_StopPlayMemory(voiceID);
			DJExg_ClearLinkPlayVoiceFromTrunk( nSS7Chnl );
		}
		else
		{
			DJExg_ClearLinkPlayVoiceFromTrunk( nSS7Chnl );
		}


		voiceID = DJTrk_GetTrunkPlayID( nSS7Chnl );
		if( voiceID != -1 )
		{
			DJVoc_VoiceStop( voiceID );
		}
		DJVoc_StopPlayFile( nSS7Chnl );
		DJVoc_StopIndexPlayFile(nSS7Chnl);
		DJVoc_InitIndexPlayFile(nSS7Chnl);
		break;
	case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
	case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
	case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
		StopPlayFile ( nSS7Chnl );
		StopPlay( nSS7Chnl );
		StartPlaySignal(nSS7Chnl,SIG_STOP);
		StopIndexPlayFile ( nSS7Chnl );
		RsetIndexPlayFile(nSS7Chnl);
		break;
	case CF_CHTYPE_VIP://VOIP通道类型
	case CF_CHTYPE_NOKNOW://未知通道类型 
	case CF_CHTYPE_EMPTY://空通道 
	default:
		break;
	}

	return;
}

int CDJSS17::GetHoldChnl(int nIndex)
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
//DTMF码转换为Ascii
int CDJSS17::DJSS_GetAsciiToCode(char code)
{
	switch(code)
	{
	case 'D':		return 0;		break;
	case '1':		return 1;		break;
	case '2':		return 2;		break;
	case '3':		return 3;		break;
	case '4':		return 4;		break;
	case '5':		return 5;		break;
	case '6':		return 6;		break;
	case '7':		return 7;		break;
	case '8':		return 8;		break;
	case '9':		return 9;		break;
	case '0':		return 10;		break;
	case '*':		return 11;		break;
	case '#':		return 12;		break;
	case 'A':		return 13;		break;
	case 'B':		return 14;		break;
	case 'C':		return 15;		break;
	default:		return -1;
	}
}

char CDJSS17::DJSS_GetCodeAscii(int code)
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

void CDJSS17::DJSS_DTMFSend(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CDTMFSendObj *pObj = (CDTMFSendObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDTMFSendObj) ) )
	{
		m_strMSG.Format("DTMF Send Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   // if pObject is not valid
	}
	
	int nSetup		= pChnlObj->m_nStep;
	int nSS7Chnl	= pChnlObj->m_nChannel;//取得通道号
	switch( nSetup )
	{
	case 0:
		{
			//if( !DJTrk_CheckWait(nSS7Chnl) ) return;
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
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					m_strMSG.Format("%s Input DTMF Express  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					return;
				}
			}
			char *pszDTMF = (LPTSTR)(LPCTSTR)strNewExpress;



			if(!DJSS_MySendDtmfStr(nIndex, pszDTMF ) )
			{
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				m_strMSG.Format("%s DJTrk_SendDtmfStr  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				return;
			}
			pChnlObj->m_nStep = 1;
		}
		break;
	case 1:
		if(DJSS_MyCheckSendEnd(nIndex) )
		{
			DJVoc_StopPlayFile(nSS7Chnl);
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		}
		break;
	default:
		break;
	}

	return;
}

void CDJSS17::DJSS_DTMFRecive(int nIndex )
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CDTMFReciveObj *pObj = (CDTMFReciveObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDTMFReciveObj) ) )
	{
		m_strMSG.Format("DTMF Recive Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   // if pObject is not valid
	}

	
	m_pPubCObj->PC_ShowChannelDTMF( nIndex,pChnlObj->m_strDTMFRecived);


	int nStep		= pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			m_pPubCObj->PC_ShowChannelDTMF( nIndex,pChnlObj->m_strDTMFRecived);
			m_pPubCObj->PC_StartTimer(pChnlObj);
			if( pObj->m_bClearDTMF ) 
			{
				DJSS_InitDtmfBuf(nIndex);
				pChnlObj->m_strDTMFRecived = "";
			}
			pChnlObj->m_strState = "ReciveDTMF";
			int  len = strlen(pChnlObj->m_strDTMFRecived);
			
			if(  pObj->m_nDTMFType > 0 )//只接收数字键
			{
				pChnlObj->m_strDTMFRecived.Replace('*',NULL);
				pChnlObj->m_strDTMFRecived.Replace('#',NULL);
			}
			if( g_pD08a->CompareVarKey( 0,pObj->m_nEndCondition) )
			//if( 0 == pObj->m_nEndCondition )
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
					pChnlObj->m_nStep = 1;
			}

			DJSS_FaxKey( nIndex );//加密
		}
		break;
	case 1:
		{
			int nCodeNum = DJSS_GetDtmfCode( nIndex ,m_strMSG);
			if( nCodeNum > 0 )
			{
				m_pPubCObj->PC_ShowChannelDTMF( nIndex,pChnlObj->m_strDTMFRecived);
				DJSS_StartPlaySignal(nIndex,SIG_STOP);
				if( 0 == pObj->m_nEndCondition )
				{
					if(  pObj->m_nDTMFType > 0 )//只接收数字键
					{
						if( "*" == m_strMSG )  return;
						if( "#" == m_strMSG )  return;
					}
					//通过判断接收按键的最大数量来结束收键
					pChnlObj->m_strDTMFRecived += m_strMSG;
					int  len = pChnlObj->m_strDTMFRecived.GetLength();
					if( len  >= pObj->m_nDTMFCount )
					{
						pChnlObj->m_nStep = 2;
						return;
					}
				}
				else
				{
					//通过判断接收特定按键值来结束收键
					if( m_strMSG == pObj->m_strDTMF )
					{
						pChnlObj->m_nStep = 2;
						return;
					}
					else
					{
						if(  pObj->m_nDTMFType > 0 )//只接收数字键
						{
							if( "*" == m_strMSG )  return;
							if( "#" == m_strMSG )  return;
						}
						pChnlObj->m_strDTMFRecived += m_strMSG;
					}
				}

				
			}
			long n = m_pPubCObj->PC_ElapseTime(pChnlObj);
			if( n >= pObj->m_nTimeOut )
			{
				m_pPubCObj->PC_SaveValuesToVar(pChnlObj, pChnlObj->m_strDTMFRecived, pObj->m_SaveDTMF.m_nKey );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				m_strMSG.Format("%s TimeOut Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				return;
			}
		}
		break;
	case 2:
		{
			BOOL bResult = m_pPubCObj->PC_SaveValuesToVar(pChnlObj, pChnlObj->m_strDTMFRecived, pObj->m_SaveDTMF.m_nKey );
			if( bResult )
			{
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
			else
			{
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				m_strMSG.Format("%s Save DTMF to Var Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			}
			
		}
	default:
		break;
	}

	
	return;
}

void CDJSS17::DJSS_DTMFMenu( int nIndex )
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CDTMFMenuObj *pObj = (CDTMFMenuObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDTMFMenuObj) ) )
	{
		m_strMSG.Format("DTMFMenu Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	m_pPubCObj->PC_ShowChannelDTMF( nIndex,pChnlObj->m_strDTMFRecived);
	int nStep = pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			m_pPubCObj->PC_StartTimer(pChnlObj);
			if( pObj->m_bClearDTMF )
			{
				DJSS_InitDtmfBuf(nIndex);
				pChnlObj->m_strDTMFRecived = "";
			}
			pChnlObj->m_nState  = CHANNEL_STATE_CONNECT ; 
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
			pChnlObj->m_nStep = 1;
			
			
		}
		break;
	case 1:
		{

			int nCodeNum = DJSS_GetDtmfCode( nIndex ,m_strMSG);
			if( nCodeNum > 0 )
			{
				DJSS_StartPlaySignal(nIndex,SIG_STOP);
				pChnlObj->m_strDTMFRecived = m_strMSG.Right( 1 );
				m_pPubCObj->PC_ShowChannelDTMF(nIndex,pChnlObj->m_strDTMFRecived);
	
			}
			
			long n = m_pPubCObj->PC_ElapseTime(pChnlObj);
			if( n >= pObj->m_nTimeOut )
			{
				DJSS_StartPlaySignal(nIndex,SIG_STOP);
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_DTMF_TIMEOUT );
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

				DJSS_StartPlaySignal(nIndex,SIG_STOP);

				DJSS_MoveNext_Node( nIndex,  nNextType );
				return;
			}
		}
		break;
	default:
		break;
	}
	return;
}



void CDJSS17::DJSS_PushPlay()
{
	if ( m_nIs_SS7 ) 
	{
		if( 0 == m_nSS7_type )//TUP
		{
			DJNo7_GetEvent() ;
		}
		else if( 1 == m_nSS7_type )//TUP and ISUP
		{
			DJISUP_GetEvent();
		}
		else if( 2 == m_nSS7_type )//TUP and ISUP
		{
			DJNo7_GetEvent();
			DJISUP_GetEvent();
		}
		else
		{
			DJNo7_GetEvent() ;
		}

	}

	if ( m_nIs_DSS1 ) 
	{
		DJISDN_GetEvent();
	}
	
	DJSys_PushPlay() ;
	return;
}

void CDJSS17::DJSS_Switch_CallFreeAgent(int nIndex, CSwitchObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nAimIndex	= pChnlObj->m_nLinkCh ;
	int nStep		= pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			nAimIndex = DJSS_FindFreeChannel(CF_CHTYPE_USER );
			if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
			{
				m_strMSG.Format("%s have not free channel ; ChIndex = %d ",pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			pChnlObj->m_nLinkCh = nAimIndex;
			//呼叫分机空闲，对其振铃
			m_pChannelInfo[nAimIndex].m_nLinkCh = nIndex;
			if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
			{
				DJSS_MyMakeCall( nAimIndex, (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN, (LPTSTR)(LPCTSTR)pChnlObj->m_strCalledID );
			}
			else
			{
				DJSS_MyMakeCall( nAimIndex, (LPTSTR)(LPCTSTR)pChnlObj->m_strCalledID, (LPTSTR)(LPCTSTR)pChnlObj->m_strCallerID );
			}

		
			//switch的等待选项，一般只用于呼叫时用
			if( 0 == pObj->m_nWaitOption )//播放回铃音
			{
				DJSS_StartPlaySignal(nIndex, SIG_RINGBACK);
			}
			else if( 2 == pObj->m_nWaitOption )//播放语音文件
			{
				CString strFile("");
				BOOL bResult = m_pPubCObj->PC_GetExpressValues( pChnlObj,  strFile, pObj->m_strPlayFile);
				if( !bResult )
				{
					m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
					DJSS_ResetUser( nAimIndex );
					pChnlObj->m_nLinkCh = -1;
					DJSS_StartPlaySignal(nIndex,SIG_BUSY1);
					m_strMSG.Format("Switch Error; ChIndex = %d   Get Play file Error", nIndex );
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				char *pszFileName = (LPTSTR)(LPCTSTR)strFile;
				BOOL bError  = DJSS_PlayFile( nIndex, pszFileName );
				if( !bError )//false
				{
					m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
					DJSS_ResetUser( nAimIndex );
					pChnlObj->m_nLinkCh = -1;
					DJSS_StartPlaySignal(nIndex,SIG_BUSY1);
					m_strMSG.Format("Switch Error; ChIndex = %d   Play file Error", nIndex );
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
				if( DJSS_CheckPlayFileEnd(nIndex) )//播放语音文件
				{
					DJSS_PlayFileStop( nIndex );
					CString strFile("");
					BOOL bResult = m_pPubCObj->PC_GetExpressValues( pChnlObj,  strFile, pObj->m_strPlayFile);
					if( !bResult )
					{
						m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
						DJSS_ResetUser( nAimIndex );
						pChnlObj->m_nLinkCh = -1;
						DJSS_StartPlaySignal(nIndex,SIG_BUSY1);
						m_strMSG.Format("Switch Error; ChIndex = %d   Get Play file Error", nIndex );
						m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
						DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
					char *pszFileName = (LPTSTR)(LPCTSTR)pObj->m_strPlayFile;
					BOOL bError  = DJSS_PlayFile( nIndex, pszFileName );
					if( !bError )//false
					{
						m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
						DJSS_ResetUser( nAimIndex );
						pChnlObj->m_nLinkCh = -1;
						DJSS_StartPlaySignal(nIndex,SIG_BUSY1);
						m_strMSG.Format("Switch Error; ChIndex = %d   Play file Error", nIndex );
						m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
						DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
				}
			}
			
			int nCallOutResult = DJSS_MyMakeCall_Check( nAimIndex, pObj->m_nTimeOut );
			if( 3 == nCallOutResult )//未得到结果
				return;
			else if( 0 == nCallOutResult )
			{
				//成功
				if( 2== pObj->m_nWaitOption ) DJSS_PlayFileStop( nIndex );
				BOOL bResult =  DJSS_MySetLink( nIndex, nAimIndex , CF_CHSWITCH_CONNECT );
				if( bResult )//成功
				{
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
					return;
				}
				else
				{
					m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
					DJSS_ResetUser( nAimIndex );
					pChnlObj->m_nLinkCh = -1;
					DJSS_StartPlaySignal(nIndex,SIG_BUSY1);
					m_strMSG.Format("%s SetLink Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				return;
			}
			else
			{
				//超时或失败	
				m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
				DJSS_ResetUser( nAimIndex );
				pChnlObj->m_nLinkCh = -1;
				DJSS_StartPlaySignal(nIndex,SIG_BUSY1);
				m_strMSG.Format("%s TimeOut Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}


		}
		break;
	default:
		break;
	}

		
	return;
}

void CDJSS17::DJSS_Switch_CallAgent(int nIndex, CSwitchObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nAimIndex	= pChnlObj->m_nLinkCh ;
	int nStep		= pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			CString strCallTel("");
			int nVarKey = pObj->GetVarKey( pObj->m_strAimChnl );
			if( nVarKey < 0 )
			{
				strCallTel = pObj->m_strAimChnl;
			}
			else if( !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strCallTel, nVarKey) )
			{
				m_strMSG.Format("%s not get aim telephone; ChIndex = %d ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

			nAimIndex = DJSS_FindFreeChannel( strCallTel, CF_CHTYPE_USER );
			if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
			{
				m_strMSG.Format("%s have not free channel ; ChIndex = %d ",pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

			if( CF_CHTYPE_USER != m_pChannelInfo[nAimIndex].m_nChanelType )
			{
				m_strMSG.Format("%s Aim Channel not is User channel, index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			pChnlObj->m_nLinkCh = nAimIndex;
			//呼叫分机空闲，对其振铃
			m_pChannelInfo[nAimIndex].m_nLinkCh = nIndex;

		
			//switch的等待选项，一般只用于呼叫时用
			if( 0 == pObj->m_nWaitOption )//播放回铃音
			{
				DJSS_StartPlaySignal(nIndex, SIG_RINGBACK);
			}
			else if( 2 == pObj->m_nWaitOption )//播放语音文件
			{
				CString strFile("");
				BOOL bResult = m_pPubCObj->PC_GetExpressValues( pChnlObj,  strFile, pObj->m_strPlayFile);
				if( !bResult )
				{
					m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
					DJSS_ResetUser( nAimIndex );
					pChnlObj->m_nLinkCh = -1;
					DJSS_StartPlaySignal(nIndex,SIG_BUSY1);
					m_strMSG.Format("Switch Error; ChIndex = %d   Get Play file Error", nIndex );
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				char *pszFileName = (LPTSTR)(LPCTSTR)strFile;
				BOOL bError  = DJSS_PlayFile( nIndex, pszFileName );
				if( !bError )//false
				{
					m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
					DJSS_ResetUser( nAimIndex );
					pChnlObj->m_nLinkCh = -1;
					DJSS_StartPlaySignal(nIndex,SIG_BUSY1);
					m_strMSG.Format("Switch Error; ChIndex = %d   Play file Error", nIndex );
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
			}
			
			if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
			{
				DJSS_SendCallerID_DTMF( nAimIndex , (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN );
				
			}
			else
			{
				DJSS_SendCallerID_DTMF( nAimIndex , (LPTSTR)(LPCTSTR)pChnlObj->m_strCallerID );
				
			}
			pChnlObj->m_nStep = 1;
			
		}
		break;
	case 1:
		{
			if( DJSS_MyCheckSendEnd( nAimIndex ) )
			{
				if( CF_CHTYPE_USER == pChnlObj->m_nChanelType )
				{
					DJSS_MyMakeCall( nAimIndex, (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN, (LPTSTR)(LPCTSTR)pChnlObj->m_strCalledID );
				}
				else
				{
					DJSS_MyMakeCall( nAimIndex, (LPTSTR)(LPCTSTR)pChnlObj->m_strCalledID, (LPTSTR)(LPCTSTR)pChnlObj->m_strCallerID );
				}
				m_pPubCObj->PC_StartTimer(pChnlObj);
				pChnlObj->m_nStep = 2;
			}
		}
		break;
	case 2:
		{
			if( 2 == pObj->m_nWaitOption )//switch的等待选项，一般只用于呼叫时用
			{
				if( DJSS_CheckPlayFileEnd(nIndex) )//播放语音文件
				{
					DJSS_PlayFileStop( nIndex );
					CString strFile("");
					BOOL bResult = m_pPubCObj->PC_GetExpressValues( pChnlObj,  strFile, pObj->m_strPlayFile);
					if( !bResult )
					{
						m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
						DJSS_ResetUser( nAimIndex );
						pChnlObj->m_nLinkCh = -1;
						DJSS_StartPlaySignal(nIndex,SIG_BUSY1);
						m_strMSG.Format("Switch Error; ChIndex = %d   Get Play file Error", nIndex );
						m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
						DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
					char *pszFileName = (LPTSTR)(LPCTSTR)pObj->m_strPlayFile;
					BOOL bError  = DJSS_PlayFile( nIndex, pszFileName );
					if( !bError )//false
					{
						m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
						DJSS_ResetUser( nAimIndex );
						pChnlObj->m_nLinkCh = -1;
						DJSS_StartPlaySignal(nIndex,SIG_BUSY1);
						m_strMSG.Format("Switch Error; ChIndex = %d   Play file Error", nIndex );
						m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
						DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
				}
			}
			
			int nCallOutResult = DJSS_MyMakeCall_Check( nAimIndex, pObj->m_nTimeOut );
			if( 3 == nCallOutResult )//未得到结果
				return;
			else if( 0 == nCallOutResult )
			{
				//成功
				if( 2== pObj->m_nWaitOption ) DJSS_PlayFileStop( nIndex );
				BOOL bResult =  DJSS_MySetLink( nIndex, nAimIndex , CF_CHSWITCH_CONNECT );
				if( bResult )//成功
				{
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
					return;
				}
				else
				{
					m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
					DJSS_ResetUser( nAimIndex );
					pChnlObj->m_nLinkCh = -1;
					DJSS_StartPlaySignal(nIndex,SIG_BUSY1);
					m_strMSG.Format("%s SetLink Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				return;
			}
			else
			{
				//超时或失败	
				m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
				DJSS_ResetUser( nAimIndex );
				pChnlObj->m_nLinkCh = -1;
				DJSS_StartPlaySignal(nIndex,SIG_BUSY1);
				m_strMSG.Format("%s TimeOut Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}


		}
		break;
	default:
		break;
	}

		
	return;
}




////////////////////////////////////
//SIG_STOP 停止播放信号音
//SIG_DIALTONE 拨号音
//SIG_BUSY1 忙音一
//SIG_BUSY2 忙音二
//SIG_RINGBACK 回铃音
/////////////////////////////////////
void CDJSS17::DJSS_StartPlaySignal(int nIndex, int nSignalType)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nSS7Chnl	= pChnlObj->m_nChannel;//取得通道号
	int nChnlType   = pChnlObj->m_nChanelType;
	switch( nChnlType )
	{
	case CF_CHTYPE_DTNO1://数字中继中国一号信令 
	case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
	case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
	case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
	case CF_CHTYPE_MTP3://七号信令MTP传输层通道
		DJVoc_StopPlayFile(nSS7Chnl);
		DJExg_ClearLinkPlayVoiceFromTrunk( nSS7Chnl );
		if( SIG_DIALTONE == nSignalType )// 拨号音
			DJExg_SetLinkPlayVoiceToTrunk(nSS7Chnl, m_nVocTone);
		else if( SIG_BUSY1 == nSignalType )// 忙音一
			DJExg_SetLinkPlayVoiceToTrunk(nSS7Chnl, m_nVocBusy);
		else if( SIG_BUSY2 == nSignalType )// 忙音二
			DJExg_SetLinkPlayVoiceToTrunk(nSS7Chnl, m_nVocBusy);
		else if( SIG_RINGBACK == nSignalType )// 回铃音
			DJExg_SetLinkPlayVoiceToTrunk(nSS7Chnl, m_nVocRingBack);
		break;
	case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
	case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
	case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
		StartPlaySignal(nSS7Chnl,nSignalType);
		break;
	case CF_CHTYPE_VIP://VOIP通道类型
	case CF_CHTYPE_NOKNOW://未知通道类型 
	case CF_CHTYPE_EMPTY://空通道 
	default:
		break;
	}

	return;
}

void CDJSS17::DJSS_Switch_CallCustomer(int nIndex, CSwitchObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nAimIndex	= pChnlObj->m_nLinkCh ;
	int nStep		= pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			CString strCallTel("");
			int nVarKey = pObj->GetVarKey( pObj->m_strAimChnl );
			if( nVarKey < 0 )
			{
				strCallTel = pObj->m_strAimChnl;
			}
			else if( !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strCallTel, nVarKey) )
			{
				m_strMSG.Format("%s not get aim telephone; ChIndex = %d ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
					m_strMSG.Format("%s Get Begin Channel error ; ChIndex = %d ",pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;

				}
				nBeginCH = atoi(strNewExpress);
				if(  !m_pPubCObj->PC_GetExpressValues( pChnlObj,  strNewExpress, pObj->m_strEndCH ) )
				{
					m_strMSG.Format("%s Get End Channel error ; ChIndex = %d ",pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;

				}
				nEndCH = atoi(strNewExpress);
				nAimIndex = DJSS_FindFreeTrunkChnl( nBeginCH, nEndCH );
			}
			else
			{
				nAimIndex = DJSS_FindFreeTrunkChnl();
			}

			if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
			{
				m_strMSG.Format("%s have not free channel ; ChIndex = %d ",pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			
			if( !DJSS_MyMakeCall( nAimIndex, (LPTSTR)(LPCTSTR)strCallTel, (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN) )
			{
				
				m_strMSG.Format("%s Call out fail ,GErrCode=%d ; ChIndex = %d ",pObj->m_strCaption, DJSys_GetErrCode(), nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			m_pChannelInfo[ nAimIndex ].m_nLinkCh = nIndex;
			m_pChannelInfo[nAimIndex].m_strDialTel		= strCallTel;
			m_pChannelInfo[nAimIndex].m_nDialType		= 1;//1外拨
			
			pChnlObj->m_strDialTel		= strCallTel;
			pChnlObj->m_nDialType		= 1;//1外拨
			pChnlObj->m_nLinkCh = nAimIndex;

			//switch的等待选项，一般只用于呼叫时用
			if( 0 == pObj->m_nWaitOption )//播放回铃音
			{
				DJSS_StartPlaySignal(nIndex, SIG_RINGBACK);
			}
			else if( 2 == pObj->m_nWaitOption )//播放语音文件
			{
				char *pszFileName = (LPTSTR)(LPCTSTR)pObj->m_strPlayFile;
				BOOL bError  = DJSS_PlayFile( nIndex, pszFileName );
				if( !bError )//false
				{
					m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
					DJSS_ResetTrunk( nAimIndex );
					pChnlObj->m_nLinkCh = -1;
					DJSS_StartPlaySignal(nIndex,SIG_BUSY1);
					m_strMSG.Format("Switch Error; ChIndex = %d   Play file Error", nIndex );
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
				if( DJSS_CheckPlayFileEnd(nIndex) )//播放语音文件
				{
					DJSS_PlayFileStop( nIndex );
					char *pszFileName = (LPTSTR)(LPCTSTR)pObj->m_strPlayFile;
					BOOL bError  = DJSS_PlayFile( nIndex, pszFileName );
					if( !bError )//false
					{
						m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
						DJSS_ResetTrunk( nAimIndex );
						pChnlObj->m_nLinkCh = -1;
						DJSS_StartPlaySignal(nIndex,SIG_BUSY1);
						m_strMSG.Format("Switch Error; ChIndex = %d   Play file Error", nIndex );
						m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
						DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
						return;
					}
				}
			}

			int nCallOutResult = DJSS_MyMakeCall_Check( nAimIndex , pObj->m_nTimeOut );
			if( 3 == nCallOutResult )//未得到结果
				return;
			else if( 0 == nCallOutResult )
			{
				//成功
				DJSS_MySetLink( nIndex, nAimIndex, CF_CHSWITCH_CONNECT );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				return;
			}
			else
			{
				//超时或失败	
				m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
				DJSS_ResetTrunk(nAimIndex);
				m_strMSG.Format("%s Call out fail ,timeout or no answer etc.; ChIndex = %d ",pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}	
		}
		break;
	default:
		break;
	}
	

	return;

}

BOOL CDJSS17::DJSS_MyMakeCall(int nIndex, char *szCalledID, char *szCallerID)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	m_pPubCObj->PC_StartTimer(pChnlObj);
	BOOL bResult = FALSE;
	int nSS7Chnl = pChnlObj->m_nChannel;
	int nType = pChnlObj->m_nChanelType;


	pChnlObj->m_strCallerID = szCallerID ;
	pChnlObj->m_strCalledID = szCalledID ;

	if( CF_CHTYPE_USER == nType)
	{
		//呼叫分机空闲，对其振铃
		FeedRealRing(nSS7Chnl);
		pChnlObj->m_nState		= CHANNEL_STATE_RINGING;
		pChnlObj->m_nEventType =  EVENTTYPE_RINGING;
		pChnlObj->m_nCallType = CHANNEL_CALLTYPE_COMEIN;
		m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");

		m_strMSG.Format("[Send Ringing DN:%s][CallerID:%s][CalledID:%s]", pChnlObj->m_strLocalDN, szCallerID ,szCalledID );
		m_pPubCObj->PC_ShowCommMsg( m_strMSG );
		CTI_RingingMsg( atoi(pChnlObj->m_strLocalDN), szCallerID, szCalledID );

		m_pPubCObj->PC_ShowChannelCallerID(nIndex,pChnlObj->m_strCallerID);
		m_pPubCObj->PC_ShowChannelCalledID(nIndex,pChnlObj->m_strCalledID);
		return TRUE;
	}
	else{
		pChnlObj->m_nCallType = CHANNEL_CALLTYPE_CALLOUT;
		BYTE cb = DJSys_GetPcmMode ( nSS7Chnl / 30 );
		switch ( cb ) {
		case PCM_WORK_MODE_NONE:		// "7号(无信令)"
		case PCM_WORK_MODE_SS7:			// "7号"
			if( CF_CHTYPE_DTNO7 == nType ) 
			{
				if( CH_FREE != DJNo7_GetChnState(nSS7Chnl/30,nSS7Chnl%30) ) return FALSE;
				DJTrk_SetTrunkType(nSS7Chnl,Type_Export);
				bResult = DJNo7_Callout( nSS7Chnl/30,nSS7Chnl%30, szCalledID, (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN ) ;
			}
			else if( CF_CHTYPE_DTNO7ISUP == nType ) 
			{
				if( ISUP_CH_FREE != DJISUP_GetChnState(nSS7Chnl/30,nSS7Chnl%30) )  return FALSE;
				DJTrk_SetTrunkType(nSS7Chnl,Type_Export);
				bResult = DJISUP_Callout( nSS7Chnl/30,nSS7Chnl%30, szCalledID, (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN ) ;
			}
			break;
		case PCM_WORK_MODE_SS1:			// "1号"
			if( DJTrk_CheckTrunkFree(nSS7Chnl) && !DJTrk_CheckTrunkIn( nSS7Chnl )  ) return FALSE;
			DJTrk_SetTrunkType(nSS7Chnl,Type_Export);
			bResult = DJTrk_StartDial( nSS7Chnl, szCalledID, pChnlObj->m_strLocalDN ) ;
			break;
		case PCM_WORK_MODE_DSS1:		// "数字1号(ISDN PRI)"
			if( ISDN_CH_FREE  != DJISDN_GetChnState( nSS7Chnl/30,nSS7Chnl%30 ) ) return FALSE;
			DJTrk_SetTrunkType(nSS7Chnl,Type_Export);
			bResult = DJISDN_Callout( nSS7Chnl/30,nSS7Chnl%30, szCalledID, "", (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN, "" ) ;
			break;
		default:
			break;
		}
		
		if( !bResult )
		{
			DJTrk_SetTrunkType(nSS7Chnl,Type_Import);
		}
		else
		{
			DJSS_InitDtmfBuf(nIndex);
			m_pPubCObj->PC_StartTimer(pChnlObj);
			pChnlObj->m_strDTMFRecived	= "";
			pChnlObj->m_nState			= CHANNEL_STATE_DIAL;
			pChnlObj->m_nEventType		=  EVENTTYPE_OFFHOOK;
			m_pPubCObj->PC_ShowChannelState( nIndex, "Dial" );
		}

		m_pPubCObj->PC_ShowChannelCallerID(nIndex,pChnlObj->m_strCallerID);
		m_pPubCObj->PC_ShowChannelCalledID(nIndex,pChnlObj->m_strCalledID);
		return bResult;

	}

}



void CDJSS17::DJSS_Switch(int nIndex )
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CSwitchObj *pObj = (CSwitchObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CSwitchObj) ) )
	{
		m_strMSG.Format("Switch Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	
	DJSS_TransferKey(nIndex);

	int nSwitchType = pObj->m_nSwitchType;
	switch( nSwitchType )
	{
		case CF_CHSWITCH_CONNECT://直接连接
		case CF_CHSWITCH_MONITOR://监听
		case CF_CHSWITCH_SPEAK://别人能听见，但听不见别人
		case CF_CHSWITCH_DISCONNECT://断开连接	
			if( 0 == pObj->m_nAimType )
				DJSS_Switch_Self( nIndex,  pObj );
			else
				DJSS_Switch_Aim( nIndex,  pObj );
			break;
		case CF_CHSWITCH_CALLAGENT://呼叫坐席，这时的通道号为分机号码
			if( 3 == pObj->m_nAimType )//呼叫空闲坐席
			{
				DJSS_Switch_CallFreeAgent( nIndex,  pObj );
			}
			else
			{
				DJSS_Switch_CallAgent( nIndex,  pObj );
			}
			break;
		case CF_CHSWITCH_CALLCUSTOMER://呼叫客户
			DJSS_Switch_CallCustomer( nIndex,  pObj );
			break;
		default:
			break;
	}


	return;
}
/*************************************************************
	对目前与自己相关联的通道进行交换。
	对目前与自己相关联的通道进行交换（Connect、Disconnect 、
	Speak 和Monitor）操作，这时Aim channel Type(目标通道类型)
	选项无效，同时Switch Type（交换类型）的Call Agent(呼叫分
	机)和Call Customer（呼叫客户）无效。常量$9:LinkChIndex里保
	存的是目前与自己相关联的通道编号，如果$9:LinkChIndex = -1
	时，表明当前没有与自己关联的通道，返回错误。
*************************************************************/
void CDJSS17::DJSS_Switch_Self(int nIndex, CSwitchObj *pObj)
{
	if( nIndex < 0 || nIndex >= m_nTotalChannel  ) return;
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nAimIndex = pChnlObj->m_nLinkCh;
	if( nAimIndex < 0 || nAimIndex >= m_nTotalChannel  )
	{
		m_strMSG.Format("%s have not the Channel that link to self . index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	int nSwitchType = pObj->m_nSwitchType;
	switch( nSwitchType )
	{
		case CF_CHSWITCH_CONNECT://直接连接
		case CF_CHSWITCH_MONITOR://监听
		case CF_CHSWITCH_SPEAK://别人能听见，但听不见别人
			if( DJSS_MySetLink(nIndex, nAimIndex, pObj->m_nSwitchType  ) )
			{
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				return;
			}
			else
			{
				m_strMSG.Format("%s Link Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			break;
		case CF_CHSWITCH_DISCONNECT://断开连接	
			if( DJSS_MyClearLink(nIndex, nAimIndex ) )
			{
				pChnlObj->m_nLinkType = -1;
				m_pChannelInfo[nAimIndex].m_nLinkType = -1;
				m_pPubCObj->PC_ShowConnectToChnl(nIndex,-1);
				m_pPubCObj->PC_ShowConnectToChnl(nAimIndex,-1);
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
			else
			{
				m_strMSG.Format("%s Link Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}
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
void CDJSS17::DJSS_Switch_Aim(int nIndex, CSwitchObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nVarKey = pObj->GetVarKey( pObj->m_strAimChnl );
	if(  nVarKey < 0 )
	{
		m_strMSG = pObj->m_strAimChnl;
	}
	else if( !m_pPubCObj->PC_GetNorVarValues(pChnlObj, m_strMSG, nVarKey ) )
	{	
		m_strMSG.Format("%s Channel index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	int nAimIndex = atoi(m_strMSG);
	if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
	{
		m_strMSG.Format("%s Channel index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	int nSwitchType = pObj->m_nSwitchType;
	switch( nSwitchType )
	{
		case CF_CHSWITCH_CONNECT://直接连接
		case CF_CHSWITCH_MONITOR://监听
		case CF_CHSWITCH_SPEAK://别人能听见，但听不见别人
			if( DJSS_MySetLink(nIndex, nAimIndex, pObj->m_nSwitchType  ) )
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
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				return;
			}
			else
			{
				m_strMSG.Format("%s Link Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			break;
		case CF_CHSWITCH_DISCONNECT://断开连接	
			if( DJSS_MyClearLink(nIndex, nAimIndex ) )
			{
				pChnlObj->m_nLinkType = -1;
				m_pChannelInfo[nAimIndex].m_nLinkType = -1;
				m_pPubCObj->PC_ShowConnectToChnl(nIndex,-1);
				m_pPubCObj->PC_ShowConnectToChnl(nAimIndex,-1);
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
			else
			{
				m_strMSG.Format("%s Channel index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}
			break;
		default:
			break;
	}
	return;	
}


int CDJSS17::DJSS_FindChannel(CString strLocalDN)
{
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( strLocalDN == m_pChannelInfo[i].m_strLocalDN )
		{
			return i;
		}
	}
	return -1;
}

void CDJSS17::DJSS_Transfer_ToAgent(int nIndex,CTransferCheckObj *pObj )
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlInfo->m_nChanelType ) return;
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
				m_strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}
			if( pObj->m_bClearDTMF )
			{
				DJSS_InitDtmfBuf( nIndex );
				pChnlInfo->m_strDTMFRecived = "";
			}

			//转移到内线通道
			CString strPhone	= pObj->m_strExpression;
			int nKey = pObj->GetVarKey(strPhone);
			if(  !m_pPubCObj->PC_GetNorVarValues(pChnlInfo,  strPhone, nKey ) )
			{
				m_strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}
	
			//已取得电话号码
			nAimIndex = DJSS_FindFreeChannel( strPhone, CF_CHTYPE_USER  );//通过分机号码查找该分机
			if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
			{
				//如果该通道不是内线，认为是错误的
				m_strMSG.Format("%s Channel index is error; ChIndex = %d ",  pObj->m_strCaption, nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}

			//呼叫分机空闲，对其振铃
			pChnlInfo->m_nTransferCh	= nAimIndex;
			m_pChannelInfo[nAimIndex].m_nLinkCh		= nIndex;
			DJSS_SendCallerID_DTMF( nAimIndex , (LPTSTR)(LPCTSTR)pChnlInfo->m_strCallerID );//发送主叫
			DJSS_StartPlaySignal(nIndex, SIG_RINGBACK);
			m_pPubCObj->PC_ShowChannelState( nAimIndex, "Ringing");	
			pChnlInfo->m_nStep = 1;
			
		}
		break;
	case 1:
		{
			nAimIndex = pChnlInfo->m_nTransferCh;
			if( DJSS_MyCheckSendEnd( nAimIndex ) )
			{
				DJSS_MyMakeCall( nAimIndex, (LPTSTR)(LPCTSTR)pChnlInfo->m_strCalledID, (LPTSTR)(LPCTSTR)pChnlInfo->m_strCallerID );
				pChnlInfo->m_nStep = 2;
			}
		}
		break;
	case 2:
		{
			//检测内线结果
			nAimIndex = pChnlInfo->m_nTransferCh;
			int nCallOutResult = DJSS_MyMakeCall_Check( nAimIndex );
			if( 3 == nCallOutResult )//未得到结果
			{
				//收到取消转移命令
				BOOL bCancel = FALSE;
				if( EVENTTYPE_AGENT_CANCEL_TRANSFER == pChnlInfo->m_nEventType )
					bCancel = TRUE;
				else
				{
					//看是否收到取消转移的按键
					if( DJSS_GetDtmfCode( nIndex , pChnlInfo->m_strDTMFRecived ) > 0 )
					{
						m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlInfo->m_strDTMFRecived );
						if(  pChnlInfo->m_strDTMFRecived == pObj->m_strCancelDTMF  )
						{
							bCancel = TRUE;
						}
					}
				}
				if( bCancel )
				{
					//满足取消转移条件
					m_pChannelInfo[nAimIndex].m_nLinkCh	= -1;
					DJSS_ResetUser( nAimIndex );
					pChnlInfo->m_nTransferCh = -1;

					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_CANCEL_TRANSFER );//取消转移
				}
				return;
			}
			else if( 0 == nCallOutResult )
			{
				//成功

				if( 1 == pObj->m_nWaitOption )
					DJSS_PlayFileStop( nLinkIndex );
				m_pChannelInfo[nAimIndex].m_nTransferCh		= -1;
				m_pChannelInfo[nAimIndex].m_nLinkCh			= nLinkIndex;

				DJSS_MySetLink( nLinkIndex, nAimIndex , CF_CHSWITCH_CONNECT );
				m_pChannelInfo[nLinkIndex].m_nState			= CHANNEL_STATE_CONNECT;
				m_pChannelInfo[nLinkIndex].m_nTransferCh	= -1;
				m_pChannelInfo[nLinkIndex].m_nLinkCh		= nAimIndex;

				pChnlInfo->m_nTransferCh		= -1;
				pChnlInfo->m_nLinkCh			= -1;
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				CTI_TransferMsg( nDN, 0, "" );
				return;
			}
			else
			{
				//超时或失败
				m_pChannelInfo[nAimIndex].m_nLinkCh	= -1;
				DJSS_ResetUser( nAimIndex );
				pChnlInfo->m_nTransferCh = -1;
				m_strMSG.Format("%s TimeOut Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );//超时，没有应答
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

void CDJSS17::DJSS_Transfer_ToCustomer(int nIndex,CTransferCheckObj *pObj  )
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) return;
	int nStep = pChnlObj->m_nStep;
	int nLinkIndex =  pChnlObj->m_nLinkCh;
	int nDN = atoi( pChnlObj->m_strLocalDN );
	switch( nStep )
	{
	case 0:
		if( nLinkIndex <= -1 )
		{
			m_strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			CTI_TransferMsg( nDN, 1, "" );//执行错误
			return;
		}
		if( pObj->m_bClearDTMF )
		{
			DJSS_InitDtmfBuf( nIndex );
			pChnlObj->m_strDTMFRecived = "";
		}
		pChnlObj->m_nStep = 1;
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
				m_strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}
			
			//已取得电话号码
			int nAimIndex = DJSS_FindFreeTrunkChnl();
			if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
			{
				m_strMSG.Format("%s have not free channel ; ChIndex = %d ",pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}

			if( !DJSS_MyMakeCall( nAimIndex, (LPSTR)(LPCSTR)strPhone, (LPTSTR)(LPCTSTR)pChnlObj->m_strLocalDN ) )
			{
				m_strMSG.Format("%s Call out fail ,GErrCode=%d ; ChIndex = %d ",pObj->m_strCaption, DJSys_GetErrCode(), nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				CTI_TransferMsg( nDN, 1, "" );//执行错误
				return;
			}

			pChnlObj->m_nTransferCh	= nAimIndex;
			m_pChannelInfo[nAimIndex].m_nLinkCh = nIndex;
			DJSS_StartPlaySignal(nIndex, SIG_RINGBACK);
			m_pPubCObj->PC_StartTimer(pChnlObj);
			pChnlObj->m_nStep = 2;

		}
		break;
	case 2:
		{
			int nAimIndex = pChnlObj->m_nTransferCh;
			int nCallOutResult = DJSS_MyMakeCall_Check( nAimIndex );
			if( 3 == nCallOutResult )//未得到结果
			{
				//收到取消转移命令
				BOOL bCancel = FALSE;
				if( EVENTTYPE_AGENT_CANCEL_TRANSFER == pChnlObj->m_nEventType )
					bCancel = TRUE;
				else
				{
					//看是否收到取消转移的按键
					if( DJSS_GetDtmfCode( nIndex , pChnlObj->m_strDTMFRecived ) > 0 )
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
					DJSS_ResetTrunk(nAimIndex);
					pChnlObj->m_nTransferCh		= -1;
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_CANCEL_TRANSFER );//取消转移
				}
				return;
			}
			else if( 0 == nCallOutResult )
			{
				//成功
				if( 1 == pObj->m_nWaitOption )
					DJSS_PlayFileStop( nLinkIndex );
				DJSS_MySetLink( nLinkIndex, nAimIndex , CF_CHSWITCH_CONNECT );

				m_pChannelInfo[nAimIndex].m_nTransferCh		= -1;
				m_pChannelInfo[nAimIndex].m_nLinkCh			= nLinkIndex;

				m_pChannelInfo[nLinkIndex].m_nState = CHANNEL_STATE_CONNECT;
				m_pChannelInfo[nLinkIndex].m_nTransferCh	= -1;
				m_pChannelInfo[nLinkIndex].m_nLinkCh		= nAimIndex;

				pChnlObj->m_nTransferCh		= -1;
				pChnlObj->m_nLinkCh			= -1;
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				CTI_TransferMsg( nDN, 0, "" );
				return;
			}
			else
			{
				//超时或失败
				m_pChannelInfo[nAimIndex].m_nLinkCh	= -1;
				DJSS_ResetTrunk(nAimIndex);
				pChnlObj->m_nTransferCh		= -1;

				m_strMSG.Format("%s Call out fail ; ChIndex = %d ",pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
void CDJSS17::DJSS_Transfer_Start(int nIndex, CTransferCheckObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) return;
	int nLinkIndex =  pChnlObj->m_nLinkCh;
	int nDN = atoi( pChnlObj->m_strLocalDN );
	if( nLinkIndex <= -1 )
	{
		m_strMSG.Format("%s Have not active call ;ChIndex = %d", pObj->m_strCaption, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		CTI_TransferMsg( nDN, 1, "" );//执行错误
		return;
	}

	DJSS_MyClearLink(nIndex,nLinkIndex );
	if( 1 == pObj->m_nWaitOption )
	{
		//播放语音

		char *pszFileName = (LPTSTR)(LPCTSTR)pObj->m_strPlayFile;
		DJSS_PlayFile( nLinkIndex, pszFileName);
		m_pChannelInfo[nLinkIndex].m_strPlayFile = pObj->m_strPlayFile;
	}
	if( pObj->m_bClearDTMF )
	{
		DJSS_InitDtmfBuf( nIndex );
		pChnlObj->m_strDTMFRecived = "";
	}
	m_pChannelInfo[nLinkIndex].m_nState = CHANNEL_STATE_TRANSFER;
	m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Busy" );
	DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);


	return;
}

void CDJSS17::DJSS_Transfer_Cancel(int nIndex, CTransferCheckObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nLinkIndex =  pChnlObj->m_nLinkCh;
	if( nLinkIndex < 0 || nLinkIndex >= m_nTotalChannel  )
	{
		m_strMSG.Format("%s Have not active call ;ChIndex = %d", pObj->m_strCaption, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		CTI_CancelTransferMsg( atoi(pChnlObj->m_strLocalDN), 1, "" );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	if( pObj->m_bClearDTMF )
	{
		DJSS_InitDtmfBuf( nIndex );
		pChnlObj->m_strDTMFRecived = "";
	}

	m_pChannelInfo[nLinkIndex].m_nState = CHANNEL_STATE_CONNECT;
	m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Busy");
	DJSS_PlayFileStop( nLinkIndex );
	DJSS_PlayFileStop( nIndex );
	if( !DJSS_MySetLink( nIndex, nLinkIndex , CF_CHSWITCH_CONNECT ) )
	{
		m_strMSG.Format("Cancel transfer error : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		CTI_CancelTransferMsg( atoi(pChnlObj->m_strLocalDN), 1, "" );
		return;
	}
	CTI_CancelTransferMsg( atoi(pChnlObj->m_strLocalDN), 0, "" );
	DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	return;
}
void CDJSS17::DJSS_Transfer(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlObj->m_nChanelType ) return;
	CTransferCheckObj *pObj = (CTransferCheckObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CTransferCheckObj) ) )
	{
		m_strMSG.Format("TransferStart Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	switch( pObj->m_nTransferType )
	{
	case 0://开始转移
		DJSS_Transfer_Start( nIndex ,pObj );
		break;
	case 1://取消转移
		DJSS_Transfer_Cancel( nIndex ,pObj );
		break;
	case 2://完成转移
		{
				switch( pObj->m_nTransferAim )
				{
				case 0:
					DJSS_Transfer_ToAgent( nIndex ,pObj );
					break;
				case 1:
					DJSS_Transfer_ToCustomer( nIndex  ,pObj );
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

///////////////////////////////////
//释放会议资源
//
///////////////////////////////////
void CDJSS17::DJSS_DJConf_ReleaseConf()
{
	//如果初始化语音卡成功
	if( 1 == m_nConfCard )
	{
		//释放DCB2560会议
		for(int i=0;i< m_nTotalConf;i++)
			DJConf_FreeConfGroup(i);//删除会议，并释放释放占用的会议资源
		DJConf_ReleaseConfCardNew();//关闭会议板卡，释放资源
	}
	else
	{
		//释放DCB320会议
		for(int i=0;i< m_nTotalConf;i++)
			DJConf_DestroyGroup(i);//删除会议，并释放释放占用的会议资源
		DJConf_ReleaseConfCard();//关闭会议板卡，释放资源

	}

	return;
}

void CDJSS17::DJSS_DJConf_InitConfCard()
{
	if(  1 == m_nConfCard )
	{
		//有D2560 会议卡
		if( 0 == DJConf_InitConfCardNew() )
		{
			CONF_SYS_INFO_NEW confsysinf;//会议参数信息结构
			DJConf_GetSysInfoNew(&confsysinf);//取得会议参数信息
			m_nTotalConf	= confsysinf.wGroupNum;//取得最大会议组数		
			m_nTotalConfResource = confsysinf.wCardNum*64*4;//计算总的会议资源数
			m_pConference = new CONF_STRUCT[m_nTotalConf];

			m_plstConfer->DeleteAllItems();
			for( int i = 0; i < m_nTotalConf; i++ )
			{
				m_pConference[ i ].State		= CONF_FREE;
				m_pConference[ i ].nChnlIndex	= -1;
				m_pConference[ i ].nConfPlayID	= -1;
				m_pConference[ i ].nConfRecordID = -1;
				m_pConference[ i ].nVocID = -1;
				m_pConference[ i ].nRecID = -1;
				m_pConference[ i ].nMemberNum	= 0;
				m_pConference[ i ].nResNum		= 0;
				m_pConference[ i ].nListenNum	= 0;

				m_strMSG.Format("%d",i);
				m_plstConfer->InsertItem(i,m_strMSG );
				m_strMSG.Format( "%d",m_nTotalConfResource );
				m_plstConfer->SetItemText(i,1,m_strMSG);
				m_plstConfer->SetItemText(i,2,"0");
				m_plstConfer->SetItemText(i,3,"0");

			}
		}

	}
	else
	{
		//DCB320会议
		DJConf_Enable_Mass();
		DJConf_InitConfCard();
		CONF_SYS_INFO confsysinf;
		DJConf_GetSysInfo( &confsysinf );
		m_nTotalConf	= confsysinf.wGroupNum;//取得最大会议组数		
		m_nTotalConfResource = confsysinf.wCardNum*64*4;//计算总的会议资源数
		m_pConference = new CONF_STRUCT[m_nTotalConf];
		m_plstConfer->DeleteAllItems();
		for( int i = 0; i < m_nTotalConf; i++ )
		{
			m_pConference[ i ].State		= CONF_FREE;
			m_pConference[ i ].nChnlIndex	= -1;
			m_pConference[ i ].nConfPlayID	= -1;
			m_pConference[ i ].nConfRecordID = -1;
			m_pConference[ i ].nVocID = -1;
			m_pConference[ i ].nRecID = -1;
			m_pConference[ i ].nMemberNum	= 0;
			m_pConference[ i ].nResNum		= 0;
			m_pConference[ i ].nListenNum	= 0;

			m_strMSG.Format("%d",i);
			m_plstConfer->InsertItem(i,m_strMSG );
			m_strMSG.Format( "%d",m_nTotalConfResource );
			m_plstConfer->SetItemText(i,1,m_strMSG);
			m_plstConfer->SetItemText(i,2,"0");
			m_plstConfer->SetItemText(i,3,"0");

		}

	}
}

BOOL CDJSS17::DJSS_CreateAndEnterConfGroup(int nIndex, int nGroupID,int nMaxMember )
{
	if( 1 == m_nConfCard )
	{
		//有D2560 会议卡
		int nResult = DJConf_CreateConfGroupEx( nGroupID, nMaxMember,10, 4,3,1 );//使用抢占模式
		if( nResult >=0 )
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
		
	}
	else
	{
		//DCB320会议
		return TRUE;
	}

}

///////////////////////////////////////////////////
//该通道创建，不加入会议
//同时将成功创建的会议编号保存在nConferNO中
//
//////////////////////////////////////////////////
void CDJSS17::DJSS_ConferenceBuild(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nConferNO = -1;
	int nMaxMember = 0;
	CConferenceBulidObj *pObj = (CConferenceBulidObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CConferenceBulidObj) ) )
	{
		m_strMSG.Format("ConferenceBuild error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}


	if( 0 == pObj->m_nBuildType )//由系统自动查找空闲会议组
	{
		nConferNO = DJSS_FindFreeConferIndex();
	}
	else
	{
		//从变量取得会议组
		int nKey = pObj->GetVarKey( pObj->m_strConferNo );
		CString strConferNO("");
		if( -1 == nKey && "" != pObj->m_strConferNo )
		{
			strConferNO = pObj->m_strConferNo;
		}
		else
		{
			if( !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strConferNO, nKey ) )
			{
				m_strMSG.Format("ConferenceBuild error : ChIndex = %d   Error = have not free conference resource", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
		}
		nConferNO = atoi(strConferNO);
	}

	if(  nConferNO <= 0 || nConferNO >= m_nTotalConf )
	{
		m_strMSG.Format("ConferenceBuild error : ChIndex = %d   Error = Input conference  error ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	int nKey = pObj->GetVarKey( pObj->m_strMaxMember );
	CString strMaxMember("");
	if( -1 == nKey )
	{
		strMaxMember =  pObj->m_strMaxMember;
	}
	else
	{
		if( !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strMaxMember, nKey ) )
		{
			m_strMSG.Format("ConferenceBuild error : ChIndex = %d   Error = have not free conference resource", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
	}
	nMaxMember = atoi(strMaxMember);
	if( !DJSS_CreateAndEnterConfGroup( nIndex, nConferNO,nMaxMember ) )
	{
		m_strMSG.Format("ConferenceBuild error : ChIndex = %d   Error = Build conference fail", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	pChnlObj->m_nConfNO = nConferNO;
	DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	m_pPubCObj->PC_ShowChannelConferenceNO( nIndex, nConferNO );

	
	return;
}

int CDJSS17::DJSS_FindHoldChannel(int nIndex )
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

int CDJSS17::DJSS_FindFreeConferIndex()
{
	int rGroupinfo;//取得会议组信息返回值
	for(int i = 0; i < m_nTotalConf; i++ )
	{
		if( 1 == m_nConfCard )
		{
			//有D2560 会议卡
			GROUP_CTRL groupctrl;
			rGroupinfo = DJConf_GetGroupInfoNew(i,&groupctrl);//根据会议组ID号获取当前会议组信息
			if(rGroupinfo==-1)return i;//寻找存在空闲会议组则返回该会议组号
		}
		else
		{
			GROUP_INFO_MASS GroupInfo;
			rGroupinfo = DJConf_GetGroupInfo_Mass(i,&GroupInfo);
			
			if( 0 == GroupInfo.wMemberNum )return i;//寻找存在空闲会议组则返回该会议组号
		}
	}
	return -1;
}

///////////////////////////////////////////////////
//加入会议
//同时将成功加入的会议编号保存在nConferNO中
//
//////////////////////////////////////////////////
void CDJSS17::DJSS_ConferenceEnter(int nIndex )
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nConferNO = -1;
	CEnterConferObj *pObj = (CEnterConferObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CEnterConferObj) ) )
	{
		m_strMSG.Format("EnterConference error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );

		return ;  
	}

	int nStep = pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
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
					m_strMSG.Format("%s Input Conference index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				nConferNO = atoi( strConfNO );
			}
			
			if(  nConferNO < 0 || nConferNO >= m_nTotalConf )
			{
				m_strMSG.Format("%s: ChIndex = %d    Group error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

			int nChnl = -1;
			CString strChnl = pObj->m_strChnlIndex;
			int nKey1 = pObj->GetVarKey( strChnl );
			if( nKey1 < 0 )
			{
				nChnl = atoi( strChnl);
			}
			else
			{
				if(  !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strChnl, nKey1 ) )
				{
					m_strMSG.Format("%s Input Channel index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				nChnl = atoi( strChnl );
			}

			if( -1 == nChnl )
			{
				//如果 nChnl = -1表示自己运行该节点的通道加入
				nChnl = nIndex;
			}

			if( nChnl < 0  || nChnl >= m_nTotalChannel  ) 
			{
				m_strMSG.Format("%s Enter Conference  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

			if( DJSS_DJConf_JoinConfGroup( nChnl,  nConferNO, pObj->m_nAction ) )
			{
				pChnlObj->m_nConfNO = nConferNO;
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				return;
			}
			else
			{
				m_strMSG.Format("%s Enter Conference  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
		}
		break;
	case 2:
		break;
	default:
		break;
	}

	return;
}

BOOL CDJSS17::DJSS_DJConf_JoinConfGroup(int nIndex, int nGroup, int nMode)
{
	if( nIndex < 0  || nIndex >= m_nTotalChannel  ) return FALSE;
	if(  nGroup <= 0 || nGroup >= m_nTotalConf ) return FALSE;
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nSS7Chnl =  pChnlObj->m_nChannel;//取得通道号
	int nJoinConf = 0;
	BOOL bResult = FALSE;
	int nConfChnlType = DJSS_GetDevConfChnlType( pChnlObj->m_nChanelType );
	if( 1 == m_nConfCard )
	{
		//有D2560 会议卡
		MEMBER_MODE mCardMode = CONF_SPEAKER_NORMAL;
		switch( nMode )
		{
		case 0://可说可听
			mCardMode = CONF_SPEAKER_NORMAL;
			break;
		case 1://只听会议，不能说
			mCardMode = CONF_LISTEN;
			break;
		case 2://始终说,不能听
			mCardMode = CONF_SPEAKER_ALWAYS;
			break;
		default:
			break;

		}
		nJoinConf = DJConf_JoinConfGroup( nGroup,nConfChnlType,nSS7Chnl,mCardMode,-8,0,true);
		//表明当指定的电话会议组ID号没有创建时，是否用iGroupID参数指定的ID号和配置文件Tce1_32.ini中设置的缺省会议组参数创建一个新的会议组
		if( nJoinConf >= 0 )
		{
			DJConf_Adjust_CtrlWord ( nConfChnlType, nSS7Chnl, 1 );// 是抑制DTMF
			pChnlObj->m_nConfNO = nGroup;
			m_pPubCObj->PC_ShowChannelConferenceNO( nIndex, nGroup );
			CString strFileName = m_strPath+"\\SysVoiceFle\\confadd.pcm";
			DJConf_StopPlayFile( m_pConference[nGroup].nVocID );
			m_pConference[nGroup].nVocID=DJConf_PlayFileNew(nGroup,(LPTSTR)(LPCTSTR)strFileName,0L,0xFFFFFFFFL);
			m_pConference[nGroup].State=CONF_ADD;
			pChnlObj->m_nConfAction =  CIF_CONF_SPEAKER_LISTEN;
			bResult = TRUE;
		}
		else
		{
			bResult = FALSE;
		}
		
	}
	else
	{
		
		nJoinConf = DJConf_EnterConf( nGroup, nMode, nConfChnlType,nSS7Chnl,0,0 );
		if( nJoinConf >= 0 )
		{
			DJConf_Adjust_CtrlWord ( nConfChnlType, nSS7Chnl, 1 );// 是抑制DTMF
			pChnlObj->m_nConfNO = nGroup;
			m_pPubCObj->PC_ShowChannelConferenceNO( nIndex, nGroup );
			CString strFileName = m_strPath+"\\SysVoiceFle\\confadd.pcm";
			DJConf_StopPlayFile( m_pConference[nGroup].nVocID );
			m_pConference[nGroup].nVocID=DJConf_PlayFileNew(nGroup,(LPTSTR)(LPCTSTR)strFileName,0L,0xFFFFFFFFL);
			m_pConference[nGroup].State=CONF_ADD;
			pChnlObj->m_nConfAction =  CIF_CONF_SPEAKER_LISTEN;
			bResult = TRUE;
		}
		else
		{
			bResult = FALSE;
		}
	}


	return bResult;

}
/////////////////////////////////////////////////////////
//通道离开会议
//
/////////////////////////////////////////////////////////
void CDJSS17::DJSS_DJConf_LeaveConf(int nIndex, int nGroup)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if(  nGroup <= 0 || nGroup >= m_nTotalConf ) return;
	int nSS7Chnl =  pChnlObj->m_nChannel;//取得通道号
	if( 1 == m_nConfCard )
	{
		//有D2560 会议卡
		int nConfChnlType = DJSS_GetDevConfChnlType( pChnlObj->m_nChanelType );
		DJConf_ExitConfGroup( nConfChnlType, nSS7Chnl,FALSE);

	}
	else
	{
		int nConfChnlType = DJSS_GetDevConfChnlType( pChnlObj->m_nChanelType );
		DJConf_LeaveConf( nConfChnlType, nSS7Chnl );

	}
	pChnlObj->m_nConfNO = -1;
	m_pPubCObj->PC_ShowChannelConferenceNO( nIndex, -1 );
	CString strFileName = m_strPath+"\\SysVoiceFle\\confexit.pcm";
	DJConf_StopPlayFile( m_pConference[nGroup].nVocID );
	m_pConference[nGroup].nVocID=DJConf_PlayFileNew(nGroup,(LPTSTR)(LPCTSTR)strFileName,0L,0xFFFFFFFFL);
	m_pConference[nGroup].State=CONF_LEAVE;

	return;
}

////////////////////////////////////////////////////////////
//删除会议，释放该会议资源。找到在该会议中的所有通道，将其从
//该会议中，同时将所有通道挂机
//
/////////////////////////////////////////////////////////////
void CDJSS17::DJSS_DJConf_DeleteGroup(int nIndex, int nGroup)
{
	if(  nIndex < 0 ) return;
	if(  nGroup <= 0 || nGroup >= m_nTotalConf ) return;
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( nIndex != i)
		{
			if( nGroup == m_pChannelInfo[i].m_nConfNO )
			{
				DJSS_DJConf_LeaveConf( i, nGroup );
				m_pChannelInfo[i].m_nConfNO = -1;
				if(  m_pChannelInfo[i].m_nChanelType >= 6 &&  m_pChannelInfo[i].m_nChanelType <= 8 ) 
					DJSS_ResetUser(i);
				else
					DJSS_ResetTrunk(i);
			}
		}
	}

	m_pConference[nGroup].nChnlIndex	= -1;
	m_pConference[nGroup].State			= CONF_EXIT;
	m_pConference[nGroup].nConfPlayID	= -1;
	m_pConference[nGroup].nConfRecordID = -1;
	return;
}
///////////////////////////////////////////////////
//删除会议
////System assign表示删除该通道主持的会议
//
//////////////////////////////////////////////////
void CDJSS17::DJSS_ConferenceDelete(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CConferenceDeleteObj *pObj = (CConferenceDeleteObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CConferenceDeleteObj) ) )
	{
		m_strMSG.Format("ConferenceDelete error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );

		return ;  
	}

	int nGroup = 0;
	if( 0 == pObj->m_nDeleteType )
	{
		//System assign表示删除该通道主持的会议
		nGroup = pChnlObj->m_nConfNO;
	}
	else
	{
		//表示删除指定编号的会议
		CString strConfNO = pObj->m_strConferNo;
		int nKey = pObj->GetVarKey( strConfNO );
		if( nKey < 0 )
		{
			//常量
			nGroup = atoi(strConfNO);
		}
		else
		{
			if(  !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strConfNO, nKey ) )
			{
				m_strMSG.Format("%s Input Conference index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			nGroup = atoi(strConfNO);
		}
		
	}
	if(  nGroup <= 0 || nGroup >= m_nTotalConf )
	{
		m_strMSG.Format("%s: ChIndex = %d    Group error ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	if( nGroup < 0 )
	{
		m_strMSG.Format("%s Input Conference index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	else
	{
		DJSS_DJConf_DeleteGroup( nIndex, nGroup );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	}



	return;
}

BOOL CDJSS17::DJSS_LoadNormalScript()
{

	CCIFSysScriptDoc doc;
	CString strfile = "";
	strfile = m_strPath + "\\CFSynWay.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		return FALSE ;
	}
	
	m_lNormal =  DJSS_PlayDTMF(doc.m_lCardKey);
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

void CDJSS17::DJSS_ChangeVoice(int nIndex)
{


	return;
}

/////////////////////////////////////
//得到某通道的时隙号,变声中使用
/////////////////////////////////////
int CDJSS17::DJSS_GetTimeSlot(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nSS7Chnl =  pChnlObj->m_nChannel;//取得通道号
	if(  pChnlObj->m_nChanelType >= 6 && pChnlObj->m_nChanelType <= 8  )
	{
		return D160PCI_GetTimeSlot(nSS7Chnl);
	}
	else
	{
		return DJTrk_GetTimeSlot(nSS7Chnl);
	}
}

int CDJSS17::DJSS_ConnectFromTS(int nIndex, int nTimeIndex )
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nSS7Chnl =  pChnlObj->m_nChannel;//取得通道号
	if(  pChnlObj->m_nChanelType >= 6 && pChnlObj->m_nChanelType <= 8  )
	{
		return D160PCI_ConnectFromTS(nSS7Chnl, nTimeIndex );
	}
	else
	{
		return DJTrk_ConnectFromTS(nSS7Chnl, nTimeIndex );
	}
}


///////////////////////////////////////////////////
//离开会议
//////////////////////////////////////////////////
void CDJSS17::DJSS_ConferenceLeave(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CLeaveConferObj *pObj = (CLeaveConferObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CLeaveConferObj) ) )
	{
		m_strMSG.Format(" LeaveConfer  error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}

	//从变量取得会议组
	int nKey = pObj->GetVarKey( pObj->m_strConferNO );
	CString strConferNO("");
	if( -1 == nKey && "" != pObj->m_strConferNO )
	{
		strConferNO = pObj->m_strConferNO;
	}
	else
	{
		if( !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strConferNO, nKey ) )
		{
			m_strMSG.Format("ConferenceBuild error : ChIndex = %d   Error = have not free conference resource", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
	}
	int nConferNO = atoi(strConferNO);
	if(  nConferNO <= 0 || nConferNO >= m_nTotalConf )
	{
		m_strMSG.Format("%s: ChIndex = %d    Group error ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	int nChnl = -1;
	CString strChnl = pObj->m_strChnlIndex;
	int nKey1 = pObj->GetVarKey( strChnl );
	if( nKey1 < 0 )
	{
		nChnl = atoi( strChnl);
	}
	else
	{
		if(  !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strChnl, nKey1 ) )
		{
			m_strMSG.Format("%s Input Channel index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		nChnl = atoi( strChnl );
	}

	if( -1 == nChnl )
	{
		//如果 nChnl = -1表示自己运行该节点的通道加入
		nChnl = nIndex;
	}
	if( nChnl < 0  || nChnl >= m_nTotalChannel  ) 
	{
		m_strMSG.Format("%s: ChIndex = %d    Channel index error ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;

	}

	DJSS_DJConf_LeaveConf( nChnl, nConferNO );

	DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );

	return;
}

void CDJSS17::DJSS_EndCallEventCheckBlock(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
    if (pChnlObj->GetCurrentNodeObj() == NULL )
	{
		m_strMSG.Format("EndCallEventCheckBlock Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		return ;   
	}
	
	DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );


	return;
}

void CDJSS17::DJSS_EndCallEventEndBlock(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
    if (pChnlObj->GetCurrentNodeObj() == NULL )
	{
		m_strMSG.Format("EndCallEventEndBlock Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		return ;   
	}


	m_pPubCObj->PC_ShowChannelState( nIndex, "Free");
	m_pPubCObj->PC_ShowConnectToChnl( nIndex, -1 );

	pChnlObj->MoveFirst_Node();

	return;
}
/////////////////////////////////////////////////////
//得到会议设备通道类型
//
////////////////////////////////////////////////////
int CDJSS17::DJSS_GetDevConfChnlType(int nCifChnlType)
{
	switch(nCifChnlType)
	{
	case CF_CHTYPE_TRUNK:  //模拟中继外线(普通市话线),接电信局
	case CF_CHTYPE_USER:  //模拟中继内线(座席线)，接电话机
	case CF_CHTYPE_RECORD:  //模拟中继高阻线(监控线)，与电话线/电话机相并联
	case CF_CHTYPE_EMPTY: //空线路类型
		return CONF_CHANNEL_TYPE_USER;
	case CF_CHTYPE_DTNO1 : //数字中继中国一号信令 
	case CF_CHTYPE_DTNO7 : //数字中继中国七号信令(TUP)
	case CF_CHTYPE_DTDSS1 :  //数字中继ISDN PRI(30B+D)
	case CF_CHTYPE_DTNO7ISUP : //数字中继中国七号信令(ISUP)
	case CF_CHTYPE_MTP3 : //七号信令MTP传输层通道
	default:
		return CONF_CHANNEL_TYPE_TRUNK;
		
	}

}


BOOL CDJSS17::DJSS_InitSystem(CListCtrl *plstChannel, CListBox *plistSystem, CListCtrl *plistScript, CListBox *pCommList, CListCtrl *plistTCPIP, CListCtrl *plstConfer, CListCtrl *plstFax)
{
	m_plistTCPIP	= plistTCPIP;
	m_pCommList		= pCommList;
	m_plistScript	= plistScript;
	m_plstChannel	= plstChannel;
	m_plistSystem	= plistSystem;
	m_plstConfer	= plstConfer;
	m_plstFax		= plstFax;
	m_pwndProg		= m_pwndProg;

	m_pPubCObj->m_plistTCPIP	= plistTCPIP;
	m_pPubCObj->m_pCommList		= pCommList;
	m_pPubCObj->m_plistScript	= plistScript;
	m_pPubCObj->m_plstChannel	= plstChannel;
	m_pPubCObj->m_plistSystem	= plistSystem;
	m_pPubCObj->m_plstConfer	= plstConfer;
	m_pPubCObj->m_plstFax		= plstFax;
	m_pPubCObj->m_pwndProg		= m_pwndProg;



	m_pwndProg->StepIt();
	m_pPubCObj->PC_OpenLogFile();
	m_pPubCObj->PC_InitSystemVar();
	if( !DJSS_LoadSysScript()) return false;
	if( !DJSS_LoadNormalScript()) return false;
	m_pwndProg->StepIt();//进度条显示
	// add for 模拟通道(使用tc08a32.dll中的函数)
	//DJSys_UserCall_TC08A32( true );
	// end

	m_nIs_SS1	= DJSys_IsUseThisSignal (PCM_WORK_MODE_SS1);
	m_nIs_DSS1	= DJSys_IsUseThisSignal (PCM_WORK_MODE_DSS1);
	m_nIs_SS7	= DJSys_IsUseThisSignal (PCM_WORK_MODE_SS7);

	if( m_nIs_SS7 )
	{
		if( 0 == m_nSS7_type )//TUP
		{
			if(	DJNo7_InitSystem() == -1 ) 
			{
				m_pPubCObj->PC_ShowSysMsg(0,"Initialize No.7 TUP Driver Fail");
				return FALSE ;
			}
		}
		else if( 1 == m_nSS7_type )//TUP and ISUP
		{
			if(	DJISUP_InitSystem() == -1 ) 
			{
				m_pPubCObj->PC_ShowSysMsg(0,"Initialize No.7 ISUP Driver Fail!") ;
				return FALSE ;
			}
		}
		else if( 2 == m_nSS7_type )//TUP and ISUP
		{
			if(	DJNo7_InitSystem() == -1 ) 
			{
				m_pPubCObj->PC_ShowSysMsg(0,"Initialize No.7 TUP Driver Fail");
				return FALSE ;
			}
			if(	DJISUP_InitSystem() == -1 ) 
			{
				m_pPubCObj->PC_ShowSysMsg(0,"Initialize No.7 ISUP Driver Fail!" ) ;
				return FALSE ;
			}
		}
		else
		{
			if(	DJNo7_InitSystem() == -1 ) 
			{
				m_pPubCObj->PC_ShowSysMsg(0,"Initialize No.7 TUP Driver Fail");
				return FALSE ;
			}
		}
	}

	int i = DJSys_EnableCard( "", "PROMPT.INI" ) ;
	if ( i != 0 ) {
		m_pPubCObj->PC_ShowSysMsg(0,"Load Driver Fail");
		return  FALSE;
    }
	if(!DJSys_EnableDtmfSend())
	{
		m_pPubCObj->PC_ShowSysMsg(0,"Enable Send Dtmf  Fail");
		return  FALSE;
	}

	if ( m_nIs_DSS1 ) {
		if(	DJISDN_InitSystem() != 1 ) 
		{
			DJSys_DisableCard();
			MessageBox(NULL,"Initialize ISDN PRI Driver Fail!","Error", MB_OK ) ;
			return false ;
		}
	}



	DJSys_DisableWarn();

	//初始化会议
	DJSS_DJConf_InitConfCard();

	if( 1 == m_nFax )
	{
		//系统有传真卡
		if ( DJFax_DriverReady(2048) ) 
		{
			DJSys_DisableCard();
			m_pPubCObj->PC_ShowSysMsg(0,"Init fax Driver Fail");
			return FALSE;
		}
		m_nTotalFaxLine = DJFax_GetTotalFaxChnl();
		m_plstFax->DeleteAllItems();
		for( int i = 0; i < m_nTotalFaxLine; i++ )
		{
			m_strMSG.Format("%d",i);
			m_plstFax->InsertItem(i,m_strMSG );
			m_plstFax->SetItemText(i,1,"Free");
			m_plstFax->SetItemText(i,2,"0");
			m_plstFax->SetItemText(i,3,"");

		}
	}

	m_nTotalTrunk	= DJTrk_GetTotalTrunkNum() ;
	m_nTotalUser	= CheckValidCh();

	char szKey[20];
	memset(szKey,0,20);
	sprintf(szKey, "%d", E1_ReadPassword( 0 ) );
	
	if( !g_pD08a->IsThisKey(szKey, sizeof(szKey)) )
	{
		DJNo7_ExitSystem() ;
		DJSys_DisableCard() ;
		return false;
	}

	PostMessage( m_hWnd , WM_MY_FRIEND_MSG, 0, E1_ReadPassword( 0 ) );
	m_nTotalChannel = m_nTotalTrunk + m_nTotalUser;



	m_nVocBusy		= DJVoc_SFVC_ForPlay_New(0, false);
	m_nVocRingBack	= DJVoc_SFVC_ForPlay_New(0, false);
	m_nVocTone		= DJVoc_SFVC_ForPlay_New(0, false);
	DJVoc_LoopPlayPromptFile( m_nVocBusy, "BUSY" ) ;
	DJVoc_LoopPlayPromptFile( m_nVocRingBack, "RING" ) ;
	DJVoc_LoopPlayPromptFile( m_nVocTone, "TONE" ) ;
	


	
	CChiefIVRDoc doc;
	CString strfile = "";
	strfile = m_strPath + "\\CFCHDJ.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		DJNo7_ExitSystem() ;
		DJSys_DisableCard() ;
		return false;
	}
	
	m_nTotalChannel = doc.m_nTotalChannel;
	
	CString str = "";
	POSITION pos = doc.m_ChSetListObj.GetHeadPosition();
	
	i= -1;
	while( pos )
	{
		CCFChnlSetObj *pObj = doc.m_ChSetListObj.GetNext(pos);
		if( NULL != pObj )
		{
			i++;
			if( !DJSS_InitChannel( pObj,i ) )
			{

				break;
			}
		}
		m_pwndProg->StepIt();//进度条显示
	}
	

	


	return TRUE;
}

void CDJSS17::DJSS_ExitSystem()
{
	if ( m_nIs_SS7 ) 
	{
		if( 0 == m_nSS7_type )//TUP
		{
			DJNo7_ExitSystem();
		}
		else if( 1 == m_nSS7_type )//TUP and ISUP
		{
			 DJISUP_ExitSystem();
		}
		else if( 2 == m_nSS7_type )//TUP and ISUP
		{
			DJNo7_ExitSystem();
			DJISUP_ExitSystem();
		}
		else
		{
			DJNo7_ExitSystem();
		}

	}

	if ( m_nIs_DSS1 ) 
	{
		DJISDN_ExitSystem();
	}

	DJSys_DisableCard() ;

	return;
}

int CDJSS17::DJSS_FindUerChannel(CString strLocalDN)
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

////////////////////////////////////////
//取得当前空闲的外线通道（SS1,DSS1,TUP,ISUP）
//的逻辑通道号
//////////////////////////////////////
int CDJSS17::DJSS_FindFreeTrunkChnl()
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
				if( CHANNEL_STATE_FREE == m_pChannelInfo[i].m_nState )
				{
					nCurrentTrunkNo = i+1 ;
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
	
	for( i = 0 ; i < nCurrentTrunkNo ; i++ )
	{
		switch( m_pChannelInfo[i].m_nChanelType )
		{
			case CF_CHTYPE_DTNO1://数字中继中国一号信令 
			case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
			case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
			case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
				if( CHANNEL_STATE_FREE == m_pChannelInfo[i].m_nState )
				{
					nCurrentTrunkNo = i+1 ;
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


void CDJSS17::DJSS_GetConfMember(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nConferNO = -1;
	CGetConferMemberObj *pObj = (CGetConferMemberObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CGetConferMemberObj) ) )
	{
		m_strMSG.Format("Get ConferMember error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );

		return ;  
	}

	
	int nKey = pObj->GetVarKey( pObj->m_strConferNo );
	CString strConfNO = "";
	if( nKey < 0 )
	{
		nConferNO = atoi(pObj->m_strConferNo);
	}
	else
	{
		if(  !m_pPubCObj->PC_GetNorVarValues(pChnlObj, strConfNO, nKey ) )
		{
			m_strMSG.Format("%s Input Conference index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		nConferNO = atoi( strConfNO );
	}
	
	nKey =  pObj->GetVarKey( pObj->m_strMemberCount );
	if( nKey < 0 )
	{
		m_strMSG.Format("%s Input Save Conference member to variable Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	int nMembers = 0;
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( nConferNO == pChnlObj->m_nConfNO ) 
			nMembers ++;
	}

	m_strMSG.Format("%d",nMembers);
	BOOL bResult = m_pPubCObj->PC_SaveValuesToVar(pChnlObj, m_strMSG, nKey );
	if( bResult )
	{
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	}
	else
	{
		m_strMSG.Format("%s Save Conference member to variable Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
	}
			
	return;
}



void CDJSS17::DJSS_ConfDoWork()
{

	for(int i = 0; i < m_nTotalConf; i++ )
	{
		DJSS_ShowConfState( i );
		switch( m_pConference[i].State )
		{
		case CONF_FREE://空闲状态，没有占用
			break;
		case CONF_PLAY_INDEX:
			break;
		case CONF_RECORD://处于会议录音状态
		case CONF_PLAY://处于会议放音状态
		case CONF_ADD:
		case CONF_LEAVE:
			m_pConference[i].nConfPlayID=-1;
			m_pConference[i].State=CONF_PLAY;
		case CONF_EXIT:
			DJSS_StopConfPlay(i);
			DJSS_StopConfRecord(i);
			m_pConference[i].nConfPlayID=-1;
			m_pConference[i].State=CONF_FREE;
			break;
		}
	}
	return;
}





void CDJSS17::DJSS_AnswerCall(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nS7Chnl = pChnlObj->m_nChannel;//取得通道号
	int nType = pChnlObj->m_nChanelType;

	switch(nType)
	{
	case CF_CHTYPE_NOKNOW://    -1 //未知通道类型 
		break;
	case CF_CHTYPE_EMPTY://     0  //空通道 
		break;
	case CF_CHTYPE_DTNO1://     1  //数字中继中国一号信令 
		{
			DJISDN_SetChnState( nS7Chnl/30,nS7Chnl%30, ISDN_CH_SET_CONNECT,0);//send offhook signa
			DJTrk_InitDtmfBuf( nS7Chnl );
		}
		break;
	case CF_CHTYPE_DTNO7 ://    2  //数字中继中国七号信令(TUP)
		{
			DJNo7_SetChnStateA( nS7Chnl/30,nS7Chnl%30, CALLEE_SET_ACM, ACM_ADDR_CHARGE + ACM_CALLEE_IDLE );//send ack signal
			DJNo7_SetChnStateA( nS7Chnl/30,nS7Chnl%30, CALLEE_SET_ANC, CSM_CHARGE );//send offhook signal
			DJTrk_InitDtmfBuf( nS7Chnl );
		}
		break;
	case CF_CHTYPE_DTDSS1://    3  //数字中继ISDN PRI(30B+D)
		{
			DJExg_VoiceToTrunk_SetParam(true);
		}
		break;
	case CF_CHTYPE_DTNO7ISUP:// 4  //数字中继中国七号信令(ISUP)
		{
			//应答，计费
			DJISUP_SetChnState( nS7Chnl/30,nS7Chnl%30, ISUP_CALLEE_SET_ACM,CI_CHARGE );//send ack signal
			DJISUP_SetChnState( nS7Chnl/30,nS7Chnl%30, ISUP_CALLEE_SET_ANM );//send offhook signal
			DJTrk_InitDtmfBuf( nS7Chnl );
		}
		break;
	case CF_CHTYPE_MTP3://		5  //七号信令MTP传输层通道
		break;
	case CF_CHTYPE_TRUNK://     6  //模拟中继外线(普通市话线),接电信局
		break;
	case CF_CHTYPE_USER://		7  //模拟中继内线(座席线)，接电话机
		{
			FeedPower( nS7Chnl );
			StartPlaySignal( nS7Chnl, SIG_STOP );
		}
		break;
	case CF_CHTYPE_RECORD://    8  //模拟中继高阻线(监控线)，与电话线/电话机相并联
		break;
	case CF_CHTYPE_VIP://	    9  //VOIP通道类型
		break;
	default:
		break;
	}

	DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
	return;

}

void CDJSS17::DJSS_StopConfRecord(WORD wGroupNo)
{
	int voiceID=-1;
	int rGroupinfo;//取得会议组信息返回值
	if( 1 == m_nConfCard )//有2560会议卡
	{
		GROUP_CTRL groupctrl;
		memset( &groupctrl, 0, sizeof( groupctrl ) );
		rGroupinfo=DJConf_GetGroupInfoNew(wGroupNo,&groupctrl);//根据会议组ID号获取当前会议组信息
		if(rGroupinfo==-1) return;
		for( int i = 0; i < groupctrl.wMemberNum ; i++)
		{
			if(groupctrl.MemberCtrl[i].cbChnlType==CONF_CHANNEL_TYPE_VOICE)
			{
				if( DJVoc_GetVoiceOperateType( groupctrl.MemberCtrl[i].wChnlNo ) == OP_Record )
				{
					voiceID=groupctrl.MemberCtrl[i].wChnlNo;
					break;
				}
			}
		}

	}
	else
	{
		GROUP_INFO GroupInfo;
		memset( &GroupInfo, 0, sizeof( GroupInfo ) );
		rGroupinfo=DJConf_GetGroupInfo(wGroupNo,&GroupInfo);//根据会议组ID号获取当前会议组信息
		if(rGroupinfo==-1) return;
		for(int i=0;i< GroupInfo.wMemberNum;i++)
		{
			if(GroupInfo.MemberInfo[i].cbChnlType==CONF_CHANNEL_TYPE_VOICE)
			{
				if( DJVoc_GetVoiceOperateType( GroupInfo.MemberInfo[i].wChnlNo ) == OP_Record )
				{
					voiceID=GroupInfo.MemberInfo[i].wChnlNo;
					break;
				}
			}
		}
	}
	return;
}

int CDJSS17::DJSS_GetRecordVoiceID(WORD wGroupNo)
{
	int voiceID=-1;
	int rGroupinfo;//取得会议组信息返回值
	if( 1 == m_nConfCard )//有2560会议卡
	{
		GROUP_CTRL groupctrl;
		memset( &groupctrl, 0, sizeof( groupctrl ) );
		rGroupinfo=DJConf_GetGroupInfoNew(wGroupNo,&groupctrl);//根据会议组ID号获取当前会议组信息
		if(rGroupinfo==-1) return -1;
		for( int i = 0; i < groupctrl.wMemberNum ; i++)
		{
			if(groupctrl.MemberCtrl[i].cbChnlType==CONF_CHANNEL_TYPE_VOICE)
			{
				if( DJVoc_GetVoiceOperateType( groupctrl.MemberCtrl[i].wChnlNo ) == OP_Record )
				{
					voiceID=groupctrl.MemberCtrl[i].wChnlNo;
					break;
				}
			}
		}

	}
	else
	{
		GROUP_INFO GroupInfo;
		memset( &GroupInfo, 0, sizeof( GroupInfo ) );
		rGroupinfo=DJConf_GetGroupInfo(wGroupNo,&GroupInfo);//根据会议组ID号获取当前会议组信息
		if(rGroupinfo==-1) return -1;
		for(int i=0;i< GroupInfo.wMemberNum;i++)
		{
			if(GroupInfo.MemberInfo[i].cbChnlType==CONF_CHANNEL_TYPE_VOICE)
			{
				if( DJVoc_GetVoiceOperateType( GroupInfo.MemberInfo[i].wChnlNo ) == OP_Record )
				{
					voiceID=GroupInfo.MemberInfo[i].wChnlNo;
					break;
				}
			}
		}
	}
	return voiceID;
}

void CDJSS17::DJSS_StopConfPlay(WORD wGroupNo)
{
	int voiceID=-1;
	int rGroupinfo;//取得会议组信息返回值
	if( 1 == m_nConfCard )//有2560会议卡
	{
		GROUP_CTRL groupctrl;
		memset( &groupctrl, 0, sizeof( groupctrl ) );
		rGroupinfo=DJConf_GetGroupInfoNew(wGroupNo,&groupctrl);//根据会议组ID号获取当前会议组信息
		if(rGroupinfo==-1) return;
		for( int i = 0; i < groupctrl.wMemberNum; i++)
		{
			if(groupctrl.MemberCtrl[i].cbChnlType==CONF_CHANNEL_TYPE_VOICE)
			{
				if( DJVoc_GetVoiceOperateType( groupctrl.MemberCtrl[i].wChnlNo ) == OP_Play )
				{
					DJConf_StopPlayFile(voiceID);
				}
			}
		}

	}
	else
	{
		GROUP_INFO GroupInfo;
		memset( &GroupInfo, 0, sizeof( GroupInfo ) );
		rGroupinfo=DJConf_GetGroupInfo(wGroupNo,&GroupInfo);//根据会议组ID号获取当前会议组信息
		if(rGroupinfo==-1) return ;
		for(int i=0;i<GroupInfo.wMemberNum;i++)
		{
			if(GroupInfo.MemberInfo[i].cbChnlType==CONF_CHANNEL_TYPE_VOICE)
			{
				if( DJVoc_GetVoiceOperateType( GroupInfo.MemberInfo[i].wChnlNo ) == OP_Play )
				{
					DJConf_StopPlayFile(voiceID);
				}
			}
		}
	}
	return ;
}
int CDJSS17::DJSS_GetPlayVoiceID(WORD wGroupNo)
{
	int voiceID=-1;
	int rGroupinfo;//取得会议组信息返回值
	if( 1 == m_nConfCard )//有2560会议卡
	{
		GROUP_CTRL groupctrl;
		memset( &groupctrl, 0, sizeof( groupctrl ) );
		rGroupinfo=DJConf_GetGroupInfoNew(wGroupNo,&groupctrl);//根据会议组ID号获取当前会议组信息
		if(rGroupinfo==-1) return -1;
		for( int i = 0; i < groupctrl.wMemberNum; i++)
		{
			if(groupctrl.MemberCtrl[i].cbChnlType==CONF_CHANNEL_TYPE_VOICE)
			{
				if( DJVoc_GetVoiceOperateType( groupctrl.MemberCtrl[i].wChnlNo ) == OP_Play )
				{
					voiceID=groupctrl.MemberCtrl[i].wChnlNo;
					break;
				}
			}
		}

	}
	else
	{
		GROUP_INFO GroupInfo;
		memset( &GroupInfo, 0, sizeof( GroupInfo ) );
		rGroupinfo=DJConf_GetGroupInfo(wGroupNo,&GroupInfo);//根据会议组ID号获取当前会议组信息
		if(rGroupinfo==-1) return -1;
		for(int i=0;i<GroupInfo.wMemberNum;i++)
		{
			if(GroupInfo.MemberInfo[i].cbChnlType==CONF_CHANNEL_TYPE_VOICE)
			{
				if( DJVoc_GetVoiceOperateType( GroupInfo.MemberInfo[i].wChnlNo ) == OP_Play )
				{
					voiceID=GroupInfo.MemberInfo[i].wChnlNo;
					break;
				}
			}
		}
	}
	return voiceID;
}

void CDJSS17::DJSS_StartRecord(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CRecordObj *pObj = (CRecordObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CRecordObj) ) )
	{
		m_strMSG.Format("Record Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	if( 0 == pObj->m_nRecordType )
		DJSS_StartRecord_Normal( nIndex,  pObj );
	else
		DJSS_StartRecord_Confer( nIndex,  pObj );



	return;
}


void CDJSS17::DJSS_StartRecord_Normal(int nIndex, CRecordObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nStep = pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			CString strFile("");
			BOOL bResult = m_pPubCObj->PC_GetExpressValues(pChnlObj, strFile, pObj->m_strFileName);
			if( !bResult )
			{
				m_strMSG.Format("Record Error; ChIndex = %d   Error = File name error", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;   	
			}
			strFile = pObj->m_strFolder + strFile;
			pChnlObj->m_strRecordFile = strFile;
			char *pszFileName = (LPTSTR)(LPCTSTR)strFile;
			bResult = DJSS_MyRecordFile( nIndex, pszFileName, 1024*1024*pObj->m_nFileLength,-1 );
			if( !bResult )
			{
				m_strMSG.Format("Record Error; ChIndex = %d   Error = Start Record error File=%s", nIndex,strFile );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;   	
			}

			pChnlObj->m_nStep = 1;

		}
		break;
	case 1:
		if( DJSS_GetDtmfCode( nIndex , pChnlObj->m_strDTMFRecived ) > 0 )
		{
			
			m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlObj->m_strDTMFRecived );
			if( pChnlObj->m_strDTMFRecived == pObj->m_strEndDTMF  )
			{
				DJSS_StopRecord( nIndex );
				//清除录音文件末尾的DTMF
				m_pPubCObj->PC_ClearVoxFileEndDTMF( pChnlObj->m_strRecordFile);
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
		}
		if( DJSS_CheckRecordFileEnd( nIndex, -1 ) )
		{
			DJSS_StopRecord( nIndex );
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		}
		break;
	default:
		break;

	}

	return;
}

void CDJSS17::DJSS_StartRecord_Confer(int nIndex, CRecordObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nStep = pChnlObj->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			int nGroupNo = 0;
			int nVarKey = pObj->GetVarKey( pObj->m_strConferNO  );
			if( nVarKey < 0 )
			{
				nGroupNo = atoi(pObj->m_strConferNO );
			}
			else
			{
				if( !m_pPubCObj->PC_GetNorVarValues(pChnlObj, m_strMSG, pObj->GetVarKey( pObj->m_strConferNO ) ) )
				{
					m_strMSG.Format("%s: ChIndex = %d    Play error ",  pObj->m_strCaption, nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				nGroupNo = atoi(m_strMSG);
			}

			if(  nGroupNo <= 0 || nGroupNo >= m_nTotalConf )
			{
				m_strMSG.Format("%s: ChIndex = %d    Group error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}

			CString strFile("");
			BOOL bResult = m_pPubCObj->PC_GetExpressValues(pChnlObj, strFile, pObj->m_strFileName);
			if( !bResult )
			{
				m_strMSG.Format("Record Error; ChIndex = %d   Error = File name error", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;   	
			}
			strFile = pObj->m_strFolder + strFile;
			pChnlObj->m_strRecordFile = strFile;
			char *pszFileName = (LPTSTR)(LPCTSTR)strFile;
			m_pConference[nGroupNo].nConfRecordID = DJConf_RecordFileNew( nGroupNo, pszFileName,0L, 1024*1024*pObj->m_nFileLength );
			if( m_pConference[nGroupNo].nConfRecordID < 0 )
			{
				m_strMSG.Format("Record Error; ChIndex = %d   Error = Start Record error File=%s", nIndex,strFile );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;   	
			}
			
			pChnlObj->m_nConfNO = nGroupNo;
			m_pConference[nGroupNo].State = CONF_RECORD;
			pChnlObj->m_nStep = 1;
			
		}
		break;
	case 1:
		{
			int nGroupNo = pChnlObj->m_nConfNO ;
			if(  nGroupNo <= 0 || nGroupNo >= m_nTotalConf ){
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			if( DJVoc_CheckVoiceEnd( m_pConference[nGroupNo].nConfRecordID ) )
			{
				DJConf_StopRecordFile(nGroupNo);
				DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
			else{
				if( DJSS_GetDtmfCode( nIndex , pChnlObj->m_strDTMFRecived ) > 0 )
				{
					m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlObj->m_strDTMFRecived );
					if( pChnlObj->m_strDTMFRecived == pObj->m_strEndDTMF  )
					{
						DJConf_StopRecordFile(nGroupNo);
						DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
					}
				}

			}
		}
		break;
	default:
		break;
	}
	return;
}

/***************************************************

返回值描述：
	0：成功
	1：失败
	2：超时
	3：未出结果
***************************************************/
int CDJSS17::DJSS_MyMakeCall_Check(int nIndex )
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nSS7Chnl	= pChnlObj->m_nChannel;//取得通道号
	int nChnlType = pChnlObj->m_nChanelType;
	if( CF_CHTYPE_USER == nChnlType )
	{
		if(OffHookDetect(nSS7Chnl) || EVENTTYPE_OFFHOOK == pChnlObj->m_nEventType)
		{
			StartPlaySignal( nSS7Chnl, SIG_STOP );
			FeedPower( nSS7Chnl );
			StartHangUpDetect( nSS7Chnl );
			if( EVENTTYPE_OFFHOOK != pChnlObj->m_nEventType )
			{
				//不是软电话应答时
				pChnlObj->m_nState =  CHANNEL_STATE_CONNECT;
			}
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
			pChnlObj->m_nEventType =  EVENTTYPE_OFFHOOK;
			
			//发送摘机消息
			m_strMSG.Format("[OffHook DN:%s]",pChnlObj->m_strLocalDN);
			m_pPubCObj->PC_ShowCommMsg(m_strMSG);
			CTI_OffHookMsg( atoi(pChnlObj->m_strLocalDN) );
			return 0;
		}
		else{
				//在30秒内未应答
				long n = m_pPubCObj->PC_ElapseTime(pChnlObj);
				if( n > 30 )
				{
					FeedPower( nSS7Chnl );
					return 2;
				}
		}
		return 3;
	}

	BYTE cb = DJSys_GetPcmMode ( nSS7Chnl / 30 );
	switch ( cb ) {
	case PCM_WORK_MODE_NONE:		// "7号(无信令)"
	case PCM_WORK_MODE_SS7:			// "7号"
		if( CF_CHTYPE_DTNO7 == nChnlType ) 
		{
				if( DJNo7_GetChnState( nSS7Chnl/30, nSS7Chnl%30 ) == CALLER_CONNECT ) 
				{
					return 0;
				}
				switch(DJNo7_CheckCalloutResult(nSS7Chnl/30,nSS7Chnl%30))
				{
				case C_USER_BUSY:
				case C_NO_ANSWER:
				case C_ADDR_LACK: 
				case C_UNALLOC_CODE:
				case C_TIME_OUT:
				case C_CALL_FAIL:
				case C_USER_ONHOOK:
				case C_REMOTE_BLOCK:
				case C_CALL_COLLIDE:
					DJNo7_SetChnState(nSS7Chnl/30,nSS7Chnl%30, CALLER_SET_CLF ) ;
					return 1;
				}
				//在60秒内未应答
				long n = m_pPubCObj->PC_ElapseTime(pChnlObj);
				if( n > 30 )
				{
					DJNo7_SetChnState(nSS7Chnl/30,nSS7Chnl%30, CALLER_SET_CLF ) ;
					return 2;

				}
		}
		else if( CF_CHTYPE_DTNO7ISUP == nChnlType ) 
		{
				if( DJISUP_GetChnState(nSS7Chnl/30,nSS7Chnl%30 ) == ISUP_CALLER_CONNECT ) 
				{
					//连接成功
					return 0;
				}
				else if(DJISUP_GetCalloutState( nSS7Chnl/30,nSS7Chnl%30 ) & CALLOUT_REV_CPG)
				{			
					DJISUP_SetChnState(nSS7Chnl/30,nSS7Chnl%30, ISUP_CH_SET_REL ) ;
					return 1;
				}
				//在30秒内未应答
				long n = m_pPubCObj->PC_ElapseTime(pChnlObj);
				if( n > 30 )
				{
					DJISUP_SetChnState(nSS7Chnl/30,nSS7Chnl%30, ISUP_CH_SET_REL ) ;
					return 2;
				}
		}
		break;
	case PCM_WORK_MODE_SS1:			// "1号"
		{
				//在60秒内未应答
				switch( DJTrk_GetDialStatus( nSS7Chnl ) ){
				case DS_Dialing://正在拨号
					break;
				case DS_NoDial://没有拨号
					break;
				case DS_Wait://等待用户摘机
					break ;
				case DS_Connect:
					//连接成功
					return 0;
				default:
					DJTrk_BackwardHangUp( nSS7Chnl );
					return 1;
				}
				long n = m_pPubCObj->PC_ElapseTime(pChnlObj);
				if( n > 30 )
				{
					DJTrk_BackwardHangUp( nSS7Chnl );
					return 2;
				}

		}
		break;
	case PCM_WORK_MODE_DSS1:		// "数字1号(ISDN PRI)"
		{
			if( DJISDN_GetChnState( nSS7Chnl/30, nSS7Chnl%30 ) == ISDN_CH_WAIT_APP_FREE ) 
			{
				DJISDN_SetChnState(nSS7Chnl/30,nSS7Chnl%30 , ISDN_CH_SET_FREE, 0);
				return 1;
			}
			else if(DJISDN_GetCalloutResult( nSS7Chnl/30,nSS7Chnl%30) == ISDN_C_USER_OFFHOOK )
			{
				return 0;
			}
			//在60秒内未应答
			long n = m_pPubCObj->PC_ElapseTime(pChnlObj);
			if( n > 30 )
			{
				DJISDN_SetChnState(nSS7Chnl/30,nSS7Chnl%30 , ISDN_CH_SET_FREE, 0);
				return 2;
			}
		}
		break;
	default:
		return 1;
	}//end switch


	return 3;//未得出结果
}



void CDJSS17::DJSS_DealCTIMsg(CFC_MESSAGE_PACK *pMsg)
{
	
	int nMsgType = pMsg->nMsgType;
	int nDN = pMsg->nSMID;
	
	m_strMSG.Format("%d",nDN);
	int nIndex = DJSS_FindUerChannel( m_strMSG  );
	if( nMsgType != CFMTYPE_REG_COMM )
	{
		if(  0 > nIndex || nIndex >= m_nTotalChannel )
		{
			m_strMSG.Format("[MType:%d][Message Error From DN:%d]", nMsgType, nDN );
			m_pPubCObj->PC_ShowSysMsg(0,m_strMSG);	
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
				m_strMSG.Format("[DN:%d][Monitoring:%s]", pMsg->nSMID, recMsg.szTel);
				m_pPubCObj->PC_ShowCommMsg( m_strMSG );
				m_strMSG.Format("%s",recMsg.szTel);
				int nMonitorIndex = DJSS_FindUerChannel( m_strMSG );
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
				m_strMSG.Format("[MType:Monitoring][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( m_strMSG );
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
				m_strMSG.Format("[DN:%d][MakeCall:%s][CallType:%d]", pMsg->nSMID, recMsg.szTel,recMsg.nCallType);
				m_pPubCObj->PC_ShowCommMsg( m_strMSG );

				//找到发送消息来的正确通道号码
				pChnlObj->m_nDialType = recMsg.nCallType;
				pChnlObj->m_strDialTel.Format("%s", recMsg.szTel );
				pChnlObj->m_nEventType	= EVENTTYPE_AGENT_MAKECALL_MSG;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");

				
				//MakeCall response message
				CTI_MakeCallMsg( nDN ,0 , "");//执行正确

				m_strMSG.Format("[MType:MakeCall][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( m_strMSG );
			}
			break;

		case CFMTYPE_TRANSFER://Transfer
			{
				//通道操作
				CFMSG_TRANSFER recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, pMsg->szMessageBody ,sizeof(CFMSG_TRANSFER) );
				m_strMSG.Format("[DN:%d][Transfer:%s]", pMsg->nSMID, recMsg.szTel);
				m_pPubCObj->PC_ShowCommMsg( m_strMSG );
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
				m_strMSG.Format("[MType:Transfer][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( m_strMSG );

			}
			break;
		case CFMTYPE_CANCEL_TRANSFER://Cancel transfer
			{
				//通道操作
				pChnlObj->m_nEventType	= EVENTTYPE_AGENT_CANCEL_TRANSFER;
				m_strMSG.Format("[MType:CancelTransfer][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( m_strMSG );

			}
			break;
		case CFMTYPE_ANSWER://Answer
			{
				//通道操作
				m_strMSG.Format("[MType:AnswerCall][From DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg(m_strMSG );

				//当用软电话AnswerCall时必须用
				int nChnl = pChnlObj->m_nChannel;
				FeedPower( nChnl );
				StartPlaySignal( nChnl, SIG_STOP );
				StartHangUpDetect(nChnl);
				pChnlObj->m_nEventType	= EVENTTYPE_OFFHOOK;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");

				CTI_AnswerCallMsg( nDN, 0 );//执行正确

				m_strMSG.Format("[MType:AnswerCall][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( m_strMSG );
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
				m_strMSG.Format("[Hold DN:%d]", pMsg->nSMID);
				m_pPubCObj->PC_ShowCommMsg( m_strMSG );
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
				m_strMSG.Format("[MType:Hold][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( m_strMSG );
			}
			break;
		case CFMTYPE_PICKUPHOLD://Pickup Hold Request
			{
				//通道操作
				CFMSG_PICKUPHOLD recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, pMsg->szMessageBody ,sizeof(CFMSG_PICKUPHOLD) );
				m_strMSG.Format("[PickupHold DN:%d]", pMsg->nSMID);
				m_pPubCObj->PC_ShowCommMsg( m_strMSG );
				int nLinkIndex = DJSS_FindHoldChannel( nIndex );
				if( -1 != nLinkIndex )
				{
					//找到发送消息来的正确通道号码
					pChnlObj->m_nEventType	= EVENTTYPE_AGENT_PICKUPHOLD_MSG;
					m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
					
					m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN, 0,m_pChannelInfo[nLinkIndex].m_strCallerID ,m_pChannelInfo[nLinkIndex].m_strCalledID);//执行正确
				}	
				else
					m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 1 ,"","");//执行错误
				m_strMSG.Format("[MType:PickupHold][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( m_strMSG );
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
				m_strMSG.Format("[MType:HangUp][From DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg(m_strMSG );
				DJSS_ResetUser( nIndex );	
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
				m_strMSG.Format("[DN:%d][ConferCall:%s][CallType:%d]", pMsg->nSMID, recMsg.szTel,recMsg.nCallType);
				m_pPubCObj->PC_ShowCommMsg( m_strMSG );

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
				m_strMSG.Format("[MType:Build Conference][From DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg(m_strMSG );

				//找到发送消息来的正确通道号码
				pChnlObj->m_nEventType	= EVENTTYPE_AGENT_BUILDCONFER_MSG;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");

				m_strMSG.Format("[MType:Build Conference][TO DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( m_strMSG );

			}
			break;
		default:
			break;
	}


	return;
}


void CDJSS17::DJSS_TransferKey(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int     nRnd = 0;
	srand( (unsigned)time( NULL ) ); 
	nRnd = rand()%6 ; 
	switch(nRnd)
	{
	case 0:
		if( DJSS_PlayDTMF(m_lSystem) !=  DJSS_PlayDTMF(m_lNormal) ) 
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 1:
		if( DJSS_PlayDTMF(m_lNormal) != DJSS_PlayDTMF(pChnlInfo->m_lCardKey) )
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 2:
		if( DJSS_PlayDTMF(m_lSeting) != DJSS_PlayDTMF(pChnlInfo->m_lCardKey)) 
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 3:
		if( DJSS_PlayDTMF(m_lCard) !=  DJSS_PlayDTMF((m_lNormal)) ) 
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 4:
		if( DJSS_PlayDTMF(m_lNormal) != DJSS_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 5:
		if( DJSS_PlayDTMF(m_lCard) != DJSS_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	}

	return;
}
void CDJSS17::DJSS_TCPIP(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int     nRnd = 0;
	srand( (unsigned)time( NULL ) ); 
	nRnd = rand()%6 ; 
	switch(nRnd)
	{
	case 0:
		if( DJSS_PlayDTMF(m_lNormal) !=  DJSS_PlayDTMF(m_lNormal) ) 
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 1:
		if( DJSS_PlayDTMF(m_lSystem) != DJSS_PlayDTMF(pChnlInfo->m_lCardKey) )
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 2:
		if( DJSS_PlayDTMF(m_lSeting) != DJSS_PlayDTMF(pChnlInfo->m_lCardKey)) 
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 3:
		if( DJSS_PlayDTMF(m_lCard) !=  DJSS_PlayDTMF((m_lNormal)) ) 
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 4:
		if( DJSS_PlayDTMF(m_lSeting) != DJSS_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 5:
		if( DJSS_PlayDTMF(m_lCard) != DJSS_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	}

	return;
}
void CDJSS17::DJSS_FaxKey(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int     nRnd = 0;
	srand( (unsigned)time( NULL ) ); 
	nRnd = rand()%6 ; 
	switch(nRnd)
	{
	case 0:
		if( DJSS_PlayDTMF(m_lCard) !=  DJSS_PlayDTMF(m_lNormal) ) 
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 1:
		if( DJSS_PlayDTMF(m_lSeting) != DJSS_PlayDTMF(pChnlInfo->m_lCardKey) )
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 2:
		if( DJSS_PlayDTMF(m_lSeting) != DJSS_PlayDTMF(pChnlInfo->m_lCardKey)) 
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 3:
		if( DJSS_PlayDTMF(m_lCard) !=  DJSS_PlayDTMF((m_lSeting)) ) 
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 4:
		if( DJSS_PlayDTMF(m_lNormal) != DJSS_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 5:
		if( DJSS_PlayDTMF(m_lSeting) != DJSS_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	}

	return;
}


void CDJSS17::DJSS_HangUpTrunk(int nIndex)
{
	
	int nS7Chnl	= m_pChannelInfo[nIndex].m_nChannel;//取得通道号
	BYTE cb = DJSys_GetPcmMode ( nS7Chnl / 30 );
	switch ( cb ) {
	case PCM_WORK_MODE_NONE:		// "7号(无信令)"
	case PCM_WORK_MODE_SS7:			// "7号"
		DJSS_HangUp_SS7( nIndex );
		break;
	case PCM_WORK_MODE_SS1:			// "1号"
		DJSS_HangUp_SS1( nIndex );
		break;
	case PCM_WORK_MODE_DSS1:		// "数字1号(ISDN PRI)"
		DJSS_HangUp_DSS1( nIndex );
		break;
	}
	DJTrk_SetTrunkType(nS7Chnl,Type_Import);
	return;
}

int CDJSS17::DJSS_FindFreeTrunkChnl(int nBeginCH, int nEndCH)
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

/***************************************************

返回值描述：
	0：成功
	1：失败
	2：超时
	3：未出结果
***************************************************/
int CDJSS17::DJSS_MyMakeCall_Check(int nIndex, int nTimeOut)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nSS7Chnl	= pChnlObj->m_nChannel;//取得通道号
	int nChnlType = pChnlObj->m_nChanelType;
	if( CF_CHTYPE_USER == nChnlType )
	{
		if(OffHookDetect(nSS7Chnl) || EVENTTYPE_OFFHOOK == pChnlObj->m_nEventType)
		{
			StartPlaySignal( nSS7Chnl, SIG_STOP );
			FeedPower( nSS7Chnl );
			StartHangUpDetect( nSS7Chnl );
			if( EVENTTYPE_OFFHOOK != pChnlObj->m_nEventType )
			{
				//不是软电话应答时
				pChnlObj->m_nState =  CHANNEL_STATE_CONNECT;
			}
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
			pChnlObj->m_nEventType =  EVENTTYPE_OFFHOOK;
			
			//发送摘机消息
			m_strMSG.Format("[OffHook DN:%s]",pChnlObj->m_strLocalDN);
			m_pPubCObj->PC_ShowCommMsg(m_strMSG);
			CTI_OffHookMsg( atoi(pChnlObj->m_strLocalDN) );
			return 0;
		}
		else{
				//在30秒内未应答
				long n = m_pPubCObj->PC_ElapseTime(pChnlObj);
				if( n > nTimeOut )
				{
					FeedPower( nSS7Chnl );
					return 2;
				}
		}
		return 3;
	}

	BYTE cb = DJSys_GetPcmMode ( nSS7Chnl / 30 );
	switch ( cb ) {
	case PCM_WORK_MODE_NONE:		// "7号(无信令)"
	case PCM_WORK_MODE_SS7:			// "7号"
		if( CF_CHTYPE_DTNO7 == nChnlType ) 
		{
				if( DJNo7_GetChnState( nSS7Chnl/30, nSS7Chnl%30 ) == CALLER_CONNECT ) 
				{
					return 0;
				}
				switch(DJNo7_CheckCalloutResult(nSS7Chnl/30,nSS7Chnl%30))
				{
				case C_USER_BUSY:
				case C_NO_ANSWER:
				case C_ADDR_LACK: 
				case C_UNALLOC_CODE:
				case C_TIME_OUT:
				case C_CALL_FAIL:
				case C_USER_ONHOOK:
				case C_REMOTE_BLOCK:
				case C_CALL_COLLIDE:
					DJNo7_SetChnState(nSS7Chnl/30,nSS7Chnl%30, CALLER_SET_CLF ) ;
					return 1;
				}
				//在60秒内未应答
				long n = m_pPubCObj->PC_ElapseTime(pChnlObj);
				if( n > nTimeOut )
				{
					DJNo7_SetChnState(nSS7Chnl/30,nSS7Chnl%30, CALLER_SET_CLF ) ;
					return 2;

				}
		}
		else if( CF_CHTYPE_DTNO7ISUP == nChnlType ) 
		{
				if( DJISUP_GetChnState(nSS7Chnl/30,nSS7Chnl%30 ) == ISUP_CALLER_CONNECT ) 
				{
					//连接成功
					return 0;
				}
				else if(DJISUP_GetCalloutState( nSS7Chnl/30,nSS7Chnl%30 ) & CALLOUT_REV_CPG)
				{			
					DJISUP_SetChnState(nSS7Chnl/30,nSS7Chnl%30, ISUP_CH_SET_REL ) ;
					return 1;
				}
				//在30秒内未应答
				long n = m_pPubCObj->PC_ElapseTime(pChnlObj);
				if( n > nTimeOut )
				{
					DJISUP_SetChnState(nSS7Chnl/30,nSS7Chnl%30, ISUP_CH_SET_REL ) ;
					return 2;
				}
		}
		break;
	case PCM_WORK_MODE_SS1:			// "1号"
		{
				//在60秒内未应答
				switch( DJTrk_GetDialStatus( nSS7Chnl ) ){
				case DS_Dialing://正在拨号
					break;
				case DS_NoDial://没有拨号
					break;
				case DS_Wait://等待用户摘机
					break ;
				case DS_Connect:
					//连接成功
					return 0;
				default:
					DJTrk_BackwardHangUp( nSS7Chnl );
					return 1;
				}
				long n = m_pPubCObj->PC_ElapseTime(pChnlObj);
				if( n > nTimeOut )
				{
					DJTrk_BackwardHangUp( nSS7Chnl );
					return 2;
				}

		}
		break;
	case PCM_WORK_MODE_DSS1:		// "数字1号(ISDN PRI)"
		{
			if( DJISDN_GetChnState( nSS7Chnl/30, nSS7Chnl%30 ) == ISDN_CH_WAIT_APP_FREE ) 
			{
				DJISDN_SetChnState(nSS7Chnl/30,nSS7Chnl%30 , ISDN_CH_SET_FREE, 0);
				return 1;
			}
			else if(DJISDN_GetCalloutResult( nSS7Chnl/30,nSS7Chnl%30) == ISDN_C_USER_OFFHOOK )
			{
				return 0;
			}
			//在60秒内未应答
			long n = m_pPubCObj->PC_ElapseTime(pChnlObj);
			if( n > nTimeOut )
			{
				DJISDN_SetChnState(nSS7Chnl/30,nSS7Chnl%30 , ISDN_CH_SET_FREE, 0);
				return 2;
			}
		}
		break;
	default:
		return 1;
	}//end switch


	return 3;//未得出结果
}

void CDJSS17::DJSS_HangUpAllCall()
{
	int nType=0;
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		nType=m_pChannelInfo[i].m_nChanelType;
		int nS7Chnl	= m_pChannelInfo[i].m_nChannel;//取得通道号
		if( nType >= 1 &&  nType <= 5 )
		{
			DJSS_HangUpTrunk( i );
		}
	}
	
	return;
}

BOOL CDJSS17::DJSS_MySendDtmfStr(int nIndex,  char *pcDtmf)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nSS7Chnl	= pChnlObj->m_nChannel;//取得通道号
	int nChnlType = pChnlObj->m_nChanelType;
	if( CF_CHTYPE_USER == nChnlType )
	{
		InitDtmfBuf( nSS7Chnl );
		SendDtmfBuf( nSS7Chnl , pcDtmf );
	}
	else
	{
		DJTrk_InitDtmfBuf(nSS7Chnl);
		if( 1 != DJTrk_SendDtmfStr(nSS7Chnl, pcDtmf) )
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CDJSS17::DJSS_MyCheckSendEnd(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nSS7Chnl	= pChnlObj->m_nChannel;//取得通道号
	int nChnlType = pChnlObj->m_nChanelType;
	if( CF_CHTYPE_USER == nChnlType )
	{
		return CheckSendEnd(nSS7Chnl);
	}
	else
	{
		return DJTrk_CheckDtmfSendEnd(nSS7Chnl);
	}

}



BOOL CDJSS17::DJSS_SendCallerID_DTMF(int nIndex, char *CallIDNumStr)
{
	if( NULL == CallIDNumStr ){
		return FALSE;
	}
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nSS7Chnl	= pChnlObj->m_nChannel;//取得通道号
	CString strCallerID = "";
	strCallerID.Format( "D%sC",CallIDNumStr );
	StartTimer( nSS7Chnl, 5);
	return DJSS_MySendDtmfStr( nIndex, (LPSTR)(LPCSTR)strCallerID );

}

BOOL CDJSS17::DJSS_SendCallerID_FSK(int nIndex, char *CallIDNumStr)
{
	

	return TRUE;

}

void CDJSS17::DJSS_Custom(int nIndex)
{

	CString strMSG("");
	CFChannelInfo *pNorTrunk = &(m_pChannelInfo[nIndex]);
	int nType = pNorTrunk->m_nChanelType;
	CCustomObj *pObj = (CCustomObj*)pNorTrunk->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CCustomObj) ) )
	{
		strMSG.Format("CustomObj Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, strMSG );
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	
	if( 0 == pNorTrunk->m_nStep )
	{
		pObj->MoveFirst();
		pNorTrunk->m_nStep = 1;
	}
	if( pObj->m_bEnd )
	{
		DJSS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		return;
	}
	switch( pObj->m_nFunType )
	{
		case 0://SendFax(<File> )
			//DJSS_Cus_SendFax(  nIndex,  pNorTrunk,  pObj );
			break;
		case 1://StartPlayFile(  <File> ; <Loop> )
			//DJSS_Cus_StartPlayFile(  nIndex,  pNorTrunk,  pObj );
			break;
		case 2://StartRecordFile(  <File>  ; <Len> ; <Loop> )
			//DJSS_Cus_StartRecordFile(  nIndex,  pNorTrunk,  pObj );
			break;
		case 3://GetAgentCall( <AgentDN> )
			//DJSS_Cus_GetAgentCall(  nIndex,  pNorTrunk,  pObj );
			break;
		case 4://EnterConfer( <Chnl> ; <conference> )
			//DJSS_Cus_EnterConfer(  nIndex,  pNorTrunk,  pObj );
			break;
		case 5://LeaveConfer( <Chnl> ; <conference> )
			//DJSS_Cus_LeaveConfer(  nIndex,  pNorTrunk,  pObj );
			break;
		case 6://ReciveDtmf( <SaveTo> ; <Len> ; <EndDtmf> )
			//DJSS_Cus_ReciveDtmf(  nIndex,  pNorTrunk,  pObj );
			break;
		case 7://ReciveFax( <File> )
			//DJSS_Cus_ReciveFax(  nIndex,  pNorTrunk,  pObj );
			break;
		case 8://OffHook( <Chnl> )
			//DJSS_Cus_OffHook(  nIndex,  pNorTrunk,  pObj );
			break;
		case 9://OutCall(   <Tel> )
			//DJSS_Cus_OutCall(  nIndex,  pNorTrunk,  pObj );
			break;
		case 10://StopPlayFile( )
			//DJSS_Cus_StopPlayFile(  nIndex,  pNorTrunk,  pObj );
			break;
		case 11://StopRecordFile()
			//DJSS_Cus_StopRecordFile(  nIndex,  pNorTrunk,  pObj );
			break;
		case 12://HangUp( <Chnl> )
			//DJSS_Cus_HangUp(  nIndex,  pNorTrunk,  pObj );
			break;
		case 13://ClearDtmf( <Chnl> )
			//DJSS_Cus_ClearDtmf(  nIndex,  pNorTrunk,  pObj );
			break;
		case 14://GetActiveCall
			//DJSS_Cus_GetAgentActiveCall( nIndex,  pNorTrunk,  pObj );
			break;
		case 15://AnswerCall
			//DJSS_Cus_AnswerCall( nIndex,  pNorTrunk,  pObj );
			break;
		case 16://Hold
			//DJSS_Cus_Hold( nIndex,  pNorTrunk,  pObj );
			break;
		case 17://PickUpHold
			//DJSS_Cus_PickupHold( nIndex,  pNorTrunk,  pObj );
			break;
		case 18://TansferToIVR
			//DJSS_Cus_TansferToIVR( nIndex,  pNorTrunk,  pObj );
			break;	
		case 19://Monitor
			//DJSS_Cus_Monitor( nIndex,  pNorTrunk,  pObj );
			break;	
		case 20://DisMonitor
			//DJSS_Cus_DisMonitor( nIndex,  pNorTrunk,  pObj );
			break;	
		case 21://JoinConfer
			//DJSS_Cus_JoinConfer( nIndex,  pNorTrunk,  pObj );
			break;	
		case 22://ResponseTransfer( <nResult> )");
			//DJSS_Cus_ResponseTransfer( nIndex,  pNorTrunk,  pObj );
			break;
		case 23://ResponsePickUpHold( <nResult> )");
			//DJSS_Cus_ResponsePickUpHold( nIndex,  pNorTrunk,  pObj );
			break;
		case 24://ResponseMonitor( <nResult> )");
			//DJSS_Cus_ResponseMonitor( nIndex,  pNorTrunk,  pObj );
			break;
		case 25://ResponseLogin( <nResult> )");
			//DJSS_Cus_ResponseLogin( nIndex,  pNorTrunk,  pObj );
			break;
		case 26://ResponseJoinConfer( <nResult> )");
			//DJSS_Cus_ResponseJoinConfer( nIndex,  pNorTrunk,  pObj );
			break;
		case 27://ResponseHold( <nResult> )");
			//DJSS_Cus_ResponseHold( nIndex,  pNorTrunk,  pObj );
			break;
		case 28://ResponseGetActive( <nResult> )");
			//DJSS_Cus_ResponseGetActive( nIndex,  pNorTrunk,  pObj );
			break;
		case 29://ResponseDisMonitor( <nResult> )");
			//DJSS_Cus_ResponseDisMonitor( nIndex,  pNorTrunk,  pObj );
			break;
		case 30://ResponseConferCall( <nResult> )");
			//DJSS_Cus_ResponseConferCall( nIndex,  pNorTrunk,  pObj );
			break;
		case 31://ResponseCancelTransfer( <nResult> )");
			//DJSS_Cus_ResponseCancelTransfer( nIndex,  pNorTrunk,  pObj );
			break;
		case 32://ResponseAnswerCall( <nResult> )");
			//DJSS_Cus_ResponseAnswerCall( nIndex,  pNorTrunk,  pObj );
			break;
		case 33://ResponesMakeCall( <nResult> )");
			//DJSS_Cus_ResponesMakeCall( nIndex,  pNorTrunk,  pObj );
			break;
		case 34://ResponseLogOut( <nResult> )");
			//DJSS_Cus_ResponseLogOut( nIndex,  pNorTrunk,  pObj );
			break;
		case 35://ResponseReady( <nResult> )");
			//DJSS_Cus_ResponseReady( nIndex,  pNorTrunk,  pObj );
			break;
		case 36://ResponseNotReady( <nResult> )");
			//DJSS_Cus_ResponseNotReady( nIndex,  pNorTrunk,  pObj );
			break;
		case 37://ResponseSetBusy( <nResult> )");
			//DJSS_Cus_ResponseSetBusy( nIndex,  pNorTrunk,  pObj );
			break;
		case 38://ResponseInServer( <nResult> )");
			//DJSS_Cus_ResponseInServer( nIndex,  pNorTrunk,  pObj );
			break;


		default:
			break;
	}

	return;
}

void CDJSS17::DJSS_Cus_ReciveFax(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}

void CDJSS17::DJSS_Cus_SendFax(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}

void CDJSS17::DJSS_Cus_OutCall(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}
void CDJSS17::DJSS_Cus_OffHook(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}
void CDJSS17::DJSS_Cus_ReciveDtmf(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}
void CDJSS17::DJSS_Cus_LeaveConfer(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}
void CDJSS17::DJSS_Cus_EnterConfer(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}
void CDJSS17::DJSS_Cus_GetAgentCall(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{
}

void CDJSS17::DJSS_Cus_StartRecordFile(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}
void CDJSS17::DJSS_Cus_StartPlayFile(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}

void CDJSS17::DJSS_Cus_StopPlayFile( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj )
{

}

void CDJSS17::DJSS_Cus_StopRecordFile( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj )
{

}

void CDJSS17::DJSS_Cus_HangUp( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj )
{

}

void CDJSS17::DJSS_Cus_ClearDtmf( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj )
{

}

