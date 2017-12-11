// TMS8E240.cpp: implementation of the CTMS8E240 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "D08A.h"
#include "TMS8E240.h"
#include "CalculateExpress.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



#define TMS_DIAL_SIG     1 //拨号音
#define TMS_RINGBACK_SIG 2 //回铃音
#define TMS_BUSY_SIG     3 //忙音

extern CD08A *g_pD08a;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTMS8E240::CTMS8E240()
{
	m_pChannelInfo = NULL;
	m_nTotalChannel = g_pD08a->GetChnlCount();
	m_pChannelInfo = new CFChannelInfo[ m_nTotalChannel ];
	for( int i = 0; i < m_nTotalChannel ;i++ )
	{
		m_pChannelInfo[i].m_nIndex = i;
	}

	m_plistTCPIP	= NULL;
	m_pCommList		= NULL;
	m_plistScript	= NULL;
	m_plstChannel	= NULL;
	m_plistSystem	= NULL;
	m_plstConfer	= NULL;
	m_plstFax		= NULL;

	m_nSaveSysLog	= 0;
	m_nSysType		= 0;
	m_nSwitchMode	= 0;
	m_AgentRecord	= 0;

	m_strCommIPAdrr = "127.0.0.1";
	m_nCommPort		= 7000;
	m_strPath		= "";
	m_strMSG		= "";
	m_bInitVoiceCard = false;
	m_hWnd			= NULL;
	m_nTrunkType	= 0;
	m_nConfCard		= 0;//是否(1/0)有会议卡
	m_nChangeVoice	= 0;
	m_nTotalUser	= 0;
	m_nTotalTrunk	= 0;
	m_nTotalChannel = 0;
	m_nTotalConf	= 0;//取得最大会议组数
	m_nTotalConfResource	= 0;//总的会议资源数
	m_pConference			= NULL;//存储会议组相关信息

	m_bThreadEnd = FALSE;
	m_nThreadCount = 0;//启动的线程数


	m_nTotalFaxLine = 0;//系统检测到的传真卡通道数
	m_nFax			= 0;//系统是否有传真卡，0没有，1有
	


	m_pNormalScriptInfo = NULL;
	m_nNormalScriptNum  = 0;
	m_nChangeVoice		= 0;
	
	m_lSeting = TMS_PlayDTMF( TMS_PlayDTMF(g_pD08a->GetVoiceCardKey()) );

	m_pwndProg = NULL;

	
	m_pPubCObj = &(m_PubCObj);

}

CTMS8E240::~CTMS8E240()
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

		if( 1 == m_nChangeVoice )
		{
			//变声
		}
		/*for( int i = 0; i < m_nTotalChannel; i++ )
		{
			TMS_Release( i );	
		}*/

		TMS_Conf_ReleaseConf();//释放会议资源

	}


	if( 1 == m_nFax )
	{
		//DJFax_DisableCard();
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
	PCIEXITCARD();
}






BOOL CTMS8E240::TMS_InitSystem(CListCtrl *plstChannel, CListBox *plistSystem, CListCtrl *plistScript, CListBox *pCommList, CListCtrl *plistTCPIP, CListCtrl *plstConfer, CListCtrl *plstFax)
{
	if( !PCIINITCARD(0) )
	{
		AfxMessageBox("It is failure to Open TMS administration implement serving");
		return FALSE;
	}

	m_plistTCPIP	= plistTCPIP;
	m_pCommList		= pCommList;
	m_plistScript	= plistScript;
	m_plstChannel	= plstChannel;
	m_plistSystem	= plistSystem;
	m_plstConfer	= plstConfer;
	m_plstFax		= plstFax;

	m_pPubCObj->m_plistTCPIP	= plistTCPIP;
	m_pPubCObj->m_pCommList		= pCommList;
	m_pPubCObj->m_plistScript	= plistScript;
	m_pPubCObj->m_plstChannel	= plstChannel;
	m_pPubCObj->m_plistSystem	= plistSystem;
	m_pPubCObj->m_plstConfer	= plstConfer;
	m_pPubCObj->m_plstFax		= plstFax;
	m_pPubCObj->m_pwndProg		= m_pwndProg;

	m_pPubCObj->PC_OpenLogFile();
	m_pwndProg->StepIt();
	m_pPubCObj->PC_InitSystemVar();
	if( !TMS_LoadSysScript()) return false;
	if( !TMS_LoadNormalScript()) return false;
	

	//初始化会议
	TMS_InitConfCard();

	if( 1 == m_nFax )
	{
		//系统有传真卡
		m_nTotalFaxLine = PCIGETLOGICCHNUM(RESTYPE_FAX);
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
	long k = PCIGETCARDHARDWARESERIAL(0);
	PostMessage( m_hWnd , WM_MY_FRIEND_MSG, 0, PCIGETCARDHARDWARESERIAL(0));
	if( 1 == m_nChangeVoice )
	{
		//变声处理
	}

	m_pwndProg->StepIt();//进度条显示
	CChiefIVRDoc doc;
	CString strfile = "";
	strfile = m_strPath + "\\CFCHDJ.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		return false;
	}
	
	m_nTotalChannel = doc.m_nTotalChannel;
	
	CString str = "";
	POSITION pos = doc.m_ChSetListObj.GetHeadPosition();
	
	int i= -1;
	while( pos )
	{
		CCFChnlSetObj *pObj = doc.m_ChSetListObj.GetNext(pos);
		if( NULL != pObj )
		{
			i++;
			if( !TMS_InitChannel( pObj,i ) )
			{
				break;
			}

		}
		m_pwndProg->StepIt();//进度条显示
	}
	

	


	return TRUE;
}

BOOL CTMS8E240::TMS_LoadNormalScript()
{

	CCIFSysScriptDoc doc;
	CString strfile = "";
	strfile = m_strPath + "\\CFSynWay.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		return FALSE ;
	}
	
	m_lNormal =  TMS_PlayDTMF(doc.m_lCardKey);
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

BOOL CTMS8E240::TMS_LoadSysScript()
{
	CCIFSysScriptDoc doc;
	CString strfile = "";
	strfile = m_strPath + "\\CFTMS.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		return FALSE;
	}

	m_lSystem = TMS_PlayDTMF(doc.m_lCardKey);
	m_pPubCObj->PC_LoadSysScript( &(doc) );



	return TRUE;
}

long CTMS8E240::TMS_PlayDTMF(long lDTMF)
{
	long kk = 0;
	kk = (lDTMF-100)*10+20-30*10-(700+30-3) + ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 ))) - ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 ))) + ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 )));
	return kk;
}


inline BOOL CTMS8E240::TMS_MoveNext_Node(int nIndex, CNextNode *pNextObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	return m_pPubCObj->PC_MoveNext_Node( nIndex, pChnlObj, pNextObj );
}

inline BOOL CTMS8E240::TMS_MoveNext_Node(int nIndex, int nNextType)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	return m_pPubCObj->PC_MoveNext_Node( nIndex, pChnlObj, nNextType );
}

void CTMS8E240::TMS_InitConfCard()
{
	if(  1 == m_nConfCard )
	{
		//增强会议

		if( NULL != m_pConference )
		{
			delete []m_pConference;
			m_pConference = NULL;
		}
		m_nTotalConf	= PCIGETLOGICCHNUM(RESTYPE_ECONF);//取得最大会议组数		
		m_nTotalConfResource = PCIGETLOGICCHNUM(RESTYPE_ECONFCH);//计算总的会议资源数
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
			m_plstConfer->SetItemText(i,1,"0");
			m_plstConfer->SetItemText(i,2,"0");
			m_plstConfer->SetItemText(i,3,"0");
		}
	}
	else
	{
		//普通会议
		if( NULL != m_pConference )
		{
			delete []m_pConference;
			m_pConference = NULL;
		}
		m_nTotalConf	= PCIGETLOGICCHNUM(RESTYPE_CONF);//取得最大会议组数		
		m_nTotalConfResource = PCIGETLOGICCHNUM(RESTYPE_CONFCH);//计算总的会议资源数
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
			m_plstConfer->SetItemText(i,1,"0");
			m_plstConfer->SetItemText(i,2,"0");
			m_plstConfer->SetItemText(i,3,"0");
		}

	}
}

BOOL CTMS8E240::TMS_InitChannel(CCFChnlSetObj *pChSetObj, int nChnlIndex)
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
	pChnlObj->m_lCardKey		= TMS_PlayDTMF(pChSetObj->m_lCardKey);
	pChnlObj->m_plistSystem		= m_plistSystem;
	


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
			CNewDBConnVar *pNewObj = new CNewDBConnVar(*pDBObj);
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

////////////////////////////////////////////////
//释放通道
//对模拟外线(TB通道)挂机或对模拟内线(AGENT通道)送忙音。高阻线(HB通道)不能使用。
//对数字线路发送释放命令，并等待释放结果。
///////////////////////////////////////////////
void CTMS8E240::TMS_ResetChannel(int nIndex)
{
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nLinkIndex = pChnlInfo->m_nLinkCh;
	if( -1 < nLinkIndex && nLinkIndex < m_nTotalChannel  )
	{
		if( CHANNEL_STATE_HOLD != pChnlInfo->m_nState && CHANNEL_STATE_MONITORING !=  pChnlInfo->m_nState )
		{
			if( CF_CHTYPE_RECORD != m_pChannelInfo[nLinkIndex].m_nChanelType )
			{
				TMS_ConF_Leave( nLinkIndex, m_pChannelInfo[nLinkIndex].m_nConfNO );
				TMS_Release(nLinkIndex);
			}
			
			TMS_RecordFileStop( nLinkIndex,-1 );
			m_pChannelInfo[nLinkIndex].MoveCallEndBlock_Node();
			m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Free" );
			
		}

	}

	/****************************/
	//TMS_TransferKey( nIndex );
   /*****************************/

	int nTransferIndex = pChnlInfo->m_nTransferCh;
	if( -1 < nTransferIndex && nTransferIndex < m_nTotalChannel  )
	{
		if( CF_CHTYPE_RECORD != m_pChannelInfo[nTransferIndex].m_nChanelType )
		{
			TMS_ConF_Leave( nTransferIndex, m_pChannelInfo[nTransferIndex].m_nConfNO );
			TMS_Release(nTransferIndex);
		}
		TMS_RecordFileStop( nTransferIndex,-1 );
		m_pChannelInfo[nTransferIndex].MoveCallEndBlock_Node();
		m_pPubCObj->PC_ShowChannelState( nTransferIndex, "Free" );
		m_pPubCObj->PC_ShowConnectToChnl( nTransferIndex, -1 );
		

	}
	
	TMS_ConF_Leave( nIndex, pChnlInfo->m_nConfNO );
	TMS_RecordFileStop( nIndex,-1 );
	TMS_Release(nIndex);
	pChnlInfo->MoveCallEndBlock_Node();
	m_pPubCObj->PC_ShowChannelState( nIndex, "Free");
	m_pPubCObj->PC_ShowConnectToChnl( nIndex, -1 );
	

	return;
}

void CTMS8E240::TMS_Conf_ReleaseConf()
{

}

void CTMS8E240::TMS_MainProcess( )
{
	CFC_MESSAGE_PACK msg;
	RMSG msgs[64];
	
	memset(&msg, 0, sizeof(CFC_MESSAGE_PACK) );
	int i = 0;
	m_bThreadEnd = FALSE;
	while( !m_bThreadEnd  )
	{
		if( 1 == m_nSysType )
		{
			if( !CTI_SocketConnect() )
			{
				
				if( CTI_InitDLL( m_nCommPort, (LPCTSTR)m_strCommIPAdrr )  )
				{
					m_pPubCObj->PC_ShowSysMsg(0,"Connect communicate Server Success !");
				}
				else
				{
					m_pPubCObj->PC_ShowSysMsg(0,"Connect communicate Server fail !");
				}
				CTI_RegToConmmServer();
			}
			else
			{
				memset(&msg, 0, sizeof(CFC_MESSAGE_PACK) );
				if( CTI_ReadData( (char *)&msg, sizeof(CFC_MESSAGE_PACK)) )
					TMS_DealCTIMsg( &msg );
			}
		}

		//加密控制
		/*
		int     nRnd = 0;
		srand( (unsigned)time( NULL ) ); 
		nRnd = rand()%5 ; 
		switch(nRnd)
		{
		case 0:
			if( m_lReadPass !=  m_lCardKey ) continue;
			break;
		case 1:
			if( m_lReadPass !=  m_lCardKey ) continue;
			break;
		case 2:
			if( m_lReadPass !=  m_lCardKey ) continue;
			break;
		case 3:
			if( m_lReadPass !=  m_lCardKey ) continue;
			break;
		case 4:
			if( m_lReadPass !=  m_lCardKey ) continue;
			break;

		}*/
		////////////////////////////////////////////

		for( i = 0; i < m_nTotalChannel; i++ )
		{
			TMS_DoWork( i );	
		}

		TMS_ConfDoWork();



		memset( &msgs, 0, sizeof(msgs) );
		int nMsgNum = PCIGRABMSG(&msgs[0],64);
		for(  i = 0; i < nMsgNum; i++ )
		{
			TMS_ProcessMsg(&msgs[i]);//用户命令处理
		}

		Sleep(100);
	}


	return;
}

void CTMS8E240::TMS_ProcessMsg(RMSG *pmsg)
{

	switch(pmsg->MsgType) 
	{
	case MSG_CONF:
	   TMS_MSG_CONF(pmsg);
       break;
	case MSG_RELEASE:
       TMS_MSG_Release(pmsg);
	   break;
	case MSG_CALLEDACK:
		//被叫应答消息
		//模拟外线(TB通道)呼出成功后对方电话摘机，
		//模拟内线(AGENT通道)振铃后电话摘机。高阻线(HB通道)无该消息。
		TMS_MSG_CalledBack( pmsg );
		break;
	case MSG_FLASH:
		TMS_MSG_Flash( pmsg );
		break;
   case UPMSG_CALLOUTCHANNEL:
		TMS_MSG_CallOutChannel( pmsg );
		break;
   case MSG_ACM:
		break;
   case MSG_CALL:
	   //呼入消息
	   //模拟外线(TB通道)收到振铃信号或模拟内线(AGENT通道)上的电话在
	   //空闲时主动摘机。高阻线(HB通道)无该消息。
        TMS_MSG_Call( pmsg );
		break;
   case MSG_MEDIA:
	   TMS_MSG_Media( pmsg );
		break;
   case UPMSG_ALARM:
	   //printf("Type=%d Ch= %d 收到线路告警消息\n",pmsg->ChType,pmsg->ChIndex);  
	   break;
   case DOWNMSG_SETPARAM:
	   //printf("Type=%d Ch= %d 收到上传参数消息\n",pmsg->ChType,pmsg->ChIndex);  
	   break;
   default:
	   //printf("Type=%d Ch= %d 收到未知消息 msg=0x%x\n",pmsg->ChType,pmsg->ChIndex,pmsg->MsgType);  
		break;
	}

	return;
}

