

#ifndef	__CTI_DEFINE_H__	//{
#define __CTI_DEFINE_H__

enum EChannelType					//通道类型
{
	CH_DIGITAL_TRUNK = 0,	//数字中继通道
	CH_USER,				//内线坐席通道
	CH_ANALOG_TRUNK,		//模拟中继通道(市话线)通道
	CH_VOIP,				//VoIP语音压缩解压通道
	CH_VOICE,				//语音通道
	CH_CONFERENCE,			//电话会议通道
	CH_PCM,
	CH_MFC,
	CH_DTMF,
	CH_HIGH_IMPEDANCE,		//高阻监控通道(模块)
	CH_MVIP,
	CH_EMPTY,				//表示空通道(模拟卡、座席卡未插模块)
	CH_SPEAKER,
	CH_MAX
};


 

enum EVoiceConferUsedMask
{
	USED_MASK_VOICE_PLAY	= 0x0001,
	USED_MASK_VOICE_RECORD	= 0x0002,
	USED_MASK_CONFER		= 0x0004,
	USED_MASK_SENDFAX		= 0x0008,
	USED_MASK_RECIEVEFAX	= 0x0010,
	USED_MASK_VOIPTALK      = 0x0020
};

enum ECardType						//卡类型
{
	CARD_TYPE_UNKNOW	= 0,
	CARD_TYPE_PCM		= 1,		//数字中继语音卡
	CARD_TYPE_SLIC		= 2,		//坐席卡 
	CARD_TYPE_ANALOG	= 3,		//模拟语音卡 
	CARD_TYPE_FAX		= 4,		//FAX卡 
	CARD_TYPE_ROUTER	= 5,		//过机卡
	CARD_TYPE_SS7		= 6,		//七号信令卡
	CARD_TYPE_VOIP		= 7			//IP压缩解压卡
};


enum EChannelState
{
	STATE_IDLE			= 0,	//0
	STATE_IN_CALLING,			//1
	STATE_IN_RINGING,			//2
	STATE_IN_TALK,				//3
	STATE_IN_HANGUP,			//4
	STATE_IN_RELEASE,			//5
	STATE_OUT_CALLING,			//6
	STATE_OUT_RINGING,			//7
	STATE_OUT_TALK,				//8
	STATE_OUT_HANGUP,			//9
	STATE_OUT_RELEASE,			//10
	STATE_DISABLE				//11
};

enum	EChannelEvent
{
	EVENT_STATE				= 100,
	EVENT_CALL_IN			= 101,
	EVENT_ANSWER			= 102,
	EVENT_HANGUP			= 103,
	EVENT_CALL_FAIL			= 104,
	EVENT_CALL_OUT			= 105,
	EVENT_TIMER				= 106,
	EVENT_TUP_GATEWAY_CLOSE = 701
};


enum EHangupCause
{
	CAUSE_UNKNOW		= 0,
	CAUSE_HANGUP		= 1,		//对方挂机
	CAUSE_TIMEOUT		= 2,		//呼叫超时
	CAUSE_UNN			= 10,		//空号
	CAUSE_SLB			= 11,		//用户市话忙
	CAUSE_SEC_CGC_NNC	= 12,		//交换设备拥塞信号、电路群拥塞信号、国内网拥塞信号
	CAUSE_ADI			= 13,		//地址不全信号
	CAUSE_LOS_CFL		= 14,		//线路不工作信号、呼叫失败信号
	CAUSE_STB			= 15,		//用户长话忙
	CAUSE_SST			= 16,		//发送专用信息音信号
	CAUSE_DPN			= 17,		//不提供数字通道信号
	CAUSE_ACB			= 18,		//接入拒绝信号
};


enum EVoiceChannelAttrib
{
	ATTRIB_VOICE_RECORD_PLAY	= 0,
	ATTRIB_VOICE_PLAY_ONLY		= 1
};


#ifndef exchangeBuildDoublyConnect
#define exchangeBuildDoublyConnect	0
#define exchangeBreakDoublyConnect	1
#define exchangeBuildSinglyConnect	2
#define exchangeBreakSinglyConnect	3
#endif

enum EVoipFaxChannelStatus
{
STATE_FREE = 0,
STATE_SENDFAX,
STATE_REVFAX,
STATE_IPUSING,
};

