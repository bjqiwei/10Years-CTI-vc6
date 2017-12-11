#ifndef	_NO7DLL_H
#define	_NO7DLL_H

//////////////////////////////////////////////////////////
//通道状态定义
//使用 DJNo7_GetChnState( 卡号, 通道号 ) 获得
//////////////////////////////////////////////////////////

//空闲状态
#define CH_FREE			1
//DLL等待应用层同步释放资源
#define CH_WAIT_RELEASE	2
//本地锁闭状态
#define CH_LOCAL_BLOCK	3
//远端锁闭状态
#define CH_REMOTE_BLOCK	4
//不可用态(网络不通等原因导致)
#define CH_UNAVIABLE	5

//去话连通状态
#define CALLER_CONNECT  6
//主叫方送拆线信号,等待被叫回送释放监护信号
#define CALLER_WAIT_RLG 7

//呼叫到达,接收全地址
#define CALLEE_RECV_IAM 8
//来话连通状态
#define CALLEE_CONNECT  9
//已发送挂机信号，等待主叫发送拆线信号
#define CALLEE_WAIT_CLF 10

//内部状态，应用程序不对此状态进行处理
#define CH_CANOMIT		11

#define CALLER_RECV_CBK	12
#define CALLER_RECV_RAN 13

//用于被叫控制关机时，被叫收到CCL
#define CALLEE_RECV_CCL 14
//用于被叫控制关机时，被叫收到RAN
#define CALLEE_RECV_RAN 15

#define CALLER_RECV_ACM 0x91	//主叫收到ACM

//////////////////////////////////////////////////////////
//通道动作定义
//使用 DJNo7_SetChnState( 卡号, 通道号, Action ) 设置
//////////////////////////////////////////////////////////
//APP 层检查通道状态为 0x02(CH_WAIT_RELEASE) 时，同步释放通道资源
#define	CH_SET_FREE		1
//锁闭通道
#define	CH_SET_BLOCK	2
//解除锁闭
#define	CH_SET_UNBLOCK	3

//来话方动作，来话到达后，送被叫闲信号，话路接通，但不送应答信号
#define	CALLEE_SET_ACM	4
//被叫摘机信号
#define	CALLEE_SET_ANC	6
//来话方动作，来话到达后，送被叫忙信号
#define	CALLEE_SET_SSB	7
//来话方动作，来话到达后，送被叫市忙信号
#define	CALLEE_SET_SLB	8
//来话方动作，来话到达后，送被叫长忙信号
#define	CALLEE_SET_STB	9
//来话方动作，来话到达后，送空号信号
#define	CALLEE_SET_UNN	10
//来话方动作，送被叫挂机信号
#define	CALLEE_SET_CBK	12
//来话方动作，送拆线证实信号
#define	CALLEE_SET_RLG	13
//去话方动作，送拆线信号
#define	CALLER_SET_CLF	14
//来话方动作，送线路不工作信号
#define	CALLEE_SET_LOS	15
//来话方动作，送接入拒绝信号
#define	CALLEE_SET_ACB	16

//电路复原
#define CH_SET_RSC		0x81
//电路群复原
#define CH_SET_GRS		0x82
//一般请求消息
#define CALLEE_SET_GRQ	0x83
//来话方动作，送呼叫故障信号
#define	CALLEE_SET_CFL	0x84
//来话方动作，送地址不全信号
#define	CALLEE_SET_ADI	0x85

//面向维护的群闭塞消息
#define	CH_SET_MGB		101	//0x65
//面向维护的解除群闭塞消息
#define	CH_SET_MGU		0x87
//硬件故障的群闭塞消息
#define	CH_SET_HGB		100	//0x64
//硬件故障的解除群闭塞消息
#define	CH_SET_HGU		0x89
//软件产生的群闭塞消息
#define	CH_SET_SGB		0x90
//软件产生的解除群闭塞消息
#define	CH_SET_SGU		0x91