void CTMS8E240::TMS_MSG_Release(RMSG *pmsg)
{
	if( NULL == pmsg ) return;
	// 通过消息取得CIF系统的逻辑通道号
	int nIndex = TMS_GetCifLogicChnl( pmsg->ChIndex, pmsg->ChType );
	if( -1 >= nIndex || nIndex >= m_nTotalChannel ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CNodeObj *pNodeObj = pChnlInfo->GetCurrentNodeObj();
	if( NULL == pNodeObj ) return;
	int nNodeType = pNodeObj->m_nType;
	if( F_R_CALLOUTERR == pmsg->Function )
	{
		//呼出失败
		if( NODETYPE_CHECK_DIAL == nNodeType )
			TMS_Dial_Result(  nIndex,  pmsg );//检查呼出结果
		else
		{
			TMS_Switch_Judge(  nIndex,  pmsg );//检查Switch结果
			TMS_Transfer_Judge(  nIndex,  pmsg );
		}
	}
	if( CF_CHTYPE_USER == pChnlInfo->m_nChanelType )
	{
		int nDN = atoi( pChnlInfo->m_strLocalDN );
		
		m_pPubCObj->PC_CTI_HangUpMsg(nIndex, pChnlInfo );
		TMS_StartPlaySignal( nIndex, TMS_BUSY_SIG );
	}
	if( NODETYPE_EVENT_CHECK == pNodeObj->m_nType )
	{
		TMS_EventCheck_Release(nIndex,pmsg);
		return;
	}
	

	TMS_ResetChannel( nIndex );

	return;

}
/////////////////////////////////////////////////////////
//模拟外线(TB通道)收到振铃信号或模拟内线(AGENT通道)上的
//电话在空闲时主动摘机。高阻线(HB通道)无该消息。
//数字线路上有电话呼入。
//所以在流程处理中，只需要判断该通道当前流程结点是否是
//WaitCall或EventCheck两个结点，其他结点不处理
//////////////////////////////////////////////////////////
void CTMS8E240::TMS_MSG_Call(RMSG *pmsg)
{
	if( NULL == pmsg ) return;
	// 通过消息取得CIF系统的逻辑通道号
	int nIndex = TMS_GetCifLogicChnl( pmsg->ChIndex, pmsg->ChType );
	if( 0 >  nIndex || nIndex >= m_nTotalChannel ) return;
	CNodeObj *pNodeObj = m_pChannelInfo[nIndex].GetCurrentNodeObj();
	if( NULL == pNodeObj ) return;
	int nChType = m_pChannelInfo[nIndex].m_nChanelType;
	switch( nChType )
	{
	case CF_CHTYPE_DTNO1://数字中继中国一号信令 
	case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
	case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
	case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
	case CF_CHTYPE_MTP3://七号信令MTP传输层通道
	case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
	case CF_CHTYPE_VIP://VOIP通道类型
		{
			m_pChannelInfo[nIndex].m_strRingTime = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");

			if( NODETYPE_EVENT_CHECK == pNodeObj->m_nType )
			{
				//模拟外线(TB通道)收到振铃信号
				TMS_EventCheck_Ringing( nIndex,  pmsg ); 
			}
		}
		break;
	case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
		{
			/**************************************************
			模拟外线(TB通道)收到振铃信号或模拟内线(AGENT通道)
			上的电话在空闲时主动摘机。高阻线(HB通道)无该消息。
			
			调用TMS_Ack与TMS_StartPlaySignal，目的是避免模拟内线
			(AGENT通道)上的电话在空闲时主动摘机，然后
			按键时，收到的DTMF保存在被叫号码中
			*************************************************/
			TMS_Ack( pmsg );//应答
			TMS_StartPlaySignal( pmsg, TMS_DIAL_SIG );
			if( NODETYPE_EVENT_CHECK == pNodeObj->m_nType )
			{
				/*对模拟外线(TB通道)摘机或对模拟内线(AGENT通道)
				响应(停止放信号音)。高阻线(HB通道)不能使用。*/
				TMS_EventCheck_OffHook( nIndex,  pmsg ); 
			}
			
		}
		break;
	case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
		break;
	case CF_CHTYPE_NOKNOW://未知通道类型 
		break;
	case CF_CHTYPE_EMPTY://空通道 
		break;
	default:
		break;
	}

	return;
}

/**************************************************************
所有用户发送的电话会议命令(MSGTYPE== MSG_CONF) ,管理器都会返回一
个结果消息给用户,用于指示命令是否成功完成
Param 会议结果,取值说明：0 成功;非0 失败
加入会议方式0 SpeakListen; 1 ListenOnly ; 2 SpeakOnly

//会议组的状态常量
CIF_CONF_NULL			0 //空闲(既没有创建或加入会议)
CIF_CONF_LISTEN		    1 //只听成员，在会议中只能听，不能说
CIF_CONF_SPEAKER_LISTEN 2 //普通成员，可说可听，具有动态发言权
CIF_CONF_SPEAKER_ALWAYS 3 //始终说成员，总是具有发言权
CIF_CONF_SPEAKER_ONLY   4 //只说成员，在会议中只能说，不能听，主要用于对会议放音。
CIF_CONF_CREAT          5 //创建了该会议，同时可说可听，有权管理该会议
***************************************************************/
void CTMS8E240::TMS_MSG_CONF(RMSG *pmsg)
{
	if( NULL == pmsg ) return;
	int nIndex = TMS_GetCifLogicChnl( pmsg->ChIndex, pmsg->ChType );// 通过消息取得CIF系统的逻辑通道号
	if( 0 > nIndex || nIndex >= m_nTotalChannel ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nConfNO = pmsg->LinkChIndex;
	switch(pmsg->Function)
	{
		case F_CF_CREATE://创建会议
			if( 0 == pmsg->Param )//成功
			{
				pChnlInfo->m_nConfNO = nConfNO;
				pChnlInfo->m_nConfAction = CIF_CONF_CREAT;//创建了该会议，同时可说可听，有权管理该会议
				m_pConference[nConfNO].nChnlIndex = nIndex;//创建会议的通道号
				m_pConference[nConfNO].nMemberNum = 1;
				m_pConference[nConfNO].nResNum	  =1;//占用的会议资源数
				m_pConference[nConfNO].nListenNum =0;
				m_pPubCObj->PC_ShowChannelConferenceNO( nIndex, nConfNO );
			}
			break;
		case F_CF_CLOSE://关闭会议
			if( 0 == pmsg->Param )//成功
			{
				pChnlInfo->m_nConfNO = -1;
				pChnlInfo->m_nConfAction = CIF_CONF_NULL;//空闲(既没有创建或加入会议)
				m_pConference[nConfNO].nChnlIndex = -1;//创建会议的通道号
				m_pConference[nConfNO].nMemberNum = 0;
				m_pConference[nConfNO].nResNum	  = 0;//占用的会议资源数
				m_pConference[nConfNO].nListenNum =0;
			}
			break;
		case F_CF_LINKCH://参加会议
			if( 0 == pmsg->Param )//成功
			{
				if( pChnlInfo->m_nConfNO < 0 )
				{
					m_pConference[nConfNO].nMemberNum += 1;
					m_pConference[nConfNO].nResNum	  += 1;//占用的会议资源数
				}   
				pChnlInfo->m_nConfAction = CIF_CONF_SPEAKER_LISTEN ;//普通成员，可说可听，具有动态发言权
				pChnlInfo->m_nConfNO = nConfNO;
				m_pPubCObj->PC_ShowChannelConferenceNO( nIndex, nConfNO );
			}
			break;
		case F_CF_UNLINKCH://离开会议
			if( 0 == pmsg->Param )//成功
			{
				switch( pChnlInfo->m_nConfAction )
				{
				case CIF_CONF_SPEAKER_LISTEN:
				case CIF_CONF_CREAT:
					{
						if( m_pConference[nConfNO].nMemberNum > 0 )
							m_pConference[nConfNO].nMemberNum -= 1;
						if( m_pConference[nConfNO].nResNum > 0 )
							m_pConference[nConfNO].nResNum	  -= 1;//占用的会议资源数
					}
					break;
				case CIF_CONF_SPEAKER_ALWAYS:
				case CIF_CONF_SPEAKER_ONLY:
				case CIF_CONF_NULL:
				case CIF_CONF_LISTEN:
					{
						if( m_pConference[nConfNO].nListenNum > 0 )
							m_pConference[nConfNO].nListenNum -= 1;
					}
					break;
				default:
					break;
				}
				pChnlInfo->m_nConfNO = -1;
				m_pPubCObj->PC_ShowChannelConferenceNO( nIndex, -1 );
				pChnlInfo->m_nConfAction = CIF_CONF_NULL;//空闲(既没有创建或加入会议)
				if( m_pConference[ nConfNO ].nMemberNum <= 0 )
				{
					//判断该通道是否是是最后一个离开会议，如果是，
					//自动关闭该会议
					TMS_ConF_Close(nIndex  , nConfNO );
				}
			}
			break;
		case F_CF_MONITOR://旁听会议(只用于增强型会议)
			if( 0 == pmsg->Param )//成功
			{
				switch( pChnlInfo->m_nConfAction )
				{
				case CIF_CONF_SPEAKER_LISTEN:
				case CIF_CONF_CREAT:
					{
						if( m_pConference[nConfNO].nMemberNum > 0 )
							m_pConference[nConfNO].nMemberNum -= 1;
						if( m_pConference[nConfNO].nResNum > 0 )
							m_pConference[nConfNO].nResNum	  -= 1;//占用的会议资源数
					}
					break;
				case CIF_CONF_SPEAKER_ALWAYS:
				case CIF_CONF_SPEAKER_ONLY:
				case CIF_CONF_NULL:
				case CIF_CONF_LISTEN:
				default:
					break;
				}
				m_pConference[nConfNO].nListenNum += 1;
				pChnlInfo->m_nConfAction = CIF_CONF_LISTEN ;//普通成员，可说可听，具有动态发言权
				pChnlInfo->m_nConfNO = nConfNO;
				m_pPubCObj->PC_ShowChannelConferenceNO( nIndex, nConfNO );
			}
			break;
		case F_CF_PARAM://设置会议室参数(只用于增强型会议)
		default:
			break;
	}
	TMS_ConferenceResult( nIndex ,pmsg->Function, pmsg->Param );
	return;
}

/********************************************************
模拟外线(TB通道)呼出成功后对方电话摘机，模拟内线(AGENT通道)
振铃后电话摘机。高阻线(HB通道)无该消息。
*********************************************************/
void CTMS8E240::TMS_MSG_CalledBack(RMSG *pmsg)
{
	if( NULL == pmsg ) return;
	// 通过消息取得CIF系统的逻辑通道号
	int nIndex = TMS_GetCifLogicChnl( pmsg->ChIndex, pmsg->ChType );
	if( 0 > nIndex || nIndex >= m_nTotalChannel ) return;
	CNodeObj *pNodeObj = m_pChannelInfo[nIndex].GetCurrentNodeObj();
	if( NULL == pNodeObj ) return;
	int nChType = m_pChannelInfo[nIndex].m_nChanelType;
	int nNodeType = pNodeObj->m_nType;
	switch( nChType )
	{
	case CF_CHTYPE_DTNO1://数字中继中国一号信令 
	case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
	case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
	case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
	case CF_CHTYPE_MTP3://七号信令MTP传输层通道
	case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
	case CF_CHTYPE_VIP://VOIP通道类型
		{
			if( NODETYPE_CHECK_DIAL == nNodeType )
				TMS_Dial_Result(  nIndex,  pmsg );//检查呼出结果
			else
			{
				TMS_Switch_Judge(  nIndex,  pmsg );//检查Switch结果
				TMS_Transfer_Judge(  nIndex,  pmsg );
			}
		}
		break;
	case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
		{
			if( NODETYPE_CHECK_DIAL== nNodeType )
				 TMS_Dial_Result(  nIndex,  pmsg );//检查呼出结果
			else 
			{
				if( NODETYPE_EVENT_CHECK== nNodeType )
				{
					TMS_EventCheck_OffHook(  nIndex,  pmsg );//模拟内线(AGENT通道)振铃后电话摘机
				}
				
				TMS_Switch_Judge(  nIndex,  pmsg );//检查Switch结果
				TMS_Transfer_Judge(  nIndex,  pmsg );

			}
		}
		break;
	case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
		break;
	case CF_CHTYPE_NOKNOW://未知通道类型 
		break;
	case CF_CHTYPE_EMPTY://空通道 
		break;
	default:
		break;
	}

	return;
}
/**************************************************************************
	线路呼出时分配的逻辑通.模拟外线(TB通道)、模拟内线(AGENT通道)呼出时分配的
	逻辑通道。高阻线(HB通道)无该消息
	----------------------------------------------------------------------
	Function 分配结果
		参数说明：
		F_CC_HANDLE 本呼出分配到参考句柄，该句柄只用于后续的重叠呼叫(参见CALL命
		令消息)。
		F_CC_NORMAL 本呼出分配到呼出逻辑通道，呼叫正在进行中。
		F_CC_NOCHANNEL 本呼出分配不到呼出通道，呼叫终止，通道释放
		本消息可能在F_CC_HANDLE 后收到，
		收到本消息后不会再收到F_CC_HANDLE 或F_CC_NORMAL，也不会收到
		MSG_RELEASE 消息。
	Param 分配失败原因
		参数说明：
		当Function=F_CC_NOCHANNEL时，本值为分配失败原因。
		P_CC_CHBUSY 指定/所有通道使用中。
		P_CC_CHCALLMASK 在配置中通道呼出被禁止。
		P_CC_CHNOTEXIST 指定的通道号不存在。
	处理呼出返回消息
**************************************************************************/
void CTMS8E240::TMS_MSG_CallOutChannel(RMSG *pmsg)
{
	if( NULL == pmsg ) return;
	// 通过消息取得CIF系统的逻辑通道号
	int nIndex = TMS_GetCifLogicChnl( pmsg->ChIndex, pmsg->ChType );
	if( 0 >  nIndex || nIndex >= m_nTotalChannel ) return;
	CNodeObj *pNodeObj = m_pChannelInfo[nIndex].GetCurrentNodeObj();
	if( NULL == pNodeObj ) return;
	int nNodeType = pNodeObj->m_nType;
	switch( pmsg->Function )
	{
	case F_CC_NOCHANNEL://数字中继中国一号信令 	
		{
			if( NODETYPE_CHECK_DIAL == nNodeType )
				TMS_Dial_Result(  nIndex,  pmsg );//检查呼出结果
			else
			{
				TMS_Switch_Judge(  nIndex,  pmsg );//检查Switch结果
				TMS_Transfer_Judge(  nIndex,  pmsg );
			}
		
		}
	default:
		break;
	}

	return;
}

/********************************************************
模拟内线(AGENT通道)上的电话被按了"闪断"("Flash")键
或者"R"键。模拟外线(TB通道)无该消息。
高阻线(HB通道)可以检测到该动作，但无本消息，而是
以MEDIA 消息(参见MEDIA 消息说明)传送给用户。
*********************************************************/
void CTMS8E240::TMS_MSG_Flash(RMSG *pmsg)
{
	if( NULL == pmsg ) return;
	// 通过消息取得CIF系统的逻辑通道号
	int nIndex = TMS_GetCifLogicChnl( pmsg->ChIndex, pmsg->ChType );
	if( 0 >  nIndex || nIndex >= m_nTotalChannel ) return;
	CNodeObj *pNodeObj = m_pChannelInfo[nIndex].GetCurrentNodeObj();
	if( NULL == pNodeObj ) return;
	switch( m_pChannelInfo[nIndex].m_nChanelType )
	{
	case CF_CHTYPE_DTNO1://数字中继中国一号信令 
	case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
	case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
	case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
	case CF_CHTYPE_MTP3://七号信令MTP传输层通道
	case CF_CHTYPE_TRUNK://模拟中继外线(普通市话线),接电信局
	case CF_CHTYPE_VIP://VOIP通道类型
		break;
	case CF_CHTYPE_USER://模拟中继内线(座席线)，接电话机
		{
			if( NODETYPE_EVENT_CHECK == pNodeObj->m_nType )
			{
				TMS_EventCheck_Flash( nIndex,  pmsg ); 
			}
		}
		break;
	case CF_CHTYPE_RECORD://模拟中继高阻线(监控线)，与电话线/电话机相并联
		break;
	case CF_CHTYPE_NOKNOW://未知通道类型 
		break;
	case CF_CHTYPE_EMPTY://空通道 
		break;
	default:
		break;
	}

	return;
}
////////////////////////////////////////////
//说明：
//多媒体事件消息一般和多媒体命令消息相对应，为多媒体命令的执行结果或收到的多媒体信息
//由于该消息内容较多，现按每一种多媒体类型分别说明，目前包括"语音"，"双音多频"，"信号音"，
//"传真"，"数据传输"等
//注意：
//多媒体功能一般适用于所有的模拟和数字线路。
////////////////////////////////////////////
void CTMS8E240::TMS_MSG_Media(RMSG *pmsg)
{
	if( NULL == pmsg ) return;
	// 通过消息取得CIF系统的逻辑通道号
	int nIndex = TMS_GetCifLogicChnl( pmsg->ChIndex, pmsg->ChType );
	if( 0 >  nIndex || nIndex >= m_nTotalChannel ) return;
	CNodeObj *pNodeObj = m_pChannelInfo[nIndex].GetCurrentNodeObj();
	if( NULL == pNodeObj ) return;

	switch(pmsg->LinkChType)
	{
	case RESTYPE_VOCPCM:
		TMS_MSG_Media_VOCPCM(pmsg);
		break;
	case RESTYPE_DTMF:
	      if(pmsg->Function==F_MEDIA_Rx_Memory) //收到dtmf
		  {
            if(pmsg->ChType==CHTYPE_HB) //高阻模块单独处理
              TMS_MSG_RecvDTMF_HB(nIndex,pmsg);
			else
              TMS_MSG_RecvDTMF(nIndex,pmsg);            
		  }
		  else if(pmsg->Function==F_MEDIA_Tx_Release) //收到发码结束
		  {

		  }
		break;
	case RESTYPE_FAX:
		TMS_MSG_Fax(  nIndex,  pmsg );
		break;
	case RESTYPE_FSK:
		break;
	case RESTYPE_CAS:
		break;
	case RESTYPE_MODEM:
		break;
	default:
		break;

	}

}
/*************************************************
	对线路的传真消息处理
*************************************************/
void CTMS8E240::TMS_MSG_Fax(int nIndex, RMSG *pmsg)
{
	CNodeObj * pNodeObj = m_pChannelInfo[nIndex].GetCurrentNodeObj();
	if( NULL == pNodeObj ) return;
	if( NODETYPE_FAXRECIVE != pNodeObj->m_nType && NODETYPE_FAXSEND != pNodeObj->m_nType ) return;
	switch( pmsg->Function )
	{
	case F_MEDIA_Tx_Release://发送传真结束
	case F_MEDIA_Rx_Release://接收传真结束
		{
			switch(pmsg->Param)
			{
			case RESTOP_FINISH://传真操作正常结束
				{
					TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
					return;
				}
				break;
			case RESTOP_USER://传真操作被用户终止
			case RESTOP_CHREL://传真操作由于通道释放而结束
			case RESTOP_COMMERR://传真操作由于握手失败而结束
			case RESTOP_FILERR://传真操作由于文件操作错误而结束
			default:
				{
					m_strMSG.Format("%s-%s ChIndex = %d",  pNodeObj->m_strCaption, GetStopReason(pmsg->Param), nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;
				}
				break;
			}


		}
		break;
	case F_MEDIA_Error://对应的传真命令消息错误
		{
			switch(pmsg->Param)
			{
			case P_MEDIA_AllocOk://分配传真资源
				break;
			case P_MEDIA_AllocErr://无法分配传真资源
			case P_MEDIA_BusyErr://通道正忙于处理其它工作
			case P_MEDIA_FullErr://发送队列缓冲区已经满，命令没有被接受
			case P_MEDIA_FuncErr://不支持该种传真放式
			case P_MEDIA_FileErr://传真文件名错误
			default:
				{
					m_strMSG.Format("%s-%s ChIndex = %d",  pNodeObj->m_strCaption, GetResErrReason(pmsg->Param), nIndex);
					m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
					TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
					return;

				}
				break;
			}
		}
		break;
	default:
		break;
	}

	return;
}

/*************************************************
	对线路或增强型会议录放音消息处理
*************************************************/
void CTMS8E240::TMS_MSG_Media_VOCPCM(RMSG *pmsg)
{
	if( RESTYPE_ECONF == pmsg->ChType )
	{
		//会议
		int nGroup =  pmsg->ChIndex;
		if(  nGroup < 0 || nGroup >= m_nTotalConf )
		{
			return;
		}
		int nRecordIndex = m_pConference[ nGroup ].nRecordIndex;
		int nPlayIndex = m_pConference[ nGroup ].nPlayIndex;
		if( F_MEDIA_Tx_Release == pmsg->Function )
		{
			m_pConference[ nGroup ].nPlayIndex = -1;
		}
		if( F_MEDIA_Rx_Release == pmsg->Function )
		{
			m_pConference[ nGroup ].nRecordIndex = -1;
		}
		if(  nRecordIndex < 0 || nRecordIndex >= m_nTotalChannel )
		{
			return;
		}
		if(  nPlayIndex < 0 || nPlayIndex >= m_nTotalChannel )
		{
			return;
		}

		CNodeObj * pRecObj = m_pChannelInfo[nRecordIndex].GetCurrentNodeObj();
		CNodeObj * pPlayObj = m_pChannelInfo[nPlayIndex].GetCurrentNodeObj();
		if( NULL != pRecObj )
		{
			if( NODETYPE_START_RECORD  == pRecObj->m_nType )
			{
				TMS_StartRecord_Result( nRecordIndex, pmsg);
				m_pConference[ nGroup ].nRecordIndex = -1;
			}
		}
		if( NULL != pPlayObj )
		{
			if( NODETYPE_START_PLAY  == pPlayObj->m_nType )
			{
				TMS_Play_Result( nPlayIndex, pmsg);
				m_pConference[ nGroup ].nPlayIndex = -1;
			}
		}
		

	}
	else
	{
		//通道录放音处理
		int nIndex = TMS_GetCifLogicChnl( pmsg->ChIndex, pmsg->ChType );
		if( 0 >  nIndex || nIndex >= m_nTotalChannel ) return;
		CNodeObj *pNodeObj = m_pChannelInfo[nIndex].GetCurrentNodeObj();
		if( NULL == pNodeObj ) return;
		if( NODETYPE_START_PLAY == pNodeObj->m_nType )
		{
			TMS_Play_Result(  nIndex,  pmsg );
		}
		else if( NODETYPE_START_RECORD  == pNodeObj->m_nType )
		{
			TMS_StartRecord_Result( nIndex, pmsg);
		}
	}


	return;
}
/*/////////////////////////////////////////////////
模拟高阻线能够收到的码：
‘V’ 检测到线上有语音活动，可作为声控录音的开始
‘S’ 检测到线上语音已经停止一段时间(10秒)，可作为声控录音的暂停/结束
‘R’ 检测到线上振铃信号开始
‘U’ 检测到线上振铃信号停止
‘T’ 检测到线上振铃信号停止后没有再来下一次振铃信号，表示本次呼叫被监
听电话没有摘机应答
‘O’ 检测到被监听电话摘机，可作为压控录音的开始
‘H’ 检测到被监听电话挂机，可作为压控录音的结束
‘F’ 检测到被监听电话拍叉簧
0x80—0xFF 表示该码为一个FSK主叫号码，由[Bit6:Bit0]表示
例如：0xB1的[Bit6:Bit0]为0x31，即’1’，表示收到一个FSK方式传送的主
叫号码值1
‘f’ 检测到FSK方式传送的主叫号码载波消失。作为主叫号码结束标志，此时
号码可能送完，也可能由于被监听电话提前摘机而中断了发送
注意：在配置器的“逻辑通道/高阻模块”设置中，可以对每一条高阻线设置摘挂机
检测电压（非0值，需要有软件设置能力的硬件模块支持）,该参数将会影响高阻线
的’O’、’H’信号的检测
*///////////////////////////////////////////////////
void CTMS8E240::TMS_MSG_RecvDTMF_HB(int nIndex,RMSG *pmsg)
{
  CNodeObj *pNodeObj = m_pChannelInfo[nIndex].GetCurrentNodeObj();
  if(pmsg->DataSpec&0x80)//fsk 数据
  {
		m_strMSG.Format("%c",pmsg->DataSpec&0x7F);
		m_pChannelInfo[nIndex].m_strDTMFRecived += m_strMSG;
	 
	  //TRACE("Type= %d Ch= %d FSK=%c\n",pmsg->ChType,pmsg->ChIndex,pmsg->DataSpec&0x7F);  
  }
  else
  { 
	 if(pmsg->DataSpec=='O') //检测到被监听电话摘机，可作为压控录音的开始
	 {
			if( NODETYPE_EVENT_CHECK == pNodeObj->m_nType )
			{
				TMS_EventCheck_OffHook( nIndex,  pmsg ); 
			}
	 }
	 else if(pmsg->DataSpec=='H') //检测到被监听电话挂机，可作为压控录音的结束
	 {

		if( NODETYPE_EVENT_CHECK == pNodeObj->m_nType )
		{
			TMS_EventCheck_Release(nIndex,pmsg);
			return;
		}
		TMS_ResetChannel( nIndex );

	 }
	 else if(pmsg->DataSpec=='V')//检测到线上有语音活动，可作为声控录音的开始
	 {
			if( NODETYPE_EVENT_CHECK == pNodeObj->m_nType )
			{
				TMS_EventCheck_OffHook( nIndex,  pmsg ); 
			}

	 }
	 else if(pmsg->DataSpec=='S')//检测到线上语音已经停止一段时间(10秒)，可作为声控录音的暂停/结束
	 {
		if( NODETYPE_EVENT_CHECK == pNodeObj->m_nType )
		{
			TMS_EventCheck_Release(nIndex,pmsg);
			return;
		}
		TMS_ResetChannel( nIndex );
	 }
	 else if(pmsg->DataSpec=='R')
	 {
		m_pChannelInfo[nIndex].m_strDTMFRecived = "";
		if( NODETYPE_EVENT_CHECK == pNodeObj->m_nType )
		{
			TMS_EventCheck_Ringing(nIndex,pmsg);
			return;
		}
	 }
	 else if(pmsg->DataSpec=='f')
	 {
		//接收主叫号码结束
		 m_pChannelInfo[nIndex].m_strCallerID = m_pChannelInfo[nIndex].m_strDTMFRecived;
	 }
  }
	
	return;
}

void CTMS8E240::TMS_MSG_RecvDTMF(int nIndex,RMSG *pmsg)
{
	CNodeObj *pNodeObj = m_pChannelInfo[nIndex].GetCurrentNodeObj();
	if( NODETYPE_EVENT_CHECK == pNodeObj->m_nType )
	{
		TMS_EventCheck_RecDTMF( nIndex, pmsg );
		return;
	}
	else
	{
		int code = pmsg->DataSpec;
		switch( pmsg->DataSpec )
		{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '*':
		case '#':
			m_pChannelInfo[nIndex].m_nEventType = EVENTTYPE_RECDTMF;
			m_strMSG.Format("%c",pmsg->DataSpec);
			m_pChannelInfo[nIndex].m_strDTMFRecived += m_strMSG;
			break;
		case 'A':
		case 'B':
		case 'C':
		case 'D':
		case 'M':
		case 'V':
		case 'S':
		case 'U':
		case 'T':
		case 'O':
		case 'H':
		case 'F':
		default:
			break;
		}

	}

	return;
}

////////////////////////////////////////////
//接收DTMF
////////////////////////////////////////////
void CTMS8E240::TMS_DTMFRecive( int nIndex )
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CDTMFReciveObj *pObj = (CDTMFReciveObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDTMFReciveObj) ) )
	{
		m_strMSG.Format("DTMFRecive error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}

	m_pPubCObj->PC_ShowChannelDTMF( nIndex,pChnlInfo->m_strDTMFRecived);

	int nStep		= pChnlInfo->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			m_pPubCObj->PC_StartTimer( pChnlInfo );
			if( pObj->m_bClearDTMF ) 
			{
				pChnlInfo->m_strDTMFRecived = "";
			}
			pChnlInfo->m_strState = "ReciveDTMF";
			int  len = strlen(pChnlInfo->m_strDTMFRecived);
			if( 0 == pObj->m_nEndCondition )
			{
				//通过判断接收按键的最大数量来结束收键
				if( len  >= pObj->m_nDTMFCount )
					pChnlInfo->m_nStep = 2;
				else
					pChnlInfo->m_nStep = 1;

			}
			else
			{
				//通过判断接收特定按键值来结束收键
				if( pChnlInfo->m_strDTMFRecived.Right(1) == pObj->m_strDTMF )
				{
					pChnlInfo->m_strDTMFRecived.Replace(pObj->m_strDTMF,NULL);
					pChnlInfo->m_nStep = 2;
				}
				else
				{
					pChnlInfo->m_nStep = 1;
				}
			}
		}
		break;
	case 1:
		{
			if( "" == pChnlInfo->m_strDTMFRecived ) return;
			if( 0 == pObj->m_nEndCondition )
			{
				//通过判断接收按键的最大数量来结束收键
				int  len = pChnlInfo->m_strDTMFRecived.GetLength();
				if( len  >= pObj->m_nDTMFCount )
				{
					m_pChannelInfo[nIndex].m_nStep = 2;
					return;
				}
			}
			else
			{
				//通过判断接收特定按键值来结束收键
				if( pChnlInfo->m_strDTMFRecived.Right(1) == pObj->m_strDTMF )
				{
					pChnlInfo->m_strDTMFRecived.Replace(pObj->m_strDTMF,NULL);
					pChnlInfo->m_nStep = 2;
					return;
				}
			}
			long n = m_pPubCObj->PC_ElapseTime( pChnlInfo);
			if( n >= pObj->m_nTimeOut )
			{
				m_pPubCObj->PC_SaveValuesToVar( pChnlInfo, pChnlInfo->m_strDTMFRecived, pObj->m_SaveDTMF.m_nKey );
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				m_strMSG.Format("%sDTMFRecive error, TimeOut Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				return;
			}
			

		}
		break;
	case 2:
		{
			BOOL bResult = m_pPubCObj->PC_SaveValuesToVar( pChnlInfo, pChnlInfo->m_strDTMFRecived, pObj->m_SaveDTMF.m_nKey );
			if( bResult )
			{
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			}
			else
			{
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				m_strMSG.Format("%s DTMFRecive error,Save DTMF to Var Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			}
			
		}
	default:
		break;
	}

	return;

}

void CTMS8E240::TMS_DTMFMenu(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CDTMFMenuObj *pObj = (CDTMFMenuObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDTMFMenuObj) ) )
	{
		m_strMSG.Format("DTMFMenu Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	int nStep = pChnlInfo->m_nStep;
	switch( nStep )
	{
	case 0:
		{
			m_pPubCObj->PC_StartTimer( pChnlInfo );
			if( pObj->m_bClearDTMF )
			{
				pChnlInfo->m_strDTMFRecived = "";
			}
			pChnlInfo->m_nState  = CHANNEL_STATE_CONNECT ; 
			m_pPubCObj->PC_ShowChannelState( nIndex, "Busy" );
			pChnlInfo->m_nStep = 1;
		}
		break;
	case 1:
		{		
			if( "" == pChnlInfo->m_strDTMFRecived )
				return;

			CString strCode =  pChnlInfo->m_strDTMFRecived.Right(1) ;
			int nNextType = 0;
			if( "*" == strCode )
				nNextType = NEXTTYPE_DTMF_11;	
			else if( "#" == strCode )
				nNextType = NEXTTYPE_DTMF_10;
			else
				nNextType = atoi(strCode) + 60;
			if( nNextType >= 60 && nNextType <= 71 )
			{
				TMS_MoveNext_Node( nIndex,  nNextType );
				return;
			}
			long n = m_pPubCObj->PC_ElapseTime( pChnlInfo);
			if( n >= pObj->m_nTimeOut )
			{
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_DTMF_TIMEOUT );
				return;
			}

		}
		break;
	default:
		break;
	}
	return;

}

/////////////////////////////////////////////////////////
//通过TMS卡语音设备通道类型，取得CIF定义的的逻辑通道类型
/////////////////////////////////////////////////////////
inline int CTMS8E240::TMS_GetCifChnlType(int nDevChnlType)
{

	if( CHCLASS_NONE == nDevChnlType) //空线路类型
		return CF_CHTYPE_EMPTY;
	//else if( CHCLASS_DT  == nDevChnlType)  //数字线
	//	return CHCLASS_DT;
	//else if(  CHCLASS_AT   == nDevChnlType)  //模拟线
	//	return CHCLASS_AT;
	else if(  CHTYPE_DTNO1  == nDevChnlType)  //数字中继中国一号信令 
		return CF_CHTYPE_DTNO1;
	else if(  CHTYPE_DTNO7   == nDevChnlType)  //数字中继中国七号信令(TUP)
		return CF_CHTYPE_DTNO7;
	else if(  CHTYPE_DTDSS1   == nDevChnlType)  //数字中继ISDN PRI(30B+D)
		return CF_CHTYPE_DTDSS1;
	else if(  CHTYPE_DTNO7ISUP  == nDevChnlType) //数字中继中国七号信令(ISUP)
		return CF_CHTYPE_DTNO7ISUP;
	else if(  CHTYPE_MTP3	 == nDevChnlType) //七号信令MTP传输层通道
		return CF_CHTYPE_MTP3;
	else if(  CHTYPE_TB    == nDevChnlType)  //模拟中继外线(普通市话线),接电信局
		return CF_CHTYPE_TRUNK;
	else if(  CHTYPE_AGENT  == nDevChnlType)  //模拟中继内线(座席线)，接电话机
		return CF_CHTYPE_USER;
	else if(  CHTYPE_HB    == nDevChnlType)  //模拟中继高阻线(监控线)，与电话线/电话机相并联
		return CF_CHTYPE_RECORD;
	else
		return CF_CHTYPE_NOKNOW;
}

/////////////////////////////////////////////////////////
//通过CIF定义的的逻辑通道类型,取得TMS卡语音设备通道类型
/////////////////////////////////////////////////////////
inline int CTMS8E240::TMS_GetDevChnlType(int nCifChnlType)
{

	if( CF_CHTYPE_EMPTY == nCifChnlType) //空线路类型
		return CHCLASS_NONE;
	else if(  CF_CHTYPE_DTNO1   == nCifChnlType)  //数字中继中国一号信令 
		return CHTYPE_DTNO1;
	else if( CF_CHTYPE_DTNO7  == nCifChnlType)  //数字中继中国七号信令(TUP)
		return CHTYPE_DTNO7 ;
	else if( CF_CHTYPE_DTDSS1  == nCifChnlType)  //数字中继ISDN PRI(30B+D)
		return  CHTYPE_DTDSS1 ;
	else if( CF_CHTYPE_DTNO7ISUP == nCifChnlType) //数字中继中国七号信令(ISUP)
		return  CHTYPE_DTNO7ISUP ;
	else if(  CF_CHTYPE_MTP3 == nCifChnlType) //七号信令MTP传输层通道
		return CHTYPE_MTP3	;
	else if( CF_CHTYPE_TRUNK == nCifChnlType)  //模拟中继外线(普通市话线),接电信局
		return CHTYPE_TB  ;
	else if( CF_CHTYPE_USER == nCifChnlType)  //模拟中继内线(座席线)，接电话机
		return  CHTYPE_AGENT;
	else if( CF_CHTYPE_RECORD == nCifChnlType)  //模拟中继高阻线(监控线)，与电话线/电话机相并联
		return CHTYPE_HB  ;
	else
		return CHCLASS_NONE;
}


/////////////////////////////////////////////////////////
//通过TMS卡语音设备的通道号以及通道类型，取得CIF定义的的逻辑
//通道号
/////////////////////////////////////////////////////////
int CTMS8E240::TMS_GetCifLogicChnl(int nDevChnlIndex, int nDevChnlType)
{
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		if( nDevChnlIndex == m_pChannelInfo[i].m_nChannel )
		{
			if( m_pChannelInfo[i].m_nChanelType == TMS_GetCifChnlType(nDevChnlType) ) 
			{
				return i;
			}
		}
	}

	return -1;
}

void CTMS8E240::TMS_ShowConfState(int nGroup)
{
	if( nGroup < 0 || nGroup >= m_nTotalConf ) return;
	char state[30];
	char tmpstr[100];
	memset( state, 0, sizeof(state) );
	memset( tmpstr, 0, sizeof(tmpstr) );

	itoa( m_pConference[nGroup].nMemberNum , state, 10);//该组会议中说话的的会议成员数
	m_plstConfer->GetItemText( nGroup, 2, tmpstr, 32 ) ;
	if ( strcmp(state,tmpstr )!=0) 
		m_plstConfer->SetItemText( nGroup, 2, state );

	itoa(m_pConference[nGroup].nResNum,state,10);//该组会议中已经占用的会议资源数
	m_plstConfer->GetItemText(nGroup,1,tmpstr,32);
	if(strcmp(state,tmpstr)!=0)
		m_plstConfer->SetItemText( nGroup,1,state);

	itoa(m_pConference[nGroup].nListenNum,state,10);//该组会议中监听的的会议成员数
	m_plstConfer->GetItemText(nGroup,3,tmpstr,32);
	if(strcmp(state,tmpstr)!=0)
		m_plstConfer->SetItemText( nGroup,3,state);
		
	return;
}

void CTMS8E240::TMS_ConfDoWork()
{

	for(int i = 0; i < m_nTotalConf; i++ )
	{
		TMS_ShowConfState( i );
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
			//m_pConference[i].nConfPlayID=-1;
			//m_pConference[i].State=CONF_PLAY;
		case CONF_EXIT:
			//TMS_StopConfPlay(i);
			//TMS_StopConfRecord(i);
			//m_pConference[i].nConfPlayID=-1;
			//m_pConference[i].State=CONF_FREE;
			break;
		}
	}
	return;
}
void CTMS8E240::TMS_DoWork(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	int nChType = pChnlObj->m_nChanelType;
	CNodeObj *pNodeObj = pChnlObj->GetCurrentNodeObj();
	if( NULL == pNodeObj || nIndex < 0 ) return;
	if( CF_CHTYPE_EMPTY == nChType ) return;
	if( CF_CHTYPE_NOKNOW == nChType ) return;

	switch( pNodeObj->m_nType )	
	{
	case NODETYPE_START:
		TMS_MoveNext_Node( nIndex, NEXTTYPE_NEXT );//查找该结点分支指向的的结点
		break;
	case NODETYPE_WAITCALL:
		TMS_WaitCall( nIndex);
		break;
	case NODETYPE_START_PLAY:
		TMS_Play( nIndex );
		break;
	case NODETYPE_CHECK_DIAL:
		TMS_DialCheck( nIndex );
		break;
	case NODETYPE_RELEASECALL:
		TMS_ReleaseCall( nIndex );
		break;
	case NODETYPE_CALLHOLD:
		TMS_CallHold( nIndex );
		break;
	case NODETYPE_CHECK_TRANSFER:
		TMS_Transfer( nIndex );
		break;
	case NODETYPE_START_RECORD:
		TMS_StartRecord( nIndex );
		break;
	case NODETYPE_SETVARIABLE:
		m_pPubCObj->PC_SetVariable( nIndex,pChnlObj );
		break;
	case NODETYPE_COMPARE:
		m_pPubCObj->PC_Compare( nIndex,pChnlObj );
		break;
	case NODETYPE_ENTER_CONFERENCE:
		TMS_ConferenceEnter(nIndex);
		break;
	case NODETYPE_LEAVE_CONFERENCE:
		TMS_ConferenceLeave(nIndex);
		break;
	case NODETYPE_FAXRECIVE:
		TMS_FaxRecive(  nIndex );
		break;
	case NODETYPE_FAXSEND:
		TMS_FaxSend(  nIndex );
		break;
	case NODETYPE_CONFERENCE_BUILD:
		TMS_ConferenceBuild( nIndex );
		break;
	case NODETYPE_CONFERENCE_REQUEST:
		//Confer_Call( nChnl );
		break;
	case NODETYPE_CONFERENCE_DELETE:
		TMS_ConferenceDelete( nIndex );
		break;
	case NODETYPE_GET_CONFERENCE_MEMBER:
		break;
	case NODETYPE_OPEN_DATABASE:
		m_pPubCObj->PC_OpenDatabase( nIndex,pChnlObj );
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
		TMS_DTMFSend(nIndex);
		break;
	case NODETYPE_RECIVE_DTMF:
		TMS_DTMFRecive( nIndex );
		m_pPubCObj->PC_ShowChannelDTMF( nIndex,pChnlObj->m_strDTMFRecived);
		break;
	case NODETYPE_EVENT_CHECK:
		TMS_EventCheck_MSG( nIndex );
		return;
		break;
	case NODETYPE_DATABASE_BROWSE:
		m_pPubCObj->PC_DBBrowse( nIndex,pChnlObj );
		break;
	case NODETYPE_DTMF_MENU:
		TMS_DTMFMenu( nIndex );
		m_pPubCObj->PC_ShowChannelDTMF( nIndex,m_pChannelInfo[nIndex].m_strDTMFRecived);
		break;
	case NODETYPE_CALLLOGREQUEST:
		TMS_CallLogRequest( nIndex );
		break;
	case NODETYPE_NODE_END:
		TMS_NodeEnd( nIndex);
		break;
	case NODETYPE_SWITCH:
		TMS_Switch( nIndex );	
		break;
	case NODETYPE_ANSWERCALL:
		TMS_AnswerCall( nIndex );	
		break;
	case NODETYPE_BLOCK:
		m_pPubCObj->PC_Block( nIndex ,pChnlObj);
		break;
	case NODETYPE_ENTER:
		m_pPubCObj->PC_Enter( nIndex ,pChnlObj);
		break;
	case NODETYPE_RETURN:
		m_pPubCObj->PC_Return( nIndex,pChnlObj );
		break;
	case NODETYPE_STOREPRO:
		m_pPubCObj->PC_StoredProcedures( nIndex,pChnlObj ); 
		break;
	case NODETYPE_CHANGEVOICE:
		//TMS_ChangeVoice( nIndex );
		break;
	case NODETYPE_ENDCALL_BLOCK:
		break;
	case NODETYPE_ENDCALLEVENT_CHECK:
		TMS_EndCallEventCheckBlock( nIndex );
		break;
	case NODETYPE_ENDCALLEVENT_END:
		TMS_EndCallEventEndBlock( nIndex );
		break;
	case NODETYPE_FUNCTION:
		m_pPubCObj->PC_Function( nIndex,pChnlObj ,m_nTotalChannel, &m_pChannelInfo[0] );
		//TMS_Function( nIndex );
		break;
	case NODETYPE_EXPRESS:
		m_pPubCObj->PC_Express( nIndex ,pChnlObj);
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
		
		break;
	case NODETYPE_MOUDLE_BEGIN:
		
		break;
	case NODETYPE_MOUDLE_END:
		
		break;
	case NODETYPE_CUSTOM:
		
		break;
	default:
		break;

	}

}

