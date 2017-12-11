// NodeObj.h: interface for the CNodeObj class.
//
//////////////////////////////////////////////////////////////////////


#ifndef _NODEOBJ_H_
#define _NODEOBJ_H_

#include <time.h> 
#include <afxtempl.h>

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Exported DLL initialization is run in context of application   
// or Regular DLL.   
//extern "C" _declspec(dllexport)  void WINAPI Init_ChiefScriptNodeDLL() ;
//DLL调用的函数类型定义
typedef void(*DLLCALL_FUNCTION)(int *nParam1, char szParam2[500], char szParam3[500]  );

//通道状态
#define CHANNEL_STATE_FREE  20 //通道处于非摘机，通话，振铃状态时，通道将处于该状态
#define CHANNEL_STATE_CONNECT  21 //通道处于使用状态
#define CHANNEL_STATE_HOLD  22 //通道处于Hold状态
#define CHANNEL_STATE_DIAL  23 //通道处于Dial状态
#define CHANNEL_STATE_RINGING     24 //通道处于振铃状态
#define CHANNEL_STATE_MONITORING  25 //通道处于监听状态
#define CHANNEL_STATE_TRANSFER  26 //通道处于呼叫转移状态
#define CHANNEL_STATE_CLOSE  27 //通道处于关闭状态
#define CHANNEL_STATE_WAITQUENE  28 //等待队列状态
#define CHANNEL_STATE_WAITFREE  29 //等待释放状态态
#define CHANNEL_STATE_OFFHOOK  30 //摘机状态
#define CHANNEL_STATE_AFTERWORK  31 //话后状态

//命令类型
#define CF_CMD_NO		1;//没有命令
#define CF_CMD_PLAYFILE 2;//播放文件
#define CF_CMD_RECDTMF  3;//接收DTMF

//呼叫类型
#define CHANNEL_CALLTYPE_NOCALL		29//没有活动呼叫
#define CHANNEL_CALLTYPE_OUT		30//模拟卡系统涉及到外线的呼叫
#define CHANNEL_CALLTYPE_IN			31//模拟卡系统分机间的呼叫
#define CHANNEL_CALLTYPE_COMEIN		32 //公网电话呼入 
#define CHANNEL_CALLTYPE_CALLOUT	33 //呼叫公网电话


//自定义的通道类型
#define CF_CHTYPE_NOKNOW    -1 //未知通道类型 
#define CF_CHTYPE_EMPTY     0  //空通道 
#define CF_CHTYPE_DTNO1     1  //数字中继中国一号信令 
#define CF_CHTYPE_DTNO7     2  //数字中继中国七号信令(TUP)
#define CF_CHTYPE_DTDSS1    3  //数字中继ISDN PRI(30B+D)
#define CF_CHTYPE_DTNO7ISUP 4  //数字中继中国七号信令(ISUP)
#define CF_CHTYPE_MTP3		5  //七号信令MTP传输层通道
#define CF_CHTYPE_TRUNK     6  //模拟中继外线(普通市话线),接电信局
#define CF_CHTYPE_USER		7  //模拟中继内线(座席线)，接电话机
#define CF_CHTYPE_RECORD    8  //模拟中继高阻线(监控线)，与电话线/电话机相并联
#define CF_CHTYPE_VIP	    9  //VOIP通道类型



//通道交换类型
#define CF_CHSWITCH_CONNECT			0//直接连接
#define CF_CHSWITCH_MONITOR			1//监听
#define CF_CHSWITCH_SPEAK			2//别人能听见，但听不见别人
#define CF_CHSWITCH_DISCONNECT		3//断开连接
#define CF_CHSWITCH_CALLAGENT		4//呼叫坐席，这时的通道号为分机号码
#define CF_CHSWITCH_CALLCUSTOMER	5//呼叫客呼

//结点保存数据的基本数据类型常量定义
#define DATATYPE_INTEGER		0
#define DATATYPE_FLOAT			1
#define DATATYPE_STRING			2
#define DATATYPE_DATE			3
#define DATATYPE_TIME			4
#define DATATYPE_DATATIME		5
#define DATATYPE_DBCONNECT      6//数据库连接变量类型
#define DATATYPE_DBRECORDSET    7//数据库记录变量类型
#define DATATYPE_DLL		    8//DLL变量类型
#define DATATYPE_TCP_SOCKET		9//TCP_SOCKET变量类型
#define DATATYPE_UDP_SOCKET		10//UDP_SOCKET变量类型

//结点类型常量定义
#define NODETYPE_START			0
#define NODETYPE_WAITCALL		1
#define NODETYPE_START_PLAY     2
#define NODETYPE_CHECK_DIAL		3
#define NODETYPE_RELEASECALL	4
#define NODETYPE_CALLHOLD		5
#define NODETYPE_CHECK_TRANSFER	6
#define NODETYPE_START_RECORD	10
#define NODETYPE_SETVARIABLE	12
#define NODETYPE_COMPARE		13
#define NODETYPE_ENTER_CONFERENCE	14
#define NODETYPE_LEAVE_CONFERENCE	15
#define NODETYPE_OPEN_DATABASE		16
#define NODETYPE_CLOSE_DATABASE		17
#define NODETYPE_EXCUTE_SQL			18
#define NODETYPE_DLL_LOAD		19
#define NODETYPE_DLL_UNLOAD		20
#define NODETYPE_DLL_CALL		21
#define NODETYPE_SEND_DTMFCODE	22
#define NODETYPE_RECIVE_DTMF	23
#define NODETYPE_EVENT_CHECK	24
#define NODETYPE_DATABASE_BROWSE 25
#define NODETYPE_DTMF_MENU       26
#define NODETYPE_CALLLOGREQUEST  27
#define NODETYPE_NODE_END        28
#define NODETYPE_SWITCH          29
#define NODETYPE_ANSWERCALL      30
#define NODETYPE_FAXRECIVE       31
#define NODETYPE_FAXSEND         32
#define NODETYPE_CONFERENCE_BUILD      33
#define NODETYPE_CONFERENCE_DELETE     34
#define NODETYPE_GET_CONFERENCE_MEMBER 35
#define NODETYPE_CONFERENCE_REQUEST    36

#define NODETYPE_BLOCK     37
#define NODETYPE_ENTER	   38
#define NODETYPE_RETURN      39


#define NODETYPE_STOREPRO			40//存储过程
#define NODETYPE_CHANGEVOICE		41//变声

#define NODETYPE_ENDCALL_BLOCK			42//呼叫结束处理流程
#define NODETYPE_ENDCALLEVENT_CHECK		43//呼叫结束事件检测
#define NODETYPE_ENDCALLEVENT_END		44//呼叫结束事件处理结束

