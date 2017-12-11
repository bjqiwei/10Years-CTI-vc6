// PublicCardObj.h: interface for the CPublicCardObj class.
//
//////////////////////////////////////////////////////////////////////

#ifndef PUBLICCARDOBJ_H
#define PUBLICCARDOBJ_H

#include "..\\include\\CFCHH.h"
#include "CIF5188.h"
#include "..\\include\\MSgPackDefine.h"
#include "..\\include\\MessageType.h"
#include "..\\include\\OneCallCTIDLL.h"

class CPublicCardObj  
{
public:
	CPublicCardObj();
	virtual ~CPublicCardObj();
public:
	CProgressCtrl *m_pwndProg;
	CListCtrl	*m_plistTCPIP;
	CListBox	*m_pCommList;
	CListCtrl	*m_plistScript;
	CListCtrl	*m_plstChannel;
	CListBox	*m_plistSystem;
	CListCtrl	*m_plstConfer;
	CListCtrl	*m_plstFax;
public:
	//CString m_strMSG;
	HWND m_hWnd;
	int m_nIsSaveSysLog;
public:
	CNewDBConnVarList m_SysDBConnVarList;
	CFDLLVariableList m_SysDLLVarList;
	CNormalVarList	  m_SysNormalVarList;
	CNewDBConnVarList *m_pSysDBConnVarList;
	CFDLLVariableList *m_pSysDLLVarList;
	CNormalVarList	  *m_pSysNormalVarList;
private:
	CString m_csFileName;
	CStdioFile m_ErrorLogFile;
public:
	BOOL PC_GetNorVarValues( CFChannelInfo *pInfo, CString &strValues, int nVarKey);
	BOOL PC_GetNorVarValues( CFChannelInfo *pInfo,CString &strValues, int &nVarType, int nVarKey);
	BOOL PC_GetSysNorVarValues(CString &strValues, int nVarKey);
	BOOL PC_GetSysNorVarValues(CString &strValues, int &nVarType, int nVarKey);
	BOOL PC_GetExpressValues( CFChannelInfo *pInfo, CString &strNewExpress, CString strOldExpress);
	BOOL PC_SaveValuesToSysVar( CString strValues, int nVarKey);
	BOOL PC_SaveValuesToVar( CFChannelInfo *pInfo , CString strValues, int nVarKey);
	long PC_PlayDTMF(long lDTMF);
	void PC_InitSystemVar();
	BOOL PC_InitSysScript(CCFSysScriptSetObj *pObj);
	BOOL PC_LoadSysScript( CCIFSysScriptDoc *pDoc );
	CNodeObj * PC_MoveNextObj(int nNextType, CNodeObj *pCurObj, CCFDocument *pDoc);
	CNodeObj * PC_OpenDatabase(CNodeObj *pCurObj, CCFDocument *pDoc);
	CNodeObj * PC_DLLLoad(CNodeObj *pCurObj, CCFDocument *pDoc);
	void PC_DLLLoad(int nIndex,CFChannelInfo *pChnlObj);
	void PC_DLLUnLoad(int nIndex,CFChannelInfo *pChnlObj);
	void PC_DLLCall(int nIndex,CFChannelInfo *pChnlObj);
	CDLLVariable * PC_FindDLLVariable( CFChannelInfo *pChnlInfo, int nKey);
	CNewDBConnVar * PC_FindSystemDBVar(int nKey);
	CDLLVariable * PC_FindSystemDLLVar(int nKey);
	BOOL PC_MoveNext_Node(int nIndex,CFChannelInfo *pChnlObj,CNextNode *pNextObj);
	BOOL PC_MoveNext_Node(int nIndex ,CFChannelInfo *pChnlObj,int nNextType);
	void PC_SetVariable( int nIndex, CFChannelInfo *pChnlObj);
	void PC_OpenDatabase(int nIndex,CFChannelInfo *pChnlObj);
	void PC_DBBrowse_GetFieldValuse(int nIndex,CFChannelInfo *pChnlObj,CNewRecordsetVar * pRS, CDBBrowseObj *pObj);
	void PC_DBBrowse(int nIndex,CFChannelInfo *pChnlObj );
	void PC_ExcuteSQL(int nIndex,CFChannelInfo *pChnlObj);
	CNodeObj * PC_ExcuteSQL(CNodeObj *pCurObj, CCFDocument *pDoc);
	void PC_CloseDatabase(int nIndex,CFChannelInfo *pChnlObj);
	void PC_StoredProcedures(int nIndex, CFChannelInfo *pChnlInfo );
	BOOL PC_SaveValuesToVar(int nIndex,CFChannelInfo *pChnlObj, CString strValues, int nVarKey);
	void PC_Enter(int nIndex,CFChannelInfo *pChnlObj);
	void PC_Return(int nIndex,CFChannelInfo *pChnlObj);
	void PC_Block(int nIndex,CFChannelInfo *pChnlObj);
	long PC_ElapseTime( CFChannelInfo *pChnlInfo );
	void PC_StartTimer( CFChannelInfo *pChnlInfo );
	void PC_GoTo(int nIndex,CFChannelInfo *pChnlInfo );
	void PC_Compare(int nIndex,CFChannelInfo *pChnlObj);
	void PC_Express(int nIndex,CFChannelInfo *pChnlInfo);
	BOOL PC_Express_Double(int nIndex,CFChannelInfo *pChnlInfo,CString strExpress, double &dResult);
	BOOL PC_Express_Integer(int nIndex,CFChannelInfo *pChnlInfo,CString strExpress, int &nResult);
	BOOL PC_Express_Str(int nIndex,CFChannelInfo *pChnlInfo,CString strExpress, CString &strResult);
public:
	void PC_CT_ConnectValidate(int nDN );

