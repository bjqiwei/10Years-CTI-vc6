// NorMarch32.h: interface for the CNorMarch32 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NORMARCH32_H__BB50894A_D3C8_4AF7_9DE6_B4DA71B429F2__INCLUDED_)
#define AFX_NORMARCH32_H__BB50894A_D3C8_4AF7_9DE6_B4DA71B429F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\include\\CFCHH.h"
#include "CIF5188.h"
#include "PublicCardObj.h"
#include "..\\include\\MSgPackDefine.h"
#include "..\\include\\MessageType.h"
#include "..\\include\\OneCallCTIDLL.h"

#include "..\include\CtiDef.h"
#include "..\include\PhonicDll.h"


void CALLBACK Nor32_EventCallBack(EPhonicEvent eventType,int channelType,int channelID, int lParam1,int lParam2);

class CNorMarch32  
{
public:
	CNorMarch32();
	virtual ~CNorMarch32();
public:
	CFChannelInfo *m_pChannelInfo;
	int m_nTotalChannel;
	BOOL   m_bThreadEnd;
	int m_nThreadCount;//启动的线程数
	BOOL m_bInitVoiceCard;//初始化语音卡是否成功
	HWND m_hWnd;

public:
	long m_lCard;//卡
	long m_lSystem;//系统文件
	long m_lNormal;//普通
	long m_lSeting;//设置
	long m_lComm;//通讯
public:
	CNormalScriptInfo *m_pNormalScriptInfo;
	int m_nNormalScriptNum;//普通脚本文件数量
public:
	int m_nTotalFaxLine;//系统检测到的传真卡通道数
	int m_nFax;//系统是否有传真卡，0没有，1有
	CFaxStateList m_pFaxChannel[MAX_FAX_CHANNEL_COUNT];
	CPublicCardObj m_PubCObj;
	CPublicCardObj *m_pPubCObj;
	CString m_strPath;
public:
	CProgressCtrl *m_pwndProg;
public:
	int m_nSysType;
	CString m_strCommIPAdrr;
	unsigned int m_nCommPort;
	int m_nSwitchMode;//0普通模式 1会议模式
	int m_AgentRecord;
public:
	void Nor32_Cus_ResponseNotReady(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_ResponseReady(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_ResponseLogOut(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_FaxKey(int nIndex);
	void Nor32_TCPIP(int nIndex);
	void Nor32_Cus_JoinConfer(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_Monitor(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void Nor32_Cus_DisMonitor(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void Nor32_Cus_TansferToIVR(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_PickupHold(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_Hold(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_AnswerCall( int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void Nor32_Cus_GetAgentActiveCall(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);

	void Nor32_Cus_ResponseTransfer(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_ResponsePickUpHold(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_ResponseMonitor(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_ResponseLogin(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_ResponseJoinConfer(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_ResponseHold(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_ResponseGetActive(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_ResponseDisMonitor(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_ResponseConferCall(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_ResponseCancelTransfer(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_ResponseAnswerCall(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);
	void Nor32_Cus_ResponesMakeCall(int nIndex, CFChannelInfo *pChnlObj, CCustomObj *pObj);

	int Nor32_FindActiveCall(int nIndex, int nType , CString strTel );
	int Nor32_FindHoldChannel( int nIndex, CString strCallerID );
	int Nor32_FindUerChannel(CString strLocalDN);
	int Nor32_FindHoldChannel( int nIndex );
	void Nor32_DealCTIMsg(CFC_MESSAGE_PACK *pMsg);
	BOOL Nor32_MyListenConference(int nIndex, int nGroup);
	BOOL Nor32_MyStopConfRecord(int nGroup  );
	BOOL Nor32_MyStopConfPlay(int nGroup );
	void Nor32_Cus_StartPlayEnd(int nIndex );
	void Nor32_Cus_OutCallResult(int nIndex, BOOL bResult );
	void Nor32_MyInitDtmfBuf( int nIndex );
	BOOL Nor32_Cus_GetAgentCall(int nIndex, CFChannelInfo *pChnlInfo, CString strDN);
	void Nor32_Dila_Result(int nIndex, BOOL bResult, CNodeObj *pNewObj);
	void Nor32_Dial_Judge(int nIndex, BOOL bResult);
	void Nor32_DialCheck(int nIndex );
	void Nor32_ConferenceLeave( int nIndex );
	void Nor32_Transfer_Result(int nIndex, BOOL bResult, CNodeObj * pNewObj );
	void Nor32_Transfer_Judge( int nIndex , BOOL bResult);
	void Nor32_Transfer_ToAgent(int nIndex,CTransferCheckObj *pObj );
	void Nor32_Transfer_ToCustomer( int nIndex ,CTransferCheckObj *pObj );
	void Nor32_Transfer_Start(int nIndex,CTransferCheckObj *pObj  );
	void Nor32_Transfer_Cancel(int nIndex,CTransferCheckObj *pObj  );
	void Nor32_Transfer( int  nIndex );
	int Nor32_GetIndexByGroup( int nGroup, int nEventType  );
	BOOL Nor32_MyReleaseConference( int nGroup );
	BOOL Nor32_MyExitConference( int nIndex );
	int Nor32_MyJoinConference( int nIndex, int nGroup );
	int Nor32_MyGetConferenceNo( int nIndex );
	void Nor32_OnEventState( int nIndex, int nState );
	int Nor32_MyAddPromptFromFile(LPCTSTR fileName);
	void Nor32_Cus_StopPlayFile( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );
	void Nor32_Cus_StopRecordFile( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );
	void Nor32_Cus_HangUp( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );
	void Nor32_Cus_ClearDtmf( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );
	void Nor32_Cus_OutCall(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void Nor32_Cus_OffHook(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void Nor32_Cus_ReciveDtmf(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void Nor32_Cus_LeaveConfer(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void Nor32_Cus_EnterConfer(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void Nor32_Cus_GetAgentCall(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void Nor32_Cus_StartRecordFile(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void Nor32_Cus_StartPlayFile(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void Nor32_Cus_SendFax(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void Nor32_Cus_ReciveFax(int nIndex,CFChannelInfo *pChnlInfo,CCustomObj *pObj );
	void Nor32_Custom(int nIndex);
	void Nor32_Switch_Result(int nIndex, BOOL bResult, CNodeObj * pNewObj );
	void Nor32_Switch_Judge( int nIndex, BOOL bResult  );
	void Nor32_Switch_TimeOut(int nIndex, CSwitchObj *pObj);
	int Nor32_FindFreeTrunk( void );
	int Nor32_FindFreeTrunkChnl(int nBeginCH, int nEndCH);
	void Nor32_MyPlaySignal(int nIndex, int nSigType);
	int Nor32_FindFreeChannel(CString strLocalDN, int nChType );
	int Nor32_FindFreeChannel( int nChnlType );
	BOOL Nor32_MyMakeCall(int nIndex, int nInbound,int nCallType,CString strCallPK, char *szCalledID, char *szCallerID);
	BOOL Nor32_MySetLink(int nIndex1, int nIndex2, int nSwitchType);
	void Nor32_Switch_CallCustomer(int nIndex, CSwitchObj *pObj);
	void Nor32_Switch_CallFreeAgent(int nIndex, CSwitchObj *pObj);
	void Nor32_Switch_CallAgent(int nIndex, CSwitchObj *pObj);
	void Nor32_Switch_Self(int nIndex, CSwitchObj *pObj);
	void Nor32_Switch_Aim(int nIndex, CSwitchObj *pObj);
	void Nor32_Switch(int nIndex );
	void Nor32_OnOneFaxPageOver(int nIndex, int nPage );
	void Nor32_OnRecordEnd( int nIndex );
	BOOL Nor32_MyStartRecordFile(  int nIndex, CString strFile, BOOL bRecordLoop=FALSE,int nRecordType=0,long lRecordLen=3600, int nGroup = -1 );
	void Nor32_StartRecord( int nIndex );
	void Nor32_FaxSend( int nIndex );
	void Nor32_MyFaxStop(int nIndex );
	void Nor32_OnFaxError( int nIndex, long cause );
	BOOL Nor32_MyFaxReceive(int nIndex, const char *fileName);
	void Nor32_FaxRecive( int nIndex );
	BOOL Nor32_MyFaxSend( int nIndex, const char * fileName );
	void Nor32_ReleaseCall( int nIndex );
	void Nor32_WaitCall( int nIndex );
	void Nor32_MyOFFHook( int nIndex , BOOL bJiFei );
	void Nor32_AnswerCall( int nIndex );
	BOOL Nor32_MyClearLink(int nIndex1, int nIndex2 );
	void Nor32_CallHold_PickUp(int nIndex, CCallHoldObj *pObj);
	void Nor32_CallHold(int nIndex );
	BOOL Nor32_MyConferEnter(int nIndex, int nConferIndex, int nEnterType);
	void Nor32_ConferenceEnter( int nIndex  );
	void Nor32_DtmfSendEndCheck( int nIndex, CDTMFSendObj*pObj  );
	BOOL Nor32_MySendDtmfStr( int nIndex, const char * dtmfString );
	void Nor32_DTMFSend(int nIndex);
	void Nor32_EndCallEventEndBlock(int nIndex);
	void Nor32_EndCallEventCheckBlock(int nIndex);
	void Nor32_DtmfRecive(int nIndex );
	void Nor32_DTMFMenu( int nIndex  );
	void Nor32_PlayEndCheck( int nIndex, CPlayStartObj *pObj ,CFChannelInfo *pChnlInfo );
	void Nor32_OnPlayEnd( int nIndex );
	BOOL Nor32_PlayExpressVox( int nIndex, CString strExpress );
	BOOL Nor32_MyStartPlayFile(  int nIndex, CString strFile, int nConNo = -1, BOOL bPlayLoop=FALSE,int nPlayType = 0/*放音类型：0普通 1会议*/ );
	
	void Nor32_MyStopPlay( int nIndex );
	void Nor32_MyStopRecord(int nIndex );
	void Nor32_PlayStart( int nIndex );
	void Nor32_NodeEnd( int nIndex );
	void Nor32_MyHangUp(int nIndex );
	void Nor32_ResetChannel(int nIndex);
	void Nor32_OnUserOffHook( int nIndex );
	void Nor32_OnFlash( int nIndex );
	void Nor32_OnReciveDtmf(int nIndex, char dtmf );
	BOOL Nor32_EventCheck_EventMsg( int nIndex, CEventCheckObj *pObj );
	void Nor32_EventCheck(int nIndex );
	void Nor32_OnRing( int nIndex );
	void Nor32_OnTrunkCallIn( int nIndex, char * pszCallerID, char * pszCalledID);
	BOOL Nor32_LoadNormalScript(void);
	int Nor32_GetCifLogicChnl(int nDevChnlIndex, int nDevChnlType);
	inline int Nor32_GetDevChnlType(int nCifChnlType);
	inline int Nor32_GetCifChnlType(int nDevChnlType);
	BOOL Nor32_InitSystem(CListCtrl *plstChannel, CListBox *plistSystem, CListCtrl *plistScript, CListBox *pCommList, CListCtrl *plistTCPIP, CListCtrl *plstConfer, CListCtrl *plstFax);
	void Nor32_OnEventNotify( EPhonicEvent	eventType,int channelType,int channelID, int lParam1,int lParam2);
	BOOL Nor32_LoadSysScript(void);

	BOOL Nor32_MoveNext_Node(int nIndex, CNextNode *pNextObj);
	BOOL Nor32_MoveNext_Node(int nChnl, int nNextType);
	CNodeObj * Nor32_MoveNextObj(int nNextType, CNodeObj *pCurObj, CCFDocument *pDoc );
	BOOL Nor32_InitChannel(CCFChnlSetObj *pChSetObj, int nChnlIndex );
	void Nor32_DoWork( int nIndex );
	long Nor32_PlayDTMF(long lDTMF);
};

#endif // !defined(AFX_NORMARCH32_H__BB50894A_D3C8_4AF7_9DE6_B4DA71B429F2__INCLUDED_)
