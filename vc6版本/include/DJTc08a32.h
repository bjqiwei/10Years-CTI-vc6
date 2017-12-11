// DJTc08a32.h: interface for the CDJTc08a32 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef DJTC08A32_H
#define DJTC08A32_H

#include "..\\include\\CFCHH.h"
#include "..\\include\\CIF5188.h"

#include "..\\include\\MSgPackDefine.h"
#include "..\\include\\MessageType.h"
#include "..\\include\\OneCallCTIDLL.h"

#include "..\\include\\Tc08a32.h"
#include "..\\include\\NewSig.h"
#include "..\\include\\NewConf.h"
#include "..\\include\\conf95.h"
#include "..\\include\\faxapi32.h"

class AFX_EXT_CLASS CDJTc08a32  
{
public:
	CFChannelInfo m_pChannelInfo[MAX_CHANNEL_COUNT];
	int m_nTotalChannel;
	BOOL   m_bThreadEnd;
	int m_nThreadCount;//启动的线程数
	BOOL m_bInitVoiceCard;//初始化语音卡是否成功
	long m_lCardKey;
	long m_lReadPass;
	HWND m_hWnd;
	//会议组,他的值为发起该会议的通道号，
	//如为-1表示该组会议还位使用
	int m_GroupCh[MAX_GROUP_NUM+1];	
	CString m_strMSG;
public:
	CNormalScriptInfo *m_pNormalScriptInfo;
	int m_nNormalScriptNum;//普通脚本文件数量
public:
	int m_nTotalFaxLine;//系统检测到的传真卡通道数
	int m_nFax;//系统是否有传真卡，0没有，1有
	FAXSTRUCT m_pFaxChannel[MAX_FAX_CHANNEL_COUNT];
	CNodeObj * m_pNodeObj;
public:
	CNewDBConnVar * FindSystemDBVar( int nKey);
	void OpenDatabase( int nChnl );
	CString m_strPath;
	void ShowChannelState(int nChnl, CString strState);
	void ShowChannelDTMF(int nChnl, CString strDTMF);
	void ShowChannelBlock(int nChnl, CString strBlock);
	void ShowSysMsg(int nChnl, CString strMSG);
	void AnswerCall(int nChnl);
	BOOL EventCheck_User( int nChnl , CEventCheckObj *pObj );
	BOOL EventCheck_Trunk( int nChnl,  CEventCheckObj *pObj );
	BOOL EventCheck_EventMsg( int nChnl, CEventCheckObj *pObj );
	void CallHold_PickUp(int nChnl, CCallHoldObj *pObj);
	BOOL CallHold_ReciveDTMF(int nChnl, CCallHoldObj *pObj);
	int FindFreeChannel( CString strCalledID  );
	int Switch_GetFreeChnlIndex(int nChnl,  CSwitchObj *pObj);
	void Switch_CallAgent(int nChnl, CSwitchObj *pObj);
	void Switch_CallCustomer( int nChnl , CSwitchObj *pObj );
	int  FindFreeChannel(int nChnlType  );
	void Switch_Channel( int nChnl );
	void NodeEnd( int nChnl );
	void ReleaseCall_Trunk(int nChnl);
	void ReleaseCall_User(int nChnl);
	BOOL MoveNext_Node( int nChnl, int nNextType );
	void DialCallUser(int nChnl, CDialCheckObj *pObj);
	void DialCallCustomer( int nChnl,CDialCheckObj *pObj );
	int FindFreeUserChannel( CString strCalledID );
	void EventCheck( int nChnl );
	void ResetUserChannel( int nChnl );
	void CallHold( int nChnl );
	void DialCheck(int nChnl);
	int FindFreeTrunk( void );
	void DialStart(int nChnl);
	void DTMFRecive(int nChnl);
	void SetVariable(int nChnl);
	void CallLogRequest(int nChnl);
	void ReleaseCall(int nChnl);
	void DTMFMenu( int nChnl );
	char GetCodeAscii(int code );
	void PlayStart(int nChnl);
	void ResetTrunkChannel(int nChnl );
	void WaitCall( int nChnl );
	void DoWork( int nChnl );