//////////////////////////////////////////////////////////
//呼出主叫用户类别
//DJNo7_CalloutEx的CallerCategory参数定义
//////////////////////////////////////////////////////////
//国内话务员
#define CALLEE_C_00		0x00
#define CALLEE_C_01		0x01
#define CALLEE_C_02		0x02
#define CALLEE_C_03		0x03
#define CALLEE_C_04		0x04
#define CALLEE_C_05		0x05
#define CALLEE_C_06		0x06
#define CALLEE_C_07		0x07
#define CALLEE_C_08		0x08
#define CALLEE_C_09		0x09
//普通用户，在长（国际）－长， 长（国际）－市局间使用
#define CALLEE_C_0A		0x0A
//优先用户，在长（国际）－长， 长（国际）－市， 市－市局间使用
#define CALLEE_C_0B		0x0B
//数据呼叫
#define CALLEE_C_0C		0x0C
//测试呼叫
#define CALLEE_C_0D		0x0D
//普通、免费		（在市－长（国际）局间使用）
#define CALLEE_C_10		0x10
//普通、定期		（在市－长（国际）局间使用）
#define CALLEE_C_11		0x11
//普通、用户表、立即（在市－长（国际）局间使用）
#define CALLEE_C_12		0x12
//普通、打印机、立即（在市－长（国际）局间使用）
#define CALLEE_C_13		0x13
//优先、免费		（在市－长（国际）局间使用）
#define CALLEE_C_14		0x14
//优先、定期		（在市－长（国际）局间使用）
#define CALLEE_C_15		0x15
//普通用户，在市－市局间使用
#define CALLEE_C_18		0x18

//////////////////////////////////////////////////////////
//呼出地址性质表示语
//DJNo7_CalloutEx的OriCalleeAddrAttr参数定义
//DJNo7_CalloutEx2的CalleeAddrAttr和OriCalleeAddrAttr参数定义
//DJNo7_CalloutBEx的CalleeAddrAttr参数定义
//////////////////////////////////////////////////////////
//市内用户号码
#define CALLEE_AT_00		0x00
//备用
#define CALLEE_AT_01		0x01
//国内有效号码
#define CALLEE_AT_10		0x02
//国际号码
#define CALLEE_AT_11		0x03

//////////////////////////////////////////////////////////
//呼出主叫用户类别
//DJNo7_CalloutEx和DJNo7_CalloutBEx的CallerAddrAttr参数定义
//////////////////////////////////////////////////////////
//市内用户号码
#define CALLER_AT_00		0x00
//国内备用
#define CALLER_AT_01		0x01
//国内有效号码
#define CALLER_AT_10		0x02
//国际号码
#define CALLER_AT_11		0x03


//////////////////////////////////////////////////////////
//呼出状态
//DJNo7_CheckCalloutResult返回
//////////////////////////////////////////////////////////
//未返回结果
#define C_NO_RESULT		0
//被叫空闲
#define C_USER_IDLE		1
//被叫摘机，连通状态
#define C_USER_OFFHOOK	2
//地址不全
#define C_ADDR_LACK		3
//占线
#define C_USER_BUSY		4
//空号
#define	C_UNALLOC_CODE	5
//发生同抢
#define	C_CALL_COLLIDE	6
//对局无回应
#define	C_TIME_OUT		7
//其它原因呼叫不成功
#define	C_CALL_FAIL		8
//远程通道闭塞
#define	C_REMOTE_BLOCK	9
//被叫挂机
#define C_USER_ONHOOK	10
//被叫60秒内无应答
#define C_NO_ANSWER		11

//////////////////////////////////////////////////////////
//用户空闲子状态,在呼出后,DJNo7_CheckCalloutResult返回C_USER_IDLE时
//调用DJNo7_CheckCalloutSubResult函数返回
//////////////////////////////////////////////////////////
#define C_SUB_NOINDICATE_NOINDICATE   0x0   //地址全,用户闲忙未指示
#define C_SUB_CHARGE_NOINDICATE       0x1   //地址全,计费,用户闲忙未指示
#define C_SUB_NOCHARGE_NOINDICATE     0x2   //地址全,免费,用户闲忙未指示
#define C_SUB_COIN_NOINDICATE         0x3   //地址全,投币式电话,用户忙闲未指示
#define C_SUB_NOINDICATE_IDLE         0x4   //地址全,用户闲
#define C_SUB_CHARGE_IDLE             0x5   //地址全,计费,用户闲
#define C_SUB_NOCHARGE_IDLE           0x6   //地址全,免费,用户闲
#define C_SUB_COIN_IDLE               0x7   //地址全,投币式电话,用户闲

