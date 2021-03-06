// DJSS17.h: interface for the CDJSS17 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef DJSS17_H
#define DJSS17_H

/////////////////////////////////////////////////////////////
//CDJSS17用于处理东进7号信令数字卡的类，在该种情况下，同时可
//以处理DJDCB2560会议卡，传真卡，变声
//
//
//
//////////////////////////////////////////////////////////////

#include "CFCHH.h"
#include "CIF5188.h"
#include "PublicCardObj.h"
#include "..\\include\\MSgPackDefine.h"
#include "..\\include\\MessageType.h"
#include "..\\include\\OneCallCTIDLL.h"


#include "..\\include\\tce1_32.h"
#include "..\\include\\ISUPDll.h"
#include "..\\include\\No7Dll.h"
#include "..\\include\\ISDNDll_1.h"
#include "..\\include\\djconf.h"
#include "..\\include\\djConfNew.h"
#include "..\\include\\faxapi32.h"
#include "..\\include\\Tc08a32.h"
#include "..\\include\\NewSig.h"
#include "..\\include\\NewConf.h"
#include "..\\include\\conf95.h"



class  CDJSS17  
{
public:
	CDJSS17();
	virtual ~CDJSS17();
public:

	int m_nVocBusy;//忙音
	int m_nVocRingBack;//回铃音
	int m_nVocTone;//拨号音
	CProgressCtrl *m_pwndProg;
private:
	
	CListCtrl	*m_plistTCPIP;
	CListBox	*m_pCommList;
	CListCtrl	*m_plistScript;
	CListCtrl	*m_plstChannel;
	CListBox	*m_plistSystem;
	CListCtrl	*m_plstConfer;
	CListCtrl	*m_plstFax;
	
private:	
	int m_nIs_SS1;
	int m_nIs_DSS1;
	int m_nIs_SS7;

public:
	int m_nSaveSysLog;//是否保存系统日志
	int m_nSysType;//系统类型IVR或CTI
	int m_nSwitchMode;
	int m_AgentRecord;
	CString m_strCommIPAdrr;
	unsigned int m_nCommPort;
	CString m_strPath;
	CString m_strMSG;
	BOOL m_bInitVoiceCard;//是否有变声
	HWND m_hWnd;
	int m_nTrunkType;
	int m_nConfCard;//是否(1/0)有会议卡
	int m_nChangeVoice;//是否(1/0)需要变声
	int m_nSS7_type; //SS7类型
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
public:
	long m_lCard;//卡
	long m_lSystem;//系统文件
	long m_lNormal;//普通
	long m_lSeting;//设置
	long m_lComm;//通讯
public:
	int m_nTotalFaxLine;//系统检测到的传真卡通道数
	int m_nFax;//系统是否有传真卡，0没有，1有
	CFaxStateList m_pFaxChannel[MAX_FAX_CHANNEL_COUNT];

