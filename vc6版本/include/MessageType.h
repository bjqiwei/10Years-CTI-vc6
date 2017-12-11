#ifndef MLMESSAGETYPE_H
#define MLMESSAGETYPE_H

const int CFMTYPE_MONITOR			= 0x3001;//Monitoring
const int CFMTYPE_DMONITOR			= 0x3003;//Disable Monitoring
const int CFMTYPE_MCALL				= 0x3005;//Make Call
const int CFMTYPE_INCONFER		    = 0x3007;//In Conference
const int CFMTYPE_LCONFER		    = 0x3009;//Leave Conference 
const int CFMTYPE_TRANSFER			= 0x3011;//Transfer
const int CFMTYPE_ANSWER			= 0x3013;//Answer
const int CFMTYPE_RELEASECALL		= 0x3015;//Release Call
const int CFMTYPE_HOLD				= 0x3017;//Hold Request
const int CFMTYPE_PICKUPHOLD		= 0x3019;//Pickup Hold Request
const int CFMTYPE_SENDFAX			= 0x3021;//Send Fax Request
const int CFMTYPE_LOGIN				= 0x3022;//Set Feature Login
const int CFMTYPE_LOGOUT			= 0x3024;//Set Feature Logout
const int CFMTYPE_MSETBBUSY			= 0x3026;//Set Feature MSB
const int CFMTYPE_MSI				= 0x3028;//Set Feature MSI
const int CFMTYPE_SFNOTR			= 0x3030;//Set Feature Not Ready
const int CFMTYPE_SFR				= 0x3032;//Set Feature  Ready
const int CFMTYPE_RINGING			= 0x3034;//Ringing event
const int CFMTYPE_HANGUP			= 0x3035;//Hangup event
const int CFMTYPE_OFFHOOK			= 0x3036;//OffHook event
const int CFMTYPE_CONFER_CALL		= 0x3037;//Conference Call 
const int CFMTYPE_BUILD_CONFER	    = 0x3038;//BUILD  Conference request
const int CFMTYPE_GETACTIVECALL	    = 0x3039;//BUILD  Conference request
const int CFMTYPE_VALIDATE	        = 0x3040;//Socket connect validate msg
 
/*================================================================*/
/*New message type const define For IVR Interfaces ====================================*/
const int CFMTYPE_REG_COMM			= 0x2000;//reg to Comm and response
const int CFMTYPE_NET_ABEND			= 0x2001;//Net abend message type
const int CFMTYPE_CANCEL_TRANSFER	= 0x2003;//cancel Transfer
const int CFMTYPE_AGENT_TO_AGENT	= 0x2004;//cancel Transfer

//Make call request and response
typedef struct{
	int nCallType;//0 呼叫客户 1 呼叫坐席
	int nResult;//呼叫结果，0 成功 1 失败
	char szTel[32];
}CFMSG_MAKECALL;

//Monitor request and response
typedef struct{
	int nResult;//结果，0 成功 1 失败
	char szTel[32];
}CFMSG_MONITOR;



//Transfer request and response
typedef struct{
	int nCallType;//0 呼叫客户 1 呼叫坐席
	int nResult;//结果，0 成功 1 失败
	char szTel[32];
}CFMSG_TRANSFER;

//Cancel Transfer request and response
typedef struct{
	int nResult;//结果，0 成功 1 失败
}CFMSG_CANCEL_TRANSFER;

//Hold request and response
typedef struct{
	int nResult;//结果，0 成功 1 失败
	char szTel[32];
}CFMSG_HOLD;

//Pickup Hold request and response
typedef struct{
	int nResult;//结果，0 成功 1 失败
	int nType;
	char szTel[32];//找回指定主叫号码的保持通道

}CFMSG_PICKUPHOLD;

//Pickup Hold response
typedef struct{
	int nResult;//结果，0 成功 1 失败
	char szCallerID[32];
	char szCalledID[32];
}CFMSG_PICKUPHOLD_RES;

//Ringing response
typedef struct{
	char szTel[32];
	char szTel2[32];
}CFMSG_RINGING;

//AnswerCALL request and response
typedef struct{
	int nResult;//结果，0 成功 1 失败
}CFMSG_ANSWERCALL;

typedef struct{
	int nResult;//结果，0 成功 1 失败
	int nConferNO;
}CFMSG_BUILD_CONFER;

typedef struct{
	int nResult;//结果，0 成功 1 失败
	char szTel[32];
}CFMSG_IN_CONFER;

typedef struct{
	int nResult;//结果，0 成功 1 失败
	int nCallType;//0 呼叫客户 1 呼叫坐席
	char szTel[32];
}CFMSG_CONFER_CALL;

typedef struct{
	int nResult;//结果，0 成功 1 失败
	int nType;
	char szTel[32];
}CFMSG_GETACTIVECALL;


typedef struct{
	int nResult;//结果，0 成功 1 失败
	char szCallerID[32];
	char szCalledID[32];
}CFMSG_GETACTIVECALL_RES;

typedef struct{
	int nResult;//结果，0 成功 1 失败
	char szAgentID[20];//工号
	char szPWD[20];//密码
	int nGroup;//技能组编号
}CFMSG_LOGIN;

typedef struct{
	int nResult;//结果，0 成功 1 失败
	char szAgentID[20];//工号
	char szPWD[20];//密码
	int nGroup;//技能组编号
}CFMSG_LOGOUT;

typedef struct{
	int nResult;//结果，0 成功 1 失败
	char szAgentID[20];//工号
	int nGroup;//技能组编号
}CFMSG_READY;

typedef struct{
	int nResult;//结果，0 成功 1 失败
	char szAgentID[20];//工号
	int nGroup;//技能组编号
}CFMSG_NOT_READY;

typedef struct{
	int nResult;//结果，0 成功 1 失败
	char szAgentID[20];//工号
	int nGroup;//技能组编号
}CFMSG_SET_BUSY;

typedef struct{
	int nResult;//结果，0 成功 1 失败
	char szAgentID[20];//工号
	int nGroup;//技能组编号
}CFMSG_INSERVER;
#endif
