#ifndef __ISUP_DLL__
#define __ISUP_DLL__

//////////////////////////////////////////////////////////
//获取通道状态
//使用 DJISUP_GetChnState( 卡号, 通道号 ) 获取
//////////////////////////////////////////////////////////
//空闲状态
#define ISUP_CH_FREE			1
//DLL等待应用层同步释放资源
#define ISUP_CH_WAIT_RELEASE	2
//本地锁闭状态
#define ISUP_CH_LOCAL_BLOCK		3
//远端锁闭状态
#define ISUP_CH_REMOTE_BLOCK	4
//本地和远端锁闭状态
#define ISUP_CH_LOCAL_AND_REMOTE_BLOCK 5
//不可用态(网络不通等原因导致)
#define ISUP_CH_UNAVIABLE		6
//被叫闲,等待被叫应答
#define ISUP_CALLER_WAIT_ANM	7
//被叫应答,去话连通
#define ISUP_CALLER_CONNECT		8
//呼叫到达,接收全地址
#define ISUP_CALLEE_RECV_IAM	9
//摘机,来话连通
#define ISUP_CALLEE_CONNECT		10
//本地暂停
#define ISUP_CH_LOCAL_SUSPEND	11
//远端暂停
#define ISUP_CH_REMOTE_SUSPEND	12

//////////////////////////////////////////////////////////
//通道动作定义
//使用 DJISUP_SetChnState( 卡号, 通道号, Action ) 设置
//////////////////////////////////////////////////////////
//APP 层检查通道状态为 OxO2 时，同步释放通道资源
#define	ISUP_CH_SET_FREE		1
//锁闭通道
#define	ISUP_CH_SET_BLOCK		2
//解除锁闭
#define	ISUP_CH_SET_UNBLOCK		3
//来话方动作，来话到达后，送被叫闲信号，话路接通，但不送应答信号
#define	ISUP_CALLEE_SET_ACM		4
//被叫摘机信号
#define	ISUP_CALLEE_SET_ANM		5
//来话方动作，来话到达后，送被叫闲信号，话路接通，送应答信号
#define	ISUP_CALLEE_SET_CON		6
//送线路释放信号
#define	ISUP_CH_SET_REL			7
//群锁闭
#define	ISUP_CH_SET_CGB			8
//群解除锁闭
#define	ISUP_CH_SET_CGU			9
//后续地址
#define ISUP_CALLER_SET_SAM		10
//暂停
#define ISUP_CH_SET_SUS			11
//恢复
#define ISUP_CH_SET_RES			12

//锁闭通道证实
#define ISUP_CH_SET_BLA			0x81
//解除锁闭证实
#define ISUP_CH_SET_UBA			0x82
//电路复原
#define ISUP_CH_SET_RSC			0x83
//信息请求
#define ISUP_CH_SET_INR			0x84
//信息
#define ISUP_CH_SET_INF			0x85
//群锁闭证实
#define ISUP_CH_SET_CGBA		0x92
//群解除锁闭证实
#define ISUP_CH_SET_CGUA		0x94
//群复原
#define ISUP_CH_SET_GRS			0x95
//群复原证实
#define ISUP_CH_SET_GRA			0x96
//群问讯
#define ISUP_CH_SET_CQM			0x97
//群问讯响应
#define ISUP_CH_SET_CQR			0x98
//释放完成
#define ISUP_CH_SET_RLC			0x99
//混淆
#define ISUP_CH_SET_CFN			0x9a
//呼叫进展
#define ISUP_CALLEE_SET_CPG		0xa2

//////////////////////////////////////////////////////////

enum CAUSE_VALUE
{
	RSN_UNALLOCATED_NUMBER = 0x01,		//空号
	RSN_NORMAL_CALL_CLEARING = 0x10,	//正常的呼叫清除
	RSN_USER_BUSY = 0x11,				//用户忙
	RSN_NO_USER_RESPONDING = 0x12,		//无用户响应，超时没有收到ACM
	RSN_USER_ALERTING_NO_ANSWER = 0x13, //被叫无应答，超时未摘机
	RSN_CALL_REJECTED = 0x15,			//呼叫拒绝
	RSN_INCOMPLETE_NUMBER = 0x1c,		//不完整的号码
	RSN_NORMAL_NO_SET = 0x1f,			//正常，未规定
	RSN_NO_CIRCUIT_CHANNELAVAILABLE = 0x22, //无可用的电路
	RSN_REQUESTEDCIRCUIT_CHANNEL_NOTAVAILABLE = 0x2c, //请求的电路不可用
	RSN_RECOVERY_ON_TIME_EXPIRY = 0x66,	//定时器超时恢复

