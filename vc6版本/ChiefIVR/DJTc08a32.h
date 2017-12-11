// DJTc08a32.h: interface for the CDJTc08a32 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef DJTC08A32_H
#define DJTC08A32_H

#include "CFCHH.h"
#include "CIF5188.h"
#include "PublicCardObj.h"

#include "..\\include\\MSgPackDefine.h"
#include "..\\include\\MessageType.h"
#include "..\\include\\OneCallCTIDLL.h"

#include "..\\include\\Tc08a32.h"
#include "..\\include\\NewSig.h"
#include "..\\include\\NewConf.h"
#include "..\\include\\conf95.h"
#include "..\\include\\faxapi32.h"


class  CDJTc08a32  
{
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
public:
	int TC08_GetHoldChnl(int nIndex);
	void TC08_AnswerCall(int nIndex);
	BOOL TC08_EventCheck_User( int nIndex , CEventCheckObj *pObj );
	BOOL TC08_EventCheck_Trunk( int nIndex,  CEventCheckObj *pObj );
	BOOL TC08_EventCheck_EventMsg( int nIndex, CEventCheckObj *pObj );
	void TC08_CallHold_PickUp(int nIndex, CCallHoldObj *pObj);
	int  TC08_FindFreeChannel( CString strLocalDN  );
	void TC08_Switch_CallAgent(int nIndex, CSwitchObj *pObj);
	void TC08_Switch_CallCustomer( int nIndex , CSwitchObj *pObj );
	int  TC08_FindFreeChannel(int nChnlType  );
	void TC08_Switch( int nIndex );
	void TC08_NodeEnd( int nIndex );
	void TC08_ReleaseCall_Trunk(int nIndex);
	void TC08_ReleaseCall_User(int nIndex);
	BOOL TC08_MoveNext_Node( int nIndex, int nNextType );
	int  TC08_FindFreeUserChannel( CString strLocalDN );
	void TC08_EventCheck( int nIndex );
	void TC08_ResetUserChannel( int nIndex );
	void TC08_CallHold( int nIndex );
	void TC08_DialCheck(int nIndex);
	int  TC08_FindFreeTrunk( void );
	void TC08_DTMFRecive(int nIndex);
	void TC08_CallLogRequest(int nIndex);
	void TC08_ReleaseCall(int nIndex);
	void TC08_DTMFMenu( int nIndex );
	char TC08_GetCodeAscii(int code );
	void TC08_PlayStart(int nIndex);
	void TC08_ResetTrunkChannel(int nIndex );
	void TC08_WaitCall( int nIndex );
	void TC08_DoWork( int nIndex );
	bool TC08_InitSystem(CListCtrl	*plstChannel,
								CListBox	*plistSystem,
								CListCtrl	*plistScript,
								CListBox	*pCommList,
								CListCtrl	*plistTCPIP,
								CListCtrl	*plstConfer,
								CListCtrl	*plstFax
								);
	CDJTc08a32();
	virtual ~CDJTc08a32();
public:
	BOOL TC08_InitChannel( CCFChnlSetObj *pChSetObj, int nChnlIndex);
private:
	void TC08_ExitVoiceSys( void );

public:
	BOOL TC08_MyCheckRecordEnd( int nIndex );
	BOOL TC08_MyCheckPlayEnd( int nIndex);
	BOOL TC08_MyStartRecordFile(  int nIndex, CString strFile, BOOL bRecordLoop=FALSE,int nRecordType=0,long lRecordLen=3600);
	void TC08_Cus_StopPlayFile( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );
	void TC08_Cus_StopRecordFile( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );
	void TC08_Cus_HangUp( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );
	void TC08_Cus_ClearDtmf( int nIndex,  CFChannelInfo *pChnlInfo,  CCustomObj *pObj );

