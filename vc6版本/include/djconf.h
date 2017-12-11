#ifndef	_DJCONF_H
#define	_DJCONF_H


#define		MAX_CONF_CARD_NUM		8
#define		MAX_CONF_RESOURCE_NUM	(32*MAX_CONF_CARD_NUM)
#define		MAX_CONF_GROUP_NUM		(16*MAX_CONF_CARD_NUM)
#define		MAX_MEMBER_NUM_PER_GROUP		32

#define		CONF_MODE_ADD		0
#define		CONF_MODE_LISTEN	1

#define		CONF_CHANNEL_TYPE_TRUNK		0
#define		CONF_CHANNEL_TYPE_USER		1
#define		CONF_CHANNEL_TYPE_VOICE		2	
// add 2002.11.02 for CTX-conf
#define		CONF_CHANNEL_TYPE_TIMESLOT	3

typedef struct {
	WORD	wCardNum;
	WORD	wCardType;
	WORD	wGroupNum;
	WORD	wAddress[MAX_CONF_CARD_NUM];
	WORD	wStream[MAX_CONF_CARD_NUM];
} CONF_SYS_INFO;

typedef struct {
	BYTE	cbMode;
	BYTE	cbChnlType;
	WORD	wChnlNo;
	WORD	wResNo;
	WORD    wLDNo;
} MEMBER_INFO;

typedef struct {
	WORD	wMemberNum;
	WORD	wResNum;
	WORD	wListenNum;
	MEMBER_INFO	MemberInfo[MAX_MEMBER_NUM_PER_GROUP];
} GROUP_INFO;

#ifndef __BUILD_FOR_SYS__

#ifdef	__cplusplus
extern "C" {
#endif

int  WINAPI DJConf_InitConfCard(void);
// -1 -2 -3 -4 : fail
// 0 : ok

void WINAPI DJConf_ReleaseConfCard(void);
void WINAPI DJConf_GetSysInfo(CONF_SYS_INFO *TmpCSI);

int WINAPI DJConf_EnterConf ( WORD wGroupNo, BYTE cbMode, BYTE cbChnlType, WORD wChnlNo, 
					   char chAtte, BYTE cbNoiseSupp );
int WINAPI DJConf_LeaveConf ( BYTE cbChnlType, WORD wChnlNo );
int WINAPI DJConf_DestroyGroup ( WORD wGroupNo );
long WINAPI DJConf_GetChnlGroupInfo ( BYTE cbChnlType, WORD wChnlNo );
int WINAPI DJConf_GetGroupInfo ( WORD wGroupNo, GROUP_INFO *TmpGroupInfo );
int WINAPI DJConf_SFVC_ForRecord ( WORD wPcmNo );
int	WINAPI DJConf_GetAvailResNum ( WORD wConfCardNo );
// add for CTX-CONF, 2002-11-02
int	WINAPI	DJConf_GetTimeSlot_OutChnl ( WORD wTimeSlot_In );
// add in 2003.07.15, after "DJConf_GetTimeSlot_OutChnl", we can get it's real output TimeSlot
WORD WINAPI DJConf_GetOutTimeSlot_ByResNo ( WORD wResNo );



int		WINAPI DJConf_PlayFileNew(WORD wGroupNo,char *FileName,DWORD Position,DWORD Length);
//return voice channel ID <0 error
//-1 no free voice resource
//-2 can't open file
//-3 invalid position parameter
//-4 play fail
//-5 enter conference fail
int		WINAPI DJConf_PlayPromptFile(WORD wGroupNo, const char * voiceName );
//return voice channel ID <0 error
//-1 no free voice resource
//-2 play fail
//-3 enter conference fail
int		WINAPI DJConf_LoopPlayPromptFile(WORD wGroupNo, const char * voiceName );
//return voice channel ID <0 error
//-1 no free voice resource
//-2 play fail
//-3 enter conference fail
void    WINAPI DJConf_StopPlayFile(WORD wVoiceChnlNo);

int		WINAPI DJConf_RecordFileNew(WORD wGroupNo,char *FileName,DWORD Position,DWORD Length);
//return voice channel ID <0 error
//-1 no free voice resource
//-2 can't open file
//-3 record fail
//-4 enter conference fail
void	WINAPI DJConf_StopRecordFile(WORD wVoiceChnlNo);


//同一时刻，1组会议只能有1个语音通道进行PlayPromptStr 
int		WINAPI DJConf_PlayPromptStr(WORD wGroupNo,const char * pcPromptStr);
//>=0 correct
//-1 allocate memory fail
//-2 no free voice resource
//-3 invalid prompt name]
//-4 enter conference fail
bool	WINAPI DJConf_CheckPlayPromptStrEnd(WORD wGroupNo);
//true correct
//false error
void WINAPI DJConf_StopPlayPromptStr(WORD wGroupNo);

//同一时刻，1组会议只能有1个语音通道进行IndexPlayFile 
void	WINAPI DJConf_InitIndexPlayFile(WORD wGroupNo);
bool	WINAPI DJConf_AddIndexPlayFile(WORD wGroupNo, char *FileName);
bool	WINAPI DJConf_StartIndexPlayFile(WORD wGroupNo);
//true correct
//false error
void	WINAPI DJConf_StopIndexPlayFile(WORD wGroupNo);
bool	WINAPI DJConf_CheckIndexPlayEnd(WORD wGroupNo);


//add by lanjun for use Truck voice
int WINAPI DJConf_PlayFileNewEx(WORD wGroupNo,char *FileName,DWORD Position,DWORD Length,int TruckID);
int WINAPI DJConf_PlayPromptFileEx(WORD wGroupNo,const char * voiceName,int TruckID );
int WINAPI DJConf_LoopPlayPromptFileEx(WORD wGroupNo,const char * voiceName,int TruckID );
int WINAPI DJConf_PlayPromptStrEx(WORD wGroupNo,const char * pcPromptStr,int TruckID);
bool WINAPI DJConf_StartIndexPlayFileEx(WORD wGroupNo,int TruckID);
//add by lanjun ***********************


// add in 2003.05.27, 支持超过32个组成员
#define		MAX_MEMBER_NUM_PER_GROUP_MASS		512

typedef struct {
	WORD	wMemberNum;
	WORD	wResNum;
	WORD	wListenNum;
	MEMBER_INFO	MemberInfo[MAX_MEMBER_NUM_PER_GROUP_MASS];
} GROUP_INFO_MASS;


void WINAPI DJConf_Enable_Mass(void);
int WINAPI DJConf_GetGroupInfo_Mass ( WORD wGroupNo, GROUP_INFO_MASS *TmpGroupInfo );

// add next function in 2004.03.29，支持会议DTMF抑制的可选择
int  WINAPI DJConf_Adjust_CtrlWord ( BYTE cbChnlType, WORD wChnlNo, WORD wCtrl );

// end of add


#ifdef	__cplusplus
}
#endif

#endif

#endif