void CTMS8E240::TMS_WaitCall(int nIndex, RMSG *pmsg)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CNodeObj *pNodeObj = pChnlInfo->GetCurrentNodeObj();
	if( NODETYPE_WAITCALL != pNodeObj->m_nType ) return;
	CWaitCallObj *pObj = (CWaitCallObj*)pNodeObj;
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CWaitCallObj) )  )
	{
		m_strMSG.Format("ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}


	m_pChannelInfo[nIndex].m_strCallerID="";
	m_pChannelInfo[nIndex].m_strCalledID="";


	if( pmsg->GetCallerLen() )
	{
		m_pChannelInfo[nIndex].m_strCallerID= pmsg->GetCaller() ;
	}
	if( pmsg->GetCalledLen() )
	{
		m_pChannelInfo[nIndex].m_strCalledID=pmsg->GetCalled() ;
	}
	if( pObj->m_bRecCallerID )
		m_pPubCObj->PC_SaveValuesToVar( pChnlInfo, m_pChannelInfo[nIndex].m_strCallerID, pObj->m_CallerID.m_nKey );
	if( pObj->m_bRecCalledID )
		m_pPubCObj->PC_SaveValuesToVar( pChnlInfo,m_pChannelInfo[nIndex].m_strCalledID, pObj->m_CalledID.m_nKey );
	if( pObj->m_bClearDTMF ) 
		m_pChannelInfo[nIndex].m_strDTMFRecived	= "" ;

	/****************************/
	//TMS_FaxKey( nIndex );
   /*****************************/


	m_pChannelInfo[nIndex].m_nCallType			= CHANNEL_CALLTYPE_COMEIN;
	m_pChannelInfo[nIndex].m_nRings				= 1;
	m_pChannelInfo[nIndex].m_nStep				= 1;
	m_pChannelInfo[nIndex].m_nState				= CHANNEL_STATE_CONNECT;
	m_pChannelInfo[nIndex].m_nEventType			=  EVENTTYPE_OFFHOOK;
	m_pChannelInfo[nIndex].m_strtBeginTime		= (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
	

	m_pPubCObj->PC_ShowChannelCallerID(nIndex,m_pChannelInfo[nIndex].m_strCallerID);
	m_pPubCObj->PC_ShowChannelCalledID(nIndex,m_pChannelInfo[nIndex].m_strCalledID);
	m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");

	if( pObj->m_bAnswerCall )
	{
		//应答记费
		TMS_Ack( pmsg);
	}
	else
	{
		//应答免费
		TMS_Ack( pmsg,FALSE);
	}
	

	TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );

	

	return;
}


///////////////////////////////////////////////////////////
//说明：
//对模拟外线(TB通道)摘机或对模拟内线(AGENT通道)响应(停止放信号音)。
//高阻线(HB通道)不能使用。
//对数字线路的呼入应答。计费、通话可以开始。
/////////////////////////////////////////////////
void CTMS8E240::TMS_Ack(RMSG *pmsg, BOOL bIsANC )
{
  pmsg->InitDataLen(0);
  pmsg->MsgType		= MSG_CALLEDACK;
  if(bIsANC)
	pmsg->Function	= F_ACK_ANC;//F_ACK_ANC 计费(默认值) F_ACK_ANN 免费
  else
	pmsg->Function	= F_ACK_ANN;//F_ACK_ANC 计费(默认值) F_ACK_ANN 免费
  pmsg->Param		= 0;
  PCISENDMSG( pmsg );
  return;
}

void CTMS8E240::TMS_FaxSend_Msg(int nIndex, char *filename)
{
	if( NULL == filename ) return;
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType		= MSG_MEDIA;
	msg.Function	= F_MEDIA_Tx_File;
	msg.Param		= RESPAR_PFIRSTMSG | RESPAR_PLASTMSG;
	msg.ChType		= TMS_GetDevChnlType( pChnlInfo->m_nChanelType );
	msg.ChIndex		= pChnlInfo->m_nChannel;
	msg.LinkChType	= RESTYPE_FAX;
	msg.LinkChIndex = 0;
	msg.DataSpec	= FAXFILE_TIFF;
	msg.AddStr(filename); 
	PCISENDMSG(&msg);
}

void CTMS8E240::TMS_FaxRecive_Msg(int nIndex, char *filename)
{
	if( NULL == filename ) return;
	if( 0 > nIndex || nIndex >= m_nTotalChannel  ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType		= MSG_MEDIA;
	msg.Function	= F_MEDIA_Rx_File;
	msg.Param		= RESPAR_PFIRSTMSG | RESPAR_PLASTMSG;
	msg.ChType		= TMS_GetDevChnlType( pChnlInfo->m_nChanelType );
	msg.ChIndex		= pChnlInfo->m_nChannel;
	msg.LinkChType	= RESTYPE_FAX;
	msg.LinkChIndex = 0;
	msg.DataSpec	= FAXFILE_TIFF;
	msg.AddStr(filename); 
	PCISENDMSG(&msg);
}

void CTMS8E240::TMS_Ack( int nIndex ,BOOL bIsANC )
{
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType		= MSG_CALLEDACK;
	if(bIsANC)
		msg.Function	= F_ACK_ANC;//F_ACK_ANC 计费(默认值) F_ACK_ANN 免费
	else
		msg.Function	= F_ACK_ANN;//F_ACK_ANC 计费(默认值) F_ACK_ANN 免费
	msg.Param		= 0;
	msg.ChIndex		= pChnlInfo->m_nChannel;
	msg.ChType		= TMS_GetDevChnlType( pChnlInfo->m_nChanelType );
	msg.LinkChIndex = 0;
	msg.LinkChType	= 0;
	msg.DataLen		= 0;
	msg.DataSpec	= 0;
	PCISENDMSG( &msg );


	return;
}

//创建会议
void CTMS8E240::TMS_ConF_Create( int nIndex, int nConferNO,int nMaxMember )
{
	if(  nConferNO < 0 || nConferNO >= m_nTotalConf )
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);

	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType		= MSG_CONF;
	msg.Function	= F_CF_CREATE;
	msg.Param		= 128;
	msg.ChIndex		= pChnlInfo->m_nChannel;//会议主席逻辑通道号
	msg.ChType		= TMS_GetDevChnlType( pChnlInfo->m_nChanelType );//会议主席逻辑通道类型
	msg.LinkChIndex = nConferNO;//会议室号
	if(  1 == m_nConfCard )
	{
		msg.LinkChType = RESTYPE_ECONF;//增强型电话会议
		msg.DataLen	= 2;
		msg.DataW[0] = 64;
	}
	else
	{
		msg.LinkChType = RESTYPE_CONF;//普通型电话会议，
	}
	msg.DataSpec = nMaxMember;// 会议室大小1—127（CONF）或1—31(ECONF)
	PCISENDMSG( &msg );
	return;
}

//离开会议
void CTMS8E240::TMS_ConF_Leave( int nIndex, int nConferNO )
{
	if(  nConferNO < 0 || nConferNO >= m_nTotalConf )
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);

	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType		= MSG_CONF;
	msg.Function	= F_CF_UNLINKCH;
	msg.Param		= 0;
	msg.ChIndex		= pChnlInfo->m_nChannel;//会议主席逻辑通道号
	msg.ChType		= TMS_GetDevChnlType( pChnlInfo->m_nChanelType );//会议主席逻辑通道类型
	msg.LinkChIndex = nConferNO;//会议室号
	if(  1 == m_nConfCard )
		msg.LinkChType = RESTYPE_ECONF;//增强型电话会议
	else
		msg.LinkChType = RESTYPE_CONF;//普通型电话会议，
	PCISENDMSG( &msg );
	return;
}
/********************************************************************
	进入会议
	nMode: 0 SpeakListen; 1 ListenOnly ; 2 SpeakOnly
*********************************************************************/
void CTMS8E240::TMS_ConF_Enter( int nIndex, int nConferNO, int nMode )
{
	if(  nConferNO < 0 || nConferNO >= m_nTotalConf )
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);

	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType		= MSG_CONF;
	if( 0 == nMode )
	{
		msg.Function	= F_CF_LINKCH;
		msg.Param		= 128;
	}
	else 
	{
		if( 0 == m_nConfCard )
			msg.Function	= F_CF_LINKCH;
		else
			msg.Function	= F_CF_MONITOR;
		msg.Param		= 0;
	}
	
	msg.ChIndex		= pChnlInfo->m_nChannel;//会议主席逻辑通道号
	msg.ChType		= TMS_GetDevChnlType( pChnlInfo->m_nChanelType );//会议主席逻辑通道类型
	msg.LinkChIndex = nConferNO;//会议室号
	if(  1 == m_nConfCard )
		msg.LinkChType = RESTYPE_ECONF;//增强型电话会议
	else
		msg.LinkChType = RESTYPE_CONF;//普通型电话会议，
	PCISENDMSG( &msg );
	return;
}
//关闭会议
void CTMS8E240::TMS_ConF_Close( int nIndex, int nConferNO )
{
	if(  nConferNO < 0 || nConferNO >= m_nTotalConf )
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);

	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType		= MSG_CONF;
	msg.Function	= F_CF_CLOSE;
	msg.Param		= 0;
	msg.ChIndex		= pChnlInfo->m_nChannel;//会议主席逻辑通道号
	msg.ChType		= TMS_GetDevChnlType( pChnlInfo->m_nChanelType );//会议主席逻辑通道类型
	msg.LinkChIndex = nConferNO;//会议室号
	if(  1 == m_nConfCard )
		msg.LinkChType = RESTYPE_ECONF;//增强型电话会议
	else
		msg.LinkChType = RESTYPE_CONF;//普通型电话会议，
	PCISENDMSG( &msg );
	return;
}
///////////////////////////////////////////////////////
//对模拟外线(TB通道)挂机或对模拟内线(AGENT通道)送忙音。
//高阻线(HB通道)不能使用,
//对数字线路发送释放命令，并等待释放结果.
void CTMS8E240::TMS_Release(int nIndex)
{
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType = MSG_RELEASE;
	msg.ChType  = TMS_GetDevChnlType( pChnlInfo->m_nChanelType );
	msg.ChIndex = pChnlInfo->m_nChannel;
	msg.Function=0;
	msg.Param=0;
	msg.DataLen=0;
	PCISENDMSG(&msg);


	return;
}
/*************************************************************
发送文件放音消息。
参数isfirst与islast的组合情况：
	first = 1,last = 1放音队列中只有一个文件
	first = 1,last = 0第一个文件
	first = 0,last = 1最后一个文件
	first = 0,last = 0中间的文件
nIndex 逻辑通道号
nGroup 逻辑会议号，nGroup >= 0时会议放音，nGroup < 0时是通道放音
bDTMFStop 是否按键打断
**************************************************************/
void CTMS8E240::TMS_PlayFile(int nIndex ,int nGroup, char *file, unsigned short isfirst, unsigned short islast,unsigned short bDTMFStop)
{
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ) return;
	if( nGroup >= m_nTotalConf  ) return;
	if( file == NULL ) return;
	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType		= MSG_MEDIA;
	msg.Function	= F_MEDIA_Tx_File;
	if( nGroup >= 0 ){
		
		msg.ChIndex		= nGroup;
		msg.ChType		= RESTYPE_ECONF;
	}
	else{
		msg.ChIndex		= m_pChannelInfo[nIndex].m_nChannel;
		msg.ChType		= TMS_GetDevChnlType( m_pChannelInfo[nIndex].m_nChanelType );
	}
	msg.Param		= (isfirst?RESPAR_PFIRSTMSG:0) | (bDTMFStop?RESPAR_DTMFSTOP:0) | (islast?RESPAR_PLASTMSG:0);
	//设置放音参数，是否是第一个文件，是否是最后一个文件，是否按键打断等
	msg.LinkChType	= RESTYPE_VOCPCM;
	msg.LinkChIndex	= 0;
	msg.DataSpec	= PFDATA_ALAW;//文件格式
	msg.AddStr(file); //最多127汉字或254字符

	PCISENDMSG(&msg);
}