#define NODETYPE_CONFER_PLAY        45//会议放音
#define NODETYPE_CONFER_RECORD	    46//会议录音
#define NODETYPE_EXPRESS			47//表达式
#define NODETYPE_FUNCTION           48//函数
#define NODETYPE_GOTO               49//函数
#define NODETYPE_ROUTE_CALL         50//呼叫路由
#define NODETYPE_TRACE_INFO         51//信息跟踪
#define NODETYPE_MOUDLE             52//调用模块文件接点
#define NODETYPE_MOUDLE_BEGIN       53//模块开始
#define NODETYPE_MOUDLE_END         54//模块结束
#define NODETYPE_CUSTOM             55//定制节点
#define NODETYPE_CC_CALLAGENT       56//定制节点
#define NODETYPE_CC_TRANSFER     57//定制节点

//结点分支类型常量定义
#define NEXTTYPE_NEXT			1
#define NEXTTYPE_ERROR			2
#define NEXTTYPE_BUSY			3
#define NEXTTYPE_NO_DIALTONE	4
#define NEXTTYPE_NO_ANSWER 		5
#define NEXTTYPE_NO_RINGBACK	6
#define NEXTTYPE_CONNECT		7
#define NEXTTYPE_NOMATCH		8//没有匹配的比较，用于Compare节点	
#define NEXTTYPE_DTMF_0			60
#define NEXTTYPE_DTMF_1			61
#define NEXTTYPE_DTMF_2			62
#define NEXTTYPE_DTMF_3			63
#define NEXTTYPE_DTMF_4			64
#define NEXTTYPE_DTMF_5			65
#define NEXTTYPE_DTMF_6			66
#define NEXTTYPE_DTMF_7			67
#define NEXTTYPE_DTMF_8			68
#define NEXTTYPE_DTMF_9			69
#define NEXTTYPE_DTMF_10		70 //DTMF #
#define NEXTTYPE_DTMF_11		71//DTMF *
#define NEXTTYPE_DTMF_TIMEOUT	72
#define NEXTTYPE_OFFHOOK		20//摘机
#define NEXTTYPE_HANGUP			21//挂机
#define NEXTTYPE_HOOKFLASH		22//拍叉簧
#define NEXTTYPE_RECDTMF		23//收到按键
#define NEXTTYPE_RECFAXTONE		24//收到对方等待接收传真信号
#define NEXTTYPE_AGENT_READY	25//Agent Ready
#define NEXTTYPE_AGENT_NOTREADY	26//Agent NotReady
#define NEXTTYPE_CONFDESTROYED	27//（该通道所在的会议资源已释放）
#define NEXTTYPE_AGENT_LOGIN_MSG	28
#define NEXTTYPE_AGENT_OUT_MSG		29
#define NEXTTYPE_AGENT_BUSY_MSG		30
#define NEXTTYPE_AGENT_INSERVE_MSG	31
#define NEXTTYPE_AGENT_ANSWER_MSG	32
#define NEXTTYPE_AGENT_MAKECALL_MSG 33
#define NEXTTYPE_AGENT_TRANSFER_MSG 34
#define NEXTTYPE_AGENT_CONFER_MSG	35
#define NEXTTYPE_AGENT_HANGUP_MSG	36
#define NEXTTYPE_AGENT_SENDDATA_MSG 37 //坐席间相互发送消息时用
#define NEXTTYPE_AGENT_SENDFAX_MSG	38
#define NEXTTYPE_RINGING			39//外线振铃
#define NEXTTYPE_NOTHING			40//没有事件发生
#define NEXTTYPE_EOF_FALSE			41//数据库查询的记录为空
#define NEXTTYPE_AGENT_HOLD_MSG		42//
#define NEXTTYPE_AGENT_PICKUPHOLD_MSG	43//
#define NEXTTYPE_AGENT_MONITOR_MSG		44//
#define NEXTTYPE_AGENT_DISMONITOR_MSG	45//
#define NEXTTYPE_AGENT_BUILDCONFER_MSG	46//创建会议
#define NEXTTYPE_ELAPSE_TIME			47//从开始计时到规定时间将发生该事件
#define NEXTTYPE_CANCEL_TRANSFER		48//取消呼叫转移
#define NEXTTYPE_AGENT_GETACTIVECALL	49//取得活动呼叫

//事件类型常量,只有读取该变量的地方才能将通道的事件类型设值为0

#define EVENTTYPE_OFFHOOK				1
#define EVENTTYPE_HANGUP				2
#define EVENTTYPE_HOOKFLASH 			3
#define EVENTTYPE_RECDTMF  				4
#define EVENTTYPE_RECFAXTONE  			5
#define EVENTTYPE_AGENT_READY 		    6
#define EVENTTYPE_AGENT_NOTREADY  	    7
#define EVENTTYPE_CONFDESTROYED 		8
#define EVENTTYPE_AGENT_LOGIN_MSG		9
#define EVENTTYPE_AGENT_OUT_MSG			10
#define EVENTTYPE_AGENT_BUSY_MSG 		11
#define EVENTTYPE_AGENT_INSERVE_MSG		12
#define EVENTTYPE_AGENT_ANSWER_MSG		13
#define EVENTTYPE_AGENT_MAKECALL_MSG	14
#define EVENTTYPE_AGENT_TRANSFER_MSG	15
#define EVENTTYPE_AGENT_CONFER_MSG		16
#define EVENTTYPE_AGENT_HANGUP_MSG		17
#define EVENTTYPE_AGENT_SENDDATA_MSG	18
#define EVENTTYPE_AGENT_SENDFAX_MSG		19
#define EVENTTYPE_RINGING		        20
#define EVENTTYPE_NOTHING				21//没有事件发生
#define EVENTTYPE_AGENT_HOLD_MSG		22
#define EVENTTYPE_AGENT_PICKUPHOLD_MSG	23
#define EVENTTYPE_AGENT_MONITOR_MSG		24
#define EVENTTYPE_AGENT_DISMONITOR_MSG	25
#define EVENTTYPE_AGENT_BUILDCONFER_MSG	26
#define EVENTTYPE_ELAPSE_TIME			27
#define EVENTTYPE_AGENT_CANCEL_TRANSFER	28//取消呼叫转移
#define EVENTTYPE_AGENT_GETACTIVECALL	29