//////////////////////////////////////////////////////////
//呼出失败子状态,在呼出后,DJNo7_CheckCalloutResult返回C_USER_BUSY时
//调用DJNo7_CheckCalloutSubResult函数返回
//////////////////////////////////////////////////////////
//用户忙
#define C_SUB_SSB					0x0001
//用户市忙
#define C_SUB_SLB					0x0002
//用户长忙
#define C_SUB_STB					0x0003
//接入拒绝
#define C_SUB_ACB					0x0004


//////////////////////////////////////////////////////////
//呼出失败子状态,在呼出后,DJNo7_CheckCalloutResult返回C_CALL_FAIL时
//调用DJNo7_CheckCalloutSubResult函数返回
//////////////////////////////////////////////////////////
//交换设备拥塞
#define C_SUB_SEC					0x0010
//电路群拥塞
#define C_SUB_CGC					0x0011
//国内网拥塞
#define C_SUB_NNC					0x0012
//线路不工作
#define C_SUB_LOS					0x0013
//发送专用信号音信号
#define C_SUB_SST					0x0014
//不提供数字通道
#define C_SUB_DPN					0x0015
//呼叫故障
#define C_SUB_CFL					0x0016

//////////////////////////////////////////////////////////
//呼出成功子状态,在呼出后,DJNo7_CheckCalloutResult返回C_USER_OFFHOOK时
//调用DJNo7_CheckCalloutSubResult函数返回
//////////////////////////////////////////////////////////
//计费未说明
#define C_SUB_NOINDICATE			0x0000
//计费
#define C_SUB_CHARGE				0x0100
//不计费
#define C_SUB_UNCHARGE				0x0200

//////////////////////////////////////////////////////////
//后向建立成功消息类型定义
//为DJNo7_SetChnStateA( nPCM, nCHN, CALLEE_SET_ACM, param )中param参数
//param must be: (ACM_ADDR_XXXX | ACM_CALLEE_XXXX | ...)
//////////////////////////////////////////////////////////
//地址全
#define ACM_ADDR_COMPLETE	0
//地址全,计费
#define ACM_ADDR_CHARGE		1
//地址全,不计费
#define ACM_ADDR_UNCHARGE	2
//地址全,投币式电话
#define ACM_ADDR_COINTEL	3

//用户忙闲状态未指示
#define ACM_CALLEE_UNKNOW	0
//用户闲
#define ACM_CALLEE_IDLE		4

//信号通道表示语
//任何通道
#define ACM_ANY_PATH		0
//全部是No7信号方式通道
#define ACM_ALL_NO7_PATH	32

//////////////////////////////////////////////////////////
//后向呼叫监视消息类型定义
//为DJNo7_SetChnStateA( nPCM, nCHN, CALLEE_SET_ANC, param )中param参数
//////////////////////////////////////////////////////////
//应答信号,计费未说明(ANU)
#define CSM_CHARGE_NOINDICATE		0x0		// == TYPE_ANSWER
//应答信号,计费(ANC)
#define CSM_CHARGE					0x1		// == TYPE_ANSWER_CHARGE
//应答信号,不计费(ANN)
#define CSM_UNCHARGE				0x2		// == TYPE_ANSWER_NOCHARGE

//////////////////////////////////////////////////////////
//当通道状态是CH_WAIT_RELEASE时，产生此状态的原因则用
//DJNo7_GetReleaseCause( nPCM, nCHN)获取原因值。
//原因值的定义如下
//////////////////////////////////////////////////////////
//应用程序初始化，正常释放
#define RELEASE_NO_CAUSE			0x00
//收到CLF信号
#define RELEASE_RECV_CLF			0x01
//收到RLG信号
#define RELEASE_RECV_RLG			0x02
//本地解除锁闭得到证实
#define RELEASE_RECV_UBA			0x03
//远端电路群复原
#define RELEASE_RECV_GRS			0x04
//本地电路群复原得到证实
#define RELEASE_RECV_GRA			0x05
//远端电路群解除闭赛
#define RELEASE_REMOTE_GU			0x06