	bool FriendVoiceSys(CListCtrl	*plstChannel,
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
	BOOL InitChannel( CCFChnlSetObj *pChSetObj);
	void CloseDatabase(int nChnl);
private:
	void ExitVoiceSys( void );
private:


private:
	CListCtrl	*m_plistTCPIP;
	CListBox	*m_pCommList;
	CListCtrl	*m_plistScript;
	CListCtrl	*m_plstChannel;
	CListBox	*m_plistSystem;
	CListCtrl	*m_plstConfer;
	CListCtrl	*m_plstFax;
public:
	int m_nSysType;
	CString m_strCommIPAdrr;
	unsigned int m_nCommPort;
public:
	void TC08_StartTimer(int nIndex);
	long TC08_ElapseTime(int nIndex);
	void TC08_EndCallEventEndBlock(int nIndex);
	void TC08_EndCallEventCheckBlock( int nIndex );
	void TC08_GoTo(int nIndex);
	void TC08_Enter(int nIndex);
	void TC08_Block(int nIndex);
	void TC08_StoredProcedures(int nIndex);
	CDLLVariable * FindDLLVariable(int nIndex, int nKey);
	void TC08_DLLUnLoad(int nIndex);
	void TC08_DLLCall(int nIndex);
	void TC08_DLLLoad(int nIndex);
	void TC08_FaxSend(int nIndex);
	BOOL LoadNormalScript( void );
	void GoTo(int nChnl );
	void Enter(int nChnl );
	void Block(int nChnl );
	void Release_Conference(int nChnl, int nConferNO );
	BOOL PlayExpressVox(int nChnl, CString strExpress );
	void Confer_CallCustomer(int nChnl, CConferRequestObj *pObj);
	void Confer_CallAgent(int nChnl, CConferRequestObj *pObj);
	void Confer_CallStart(  int nChnl ,CConferRequestObj *pObj  );
	void ShowConferenceNO( int nChnl, int nConferNO );
	BOOL MyEnterConfer( int nChnl, int nConferIndex, int nEnterType );
	void ConferenceEnter(int nChnl );
	void Confer_Call(int nChnl);
	int GetConferIndexForExpress( CString strExpress );
	int FindFreeConferIndex( void );
	void ConferenceBuild(int nChnl);

	long PlayDTMF( long lDTMF );
	void ShowChannelCallerID(int nChnl, CString strCallerID);
	void ShowConnectToChnl(int nChnl, int nConnectToChnl );
	CNodeObj * OpenDatabase(CNodeObj *pCurObj, CCFDocument *pDoc );
	CNodeObj * MoveNextObj(int nNextType, CNodeObj *pCurObj, CCFDocument *pDoc );
	BOOL InitSysScript( CCFSysScriptSetObj *pObj );
	BOOL LoadSysScript( void );
	void MyHangUpDetect(int nChnl );
	int FindHoldChannel( int nChnl );
	void ShowCommMsg( CString strMSG );
	void DealCTIMsg( CFC_MESSAGE_PACK *pMsg );
	BOOL GetExpressValues( int nChnl, CString &strNewExpress, CString strOldExpress );
	void StartRecord( int nChnl );
	BOOL GetSysNorVarValues( CString &strValues, int nVarKey);
	BOOL GetNorVarValues( int nChnl, CString &strValues, int nVarKey);
	int Switch_GetChnlIndex(int nChnl, CSwitchObj *pObj );
	int FindChannel(CString strCalledID);
	int FindUerChannel(CString strCalledID);
	void Switch_Connect(int nChnl, CSwitchObj *pObj);
	void Switch_Monitor(int nChnl, CSwitchObj *pObj);
	void Switch_Speaker(int nChnl, CSwitchObj *pObj);
	void Switch_DisConnect(int nChnl, CSwitchObj *pObj);

	void FeedSig_PUSH( void );

	BOOL inline MyClearLink(int nChnl1, int nChnl2 );
	BOOL inline MySetLink( int nChnl1, int nChnl2 , int nSwitchType);
	void TransferCheck(int nChnl);
	BOOL MoveNext_Node( int nChnl , CNextNode * pNextObj );
	void Compare( int nChnl );
	void DBBrowse_GetFieldValuse(int nChnl, CDBBrowseObj *pObj );
	void DBBrowse_Excute(int nChnl, CDBBrowseObj *pObj );
	void DBBrowse(int nChnl);
	void ExcuteSQL( int nChnl );
	void InitSystemVar( void );
	CNewDBConnVarList m_SysDBConnVarList;
	CFDLLVariableList m_SysDLLVarList;
	CNormalVarList	  m_SysNormalVarList;
};

#endif 