	CNormalScriptInfo *m_pNormalScriptInfo;
	int m_nNormalScriptNum;//普通脚本文件数量
public:
	BOOL DJSS_LoadSysScript();
public:
	void DJSS_DoWork(int nIndex);
	BOOL DJSS_InitChannel(CCFChnlSetObj *pChSetObj, int nChnlIndex );
	CNodeObj * DJSS_MoveNextObj(int nNextType, CNodeObj *pCurObj, CCFDocument *pDoc );
	BOOL DJSS_MoveNext_Node(int nChnl, int nNextType);
	long DJSS_PlayDTMF(long lDTMF);
public:
	int GetHoldChnl(int nIndex);
	int DJSS_FindHoldChannel(int nIndex );
	int DJSS_FindFreeChannel(int nChnlType );
	void DJSS_WaitCall_TUP( int nIndex );
public:
	void DJSS_TransferKey(int nIndex);
	void DJSS_TCPIP(int nIndex);
	void DJSS_FaxKey(int nIndex);
public:
	void DJSS_Cus_StopPlayFile( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );
	void DJSS_Cus_StopRecordFile( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );
	void DJSS_Cus_HangUp( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );
	void DJSS_Cus_ClearDtmf( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );
	void DJSS_Cus_OutCall(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void DJSS_Cus_OffHook(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void DJSS_Cus_ReciveDtmf(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void DJSS_Cus_LeaveConfer(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void DJSS_Cus_EnterConfer(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void DJSS_Cus_GetAgentCall(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void DJSS_Cus_StartRecordFile(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void DJSS_Cus_StartPlayFile(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void DJSS_Cus_SendFax(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void DJSS_Cus_ReciveFax(int nIndex,CFChannelInfo *pChnlInfo,CCustomObj *pObj );
	void DJSS_Custom(int nIndex);
	BOOL DJSS_SendCallerID_FSK(int nIndex,  char *CallIDNumStr );
	BOOL DJSS_SendCallerID_DTMF(int nIndex,  char *CallIDNumStr );
	void DJSS_WaitCall_Record_DTMF(int nIndex);
	BOOL DJSS_MyCheckSendEnd( int nIndex );
	BOOL DJSS_MySendDtmfStr( int nIndex,  char * pcDtmf );
	void DJSS_HangUpAllCall( void );
	int DJSS_MyMakeCall_Check(int nIndex , int nTimeOut );
	int DJSS_FindFreeTrunkChnl( int nBeginCH, int nEndCH );
	void DJSS_HangUpTrunk(int nIndex);
	void DJSS_Switch_Aim(int nIndex, CSwitchObj *pObj);
	void DJSS_Switch_Self(int nIndex, CSwitchObj *pObj);
	void DJSS_DealCTIMsg( CFC_MESSAGE_PACK  *pMsg );
	int DJSS_MyMakeCall_Check( int nIndex );
	int  DJSS_GetRecordVoiceID(WORD wGroupNo);
	void DJSS_StopConfRecord(WORD wGroupNo);
	void DJSS_StartRecord_Confer(int nIndex, CRecordObj *pObj );
	void DJSS_StartRecord_Normal(int nIndex, CRecordObj *pObj );
	BOOL DJSS_CheckRecordFileEnd(int nIndex,int nGroupNo );
	BOOL DJSS_MyRecordFile(int nIndex, char *pszfilename, DWORD dRecordLen,int nGroupNo);
	void DJSS_StartRecord( int nIndex );
	int  DJSS_GetPlayVoiceID(WORD wGroupNo);
	void DJSS_StopConfPlay(WORD wGroupNo);
	void DJSS_PlayStart_Confer(int nIndex,  CPlayStartObj* pObj);
	void DJSS_AnswerCall( int nIndex );
	void DJSS_ShowConfState( int nGroup );
	void DJSS_ConfDoWork( void );
	void DJSS_GetConfMember( int nIndex );
	void DJSS_DTMFSend( int nIndex );
	void DJSS_Transfer_Cancel(int nIndex,CTransferCheckObj *pObj  );
	void DJSS_Transfer_Start(int nIndex,CTransferCheckObj *pObj  );
	int DJSS_FindFreeTrunkChnl( void );
	void DJSS_Transfer_ToCustomer( int nIndex ,CTransferCheckObj *pObj );
	void DJSS_Transfer_ToAgent(int nIndex,CTransferCheckObj *pObj );
	void DJSS_EventCheck_DSS1(int nIndex, CEventCheckObj *pObj);
	void DJSS_EventCheck_SS1(int nIndex, CEventCheckObj *pObj);
	void DJSS_WaitCall_SS1(int nIndex);
	void DJSS_WaitCall_DSS1(int nIndex);
	void DJSS_HangUp_SS7(int nIndex);
	void DJSS_HangUp_DSS1(int nIndex);
	void DJSS_HangUp_SS1(int nIndex);
	void DJSS_MyHangUpDetect_Record(int nIndex);
	void DJSS_MyHangUpDetect_User(int nIndex);
	void DJSS_MyHangUpDetect_SS7(int nIndex);
	void DJSS_MyHangUpDetect_SS1(int nIndex);
	void DJSS_MyHangUpDetect_DSS1(int nIndex);
	void DJSS_ExitSystem( void );
	BOOL DJSS_InitSystem(CListCtrl *plstChannel, CListBox *plistSystem, CListCtrl *plistScript, CListBox *pCommList, CListCtrl *plistTCPIP, CListCtrl *plstConfer, CListCtrl *plstFax);
	void DJSS_WaitCall_ISUP(int nIndex);
	void DJSS_WaitCall(int nIndex);
	int DJSS_GetDevConfChnlType(int nCifChnlType);
	void DJSS_EndCallEventEndBlock(int nIndex);
	void DJSS_EndCallEventCheckBlock( int nIndex );
	void DJSS_ConferenceLeave(int nIndex);
	int DJSS_ConnectFromTS( int nIndex ,int nTimeIndex );
	int DJSS_GetTimeSlot( int nIndex );
	void DJSS_ChangeVoice(int nIndex);
	BOOL DJSS_LoadNormalScript( void );
	void DJSS_ConferenceDelete( int nIndex );
	void DJSS_DJConf_DeleteGroup( int nIndex, int nGroup );
	void DJSS_DJConf_LeaveConf( int nIndex, int nGroup );
	BOOL DJSS_DJConf_JoinConfGroup( int nIndex, int nGroup , int  nMode );
	void DJSS_ConferenceEnter( int nIndex );
	int DJSS_FindFreeConferIndex( void );
	void DJSS_ConferenceBuild( int nIndex );
	BOOL DJSS_CreateAndEnterConfGroup(int nIndex, int nGroupID,int nMaxMember );
	void DJSS_DJConf_InitConfCard( void );
	void DJSS_DJConf_ReleaseConf( void );
	void DJSS_Transfer( int nIndex );
	int DJSS_FindUerChannel(CString strLocalDN);
	int DJSS_FindChannel(CString strLocalDN);
	void DJSS_Switch( int nIndex  );
	BOOL DJSS_MyMakeCall( int nIndex , char *szCalledID, char *szCallerID );
	void DJSS_Switch_CallCustomer(int nIndex, CSwitchObj *pObj);
	void DJSS_StartPlaySignal( int nIndex, int nSignalType );
	BOOL DJSS_CheckPlayFileEnd( int nIndex  );
	int DJSS_FindFreeChannel(CString strLocalDN, int nChType );
	void DJSS_Switch_CallAgent(int nIndex, CSwitchObj *pObj);
	void DJSS_Switch_CallFreeAgent(int nIndex, CSwitchObj *pObj);
	void DJSS_PushPlay( void );
	int DJSS_GetAsciiToCode( char code);
	void DJSS_DTMFMenu( int nIndex );
	void DJSS_DTMFRecive( int nIndex );
	char DJSS_GetCodeAscii(int code );
	void DJSS_StopRecord(int nIndex);
	void DJSS_StopPlay( int nIndex );
	BOOL DJSS_MySetLink_Trunk(int nIndex1, int nIndex2, int nSwitchType);
	BOOL DJSS_MySetLink_User(int nIndex1, int nIndex2, int nSwitchType);
	BOOL DJSS_MyClearLink(int nIndex1, int nIndex2);
	BOOL DJSS_MySetLink(int nIndex1, int nIndex2 , int nSwitchType);
	void DJSS_CallHold_PickUp(int nIndex, CCallHoldObj *pObj);
	void DJSS_CallHold( int nIndex );
	void DJSS_ReleaseCall( int nIndex );
	void DJSS_Dial( int nIndex );
	void DJSS_FaxSend(int nIndex);
	void DJSS_FaxRecive( int nIndex );
	void DJSS_CallLogRequest( int nIndex );
	BOOL DJSS_MoveNext_Node(int nIndex, CNextNode *pNextObj);
	BOOL DJSS_PlayExpressVox(int nIndex, CString strExpress, int nGroupNo  );
	void DJSS_PlayStartTrunk(int nIndex, CPlayStartObj* pObj);
	void DJSS_PlayStartUser(int nIndex, CPlayStartObj* pObj );
	void DJSS_PlayFileStop( int nIndex );
	BOOL DJSS_PlayFile( int nIndex , char * pszfilename);
	int DJSS_GetDtmfCode(int nIndex ,CString &strDtmf);
	void DJSS_InitDtmfBuf( int nIndex );
	void DJSS_PlayStart( int nIndex );
	void DJSS_NodeEnd(int nIndex);
	void DJSS_EventCheck_User(int nIndex, CEventCheckObj *pObj);
	void DJSS_ResetTrunk( int nIndex );
	void DJSS_EventCheck_SS7(int nIndex, CEventCheckObj *pObj);
	BOOL DJSS_EventCheck_EventMsg(int nIndex, CEventCheckObj *pObj);
	void DJSS_EventCheck( int nIndex );
	void DJSS_ReleaseUser(int nIndex);
	void DJSS_ReleaseTrunk( int nIndex );
	void DJSS_ResetUser( int nIndex );
	void DJSS_MyHangUpDetect( int nIndex  );
	
};

#endif 