//Action 类型
#define ACTION_EXPRESS			0 //"Express( <SaveTo> ; <  > )");
#define ACTION_LEFT				1// "Left( <SaveTo> ; <string> ; <length> )");
#define ACTION_MID				2// "Mid( <SaveTo> ; <string> ; <start> ; <length> )");
#define ACTION_LEN				3// "Len( <SaveTo> ; <string> )");
#define ACTION_RIGHT			4// "Right( <SaveTo> ; <string> ; <length> )");
#define ACTION_GETHOLDCHNL		5// "GetHoldChnl( <SaveTo> )");
#define ACTION_AGENTISFREE		6// "AgentIsFree( <SaveTo> ; <AgentDN> )");
#define ACTION_GETLINKCHNLVAR	7// "GetLinkChnlVar( <SaveTo> ; <Variable> )");
#define ACTION_GETLINKCHNLTYPE	8//"GetLinkChnlType( <SaveTo> )");
#define ACTION_GETFREECHNL		9// "GetFreeChnl( <SaveTo> ; <ChnlType> ; <Begin> ; <End> )");
#define ACTION_GETCHNLINDEX		10// "GetChnlIndex( <SaveTo> ; <ChnlType> ; <LocalDN> )");

class AFX_EXT_CLASS CDBConnVar : public CObject  
{
protected:
	DECLARE_SERIAL(CDBConnVar);
public:
	CDBConnVar();
	CDBConnVar( const CDBConnVar &s );
	CDBConnVar & operator =( const CDBConnVar &s );
	virtual ~CDBConnVar();
public:
	CString m_strName;
	CString m_strConn;
	int m_nVarDataType;
	int m_nKey;

public:
	virtual void Serialize(CArchive &ar);
};

typedef AFX_EXT_CLASS CTypedPtrList<CObList, CDBConnVar*> CDBConnVarList;

/////////////////////////////////////////////////////////////////////////////
// CDLLVariable document
//////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CDLLVariable : public CObject  
{
protected:
	DECLARE_SERIAL(CDLLVariable);
public:
	HINSTANCE m_hDLL;
	CString m_strName;
	int m_nVarDataType;
	int m_nKey;
	CString m_strDLLFileName;
	virtual void Serialize(CArchive &ar);
	CDLLVariable();
	CDLLVariable( const CDLLVariable &s );
	CDLLVariable & operator =( const CDLLVariable &s );

	virtual ~CDLLVariable();

};
typedef AFX_EXT_CLASS CTypedPtrList<CObList, CDLLVariable*> CFDLLVariableList;


////////////////////////////////////////////////////////////////
//CNormalVar
///////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CNormalVar : public CObject  
{
protected:
	DECLARE_SERIAL(CNormalVar);
public:
	CNormalVar();
	CNormalVar( const CNormalVar &s );
	CNormalVar & operator =( const CNormalVar &s );
	virtual ~CNormalVar();
	inline int	GetDataType( CString strDataType ) ;
	inline CString	GetDataTypeName( int nType ) ;
public:
	CString m_strName;
	int m_nVarDataType;
	int m_nKey;
	CString m_strVariable;
	CString m_strDescription;
public:
	virtual void Serialize(CArchive &ar);
};
typedef AFX_EXT_CLASS CTypedPtrList<CObList, CNormalVar*> CNormalVarList;


////////////////////////////////////////////////////////////////
//CParameter
///////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CParameter : public CObject  
{
protected:
	DECLARE_SERIAL(CParameter);
public:
	CParameter();
	CParameter( const CParameter &s );
	CParameter & operator =( const CParameter &s );
	virtual ~CParameter();
public:
	int     m_nKey;//参数的Key
	CString m_strName;//参数名
	int		m_nVarKey;//参数对应变量的Key
	CString m_strVariable;//参数对应输入输出的变量
	CString m_strDescription;//参数描述
	CString m_strValues;//参数值
public:
	virtual void Serialize(CArchive &ar);
};
typedef AFX_EXT_CLASS CTypedPtrList<CObList, CParameter*> CParameterList;

////////////////////////////////////////////////////////////////
//CRecordsetVar
///////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CRecordsetVar : public CObject  
{
protected:
	DECLARE_SERIAL(CRecordsetVar);
public:
	CRecordsetVar();
	CRecordsetVar( const CRecordsetVar &s );
	CRecordsetVar & operator =( const CRecordsetVar &s );
	virtual ~CRecordsetVar();
public:
	CString m_strName;
	int m_nVarDataType;
	int m_nKey;

	
public:
	int m_nSQLType;
	CString m_strDataBase;
	CString m_strSQL;

	virtual void Serialize(CArchive &ar);
};
typedef AFX_EXT_CLASS CTypedPtrList<CObList, CRecordsetVar*> CRecordsetVarList;
////////////////////////////////////////////////////////////////
//CSystemVar
///////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CSystemVar : public CObject  
{
protected:
	DECLARE_SERIAL(CSystemVar);
public:
	CSystemVar();
	CSystemVar( const CSystemVar &s );
	CSystemVar & operator =( const CSystemVar &s );
	virtual ~CSystemVar();
public:
	CString m_strName;
	int m_nVarDataType;
	int m_nKey;
public:
	virtual void Serialize(CArchive &ar);
};
typedef AFX_EXT_CLASS CTypedPtrList<CObList, CSystemVar*> SystemVariableList;


//////////////////////////////////////////////////////////////////////////
//CLineObj
///////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS  CLineObj : public CObject  
{
protected:
	DECLARE_SERIAL(CLineObj);
public:
	CLineObj();
	CLineObj( const CLineObj &s );
	CLineObj & operator =( const CLineObj &s );
	virtual ~CLineObj();

public:

	inline void OnDrawRubberLine( CWnd* pWnd , CPoint point );
	void OnDraw(CDC *pDC);
	virtual void Serialize(CArchive& ar);

public:
	CRect m_BeginRect;
	CRect m_EndRect;
	CPoint m_OldPoint;
	int m_nOffset;//偏移量，画圆弧


	CPoint m_BeginPoint;
	CString m_strCaption;
	COLORREF m_crLineColor;
	COLORREF m_crOldLineColor;
};

//////////////////////////////////////////////////////////////////////////
//CNextNode
///////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CNextNode : public CObject  
{
protected:
	DECLARE_SERIAL(CNextNode);
public:
	inline BOOL Intersects(const CRect &rect);//矩形内判断
	inline void SetNodeInfo(CPoint point, LPCTSTR lpszName, LPCTSTR lpszData, int nDataType, int nType ,int nIndex );
	inline void MoveTo( CPoint point , CRect BeginRect );
	inline void OnDraw(CDC *pDC  );
	inline void SetNodeObj( LPCTSTR lpszName, int nType );
	CNextNode();
	CNextNode( const CNextNode &s );
	CNextNode & operator =( const CNextNode &s );

	virtual ~CNextNode();
public:
	inline void DisConnectToNodeObj( void );
	inline void ConnectToNodeObj(const char *pszNodeObjName, int nNodeObjType, int nNodeObjKey, CRect endRect );
	inline void OnDrawRubberLine( CWnd* pWnd , CPoint point );
	inline CPoint GetLinePoint( void );
	inline BOOL HitTest(CPoint point);
	long GetHigh( void );
	virtual void Serialize(CArchive& ar);
	inline void UpdateNodeInfo(CPoint point, int nIndex );
public:
	
	CString m_strData;//保存自己数据
	int  m_nDataType;//自己数据类型，
	CString m_strName;//名称
	CRect m_NodeObjRect;
	CString m_strNodeObjName;//与之相关的结点对象名称，如果为""表示没有相关对象
	int  m_nNodeObjType;//与之相关的结点对象类型，如果等于0表示没有相关对象
	CLineObj m_LineObj;
	int m_nNodeObjKey;//连向
	int m_nType;//0:Next; 1:Error; 其他特定类型必须大于10
	CRect m_rect;

private:
	int m_nIndex;
	long m_width;
	long m_high;
};