	RSN_LOCAL_RLC = 0x20,				//本地释放电路
	RSN_RECEIVE_RSC = 0x21,				//电路复原
};

//////////////////////////////////////////////////////////
//呼出后状态值列表
//将DJISUP_GetCalloutState的返回值与下列值做与(&)运算，即可知当前呼出状态
//////////////////////////////////////////////////////////
//同抢
#define CALLOUT_DUAL_SEIZURE		0x01
//收到电路锁闭
#define CALLOUT_REV_BLO				0x02
//收到电路复原
#define CALLOUT_REV_RSC				0x04
//收到地址全
#define CALLOUT_REV_ACM				0x08
//收到呼叫进展
#define CALLOUT_REV_CPG				0x10
//收到不合理的消息
#define CALLOUT_REV_UNREASONABLE	0X80

//////////////////////////////////////////////////////////
//取CPG的事件信息
//DJISUP_GetEventInfo返回如下事件信息
//////////////////////////////////////////////////////////
enum EVENT_INFO
{
	EI_NO_VALUE = 0x00,						//未有结果
	EI_ALERTING = 0x01,						//通知
	EI_PROGRESS = 0x02,						//进行
	EI_INBINFO_OR_APAVA = 0x03,				//带内信息或一个适合型式现可获得
	EI_BUSY_CALLTRANSFER = 0x04,			//遇忙呼叫前转
	EI_NOANSWER_CALLTRANSFER = 0x05,		//无应答呼叫前转
	EI_UNCONDITIONAL_CALLTRANSFER = 0x06,	//无条件呼叫前转
};


//////////////////////////////////////////////////////////
//计费指示码，其含义取决于计费的交换局
//DJISUP_GetChargeIndicator返回如下数值
//DJISUP_SetChnState函数当nState＝ISUP_CALLEE_SET_ACM时，Param1可取如下数值
//////////////////////////////////////////////////////////
enum CHARGE_INDICATOR
{
	CI_NO_INDICATION = 0x00,		//无指示
	CI_NO_CHARGE	 = 0x01,		//不计费
	CI_CHARGE		 = 0x02,		//计费
	CI_SPARE		 = 0x03,		//备用
};


//////////////////////////////////////////////////////////
//取改发信息中的改发指示码：默认值为0x04
//DJISUP_GetRedirectionInfIndicator返回如下信息
//ARIPR: all redirection information presentation restricted
//RNPR: redirection number presentation restricted
//////////////////////////////////////////////////////////
enum REDIRECTION_INDICATOR
{
	RI_NO_REDIRECTION				= 0x00,		//无改发
	RI_CALL_REROUTED				= 0x01,		//呼叫重新编路
	RI_CALL_REROUTED_ARIPR_INF		= 0x02,		//呼叫重新编路，所有改发信息显示限制
	RI_CALL_DIVERTED				= 0x03,		//呼叫转移
	RI_CALL_DIVERTED_ARIPR_INF		= 0x04,		//呼叫转移，所有改发信息显示限制
	RI_CALL_REROUTED_RNPR_NUM		= 0x05,		//呼叫重新编路，改发号码显示限制
	RI_CALL_DIVERTION_RNPR_NUM		= 0x06,		//呼叫转移，改发号码显示限制
};

//////////////////////////////////////////////////////////
//取改发信息中的改发原因：默认值为0x00
//DJISUP_GetRedirectionInfReason返回如下信息
//////////////////////////////////////////////////////////
enum REDIRECTION_REASON
{
	RI_UNKNOW_UNAVAILABLE	= 0x00,		//不知/不可获得
	RI_USER_BUSY			= 0x01,		//用户忙
	RI_NO_REPLY				= 0x02,		//无应答
	RI_UNCONDITIONAL		= 0x03,		//无条件
	RI_DEFLECTION_ALERTING	= 0x04,		//通知时偏转
	RI_DEFLECTION_RESPONSE	= 0x05,		//偏转立即响应
	RI_UNREACHABLE			= 0x06,		//移动用户不可达到
};


