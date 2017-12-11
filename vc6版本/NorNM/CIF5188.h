#ifndef CIF5188_H
#define CIF5188_H


#include "..\\include\\NodeObj.h"

extern CD08A *g_pD08a;


#define WM_MY_FRIEND_MSG		WM_USER+220//发送卡的序列号消息
#define MAX_CHANNEL_COUNT		60		//最大通道数
#define MAX_FAX_CHANNEL_COUNT	2		//最大的FAX通道数


class CFaxStateList
{
public:
	CFaxStateList(){
	m_nStatus = 0;
	m_nCurrPage = 0;
	m_nTotalPage = 0;
	m_strFaxFileName = "";
	m_strPhone = "";
	};
	~CFaxStateList(){};
public:
	int m_nStatus;
	int m_nCurrPage;
	int m_nTotalPage;
	CString m_strFaxFileName;
	CString m_strPhone;
};


class CPlayFileList
{
public:
	CPlayFileList(){ m_nIndex = -1; m_strPlayFile = ""; };
	~CPlayFileList(){};
public:
	int m_nIndex;
	CString m_strPlayFile;
};


enum CONF_STATE
{
	CONF_FREE,//空闲状态，没有占用
	CONF_RECORD,//处于会议录音状态
	CONF_PLAY,//处于会议放音状态
	CONF_PLAY_INDEX,//播放文件列表
	CONF_ADD,
	CONF_EXIT,
	CONF_LEAVE
};
typedef struct
{
	CONF_STATE State;
	int nChnlIndex;//创建会议的通道号，当处于空闲状态时，其值为-1
	int nPlayIndex;//发起该会议放音的通道号
	int nRecordIndex;//发起该会议录音的通道号
	int nConfPlayID;//会议放音语音通道号；当处于空闲状态时，其值为-1
	int nConfRecordID;//会议录音语音通道号；当处于空闲状态时，其值为-1
	int nVocID;//加入或离开会议语音通道
	int nRecID;//加入或离开会议语音通道
	int nMemberNum;
	int nResNum;
	int nListenNum;
}CONF_STRUCT;


#define CIF_FAX_FREE        0
#define CIF_FAX_RECEIVE     1
#define CIF_FAX_SEND        2

//会议组的状态常量
#define CIF_CONF_NULL			0 //空闲(既没有创建或加入会议)
#define CIF_CONF_LISTEN		    1 //只听成员，在会议中只能听，不能说
#define CIF_CONF_SPEAKER_LISTEN 2 //普通成员，可说可听，具有动态发言权
#define CIF_CONF_SPEAKER_ALWAYS 3 //始终说成员，总是具有发言权
#define CIF_CONF_SPEAKER_ONLY   4 //只说成员，在会议中只能说，不能听，主要用于对会议放音。
#define CIF_CONF_CREAT          5 //创建了该会议，同时可说可听，有权管理该会议

 void NewAssignsObjList( const CNodeObjList &s , CNodeObjList *d,int nAssigns );
///////////////////////////////////////////////
//class  CNewRecordsetVar
//////////////////////////////////////////////
class  CNewRecordsetVar : public CRecordsetVar  
{
public:
	CNewRecordsetVar();
	virtual ~CNewRecordsetVar();
	CNewRecordsetVar( const CNewRecordsetVar &s );
	CNewRecordsetVar( const CRecordsetVar &s );
	CNewRecordsetVar & operator =( const CNewRecordsetVar &s );

public:
	void PrintComError(CString &ErrStr , _com_error &e);
	_RecordsetPtr m_pRecordset;
	_CommandPtr m_pCommand;
};

typedef  CTypedPtrList<CObList, CNewRecordsetVar*> CNewRecordsetVarList;

///////////////////////////////////////////////
//class  CNewDBConnVar
//////////////////////////////////////////////