void CTMS8E240::TMS_PlayFile(RMSG *pmsg, char *file, unsigned short isfirst, unsigned short islast, unsigned short bDTMFStop)
{
  if( file == NULL ) return;
  pmsg->InitDataLen(0);
  pmsg->MsgType		= MSG_MEDIA;
  pmsg->Function		= F_MEDIA_Tx_File;
  pmsg->Param			= (isfirst?RESPAR_PFIRSTMSG:0) | (bDTMFStop?RESPAR_DTMFSTOP:0) | (islast?RESPAR_PLASTMSG:0);
  pmsg->LinkChType	= RESTYPE_VOCPCM;
  pmsg->LinkChIndex	= PFDATA_ALAW;//文件格式
  pmsg->DataSpec		= PFDATA_ALAW;//文件格式
  pmsg->AddStr(file); //最多127汉字或254字符

  PCISENDMSG( pmsg );
}

void CTMS8E240::TMS_PlayTTS(int nIndex, int nGroup, char *str, unsigned char isfirst, unsigned char islast, unsigned short bDTMFStop)
{
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ) return;
	if( nGroup >= m_nTotalConf  ) return;
	if( str == NULL || strlen(str) >= 254 ) return;
	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType		= MSG_MEDIA;
	msg.Function	= F_MEDIA_Tx_TTStr;
	if( nGroup >= 0 ){
		
		msg.ChIndex		= nGroup;
		msg.ChType		= RESTYPE_ECONF;
	}
	else{
		msg.ChIndex		= m_pChannelInfo[nIndex].m_nChannel;
		msg.ChType		= TMS_GetDevChnlType( m_pChannelInfo[nIndex].m_nChanelType );
	}
	msg.Param		=  (isfirst?RESPAR_PFIRSTMSG:0) | (bDTMFStop?RESPAR_DTMFSTOP:0) | (islast?RESPAR_PLASTMSG:0);
	msg.LinkChType	= RESTYPE_VOCPCM;
	msg.LinkChIndex	= PFDATA_ALAW;//文件格式
	msg.AddStr(str); //最多127汉字或254字符
	PCISENDMSG(&msg);
}

void CTMS8E240::TMS_PlayTTS(RMSG *pmsg, char *str, unsigned char isfirst, unsigned char islast, unsigned short bDTMFStop)
{
	if( str == NULL || strlen(str) >= 254 ) return;
	pmsg->InitDataLen(0);
	pmsg->MsgType		= MSG_MEDIA;
	pmsg->Function	= F_MEDIA_Tx_TTStr;
	pmsg->Param		= (isfirst?RESPAR_PFIRSTMSG:0) | (bDTMFStop?RESPAR_DTMFSTOP:0) | (islast?RESPAR_PLASTMSG:0);
	pmsg->LinkChType	= RESTYPE_VOCPCM;
	pmsg->LinkChIndex	= 0;
	pmsg->AddStr(str); //最多127汉字或254字符
	PCISENDMSG(pmsg);
}

void CTMS8E240::TMS_CallLogRequest(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CCallLogRequestObj *pObj = (CCallLogRequestObj*)pChnlObj->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CCallLogRequestObj) ) )
	{
		m_strMSG.Format("CallLogRequest ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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

	TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	return;

}
////////////////////////////////////////////////////////////
//当模拟外线(TB通道)收到振铃信号或数字线路上有电话呼入
//时,用该函数处理,
///////////////////////////////////////////////////////////
void CTMS8E240::TMS_EventCheck_Ringing(int nIndex, RMSG *pmsg)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CEventCheckObj *pObj = (CEventCheckObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CEventCheckObj) ) )
	{
		m_strMSG.Format("EventCheck error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}
	
	pChnlInfo->m_nCallType	= CHANNEL_CALLTYPE_COMEIN;
	pChnlInfo->m_nEventType =  EVENTTYPE_RINGING;
	pChnlInfo->m_nState		=  CHANNEL_STATE_RINGING;
	m_pChannelInfo[nIndex].m_strRingTime = (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
	m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");

	if( 0 == pChnlInfo->m_nStep )
	{
		pChnlInfo->m_nStep = 1;
		m_pPubCObj->PC_StartTimer( pChnlInfo);
		if(  pObj->m_bClearDTMF )
		{
			if( EVENTTYPE_RECDTMF == pChnlInfo->m_nEventType )
			{
				pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
			}
			pChnlInfo->m_strDTMFRecived = "";
		}
	}

	if(  pObj->m_bElapseTime )
	{
		if( m_pPubCObj->PC_ElapseTime( pChnlInfo) >=  pObj->m_nElapseTime )
		{
			//到了指定的时间，将发生时钟事件
			m_pPubCObj->PC_StartTimer( pChnlInfo);
			pChnlInfo->m_nEventType		=  EVENTTYPE_ELAPSE_TIME;
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ELAPSE_TIME);
			return;
		}
	}
	if( pObj->m_bRinging )
	{
		int nChType = m_pChannelInfo[nIndex].m_nChanelType;
		switch( nChType )
		{
		case CF_CHTYPE_DTNO1://数字中继中国一号信令 
		case CF_CHTYPE_DTNO7://数字中继中国七号信令(TUP)
		case CF_CHTYPE_DTDSS1://数字中继ISDN PRI(30B+D)
		case CF_CHTYPE_DTNO7ISUP://数字中继中国七号信令(ISUP)
		case CF_CHTYPE_MTP3://七号信令MTP传输层通道
			{

				m_pChannelInfo[nIndex].m_strCallerID="";
				m_pChannelInfo[nIndex].m_strCalledID="";
				if( pmsg->GetCallerLen() )
				{
					m_pChannelInfo[nIndex].m_strCallerID=pmsg->GetCaller() ;
				}
				if( pmsg->GetCalledLen() )
				{
					m_pChannelInfo[nIndex].m_strCalledID= pmsg->GetCalled() ;
				}

				/****************************/
				//TMS_FaxKey( nIndex );
			   /*****************************/

				m_pChannelInfo[nIndex].m_nCallType			= CHANNEL_CALLTYPE_COMEIN;
				m_pChannelInfo[nIndex].m_nRings				= 1;
				m_pChannelInfo[nIndex].m_nStep				= 1;
				m_pChannelInfo[nIndex].m_nState				= CHANNEL_STATE_CONNECT;
				m_pChannelInfo[nIndex].m_nEventType			=  EVENTTYPE_OFFHOOK;
				m_pChannelInfo[nIndex].m_strtBeginTime		= (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
				

				m_pPubCObj->PC_ShowChannelCallerID(nIndex,m_pChannelInfo[nIndex].m_strCallerID);
				m_pPubCObj->PC_ShowChannelCalledID(nIndex,m_pChannelInfo[nIndex].m_strCalledID);
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
				
				//TMS_Acm( pmsg,0 );//发送地址全消息
				
			}

			break;
		default:
			break;
		}

		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_RINGING);
	}


	return;
}


BOOL CTMS8E240::TMS_EventCheck_Release(int nIndex, RMSG *pmsg)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CEventCheckObj *pObj = (CEventCheckObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CEventCheckObj) ) )
	{
		m_strMSG.Format("EventCheck error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return FALSE ;  
	}
	

	pChnlInfo->m_nEventType =  EVENTTYPE_HANGUP;
	if( pObj->m_bHangUp )
	{
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		return TMS_MoveNext_Node( nIndex,  NEXTTYPE_HANGUP);
	}
	else
	{
		//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		pChnlInfo->MoveCallEndBlock_Node();
	}

	return TRUE;
}

////////////////////////////////////////////////////////////
//当模拟内线(AGENT通道)上的电话在空闲时主动摘机
//时，用该函数处理
//
///////////////////////////////////////////////////////////
void CTMS8E240::TMS_EventCheck_OffHook(int nIndex, RMSG *pmsg)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CEventCheckObj *pObj = (CEventCheckObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CEventCheckObj) ) )
	{
		m_strMSG.Format("EventCheck error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}
	
	if( 0 == pChnlInfo->m_nStep )
	{
		pChnlInfo->m_nStep = 1;
		m_pPubCObj->PC_StartTimer( pChnlInfo);
		if(  pObj->m_bClearDTMF )
		{
			pChnlInfo->m_strDTMFRecived = "";
		}
	}

	if(  pObj->m_bElapseTime )
	{
		if( m_pPubCObj->PC_ElapseTime( pChnlInfo) >=  pObj->m_nElapseTime )
		{
			//到了指定的时间，将发生时钟事件
			m_pPubCObj->PC_StartTimer( pChnlInfo);
			pChnlInfo->m_nEventType		=  EVENTTYPE_ELAPSE_TIME;
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ELAPSE_TIME);
			return;
		}
	}

	//发送摘机消息
	if( CF_CHTYPE_USER == pChnlInfo->m_nChanelType )
	{
		m_pPubCObj->PC_CTI_OffHookMsg( nIndex, pChnlInfo );
	}
	pChnlInfo->m_nState =  CHANNEL_STATE_CONNECT;
	pChnlInfo->m_nEventType = EVENTTYPE_OFFHOOK;
	m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
	if( pObj->m_bOffHook )
	{
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_OFFHOOK);
	}

	return;
}

void CTMS8E240::TMS_EventCheck_RecDTMF(int nIndex, RMSG *pmsg)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CEventCheckObj *pObj = (CEventCheckObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CEventCheckObj) ) )
	{
		m_strMSG.Format("EventCheck error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}
	if(pmsg->ChType==CHTYPE_HB) return;
	if( 0 == pChnlInfo->m_nStep )
	{
		pChnlInfo->m_nStep = 1;
		m_pPubCObj->PC_StartTimer( pChnlInfo);
		if(  pObj->m_bClearDTMF )
		{
			if( EVENTTYPE_RECDTMF == pChnlInfo->m_nEventType )
			{
				pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
			}
			pChnlInfo->m_strDTMFRecived = "";
			return;
		}
	}

	if(  pObj->m_bElapseTime )
	{
		if( m_pPubCObj->PC_ElapseTime( pChnlInfo) >=  pObj->m_nElapseTime )
		{
			//到了指定的时间，将发生时钟事件
			m_pPubCObj->PC_StartTimer( pChnlInfo);
			pChnlInfo->m_nEventType		=  EVENTTYPE_ELAPSE_TIME;
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ELAPSE_TIME);
			return;
		}
	}

	if(pmsg->LinkChType==RESTYPE_DTMF)
	{
		m_strMSG.Format("%c",pmsg->DataSpec);
		pChnlInfo->m_strDTMFRecived += m_strMSG ;
		m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlInfo->m_strDTMFRecived );
		if( pObj->m_bRecDTMF )
		{
			pChnlInfo->m_nEventType		=  EVENTTYPE_RECDTMF;
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_RECDTMF);
			m_pPubCObj->PC_ShowChannelDTMF( nIndex, pChnlInfo->m_strDTMFRecived );
			return;
		}
		
	}

	return;
}

void CTMS8E240::TMS_EventCheck_MSG(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CEventCheckObj *pObj = (CEventCheckObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CEventCheckObj) ) )
	{
		m_strMSG.Format("EventCheck error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}

	if( 0 == pChnlInfo->m_nStep )
	{
		pChnlInfo->m_nStep = 1;
		m_pPubCObj->PC_StartTimer( pChnlInfo);
		if(  pObj->m_bClearDTMF )
		{
			if( EVENTTYPE_RECDTMF == pChnlInfo->m_nEventType )
			{
				pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
			}
			pChnlInfo->m_strDTMFRecived = "";
		}
	}

	if(  pObj->m_bElapseTime )
	{
		if( m_pPubCObj->PC_ElapseTime( pChnlInfo) >=  pObj->m_nElapseTime )
		{
			//到了指定的时间，将发生时钟事件
			m_pPubCObj->PC_StartTimer( pChnlInfo);
			pChnlInfo->m_nEventType		=  EVENTTYPE_ELAPSE_TIME;
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ELAPSE_TIME);
			return;
		}
	}

	switch( pChnlInfo->m_nEventType )
	{
	case EVENTTYPE_OFFHOOK:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( CF_CHTYPE_USER == pChnlInfo->m_nChanelType )
		{
			m_pPubCObj->PC_CTI_OffHookMsg( nIndex, pChnlInfo );
		}
		if( pObj->m_bOffHook )
		{
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_OFFHOOK);
			return ;
		}
		break;
	case EVENTTYPE_HANGUP:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( CF_CHTYPE_USER == pChnlInfo->m_nChanelType )
		{
			m_strMSG.Format("[Send Hangup DN:%s]", pChnlInfo->m_strLocalDN );
			m_pPubCObj->PC_ShowCommMsg( m_strMSG );
			
			
		}
		m_pPubCObj->PC_ShowChannelState( nIndex, "Free" );
		if( pObj->m_bHangUp )
		{
			
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_HANGUP);
			return ;
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
			
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_HOOKFLASH);
			return ;
		}
		break;
	case EVENTTYPE_RECDTMF:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bRecDTMF )
		{
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_RECDTMF);
			return ;
		}
		break;
	case EVENTTYPE_RECFAXTONE:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bRecFaxTone )
		{
			
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_RECFAXTONE);
			return ;
		}
		break;

	case EVENTTYPE_CONFDESTROYED:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bConfDestroyed )
		{
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_CONFDESTROYED);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_LOGIN_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentLoginMsg )
		{
			
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_LOGIN_MSG);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_OUT_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentLogOutMsg )
		{
			
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_OUT_MSG);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_BUSY_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentBusyMsg )
		{
			
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_BUSY_MSG);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_INSERVE_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentInserveMsg )
		{
			
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_INSERVE_MSG);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_ANSWER_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentAnswerMsg )
		{
			
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_ANSWER_MSG);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_MAKECALL_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentMakeCallMsg )
		{
			
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_MAKECALL_MSG);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_TRANSFER_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentTransferMsg )
		{
			
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_TRANSFER_MSG);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_CONFER_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentConferMsg )
		{
			
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_CONFER_MSG);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_HANGUP_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentHangUpMsg )
		{
			
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_HANGUP_MSG);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_SENDDATA_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentSendDataMsg )
		{
			
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_SENDDATA_MSG);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_SENDFAX_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentSendFaxMsg )
		{
			
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_SENDFAX_MSG);
			return ;
		}
		break;
	case EVENTTYPE_RINGING:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bRinging )
		{
			
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_RINGING);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_HOLD_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentHold )
		{
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_HOLD_MSG);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_PICKUPHOLD_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentPickupHold )
		{
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_PICKUPHOLD_MSG);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_MONITOR_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentMonitor )
		{
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_MONITOR_MSG);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_DISMONITOR_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentDisMonitor )
		{
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_DISMONITOR_MSG);
			return ;
		}
		break;
	case EVENTTYPE_AGENT_BUILDCONFER_MSG:
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		if( pObj->m_bAgentBuildConfer )
		{
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_AGENT_BUILDCONFER_MSG);
			return ;
		}
		break;
	default:
		break;
	
	}

	return ;
}

void CTMS8E240::TMS_EventCheck_Flash(int nIndex, RMSG *pmsg)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CEventCheckObj *pObj = (CEventCheckObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CEventCheckObj) ) )
	{
		m_strMSG.Format("EventCheck error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return;
	}

	if( 0 == pChnlInfo->m_nStep )
	{
		pChnlInfo->m_nStep = 1;
		m_pPubCObj->PC_StartTimer( pChnlInfo);
		if(  pObj->m_bClearDTMF )
		{
			pChnlInfo->m_strDTMFRecived = "";
		}
	}

	if(  pObj->m_bElapseTime )
	{
		if( m_pPubCObj->PC_ElapseTime( pChnlInfo) >=  pObj->m_nElapseTime )
		{
			//到了指定的时间，将发生时钟事件
			m_pPubCObj->PC_StartTimer( pChnlInfo);
			pChnlInfo->m_nEventType		=  EVENTTYPE_ELAPSE_TIME;
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ELAPSE_TIME);
			return;
		}
	}

	m_pPubCObj->PC_ShowChannelState( nIndex, "HookFlash");
	if( pObj->m_bHookFlash )
	{
		pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_HOOKFLASH);
		return ;
	}


	return;
}


void CTMS8E240::TMS_EndCallEventCheckBlock(int nIndex)
{
	CNodeObj *pNodeObj= m_pChannelInfo[nIndex].GetCurrentNodeObj();
    if (pNodeObj == NULL )
	{
		m_strMSG.Format("EndCallEventCheckBlock Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		return ;   
	}
	

	TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );


	return;
}