//呼出的主叫参数
//DJISUP_SetCallingParam输入参数
//////////////////////////////////////////////////////////
typedef struct _CALLING_PARTY_PARAM
{
	BYTE bNatureOfAddressIndicator;
		//地址性质指示码
		//0x00：备用
		//0x01：用户号码
		//0x02：不知
		//0x03：国内（有效）号码
		//0x04：国际号码
	
	BYTE bScreeningIndicator;  //屏蔽
		//屏蔽指示码
		//0x00：备用
		//0x01：用户提供，验证和通过
		//0x02：保留
		//0x03：网络提供

	BYTE bAddressPresentationRestrictedIndicator;  
		//地址显示限制指示码
		//0x00：显示允许
		//0x01：显示限制
		//0x02：地址不可利用
		//0x03：备用

	BYTE bNumberingPlanIndicator;
		//号码计划指示码
		//0x00：备用
		//0x01：ISDN（电话）号码计划（E.164）
		//0x02：备用
		//0x03：数据号码计划（X.121）
		//0x04：用户电报号码计划
		//0x05：保留用于国内应用
		//0x06：保留用于国内应用
		//0x07：备用

	BYTE bNumberIncompleteIndicator;
		//主叫用户号码不全指示码
		//0x00：完全
		//0x01：不完全

	BYTE bCallingPartyCategory;
		//主叫用户类别
		//0x00：类别未知
		//0x01：话务员，法语
		//...
		//0x0A：普通主叫用户
		//0x0B：优先主叫用户
		//0x0C：数据呼叫
		//0x0D：测试呼叫
		//0x0E：备用
		//0x0F：公用（投币）电话
		//0x09：可用于指明主叫用户是一个国内话务员
}CALLING_PARTY_PARAM, *LPCALLING_PARTY_PARAM;

#define DEFUALT_SET		0xFF

//////////////////////////////////////////////////////////
//设置前向呼叫指示码
//DJISUP_SetForwardCallIndicatorsParam输入参数
//////////////////////////////////////////////////////////
typedef struct _FORWARD_CALL_INDICATORS_PARAM
{
	BYTE bNationalInternationalCallIndicator;
		//国内/国际呼叫指示码
		//0x00：呼叫作为一个国内呼叫处理
		//0x01：呼叫作为一个国际呼叫处理
	BYTE bEndToEndMethodIndicator;
		//端到端方法指示码
		//0x00：端到端方法不可获得（仅可获得逐段链路转接方法）
		//0x01：传递（pass along）方法可获得
		//0x02：SCCP方法可获得
		//0x03：传递及SCCP方法可获得
	BYTE bInterworkingIndicator; 
		//互通指示码
		//0x00：不会碰到互通
		//0x01：会碰到互通
	BYTE bEndToEndInformationIndicator;
		//端到端信息指示码
		//0x00：无端到端信息可利用
		//0x01：端到端信息可利用
	BYTE bISDNUserPartIndicator;
		//ISDN用户部分指示码
		//0x00：非全程应用ISDN用户部分
		//0x01：全程应用ISDN用户部分
	BYTE bISDNUserPartPreferenceIndicator;
		//ISDN用户部分优先指示码
		//0x00：ISDN用户部分全程优先
		//0x01：非全程需要ISDN用户部分
		//0x02：全程需要ISDN用户部分
		//0x03：备用
	BYTE bISDNAccessIndicator;
		//ISDN接入指示码
		//0x00：始发端接入非ISDN
		//0x01：始发端接入ISDN
	BYTE bSCCPMethodIndicator;
		//SCCP方法指示码
		//0x00：无指示
		//0x01：无接续方法可获得
		//0x02：接续方法可获得
		//0x03：无接续及面向接续方法可获得
}FORWARD_CALL_INDICATORS_PARAM, *LPFORWARD_CALL_INDICATORS_PARAM;

//////////////////////////////////////////////////////////
//设置改发信息
//DJISUP_SetRedirectionInf输入参数
//////////////////////////////////////////////////////////
typedef struct _REDIRECTION_INFORMATION_PARAM
{
	BYTE bRedirectingIndicator;
		//改发指示码：默认为0x04
		//000：无改发
		//001：呼叫重新编路
		//010：呼叫重新编路，所有改发信息显示限制
		//011：呼叫转移
		//100：呼叫转移，所有改发信息显示限制
		//101：呼叫重新编路，改发号码显示限制
		//110：呼叫转移，改发号码显示限制
		//111：备用
	BYTE bOriginalRedirectionReason;
		//原来的改发原因：默认为0x00
		//0000：不知/不可获得
		//0001：用户忙
		//0010：无应答
		//0011：无条件
		//0100-1111：备用
	BYTE bRedirectionCounter;
		//改发计数器：默认为0x05
	BYTE bRedirectingReason;
		//原来的改发原因：默认为0x00
		//0000：不知/不可获得
		//0001：用户忙
		//0010：无应答
		//0011：无条件
		//0100：通知时偏转
		//0101：偏转立即响应
		//0110：移动用户不可达到
		//0111-1111：备用
}REDIRECTION_INFORMATION_PARAM, *LPREDIRECTION_INFORMATION_PARAM;

