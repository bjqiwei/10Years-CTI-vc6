#ifndef	__ISDNDLL_1_H__
#define	__ISDNDLL_1_H__

//////////////////////////////////////////////////////////
//通道释放原因定义
//使用 DJISDN_GetDsiconnectReason( 卡号, 通道号 ) 获得
//////////////////////////////////////////////////////////
enum DISCONNECT_REASON
{
	RSN_UNKNOW_REASON = 0X00,		//原因未知
	RSN_UNALLOC_NUMBER = 0x01,		//空号
	RSN_NORMAL_DISCONNECT = 0X10,	//正常的呼叫清除 
	ISDN_RSN_USER_BUSY = 0X11,			//用户忙
	RSN_NO_RESPOND = 0X12,			//无响应
	RSN_NO_ANSWER = 0X13,			//无用户应答
	RSN_REFUSR_CALL = 0X15,			//呼叫拒绝
	RSN_NUMBER_ERROR = 0X1C,		//号码不全
	RSN_TIMEOUT	= 0X66,				//超时
	RSN_DCHANNEL_DOWN = 0Xfd,		//链路中断
	RSN_BCHANNEL_UNAVIABLE = 0X22,  //通道不可用
	RSN_UNAVIABLE_CIRCULT = 0X2c,	//无可用通路
	RSN_UNSVIABLE_MSG = 0X5F,		//无效的消息 
};

//////////////////////////////////////////////////////////
//通道状态定义
//使用 DJISDN_GetChnState( 卡号, 通道号 ) 获得
//////////////////////////////////////////////////////////
//空闲状态   //
#define ISDN_CH_FREE					1

//DLL等待应用层同步释放资源
#define ISDN_CH_WAIT_APP_FREE		2

//不可用态(网络不通等原因导致)
#define ISDN_CH_UNAVIABLE			3

//等待连接证实
#define ISDN_CH_WAIT_CONNECT_ACK		4

//连通状态  
#define ISDN_CH_CONNECT				5 

//已发送拆线信号，等待对方发送释放信号  
#define ISDN_CH_WAIT_RELEASE			6

//已发送释放信号，等待对方发送释放证实信号
#define ISDN_CH_WAIT_RELEASE_COMPLETE 7

//来话方 呼叫到达
#define ISDN_CALLEE_WAIT_ANSWER		11 

//去话方 已呼出，等待被叫回应
#define ISDN_CALLER_WAIT_ANSWER		12

//去话方 已接收到提醒消息
#define ISDN_CALLER_RECV_ALERT		21

//去话方 已接收到建立确认消息 
#define ISDN_CALLER_RECV_SETUP_ACK	22

//去话方 已接收到呼叫过程消息
#define ISDN_CALLER_RECV_CALLPROCESS 23

#define ISDN_CALLER_WAIT_SETUP_ACK	24

//////////////////////////////////////////////////////////
//通道动作定义
//使用 DJISDN_SetChnState( 卡号, 通道号, Action ) 设置
//////////////////////////////////////////////////////////
//APP 层检查通道状态为 OxO2 时，同步释放通道资源
#define	ISDN_CH_SET_FREE			1
//设置通道连接
#define ISDN_CH_SET_CONNECT		2
//解除通道连接
#define ISDN_CH_SET_DISCONNECT	3
//被叫方发送提醒消息
#define ISDN_CALLEE_SET_ALERTING	4
//设置通道重新启动
#define	ISDN_CH_SET_RESTART		5

//////////////////////////////////////////////////////////
//呼出结果
//////////////////////////////////////////////////////////

//未返回结果  //
#define ISDN_C_NO_RESULT		 0
//被叫空闲
#define ISDN_C_USER_IDLE		 1
//被叫摘机，连通状态
#define ISDN_C_USER_OFFHOOK	 2
//重叠发送，等待网络确认被叫号收齐
#define ISDN_C_WAIT_CALL_PROC 3
//其它，呼叫可能结束，请使用 DJISDN_GetChnState 函数查阅通道状态
#define ISDN_C_OTHERS 10

//////////////////////////////////////////////////////////
//号码类别 
//////////////////////////////////////////////////////////
//未知  
#define ISDN_CALLNUM_UNKNOWN			0x00
//国际号码
#define ISDN_CALLNUM_INTERNATIONAL	0x01
//国内号码
#define ISDN_CALLNUM_DOMESTIC		0x02
//网络特定号码
#define ISDN_CALLNUM_NETWORK_SPECIFIC	0x03
//用户号码
#define ISDN_CALLNUM_USER			0x04
//扩展保留
#define ISDN_CALLNUM_RESERVE			0x07


#ifdef	__cplusplus
extern "C" {
#endif

int WINAPI DJISDN_InitSystem();
void WINAPI DJISDN_ExitSystem();
void WINAPI DJISDN_GetEvent();

int WINAPI DJISDN_GetChnState(int nPCM, int nCHN);
int WINAPI DJISDN_GetChnStateA(int nPCM, int nCHN);

int WINAPI DJISDN_SetChnState(int nPCM, int nCHN, int nState, int nParam=0);
void WINAPI DJISDN_GetCallerNumber(int nPCM, int nCHN, char * szNumber);
void WINAPI DJISDN_GetCallerSubAddr(int nPCM, int nCHN, char * szSubAddr);
void WINAPI DJISDN_GetCalleeNumber(int nPCM, int nCHN, char * szNumber);
void WINAPI DJISDN_GetCalleeSubAddr(int nPCM, int nCHN, char * szSubAddr);

bool WINAPI DJISDN_GetCalloutChn( int * nPCM, int * nCHN );

void WINAPI DJISDN_GetChangeNumber(int nPCM, int nCHN, char * szNumber);

int WINAPI DJISDN_Callout(int nPCM, int nCHN, char * szCalleeNumber, char * szCalleeSubAddr="", char * szCallerNumber="", char * szCallerSubAddr="");
int WINAPI DJISDN_CalloutA(int nPCM, int nCHN, char * szCalleeNumber, char * szCalleeSubAddr, char * szCallerNumber, char * szCallerSubAddr, int nCalleeNumType=0x02, int nCallerNumType=0x02);
int WINAPI DJISDN_CalloutB(int nPCM, int nCHN, char * szCalleeNumber, char * szCalleeSubAddr, char * szCallerNumber, char * szCallerSubAddr, int nCalleeNumType=0x02, int nCallerNumType=0x02);
int WINAPI DJISDN_CalloutC(int nPCM, int nCHN, char * szCalleeNumber , bool bSendEnd);

int WINAPI DJISDN_GetCalloutResult(int nPCM, int nCHN);
int WINAPI DJISDN_GetDisconnectReason(int nPCM, int nCHN);
bool WINAPI DJISDN_SendRawData(int nPCM, BYTE * pRawData, int nLen);
//
BYTE * WINAPI DJISDN_GetRawDate(int *pPCM, int *pLen);
int CCB_RawDateGetCallNum(int *pPCM, int *pCHN, char *pCalleeNum, char *pCallerNum);

void WINAPI DJISDN_SetSystemMode( int nMode , int nParam );

int WINAPI DJISDN_GetErrorPacket(int nPCM);
#ifdef	__cplusplus
}
#endif
#endif


// 