typedef AFX_EXT_CLASS CTypedPtrList<CObList, CNextNode*> CNextNodeList;

//////////////////////////////////////////////////////////////////////////
//CNodeData
///////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CNodeData  : public CObject  
{
protected:
	DECLARE_SERIAL(CNodeData);
public:
	CNodeData();
	CNodeData( const CNodeData &s );
	CNodeData & operator =( const CNodeData &s );
	virtual ~CNodeData();
public:
	CString m_strData;//数据结点保存数据
	int  m_nDataType;//数据结点数据类型
	CString m_strName;//数据结点名称
	/*************************************************
	//当存储变量时，作为变量的标志，初始化值为0；
	

	**************************************************/
	int  m_nKey;
public:
	virtual void Serialize(CArchive &ar);

};
typedef AFX_EXT_CLASS CTypedPtrList<CObList, CNodeData*> CNodeDataList;

/////////////////////////////////////////////////////////////////////
//CNodeObj
//
////////////////////////////////////////////////////////////////////


class AFX_EXT_CLASS CNodeObj : public CObject  
{
protected:
	DECLARE_SERIAL(CNodeObj);
	CNodeObj();
public:
	int m_nClassType;//0:根，1，2，3依次类推
	int m_nBlockKey;
	//如果属于Script，那么m_nBlockKey = 0; 
	//如果属于哪个Child Block，那么m_nBlockKey就等于该Child Block的key
public:
	
	CNodeObj(CPoint point);
	CNodeObj( const CNodeObj &s );
	CNodeObj & operator =( const CNodeObj &s );

	virtual ~CNodeObj();
public:

	CNextNodeList m_NextNodeList;//保存相关的下一个Nodeobj的信息
	CNodeDataList m_NodeDataList;//保存相关的NodeData的信息
	CRect	m_rect;//结点对象的矩形
	CString	m_strCaption;//结点对象的标题

	int		m_nType;//接点对象的类型
	/*************************************************************/
	//在系统创建脚本时，每个脚本文件都从在添加第一个对象时开始记数，
	//该基数为100，每增加一个对象，该基数加一得到新的基数，
	//m_nKey = 该新的基数，用以标识该对象；所以在整个脚本文件里，所
	//有对象的该值是唯一的，同时把最后的基数存储到脚本文件中，当文件
	//从新打开编辑时，以该基数为基数递增，以此作为新增对象的m_nKey值。
	//
	int     m_nKey;
	/*************************************************************/

	UINT    m_nIcoID; //图标ID
	HICON	m_hIcon;//结点对象的图标

public:
	void RemoveAllNextNode( void );
	CDBConnVar * FindDBConnectVariable( CDBConnVarList* pDBLst, int nKey);
	CDLLVariable * FindDLLVariable(CFDLLVariableList *pDLLlst, int nKey);
	CRecordsetVar * FindRecordsetVar(CRecordsetVarList *pRSlst, int nKey);
	void ChangeLineColor( CPoint point );
	inline int GetVarKey( CString strVar );
	CString m_strControlMsg;
	CString m_strDescription;
	BOOL CheckConnectToThis( int nThisKey );
	void DisConnectToNode( int nKey );
	inline CString GetNodeObjTypeName();
	CNextNode * FindNextNodeObj( CString strName );
	void UpDate( void );
	CNextNode * FindNextNodeObj( int nType );
	void RemoveNodeData( void);
	void  MoveLineObj( int nNodeObjKey ,CRect EndRect  );
	inline CPoint GetLinePoint();
	void SetSelected( BOOL bSelected );

	CRect m_MoveRect;//用于保存拖动时的逻辑位置矩形
	CNextNode * HitTestNextNode(CPoint point);
	long GetWidth( void );
	long GetHigh(void);
	inline BOOL HitTest( CPoint point );
	virtual void Serialize(CArchive& ar);
	void Remove(void);
	void RemoveNextNodeObj( CNextNode *pObj );
	BOOL AddNextNodeObj( LPCTSTR lpszName, LPCTSTR lpszData, int nDataType, int nType, int nIndex );
	inline void SetBaseInfo(  LPCTSTR lpszCaption, HICON hIcon , int nType ,int nKey );
	inline BOOL Intersects(const CRect& rect);
	void MoveTo( CPoint point);
	void OnDraw(CDC* pDC);
	inline int	GetDataType( CString strDataType ) ;
	inline CString	GetDataTypeName( int nType ) ;
private:
	long	m_width;//结点对象的宽度
	long	m_high;//结点对象的高度
	BOOL	m_bSelected;////结点对象是否被选中

private:

	void OnDrawCaption(CDC *pDC );
	void OnDrawIco(CDC *pDC, HICON hIcon );
	void OnDrawBaseRect(CDC *pDC);
	
};

typedef AFX_EXT_CLASS CTypedPtrList<CObList, CNodeObj*> CNodeObjList;


/////////////////////////////////////////////////////////////////////////////
// CCFDocument document
//////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CCFDocument : public CDocument
{

	DECLARE_DYNCREATE(CCFDocument)
public:
	CCFDocument();           // protected constructor used by dynamic creation


// Attributes
public:
	/*************************************************************/
	//在系统创建脚本时，每个脚本文件都从在添加第一个NodeObj对象时开始记数，
	//该基数为100，每增加一个对象，该基数加一得到新的基数，
	//m_nKey = 该新的基数，用以标识该对象；所以在整个脚本文件里，所
	//有对象的该值是唯一的，同时把最后的基数存储到脚本文件中，当文件
	//从新打开编辑时，以该基数为基数递增，以此作为新增对象的m_nBaseKey值。
	//
	int     m_nBaseKey;
	/*************************************************************/
	int     m_nVarBaseKey;
	CNodeObjList m_objects;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCFDocument)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	public:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	int m_nDocType ;//0 cef文件 1 cmf文件
	CParameterList m_ParameterLst;//cmf文件参数列表
	CRecordsetVarList m_RecordsetVariList;
	CDBConnVarList m_DBConnVarList;
	CNormalVarList m_NormalVarList;
	CFDLLVariableList m_DLLVariableList;
	virtual ~CCFDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCFDocument)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};