////////////////////////////////////////////////////////////
//设置呼叫进展消息
//DJISUP_SetEventInfParam 输入参数
//////////////////////////////////////////////////////////
typedef struct _EVENT_INFORMATION_PARAM
{
	BYTE bEventIndicator;
	//事件指示码
	//0000000：备用
	//0000001：通知
	//0000010：进行
	//0000011：带内信息或一个合适型式现可获得
	//0000100：遇忙呼叫前转
	//0000101：无应答呼叫前转
	//0000110：无条件呼叫前转
	BYTE bReserved : 1;
}EVENT_INFORMATION_PARAM, *LPEVENT_INFORMATION_PARAM;

////////////////////////////////////////////////////////////
//被叫号码信息
//DJISUP_SetCalledNumPartyParam 入参
////////////////////////////////////////////////////////////
typedef struct _CALLED_PARTY_NUMBER_PARAM
{
	BYTE bNatureOfAddressIndicator;
		//地址性质指示码
		//000000：备用
		//000001：用户号码
		//000010：不知
		//000011：国内（有效）号码
		//000100：国际号码
	BYTE bOddEvenIndicator;
		//奇/偶指示码
		//0：偶数个地址号码
		//1：奇数个地址号码
	BYTE bReserved;
	BYTE bNumberingPlanIndicator;
		//号码计划指示码
		//000：备用
		//001：ISDN（电话）号码计划（E.164）
		//010：备用
		//011：数据号码计划（X.121）
		//100：用户电报号码计划
		//101：保留用于国内应用
		//110：保留用于国内应用
		//111：备用
	BYTE InternalNetworkNumberIndicator;
		//内部网络号码指示码
		//0：编路至内部网络号码允许
		//1：编路至内部网络号码不允许（默认值）
	BYTE bAddressSignal[17];
}CALLED_PARTY_NUMBER_PARAM, *LPCALLED_PARTY_NUMBER_PARAM;


////////////////////////ISDN用户部分参数///////////////////
//发送ISDN用户部分参数
//DJISUP_SetParam(中继号，通道号，参数标识， 输入参数的指针)
//取已经收到的ISDN用户部分参数
//DJISUP_GetParam(中继号，通道号，参数标识， 输入参数的指针)
///////////////////////////////////////////////////////////
typedef enum _ISUP_PARAM
{
	ISUP_PARAM_AT	= 0x03,				//接入传送ACCESS TRANSPORT
	ISUP_PARAM_UTU	= 0x20,				//用户至用户
}ISUP_PARAM;

#define AT_LEN		100
//接入传送参数内容
typedef struct _PARAM_ACCESSTRANSPORT
{
	BYTE		bLen;	
	BYTE		bInfoUnit[AT_LEN];		//信息单元
}PARAM_ACCESSTRANSPORT;

#define UTU_LEN		100
//用户-用户信息参数内容
typedef struct _PARAM_USERTOUSERINFOR
{
	BYTE bLen;
	BYTE bUserToUserInformation[UTU_LEN];	//用户-用户信息
}PARAM_USERTOUSERINFOR;

///////////////////////////////////////////////////////////

#define EXIST_REDIRECTION_NUM	0x0001
#define EXIST_REDIRECTION_INF	0x0001

///////////////////////////////////////////////////////////

