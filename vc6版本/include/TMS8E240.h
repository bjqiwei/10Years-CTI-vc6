// TMS8E240.h: interface for the CTMS8E240 class.
//
//////////////////////////////////////////////////////////////////////

#ifndef TMS8E240_H
#define TMS8E240_H

#include "..\\include\\CFCHH.h"
#include "..\\include\\CIF5188.h"
#include "..\\include\\MSgPackDefine.h"
#include "..\\include\\MessageType.h"
#include "..\\include\\OneCallCTIDLL.h"

#include "..\\include\\structO.h"
#include "..\\include\\PCIcard.h"


#include "..\\include\\CFCHH.h"
#include "..\\include\\CIF5188.h"
#include "..\\include\\MSgPackDefine.h"
#include "..\\include\\MessageType.h"
#include "..\\include\\OneCallCTIDLL.h"



class AFX_EXT_CLASS CTMS8E240  
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
	CNewDBConnVarList m_SysDBConnVarList;
	CFDLLVariableList m_SysDLLVarList;
	CNormalVarList	  m_SysNormalVarList;
public:
	BOOL   m_bThreadEnd;
	int m_nThreadCount;//启动的线程数
	CFChannelInfo m_pChannelInfo[MAX_CHANNEL_COUNT];
public:
	long m_lCardKey;
	long m_lReadPass;
public:
	CNodeObj * TMS_MoveNextObj(int nNextType, CNodeObj *pCurObj, CCFDocument *pDoc);
	BOOL TMS_MoveNext_Node(int nIndex, CNextNode *pNextObj);
	BOOL TMS_MoveNext_Node(int nIndex, int nNextType);
	void TMS_ShowSysMsg(int nIndex, CString strMSG);
	void TMS_ShowChannelState(int nIndex, CString strState);
	void TMS_ShowChannelDTMF(int nIndex, CString strDTMF);
	void TMS_ShowChannelBlock(int nIndex, CString strBlock);
	void TMS_ShowCommMsg(CString strMSG);
	void TMS_ShowConnectToChnl(int nIndex, int nConnectToIndex);
	void TMS_ShowChannelCallerID(int nIndex, CString strCallerID);
	void TMS_ShowChannelCalledID(int nIndex, CString strCalledID);
	void TMS_ShowChannelConferenceNO(int nIndex, int nConferNO);
	CNewDBConnVar * TMS_FindSystemDBVar(int nKey);
	CNodeObj * TMS_OpenDatabase(CNodeObj *pCurObj, CCFDocument *pDoc);
	BOOL TMS_InitSysScript(CCFSysScriptSetObj *pObj);
	long TMS_PlayDTMF(long lDTMF);
	BOOL TMS_LoadSysScript( void );
	BOOL TMS_LoadNormalScript( void );
	void TMS_InitSystemVar( void );
	BOOL TMS_FriendVoiceSys(CListCtrl *plstChannel, CListBox *plistSystem, CListCtrl *plistScript, CListBox *pCommList, CListCtrl *plistTCPIP, CListCtrl *plstConfer, CListCtrl *plstFax);
public:
	void TMS_DoWork( int nIndex );
	int TMS_GetDevChnlType(int nCifChnlType);
	int TMS_GetCifChnlType( int nDevChnlType );
	int TMS_GetCifLogicChnl(int nDevChnlIndex, int nDevChnlType);
	void TMS_MSG_Call( RMSG *pmsg );
	void TMS_ProcessMsg( RMSG  *pmsg );
	void TMS_MainProcess( void );
	void TMS_Conf_ReleaseConf();
	void TMS_ResetChannel(int nIndex);
	BOOL TMS_InitChannel(CCFChnlSetObj *pChSetObj, int nChnlIndex );
	void TMS_InitConfCard( void );
	void TMS_StartTimer(int nIndex);
	long TMS_ElapseTime(int nIndex);
public:
	void TMS_MSG_CallOutChannel( RMSG *pmsg );
	void TMS_Dial_Result(int nIndex, RMSG *pmsg);
	void TMS_Call_Customer( int nIndex, const char *calledstr, const char *callerstr );
	void TMS_DialCheck( int nIndex );
	void TMS_MSG_CalledBack(RMSG *pmsg);
	void TMS_Play_Result(int nIndex, RMSG *pmsg);
	BOOL TMS_GetExpressValues(int nIndex, CString &strNewExpress, CString strOldExpress);
	void TMS_PlayExpressVox(int nIndex, CNodeDataList *p, CString strExpress);
	void TMS_Play_Start(int nIndex,CPlayStartObj *pObj);
	void TMS_PlayFile(RMSG &msg,char *file, unsigned short isfirst, unsigned short islast,unsigned short bDTMFStop);
	void TMS_PlayTTS(RMSG &msg,char *str,unsigned char isfirst,unsigned char islast, unsigned short bDTMFStop);
	void TMS_PlayTTS( int nIndex,char *str,unsigned char isfirst,unsigned char islast, unsigned short bDTMFStop);
	void TMS_PlayFile(int nIndex ,char *file,unsigned short isfirst, unsigned short islast,unsigned short bDTMFStop);
	void TMS_Play( int  nIndex );
	void TMS_DLLUnLoad(int nIndex);
	void TMS_DLLCall(int nIndex);
	CDLLVariable * TMS_FindDLLVariable(int nIndex, int nKey);
	void TMS_DLLLoad(int nIndex);
	BOOL TMS_GetSysNorVarValues(CString &strValues, int nVarKey);
	BOOL TMS_GetNorVarValues(int nIndex, CString &strValues, int nVarKey);
	void TMS_Compare(int nIndex);
	void TMS_StoredProcedures(int nIndex);
	void TMS_ExcuteSQL(int nIndex);
	void TMS_DBBrowse_GetFieldValuse(int nIndex, CDBBrowseObj *pObj);
	void TMS_DBBrowse_Excute(int nIndex, CDBBrowseObj *pObj);
	void TMS_DBBrowse(int nIndex);
	void TMS_CloseDatabase(int nIndex);
	void TMS_OpenDatabase(int nIndex);
	void TMS_Enter(int nIndex);
	void TMS_Block(int nIndex);
	void TMS_NodeEnd(int nIndex);
	void TMS_GoTo(int nIndex);
	void TMS_EndCallEventEndBlock(int nIndex);
	void TMS_EndCallEventCheckBlock(int nIndex);
	BOOL TMS_EventCheck_Release(int nIndex, RMSG *pmsg);
	void TMS_Release(  int nIndex );
	void TMS_MSG_Release(RMSG *pmsg);
	void TMS_SetVariable( int nIndex );
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
	void TMS_Ack( RMSG *pmsg );
	void TMS_WaitCall( int nIndex, RMSG *pmsg );
	int m_nTotalFaxLine;//系统检测到的传真卡通道数
	int m_nFax;//系统是否有传真卡，0没有，1有
	FAXSTRUCT m_pFaxChannel[MAX_FAX_CHANNEL_COUNT];
	CNodeObj * m_pNodeObj;
	CNormalScriptInfo *m_pNormalScriptInfo;
	int m_nNormalScriptNum;//普通脚本文件数量
};

#endif