class AFX_EXT_CLASS CWaitCallObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CWaitCallObj);
	CWaitCallObj();
public:
	CWaitCallObj( CPoint point );
	CWaitCallObj( const CWaitCallObj &s );
	CWaitCallObj & operator =( const CWaitCallObj &s );
	virtual ~CWaitCallObj();
public:
	CNodeData m_CalledID;
	BOOL m_bRecCalledID;
	CNodeData m_CallerID;
	BOOL m_bRecCallerID;
	BOOL m_bClearDTMF;
	int m_nRings;
	BOOL m_bAnswerCall;	
	
public:
	virtual void Serialize(CArchive& ar);
};



class AFX_EXT_CLASS CPlayStartObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CPlayStartObj);
	CPlayStartObj();
public:
	CPlayStartObj( CPoint point );
	CPlayStartObj( const CPlayStartObj &s );
	CPlayStartObj & operator =( const CPlayStartObj &s );
	virtual ~CPlayStartObj();
public:
	CString m_strSaveDTMF;
	BOOL m_bRecDTMF;
	BOOL m_bClearDTMF;
	BOOL m_bPlayStop;
	CNodeDataList m_PlayList;
	
	CString	m_strConfNO;
	int		m_nPlayWay;

public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CDialCheckObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CDialCheckObj);
	CDialCheckObj();
public:
	CDialCheckObj( CPoint point );
	CDialCheckObj( const CDialCheckObj &s );
	CDialCheckObj & operator =( const CDialCheckObj &s );
	virtual ~CDialCheckObj();
public:
	BOOL	m_bAfterRings;
	BOOL	m_bNotAnswer;
	int		m_nRings;
	int		m_nTimes;
	CString	m_strPhone;
	int		m_nChnlType;
	CString	m_strPrefix;
public:
	BOOL m_bConnect;
	BOOL m_bBusy;
	BOOL m_bNoDialTone;
	BOOL m_bNoAnswer;
	BOOL m_bNoRingBack;
	BOOL m_bError;
public:
	virtual void Serialize(CArchive& ar);
};


class AFX_EXT_CLASS CReleaseCallObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CReleaseCallObj);
	CReleaseCallObj();
public:
	CReleaseCallObj( CPoint point );
	CReleaseCallObj( const CReleaseCallObj &s );
	CReleaseCallObj & operator =( const CReleaseCallObj &s );
	virtual ~CReleaseCallObj();
public:
	BOOL	m_bSaveDetail;

public:
	virtual void Serialize(CArchive& ar);
};


class AFX_EXT_CLASS CCallHoldObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CCallHoldObj);
	CCallHoldObj();
public:
	CCallHoldObj( CPoint point );
	CCallHoldObj( const CCallHoldObj &s );
	CCallHoldObj & operator =( const CCallHoldObj &s );
	virtual ~CCallHoldObj();
public:
	BOOL m_bPlayVox;//是否对通话的另一方播放音乐
	CString m_strPlayFile;
	int m_nCallHold;//hold 类型：0:call hold 1:pickup from hold

public:
	virtual void Serialize(CArchive& ar);
};



class AFX_EXT_CLASS CTransferCheckObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CTransferCheckObj);
	CTransferCheckObj();
public:
	CTransferCheckObj( CPoint point );
	CTransferCheckObj( const CTransferCheckObj &s );
	CTransferCheckObj & operator =( const CTransferCheckObj &s );
	virtual ~CTransferCheckObj();
public:
	BOOL	m_bClearDTMF;
	BOOL	m_bAfterRings;
	BOOL	m_bNotAnswer;
	int		m_nRings;
	int		m_nTimes;
	int     m_nWaitOption;
	CString m_strPlayFile;
	int		m_nDialType;
	CString	m_strExpression;
	int		m_nTransferAim;//0 转移到坐席,1 转移到外线，2 转移到传真
	int     m_nTransferType;//0 开始转移,1 取消转移，2 完成转移
	CString	m_strCancelDTMF;
	BOOL	m_bTalkWitDN;
	CString	m_strCompleteDTMF;
public:
	virtual void Serialize(CArchive& ar);
};



class AFX_EXT_CLASS CCallLogRequestObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CCallLogRequestObj);
	CCallLogRequestObj();
public:
	CCallLogRequestObj( CPoint point );
	CCallLogRequestObj( const CCallLogRequestObj &s );
	CCallLogRequestObj & operator =( const CCallLogRequestObj &s );
	virtual ~CCallLogRequestObj();
public:
	int	m_nRequestType;

public:
	virtual void Serialize(CArchive& ar);
};


class AFX_EXT_CLASS CDLLLoadObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CDLLLoadObj);
	CDLLLoadObj();
public:
	CDLLLoadObj( CPoint point );
	CDLLLoadObj( const CDLLLoadObj &s );
	CDLLLoadObj & operator =( const CDLLLoadObj &s );
	virtual ~CDLLLoadObj();
public:
	int m_nDLLKey;
	int m_nHandleType;
	CString m_strDLLFile;
	
public:
	virtual void Serialize(CArchive& ar);
};


class AFX_EXT_CLASS CDLLCallObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CDLLCallObj);
	CDLLCallObj();
public:
	CDLLCallObj( CPoint point );
	CDLLCallObj( const CDLLCallObj &s );
	CDLLCallObj & operator =( const CDLLCallObj &s );
	virtual ~CDLLCallObj();
public:
	int m_nDLLKey;
	int m_nHandleType;
	CString m_strDLL;
	CString m_strFunction;	
	
public:
	virtual void Serialize(CArchive& ar);
};


class AFX_EXT_CLASS CDLLUnLoadObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CDLLUnLoadObj);
	CDLLUnLoadObj();
public:
	CDLLUnLoadObj( CPoint point );
	CDLLUnLoadObj( const CDLLUnLoadObj &s );
	CDLLUnLoadObj & operator =( const CDLLUnLoadObj &s );
	virtual ~CDLLUnLoadObj();
public:
	int m_nDLLKey;
	int m_nHandleType;
	CString m_strDLL;
	
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CDBOpenObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CDBOpenObj);
	CDBOpenObj();
public:
	CDBOpenObj( CPoint point );
	CDBOpenObj( const CDBOpenObj &s );
	CDBOpenObj & operator =( const CDBOpenObj &s );
	virtual ~CDBOpenObj();
public:
	int m_nDBKey;
	int m_nDBType;
	CString m_strConn;
	CString m_strDBName;
	