enum EDtvcErrorCode
{
	ERR_SUCCESS					= 0,		//操作成功
	ERR_UNKNOW					= -1,		//未知原因的错误
	ERR_NO_ENOUGH_RESOURCE		= -2,
	ERR_INTI_DEVICE_FAIL		= -3,
	ERR_INVALID_FUNC			= -4,
	ERR_CANNOT_LOAD_DRIVER		= -5,
	ERR_CANNOT_OPEN_NAMED_EVENT	= -6,


	ERR_INVALID_CHANNEL_ID		= -11,
	ERR_INVALID_CHANNEL_TYPE	= -12,
	ERR_INVALID_CARD_TYPE		= -13,
	ERR_INVALID_CARD_ID			= -14,
	ERR_INVALID_GROUP_ID		= -15,
	ERR_INVALID_CHANNEL_STATE	= -16,

	//Voice
	ERR_CANNOT_OPEN_FILE		= -101,		//打不开文件
	ERR_INVALID_FILE_SEEK		= -102,		//文件定位(设置读写位置)错误

	ERR_INVALID_PMT_INDEX		= -113,

	//Conference
	ERR_INVALID_CONFER_SET_ID	= -201,
	ERR_NOT_EXIST_CONFER_SET	= -202,
	ERR_INVALID_CONFER_HANDLE	= -203,
	ERR_INVALID_CONFER_MEMBER_ID	= -204,

	//VOIP
	ERR_VOIP_SEND_BUF_FULL		= -301,
	ERR_INVALID_CODER			= -302
};


enum EResourceUsage
{
	USAGE_NORAML		= 0,
	USAGE_PLAY_VOICE,
	USAGE_RECORD_VOICE,
	USAGE_VOIP_TALK,
	USAGE_VOIP_PLAY,
	USAGE_VOIP_RECORD
};

/*
enum	EVoiceUsageOrder	//申请语音服务资源的顺序
{
	VOICE_NORMAL = 0,		//只申请普通语音服务
	VOICE_VOIP,				//只申请IP语音服务
	VOICE_NORMAL_THEN_VOIP,	//先申请普通语音服务，如果未满足要求则申请IP语音服务
	VOICE_VOIP_THEN_NORMAL	//先申请IP语音服务，如果未满足要求则申请普通语音服务
};
*/


#define	_PAYLOAD_SIZE_G711_64K				40
#define	_PAYLOAD_SIZE_TRANSPARENT_64K		40
#define	_PAYLOAD_SIZE_G726_727_16K			10
#define	_PAYLOAD_SIZE_G726_727_24K			15
#define	_PAYLOAD_SIZE_G726_727_32K			20
#define	_PAYLOAD_SIZE_G726_727_40K			25
#define	_PAYLOAD_SIZE_G729A_8K				10
#define	_PAYLOAD_SIZE_G723d1_6d3K			24
#define	_PAYLOAD_SIZE_G723d1_5d3K			20
#define	_PAYLOAD_SIZE_NETCODER_6d4K			16
#define	_PAYLOAD_SIZE_NETCODER_7d2K			18
#define	_PAYLOAD_SIZE_NETCODER_8K			20
#define	_PAYLOAD_SIZE_NETCODER_8d8K			22
#define	_PAYLOAD_SIZE_NETCODER_9d6K			24
/*
enum	ECoderCodeType
{
	CODRE_G711_64K_ALAW			= 0,
	CODRE_G711_64K_ULAW			= 1,
	
	CODRE_TRANSPARENT_64K		= 0x16,
	
	CODRE_G726_16K				= 0x02,
	CODRE_G726_24K				= 0x03,
	CODRE_G726_32K				= 0x04,
	CODRE_G726_40K				= 0x05,

	CODRE_G727_16K				= 0x06,
	CODRE_G727_24K				= 0x08,
	CODRE_G727_32K				= 0x0b,

	CODRE_G723d1_6d3K			= 0x10,
	CODRE_G723d1_5d3K			= 0x0f,
	
	CODRE_G729A_8K				= 0x11,

	CODRE_NETCODER_6d4K			= 0x1b,
	CODRE_NETCODER_7d2K			= 0x1c,
	CODRE_NETCODER_8K			= 0x1d,
	CODRE_NETCODER_8d8K			= 0x1e,
	CODRE_NETCODER_9d6K			= 0x1f
};
*/
#ifndef DEFINE_VOICE_CODER
#define DEFINE_VOICE_CODER 
enum	EVoiceCoder
{
	CODER_ALAW					= 0,
	CODER_G726_32K				= 1,
	CODER_G726_16K				= 2,
	CODER_G729_8K				= 3,
	CODER_G723_6d3K				= 4,
	CODER_G723_5d3K				= 5,
	CODER_DIALOGIC_ADPCM_8Kx4	= 10,
	CODER_DIALOGIC_ADPCM_6Kx4	= 11,