class  CNewDBConnVar:public CDBConnVar
{
public:
	BOOL OpenDataBase( CString strConn, CString &strError );
	BOOL CloseDatabase( CString &strError );
	CNewDBConnVar();
	CNewDBConnVar( const CNewDBConnVar &s );
	CNewDBConnVar( const CDBConnVar &s );
	CNewDBConnVar & operator =( const CNewDBConnVar &s );
	virtual ~CNewDBConnVar();
public:
	void PrintProviderError( CString &ErrStr, _ConnectionPtr pConnection);
	void PrintComError(CString &ErrStr , _com_error &e);
	BOOL ExcuteSQL(  CString strSQL, CString &strError );
	BOOL ExcuteSQL( CNewRecordsetVar *RS,CString strSQL,CString &strError );
	_ConnectionPtr	m_pConnection;
	BOOL m_bConnect;
	_RecordsetPtr m_pRecordset;
	_CommandPtr m_pCommand;

};

typedef  CTypedPtrList<CObList, CNewDBConnVar*> CNewDBConnVarList;

///////////////////////////////////////////////
//class  CFChannelInfo
//////////////////////////////////////////////
class   CFChannelInfo
{

public:
	CFChannelInfo();
	virtual ~CFChannelInfo();
public:
	void MoveFirst_Node( void );
	bool SaveToNorVar( CString strValues, int nVarKey );
	BOOL MoveNext_Node( int nNextType );
	void RemoveALLObjects();

	CNodeObj * FindNodeObj( int nKey );
public:
	BOOL GetNorVarValues(CString &strValues, int &nVarType, int nVarKey);
	BOOL GetSQLExpressValues( CString &strNewExpress, CString strOldExpress );
	BOOL MoveNext_Node( CNextNode *pNextObj );
	void ExcuteSQL( void );
	CNewRecordsetVar * FindRecordsetVar( int nKey );
	CNewDBConnVar *  FindDBConnectVar( int nKey);
	bool GetNorVarValues(CString &strValues, int nVarKey);
	void PrintADOVersion( _ConnectionPtr pConnection );
	void PrintComError(_com_error &e);
	void PrintProviderError(_ConnectionPtr pConnection);
public:
	void PrintADOConnectString( _ConnectionPtr pConnection);
	void InitNormalVar( void );
	BOOL GetContantValues(CString &strValues, int &nVarType,int nVarKey);
	void MoveCallEndBlock_Node( void );
	BOOL GetConstAndVarValues( CString &strValues, int nVarKey);
	void ExcuteStoredPro( void );
	CDLLVariable * FindDLLVariable( int nKey );
	void ShowSysMsg(CString strMSG);
	BOOL GetContantValues(CString &strValues,  int nVarKey);
public:
	BOOL m_bRecordLoop;
	BOOL m_bPlayLoop;

	int m_nRecordType;//录音类型：0普通 1会议
	int m_nPlayType;//放音类型：0普通 1会议
public:
	CString m_strAgentPwd;//坐席密码
	CString m_strAgentID;//坐席工号
	CString m_nGroup;//坐席当前组
	CString m_strLoginTime;//坐席Login 时间
	CString m_strLogOutTime;//坐席Logout 时间
	CString m_strReadyTime;//坐席Ready 时间
	CString m_strNotReadyTime;//坐席Not Ready 时间
	CString m_strAgentName;//坐席姓名
	CString m_strGroupName;//坐席当前组名

public:
	int m_nDtmfLen;//接收DTMF的长度
	CString m_strDtmfEnd;//接收DTMF的结束按键
	int m_nSaveDtmfKey;//接收DTMF的保存变量的Key
	int m_nTimeOut;
public:
	BOOL m_bExcuteSQL;//执行SQL语句是否完成
	BOOL m_bRecord;//是否已要求录音
	CNewDBConnVar *m_pCurDBConn;
	CNewRecordsetVar *m_pCurRecset;
	CStoreProObj	*m_pCurStoreProObj;
	int m_nChanelType;
	int m_nLinkCh;//连接的通道
	int m_nFaxChnl;//占用的传真资源通道
	int m_nLinkType;//连接通道的连接类型
	int m_nTransferCh;//呼叫转移时的目标通道
	int m_nHoldChnl;//该被该通道Hold的通道号
	int m_nInvitConfChnl;//正在邀请加入会议的通道号
	int m_nConfNO;//所在会议资源编号
	int m_nConfAction;//该通道的会议行为，见定义
	int m_nState;//通道状态
	int m_nChannel;//通道编号
	int m_nIndex;//通道在通道数组里的编号
	int m_nChangeVoiceChnl;//变声通道号
	int m_nStep;//子状态步骤
	long m_lTimeElapse;//计时器变量，单位：秒
	int m_nRings;
	long m_lCardKey;
	int  m_nEventType;//时间类型比如挂机、拍插簧，收到软电话消息事件等
	int  m_nCallType;//呼叫类型：1 Inbound, 0 Outbound -1 not any call
	int  m_nRecordLength;
	int  m_nPlayGroup;//该通道正在控制会议放音的会议组
	int  m_nRecordGroup;//该通道正在控制会议录音音的会议组
	int  m_nHandleConferChnl;//操作的会议通道号,或通道号
	int  m_nHandleConferNo;//操作的会议号。

