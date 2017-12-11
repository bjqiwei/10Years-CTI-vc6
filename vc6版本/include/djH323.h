#ifndef __DJH323_H__
#define __DJH323_H__

/////////////////////////////////////////////////////////////////////////////////////////////////
//参数、结构定义

#define  MAX_CALLINFO_LEN  256
#define  MAX_FILENAME      256

//定义编码类型
#define  CT_G711ALAW     0x0001
#define  CT_G7231_53     0x0002
#define  CT_G7231_63     0x0004

// 定义呼叫状态
typedef enum {
	CS_INVALID,               //invalid
	CS_DIALTONE,              //dial tone
    CS_PROCEEDING,            //proceeding
    CS_RINGBACK,              //ring back
    CS_CONNECTED,             //connect
    CS_DISCONNECTED,          //disconnect
    CS_IDLE,                  //idle
    CS_OFFERING,              //offering
	CS_ADMISSIONCONFIRM,      //ACF
	CS_ADMISSIONREJECT,       //ARJ
    CS_WAITADDRESSACK,        //wait addr ack
	CS_TEMP,                  //temp
}DJH323_CALL_STATE;

// 定义呼叫模式
typedef enum {
	CM_INVALID,                   //invalid

	CM_CONTROLCONNECTED,          //connected mode: control connected
    CM_CALLSETUPCONNECTED,        //connected mode: call setup connected
    CM_CALLCONNECTED,             //connected mode: call connected
    CM_CONNECTEDCONFERENCE,       //connected mode: connected conference

    CM_DISCONNECTEDBUSY,          //disconnected mode: busy
    CM_DISCONNECTEDNORMAL,        //disconnected mode: normal
	CM_DISCONNECTEDREJECT,        //disconnected mode: reject
	CM_DISCONNECTEDUNREACHABLE,   //disconnected mode: unreachable
    CM_DISCONNECTEDUNKNOWN,       //disconnected mode: unknown
	CM_DISCONNECTEDLOCAL,         //disconnected mode: local

	CM_OFFERINGCREATE,            //offering mode: create
    CM_OFFERINGINVITE,            //offering mode: invite
	CM_OFFERINGJOIN,              //offering mode: join
}DJH323_CALL_MODE;


// 定义DJH323事件
#define H323_EV_REGISTERED          0x00000001
#define H323_EV_UNREGISTERED        0x00000002
#define	H323_EV_NEWCALL     		0x00000004
#define H323_EV_STATE_CHANGED       0x00000008
#define	H323_EV_DTMFRECV    		0x00000010

#define	H323_EV_ALL					0xFFFFFFFF


// 定义DJIP.INI的结构(基本不会使用)
typedef struct {
	int		iDetailInfo;
	WORD	wVAD_CNG_Enable;
	short	shVAD_Level;
	WORD	wDTMF_TransMode;
	WORD	wDTMF_On_Time;
	WORD	wDTMF_Off_Time;
	WORD	wEC_Enable;
	WORD	wPLR_OptNum;
	WORD	wPLR_MaxNum;
	WORD	wCoder;
	WORD	wFrameSize;
	WORD	wPort_Call;
	WORD	wPort_RTP;
}DJH323_INI_PARAM;

// 定义系统参数的结构
typedef struct {
	int		iTotalNum;
	BOOL    writeLog;
	char    logFileName[MAX_FILENAME+1];
	DWORD	dwEventMask;
	long	( WINAPI *CallEventHandler) (DWORD dwEvent, int iChID);
}DJH323_SYS_PARAM;

///////////////////////////////////////////////////////////////////////////////////////////////
//接口函数定义