public:
	virtual void Serialize(CArchive& ar);
};


class AFX_EXT_CLASS CSQLExcuteObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CSQLExcuteObj);
	CSQLExcuteObj();
public:
	CSQLExcuteObj( CPoint point );
	CSQLExcuteObj( const CSQLExcuteObj &s );
	CSQLExcuteObj & operator =( const CSQLExcuteObj &s );
	virtual ~CSQLExcuteObj();
public:
	int m_nRecordsetKey;
	CString m_strRecordset;
	int m_nSQLType;
	CString m_strSQL;
	
	int m_nDBKey;
	int m_nDBType;
	CString m_strDB;
	
	
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CDBBrowseObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CDBBrowseObj);
	CDBBrowseObj();
public:
	CDBBrowseObj( CPoint point );
	CDBBrowseObj( const CDBBrowseObj &s );
	CDBBrowseObj & operator =( const CDBBrowseObj &s );
	virtual ~CDBBrowseObj();
public:
	int m_nRecordsetKey;
	CString m_strRecordset;
	int m_nBrowseType;

	
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CDBCloseObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CDBCloseObj);
	CDBCloseObj();
public:
	CDBCloseObj( CPoint point );
	CDBCloseObj( const CDBCloseObj &s );
	CDBCloseObj & operator =( const CDBCloseObj &s );
	virtual ~CDBCloseObj();
public:
	int m_nDBKey;
	int m_nDBType;
	CString m_strDB;
	
public:
	virtual void Serialize(CArchive& ar);
};


class AFX_EXT_CLASS CDTMFReciveObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CDTMFReciveObj);
	CDTMFReciveObj();
public:
	CDTMFReciveObj( CPoint point );
	CDTMFReciveObj( const CDTMFReciveObj &s );
	CDTMFReciveObj & operator =( const CDTMFReciveObj &s );
	virtual ~CDTMFReciveObj();
public:
	CNodeData m_SaveDTMF;
	BOOL m_bClearDTMF;
	int m_nEndCondition;
	int m_nDTMFCount;
	CString	m_strDTMF;
	int m_nTimeOut;//超时，单位秒
	int m_nDTMFType;//接收DTMF的类型 0接收所有DTMF，1只接收数字键
public:
	virtual void Serialize(CArchive& ar);
};


class AFX_EXT_CLASS CEventCheckObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CEventCheckObj);
	CEventCheckObj();
public:
	CEventCheckObj( CPoint point );
	CEventCheckObj( const CEventCheckObj &s );
	CEventCheckObj & operator =( const CEventCheckObj &s );
	virtual ~CEventCheckObj();
public:
	BOOL m_bError;
	BOOL m_bOffHook;
	BOOL m_bHangUp;
	BOOL m_bHookFlash;
	BOOL m_bRecDTMF;
	BOOL m_bRecFaxTone;
	BOOL m_bAppExit;
	BOOL m_bFaxSendTone;
	BOOL m_bConfDestroyed;
	BOOL m_bAgentLoginMsg;
	BOOL m_bAgentLogOutMsg;
	BOOL m_bAgentBusyMsg;
	BOOL m_bAgentInserveMsg;
	BOOL m_bAgentAnswerMsg;
	BOOL m_bAgentMakeCallMsg;
	BOOL m_bAgentTransferMsg;
	BOOL m_bAgentConferMsg;
	BOOL m_bAgentHangUpMsg;
	BOOL m_bAgentSendDataMsg;
	BOOL m_bAgentSendFaxMsg;
	BOOL m_bRinging;
	BOOL m_bNothing;
	BOOL m_bAgentHold;
	BOOL m_bAgentPickupHold;
	BOOL m_bAgentMonitor;
	BOOL m_bAgentDisMonitor;
	BOOL m_bAgentBuildConfer;
	BOOL m_bElapseTime;
	BOOL m_bAgentGetActiveCall;
public:
	int  m_nElapseTime;
	BOOL m_bClearDTMF;
	BOOL m_bPlay;
	CString m_strPlayFile;
	int		m_nPlayType;
	CString	m_strPlayDTMF;
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CSwitchObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CSwitchObj);
	CSwitchObj();
public:
	CSwitchObj( CPoint point );
	CSwitchObj( const CSwitchObj &s );
	CSwitchObj & operator =( const CSwitchObj &s );
	virtual ~CSwitchObj();
public:
	int		m_nSwitchType;//Switch类型
	CString m_strAimChnl;
	int		m_nAimType;//0:System free channel 1:From
	int		m_nWaitOption;//0:play ringback tone 1:not tone 2:play file	
	CString	m_strPlayFile;
	

	int		m_nGetFreeChWay;//取得用于外拨的外线通道方式：0从所有外线中寻找一个外线通道；1从指定范围的外线中寻找一个外线通道
	CString	m_strBeginCH;
	CString	m_strEndCH;
	int		m_nTimeOut;//当CallAgent或CallCustomer多少秒认为呼叫超时

public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CRouteCallObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CRouteCallObj);
	CRouteCallObj();
public:
	CRouteCallObj( CPoint point );
	CRouteCallObj( const CRouteCallObj &s );
	CRouteCallObj & operator =( const CRouteCallObj &s );
	virtual ~CRouteCallObj();
public:
	CString m_strBeginCh;
	CString m_strEndCh;
	CString m_strTel;
	int		m_nWaitOption;//0:play ringback tone 1:not tone 2:play file	
	CString	m_strWaitFile;
	int		m_nConnOption;//0:play ringback tone 1:not tone 2:play file	
	CString	m_strConnFile;
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CCompareObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CCompareObj);
	CCompareObj();
public:
	CCompareObj( CPoint point );
	CCompareObj( const CCompareObj &s );
	CCompareObj & operator =( const CCompareObj &s );
	virtual ~CCompareObj();
public:
	int		m_nVarKey;
	CString	m_strVarName;
	
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CDTMFMenuObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CDTMFMenuObj);
	CDTMFMenuObj();
public:
	CDTMFMenuObj( CPoint point );
	CDTMFMenuObj( const CDTMFMenuObj &s );
	CDTMFMenuObj & operator =( const CDTMFMenuObj &s );
	virtual ~CDTMFMenuObj();
public:
	BOOL m_bClearDTMF;
	int m_nTimeOut;
	
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CRecordObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CRecordObj);
	CRecordObj();
public:
	CRecordObj( CPoint point );
	CRecordObj( const CRecordObj &s );
	CRecordObj & operator =( const CRecordObj &s );
	virtual ~CRecordObj();