	CString m_strPlayFile;
	CString m_strScriptFile;
	CString m_strRecordFile;//存放录音文件名的表达试
	CString m_strReciveFaxFile;
	CString m_strSendFaxFile;
	CString m_strState;
	CString m_strtEndTime;//通话结束时间
	CString m_strtBeginTime;//通话开始时间
	CString m_strDTMFRecived;//接收的DMF传
	CString m_strRingTime;//开始振铃时间
	int m_nHangUpType;//挂机类型：0非主动挂机 1主动挂机
	CNodeObjList m_objects;//脚本节点对象
	CNewRecordsetVarList m_NewRecordsetVariList;
	CNewDBConnVarList m_NewDBConnVarList;
	CNormalVarList m_NormalVarList;
	CFDLLVariableList m_DLLVariableList;
	CNodeObj * m_pCurrentNodeObj;

	CString m_strCalledID;//被叫号码
	CString m_strCallerID;//主叫号码
	CString m_strLocalDN;//本地号码
	CString m_strInboundCallPK;//该通道进行的通话标识
	CString m_strOutboundCallPK;//该通道进行的通话标识

	int  m_nDialType;//拨号类型
	CString m_strDialTel;
	int m_nDialChIndex;//用于保存makecall等时使用的通道号

	int m_nPickUpType;//取回保持的类型
	CString m_strPickUpTel;//取回保持的主叫号码

	int m_nActiveType;
	CString m_strActiveTel;

	int m_nThreadCount;
	CListBox	*m_plistSystem;
	BOOL m_bEventCheckPlay;

	int m_nLineInit;//线路是否初始化复原；0未复原，1复原；

	int m_nCallInType;//0 分机来电 1非分机来电 只对内线有效
public:
	BOOL InPutVarValuesToParam( CParameter * pParam , int nVarKey);
	BOOL OutParamValuesToVariable(CString strValues, int nVarKey);
	BOOL SaveToModuleParam(CString strValues, int nVarKey);
	BOOL GetModuleParamValues(CString &strValues, int nVarKey);
	void SetCurrentNodeObj( CNodeObj *pObj );
	CNodeObj * GetCurrentNodeObj( void );
	BOOL m_bDBExcuteEnd;//数据库操作执行结束

	HANDLE m_Threadhandle;//当前该通道执行的线程句柄，打开数据库、执行存储过程和SQL语句的线程
};

class  CNormalScriptInfo  
{
public:
	CNormalScriptInfo();
	virtual ~CNormalScriptInfo();
	void InitNodeObjList(CNodeObjList *pObjList, CNodeObj *pObj);
	BOOL InitNormalScript( CString strFile );
public:
	CNodeObjList m_objects;
	CString m_strScriptFile;
public:
	CRecordsetVarList m_RecordsetVariList;
	CDBConnVarList m_DBConnVarList;
	CNormalVarList m_NormalVarList;
	CFDLLVariableList m_DLLVariableList;
};

 


class CNewModuleObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CNewModuleObj);
	CNewModuleObj();
public:

	virtual ~CNewModuleObj();
	CNewModuleObj( const CNewModuleObj &s );
	CNewModuleObj & operator =( const CNewModuleObj &s );
	CNewModuleObj( const CModuleObj &s );
	virtual void Serialize(CArchive& ar);
public:
	CNodeObj *m_pCurObj;
	CNodeObjList m_objects;
	CNewRecordsetVarList m_RecordsetVariList;
	CNewDBConnVarList m_DBConnVarList;
	CNormalVarList m_NormalVarList;
	CFDLLVariableList m_DLLVariableList;

	CParameterList m_ParameterLst;//模块节点参数列表
	CString m_strFile;//调用模块文件的绝对路径以及文件名

	CNewDBConnVar *m_pCurDBConn;
	CNewRecordsetVar *m_pCurRecset;
	CStoreProObj	*m_pCurStoreProObj;
};


class  CNewBlockObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CNewBlockObj);
	CNewBlockObj();
public:

	virtual ~CNewBlockObj();
	CNewBlockObj( const CNewBlockObj &s );
	CNewBlockObj( const CBlockObj &s );
	CNewBlockObj & operator =( const CNewBlockObj &s );
	CNodeObj * CNewBlockObj::FindNodeObj(int nKey, int &nParentKey);
	CNodeObj * FindObj( int nKey );
	virtual void Serialize(CArchive& ar);
public:
	CNodeObjList m_ChildObjLst;
};


class CDBParameters  
{
public:
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
	_CommandPtr m_pCommand;
	CString strSQL;
	CString strError;
public:
	CDBParameters();
	virtual ~CDBParameters();
	CDBParameters( const CDBParameters &s );
	CDBParameters & operator =( const CDBParameters &s );

};


class CNewSQLExcuteObj : public CSQLExcuteObj  
{
protected:
	DECLARE_SERIAL(CNewSQLExcuteObj);
	CNewSQLExcuteObj();
public:
	virtual ~CNewSQLExcuteObj();
	CNewSQLExcuteObj( CPoint point );
	CNewSQLExcuteObj( const CSQLExcuteObj &s );
	CNewSQLExcuteObj( const CNewSQLExcuteObj &s );
	CNewSQLExcuteObj & operator =( const CNewSQLExcuteObj &s );
public:
	CNewDBConnVar *m_pDB;
	CNewRecordsetVar *m_pRS;
	CString m_str;
	CString m_strError;
	
public:
	virtual void Serialize(CArchive& ar);
};


class CNewStoreProObj : public CStoreProObj  
{
protected:
	DECLARE_SERIAL(CNewStoreProObj);
	CNewStoreProObj();
public:
	
	virtual ~CNewStoreProObj();
	CNewStoreProObj( CPoint point );
	CNewStoreProObj( const CStoreProObj &s );
	CNewStoreProObj( const CNewStoreProObj &s );
	CNewStoreProObj & operator =( const CNewStoreProObj &s );
public:
	CNewDBConnVar *m_pDB;
	CNewRecordsetVar *m_pRS;
	CString m_str;
	CString m_strError;
	CFChannelInfo *m_pChnlInfo;
public:
	virtual void Serialize(CArchive& ar);
};


class CNewEndCallBlockObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CNewEndCallBlockObj);
	CNewEndCallBlockObj();
public:
	CNewEndCallBlockObj( const CNewEndCallBlockObj &s );
	CNewEndCallBlockObj( const CEndCallBlockObj &s );
	CNewEndCallBlockObj & operator =( const CNewEndCallBlockObj &s );
	virtual ~CNewEndCallBlockObj();
public:
	CNodeObjList m_ChildObjLst;
public:
	CNodeObj * FindNodeObj(int nKey, int &nParentKey);
	CNodeObj * GetEndCallEventCheckObj( void );
	CNodeObj * FindObj( int nKey );
	virtual void Serialize(CArchive& ar);
};

#endif 