void CTMS8E240::TMS_EndCallEventEndBlock(int nIndex)
{
	CNodeObj *pNodeObj= m_pChannelInfo[nIndex].GetCurrentNodeObj();
    if (pNodeObj == NULL )
	{
		m_strMSG.Format("EndCallEventEndBlock Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		return ;   
	}



	m_pPubCObj->PC_ShowChannelState( nIndex, "Free");
	m_pPubCObj->PC_ShowConnectToChnl( nIndex, -1 );

	m_pChannelInfo[nIndex].MoveFirst_Node();

	return;
}


void CTMS8E240::TMS_NodeEnd(int nIndex)
{
	TMS_ResetChannel(nIndex);
	return;
}





int CTMS8E240::TMS_FindHoldChannel(int nIndex )
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

int CTMS8E240::TMS_FindUerChannel(CString strLocalDN )
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
int CTMS8E240::TMS_FindFreeTrunk()
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
int CTMS8E240::TMS_FindFreeChannel(CString strLocalDN, int nChType )
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


void CTMS8E240::TMS_Play(int nIndex)
{
	CPlayStartObj *pObj = (CPlayStartObj*)m_pChannelInfo[nIndex].GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CPlayStartObj) ) )
	{
		m_strMSG.Format("ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	TMS_Play_Start( nIndex, pObj );

	return;
}


void CTMS8E240::TMS_Play_Start(int nIndex,CPlayStartObj *pObj)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( 0 != pChnlInfo->m_nStep ) return;
	if( pObj->m_bClearDTMF ) 
	{
		pChnlInfo->m_strDTMFRecived = "";
	}
	
	CNodeDataList *pDadaList = &(pObj->m_PlayList);
	if( pDadaList->IsEmpty() )
	{
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		return;
	}

	int nGroupNo = -1;
	if( 1 == pObj->m_nPlayWay )//会议放音
	{
		int nVarKey = pObj->GetVarKey( pObj->m_strConfNO  );
		if( nVarKey < 0 )
		{
			nGroupNo = atoi(pObj->m_strConfNO );
		}
		else
		{
			if( !m_pPubCObj->PC_GetNorVarValues( pChnlInfo, m_strMSG, pObj->GetVarKey( pObj->m_strConfNO ) ) )
			{
				m_strMSG.Format("%s Play error, ChIndex = %d    Play error ",  pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			nGroupNo = atoi(m_strMSG);
		}

		if(  nGroupNo < 0 || nGroupNo >= m_nTotalConf )
		{
			m_strMSG.Format("%s Play error, ChIndex = %d    Group error ",  pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
	}


	/****************************/
	//TMS_FaxKey( nIndex );
   /*****************************/

	CNodeDataList PlayList;
	POSITION pos = pDadaList->GetHeadPosition( );
	//下面开始初始化播放队列
	CNodeData *pData = NULL;
	while( pos )
	{
		pData = pDadaList->GetNext(pos);
		if( NULL == pData ) continue;
		if( 0 == pData->m_nDataType )//是文件
		{
			CNodeData *pNewData = new CNodeData;
			m_pPubCObj->PC_GetExpressValues( pChnlInfo, m_strMSG, pData->m_strData);
			pNewData->m_strData = m_strMSG;
			pNewData->m_nDataType = 0;
			PlayList.AddTail(  pNewData );
		}
		else if( 1 == pData->m_nDataType )//是数字表达式
		{
			//表达式
			if( !m_pPubCObj->PC_GetExpressValues( pChnlInfo, m_strMSG, pData->m_strData ) )
			{
				m_strMSG.Format("%s Play error, ChIndex = %d    Play Number Express Error :%s ",  pObj->m_strCaption, nIndex, pData->m_strData );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;   
			}
			TMS_PlayExpressVox( nIndex, &PlayList , m_strMSG  );
		}
		else if( 2 == pData->m_nDataType )//TTS
		{
			
			if( !m_pPubCObj->PC_GetExpressValues( pChnlInfo, m_strMSG, pData->m_strData ) )
			{
				m_strMSG.Format("%s Play error, ChIndex = %d    Play TTS Express Error :%s ",  pObj->m_strCaption, nIndex, pData->m_strData );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return ;   
			}
			CNodeData *pNewData = new CNodeData;
			pNewData->m_strData = m_strMSG;
			pNewData->m_nDataType = 1;
			PlayList.AddTail(  pNewData );
		}

	}//end while

	pos = PlayList.GetHeadPosition( );	
	int nListCount = PlayList.GetCount();
	int nPos = 0;
	while( pos )
	{
		nPos++;
		pData = PlayList.GetNext(pos);
		if( NULL == pData ) continue;
		
		char *pszFileName = (LPTSTR)(LPCTSTR)pData->m_strData;
		if( 0 == pData->m_nDataType )//是文件
		{
			if( 1 == nListCount  )
			{
				TMS_PlayFile( nIndex , nGroupNo ,pszFileName , 1, 1, pObj->m_bPlayStop );//放音队列中只有一个文件
			}
			else
			{
				if( 1 == nPos )
					TMS_PlayFile( nIndex ,nGroupNo , pszFileName , 1, 0, pObj->m_bPlayStop );//第一个文件
				else 
				{
					if( nListCount <= nPos )
						TMS_PlayFile( nIndex ,nGroupNo , pszFileName , 0, 1, pObj->m_bPlayStop );//最后一个文件
					else
						TMS_PlayFile( nIndex ,nGroupNo , pszFileName , 0, 0, pObj->m_bPlayStop );//中间的文件

				}

			}

		}
		else//TTS
		{
			if( 1 == nListCount  )
			{
				TMS_PlayTTS( nIndex , nGroupNo ,pszFileName , 1, 1, pObj->m_bPlayStop );//放音队列中只有一个文件
			}
			else
			{
				if( 1 == nPos )
					TMS_PlayTTS( nIndex , nGroupNo ,pszFileName , 1, 0, pObj->m_bPlayStop );//第一个文件
				else 
				{
					if( nListCount <= nPos )
						TMS_PlayTTS( nIndex , nGroupNo ,pszFileName , 0, 1, pObj->m_bPlayStop );//最后一个文件
					else
						TMS_PlayTTS( nIndex , nGroupNo ,pszFileName , 0, 0, pObj->m_bPlayStop );//中间的文件

				}

			}

		}

	}
	
	//释放资源
	while ( PlayList.GetHeadPosition() )
	{
		delete PlayList.RemoveHead();
	}

	if( 1 == pObj->m_nPlayWay )
	{

	}
	else
	{
		
	}
	pChnlInfo->m_nStep = 1;
	return;
}

void CTMS8E240::TMS_Play_Result( int nIndex, RMSG *pmsg)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CNodeObj *pNodeObj = pChnlInfo->GetCurrentNodeObj();
	if( NODETYPE_START_PLAY != pNodeObj->m_nType ) return;
	CPlayStartObj *pObj = (CPlayStartObj*)pNodeObj;
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CPlayStartObj) ) )
	{
		m_strMSG.Format("Play error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	/***********************************/
	//TMS_TransferKey( nIndex );//加密设置
    /***********************************/

	if( F_MEDIA_Tx_Release == pmsg->Function )
	{
		if( RESTOP_FILERR == pmsg->Param )
		{
			m_strMSG.Format("%s Play error, ChIndex = %d    Play error ",  pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		}
		else
		{
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		}
	}
	else if( F_MEDIA_Error == pmsg->Function )
	{
		if( P_MEDIA_AllocOk != pmsg->Param )
		{
			m_strMSG.Format("%s Play error, ChIndex = %d    Play error %s",  pObj->m_strCaption, nIndex,GetResErrReason(pmsg->Param));
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		}
	}

	return;
}
void CTMS8E240::TMS_PlayExpressVox(int nIndex, CNodeDataList *p, CString strExpress)
{
	if( NULL == p || "" == strExpress ) return ;
	int nStrLen = strExpress.GetLength();
	CString strPaht = m_strPath+"\\SysVoiceFle\\";
	CString strFileName("");
	for( int i = 0; i< nStrLen; i++ )
	{
		strFileName = "";
		if( '0' == m_strMSG.GetAt(i) )
		{
			strFileName = strPaht + "D0";
		}
		else if( '1' == m_strMSG.GetAt(i) )
		{
			strFileName = strPaht + "D1";
		}
		else if( '2' == m_strMSG.GetAt(i) )
		{
			strFileName = strPaht + "D2";
		}
		else if( '3' == m_strMSG.GetAt(i) )
		{
			strFileName = strPaht + "D3";
		}
		else if( '4' == m_strMSG.GetAt(i) )
		{
			strFileName = strPaht + "D4";
		}
		else if( '5' == m_strMSG.GetAt(i) )
		{
			strFileName = strPaht + "D5";
		}
		else if( '6' == m_strMSG.GetAt(i) )
		{
			strFileName = strPaht + "D6";
		}
		else if( '7' == m_strMSG.GetAt(i) )
		{
			strFileName = strPaht + "D7";
		}
		else if( '8' == m_strMSG.GetAt(i) )
		{
			strFileName = strPaht + "D8";
		}
		else if( '9' == m_strMSG.GetAt(i) )
		{
			strFileName = strPaht + "D9";
		}
		else if( '.' == m_strMSG.GetAt(i) )
		{
			strFileName = strPaht + "D15";
		}

		if( "" != strFileName )
		{
			CNodeData *pp = new CNodeData;
			pp->m_strData = strFileName;
			pp->m_nDataType= 0;
			p->AddTail(pp);
		}

	}
	
	return ;
}


void CTMS8E240::TMS_DialCheck(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( 0 != pChnlInfo->m_nStep ) return;
	CDialCheckObj *pObj = (CDialCheckObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDialCheckObj) ) )
	{
		m_strMSG.Format("DialCheck Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		if( pObj->m_bError )
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	
	int nKey = pObj->GetVarKey( pObj->m_strPhone );
	CString strPhone = "";
	if( nKey < 0 )
	{
		strPhone = pObj->m_strPhone;//输入的是号码串，不是变量
	}
	else
	{
		if(  !m_pPubCObj->PC_GetNorVarValues( pChnlInfo, strPhone, nKey ) )
		{
			m_strMSG.Format("%s Dial tel Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			if( pObj->m_bError )
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
	}

	/****************************/
	//TMS_TCPIP( nIndex );
   /*****************************/
	
	char szPhone[32] ;
	memset( szPhone, 0 , sizeof(szPhone) );
	memmove( szPhone, strPhone, strlen(strPhone) >= 32 ? 31:strlen(strPhone) );
	if( strlen(szPhone) < 3 )
	{
		//号码长度小于3时，被认为号码错误
		m_strMSG.Format("%s  Phone Error; ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		if( pObj->m_bError )
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	CString strPrefix = pObj->m_strPrefix;
	char szPrefix[32];
	memset( szPrefix, 0 , sizeof(szPrefix) );
	memmove( szPrefix, strPrefix, strlen(strPrefix) >= 32 ? 31:strlen(strPrefix) );
	TMS_MakeCall( nIndex,szPhone, pChnlInfo->m_strLocalDN  );
	pChnlInfo->m_nStep			= 1;

	return;
}
/*************************************************
呼叫分机或呼叫外部电话
**************************************************/
void CTMS8E240::TMS_MakeCall(int nIndex, const char *calledstr, const char *callerstr)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nChnlType = pChnlInfo->m_nChanelType;
	RMSG msg; 
	msg.InitDataLen(0);
	msg.MsgType	= MSG_CALL;
	msg.Function= F_C_NORMAL;
	msg.Param	= P_C_NORMAL;//default param
	msg.ChType	= TMS_GetDevChnlType( nChnlType );
	msg.ChIndex	= pChnlInfo->m_nChannel;
	msg.LinkChType	= 0;
	msg.LinkChIndex	= 0;//句柄
	msg.InitCallNull();//清除主叫/被叫号码为空
	msg.AppendCalled( calledstr ); //被叫
	msg.AppendCaller( callerstr );//主叫+逗号+源被叫
	PCISENDMSG( &msg );
	m_pPubCObj->PC_StartTimer( pChnlInfo );
	pChnlInfo->m_strDTMFRecived	= "";


	pChnlInfo->m_strCallerID = callerstr ;
	pChnlInfo->m_strCalledID = calledstr ;

	if( CF_CHTYPE_USER == nChnlType )
	{
		//呼叫分机空闲，对其振铃
		pChnlInfo->m_nCallType = CHANNEL_CALLTYPE_IN;
		pChnlInfo->m_nState		= CHANNEL_STATE_RINGING;
		pChnlInfo->m_nEventType =  EVENTTYPE_RINGING;


		m_pPubCObj->PC_ShowChannelState( nIndex, "Ringing");
		m_pPubCObj->PC_CTI_RingingMsg( atoi( pChnlInfo->m_strLocalDN), callerstr, calledstr );
		
	}
	else
	{
		pChnlInfo->m_nCallType = CHANNEL_CALLTYPE_CALLOUT;
		pChnlInfo->m_nState		= CHANNEL_STATE_DIAL;
		pChnlInfo->m_nEventType	=  EVENTTYPE_OFFHOOK;
		m_pPubCObj->PC_ShowChannelState( nIndex, "Dial" );
	}
	return;
}

void CTMS8E240::TMS_Dial_Result(int nIndex, RMSG *pmsg)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( 0 == pChnlInfo->m_nStep ) return;
	CDialCheckObj *pObj = (CDialCheckObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDialCheckObj) ) )
	{
		m_strMSG.Format("DialCheck Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		if( pObj->m_bError )
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	switch( pmsg->MsgType )
	{
	case MSG_RELEASE:
		if( F_R_CALLOUTERR == pmsg->Function )//呼出失败
		{
			pChnlInfo->m_strDTMFRecived	= "";
			pChnlInfo->m_nState			= CHANNEL_STATE_FREE;
			pChnlInfo->m_nStep			= 0;
			pChnlInfo->m_nEventType		=  EVENTTYPE_HANGUP;
			pChnlInfo->m_nLinkCh			= -1;
			m_pPubCObj->PC_ShowChannelState( nIndex, "Free" );
			switch(pmsg->Param)
			{
			case P_R_NODIALTONE  :     // 1   //没有检测到拨号音 
				if( pObj->m_bNoDialTone)
				{
					TMS_MoveNext_Node( nIndex,  NEXTTYPE_NO_DIALTONE );
				}
				else
				{
					//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
					pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
					pChnlInfo->MoveCallEndBlock_Node();
				}				
				break;
			case P_R_RINGTIMEOUT   :   // 3   //振铃超时 （无人接）
			case P_R_NOANSWER     :     //2   //没有响应
				if( pObj->m_bNoAnswer)
				{
					TMS_MoveNext_Node( nIndex,  NEXTTYPE_NO_ANSWER );
				}
				else
				{
					//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
					pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
					pChnlInfo->MoveCallEndBlock_Node();
				}
				break;
			case P_R_BUSYLOCAL    :    // 19  //市话忙
			case P_R_BUSYDDD       :   // 20  //长话忙
				if( pObj->m_bNoDialTone)
				{
					TMS_MoveNext_Node( nIndex,  NEXTTYPE_BUSY );
				}
				else
				{
					//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
					pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
					pChnlInfo->MoveCallEndBlock_Node();
				}	
				break;
			case P_R_NULLDN         :   //22  //空号
			case P_R_NODIGITALPATH   : // 26  //没有数字通路
			case P_R_LINEUSED       :  // 4  //线路正被对方占用或没有复位完成
			case P_R_CONGESTION      :  //12  //设备拥塞
			case P_R_INCOMPLETE       : //15  //地址不全
			case P_R_FAILURE           ://16  //呼叫故障
			case P_R_CALLBARRING :      //18  //接入拒绝
			default://其它呼叫失败方式
				if( pObj->m_bError)
				{
					TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				}
				else
				{
					//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
					pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
					pChnlInfo->MoveCallEndBlock_Node();
				}	
				break;
			}

		}
		break;
	case MSG_CALLEDACK:
		//连接成功
		pChnlInfo->m_nState			= CHANNEL_STATE_CONNECT;
		pChnlInfo->m_strState ="Connect";
		m_pPubCObj->PC_ShowChannelState( nIndex, "Connect" );
		if( pObj->m_bConnect )
		{
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_CONNECT );
		}
		else
		{
			//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
			pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
			pChnlInfo->MoveCallEndBlock_Node();
		}
		break;
	case UPMSG_CALLOUTCHANNEL:
		if( F_CC_NOCHANNEL == pmsg->Function )
		{
			if( pObj->m_bError)
			{
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}
			else
			{
				//当此时发生挂机事件，但没有相应的处理，就直接回到初始结点
				pChnlInfo->m_nEventType = EVENTTYPE_NOTHING;
				pChnlInfo->MoveCallEndBlock_Node();
			}	
		}
	default:
		break;
	}

	return;
}


void CTMS8E240::TMS_Function(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CFunctionObj *pObj = (CFunctionObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CFunctionObj) ) )
	{
		m_strMSG.Format("Function error , ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}

	try
	{
			int nPos1	= -1;
			int nPos2	= -1;
			int nPos3	= -1;
			int nVarKey = -1;
			BOOL bResult = TRUE;
			CString strVar("");
			CString strVarValues("");
			
			
			CString strOldExpress = pObj->m_strFunction;
			strOldExpress.Replace( "\"" , NULL );
			strOldExpress.Replace("+",NULL);
			CString strNewExpress = "";
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
				strNewExpress =  strNewExpress + strOldExpress.Mid( nPos3+1, nPos1 - nPos3-1 ) ;
				nPos2 = strOldExpress.Find( ':', nPos1+1 );
				nPos3 = strOldExpress.Find( '|', nPos1+1 );
				strVar = strOldExpress.Mid(nPos1+2,nPos2-nPos1-2);
				nVarKey = atoi(strVar);
				if( 0 == nVarKey ) 
				{
					bResult = FALSE;
					break;
				}
				int nVarType = 0;
				if( !m_pPubCObj->PC_GetNorVarValues( pChnlInfo, strVarValues,nVarType, nVarKey ) )
				{
					//没有找到该变量
					bResult = FALSE;
					break;
				}
				if( "" == strVarValues ){
					if( DATATYPE_INTEGER == nVarType || DATATYPE_FLOAT == nVarType )
						strNewExpress = strNewExpress + "0";
				}
				else
					strNewExpress = strNewExpress + strVarValues;

			}//end while

			if( bResult )
			if( "" == strNewExpress ) 
			{
				//里面没有变量
				strNewExpress = strOldExpress;
			}

			strNewExpress.Replace(" ", NULL);
			CString str("");
			CString str1("");
			CString str2("");
			CString strReturn("");
			int nReturn=0;
			if( strNewExpress.Left(5) == "Left(" )
			{
				strNewExpress.Replace("Left(",NULL);
				int i = strNewExpress.Find(';');
				str = strNewExpress.Left(i);
				
				str1= strNewExpress.Mid( i );
				str1.Replace(";",NULL);
				str1.Replace(")",NULL);

				i = atoi(str1);
				strReturn = str.Left(i);

			}
			else if( strNewExpress.Left(4) == "Len(" )
			{
				strNewExpress.Replace( "Len(", NULL);
				strNewExpress.Replace( ")", NULL);
				nReturn = strlen( strNewExpress );
				strReturn.Format("%d",nReturn);

			}
			else if( strNewExpress.Left(4) == "Mid(" )
			{
				strNewExpress.Replace("Mid(",NULL);
				int i = strNewExpress.Find(';');
				str = strNewExpress.Left(i);
				int j = strNewExpress.Find(';',i+1);
				str1 = strNewExpress.Mid(i+1,j-i);
				str1.Replace(";",NULL);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
				str2 = strNewExpress.Mid(j+1);
				str2.Replace(")",NULL);   
				strReturn = str.Mid( atoi(str1), atoi(str2) );

			}
			else if( strNewExpress.Left(6) == "Right(" )
			{
				strNewExpress.Replace("Right(",NULL);
				int i = strNewExpress.Find(';');
				str = strNewExpress.Left(i);
				
				str1= strNewExpress.Mid( i );
				str1.Replace(";",NULL);
				str1.Replace(")",NULL);

				i = atoi(str1);
				strReturn = str.Right(i);
			}
			else if( strNewExpress.Left(12) == "AgentIsFree(" )
			{
				strNewExpress.Replace("AgentIsFree(",NULL);
				int i = strNewExpress.Find(';');
				str = strNewExpress.Left(i);
				
				str1= strNewExpress.Mid( i );
				str1.Replace(";",NULL);
				str1.Replace(")",NULL);
				
				i = TMS_FindUerChannel( str1 );
				strReturn.Format( "%d",i );
			}
			else if( strNewExpress.Left(12) == "GetHoldChnl(" )
			{
				int nHold = GetHoldChnl( nIndex );
				strReturn.Format( "%d",nHold );
			}
			else if( strNewExpress.Left(13) == "GetFreeAgent(" )
			{
				int nHold = TMS_FindFreeChannel( CF_CHTYPE_USER );
				strReturn.Format( "%d",nHold );
			}

			//保存返回值
			int nVarKey1 = pObj->GetVarKey( pObj->m_strReturnValues);
			if( m_pPubCObj->PC_SaveValuesToVar( pChnlInfo,strReturn, nVarKey1 ) )
			{
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
			}
			else
			{
				m_strMSG.Format("%s Function error , Save the return Values of the function  is Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			}

	}
	catch(...)
	{
		m_strMSG.Format("%s Function error ,unknown  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
	}


	return;
}

void CTMS8E240::TMS_DealCTIMsg(CFC_MESSAGE_PACK *pMsg)
{
	int nMsgType = pMsg->nMsgType;
	int nDN = pMsg->nSMID;
	
	CString strMSG("");
	strMSG.Format("%d",nDN);
	int nIndex = TMS_FindUerChannel( strMSG );
	if( nMsgType != CFMTYPE_REG_COMM )
	{
		if( 0 > nIndex  || nIndex >= m_nTotalChannel )
		{
			strMSG.Format("[Recive MType:%d][Message Error From DN:%d]", nMsgType, nDN );
			m_pPubCObj->PC_ShowSysMsg(0,strMSG);	
			return;
		}
	}

	m_lComm = pMsg->lCardKey;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	switch( nMsgType )
	{
		case CFMTYPE_MONITOR://Monitoring
			{
				//通道操作
				CFMSG_MONITOR recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, pMsg->szMessageBody ,sizeof(CFMSG_MONITOR) );
				strMSG.Format("[Recive MType:Agent Monitoring][DN:%d][DDN:%s]", pMsg->nSMID, recMsg.szTel);
				m_pPubCObj->PC_ShowCommMsg( strMSG );
				pChnlInfo->m_strDialTel.Format("%s", recMsg.szTel );
				pChnlInfo->m_nEventType	= EVENTTYPE_AGENT_MONITOR_MSG;
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
				strMSG.Format("[Recive MType:Agent MakeCall][DN:%d][DDN:%s][CallType:%d]", pMsg->nSMID, recMsg.szTel,recMsg.nCallType);
				m_pPubCObj->PC_ShowCommMsg( strMSG );

				//找到发送消息来的正确通道号码
				pChnlInfo->m_nDialType = recMsg.nCallType;
				pChnlInfo->m_strDialTel.Format("%s", recMsg.szTel );
				pChnlInfo->m_nEventType	= EVENTTYPE_AGENT_MAKECALL_MSG;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
			}
			break;

		case CFMTYPE_TRANSFER://Transfer
			{
				//通道操作
				CFMSG_TRANSFER recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, pMsg->szMessageBody ,sizeof(CFMSG_TRANSFER) );
				strMSG.Format("[Recive MType:Agent Transfer][DN:%d][DDN:%s]", pMsg->nSMID, recMsg.szTel);
				m_pPubCObj->PC_ShowCommMsg( strMSG );
				pChnlInfo->m_nDialType = recMsg.nCallType;
				pChnlInfo->m_strDialTel.Format("%s", recMsg.szTel );
				pChnlInfo->m_nEventType	= EVENTTYPE_AGENT_TRANSFER_MSG;

			}
			break;
		case CFMTYPE_CANCEL_TRANSFER://Cancel transfer
			{
				//通道操作
				pChnlInfo->m_nEventType	= EVENTTYPE_AGENT_CANCEL_TRANSFER;
				strMSG.Format("[Recive MType:Agent CancelTransfer][DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg( strMSG );
			}
			break;
		case CFMTYPE_ANSWER://Answer
			{
				//通道操作
				strMSG.Format("[Recive MType:Agent AnswerCall][DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg(strMSG );

				//找到发送消息来的正确通道号码
				//当用软电话AnswerCall时必须用
				pChnlInfo->m_nEventType	= EVENTTYPE_OFFHOOK;
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
				strMSG.Format("[Recive MType:Agent Hold][DN:%d]", pMsg->nSMID);
				m_pPubCObj->PC_ShowCommMsg( strMSG );
				pChnlInfo->m_nEventType	= EVENTTYPE_AGENT_HOLD_MSG;

			}
			break;
		case CFMTYPE_PICKUPHOLD://Pickup Hold Request
			{
				//通道操作
				CFMSG_PICKUPHOLD recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, pMsg->szMessageBody ,sizeof(CFMSG_PICKUPHOLD) );
				strMSG.Format("[Recive MType:Agent PickupHold][DN:%d]", pMsg->nSMID);
				m_pPubCObj->PC_ShowCommMsg( strMSG );
				pChnlInfo->m_nEventType	= EVENTTYPE_AGENT_PICKUPHOLD_MSG;

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
		case CFMTYPE_HANGUP:
			{
				//通道操作
				strMSG.Format("[MType:HangUp][From DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg(strMSG );
				TMS_Release( nIndex );
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
				pChnlInfo->m_nDialType = recMsg.nCallType;
				pChnlInfo->m_strDialTel.Format("%s", recMsg.szTel );
				pChnlInfo->m_nEventType	= EVENTTYPE_AGENT_CONFER_MSG;
				m_pPubCObj->PC_ShowChannelState( nIndex, "Busy");
			}
			break;
		case CFMTYPE_BUILD_CONFER:
			{
				//通道操作
				strMSG.Format("[MType:Build Conference][From DN:%d]", nDN  );
				m_pPubCObj->PC_ShowCommMsg(strMSG );

				//找到发送消息来的正确通道号码
				pChnlInfo->m_nEventType	= EVENTTYPE_AGENT_BUILDCONFER_MSG;
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



void CTMS8E240::TMS_Switch(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CSwitchObj *pObj = (CSwitchObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CSwitchObj) ) )
	{
		m_strMSG.Format("Switch Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
				TMS_Switch_Self( nIndex,  pObj );
			else
				TMS_Switch_Aim( nIndex,  pObj );
			break;
		case CF_CHSWITCH_CALLAGENT://呼叫坐席，这时的通道号为分机号码
			if(  0 == pChnlInfo->m_nStep  )
				TMS_Switch_CallAgent( nIndex,  pObj );
			else
				TMS_Switch_TimeOut( nIndex,  pObj );
			break;
		case CF_CHSWITCH_CALLCUSTOMER://呼叫客户
			if(  0 == pChnlInfo->m_nStep  )
				TMS_Switch_CallCustomer( nIndex,  pObj );
			else
				TMS_Switch_TimeOut( nIndex,  pObj );
			break;
		default:
			break;
	}
	

	return;

}

void CTMS8E240::TMS_Switch_CallAgent(int nIndex, CSwitchObj *pObj)
{
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
		m_strMSG.Format("%s not get aim telephone; ChIndex = %d ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1, "");
		}
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	nAimIndex = TMS_FindFreeChannel( strCallTel, CF_CHTYPE_USER );
	if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
	{
		m_strMSG.Format("%s Channel index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1, "");
		}
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	CFChannelInfo *pAimChnlInfo = &(m_pChannelInfo[nAimIndex]);
	if( CF_CHTYPE_USER != pAimChnlInfo->m_nChanelType )
	{
		m_strMSG.Format("%s Aim Channel not is User channel, index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1, "");
		}
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	pChnlInfo->m_nLinkCh = nAimIndex;

	//呼叫分机空闲，对其振铃
	pAimChnlInfo->m_nLinkCh		= nIndex;
	if( CF_CHTYPE_USER == nChType)
	{
		TMS_MakeCall( nAimIndex, pAimChnlInfo->m_strLocalDN, pChnlInfo->m_strLocalDN );
	}
	else
	{
		TMS_MakeCall( nAimIndex, pChnlInfo->m_strCalledID, pChnlInfo->m_strCallerID );
	}


	//switch的等待选项，一般只用于呼叫时用
	if( 0 == pObj->m_nWaitOption )//播放回铃音
	{
		TMS_StartPlaySignal(nIndex, TMS_RINGBACK_SIG);
	}
	else if( 2 == pObj->m_nWaitOption )//播放语音文件
	{
		
		char *pszFileName = (LPTSTR)(LPCTSTR)pObj->m_strPlayFile;
		TMS_PlayFile_Loop( nIndex , -1 ,pszFileName , 1, 1, false );
	}

	m_pPubCObj->PC_StartTimer( pChnlInfo );
	pChnlInfo->m_nStep = 1;
	
		
	return;
}

void CTMS8E240::TMS_Switch_CallCustomer(int nIndex, CSwitchObj *pObj)
{
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
		m_strMSG.Format("%s not get aim telephone; ChIndex = %d ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1, "");
		}
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
			m_strMSG.Format("%s Get Begin Channel error ; ChIndex = %d ",pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;

		}
		nBeginCH = atoi(strNewExpress);
		if(  !m_pPubCObj->PC_GetExpressValues( pChnlInfo,  strNewExpress, pObj->m_strEndCH ) )
		{
			m_strMSG.Format("%s Get End Channel error ; ChIndex = %d ",pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;

		}
		nEndCH = atoi(strNewExpress);
		nAimIndex = TMS_FindFreeTrunkChnl( nBeginCH, nEndCH );
	}
	else
	{
		nAimIndex = TMS_FindFreeTrunk();
	}
	if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
	{
		m_strMSG.Format("%s have not free channel ; ChIndex = %d ",pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1, "");
		}
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	
	CFChannelInfo *pAimChnlInfo = &(m_pChannelInfo[nAimIndex]);
	TMS_MakeCall( nAimIndex, strCallTel, pAimChnlInfo->m_strLocalDN  );
	pAimChnlInfo->m_nLinkCh = nIndex;
	pAimChnlInfo->m_strDialTel		= strCallTel;
	pAimChnlInfo->m_nDialType		= 1;//1外拨
	m_pPubCObj->PC_ShowChannelState( nAimIndex, "Dial" );


	if( 0 == pObj->m_nWaitOption )//播放回铃音
	{
		TMS_StartPlaySignal(nIndex, TMS_RINGBACK_SIG);
	}
	else if( 2 == pObj->m_nWaitOption )//播放语音文件
	{
		
		char *pszFileName = (LPTSTR)(LPCTSTR)pObj->m_strPlayFile;
		TMS_PlayFile_Loop( nIndex , -1 ,pszFileName , 1, 1, false );
	}

	m_pPubCObj->PC_StartTimer( pChnlInfo );
	pChnlInfo->m_strDialTel		= strCallTel;
	pChnlInfo->m_nDialType		= 1;//1外拨
	pChnlInfo->m_nLinkCh = nAimIndex;
	pChnlInfo->m_nStep = 1;

	return;
}
//判断是否超时
void CTMS8E240::TMS_Switch_TimeOut(int nIndex, CSwitchObj *pObj)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nAimIndex	=  pChnlInfo->m_nLinkCh ;
	if( 0 > nAimIndex || nAimIndex >= m_nTotalChannel  ) return;
	long n = m_pPubCObj->PC_ElapseTime( pChnlInfo);
	if( n > pObj->m_nTimeOut )
	{
		//超时或失败
		m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
		TMS_ResetChannel( nAimIndex );
		m_pChannelInfo[nIndex].m_nLinkCh = -1;
		TMS_StartPlaySignal(nIndex,TMS_BUSY_SIG);
		m_strMSG.Format("%s Switch Time out fail ; ChIndex = %d ",pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );

		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1, "");
		}
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );

	}

	return;
}
/******************************************************
	判断nIndex通道是否被其他通道switch,如果是，进行相应
	处理。
******************************************************/
void CTMS8E240::TMS_Switch_Judge(int nIndex, RMSG *pmsg)
{
	int nLinkIndex = m_pChannelInfo[nIndex].m_nLinkCh;
	if( -1 >= nLinkIndex || nLinkIndex >= m_nTotalChannel  ) return;
	CNodeObj * pObj = m_pChannelInfo[nLinkIndex].GetCurrentNodeObj();
	if( NULL == pObj ) return;
	if( NODETYPE_SWITCH != pObj->m_nType ) return;
	TMS_Switch_Result(  nLinkIndex,  pmsg , pObj );//检查Switch结果
	return;
}

void CTMS8E240::TMS_Switch_Result(int nIndex, RMSG *pmsg, CNodeObj * pNewObj )
{
	if( 0 == m_pChannelInfo[nIndex].m_nStep ) return;
	CSwitchObj *pObj = (CSwitchObj*)pNewObj;
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CSwitchObj) ) )
	{
		m_strMSG.Format("Switch Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	int nAimIndex	= m_pChannelInfo[nIndex].m_nLinkCh ;
	switch( pmsg->MsgType )
	{
	case MSG_RELEASE:
		if( F_R_CALLOUTERR == pmsg->Function )//呼出失败
		{
			switch(pmsg->Param)
			{
			case P_R_NODIALTONE  :     // 1   //没有检测到拨号音 
			case P_R_RINGTIMEOUT   :   // 3   //振铃超时 （无人接）
			case P_R_NOANSWER     :     //2   //没有响应
			case P_R_BUSYLOCAL    :    // 19  //市话忙
			case P_R_BUSYDDD       :   // 20  //长话忙
			case P_R_NULLDN         :   //22  //空号
			case P_R_NODIGITALPATH   : // 26  //没有数字通路
			case P_R_LINEUSED       :  // 4  //线路正被对方占用或没有复位完成
			case P_R_CONGESTION      :  //12  //设备拥塞
			case P_R_INCOMPLETE       : //15  //地址不全
			case P_R_FAILURE           ://16  //呼叫故障
			case P_R_CALLBARRING :      //18  //接入拒绝
			default://其它呼叫失败方式
				m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
				TMS_ResetChannel( nAimIndex );
				m_pChannelInfo[nIndex].m_nLinkCh = -1;
				TMS_StartPlaySignal(nIndex,TMS_BUSY_SIG);
				m_strMSG.Format("%s Switch  fail, %s ; ChIndex = %d ",pObj->m_strCaption, GetReleaseParamNameStr(pmsg->Param) , nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
				{
					m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1,"");
				}
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				break;
			}

		}
		break;
	case MSG_CALLEDACK:
		//连接成功
		TMS_MySetLink( nIndex, nAimIndex, CF_CHSWITCH_CONNECT );
		if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
		{
			m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 0, "");
		}
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		break;
	case UPMSG_CALLOUTCHANNEL:
		if( F_CC_NOCHANNEL == pmsg->Function )
		{
			//本呼出分配不到呼出通道，呼叫终止，通道释放
			m_pChannelInfo[nAimIndex].m_nLinkCh = -1;
			TMS_ResetChannel( nAimIndex );
			m_pChannelInfo[nIndex].m_nLinkCh = -1;
			TMS_StartPlaySignal(nIndex,TMS_BUSY_SIG);
			m_strMSG.Format("%s Switch  fail,%s; ChIndex = %d ",pObj->m_strCaption, GetCallOutChannelParamNameStr(pmsg->Param), nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			if( CF_CHTYPE_USER == m_pChannelInfo[nIndex].m_nChanelType )
			{
				m_pPubCObj->PC_CTI_MakeCallResponseMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1, "");
			}
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		}
	default:
		break;
	}

	return;
}

void CTMS8E240::TMS_MyClearLink(int nIndex1, int nIndex2)
{
	TMS_Router( nIndex1,  nIndex1, F_R_TALK );
	TMS_Router( nIndex2,  nIndex2, F_R_TALK );
	return;
}
void CTMS8E240::TMS_MySetLink(int nIndex1, int nIndex2, int nSwitchType)
{
	
	if( CF_CHSWITCH_CONNECT == nSwitchType )
	{
		TMS_Router( nIndex1,  nIndex2, F_R_TALK );
	}
	else if( CF_CHSWITCH_MONITOR == nSwitchType )
	{
		TMS_Router( nIndex1,  nIndex2, F_R_MONITOR );
	}
	else 
	{
		//对方能听见
		TMS_Router( nIndex2,  nIndex1, F_R_MONITOR );
	}

	m_pChannelInfo[nIndex1].m_nLinkType = nSwitchType;
	m_pChannelInfo[nIndex2].m_nLinkType = nSwitchType;
	m_pPubCObj->PC_ShowConnectToChnl(nIndex1,nIndex2);
	m_pPubCObj->PC_ShowConnectToChnl(nIndex2,nIndex1);

	return ;
}
/***************************************************************
nSwitchType =
  F_R_SILENCE           0   //通道输出静音
  F_R_MONITOR           1   //单向监听
  F_R_TALK              2   //双向通话
  F_R_TALK_RP		   3   //双向通话并通话中可混合录放语音	
  F_R_MONITOR_P		   4   //强制连接到卡内放音资源		   
  F_R_TRANSTO_CT        13  //直接发送到CT总线一个时隙 
  F_R_MONITOR_CT        14  //直接监听CT总线一个时隙 
***************************************************************/
void CTMS8E240::TMS_Router(int nIndex1, int nIndex2, int nSwitchType)
{
	if( -1 >= nIndex1 || nIndex1 >= m_nTotalChannel  ) return;
	if( -1 >= nIndex2 || nIndex2 >= m_nTotalChannel  ) return;
	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType = DOWNMSG_ROUTER;
	msg.Function = nSwitchType;
	msg.ChType  = TMS_GetDevChnlType( m_pChannelInfo[nIndex1].m_nChanelType );
	msg.ChIndex = m_pChannelInfo[nIndex1].m_nChannel;
	msg.LinkChType  = TMS_GetDevChnlType( m_pChannelInfo[nIndex2].m_nChanelType );
	msg.LinkChIndex = m_pChannelInfo[nIndex2].m_nChannel;
	msg.DataLen = 0;
	msg.DataSpec = 0;
	PCISENDMSG(&msg);
	return;
}

int CTMS8E240::TMS_FindFreeChannel(int nChnlType)
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
/**********************************************************
  TMS_DIAL_SIG     1 //拨号音
  TMS_RINGBACK_SIG 2 //回铃音
  TMS_BUSY_SIG     3 //忙音
**********************************************************/
void CTMS8E240::TMS_StartPlaySignal(RMSG *pmsg, int nSigType)
{
	pmsg->InitDataLen(0);
	pmsg->MsgType=MSG_MEDIA;
	pmsg->Function=F_MEDIA_Tx_CPSign;
	pmsg->LinkChType=RESTYPE_VOCPCM;
	if( TMS_DIAL_SIG == nSigType  )
	{
		//拨号音
		pmsg->Param		=  RESPAR_PFIRSTMSG | RESPAR_PLASTMSG | RESPAR_PLOOPMSG | RESPAR_DTMFSTOP;
	}
	else 
	{
		pmsg->Param		=  RESPAR_PFIRSTMSG | RESPAR_PLASTMSG | RESPAR_PLOOPMSG;
	}
	pmsg->LinkChIndex=0;
	pmsg->AddByte( nSigType );
    PCISENDMSG(pmsg);
  return;
}

/**********************************************************
  TMS_DIAL_SIG     1 //拨号音
  TMS_RINGBACK_SIG 2 //回铃音
  TMS_BUSY_SIG     3 //忙音
**********************************************************/
void CTMS8E240::TMS_StartPlaySignal(int nIndex, int nSigType)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ) return;
	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType		= MSG_MEDIA;
	msg.ChIndex		= pChnlInfo->m_nChannel;
	msg.LinkChType	= RESTYPE_VOCPCM;
	msg.ChType		= TMS_GetDevChnlType( pChnlInfo->m_nChanelType );
	msg.Function	= F_MEDIA_Tx_CPSign;
	if( TMS_DIAL_SIG == nSigType  )
	{
		//拨号音
		msg.Param		=  RESPAR_PFIRSTMSG | RESPAR_PLASTMSG | RESPAR_PLOOPMSG | RESPAR_DTMFSTOP;
	}
	else 
	{
		msg.Param		=  RESPAR_PFIRSTMSG | RESPAR_PLASTMSG | RESPAR_PLOOPMSG;
	}
	msg.LinkChIndex=0;
	msg.AddByte( nSigType );
	PCISENDMSG(&msg);
}

void CTMS8E240::TMS_StopPlay(RMSG *pmsg)
{
  pmsg->InitDataLen(0);
  pmsg->MsgType=MSG_MEDIA;
  pmsg->Function=F_MEDIA_Tx_Release;
  pmsg->Param=0;
  pmsg->LinkChType=RESTYPE_VOCPCM;
  pmsg->LinkChIndex=0;
  PCISENDMSG(pmsg);
  return;
}
/*************************************************************
nIndex 逻辑通道号
nGroup 逻辑会议号，nGroup >= 0时会议放音，nGroup < 0时是通道放音
**************************************************************/
void CTMS8E240::TMS_StopPlay(int nIndex,int nGroup )
{
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ) return;
	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType		= MSG_MEDIA;
	if( nGroup >= 0 ){
		
		msg.ChIndex		= nGroup;
		msg.ChType		= RESTYPE_ECONF;
	}
	else{
		msg.ChIndex		= m_pChannelInfo[nIndex].m_nChannel;
		msg.ChType		= TMS_GetDevChnlType( m_pChannelInfo[nIndex].m_nChanelType );
	}
	msg.Function=F_MEDIA_Tx_Release;
	msg.Param=0;
	msg.LinkChType=RESTYPE_VOCPCM;
	msg.LinkChIndex=0;
	PCISENDMSG(&msg);
	return;
}

void CTMS8E240::TMS_Switch_Self(int nIndex, CSwitchObj *pObj)
{
	if( nIndex < 0 || nIndex >= m_nTotalChannel  ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nAimIndex = m_pChannelInfo[nIndex].m_nLinkCh;
	if( nIndex < 0 || nIndex >= m_nTotalChannel  )
	{
		m_strMSG.Format("%s have not the Channel that link to self . index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	int nSwitchType = pObj->m_nSwitchType;
	switch( nSwitchType )
	{
		case CF_CHSWITCH_CONNECT://直接连接
		case CF_CHSWITCH_MONITOR://监听
		case CF_CHSWITCH_SPEAK://别人能听见，但听不见别人
			TMS_MySetLink(nIndex, nAimIndex, pObj->m_nSwitchType  );
			break;
		case CF_CHSWITCH_DISCONNECT://断开连接	
			TMS_MyClearLink(nIndex, nAimIndex );
			pChnlInfo->m_nLinkType = -1;
			m_pChannelInfo[nAimIndex].m_nLinkType = -1;
			m_pPubCObj->PC_ShowConnectToChnl(nIndex,-1);
			m_pPubCObj->PC_ShowConnectToChnl(nAimIndex,-1);
			break;
		default:
			break;
	}
	m_pPubCObj->PC_StartTimer( pChnlInfo );
	pChnlInfo->m_nStep = 1;
	return;
}

void CTMS8E240::TMS_Switch_Aim(int nIndex, CSwitchObj *pObj)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nVarKey = pObj->GetVarKey( pObj->m_strAimChnl );
	if(  nVarKey < 0 )
	{
		m_strMSG = pObj->m_strAimChnl;
	}
	else if( !m_pPubCObj->PC_GetNorVarValues( pChnlInfo, m_strMSG, nVarKey ) )
	{	
		m_strMSG.Format("%s Channel index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	int nAimIndex = atoi(m_strMSG);
	if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
	{
		m_strMSG.Format("%s Channel index is  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	int nSwitchType = pObj->m_nSwitchType;
	switch( nSwitchType )
	{
		case CF_CHSWITCH_CONNECT://直接连接
		case CF_CHSWITCH_MONITOR://监听
		case CF_CHSWITCH_SPEAK://别人能听见，但听不见别人
			TMS_MySetLink(nIndex, nAimIndex, pObj->m_nSwitchType  );
			pChnlInfo->m_nLinkCh = nAimIndex;
			if( CF_CHSWITCH_MONITOR != pObj->m_nSwitchType)
			{
				m_pChannelInfo[nAimIndex].m_nLinkCh = nIndex;
				pChnlInfo->m_nState = CHANNEL_STATE_CONNECT;
			}
			else
			{
				pChnlInfo->m_nState = CHANNEL_STATE_MONITORING;
			}
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			return;
		case CF_CHSWITCH_DISCONNECT://断开连接	
			TMS_MyClearLink(nIndex, nAimIndex ) ;
			pChnlInfo->m_nLinkType = -1;
			m_pChannelInfo[nAimIndex].m_nLinkType = -1;
			m_pPubCObj->PC_ShowConnectToChnl(nIndex,-1);
			m_pPubCObj->PC_ShowConnectToChnl(nAimIndex,-1);
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
			return;
		default:
			break;
	}
	return;	
}

void CTMS8E240::TMS_Transfer(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlInfo->m_nChanelType ) return;
	CTransferCheckObj *pObj = (CTransferCheckObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CTransferCheckObj) ) )
	{
		m_strMSG.Format("TransferStart Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	switch( pObj->m_nTransferType )
	{
	case 0://开始转移
		TMS_Transfer_Start( nIndex ,pObj );
		break;
	case 1://取消转移
		TMS_Transfer_Cancel( nIndex ,pObj );
		break;
	case 2://完成转移
		{
				switch( pObj->m_nTransferAim )
				{
				case 0:
					if(  0 == pChnlInfo->m_nStep )
						TMS_Transfer_ToAgent( nIndex ,pObj );
					else
					    TMS_Transfer_CancelCheck( nIndex ,pObj );
					break;
				case 1:
					if(  0 == pChnlInfo->m_nStep )
						TMS_Transfer_ToCustomer( nIndex  ,pObj );
					else
						TMS_Transfer_CancelCheck( nIndex ,pObj );
					break;
				case 2:
					break;
				default:
					break;
				}
				/******************************************************************/
				//TMS_TransferKey( nIndex );//加密设置
				/******************************************************************/
		}
		break;
	default:
		break;
	}

	return;
}

void CTMS8E240::TMS_Transfer_Start(int nIndex, CTransferCheckObj *pObj)
{
	if( CF_CHTYPE_USER != m_pChannelInfo[nIndex].m_nChanelType ) return;
	int nLinkIndex =  m_pChannelInfo[nIndex].m_nLinkCh;
	int nDN = atoi( m_pChannelInfo[nIndex].m_strLocalDN );
	if( nLinkIndex <= -1 )
	{
		m_strMSG.Format("%s Have not active call ;ChIndex = %d", pObj->m_strCaption, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		CTI_TransferMsg( nDN, 1, "" );//执行错误
		return;
	}
	if( pObj->m_bClearDTMF )
	{
		m_pChannelInfo[nIndex].m_strDTMFRecived = "";
	}
	TMS_MyClearLink(nIndex,nLinkIndex );
	if( 1 == pObj->m_nWaitOption )
	{
		//循环放音
		char *pszFileName = (LPTSTR)(LPCTSTR)pObj->m_strPlayFile;
		TMS_PlayFile_Loop( nLinkIndex, -1, pszFileName, 1, 1, false );
		m_pChannelInfo[nLinkIndex].m_strPlayFile = pObj->m_strPlayFile;
	}

	m_pChannelInfo[nLinkIndex].m_nState = CHANNEL_STATE_TRANSFER;
	m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Busy" );
	TMS_StartPlaySignal( nIndex, TMS_DIAL_SIG );
	TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);


	return;
}

void CTMS8E240::TMS_Transfer_Cancel(int nIndex, CTransferCheckObj *pObj)
{
	
	int nLinkIndex =  m_pChannelInfo[nIndex].m_nLinkCh;
	if( nLinkIndex < 0 || nLinkIndex >= m_nTotalChannel  )
	{
		m_strMSG.Format("%s Have not active call ;ChIndex = %d", pObj->m_strCaption, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		CTI_CancelTransferMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 1, "" );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	if( pObj->m_bClearDTMF )
	{
		m_pChannelInfo[nIndex].m_strDTMFRecived = "";
	}
	m_pChannelInfo[nLinkIndex].m_nState = CHANNEL_STATE_CONNECT;
	m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Busy");
	TMS_MySetLink( nIndex, nLinkIndex , CF_CHSWITCH_CONNECT );
	CTI_CancelTransferMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 0, "" );
	TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
	return;
}

void CTMS8E240::TMS_Transfer_ToCustomer(int nIndex, CTransferCheckObj *pObj)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( pChnlInfo->m_nStep > 0 ) return;
	if( CF_CHTYPE_USER != pChnlInfo->m_nChanelType ) return;
	int nDN = atoi( pChnlInfo->m_strLocalDN );
	int nLinkIndex =  pChnlInfo->m_nLinkCh;
	if( nLinkIndex < 0 || nLinkIndex >= m_nTotalChannel  )
	{
		m_strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		m_pPubCObj->PC_CTI_TransferResponseMsg( nDN, 1, "" );//执行错误
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		
		return;
	}
	if( pObj->m_bClearDTMF )
	{
		pChnlInfo->m_strDTMFRecived = "";
	}

	CString strPhone	= pObj->m_strExpression;//呼叫转移到外线时，目标号码必须从表达式获取
	int nKey = pObj->GetVarKey(strPhone);
	if(  !m_pPubCObj->PC_GetNorVarValues( pChnlInfo,  strPhone, nKey ) )
	{
		m_strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		m_pPubCObj->PC_CTI_TransferResponseMsg( nDN, 1, "" );//执行错误
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		
		return;
	}
	
	//已取得电话号码
	int nAimIndex = TMS_FindFreeTrunk();
	if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
	{
		m_strMSG.Format("%s have not free channel ; ChIndex = %d ",pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		m_pPubCObj->PC_CTI_TransferResponseMsg( nDN, 1, "" );//执行错误
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	TMS_MakeCall( nAimIndex, (LPSTR)(LPCSTR)strPhone, pChnlInfo->m_strLocalDN );
	pChnlInfo->m_nTransferCh	= nAimIndex;
	m_pChannelInfo[nAimIndex].m_nLinkCh = nIndex;
	TMS_StartPlaySignal(nIndex, TMS_RINGBACK_SIG);
	m_pPubCObj->PC_StartTimer( pChnlInfo );
	pChnlInfo->m_nStep = 1;

	return;
}

void CTMS8E240::TMS_Transfer_ToAgent(int nIndex, CTransferCheckObj *pObj)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( pChnlInfo->m_nStep > 0 ) return;
	if( CF_CHTYPE_USER != pChnlInfo->m_nChanelType ) return;
	int nDN = atoi( pChnlInfo->m_strLocalDN );
	int nLinkIndex =  pChnlInfo->m_nLinkCh;
	if( nLinkIndex < 0 || nLinkIndex >= m_nTotalChannel  )
	{
		m_strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		m_pPubCObj->PC_CTI_TransferResponseMsg( nDN, 1, "" );//执行错误
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		
		return;
	}
	if( pObj->m_bClearDTMF )
	{
		pChnlInfo->m_strDTMFRecived = "";
	}
	//转移到内线通道
	CString strPhone	= pObj->m_strExpression;
	int nKey = pObj->GetVarKey(strPhone);
	if(  !m_pPubCObj->PC_GetNorVarValues( pChnlInfo,  strPhone, nKey ) )
	{
		m_strMSG.Format("%s Get telnumber Error ;ChIndex = %d", pObj->m_strCaption, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		m_pPubCObj->PC_CTI_TransferResponseMsg( nDN, 1, "" );//执行错误
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	//已取得电话号码
	int nAimIndex = TMS_FindFreeChannel( strPhone, CF_CHTYPE_USER  );//通过分机号码查找该分机
	if( -1 >= nAimIndex || nAimIndex >= m_nTotalChannel  )
	{
		//如果该通道不是内线，认为是错误的
		m_strMSG.Format("%s Channel index is error; ChIndex = %d ",  pObj->m_strCaption, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		m_pPubCObj->PC_CTI_TransferResponseMsg( nDN, 1, "" );//执行错误
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	CFChannelInfo *pAimChnlInfo = &(m_pChannelInfo[nAimIndex]);
	//呼叫分机空闲，对其振铃
	pChnlInfo->m_nTransferCh	= nAimIndex;
	pAimChnlInfo->m_nLinkCh		= nIndex;
	TMS_MakeCall( nAimIndex, pChnlInfo->m_strCalledID, pChnlInfo->m_strCallerID );
	TMS_StartPlaySignal(nIndex, TMS_RINGBACK_SIG);
	m_pPubCObj->PC_ShowChannelState( nAimIndex, "Ringing");
	m_pPubCObj->PC_StartTimer(pChnlInfo);
	pChnlInfo->m_nStep = 1;


	return;
}
/*************************************************************
发送循环文件放音消息，本放音消息为循环放音消息，系统将重复播放
该语音，直到用户停止或重新启动该通道的放音
参数isfirst与islast的组合情况：
	first = 1,last = 1放音队列中只有一个文件
	first = 1,last = 0第一个文件
	first = 0,last = 1最后一个文件
	first = 0,last = 0中间的文件
nIndex 逻辑通道号
nGroup 逻辑会议号，-1通道放音，>=1表示是增强会议放音的组号
bDTMFStop 是否按键打断
**************************************************************/
void CTMS8E240::TMS_PlayFile_Loop(int nIndex, int nGroup, char *file, unsigned short isfirst, unsigned short islast, unsigned short bDTMFStop)
{
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ) return;
	if( nGroup >= m_nTotalConf  ) return;
	if( file == NULL ) return;
	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType		= MSG_MEDIA;
	msg.Function	= F_MEDIA_Tx_File;
	if( nGroup >= 0 ){
		
		msg.ChIndex		= nGroup;
		msg.ChType		= RESTYPE_ECONF;
	}
	else{
		msg.ChIndex		= m_pChannelInfo[nIndex].m_nChannel;
		msg.ChType		= TMS_GetDevChnlType( m_pChannelInfo[nIndex].m_nChanelType );
	}
	msg.Param		= RESPAR_PLOOPMSG | (isfirst?RESPAR_PFIRSTMSG:0) | (bDTMFStop?RESPAR_DTMFSTOP:0) | (islast?RESPAR_PLASTMSG:0);
	//设置放音参数，是否是第一个文件，是否是最后一个文件，是否按键打断等
	msg.LinkChType	= RESTYPE_VOCPCM;
	msg.LinkChIndex	= 0;
	msg.DataSpec	= PFDATA_ALAW;//文件格式
	msg.AddStr(file); //最多127汉字或254字符

	PCISENDMSG(&msg);
}
void CTMS8E240::TMS_Transfer_Judge(int nIndex, RMSG *pmsg)
{
	int nLinkIndex = m_pChannelInfo[nIndex].m_nLinkCh;
	if( -1 >= nLinkIndex || nLinkIndex >= m_nTotalChannel  ) return;
	CNodeObj * pObj = m_pChannelInfo[nLinkIndex].GetCurrentNodeObj();
	if( NULL == pObj ) return;
	if( NODETYPE_CHECK_TRANSFER != pObj->m_nType ) return;
	TMS_Transfer_Result(  nLinkIndex,  pmsg , pObj );//检查Switch结果
	return;
}
void CTMS8E240::TMS_Transfer_Result(int nIndex, RMSG *pmsg, CNodeObj *pNewObj)
{
	if( 0 == m_pChannelInfo[nIndex].m_nStep ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CTransferCheckObj *pObj = (CTransferCheckObj*)pNewObj;
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CTransferCheckObj) ) )
	{
		m_strMSG.Format("TransferStart Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	int nAimIndex	= pChnlInfo->m_nTransferCh ;
	int nLinkIndex	= pChnlInfo->m_nLinkCh ;
	int nDN = atoi( pChnlInfo->m_strLocalDN );
	switch( pmsg->MsgType )
	{
	case MSG_RELEASE:
		if( F_R_CALLOUTERR == pmsg->Function )//呼出失败
		{
			switch(pmsg->Param)
			{
			case P_R_NODIALTONE  :     // 1   //没有检测到拨号音 
			case P_R_RINGTIMEOUT   :   // 3   //振铃超时 （无人接）
			case P_R_NOANSWER     :     //2   //没有响应
			case P_R_BUSYLOCAL    :    // 19  //市话忙
			case P_R_BUSYDDD       :   // 20  //长话忙
			case P_R_NULLDN         :   //22  //空号
			case P_R_NODIGITALPATH   : // 26  //没有数字通路
			case P_R_LINEUSED       :  // 4  //线路正被对方占用或没有复位完成
			case P_R_CONGESTION      :  //12  //设备拥塞
			case P_R_INCOMPLETE       : //15  //地址不全
			case P_R_FAILURE           ://16  //呼叫故障
			case P_R_CALLBARRING :      //18  //接入拒绝
			default://其它呼叫失败方式
				m_pChannelInfo[nAimIndex].m_nLinkCh	= -1;
				TMS_ResetChannel(nAimIndex);
				pChnlInfo->m_nTransferCh		= -1;

				m_strMSG.Format("%s Call out fail ; ChIndex = %d ",pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				m_pPubCObj->PC_CTI_TransferResponseMsg( nDN, 1, "" );//执行错误
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;	
			}

		}
		break;
	case MSG_CALLEDACK:
		//连接成功
		if( 1 == pObj->m_nWaitOption )
			TMS_StopPlay( nLinkIndex,-1 );
		TMS_MySetLink( nLinkIndex, nAimIndex , CF_CHSWITCH_CONNECT );

		m_pChannelInfo[nAimIndex].m_nTransferCh		= -1;
		m_pChannelInfo[nAimIndex].m_nLinkCh			= nLinkIndex;

		m_pChannelInfo[nLinkIndex].m_nState = CHANNEL_STATE_CONNECT;
		m_pChannelInfo[nLinkIndex].m_nTransferCh	= -1;
		m_pChannelInfo[nLinkIndex].m_nLinkCh		= nAimIndex;

		pChnlInfo->m_nTransferCh		= -1;
		pChnlInfo->m_nLinkCh			= -1;
		TMS_StartPlaySignal( nIndex, TMS_BUSY_SIG );
		m_pPubCObj->PC_CTI_TransferResponseMsg( nDN, 0, "" );//执行错误
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		break;
	case UPMSG_CALLOUTCHANNEL:
		if( F_CC_NOCHANNEL == pmsg->Function )
		{
			//本呼出分配不到呼出通道，呼叫终止，通道释放
			m_pChannelInfo[nAimIndex].m_nLinkCh	= -1;
			TMS_ResetChannel(nAimIndex);
			pChnlInfo->m_nTransferCh		= -1;

			m_strMSG.Format("%s Call out fail ; ChIndex = %d ",pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			m_pPubCObj->PC_CTI_TransferResponseMsg( nDN, 1, "" );//执行错误
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;	
		}
	default:
		break;
	}

	return;
}

void CTMS8E240::TMS_Transfer_CancelCheck(int nIndex, CTransferCheckObj *pObj)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nAimIndex	=  pChnlInfo->m_nTransferCh ;
	if( 0 > nAimIndex || nAimIndex >= m_nTotalChannel  ) return;
	int nDN = atoi( pChnlInfo->m_strLocalDN );
	BOOL bCancel = FALSE;
	if( EVENTTYPE_AGENT_CANCEL_TRANSFER == pChnlInfo->m_nEventType )
		bCancel = TRUE;
	else
	{
		//看是否收到取消转移的按键
		if( "" != pChnlInfo->m_strDTMFRecived  )
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
		m_pChannelInfo[nAimIndex].m_nLinkCh	= -1;
		TMS_ResetChannel(nAimIndex);
		pChnlInfo->m_nTransferCh = -1;
		CTI_CancelTransferMsg( atoi(m_pChannelInfo[nIndex].m_strLocalDN), 0, "" );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_CANCEL_TRANSFER );//取消转移
	}

	/////////////////////////////
	long n = m_pPubCObj->PC_ElapseTime( pChnlInfo);
	if( n > 30 )
	{
		//超时或失败
		m_pChannelInfo[nAimIndex].m_nLinkCh	= -1;
		TMS_ResetChannel(nAimIndex);
		pChnlInfo->m_nTransferCh = -1;

		m_strMSG.Format("%s Transfer Call out fail ; ChIndex = %d ",pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		CTI_CancelTransferMsg( nDN, 1, "" );//执行错误
		return;
	}

	return;
}

void CTMS8E240::TMS_DTMFSend(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( pChnlInfo->m_nStep > 0 ) return;
	CDTMFSendObj *pObj = (CDTMFSendObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CDTMFSendObj) ) )
	{
		m_strMSG.Format("DTMF Send Error, ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;
	}
	CString strNewExpress("");
	if(  !m_pPubCObj->PC_GetExpressValues( pChnlInfo,  strNewExpress, pObj->m_strDTMF ) )
	{
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_strMSG.Format("%s DTMF Send Express  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		return;
	}

	char *pszDTMF = (LPTSTR)(LPCTSTR)strNewExpress;
	TMS_DTMFSend_MSG( nIndex, pszDTMF );
	TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );

	return;
}


inline void CTMS8E240::TMS_DTMFSend_MSG(int nIndex, char *szDTMF )
{
	if( NULL == szDTMF ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType		= MSG_MEDIA;
	msg.Function	= F_MEDIA_Tx_DTMF;
	msg.Param		= RESPAR_PFIRSTMSG |RESPAR_PLASTMSG ;
	msg.ChIndex		= pChnlInfo->m_nChannel;
	msg.ChType		= TMS_GetDevChnlType( pChnlInfo->m_nChanelType );
	msg.LinkChType	= RESTYPE_VOCPCM;
	msg.AddStr( szDTMF );
	PCISENDMSG( &msg );

	return;
}

void CTMS8E240::TMS_CallHold(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( CF_CHTYPE_USER != pChnlInfo->m_nChanelType ) 
	{
		m_strMSG.Format("CallHold error : CallHold are only used by the User channel,  ChIndex = %d ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	CCallHoldObj *pObj = (CCallHoldObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CCallHoldObj) ) )
	{
		m_strMSG.Format("CallHold error : ChIndex = %d   Error = Object is not valid", nIndex );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	
	int nDN = atoi( m_pChannelInfo[nIndex].m_strLocalDN );
	//PickUp Hold
	if( 1 == pObj->m_nCallHold ) 
	{
		TMS_CallHold_PickUp( nIndex, pObj );//判断是否是恢复通话
		pChnlInfo->m_nHoldChnl = -1;
		return;
	}

	//如果是Hold,进行下面操作
	int nIndexLink = pChnlInfo->m_nLinkCh;
	if( 0 > nIndexLink || nIndexLink >= m_nTotalChannel  )
	{
		m_strMSG.Format("%s CallHold error, have not active call; ChIndex = %d ", pObj->m_strCaption, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		m_pPubCObj->PC_CTI_HoldResponseMsg( nDN, 1 );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;	
	}

	pChnlInfo->m_strDTMFRecived = "";
	TMS_MyClearLink( nIndex, nIndexLink) ;
	if( pObj->m_bPlayVox ) //看是否播放音乐
	{
		
		char *pszFileName = (LPTSTR)(LPCTSTR)pObj->m_strPlayFile;
		TMS_PlayFile_Loop( nIndexLink, -1, pszFileName, 1, 1, false );
		m_pChannelInfo[nIndexLink].m_strPlayFile = pObj->m_strPlayFile;
	}
	pChnlInfo->m_nHoldChnl = nIndexLink;
	pChnlInfo->m_nLinkCh = -1;
	m_pChannelInfo[nIndexLink].m_nState = CHANNEL_STATE_HOLD;
	m_pPubCObj->PC_ShowChannelState( nIndexLink, "Hold" );
	m_pPubCObj->PC_CTI_HoldResponseMsg( nDN, 0);
	TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);

	return;
}


void CTMS8E240::TMS_CallHold_PickUp(int nIndex, CCallHoldObj *pObj)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nLinkIndex = TMS_FindHoldChannel(nIndex );

	int nDN = atoi( pChnlInfo->m_strLocalDN );
	if( 0 > nLinkIndex || nLinkIndex >= m_nTotalChannel  )
	{
		m_strMSG.Format("PickUp Hold error,have not PickUp Hold active Call  : ChIndex = %d", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 1,"","" );
		return;
	}
	m_pChannelInfo[nLinkIndex].m_nState = CHANNEL_STATE_CONNECT;
	m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Busy");
	TMS_MySetLink( nIndex, nLinkIndex , CF_CHSWITCH_CONNECT );
	pChnlInfo->m_nLinkCh = nLinkIndex;
	m_pPubCObj->PC_CTI_PickupHoldResponseMsg( nDN , 0,m_pChannelInfo[nLinkIndex].m_strCallerID,m_pChannelInfo[nLinkIndex].m_strCalledID );
	TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );

	return;
}

int CTMS8E240::GetHoldChnl(int nIndex)
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

void CTMS8E240::TMS_ReleaseCall(int nIndex)
{
	if( nIndex == -1 ) return ;
	int nLinkIndex = m_pChannelInfo[nIndex].m_nLinkCh;
	if( -1 < nLinkIndex && nLinkIndex < m_nTotalChannel  )
	{
		if( CHANNEL_STATE_HOLD != m_pChannelInfo[nIndex].m_nState && CHANNEL_STATE_MONITORING !=  m_pChannelInfo[nIndex].m_nState )
		{
			if( CF_CHTYPE_RECORD != m_pChannelInfo[nLinkIndex].m_nChanelType )
			{
				TMS_Release(nLinkIndex);
			}

			TMS_StartPlaySignal( nLinkIndex, TMS_BUSY_SIG );
			m_pChannelInfo[nLinkIndex].MoveCallEndBlock_Node();
			m_pPubCObj->PC_ShowChannelState( nLinkIndex, "Free" );
			
		}

	}

	int nTransferIndex = m_pChannelInfo[nIndex].m_nTransferCh;
	if( -1 < nTransferIndex && nTransferIndex < m_nTotalChannel  )
	{
		if( CF_CHTYPE_RECORD != m_pChannelInfo[nTransferIndex].m_nChanelType )
		{
			TMS_Release(nTransferIndex);
		}

		TMS_StartPlaySignal( nTransferIndex, TMS_BUSY_SIG );
		m_pChannelInfo[nTransferIndex].MoveCallEndBlock_Node();
		m_pPubCObj->PC_ShowChannelState( nTransferIndex, "Free" );
		m_pPubCObj->PC_ShowConnectToChnl( nTransferIndex, -1 );
		

	}

	TMS_ConF_Leave( nIndex, m_pChannelInfo[nIndex].m_nConfNO );
	TMS_Release(nIndex);

	m_pPubCObj->PC_ShowChannelState( nIndex, "Free");
	m_pPubCObj->PC_ShowConnectToChnl( nIndex, -1 );
	TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );

	return;
}

void CTMS8E240::TMS_StartRecord(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CRecordObj *pObj = (CRecordObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CRecordObj) ) )
	{
		m_strMSG.Format("Record Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	if( 0 == pObj->m_nRecordType )
		TMS_StartRecord_Normal( nIndex,  pObj );
	else
		TMS_StartRecord_Confer( nIndex,  pObj );


	return;
}

void CTMS8E240::TMS_AnswerCall(int nIndex)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	pChnlObj->m_nCallType			= CHANNEL_CALLTYPE_COMEIN;
	pChnlObj->m_nRings				= 1;
	pChnlObj->m_nStep				= 1;
	pChnlObj->m_nState				= CHANNEL_STATE_CONNECT;
	pChnlObj->m_nEventType			=  EVENTTYPE_OFFHOOK;
	pChnlObj->m_strtBeginTime		= (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
	

	TMS_Ack( nIndex,TRUE );
	TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
	return;
}

/*************************************************
//  会议操作结果处理 
//  
//  
*************************************************/
void CTMS8E240::TMS_ConferenceResult(int nIndex, unsigned short Function ,unsigned short Param )
{
	CNodeObj * pNodeObj = m_pChannelInfo[nIndex].GetCurrentNodeObj();
	if( NULL == pNodeObj ) return;
	switch( pNodeObj->m_nType )
	{
	case NODETYPE_ENTER_CONFERENCE:
	case NODETYPE_LEAVE_CONFERENCE:
	case NODETYPE_CONFERENCE_BUILD:
	case NODETYPE_CONFERENCE_DELETE:
		{
			if( 0 == Param	)
			{
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
				return;
			}
			else//失败
			{
				m_strMSG.Format("%s-%s-%s ChIndex = %d",  pNodeObj->m_strCaption, GetConfFuncNameStr(Function),GetConfResultStr( (char)Param ), nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
		}
		break;
	default:
		break;
	}

	return;
}

///////////////////////////////////////////////////
//该通道创建，不加入会议
//同时将成功创建的会议编号保存在nConferNO中
//
//////////////////////////////////////////////////
void CTMS8E240::TMS_ConferenceBuild(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CConferenceBulidObj *pObj = (CConferenceBulidObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CConferenceBulidObj) ) )
	{
		m_strMSG.Format("ConferenceBuild error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}

	if( pChnlInfo->m_nStep > 0 )
	{

		long n = m_pPubCObj->PC_ElapseTime( pChnlInfo);
		if( n > 30 )
		{
			//超时或失败
			m_strMSG.Format("%s:ConferenceBuild Timeout Error ChIndex = %d",  pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			
		}
		return;
	}

	int nConferNO = -1;
	int nMaxMember = 0;
	if( 0 == pObj->m_nBuildType )//由系统自动查找空闲会议组
	{
		nConferNO = TMS_FindFreeConferIndex();
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
			if( !m_pPubCObj->PC_GetNorVarValues( pChnlInfo, strConferNO, nKey ) )
			{
				m_strMSG.Format("ConferenceBuild error : ChIndex = %d   Error = have not free conference resource", nIndex );
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
		}
		nConferNO = atoi(strConferNO);
	}

	if(  nConferNO < 0 || nConferNO >= m_nTotalConf )
	{
		m_strMSG.Format("ConferenceBuild error : ChIndex = %d   Error = Input conference  error ", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
		if( !m_pPubCObj->PC_GetNorVarValues( pChnlInfo,strMaxMember, nKey ) )
		{
			m_strMSG.Format("ConferenceBuild error : ChIndex = %d   Error = have not free conference resource", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
	}
	nMaxMember = atoi(strMaxMember);
	TMS_ConF_Create( nIndex, nConferNO,nMaxMember ) ;
	m_pPubCObj->PC_StartTimer( pChnlInfo );
	pChnlInfo->m_nStep = 1;

	return;
}

int CTMS8E240::TMS_FindFreeConferIndex()
{
	int nIndex = -1;
	int m = 0;
	for( int i = 0; i < m_nTotalConf; i++ )
	{
		m = m_pConference[i].nMemberNum + m_pConference[i].nResNum + m_pConference[i].nListenNum;
		if( m <= 0 )
			return i;
	}
	return nIndex;
}


void CTMS8E240::TMS_ConferenceEnter(int nIndex )
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CEnterConferObj *pObj = (CEnterConferObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CEnterConferObj) ) )
	{
		m_strMSG.Format("EnterConference error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}

	if( pChnlInfo->m_nStep > 0 )
	{
		long n = m_pPubCObj->PC_ElapseTime( pChnlInfo);
		if( n > 30 )
		{
			//超时或失败
			m_strMSG.Format("%s:EnterConference Timeout Error ChIndex = %d",  pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		}
		return;
	}
	
	int nConferNO = -1;
	CString strConfNO = pObj->m_strConferNO;
	int nKey = pObj->GetVarKey( strConfNO  );
	if( nKey < 0 )
	{
		nConferNO = atoi(strConfNO );
	}
	else
	{
		if( !m_pPubCObj->PC_GetNorVarValues( pChnlInfo, strConfNO, nKey ) )
		{
			m_strMSG.Format("%s Input Conference index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		nConferNO = atoi( strConfNO );
	}
	
	if(  nConferNO < 0 || nConferNO >= m_nTotalConf )
	{
		m_strMSG.Format("%s: ChIndex = %d    Group error ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
		if(  !m_pPubCObj->PC_GetNorVarValues( pChnlInfo, strChnl, nKey1 ) )
		{
			m_strMSG.Format("%s Input Channel index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		nChnl = atoi( strChnl );
	}

	if( -1 == nChnl )
	{
		nChnl = nIndex;//如果 nChnl = -1表示自己运行该节点的通道加入
	}
	if( nChnl < 0  || nChnl >= m_nTotalChannel  ) 
	{
		m_strMSG.Format("%s Enter Conference  Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	TMS_ConF_Enter( nChnl, nConferNO, pObj->m_nAction );
	pChnlInfo->m_nStep = 1;
	m_pPubCObj->PC_StartTimer( pChnlInfo );

	return;
}

void CTMS8E240::TMS_ConferenceLeave(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CLeaveConferObj *pObj = (CLeaveConferObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CLeaveConferObj) ) )
	{
		m_strMSG.Format(" LeaveConfer  error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}
	
	if( pChnlInfo->m_nStep > 0 )
	{

		long n = m_pPubCObj->PC_ElapseTime( pChnlInfo);
		if( n > 30 )
		{
			//超时或失败
			m_strMSG.Format("%s:LeaveConfer Timeout Error ChIndex = %d",  pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			
		}
		return;
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
		if( !m_pPubCObj->PC_GetNorVarValues( pChnlInfo, strConferNO, nKey ) )
		{
			m_strMSG.Format("ConferenceBuild error : ChIndex = %d   Error = have not free conference resource", nIndex );
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
	}
	int nConferNO = atoi(strConferNO);
	if(  nConferNO < 0 || nConferNO >= m_nTotalConf )
	{
		m_strMSG.Format("%s: ChIndex = %d    Group error ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
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
		if(  !m_pPubCObj->PC_GetNorVarValues( pChnlInfo, strChnl, nKey1 ) )
		{
			m_strMSG.Format("%s Input Channel index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		nChnl = atoi( strChnl );
	}

	if( -1 == nChnl )
	{
		nChnl = nIndex;//如果 nChnl = -1表示自己运行该节点的通道加入
	}
	if( nChnl < 0  || nChnl >= m_nTotalChannel  ) 
	{
		m_strMSG.Format("%s: ChIndex = %d    Channel index error ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;

	}

	TMS_ConF_Leave( nChnl, nConferNO );
	pChnlInfo->m_nStep = 1;
	m_pPubCObj->PC_StartTimer( pChnlInfo );

	return;
}

void CTMS8E240::TMS_ConferenceDelete(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CConferenceDeleteObj *pObj = (CConferenceDeleteObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CConferenceDeleteObj) ) )
	{
		m_strMSG.Format("ConferenceDelete error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;  
	}

	if( pChnlInfo->m_nStep > 0 )
	{

		long n = m_pPubCObj->PC_ElapseTime( pChnlInfo);
		if( n > 30 )
		{
			//超时或失败
			m_strMSG.Format("%s:ConferenceDelete Timeout Error ChIndex = %d",  pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			
		}
		return;
	}

	int nGroup = 0;
	if( 0 == pObj->m_nDeleteType )
	{
		nGroup = pChnlInfo->m_nConfNO;//System assign表示删除该通道主持的会议
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
			if(  !m_pPubCObj->PC_GetNorVarValues( pChnlInfo, strConfNO, nKey ) )
			{
				m_strMSG.Format("%s Input Conference index Error ;ChIndex = %d", pObj->m_strCaption, nIndex);
				m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
				TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
				return;
			}
			nGroup = atoi(strConfNO);
		}
		
	}
	if(  nGroup < 0 || nGroup >= m_nTotalConf )
	{
		m_strMSG.Format("%s: ChIndex = %d    Group error ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}

	TMS_ConF_Close( nIndex,  nGroup );
	m_pPubCObj->PC_StartTimer( pChnlInfo );
	pChnlInfo->m_nStep = 1;

	return;
}

void CTMS8E240::TMS_FaxRecive(int nIndex)
{
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( pChnlInfo->m_nStep > 0 ) return;
	CFaxReciveObj *pObj = (CFaxReciveObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CFaxReciveObj) ) )
	{
		m_strMSG.Format("Recive Fax error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}
	if( 1 != m_nFax )//判断是否有传真卡
	{
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}

	CString strFile("");
	BOOL bResult = m_pPubCObj->PC_GetExpressValues( pChnlInfo,  strFile, pObj->m_strFileName);
	if( !bResult || -1 == pObj->GetVarKey( pObj->m_strPage)  )
	{
		m_strMSG.Format("%s  FaxRecive Error,file=; ChIndex = %d   Error = File name error",pObj->m_strCaption, pObj->m_strFileName, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   	
	}
	strFile = pObj->m_strFolder + strFile;
	TMS_FaxRecive_Msg( nIndex, (LPTSTR)(LPCTSTR)strFile );
	pChnlInfo->m_strReciveFaxFile = strFile;
	pChnlInfo->m_nStep = 1;

	return;
}

void CTMS8E240::TMS_FaxSend(int nIndex)
{
	if( -1 >= nIndex || nIndex >= m_nTotalChannel  ) return;
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	if( pChnlInfo->m_nStep > 0 ) return;
	CFaxSendObj *pObj = (CFaxSendObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CFaxSendObj) ) )
	{
		m_strMSG.Format("Send Fax error : ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}
	if( 1 != m_nFax )//没有传真卡，不提示错误信息
	{
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR);
		return ;  
	}

	CString strFile("");
	BOOL bResult = m_pPubCObj->PC_GetExpressValues( pChnlInfo,  strFile, pObj->m_strFileName);
	if( !bResult  )
	{
		m_strMSG.Format("%s Fax Send Error; ChIndex = %d   Error = File name error",pObj->m_strDescription, nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   	
	}
	strFile = pObj->m_strFolder + strFile;
	TMS_FaxSend_Msg( nIndex, (LPTSTR)(LPCTSTR)strFile );

	pChnlInfo->m_strSendFaxFile = strFile;
	pChnlInfo->m_nStep = 1;
	return;
}



void CTMS8E240::TMS_TransferKey(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int     nRnd = 0;
	srand( (unsigned)time( NULL ) ); 
	nRnd = rand()%6 ; 
	switch(nRnd)
	{
	case 0:
		if( TMS_PlayDTMF(m_lSystem) !=  TMS_PlayDTMF(m_lNormal) ) 
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 1:
		if( TMS_PlayDTMF(m_lNormal) != TMS_PlayDTMF(pChnlInfo->m_lCardKey) )
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 2:
		if( TMS_PlayDTMF(m_lSeting) != TMS_PlayDTMF(pChnlInfo->m_lCardKey)) 
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 3:
		if( TMS_PlayDTMF(m_lCard) !=  TMS_PlayDTMF((m_lNormal)) ) 
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 4:
		if( TMS_PlayDTMF(m_lNormal) != TMS_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 5:
		if( TMS_PlayDTMF(m_lCard) != TMS_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	}

	return;
}

void CTMS8E240::TMS_FaxKey(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int     nRnd = 0;
	srand( (unsigned)time( NULL ) ); 
	nRnd = rand()%6 ; 
	switch(nRnd)
	{
	case 0:
		if( TMS_PlayDTMF(m_lCard) !=  TMS_PlayDTMF(m_lNormal) ) 
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 1:
		if( TMS_PlayDTMF(m_lSeting) != TMS_PlayDTMF(pChnlInfo->m_lCardKey) )
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 2:
		if( TMS_PlayDTMF(m_lSeting) != TMS_PlayDTMF(pChnlInfo->m_lCardKey)) 
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 3:
		if( TMS_PlayDTMF(m_lCard) !=  TMS_PlayDTMF((m_lSeting)) ) 
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 4:
		if( TMS_PlayDTMF(m_lNormal) != TMS_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 5:
		if( TMS_PlayDTMF(m_lSeting) != TMS_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	}

	return;
}
void CTMS8E240::TMS_TCPIP(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int     nRnd = 0;
	srand( (unsigned)time( NULL ) ); 
	nRnd = rand()%6 ; 
	switch(nRnd)
	{
	case 0:
		if( TMS_PlayDTMF(m_lNormal) !=  TMS_PlayDTMF(m_lNormal) ) 
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 1:
		if( TMS_PlayDTMF(m_lSystem) != TMS_PlayDTMF(pChnlInfo->m_lCardKey) )
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 2:
		if( TMS_PlayDTMF(m_lSeting) != TMS_PlayDTMF(pChnlInfo->m_lCardKey)) 
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 3:
		if( TMS_PlayDTMF(m_lCard) !=  TMS_PlayDTMF((m_lNormal)) ) 
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 4:
		if( TMS_PlayDTMF(m_lSeting) != TMS_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	case 5:
		if( TMS_PlayDTMF(m_lCard) != TMS_PlayDTMF(pChnlInfo->m_lCardKey) ) 
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT);
		break;
	}

	return;
}

void CTMS8E240::TMS_WaitCall(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	CNodeObj *pNodeObj = pChnlInfo->GetCurrentNodeObj();
	if( NODETYPE_WAITCALL != pNodeObj->m_nType ) return;
	CWaitCallObj *pObj = (CWaitCallObj*)pNodeObj;
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CWaitCallObj) )  )
	{
		m_strMSG.Format("ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	if( pObj->m_bRecCallerID )
		m_pPubCObj->PC_SaveValuesToVar( pChnlInfo, m_pChannelInfo[nIndex].m_strCallerID, pObj->m_CallerID.m_nKey );
	if( pObj->m_bRecCalledID )
		m_pPubCObj->PC_SaveValuesToVar( pChnlInfo,m_pChannelInfo[nIndex].m_strCalledID, pObj->m_CalledID.m_nKey );
	if( pObj->m_bClearDTMF ) 
		m_pChannelInfo[nIndex].m_strDTMFRecived	= "" ;

	/****************************/
	//TMS_FaxKey( nIndex );
   /*****************************/


	m_pChannelInfo[nIndex].m_nCallType			= CHANNEL_CALLTYPE_COMEIN;
	m_pChannelInfo[nIndex].m_nRings				= 1;
	m_pChannelInfo[nIndex].m_nStep				= 1;
	m_pChannelInfo[nIndex].m_nState				= CHANNEL_STATE_CONNECT;
	m_pChannelInfo[nIndex].m_nEventType			=  EVENTTYPE_OFFHOOK;
	m_pChannelInfo[nIndex].m_strtBeginTime		= (CTime::GetCurrentTime()).Format("%Y-%m-%d %H:%M:%S");
	

	if( pObj->m_bAnswerCall )
	{
		//应答记费
		TMS_Ack( nIndex);
	}
	else
	{
		//应答免费
		TMS_Ack( nIndex,FALSE);
	}
	
	TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );

		
	return;
}
/***************************************
nAcmType类型指示码
0=地址全
1=地址全，计费（系统自动发送时的默认值）
2=地址全，不计费
3=地址全，公用电话
***************************************/
void CTMS8E240::TMS_Acm(RMSG *pmsg, int nAcmType )
{
  pmsg->InitDataLen(0);
  pmsg->MsgType		= MSG_ACM;
  pmsg->Function	= F_ACM_IDLE;//空闲可用
  pmsg->Param	    = nAcmType;//0=地址全
  PCISENDMSG( pmsg );
  return;
}

int CTMS8E240::TMS_FindFreeTrunkChnl(int nBeginCH, int nEndCH)
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

void CTMS8E240::TMS_HangUpAllCall()
{
	int nType=0;
	for( int i = 0; i < m_nTotalChannel; i++ )
	{
		nType=m_pChannelInfo[i].m_nChanelType;
		int nS7Chnl	= m_pChannelInfo[i].m_nChannel;//取得通道号
		if( nType >= 1 &&  nType <= 5 )
		{
			TMS_Release( i );
		}
	}
	return;
}
//普通通道或会议录音，如果nGroup>=0表示会议录音
void CTMS8E240::TMS_RecordFile(int nIndex, int nGroup, char *file, int nFileLen )
{
	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType=MSG_MEDIA;
	msg.Function=F_MEDIA_Rx_File;
	if( nGroup >= 0 )//会议录音
	{
		msg.ChIndex		= nGroup;
		msg.ChType		= RESTYPE_ECONF;
		msg.Param= RESPAR_RAPPEND|RESPAR_RWAITP|RESPAR_RAGC;//等待放音结束后才开始录音
	}
	else//通道录音
	{
		msg.ChIndex		= m_pChannelInfo[nIndex].m_nChannel;
		msg.ChType		= TMS_GetDevChnlType( m_pChannelInfo[nIndex].m_nChanelType );
		msg.Param= RESPAR_RAPPEND|RESPAR_DTMFSTOP|RESPAR_RWAITP|RESPAR_RAGC;//等待放音结束后才开始录音
	}
	msg.LinkChType=RESTYPE_VOCPCM;
	msg.LinkChIndex=0;
	msg.DataSpec=nFileLen;//不限制长度
	msg.AddStr(file); //最多127汉字或254字符  
	PCISENDMSG(&msg);

	return;
}
void CTMS8E240::TMS_RecordFileStop(int nIndex, int nGroup )
{
	RMSG msg;
	msg.InitDataLen(0);
	msg.MsgType=MSG_MEDIA;
	msg.Function=F_MEDIA_Rx_Release;
	if( nGroup >= 0 )//会议录音
	{
		msg.ChIndex		= nGroup;
		msg.ChType		= RESTYPE_ECONF;
	}
	else//通道录音
	{
		msg.ChIndex		= m_pChannelInfo[nIndex].m_nChannel;
		msg.ChType		= TMS_GetDevChnlType( m_pChannelInfo[nIndex].m_nChanelType );
	}
	msg.LinkChType=RESTYPE_VOCPCM;
	PCISENDMSG(&msg);

	return;
}
void CTMS8E240::TMS_StartRecord_Normal(int nIndex, CRecordObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( pChnlObj->m_nStep > 0 ) return;

	CString strFile("");
	BOOL bResult = m_pPubCObj->PC_GetExpressValues(pChnlObj, strFile, pObj->m_strFileName);
	if( !bResult )
	{
		m_strMSG.Format("Record Error; ChIndex = %d   Error = File name error", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   	
	}
	strFile = pObj->m_strFolder + strFile;
	pChnlObj->m_strRecordFile = strFile;
	char *pszFileName = (LPTSTR)(LPCTSTR)strFile;
	TMS_RecordFile( nIndex, -1,pszFileName,60* pObj->m_nFileLength);//发送录音命令
	
	pChnlObj->m_nStep = 1;

	return;
}

void CTMS8E240::TMS_StartRecord_Confer(int nIndex, CRecordObj *pObj)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	if( 0 == m_nConfCard )//无增强型会议
	{
		//只有增强型会议才能使用会议放音
		m_strMSG.Format("%s: ChIndex = %d It is the Play error that has not Enhancement type conference ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	if( pChnlObj->m_nStep > 0 ) return;
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
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
			return;
		}
		nGroupNo = atoi(m_strMSG);
	}

	if(  nGroupNo < 0 || nGroupNo >= m_nTotalConf)
	{
		m_strMSG.Format("%s: ChIndex = %d    Group error ",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	if( m_pConference[nGroupNo].nRecordIndex > 0 )
	{
		m_strMSG.Format("%s: ChIndex = %d It is the error that the group is recording",  pObj->m_strCaption, nIndex);
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return;
	}
	CString strFile("");
	BOOL bResult = m_pPubCObj->PC_GetExpressValues(pChnlObj, strFile, pObj->m_strFileName);
	if( !bResult )
	{
		m_strMSG.Format("Record Error; ChIndex = %d   Error = File name error", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   	
	}
	strFile = pObj->m_strFolder + strFile;
	pChnlObj->m_strRecordFile = strFile;
	char *pszFileName = (LPTSTR)(LPCTSTR)strFile;
	m_pConference[nGroupNo].nRecordIndex = nIndex;//发起该会议放音的通道号
	TMS_RecordFile( nIndex, nGroupNo,pszFileName,60* pObj->m_nFileLength );//发送录音命令
	pChnlObj->m_nStep = 1;
			
	return;
}

void CTMS8E240::TMS_StartRecord_Result(  int nIndex, RMSG *pmsg)
{
	CFChannelInfo *pChnlObj = &(m_pChannelInfo[nIndex]);
	CNodeObj *pNodeObj = pChnlObj->GetCurrentNodeObj();
	if( NODETYPE_START_RECORD != pNodeObj->m_nType ) return;
	CRecordObj *pObj = (CRecordObj*)pNodeObj;
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CRecordObj) ) )
	{
		m_strMSG.Format("Record Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}

	/***********************************/
	//TMS_TransferKey( nIndex );//加密设置
    /***********************************/

	if( F_MEDIA_Rx_Release == pmsg->Function )
	{
		if( RESTOP_FILERR == pmsg->Param )
		{
			m_strMSG.Format("%s Record error, ChIndex = %d    Record error ",  pObj->m_strCaption, nIndex);
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		}
		else
		{
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		}
	}
	else if( F_MEDIA_Error == pmsg->Function )
	{
		if( P_MEDIA_AllocOk != pmsg->Param )
		{
			m_strMSG.Format("%s Record error, ChIndex = %d    Record error %s",  pObj->m_strCaption, nIndex,GetResErrReason(pmsg->Param));
			m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		}
		/*else
		{
			TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		}*/
	}

	return;
}

void CTMS8E240::TMS_Custom(int nIndex)
{
	CFChannelInfo *pChnlInfo = &(m_pChannelInfo[nIndex]);
	int nType = pChnlInfo->m_nChanelType;
	CCustomObj *pObj = (CCustomObj*)pChnlInfo->GetCurrentNodeObj();
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CCustomObj) ) )
	{
		m_strMSG.Format("CustomObj Error; ChIndex = %d   Error = Object is not valid", nIndex );
		m_pPubCObj->PC_ShowSysMsg( nIndex, m_strMSG );
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_ERROR );
		return ;   
	}
	
	if( 0 == pChnlInfo->m_nStep )
	{
		pObj->MoveFirst();
		pChnlInfo->m_nStep = 1;
	}
	if( NULL == pObj->m_pos )
	{
		TMS_MoveNext_Node( nIndex,  NEXTTYPE_NEXT );
		return;
	}
	switch( pObj->m_nFunType )
	{
		case 0://SendFax(  <File> )
			TMS_Cus_SendFax(  nIndex,  pChnlInfo,  pObj );
			break;
		case 1://StartPlayFile(  <File> ; <Loop> )
			TMS_Cus_StartPlayFile(  nIndex,  pChnlInfo,  pObj );
			break;
		case 2://StartRecordFile( <File>  ; <Len> ; <Loop> )
			TMS_Cus_StartRecordFile(  nIndex,  pChnlInfo,  pObj );
			break;
		case 3://GetAgentCall(  <AgentDN> )
			TMS_Cus_GetAgentCall(  nIndex,  pChnlInfo,  pObj );
			break;
		case 4://EnterConfer( <Chnl> ; <conference> )
			TMS_Cus_EnterConfer(  nIndex,  pChnlInfo,  pObj );
			break;
		case 5://LeaveConfer( <Chnl> ; <conference> )
			TMS_Cus_LeaveConfer(  nIndex,  pChnlInfo,  pObj );
			break;
		case 6://ReciveDtmf(  <SaveTo> ; <Len> ; <EndDtmf> )
			TMS_Cus_ReciveDtmf(  nIndex,  pChnlInfo,  pObj );
			break;
		case 7://ReciveFax(   <File> )
			TMS_Cus_ReciveFax(  nIndex,  pChnlInfo,  pObj );
			break;
		case 8://OffHook( <Chnl>  )
			TMS_Cus_OffHook(  nIndex,  pChnlInfo,  pObj );
			break;
		case 9://OutCall( <Tel> )
			TMS_Cus_OutCall(  nIndex,  pChnlInfo,  pObj );
			break;
		case 10://StopPlayFile()
			TMS_Cus_StopPlayFile(  nIndex,  pChnlInfo,  pObj );
			break;
		case 11://StopRecordFile()
			TMS_Cus_StopRecordFile(  nIndex,  pChnlInfo,  pObj );
			break;
		case 12://HangUp( <Chnl> )
			TMS_Cus_HangUp(  nIndex,  pChnlInfo,  pObj );
			break;
		case 13://ClearDtmf( <Chnl> )
			TMS_Cus_ClearDtmf(  nIndex,  pChnlInfo,  pObj );
			break;
		default:
			break;
	}

	return;
}

void CTMS8E240::TMS_Cus_ReciveFax(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}

void CTMS8E240::TMS_Cus_SendFax(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}

void CTMS8E240::TMS_Cus_OutCall(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}
void CTMS8E240::TMS_Cus_OffHook(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}
void CTMS8E240::TMS_Cus_ReciveDtmf(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}
void CTMS8E240::TMS_Cus_LeaveConfer(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}
void CTMS8E240::TMS_Cus_EnterConfer(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}
void CTMS8E240::TMS_Cus_GetAgentCall(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}

void CTMS8E240::TMS_Cus_StartRecordFile(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}
void CTMS8E240::TMS_Cus_StartPlayFile(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj)
{

}

void CTMS8E240::TMS_Cus_StopPlayFile( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj )
{

}

void CTMS8E240::TMS_Cus_StopRecordFile( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj )
{

}

void CTMS8E240::TMS_Cus_HangUp( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj )
{

}
void CTMS8E240::TMS_Cus_ClearDtmf( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj )
{

}