	void PC_CTI_LoginResponseMsg(int nDN , int nResult );
	void PC_CTI_LogOutResponseMsg(int nDN , int nResult );
	void PC_CTI_ReadyResponseMsg(int nDN , int nResult );
	void PC_CTI_NotReadyResponseMsg(int nDN , int nResult );
	void PC_CTI_AnswerCallResponseMsg(int nDN, int nResult);
	void PC_CTI_CancelTransferResponseMsg(int nDN, int nResult, char *pszTel);
	void PC_CTI_ConferenceCallResponseMsg(int nDN, int nResult);
	void PC_CTI_JoinConferenceResponseMsg(int nDN, int nResult);
	void PC_CTI_DisMonitorResponseMsg(int nDN, int nResult);
	void PC_CTI_GetActiveCallResponseMsg(int nDN, int nResult,const char *pszCallerID, const char *pszCalledID );
	BOOL PC_TransferToIVR( int nIndex,int NodeKey, CFChannelInfo *pChnlInfo );
	BOOL PC_GetParameterValues( CFChannelInfo *pInfo, CString strOld, CString &str1, CString &str2,CString &str3,CString &str4,CString &str5);
	BOOL PC_GetParameterValues( CFChannelInfo *pInfo, CString strOld, CString &str1, CString &str2,CString &str3,CString &str4);
	BOOL PC_GetParameterValues( CFChannelInfo *pInfo, CString strOld, CString &str1, CString &str2,CString &str3);
	BOOL PC_GetParameterValues( CFChannelInfo *pInfo, CString strOld, CString &str1, CString &str2);
	BOOL PC_GetParameterValues( CFChannelInfo *pInfo, CString strOld, CString &str1);

	BOOL PC_DivideString(CString strOld, LPCTSTR lpszSub,CString &str1, CString &str2,CString &str3,CString &str4,CString &str5);
	BOOL PC_DivideString(CString strOld, LPCTSTR lpszSub,CString &str1, CString &str2,CString &str3,CString &str4);
	BOOL PC_DivideString(CString strOld, LPCTSTR lpszSub,CString &str1, CString &str2,CString &str3);
	BOOL PC_DivideString(CString strOld, LPCTSTR lpszSub,CString &str1, CString &str2);
	BOOL PC_DivideString(CString strOld, LPCTSTR lpszSub,CString &str1);

	void PC_Cus_MoveNextFun( CFChannelInfo *pChnlObj, CCustomObj *pObj );
	void PC_Cus_Express(int nIndex,CFChannelInfo *pChnlInfo,CCustomObj *pObj );
	void PC_MoudleEnd(int nIndex, CFChannelInfo *pChnlInfo);
	void PC_Moudle(int nIndex,CFChannelInfo *pChnlInfo);
	void PC_TraceInfo(int nIndex, CFChannelInfo *pChnlInfo);
	void PC_CTI_TransferResponseMsg(int nDN, int nResult, char *pszTel);
	void PC_CTI_MonitorResponseMsg(int nDN, int nResult);
	void PC_CTI_HoldResponseMsg(int nDN, int nResult);
	void PC_CTI_PickupHoldResponseMsg(int nDN, int nResult,const char *pszCallerID, const char *pszCalledID  );
	void PC_CTI_MakeCallResponseMsg(int nDN, int nResult, const char *pszTel);
	void PC_CTI_RingingMsg(int nDN, const char *pszTel, const char *pszTel2);
	void PC_CTI_HangUpMsg(int nIndex, CFChannelInfo *pChnlInfo);
	void PC_CTI_OffHookMsg(int nIndex,CFChannelInfo *pChnlInfo);
	void PC_WriteSysLog(LPCTSTR lpsz);
	bool PC_OpenLogFile(void);
	int PC_FindFreeChannel(int nChnlType,int nTotalChnl, CFChannelInfo *pArrChnlInfo);
	int PC_FindUerChannel(CString strLocalDN,int nTotalChnl, CFChannelInfo *pArrChnlInfo);
	int PC_GetHoldChnl(int nIndex,int nTotalChnl, CFChannelInfo *pArrChnlInfo);
	void PC_Function(int nIndex, CFChannelInfo *pChnlInfo,int nTotalChnl ,CFChannelInfo *pArrChnlInfo);
	void PC_ClearVoxFileEndDTMF( LPCTSTR lpszFileName );
	void PC_ShowChannelConferenceNO(int nIndex, int nConferNO);
	void PC_ShowChannelState(int nIndex, CString strState);
	void PC_ShowChannelDTMF(int nIndex, CString strDTMF);
	void PC_ShowChannelBlock(int nIndex, CString strBlock);
	void PC_ShowSysMsg(int nIndex, CString strMSG);
	void PC_ShowCommMsg(CString strMSG);
	void PC_ShowConnectToChnl(int nIndex, int nConnectToIndex);
	void PC_ShowChannelCallerID(int nIndex, CString strCallerID);
	void PC_ShowChannelCalledID(int nIndex, CString strCalledID);
	void PC_ShowFaxUpdate(int nFaxIndex ,CFaxStateList *pFaxChannel );
};

#endif // !defined(AFX_PUBLICCARDOBJ_H__9E10C44C_490D_47D5_8EFE_A4C4518B2397__INCLUDED_)