	BOOL TC08_Cus_GetAgentCall(int nIndex, CFChannelInfo *pChnlInfo, CString strDN );
	void TC08_Cus_OutCall(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TC08_Cus_OffHook(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TC08_Cus_ReciveDtmf(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TC08_Cus_LeaveConfer(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TC08_Cus_EnterConfer(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TC08_Cus_GetAgentCall(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TC08_Cus_StartRecordFile(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TC08_Cus_StartPlayFile(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TC08_Cus_SendFax(int nIndex, CFChannelInfo *pChnlInfo, CCustomObj *pObj);
	void TC08_Cus_ReciveFax(int nIndex,CFChannelInfo *pChnlInfo,CCustomObj *pObj );
	void TC08_Custom(int nIndex );
	void TC08_Switch_CallFreeAgent(int nIndex, CSwitchObj *pObj);
	inline void TC08_MyStopRecordFile( int nIndex );
	inline void TC08_MyInitDtmfBuf( int nIndex );
	BOOL TC08_SendCallerID_DTMF(int nIndex, char *CallIDNumStr);
	int TC08_MyMakeCall_Check(int nIndex, int nTimeOut);
	int TC08_FindFreeTrunk( int nBeginCH, int nEndCH );
	BOOL TC08_MyStartPlayFile(  int nIndex, CString strFile, BOOL bPlayLoop=FALSE,int nPlayType = 0/*放音类型：0普通 1会议*/ );
	void TC08_MyOFFHook(int nIndex );
	void TC08_FaxKey(int nIndex);
	void TC08_TransferKey(int nIndex);
	inline void TC08_EventCheck_Play(int nIndex, CString strDTMF, CEventCheckObj *pObj );
	void TC08_ConferenceLeave( int nIndex );
	void TC08_Transfer_ToCustomer(int nIndex,CTransferCheckObj *pObj  );
	inline  BOOL TC08_GetDtmfCode(int nIndex, CString &strDTMF );
	void TC08_Transfer_ToAgent(int nIndex,CTransferCheckObj *pObj );
	void TC08_Transfer_Cancel(int nIndex, CTransferCheckObj *pObj);
	void TC08_Transfer_Start(int nIndex, CTransferCheckObj *pObj);
	void TC08_MyHangUp( int nIndex );
	void TC08_Switch_Aim(int nIndex, CSwitchObj *pObj);
	void TC08_Switch_Self(int nIndex, CSwitchObj *pObj);
	void TC08_MyStopPlay( int nIndex );
	int	 TC08_MyMakeCall_Check(int nIndex);
	BOOL TC08_MyMakeCall(int nIndex, char *szCalledID, char *szCallerID);
	BOOL TC08_PlayExpress( long lDTMF );
	void TC08_TCPIP( int nIndex );
	BOOL TC08_EventCheck_Record(int nIndex, CEventCheckObj *pObj);
	void TC08_WaitCall_Record( int nIndex,CWaitCallObj *pObj);
	void TC08_WaitCall_Trunk(int nIndex,CWaitCallObj *pObj);
	void TC08_DTMFSend(int nIndex);
	void TC08_FaxRecive( int nIndex );
	void TC08_EndCallEventEndBlock(int nIndex);
	void TC08_EndCallEventCheckBlock( int nIndex );
	void TC08_FaxSend(int nIndex);
	BOOL TC08_LoadNormalScript( void );
	void TC08_Release_Conference(int nIndex, int nConferNO );
	BOOL TC08_PlayExpressVox(int nIndex, CString strExpress );
	BOOL TC08_MyConferLeave(int nIndex, int nConferIndex);
	BOOL TC08_MyConferEnter( int nIndex, int nConferIndex, int nEnterType );
	void TC08_ConferenceEnter(int nIndex );
	long TC08_PlayDTMF( long lDTMF );
	BOOL TC08_LoadSysScript( void );
	void TC08_MyHangUpDetect(int nIndex );
	int TC08_FindHoldChannel( int nIndex );
	void TC08_DealCTIMsg( CFC_MESSAGE_PACK *pMsg );
	void TC08_StartRecord( int nIndex );
	int	 TC08_FindUerChannel(CString strLocalDN);
	void TC08_FeedSig_PUSH( void );

	BOOL inline TC08_MyClearLink(int nIndex1, int nIndex2 );
	BOOL inline TC08_MySetLink( int nIndex1, int nIndex2 , int nSwitchType);
	void TC08_Transfer(int nIndex);
	BOOL TC08_MoveNext_Node( int nIndex , CNextNode * pNextObj );

};

#endif 