	CODER_FOX_8K				= 20,
};
#endif 

enum EVoIPLoop
{
	VOIP_LOOP_NONE	= 0,
	VOIP_LOOP_HARD,
	VOIP_LOOP_DRIVER,
	VOIP_LOOP_INTERFACE,
	VOIP_LOOP_APPLICATION,
	VOIP_LOOP_NETWORK
};

enum EChannelCmdParameter
{
	PARAM_CODRE = 1
};

enum ESignalType
{
	SIGNAL_NONE				= 0,
	SIGNAL_REMOTE_DL,					//对方直流信令改变
	SIGNAL_LOCAL_DL,					//本地直流信令改变
	SIGNAL_REMOTE_SEND,					//对方发送MFC码
	SIGNAL_REMOTE_STOP,					//对方停发
	SIGNAL_LOCAL_SEND,					//本地发送MFC码
	SIGNAL_LOCALE_STOP					//本地停发
};

#define _DL_FORE_IDLE					0x0b	//A
#define _DL_FORE_USED					0x03	//B
#define _DL_FORE_CONNECT				0x03	//D

#define _DL_BACK_IDLE					0x0b	//A
#define _DL_BACK_CONFIRM				0x0f	//C
#define _DL_BACK_CONNECT				0x07	//D
#define _DL_BACK_BLOCK					0x0f	//F

//The follow message is for Mfc channel
#define _MFC_BACK_A1 					0x10	//request next
#define _MFC_BACK_A2 					0x11	//request from home
#define _MFC_BACK_A3 					0x12	//change to B signal
#define _MFC_BACK_A4 					0x13	//busy
#define _MFC_BACK_A5 					0x14	//space No.
#define _MFC_BACK_A6 					0x15	//request KA and caller

#define _MFC_BACK_KB1					0x10	//user is free
#define _MFC_BACK_KB2					0x11	//no used
#define _MFC_BACK_KB3					0x12	//no used
#define _MFC_BACK_KB4					0x13	//user is busy
#define _MFC_BACK_KB5					0x14	//space No.
#define _MFC_BACK_KB6					0x15	//user is free and controled by Caller

#define _MFC_BACK_KD1					1	//long half auto call
#define _MFC_BACK_KD2					2	//long auto call
#define _MFC_BACK_KD3					3	//city phone
#define _MFC_BACK_KD4					4	//city FAX or DATA
#define _MFC_BACK_KD5					5	//half auto confirm caller number
#define _MFC_BACK_KD6					6	//test call

#define _MFC_FORE_KA1					1
#define _MFC_FORE_KA2					2
#define _MFC_FORE_KA3					3
#define _MFC_FORE_KA4					4
#define _MFC_FORE_KA5					5
#define _MFC_FORE_KA6					6
#define _MFC_FORE_KA7					7
#define _MFC_FORE_KA8					8
#define _MFC_FORE_KA9					9
#define _MFC_FORE_KA10					10

#define _MFC_FORE_KA11					11
#define _MFC_FORE_KA12					12
#define _MFC_FORE_KA13					13
#define _MFC_FORE_KA14					14
#define _MFC_FORE_KA15					15

#define	_MFC_CALLER_END					0x0f



#define	_MAX_CONFERENCE_SET				10

union UConferenceHandle
{
	DWORD	m_hConferenceHandle;
	struct
	{
		WORD	m_wConferGroupID;
        WORD	m_wConferenceSetID;
	};
};

enum	EConferAttenution
{
	ATTEN_0DB	= 0,
	ATTEN_MINUS_3DB	= 1,
	ATTEN_MINUS_6DB	= 2
};

enum	EConferNoiseSuppresion
{
	NOISE_NO_SUPPRESION = 0,
	NOISE_9_DIVIDE_BY_4096 = 1,		//	9/4096
	NOISE_16_DIVIDE_BY_4096 = 2,	//	16/4096
	NOISE_32_DIVIDE_BY_4096 = 3		//	32/4096
};

#endif	//}__FIHO_DTVC_DEFINE_H__