public:
	CString m_strFileName;
	CString	m_strFolder;
	int		m_nFileLength;
	int		m_nRecordType;//0普通录音 1会议录音
	CString	m_strConferNO;
	CString m_strEndDTMF;
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CFaxReciveObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CFaxReciveObj);
	CFaxReciveObj();
public:
	CFaxReciveObj( CPoint point );
	CFaxReciveObj( const CFaxReciveObj &s );
	CFaxReciveObj & operator =( const CFaxReciveObj &s );
	virtual ~CFaxReciveObj();
public:
	CString m_strFileName;
	CString	m_strFolder;
	CString	m_strPage;
	
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CFaxSendObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CFaxSendObj);
	CFaxSendObj();
public:
	CFaxSendObj( CPoint point );
	CFaxSendObj( const CFaxSendObj &s );
	CFaxSendObj & operator =( const CFaxSendObj &s );
	virtual ~CFaxSendObj();
public:
	CString m_strFileName;
	CString	m_strFolder;
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CConferenceBulidObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CConferenceBulidObj);
	CConferenceBulidObj();
public:
	CConferenceBulidObj( CPoint point );
	CConferenceBulidObj( const CConferenceBulidObj &s );
	CConferenceBulidObj & operator =( const CConferenceBulidObj &s );
	virtual ~CConferenceBulidObj();
public:
	BOOL m_nBuildType;
	CString m_strConferNo;
	CString m_strMaxMember;
public:
	virtual void Serialize(CArchive& ar);
};



class AFX_EXT_CLASS CConferenceDeleteObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CConferenceDeleteObj);
	CConferenceDeleteObj();
public:
	CConferenceDeleteObj( CPoint point );
	CConferenceDeleteObj( const CConferenceDeleteObj &s );
	CConferenceDeleteObj & operator =( const CConferenceDeleteObj &s );
	virtual ~CConferenceDeleteObj();
public:
	BOOL m_nDeleteType;
	//删除类型：一种是根据系统，删除读取该通道指定的会议
	//一种是根据输入的会议编号，删除该会议
	//删除会议时，把会议里的所有通道都清除出去，同时释放会议资源
	CString m_strConferNo;
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CConferencePlayObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CConferencePlayObj);
	CConferencePlayObj();
public:
	CConferencePlayObj( CPoint point );
	CConferencePlayObj( const CConferencePlayObj &s );
	CConferencePlayObj & operator =( const CConferencePlayObj &s );
	virtual ~CConferencePlayObj();
public:
	CString	m_strConferNO;
	CString	m_strFileExpress;
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CConferenceRecordObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CConferenceRecordObj);
	CConferenceRecordObj();
public:
	CConferenceRecordObj( CPoint point );
	CConferenceRecordObj( const CConferenceRecordObj &s );
	CConferenceRecordObj & operator =( const CConferenceRecordObj &s );
	virtual ~CConferenceRecordObj();
public:
	CString	m_strConferNO;
	CString	m_strFileExpress;
	UINT	m_nFileLength;
	CString	m_strFolder;
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CGetConferMemberObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CGetConferMemberObj);
	CGetConferMemberObj();
public:
	CGetConferMemberObj( CPoint point );
	CGetConferMemberObj( const CGetConferMemberObj &s );
	CGetConferMemberObj & operator =( const CGetConferMemberObj &s );
	virtual ~CGetConferMemberObj();
public:
	CString m_strConferNo;
	CString m_strMemberCount;//当前会议成员数
	CString m_strMaxMemberNum; //允许最大会议成员数
	CString m_strMaxSpeakerNum; //允许能够发言最大成员数
	CString m_strMaxSpeakingNum; //允许同时发言最大成员数
	CString m_strListenNum; //当前监听成员数
	CString m_strSpeakerNum; //当前允许发言成员数
	CString m_strSpeakingNum; //当前正在发言成员数

public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CEnterConferObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CEnterConferObj);
	CEnterConferObj();
public:
	CEnterConferObj( CPoint point );
	CEnterConferObj( const CEnterConferObj &s );
	CEnterConferObj & operator =( const CEnterConferObj &s );
	virtual ~CEnterConferObj();
public:
	CString m_strConferNO;
	int m_nAction;
	CString m_strChnlIndex;//加入会议的通道号，-1表示自己加入会议
public:
	virtual void Serialize(CArchive& ar);
};


class AFX_EXT_CLASS CLeaveConferObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CLeaveConferObj);
	CLeaveConferObj();
public:
	CLeaveConferObj( CPoint point );
	CLeaveConferObj( const CLeaveConferObj &s );
	CLeaveConferObj & operator =( const CLeaveConferObj &s );
	virtual ~CLeaveConferObj();
public:
	CString m_strConferNO;
	CString m_strChnlIndex;//加入会议的通道号，-1表示自己加入会议

public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CDTMFSendObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CDTMFSendObj);
	CDTMFSendObj();
public:
	CDTMFSendObj( CPoint point );
	CDTMFSendObj( const CDTMFSendObj &s );
	CDTMFSendObj & operator =( const CDTMFSendObj &s );
	virtual ~CDTMFSendObj();
public:
	CString m_strDTMF;
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CConferRequestObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CConferRequestObj);
	CConferRequestObj();
public:
	CConferRequestObj( CPoint point );
	CConferRequestObj( const CConferRequestObj &s );
	CConferRequestObj & operator =( const CConferRequestObj &s );
	virtual ~CConferRequestObj();
public:
	int m_nDialType;
	CString m_strDialTel;
	int m_nAction;//加入会议的形式：SpeakListen ListenOnly SpeakOnly
public:
	virtual void Serialize(CArchive& ar);
};

/************************************************************
CBlockObj的成员必定有enter和return两种类型的个一个节点，
必定有一个CNEXTObj，同时所有的CNEXTObj必定有一个return与其关联。
CNEXTObj的m_strName==return的m_strCaption
CNEXTObj的m_nType==return的m_nKey

CNEXTObj的m_strNodeObjName==连接到的接点对象的Description
CNEXTObj的m_nNodeObjKey==连接到的接点对象的m_nKey

*************************************************************/

class AFX_EXT_CLASS CBlockObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CBlockObj);
	CBlockObj();
public:
	CBlockObj( CPoint point );
	CBlockObj( const CBlockObj &s );
	CBlockObj & operator =( const CBlockObj &s );
	virtual ~CBlockObj();
public:
	CNodeObjList m_ChildObjLst;
public:
	void PasteReplcaeDBBrowse(CBlockObj *pBlock, int nRSKey, int nNewRSKey,CString strNewRecordset );
	CNodeObj * FindNodeObj( int nKey, int &nParentKey );
	int PasteNodeObj(CDBConnVarList* pDBLst,CFDLLVariableList *pDLLlst, CRecordsetVarList *pRSlst, CBlockObj *pBlock, int nBaseKey );
	void PasteReplaceNextKey( CBlockObj *pBlock,int nNodeKey , int nNewNodeKey );
	CNodeObj * FindObj( int nKey );
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CStoreProObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CStoreProObj);
	CStoreProObj();