#ifdef	__cplusplus
extern "C" {
#endif

int  WINAPI DJNo7_InitSystem();
void WINAPI DJNo7_ExitSystem();
void WINAPI DJNo7_GetEvent();
bool WINAPI DJNo7_GetEventA(BYTE nLinkId);
int  WINAPI DJNo7_GetEventRawFrame(BYTE *pData);	   //pData 从SIO开始
BYTE WINAPI DJNo7_GetChnState( BYTE mo_num, BYTE dt_num );
void WINAPI DJNo7_SetChnState( BYTE mo_num, BYTE dt_num, BYTE state );
void WINAPI DJNo7_SetChnStateA( BYTE nPCM, BYTE nCHN, BYTE state, BYTE param ) ;
void WINAPI DJNo7_SetChnStateB( BYTE nPCM, BYTE nCHN, BYTE state, unsigned long param ) ;
void WINAPI DJNo7_GetCalleeNum( BYTE mo_num, BYTE dt_num, char* call_id );
void WINAPI DJNo7_GetCallerNum( BYTE mo_num, BYTE dt_num, char* call_id );
void WINAPI DJNo7_GetOriCalleeNum( BYTE mo_num, BYTE dt_num, char* call_id );
//add for ZDH for Power builder
char * WINAPI DJNo7_GetCallerNumA(BYTE nPCM, BYTE nCHN);
char * WINAPI DJNo7_GetCalleeNumA(BYTE nPCM, BYTE nCHN);
char * WINAPI DJNo7_GetOriCalleeNumA(BYTE nPCM, BYTE nCHN);
//add for ZDH for Power builder

int  WINAPI DJNo7_GetSpNo( char *sDPC );	// sDPC以"x.y.z"的形式给出
bool WINAPI DJNo7_GetCalloutChn( BYTE* mo_num, BYTE* dt_num );
bool WINAPI DJNo7_GetCalloutChnMSP(BYTE nSp_No, BYTE* mo, BYTE* dt );
bool WINAPI DJNo7_Callout( BYTE mo_num, BYTE dt_num, char* callee, char* caller = "" );
bool WINAPI DJNo7_CalloutEx( BYTE nPCM, BYTE nCHN, char* callee, char *caller, BYTE CallerCategory = 0x18, BYTE CallerAddrAttr = 0, bool bTranCall = false, char* OriCallee = "", BYTE OriCalleeAddrAttr = 0);
bool WINAPI DJNo7_CalloutEx2( BYTE nPCM, BYTE nCHN, char* callee, char *caller, BYTE CallerCategory = 0x18, BYTE CalleeAddrAttr = 0, BYTE CallerAddrAttr = 0, bool bTranCall = false, char* OriCallee = "", BYTE OriCalleeAddrAttr = 0);
bool WINAPI DJNo7_CalloutB( BYTE nPCM, BYTE nCHN, char* callee, char *caller );
bool WINAPI DJNo7_CalloutBEx( BYTE nPCM, BYTE nCHN, char* callee, char *caller, BYTE CallerCategory = 0x18, BYTE CalleeAddrAttr = 0, BYTE CallerAddrAttr = 0);
BYTE WINAPI DJNo7_CheckCalloutResult( BYTE mo_num, BYTE dt_num );
DWORD WINAPI DJNo7_CheckCalloutSubResult( BYTE nPCM, BYTE nCHN );
bool WINAPI DJNo7_CalloutA( BYTE mo_num, BYTE dt_num, BYTE* data, int len );
void WINAPI DJNo7_SetCalloutAParam( BYTE nPCM, BYTE nCHN, char* caller);
bool WINAPI DJNo7_CalloutC( BYTE mo_num, BYTE dt_num, char * s_callee);
//自己填充呼出包,可以设置主叫用户类别,回声抑制等,data包含SIF中的H0,H1和信令信息部分
void WINAPI DJNo7_GetCallInfo( BYTE mo_num, BYTE dt_num, char * info, int len );

int WINAPI DJNo7_GetCallRawFrame(BYTE nPCM, BYTE nCHN, char *RawFrame);
BYTE WINAPI DJNo7_GetACMMessage(BYTE nPCM, BYTE nCHN);

void WINAPI DJNo7_SetSystemMode( DWORD mode , DWORD param=0 );
//该函数用来设置系统的工作模式，此函数应在系统初始化完成后调用
//mode的第0位为1(mode|=1)时,在接收到IAM或IAI后,开始等待SAM,在等待param秒后,如还未收到SAM,则认为地址全,
//               该项功能在被叫地址长度未知时使用
//mode的第1位为1(mode|=2)时,话务员插入功能,在呼出时,会以话务员主叫用户类别呼出
//               在等待ACM时本地不做超时判断,因交换机不会回ACM信号,交换机可能回SSB,SLB,STB
//               如收到用户忙,用户市忙,用户长忙信号,系统不回送CLF,用DJNo7_CheckCalloutResult和DJNo7_CheckCalloutSubResult
//				 可以得到呼出结果
//               如收到后向挂机信号CBK,系统不回送CLF,而是将通道状态置为CALLER_RECV_CBK,用DJNo7_GetChnState可以捕捉到该状态
//mode的第2位为1(mode|=4)时, 接收与发送原始数据包, 系统不对数据包做任何处理，不可与DJNo7_GetEvent共用
//mode的第3位为1(mode|=8)时, 该模式用于7号信令网关，当节点非东进板卡（如Dialogic板卡）时，设置中继的同步信息。
//				必须在调用DJNo7_GetEvent函数之前用DJNo7_SetSystemMode向系统提示每个中继的同步状态。
//				当DJNo7_SetSystemMode的参数 mode的值设为8，参数param是32位数值，每一位表示对应PCM的同步状态，
//				支持32个中继状态，1为同步，0为失步。例如：现有一个子应用系统中有3个中继，其中第0 中继失步了，
//				第1、2 中继正常，这样调用函数：DJNo7_SetSystemMode(8,0x00000006)。
//mode的第4位为1(mode|=16)时，用于获取收到的非TUP数据，参见函数DJNo7_GetRawFrameOt。
//mode的第5位为1(mode|=32)时，被叫控制挂机方式

void WINAPI DJNo7_SetCalloutAParam( BYTE nPCM, BYTE nCHN, char* caller);
void WINAPI DJNo7_StartTimer(BYTE nPCM, BYTE nCHN, int nTimerValue);
int WINAPI DJNo7_TimerElapsed(BYTE nPCM, BYTE nCHN);

int WINAPI DJNo7_GetRawFrame(int nLinkId, BYTE * strRawFrame);//strRawFrame包括SIO 和SIF 
bool WINAPI DJNo7_SendRawFrame(BYTE nPCM, BYTE nCHN, BYTE* pData, int nLen);//pData为SIF中的H0,H1和信令信息部分

//int WINAPI DJISUP_GetRawFrame(int nLinkId,BYTE * strRawFrame);	//strRawFrame包括SIO 和SIF 
//bool WINAPI DJISUP_SendRawFrame(BYTE* pData,int nLen);//pData包括SIO 和SIF 

bool WINAPI DJNo7_SendSS7RawFrame(BYTE* pData, int nLen);//pData包括SIO 和SIF 

int WINAPI DJNo7_GetRawFrameOt(BYTE * strRawFrame);//strRawFrame包括SIO 和SIF 
bool WINAPI DJNo7_SendRawFrameOt(BYTE* pData,int nLen);//pData包括SIO 和SIF 
int WINAPI DJNo7_GetSccpRawFrame(BYTE * strRawFrame);//strRawFrame从消息类型开始，不包括SIO、DPC、OPC & SLS

int WINAPI DJNo7_GetReleaseCause(BYTE nPCM, BYTE nCHN);

BYTE WINAPI DJNo7_GetACMMessage(BYTE nPCM, BYTE nCHN);

#ifdef	__cplusplus
}
#endif

#endif