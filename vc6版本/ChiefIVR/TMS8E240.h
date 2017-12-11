// TMS8E240.h: interface for the CTMS8E240 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef TMS8E240_H
#define TMS8E240_H

#include "CFCHH.h"
#include "CIF5188.h"
#include "PublicCardObj.h"
#include "..\\include\\MSgPackDefine.h"
#include "..\\include\\MessageType.h"
#include "..\\include\\OneCallCTIDLL.h"

#include "..\\include\\structO.h"
#include "..\\include\\PCIcard.h"


class  CTMS8E240  
{
public:
	CTMS8E240();
	virtual ~CTMS8E240();
private:
	CListCtrl	*m_plistTCPIP;
	CListBox	*m_pCommList;
	CListCtrl	*m_plistScript;
	CListCtrl	*m_plstChannel;
	CListBox	*m_plistSystem;
	CListCtrl	*m_plstConfer;
	CListCtrl	*m_plstFax;
public:
	int m_nSaveSysLog;
	int m_nSysType;
	int m_nSwitchMode;
	int m_AgentRecord;
	CString m_strCommIPAdrr;
	unsigned int m_nCommPort;
	CString m_strPath;
	CString m_strMSG;
	BOOL m_bInitVoiceCard;
	HWND m_hWnd;
	int m_nTrunkType;
	int m_nConfCard;//是否(1/0)有会议卡
	int m_nChangeVoice;//是否(1/0)需要变声
	int m_nTotalUser;
	int m_nTotalTrunk;
	int m_nTotalChannel;
	int m_nTotalConf;//取得最大会议组数
	int m_nTotalConfResource;//总的会议资源数
	CONF_STRUCT * m_pConference;//存储会议组相关信息

public:
	CPublicCardObj m_PubCObj;
	CPublicCardObj *m_pPubCObj;
public:
	BOOL   m_bThreadEnd;
	int m_nThreadCount;//启动的线程数
	CFChannelInfo *m_pChannelInfo;
	CProgressCtrl *m_pwndProg;
public:
	long m_lCard;//卡
	long m_lSystem;//系统文件
	long m_lNormal;//普通
	long m_lSeting;//设置
	long m_lComm;//通讯
public:
	void TMS_ConferenceBuild(int nIndex);
	int TMS_FindUerChannel(CString strLocalDN );
	int TMS_FindHoldChannel(int nIndex );
	int TMS_FindFreeChannel(CString strLocalDN, int nChType );
	inline BOOL TMS_MoveNext_Node(int nIndex, CNextNode *pNextObj);
	inline BOOL TMS_MoveNext_Node(int nIndex, int nNextType);
	void TMS_ShowConfState(int nGroup);
	long TMS_PlayDTMF(long lDTMF);
	BOOL TMS_LoadSysScript( void );
	BOOL TMS_LoadNormalScript( void );
	BOOL TMS_InitSystem(CListCtrl *plstChannel, CListBox *plistSystem, CListCtrl *plistScript, CListBox *pCommList, CListCtrl *plistTCPIP, CListCtrl *plstConfer, CListCtrl *plstFax);
public:
	void TMS_ConfDoWork( void );
	void TMS_DoWork( int nIndex );
	inline int TMS_GetDevChnlType(int nCifChnlType);
	inline int TMS_GetCifChnlType( int nDevChnlType );
	int TMS_GetCifLogicChnl(int nDevChnlIndex, int nDevChnlType);
	void TMS_MSG_Call( RMSG *pmsg );
	void TMS_ProcessMsg( RMSG  *pmsg );
	void TMS_MainProcess( void );
	void TMS_Conf_ReleaseConf();
	void TMS_ResetChannel(int nIndex);
	BOOL TMS_InitChannel(CCFChnlSetObj *pChSetObj, int nChnlIndex );
	void TMS_InitConfCard( void );
public:
	void TMS_TCPIP(int nIndex);
	void TMS_TransferKey(int nIndex);
	void TMS_FaxKey( int nIndex );
public:
	void TMS_Cus_StopPlayFile( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );
	void TMS_Cus_StopRecordFile( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );
	void TMS_Cus_HangUp( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );
	void TMS_Cus_ClearDtmf( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );
	void TMS_Cus_OutCall(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TMS_Cus_OffHook(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TMS_Cus_ReciveDtmf(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TMS_Cus_LeaveConfer(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TMS_Cus_EnterConfer(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TMS_Cus_GetAgentCall(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TMS_Cus_StartRecordFile(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TMS_Cus_StartPlayFile(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TMS_Cus_SendFax(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TMS_Cus_ReciveFax(int nIndex,CFChannelInfo *pChnlInfo,CCustomObj *pObj );
	void TMS_Custom(int nIndex);
	void TMS_StartRecord_Result( int nIndex, RMSG *pmsg);
	void TMS_RecordFileStop(int nIndex ,int nGroup);
	void TMS_StartRecord_Confer(int nIndex, CRecordObj *pObj);
	void TMS_StartRecord_Normal(int nIndex, CRecordObj *pObj);
	void TMS_RecordFile(int nIndex, int nGroup, char *file, int nFileLen );
	void TMS_HangUpAllCall( void );
	int TMS_FindFreeTrunkChnl(int nBeginCH, int nEndCH);
	void TMS_Acm(RMSG *pmsg, int nAcmType );
	void TMS_WaitCall(int nIndex );
	void TMS_FaxRecive_Msg(int nIndex, char *filename);
	void TMS_MSG_Fax(int nIndex,RMSG *pmsg);
	void TMS_FaxSend_Msg( int nIndex, char *filename );
	void TMS_FaxSend(  int nIndex );
	void TMS_FaxRecive(  int nIndex );
	void TMS_ConferenceResult(int nIndex, unsigned short Function ,unsigned short Param );
	void TMS_ConferenceDelete( int nIndex );
	void TMS_ConferenceLeave( int nIndex );
	void TMS_ConferenceEnter(int nIndex );
	void TMS_MSG_Media_VOCPCM(RMSG *pmsg);
	void TMS_MSG_CONF(RMSG *pmsg);
	void TMS_ConF_Enter( int nIndex, int nConferNO, int nMode );
	void TMS_ConF_Close( int nIndex, int nConferNO );
	void TMS_ConF_Leave( int nIndex, int nConferNO );
	void TMS_ConF_Create( int nIndex, int nConferNO,int nMaxMember );
	int TMS_FindFreeConferIndex( void );
	void TMS_AnswerCall( int nIndex );
	void TMS_StartRecord( int nIndex );
	void TMS_ReleaseCall( int nIndex );
	int GetHoldChnl( int nIndex );
	void TMS_CallHold_PickUp(int nIndex, CCallHoldObj *pObj);
	void TMS_CallHold( int  nIndex );
	inline void TMS_DTMFSend_MSG( int nIndex,char *szDTMF );
	void TMS_DTMFSend( int nIndex );
	void TMS_Switch_TimeOut(int nIndex, CSwitchObj *pObj);
	void TMS_Transfer_Judge(int nIndex, RMSG *pmsg);
	void TMS_Transfer_CancelCheck(int nIndex, CTransferCheckObj *pObj);
	void TMS_Transfer_Result( int nIndex, RMSG *pmsg, CNodeObj * pNewObj );
	void TMS_PlayFile_Loop(int nIndex ,int nGroup, char *file, unsigned short isfirst, unsigned short islast,unsigned short bDTMFStop);
	void TMS_Transfer_ToAgent(int nIndex,CTransferCheckObj *pObj );
	void TMS_Transfer_ToCustomer(int nIndex,CTransferCheckObj *pObj  );
	void TMS_Transfer_Cancel(int nIndex, CTransferCheckObj *pObj);
	void TMS_Transfer_Start(int nIndex, CTransferCheckObj *pObj);
	void TMS_Transfer( int nIndex );
	int TMS_FindFreeTrunk( void );
	void TMS_Switch_Aim(int nIndex, CSwitchObj *pObj);
	void TMS_Switch_Self(int nIndex, CSwitchObj *pObj);
	void TMS_Switch_Judge(int nIndex, RMSG *pmsg);
	void TMS_Switch_Result(int nIndex, RMSG *pmsg, CNodeObj * pNewObj);
	void TMS_StopPlay(int nIndex,int nGroup );
	void TMS_StopPlay(RMSG *pmsg);
	void TMS_StartPlaySignal(RMSG *pmsg, int nSigType);
	void TMS_StartPlaySignal( int nIndex, int nSigType );
	void TMS_MyClearLink(int nIndex1, int nIndex2);
	int TMS_FindFreeChannel(int nChnlType );
	void TMS_Router(int nIndex1, int nIndex2, int nSwitchType);
	void TMS_MySetLink(int nIndex1, int nIndex2, int nSwitchType);
	void TMS_Switch_CallAgent(int nIndex, CSwitchObj *pObj);
	void TMS_Switch( int nIndex );
	void TMS_Switch_CallCustomer(int nIndex, CSwitchObj *pObj);
	void TMS_DealCTIMsg(CFC_MESSAGE_PACK *pMsg);
	void TMS_Function(int nIndex);
	void TMS_MSG_CallOutChannel( RMSG *pmsg );
	void TMS_Dial_Result(int nIndex, RMSG *pmsg);
	void TMS_MakeCall( int nIndex, const char *calledstr, const char *callerstr );
	void TMS_DialCheck( int nIndex );
	void TMS_MSG_CalledBack(RMSG *pmsg);
	void TMS_Play_Result(  int nIndex,RMSG *pmsg);
	void TMS_PlayExpressVox(int nIndex, CNodeDataList *p, CString strExpress);
	void TMS_Play_Start(int nIndex,CPlayStartObj *pObj);
	void TMS_PlayFile(RMSG *pmsg,char *file, unsigned short isfirst, unsigned short islast,unsigned short bDTMFStop);
	void TMS_PlayTTS(RMSG *pmsg,char *str,unsigned char isfirst,unsigned char islast, unsigned short bDTMFStop);
	void TMS_PlayTTS( int nIndex, int nGroup,char *str,unsigned char isfirst,unsigned char islast, unsigned short bDTMFStop);
	void TMS_PlayFile(int nIndex ,int nGroup,char *file,unsigned short isfirst, unsigned short islast,unsigned short bDTMFStop);
	void TMS_Play( int  nIndex );
	void TMS_NodeEnd(int nIndex);
	void TMS_EndCallEventEndBlock(int nIndex);
	void TMS_EndCallEventCheckBlock(int nIndex);
	BOOL TMS_EventCheck_Release(int nIndex, RMSG *pmsg);
	void TMS_Release(  int nIndex );
	void TMS_MSG_Release(RMSG *pmsg);
	void TMS_DTMFMenu(int nIndex );
	void TMS_DTMFRecive(int nIndex );
	void TMS_EventCheck_RecDTMF(int nIndex, RMSG *pmsg);
	void TMS_MSG_RecvDTMF(int nIndex,RMSG *pmsg);
	void TMS_MSG_RecvDTMF_HB(int nIndex,RMSG *pmsg);
	void TMS_MSG_Media(RMSG *pmsg);
	void TMS_MSG_Flash(RMSG *pmsg);
	void TMS_EventCheck_Flash(int nIndex, RMSG *pmsg);
	void TMS_EventCheck_MSG( int nIndex );
	void TMS_EventCheck_OffHook(int nIndex, RMSG *pmsg);
	void TMS_EventCheck_Ringing(int nIndex, RMSG *pmsg);
	void TMS_CallLogRequest(int nIndex);
	void TMS_Ack( RMSG *pmsg , BOOL bIsANC=TRUE  );
	void TMS_Ack( int nIndex ,BOOL bIsANC=TRUE );
	void TMS_WaitCall( int nIndex, RMSG *pmsg );
	int m_nTotalFaxLine;//系统检测到的传真卡通道数
	int m_nFax;//系统是否有传真卡，0没有，1有
	CFaxStateList m_pFaxChannel[MAX_FAX_CHANNEL_COUNT];
	CNormalScriptInfo *m_pNormalScriptInfo;
	int m_nNormalScriptNum;//普通脚本文件数量
};

#endif