public:
	CStoreProObj( CPoint point );
	CStoreProObj( const CStoreProObj &s );
	CStoreProObj & operator =( const CStoreProObj &s );
	virtual ~CStoreProObj();
public:
	CString	m_strStoreProName;//存储过程名
	CString	m_strParameterVar;//参数常量或变量
	CString m_strRecordset;//存储过程返回的记录集
	
	CString m_strReturn;
	BOOL m_bReturnRecordSet;

	int m_nRecordsetKey;
	int m_nDBKey;
	int m_nDBType;
	CString m_strDB;

public:
	virtual void Serialize(CArchive& ar);
};


class AFX_EXT_CLASS CChangeVoiceObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CChangeVoiceObj);
	CChangeVoiceObj();
public:
	CChangeVoiceObj( CPoint point );
	CChangeVoiceObj( const CChangeVoiceObj &s );
	CChangeVoiceObj & operator =( const CChangeVoiceObj &s );
	virtual ~CChangeVoiceObj();
public:
	int		m_nChangeVoiceType;//参数的长度
	
public:
	virtual void Serialize(CArchive& ar);
};


class AFX_EXT_CLASS CEndCallBlockObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CEndCallBlockObj);
	CEndCallBlockObj();
public:
	CEndCallBlockObj( CPoint point );
	CEndCallBlockObj( const CEndCallBlockObj &s );
	CEndCallBlockObj & operator =( const CEndCallBlockObj &s );
	virtual ~CEndCallBlockObj();
public:
	CNodeObjList m_ChildObjLst;
public:
	CNodeObj * FindNodeObj(int nKey, int &nParentKey);
	CNodeObj * GetEndCallEventCheckObj( void );
	CNodeObj * FindObj( int nKey );
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CFunctionObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CFunctionObj);
	CFunctionObj();
public:
	CFunctionObj( CPoint point );
	CFunctionObj( const CFunctionObj &s );
	CFunctionObj & operator =( const CFunctionObj &s );
	virtual ~CFunctionObj();
public:
	CString m_strFunction;
	CString	m_strReturnValues;
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CExpressObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CExpressObj);
	CExpressObj();
public:
	CExpressObj( CPoint point );
	CExpressObj( const CExpressObj &s );
	CExpressObj & operator =( const CExpressObj &s );
	virtual ~CExpressObj();
public:
	CString	m_strExpressValues;
	CString m_strExpress;
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CGoToObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CGoToObj);
	CGoToObj();
public:
	CGoToObj( CPoint point );
	CGoToObj( const CGoToObj &s );
	CGoToObj & operator =( const CGoToObj &s );
	virtual ~CGoToObj();
public:
	int m_nNextObjKey;
	CString m_strConnPath;//连接目标对象的路径
public:
	void DisConnectToNode(int nKey);
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CTraceObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CTraceObj);
	CTraceObj();
public:
	CTraceObj( CPoint point );
	CTraceObj( const CTraceObj &s );
	CTraceObj & operator =( const CTraceObj &s );
	virtual ~CTraceObj();
public:
	CString m_strTrace;
public:
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CModuleObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CModuleObj);
	CModuleObj();
public:
	CModuleObj( CPoint point );
	CModuleObj( const CModuleObj &s );
	CModuleObj & operator =( const CModuleObj &s );
	virtual ~CModuleObj();
public:
	CNodeObj *m_pCurObj;
	CNodeObjList m_objects;
	CRecordsetVarList m_RecordsetVariList;
	CDBConnVarList m_DBConnVarList;
	CNormalVarList m_NormalVarList;
	CFDLLVariableList m_DLLVariableList;

	CParameterList m_ParameterLst;//模块节点参数列表
	CString m_strFile;//调用模块文件的绝对路径以及文件名
public:
	void UpdateParameterList( CParameterList *p );
	void RemoveParameterList( void );
	virtual void Serialize(CArchive& ar);
};

class AFX_EXT_CLASS CCCCallAgentObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CCCCallAgentObj);
	CCCCallAgentObj();
public:
	CCCCallAgentObj( CPoint point );
	CCCCallAgentObj( const CCCCallAgentObj &s );
	CCCCallAgentObj & operator =( const CCCCallAgentObj &s );
	virtual ~CCCCallAgentObj();
public:
	UINT	m_nTimeOut;
	int		m_nWaitType;
	CString	m_strWaitPlayFile;
	int		m_nPlayType;
	CString	m_strAimDN;
	BOOL	m_bIsPlayBeforeTalk;

	CNodeDataList m_PlayList;
	

public:
	virtual void Serialize(CArchive& ar);
};

/////////////////////////////////////////////////////////////////////////////
// CModuleDocument document

class AFX_EXT_CLASS CModuleDocument : public CDocument
{
	DECLARE_DYNCREATE(CModuleDocument)
public:
	CModuleDocument();  


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModuleDocument)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	public:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	CModuleObj m_ModuleObj;
	virtual ~CModuleDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CModuleDocument)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


extern "C" _declspec(dllexport) void AssignsObjList( const CNodeObjList &s ,CNodeObjList *d,int nAssigns );


class AFX_EXT_CLASS CCustomObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CCustomObj);
	CCustomObj();
public:
	CCustomObj( CPoint point );
	CCustomObj( const CCustomObj &s );
	CCustomObj & operator =( const CCustomObj &s );
	virtual ~CCustomObj();
public:
	POSITION	m_pos;
	CString     m_strFunName;
	int         m_nFunType;
	BOOL		m_bEnd;//是否到末尾了
public:
	void MoveFirst( void );
	POSITION MoveNextFun( void );
	virtual void Serialize(CArchive& ar);
};




class AFX_EXT_CLASS CCCTransferObj : public CNodeObj  
{
protected:
	DECLARE_SERIAL(CCCTransferObj);
	CCCTransferObj();
public:
	CCCTransferObj( CPoint point );
	CCCTransferObj( const CCCTransferObj &s );
	CCCTransferObj & operator =( const CCCTransferObj &s );
	virtual ~CCCTransferObj();
public:
	int		m_nTransferType;
	UINT	m_nTimeOut;
	int		m_nWaitType;
	CString	m_strPlayFile;
	CString	m_strCompleteDTMF;
	CString	m_strCancelDTMF;
	CString	m_strAimDN;
	BOOL	m_bTimeOut;
	BOOL	m_bTalkWitDN;
	
public:
	virtual void Serialize(CArchive& ar);
};
#endif 