#ifdef __cplusplus
extern "C" {
#endif
//=============================================================================================
//系统函数
int  WINAPI DJH323_InitSystem( void );
void WINAPI DJH323_ExitSystem( void );
void WINAPI	DJH323_GetIniInfo ( DJH323_INI_PARAM *pIniInfo );
void WINAPI	DJH323_GetSysInfo( DJH323_SYS_PARAM *pSysInfo );
int  WINAPI DJH323_GetIPChanNum(void);
int  WINAPI	DJH323_SetCallEventHandler( DWORD dwEventMask, long (WINAPI * hdlr)(DWORD dwEvent, int iChID));

//呼叫控制
int  WINAPI DJH323_CallMake(int iChID, char* strDial, char* Caller, char* Display, char* userUser, int userUserSize);
int  WINAPI DJH323_CallDrop(int iChID);
int  WINAPI DJH323_CallAnswerExt(int iChID, char* Display, char* userUser, int userUserSize);//发送Connect
int  WINAPI DJH323_CallAccept(int iChID); //接收端发送Alert

//呼叫信息
DJH323_CALL_STATE WINAPI DJH323_GetCallState( int iChID );
DJH323_CALL_MODE  WINAPI DJH323_GetCallMode( int iChID );

char* WINAPI DJH323_NewCallGetCallerId( char* CallerId, int size );
char* WINAPI DJH323_NewCallGetCalleeId( char* CalleeId, int size );
char* WINAPI DJH323_NewCallGetDisplayInfo(char* DisplayInfo, int size );
char* WINAPI DJH323_NewCallGetUserUserInfo(char* UserUserInfo, int size );

char* WINAPI DJH323_GetCallerId( int iChID, char* CallerId, int size );
char* WINAPI DJH323_GetCalleeId( int iChID, char* CalleeId, int size );
char* WINAPI DJH323_GetDisplayInfo( int iChID, char* DisplayInfo, int size );
char* WINAPI DJH323_GetUserUserInfo( int iChID, char* UserUserInfo, int size );

char* WINAPI DJH323_CallGetDTMF( int iChID , char* buf, int buflen);
int   WINAPI DJH323_CallSendDTMF(int iChID, char* dtmf);

int  WINAPI DJH323_SetCodeType(int iChID, WORD codetype);
WORD WINAPI DJH323_GetCodeType(int iChID);                     
WORD WINAPI DJH323_GetTransCodeType(int iChID, BOOL direction);    
DWORD WINAPI DJH323_GetTransPackets(int iChID, BOOL direction);  

//时隙管理
int  WINAPI DJH323_GetTimeSlot( int iChID );
int	 WINAPI DJH323_ConnectFromTS( int iChID, int iTS );
int	 WINAPI DJH323_DisconnectTS( int iChID );

//H.323配置管理
int  WINAPI DJH323_GetCallId( int nIndex, char* CallId, int size );
int  WINAPI DJH323_SetCallId(int nIndex, char* CallId);
int  WINAPI DJH323_ResetCallId(void);
int  WINAPI DJH323_DeleteCallId(int nIndex);
int  WINAPI DJH323_GetCallIdCount(void);
int  WINAPI DJH323_AddCallId(char* CallId);

int  WINAPI DJH323_SetManualRASFlag(BOOL flag);

int  WINAPI DJH323_GetGKAddress(char* GKAddr, int size);
int  WINAPI DJH323_SetGKAddress(char* GKAddr);
int  WINAPI DJH323_SetSysMaxCalls(int maxCalls);
long WINAPI DJH323_GetSysMaxCalls(void);
BOOL WINAPI DJH323_GetAllowCallsWhenNonRegFlag(void);
BOOL WINAPI DJH323_SetAllowCallsWhenNonRegFlag(BOOL flag);
int  WINAPI DJH323_Register(void);
int  WINAPI DJH323_UnRegister(void);

//语音(媒体流)处理
int   WINAPI DJH323_SetJitterSize(int iChID, BYTE size);
int   WINAPI DJH323_GetJitterSize(int iChID);
int   WINAPI DJH323_SetVADCtrl(int iChID, BOOL flag);
int   WINAPI DJH323_GetVADCtrl(int iChID);
int   WINAPI DJH323_SetVADThreshold(int iChID, short threshold);
short WINAPI DJH323_GetVADThreshold(int iChID);
int   WINAPI DJH323_SetECCtrl(int iChID, BOOL flag);
int   WINAPI DJH323_GetECCtrl(int iChID);

//日志管理
int  WINAPI DJH323_SetLogCtrl(char* filename, BOOL write);
//=============================================================================================
#ifdef __cplusplus
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
#endif //#ifndef __DJH323_H__