#ifdef	__cplusplus
extern "C" {
#endif

int		WINAPI DJISUP_InitSystem();
void	WINAPI DJISUP_ExitSystem();
int		WINAPI DJISUP_GetChnState(BYTE nPCM, BYTE nCHN);
void	WINAPI DJISUP_SetChnState(BYTE nPCM, BYTE nCHN, BYTE nState, int Param1 = 0, int Param2 = 0, int Param3 = 0, int Param4 = 0);
void	WINAPI DJISUP_SetChnState_ForVB(BYTE nPCM, BYTE nCHN, BYTE nState, void *Param1 = NULL, int Param2 = 0, int Param3 = 0, int Param4 = 0);
void	WINAPI DJISUP_GetCallerNum(BYTE nPCM, BYTE nCHN, char * strCallerId);
void	WINAPI DJISUP_GetCalleeNum(BYTE nPCM, BYTE nCHN, char * strCalleeId);
int		WINAPI DJISUP_GetOraCalleeNum(BYTE nPCM, BYTE nCHN, char * strOraCalleeId);
void	WINAPI DJISUP_GetEvent();
bool	WINAPI DJISUP_GetEventA(BYTE nLinkId);
int		WINAPI DJISUP_GetEventRawFrame(BYTE * pData);   //pData 从SIO开始
int		WINAPI DJISUP_SetOraCalleeNumber(int nPCM, int nCHN, char * szOraCalleeNumber = "" );
int		WINAPI DJISUP_Callout(int nPCM, int nCHN, char * szCalleeNumber, char * szCallerNumber = "", bool bCalleeSignalTerminal = false, bool bCallerSignalTerminal = false);
int		WINAPI DJISUP_GetCalloutChn(BYTE nPCM);
bool	WINAPI DJISUP_GetCalloutChnNew( BYTE* mo, BYTE* dt );//Add for dual seizure
int		WINAPI DJISUP_GetSpNo( char *sDPC );
bool	WINAPI DJISUP_GetCalloutChnMSP( BYTE nSp_No, BYTE* mo, BYTE* dt );
int		WINAPI DJISUP_GetReleaseReason(int nPCM, int nCHN);
int		WINAPI DJISUP_GetCalloutState(int nPCM, int nCHN);
int		WINAPI DJISUP_GetEventInfo(int nPCM, int nCHN);

int		WINAPI DJISUP_GetRawFrame(int nLinkId, BYTE * pData);
int		WINAPI DJISUP_GetRawFrameFromMTC(BYTE nPCM, BYTE nCHN, BYTE * pData); //pData 从消息类型开始
int		WINAPI DJISUP_GetNonIsupRawFrame(BYTE * pData);
int		WINAPI DJISUP_GetSccpRawFrame(BYTE * pData);
bool	WINAPI DJISUP_SendRawFrame(BYTE nPCM, BYTE nCHN, BYTE * pData, int nLen);
bool	WINAPI DJISUP_SendSccpRawFrame(BYTE * pData, int nLen);
bool	WINAPI DJISUP_SendSS7RawFrame(BYTE * pData, int nLen);

int		WINAPI DJISUP_SetParam(BYTE nPCM, BYTE nCHN, ISUP_PARAM ParamID, void * pParam);
int		WINAPI DJISUP_GetParam(BYTE nPCM, BYTE nCHN, ISUP_PARAM ParamID, void * pParam);

void	WINAPI DJISUP_SetRedirectionNum(BYTE nPCM, BYTE nCHN, char* pRedirectionNum);

int		WINAPI DJISUP_SetCallingParam(BYTE nPCM, BYTE nCHN, CALLING_PARTY_PARAM *pSetCallingParm);

//设置中继的同步状态，在七号网关应用中，节点使用非东进D系列板卡时使用,其它情况中不能使用此函数！
int		WINAPI DJISUP_SetPcmSyncState(BYTE nPCM, bool bSync);

int		WINAPI DJISUP_SetForwardCallIndicatorsParam(BYTE nPCM, BYTE nCHN, FORWARD_CALL_INDICATORS_PARAM *pSetForwardCallIndicatorsParam);
int		WINAPI DJISUP_GetRecvCauseValue(int nPCM, int nCHN);
int		WINAPI DJISUP_GetChargeIndicator(int nPCM, int nCHN);

int		WINAPI DJISUP_GetRedirectingNum(BYTE nPCM, BYTE nCHN, char * strRedirectingId);
int		WINAPI DJISUP_SetRedirectingNum(BYTE nPCM, BYTE nCHN, char* pRedirectingNum);

int		WINAPI DJISUP_GetRedirectionInfIndicator(BYTE nPCM, BYTE nCHN );
int		WINAPI DJISUP_GetRedirectionInfReason(BYTE nPCM, BYTE nCHN );
int		WINAPI DJISUP_SetRedirectionInf(BYTE nPCM, BYTE nCHN, REDIRECTION_INFORMATION_PARAM *pSetRedirectionInfParam );

int		WINAPI DJISUP_SetEventInfParam(BYTE nPCM, BYTE nCHN, LPEVENT_INFORMATION_PARAM pSetEventInfParam); 
int		WINAPI DJISUP_SetCalledPartyNumberParam(BYTE nPCM, BYTE nCHN, LPCALLED_PARTY_NUMBER_PARAM pParam);


#ifdef	__cplusplus
}
#endif

#endif
