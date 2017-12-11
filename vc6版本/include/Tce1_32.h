#ifndef	_TCE1_32_H
#define	_TCE1_32_H
 
// add by H.J.N ------------
#include "windows.h"

// add next define is just for E1
#define UINT   unsigned int
// end of add --------------

#define _DEBUG_CONVER

#define PCM_TRUNK_NUM			30

#define _MFC_BUF_LEN            31
#define _CALL_BUF_LEN           31
#define _DTMF_BUF_LEN			31
#define _PMT_VOICE_BUF_LEN		31

#define _CONVERSATION_RECORD_BEGIN_VOICE_ID   20

typedef enum enumTrunkStep {
	Step_Free			= 0,	//for import and export

	Step_Used,					//1 for import
	Step_RecvCall,				//2
	Step_RecvMfc,				//3
	Step_StopMfc,				//4
	Step_RecvFail,				//5

	Step_Delay, 				//6
	Step_Wait,					//7
	Step_Connect,				//8
	Step_ForeHangOff,			//9
	Step_BackHangOff,			//10

	Step_Block, 				//11
	Step_OverTime,				//12

	Step_Disable,				//13	//disable send and receive phone

	Step_ReadyDial, 			//14	//for export
	Step_DialCall,				//15
	Step_DialMfc,				//16
	Step_DialStop,				//17
	Step_DialFail,				//18

	Step_FreeRes				//19
}TrunkStep;

typedef enum enumTrunkType{
	Type_Import = 0,
	Type_Export = 1
}TrunkType;

typedef enum enumDialStatus{
	DS_NoDial = 0,
	DS_Dialing, 	//1
	DS_Busy,		//2
	DS_Wait,		//3
	DS_Connect, 	//4
	DS_LineError,	//5
	DS_NoUser,		//6
	DS_OverTime 	//7
}DialStatus;

typedef enum enumVoiceResourcesType{
	Res_NoRes		= 0,
	Res_File		= 1,
	Res_Xms 		= 2,
}VoiceResourcesType;
typedef enum enumVoiceOperatorType{
	OP_NoOperator	= 0,
	OP_Play 		= 1,
	OP_LoopPlay 	= 2,
	OP_Record		= 3
}VoiceOperatorType;


typedef enum enumPstnErr{
	_ERR_OK 					= 0,
	_ERR_XmsAllocError			= -1,
	_ERR_MemAllocError			= -2,
	_ERR_CanNotOpenFile 		= -3,
	_ERR_NoTrunkRes 			= -4,
	_ERR_NoUserRes				= -5,
	_ERR_NoVoiceRes 			= -6,
	_ERR_InvalidID				= -7,
	_ERR_ReConnDifferChannel	= -8,
	_ERR_OperateTypeErr 		= -9,
	_ERR_InvalidTrunkStep		= -10,
	_ERR_NoConnChannel			= -11,
	_ERR_INIsetErr				= -12,
	_ERR_NullPtr				= -13,
	_ERR_NoMvipRes				= -14,
	_ERR_InvalidDTMF			= -15,
    _ERR_LastDtmfNotSendEnd     = -16,
	_ERR_OpenTCE1Device			= -17,		//	add by H.J.N
	_ERR_CheckHardware			= -18,		//	add by H.J.N
	_ERR_PromptFile				= -19,		//	add by H.J.N
	_ERR_DirectPlayXms			= -20,		//	add by H.J.N
	_ERR_FilePosition			= -21       //  add by N.C.J
}PstnErr;
struct	tagPstnParam{
	BYTE	bySysHour;
	BYTE	bySysMinute;
	BYTE	bySysSecond;

	DWORD	dwRunHours;
	BYTE	byRunMinutes;
	BYTE	byRunSeconds;

	DWORD	dwRunSeconds;

	int 	iCountINT;

	UINT	uHardWareINT;

	char *	lpClearINT;
	char *	lpVoiceBufferFlag;		// change "char far *" to "char *" by H.J.N

    BOOL    fSendDLForForeHangOff;
	BYTE	byEnableWarning;
	int 	iSpeakEnable;
	int 	iIsBeep;
	int 	iBeepCount;
	UINT	uFreq;
	BYTE	bySoundControl;
	int		iAutoApplyDtmf;


	DWORD	dwImportPhoneNum;
	DWORD	dwExportPhoneNum;
	UINT	uCurrentImportNum;
	UINT	uCurrentExportNum;
	UINT	uLastImportNum;
	UINT	uLastExportNum;

	int 	iExportNum;

	int 	iTrunkNum;
	int 	iVoiceNum;
	int 	iUserNum;
	int 	iMfcNum;
	int 	iDtmfNum;
	int		iBusMVIPNum;

	WORD	wHndPmtVoice;
	int 	iPmtVoiceNum;
	
	BOOL	bAutoKB;

	PstnErr ErrCode;                
	
	int		iConversationNum;
	int		iConversationGroupNum;
	int		iaConversationFlag[12];

    BYTE    byHangCount;
};

#define _DL_FORE_FREE			0x0b	//A
#define _DL_FORE_USED			0x03	//B
#define _DL_FORE_CONFIRM		0x03	//C
#define _DL_FORE_CONNECT		0x03	//D
#define _DL_FORE_FOREHANGOFF	0x0b	//E
#define _DL_FORE_BUSY			0x0b	//F
#define _DL_FORE_BACKHANGOFF	0x03	//C

#define _DL_BACK_FREE			0x0b	//A 0x08|0x03
#define _DL_BACK_USED			0x0b	//B
#define _DL_BACK_CONFIRM		0x0f	//C 0x0c|0x03
#define _DL_BACK_CONNECT		0x07	//D 0x04|0x03
#define _DL_BACK_BACKHANGOFF	0x0f	//C
#define _DL_BACK_FOREHANGOFF	0x0f	//F
#define _DL_BACK_BLOCK			0x0f	//F

#define MFC_BACK_A1 				16	//request next
#define MFC_BACK_A2 				17	//request from home
#define MFC_BACK_A3 				18	//change to B signal
#define MFC_BACK_A4 				19	//busy
#define MFC_BACK_A5 				20	//space No.
#define MFC_BACK_A6 				21	//request KA and caller

#define MFC_BACK_KB1				16	//user is free
#define MFC_BACK_KB2				17	//no used
#define MFC_BACK_KB3				18	//no used
#define MFC_BACK_KB4				19	//user is busy
#define MFC_BACK_KB5				20	//space No.
#define MFC_BACK_KB6				21	//user is free and controled by Caller

#define MFC_BACK_KD1				1	//long half auto call
#define MFC_BACK_KD2				2	//long auto call
#define MFC_BACK_KD3				3	//city phone
#define MFC_BACK_KD4				4	//city FAX or DATA
#define MFC_BACK_KD5				5	//half auto confirm caller number
#define MFC_BACK_KD6				6	//test call

#define MFC_FORE_KA1				1
#define MFC_FORE_KA2				2
#define MFC_FORE_KA3				3
#define MFC_FORE_KA4				4
#define MFC_FORE_KA5				5
#define MFC_FORE_KA6				6
#define MFC_FORE_KA7				7
#define MFC_FORE_KA8				8
#define MFC_FORE_KA9				9
#define MFC_FORE_KA10				10

//define the failure reason
#define _ERR_USED_FOREHANGOFF           1
#define _ERR_USED_OVERTIME              2
#define _ERR_RCALL_OVERTIME             3
#define _ERR_RCALL_FOREHANGOFF          4
#define _ERR_RCALL_TOO_LEN              5
#define _ERR_RMFC_FOREHANGOFF           6
#define _ERR_RMFC_OVERTIME              7
#define _ERR_RMFC_TOO_LEN               8
#define _ERR_RSTOP_FOREHANGOFF          9
#define _ERR_RSTOP_OVERTIME             10
#define _ERR_DREDY_OVERTIME             11
#define _ERR_DMFC_LINEERR               12
#define _ERR_DMFC_BACK_A_ERR1           13
#define _ERR_DMFC_BACK_A_ERR2           14
#define _ERR_DMFC_BACK_A_ERR3           15
#define _ERR_DMFC_OVERTIME              16
#define _ERR_DCALL_LINEERR              17
#define _ERR_DCALL_BACK_A_ERR1          18
#define _ERR_DCALL_BACK_A_ERR2          19
#define _ERR_DCALL_OVERTIME             20
#define _ERR_DSTOP_LINEERR              21
#define _ERR_DSTOP_NOUSER               22
#define _ERR_DSTOP_BUSY                 23
#define _ERR_DSTOP_OVERTIME             24
#define _ERR_RCALL_LONGSEND             25

#ifdef	__cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
//initiate the D320/D640 PCM Voice board
int 	WINAPI DJSys_EnableCard ( const char * configFile, const char * promptVoiceIdxFile );
void	WINAPI DJSys_DisableCard ( void );
void	WINAPI DJSys_PushPlay ( void );

/******************************************************************************\
 *
 *	Functions for Trunk channel
 *
\******************************************************************************/

DWORD  E1_ReadPassword(int pcmID); 

int 	WINAPI DJTrk_GetTotalTrunkNum	( void );
int 	WINAPI DJTrk_GetPcmTrunkNum ( void );

UINT	WINAPI DJTrk_GetWaitTime		( int trunkID );
UINT	WINAPI DJTrk_GetConnectVoiceTime( int trunkID );
UINT	WINAPI DJTrk_GetConnectTime	( int trunkID );
UINT	WINAPI DJTrk_GetDtmfTime		( int trunkID );
DWORD	WINAPI DJTrk_GetTrunkConnectTime( int trunkID );

int 	WINAPI DJTrk_GetPcmID	( int trunkID );

int 	WINAPI DJTrk_GetConnectTrunkID	( int trunkID );
int 	WINAPI DJTrk_GetConnectUserID	( int trunkID );
int 	WINAPI DJTrk_GetListenUserID	( int trunkID );
int 	WINAPI DJTrk_GetListenTrunkID	( int trunkID );
int 	WINAPI DJTrk_GetTrunkPlayID 	( int trunkID );
int 	WINAPI DJTrk_GetTrunkRecordID	( int trunkID );

BOOL	WINAPI DJTrk_CheckReady		( int trunkID );
BOOL	WINAPI DJTrk_CheckConnect 	( int trunkID );
BOOL	WINAPI DJTrk_CheckWait		( int trunkID );
BOOL	WINAPI DJTrk_CheckForwardHangUp( int trunkID );
BOOL	WINAPI DJTrk_CheckTrunkIn		( int trunkID );
BOOL	WINAPI DJTrk_CheckTrunkFree	( int trunkID );
BOOL	WINAPI DJTrk_CheckTrunkEnable	( int trunkID );

BOOL	WINAPI DJTrk_BackwardHangUp	( int trunkID );
BOOL	WINAPI DJTrk_DisableTrunk		( int trunkID );
BOOL	WINAPI DJTrk_EnableTrunk		( int trunkID );

int 	WINAPI DJTrk_GetFreeTrunkIDForDial ( int pcmID );
// modify by H.J.N
BOOL	WINAPI DJTrk_StartDial	( int trunkID,
				const char * szPhoneNum,
				const char * szCallerStr	/* ,		rem by H.J.N
				BYTE	foreKD = MFC_BACK_KD3,
				BYTE	foreKA = MFC_FORE_KA1*/ );
void WINAPI DJTrk_StartDial_SetParam ( BYTE foreKD ,BYTE	foreKA );
// end of add 

BOOL	WINAPI DJTrk_AppendTelNum	( int trunkID, char phoneCode );//ADD
DialStatus WINAPI DJTrk_GetDialStatus	( int trunkID );
//BOOL	trunkCheckDialIsFail	( int trunkID );

BOOL	WINAPI DJTrk_CheckApplyDtmf	( int trunkID ); //these is a dtmf resource link to this trunk
BOOL	WINAPI DJTrk_ApplyDtmf	( int trunkID );
BOOL	WINAPI DJTrk_FreeDtmf( int trunkID );

char *	WINAPI DJTrk_GetMfcCode 		( int trunkID );
char *	WINAPI DJTrk_GetHostCode		( int trunkID );
char *	WINAPI DJTrk_GetDtmfCode		( int trunkID );
int 	WINAPI DJTrk_GetReciveMfcNum	( int trunkID );
int 	WINAPI DJTrk_GetReciveCallerNum( int trunkID );
int 	WINAPI DJTrk_GetReciveDtmfNum	( int trunkID );
char	WINAPI DJTrk_GetFirstDtmfCode	( int trunkID );
char	WINAPI DJTrk_GetLastDtmfCode	( int trunkID );
BOOL	WINAPI DJTrk_InitDtmfBuf		( int trunkID );

BOOL	WINAPI DJTrk_SetTrunkType		( int trunkID, TrunkType type );
int 	WINAPI DJTrk_GetTrunkStatus	( int trunkID );


BYTE	WINAPI DJTrk_GetTrunkKD		( int trunkID );
BYTE	WINAPI DJTrk_GetTrunkKB		( int trunkID );

BYTE	WINAPI DJTrk_GetTrunkForwardKA	( int trunkID );

BYTE	WINAPI DJTrk_GetTrunkBackwardA	( int trunkID );
BYTE	WINAPI DJTrk_GetTrunkForwardMFN( int trunkID );

BYTE	WINAPI DJTrk_GetTrunkForwardDL	( int trunkID );
BYTE	WINAPI DJTrk_GetTrunkBackwardDL( int trunkID );
//BOOL	trunkPutBackDL			( int trunkID, BYTE backDL );

BOOL	WINAPI DJTrk_WaitBackwardA3	( int trunkID );
// modify by H.J.N
BOOL	WINAPI DJTrk_SetTrunkKB			( int trunkID, BYTE backKB/*, BYTE backA=MFC_BACK_A3*/ );
void WINAPI DJTrk_SetTrunkKB_SetParam (BYTE backA);
// end of add

//add by N.C.J, 1999.07.29
BOOL  WINAPI DJTrk_InitDtmfBufNew(int trunkID);
int  WINAPI DJTrk_GetReceiveDtmfNumNew(int trunkID);
char  WINAPI DJTrk_GetDtmfCodeNew(int trunkID);
char  WINAPI DJTrk_GetDtmfHitNew(int trunkID);
//end of add

//1999.8.11
int   WINAPI DJTrk_SendDtmfStr(int trunkID,const char * pcDtmf);
bool   WINAPI DJTrk_CheckDtmfSendEnd(int trunkID);
int   WINAPI DJTrk_PlayPromptStr(int trunkID,const char * pcPromptStr);
bool   WINAPI DJTrk_CheckPlayPromptStrEnd(int trunkID);
//end of add

// modify for new_mode_dtmf, 1999.09.06
#define	TRUNK_DTMF_MODE_NORMAL	0
#define	TRUNK_DTMF_MODE_QUICK	1
#define	TRUNK_DTMF_MODE_SLOW	2

BOOL WINAPI DJTrk_SetDtmfWorkMode ( int trunkID, BYTE byDtmfMode );
//end of add

/* rem by H.J.N, no use
BOOL	trunkSendDtmf(	int trunkID, char dtmfCode );
BOOL	trunkCheckSendDtmfIsEnd( int trunkID );

UINT	trunkGetRecvDialCount	( int trunkID );

BOOL	trunkEnterConversation	( int trunkID, int groupID, BOOL conn=TRUE );
void	trunkExitConversation	( int trunkID );
int 	trunkGetConversationGroupID ( int trunkID );

int 	trunkGetFailReason		( int trunkID );
void	trunkResetFailFalg		( int trunkID );


//the follow function is for debug only.
WORD	dbgTrunkGetFlag			( int trunkID );
*/

/******************************************************************************\
 *
 *	Functions for User channel
 *
\******************************************************************************/
int WINAPI DJUser_GetTotalUserNum		( void );

void WINAPI DJUser_SetPowerON 			( int userID ); 	//default
void WINAPI DJUser_SetPowerOFF			( int userID );
BOOL WINAPI DJUser_CheckPowerStatus	( int userID ); 	//if Power is ON then return TRUE

void WINAPI DJUser_SetUserHalfPower	( int userID );
void WINAPI DJUser_SetUserFullPower	( int userID );

BOOL WINAPI DJUser_RingDetect 		( int userID );
BOOL WINAPI DJUser_StartRing			( int userID );
BOOL WINAPI DJUser_StopRing			( int userID );

BOOL WINAPI DJUser_EnableDialSound 	( int userID ); //default
BOOL WINAPI DJUser_DisableDialSound	( int userID );

BOOL WINAPI DJUser_CheckHookOFF		( int userID );

int  WINAPI DJUser_GetDialNum			( int userID );
char * WINAPI DJUser_GetDialCode 		( int userID );
char WINAPI DJUser_GetFirstDialCode	( int userID );
char WINAPI DJUser_GetLastDialCode		( int userID );
BOOL WINAPI DJUser_InitDialBuf		( int userID );

int WINAPI DJUser_GetConnectTrunkID		( int userID );
int WINAPI DJUser_GetConnectUserID		( int userID );
int WINAPI DJUser_GetListenUserID 	( int userID );
int WINAPI DJUser_GetListenTrunkID	( int userID );
int WINAPI DJUser_GetPlayChannel	( int userID );
int WINAPI DJUser_GetRecordChannel	( int userID );

BOOL WINAPI DJUser_SetSendSoundValue	( int userID, int value );
BOOL WINAPI DJUser_SetReciveSoundValue	( int userID, int value );
int  WINAPI DJUser_GetSendSoundValue	( int userID );
int  WINAPI DJUser_GetReciveSoundValue	( int userID );

UINT WINAPI DJUser_GetPreStopDialToNowTime( int userID );

PstnErr WINAPI DJUser_SearchFreeTrunkAndDial ( int userID,
							int pcmID,
							const char * szPhoneNum,
							const char * szCallerStr );
BOOL WINAPI DJUser_DialByTrunk ( int userID,
						  int trunkID,
						  const char * szPhoneNum,
						  const char * szCallerStr );
int WINAPI DJUser_GetDialTrunkID	( int userID );
DialStatus WINAPI DJUser_GetDialStatus	( int userID );

/* rem by H.J.N, no use
BOOL	userEnterConversation	( int userID, int groupID );
void	userExitConversation	( int userID );
int 	userGetConversationGroupID	( int userID );
*/
//added by N.C.J 1999.6.14
int WINAPI DJUser_PlayFileNew(int userID,char *FileName,DWORD Position,DWORD Length);
void WINAPI DJUser_StopPlayFile(int userID);
int WINAPI DJUser_RecordFileNew(int userID,char *FileName,DWORD Position,DWORD Length);
void WINAPI DJUser_StopRecordFile(int userID);
//end of N.C.J

//add by N.C.J, 1999.7.29
BOOL WINAPI DJUser_InitDialBufNew(int userID);
int WINAPI DJUser_GetDialNumNew(int userID);
char WINAPI DJUser_GetDialCodeNew(int userID);
char WINAPI DJUser_GetDialHitNew(int userID);
//end of N.C.J

//1999.8.11
int   WINAPI DJUser_SendDtmfStr(int userID,const char * pcDtmf);
bool   WINAPI DJUser_CheckDtmfSendEnd(int userID);
int   WINAPI DJUser_PlayPromptStr(int userID,const char * pcPromptStr);
bool   WINAPI DJUser_CheckPlayPromptStrEnd(int userID);
//end of N.C.J

/******************************************************************************\
 *
 *	Functions for Voice channel
 *
\******************************************************************************/
int 	WINAPI DJVoc_GetTotalVoiceChannel( void );

int 	WINAPI DJVoc_VoiceStart	(	int 	voiceChannelID,
						int 	voiceResHandle,
						DWORD	voiceResSize,
						DWORD	voiceResOffset,
						VoiceResourcesType	voiceResType,
						VoiceOperatorType	voiceOpType );
int 	WINAPI DJVoc_PlayNextVoice	(	int 	voiceChannelID,
						int 	voiceResHandle,
						DWORD	voiceResSize,
						DWORD	voiceResOffset );
BOOL	WINAPI DJVoc_FromHeadRePlay	( int voiceID );

/***********************************************************************
BOOL	voicePlayPromptVoice	( int voiceID, const char * voiceName );
BOOL	voiceLoopPlayPromptVoice( int voiceID, const char * voiceName );
BOOL	voicePlayPromptVoice	( int voiceID, int voiceResID );
BOOL	voiceLoopPlayPromptVoice( int voiceID, int voiceResID );
************************************************************************/
BOOL	WINAPI DJVoc_PlayPromptFile		( int voiceID, const char * voiceName );
BOOL	WINAPI DJVoc_LoopPlayPromptFile	( int voiceID, const char * voiceName );
BOOL	WINAPI DJVoc_PlayPromptID		( int voiceID, int voiceResID );
BOOL	WINAPI DJVoc_LoopPlayPromptID	( int voiceID, int voiceResID );


BOOL	WINAPI DJVoc_PauseChannelVoiceOp( int voiceID );
BOOL	WINAPI DJVoc_ContinueChannelVoiceOp( int voiceID );
BOOL	WINAPI DJVoc_CheckChannelVoicePause( int voiceID );

BOOL	WINAPI DJVoc_VoiceStop			( int voiceChannelID );

BOOL	WINAPI DJVoc_CheckVoiceEnd		( int voiceChannelID );
BOOL	WINAPI DJVoc_CheckVoiceChannelOp( int voiceChannelID );

long	WINAPI DJVoc_GetVoiceDataLength ( int voiceChannelID );
int 	WINAPI DJVoc_GetVoiceHandle	( int voiceChannelID );
int 	WINAPI DJVoc_GetVoiceErr		( int voiceID );
VoiceResourcesType WINAPI DJVoc_GetVoiceResourceType( int voiceID );
VoiceOperatorType WINAPI DJVoc_GetVoiceOperateType ( int voiceID );

/**********************************************************
BOOL	voiceFindPromptVoiceRes( const char * voiceName,
							int   & voiceHandle,
							DWORD & voiceSize,
							DWORD & voiceOffset );

BOOL	voiceFindPromptVoiceRes( int voiceResID,
							int   & voiceHandle,
							DWORD & voiceSize,
							DWORD & voiceOffset );
**********************************************************/
BOOL	WINAPI DJVoc_GetPromptInfoByName( const char * voiceName,
							int   * voiceHandle,
							DWORD * voiceSize,
							DWORD * voiceOffset );

BOOL	WINAPI DJVoc_GetPromptInfoByHandle( int voiceResID,
							int   * voiceHandle,
							DWORD * voiceSize,
							DWORD * voiceOffset );

int 	WINAPI DJVoc_SearchFreeVoiceChannelForPlay 	( void );
int 	WINAPI DJVoc_SearchFreeVoiceChannelForRecord	( void );

// new add for multi-card, hjn_new
int 	WINAPI DJVoc_SFVC_ForPlay 	( int trunkID );
int 	WINAPI DJVoc_SFVC_ForRecord	( int trunkID );
// new add for multi-card

// new add for E1-PCI
int 	WINAPI DJVoc_SFVC_ForPlay_New 	( int trunkID, int IsForTrunk );
int 	WINAPI DJVoc_SFVC_ForRecord_New	( int trunkID, int IsForTrunk );
// end 

int 	WINAPI DJVoc_GetRecordTrunkID ( int voiceChannelID );
int 	WINAPI DJVoc_GetRecordUserID	( int voiceChannelID );

/******************************************************************************\
 *
 *	Functions for Connect Operate
 *
\******************************************************************************/
BOOL	WINAPI DJExg_SetListenUserToUser	 ( int trgUserID, int srcUserID );
BOOL	WINAPI DJExg_ClearListenUserFromUser( int trgUserID );
BOOL	WINAPI DJExg_SetListenUserToTrunk 	( int userID, int trunkID );
BOOL	WINAPI DJExg_ClearListenUserFromTrunk( int userID );

BOOL	WINAPI DJExg_SetListenTrunkToUser 	( int trunkID, int userID );
BOOL	WINAPI DJExg_ClearListenTrunkFromUser( int trunkID );
BOOL	WINAPI DJExg_SetListenTrunkToTrunk	( int trgTrunkID, int srcTrunkID );
BOOL	WINAPI DJExg_ClearListenTrunkFromTrunk(int trgTrunkID );

BOOL	WINAPI DJExg_SetLinkTrunkAndUser				( int trunkID, int userID );
BOOL	WINAPI DJExg_ClearLinkTrunkAndUserByTrunk	( int trunkID );
BOOL	WINAPI DJExg_ClearLinkTrunkAndUserByUser	( int userID );

BOOL	WINAPI DJExg_SetLinkTrunkAndTrunk		( int trgTrunkID, int srcTrunkID );
BOOL	WINAPI DJExg_ClearLinkTrunkAndTrunk ( int trunkID );
BOOL	WINAPI DJExg_LinkUserAndUser		( int trgUserID, int srcUserID );
BOOL	WINAPI DJExg_ClearLinkUserAndUser	( int userID );

// modify by H.J.N
BOOL	WINAPI DJExg_SetLinkPlayVoiceToTrunk	( int trunkID, int voiceID/*rem by H.J.N*//*, BOOL conn=TRUE*/ );
void WINAPI DJExg_VoiceToTrunk_SetParam (BOOL conn);
// end of add

BOOL	WINAPI DJExg_ClearLinkPlayVoiceFromTrunk(int trunkID );
BOOL	WINAPI DJExg_SetLinkPlayVoiceToUser 	( int userID, int voiceID );
BOOL	WINAPI DJExg_ClearLinkPlayVoiceFromUser( int userID );

BOOL	WINAPI DJExg_SetLinkRecordVoiceToUser 	( int userID, int voiceID );
BOOL	WINAPI DJExg_ClearLinkRecordVoiceFromUser( int userID );
BOOL	WINAPI DJExg_SetLinkRecordVoiceToTrunk	( int trunkID, int voiceID );
BOOL	WINAPI DJExg_ClearLinkRecordVoiceFromTrunk(int trunkID );

/* rem by H.J.N, no use
BOOL	coonConnectSignalVoiceToUser( int userID, int signalVoiceStream=11, int signalVoiceChannel=22);
BOOL	coonBreakSignalVoiceToUser( int userID, int signalVoiceStream=11, int signalVoiceChannel=22 );
BOOL	coonConnectSignalVoiceToTrunk( int trunkID, int signalVoiceStream=11, int signalVoiceChannel=22);
BOOL	coonBreakSignalVoiceToTrunk( int trunkID, int signalVoiceStream=11, int signalVoiceChannel=22 );
*/

/////////////////////////////////////////////////////////////////////////////
int 	WINAPI DJSys_GetPcmNum		( void );

#define BASE 		500
#define CURRENTIN   BASE
#define CURRENTOUT	BASE + 1
#define LASTIN		BASE + 2
#define LASTOUT		BASE + 3
#define TOTALIN		BASE + 4
#define TOTALOUT	BASE + 5

UINT WINAPI DJSys_GetImpExpNum(int ImpExpType);
DWORD WINAPI DJSys_GetTotalImpExpNum(int ImpExpType);
UINT WINAPI DJSys_GetPcmImpExpNum(int pcmID,int ImpExpType);
DWORD WINAPI DJSys_GetPcmTotalImpExpNum(int pcmID, int ImpExpType);

BYTE	WINAPI DJSys_GetPcmStatus	( int pcmID );
void	WINAPI DJSys_StopWarning 	( void );
void	WINAPI DJSys_EnableWarn	( void );
void	WINAPI DJSys_DisableWarn 	( void );

/*****************************************************************************
void	pstnGetRunTime		( DWORD & hours, BYTE & minutes, BYTE & seconds );
void	pstnGetCurrentTime	( BYTE & hours, BYTE & minutes, BYTE & seconds );
******************************************************************************/
void	WINAPI DJSys_GetSysRunTime		( DWORD * hours, BYTE * minutes, BYTE * seconds );
void	WINAPI DJSys_GetSysTime	( BYTE * hours, BYTE * minutes, BYTE * seconds );
void	WINAPI DJSys_SetSysTime 	( BYTE hours, BYTE minutes, BYTE seconds );

PstnErr WINAPI DJSys_GetErrCode( void );

int 	WINAPI DJSys_FreeMfcNum( void );
int 	WINAPI DJSys_FreePlayVoiceNum( void );
int 	WINAPI DJSys_FreeDtmfNum( void );

// hjn_new
int 	WINAPI DJSys_FreeMfcNumNew( int trunkID );
int 	WINAPI DJSys_GetCardType(void);
// end of hjn_new

// add by H.J.N
char  *		WINAPI DJSys_GetInstallDir( void );

//void	DJSys_EnableAutoApplyDtmf( void );
//void	pstnDisableAutoApplyDtmf( void );
#define DISABLEDTMF 0
#define ENABLEDTMF  1
void WINAPI DJSys_AutoApplyDtmf(int DtmfMode);

BOOL WINAPI DJSys_IsAutoApplyDtmf( void );

void WINAPI DJSys_EnableAutoKB( void );
void WINAPI DJSys_DisableAutoKB( void );

/* rem by H.J.N, no use
void	pstnGetDialVoiceChannel( int * stream, int * channel );

int 	pstnSetConversationNum( int num = 3 );
int 	pstnGetConversationGroupNum( void );
int     pstnGetConvGroupFreeMemberNum( int groupID );   //how many free member in this group
int     pstnGetConverGroupRecordVoiceID( int groupID );

void	pstnEnableSendBackDLForForeHangOff( void );
void	pstnDisableSendBackDLForForeHangOff( void );

void    pstnSetHangOffCount( BYTE count );
*/

//add by N.C.J 1999.8.11
bool WINAPI DJSys_EnableDtmfSend();
//end of add

//--------------------------------------------------------
//Add by Wang Guoli 1998.3.17
BOOL WINAPI DJVoc_PlayFile(int trunkID,char *FileName);
void WINAPI DJVoc_StopPlayFile(int trunkID);
void WINAPI DJVoc_InitIndexPlayFile(int trunkID);
BOOL WINAPI DJVoc_AddIndexPlayFile(int trunkID, char *FileName);
// add for zyb
BOOL WINAPI DJVoc_AddIndexPlayFile_ZYB(int trunkID, char *FileName, int DNum );

BOOL WINAPI DJVoc_StartIndexPlayFile(int trunkID);
void WINAPI DJVoc_StopIndexPlayFile(int trunkID);
BOOL WINAPI DJVoc_CheckIndexPlayEnd(int trunkID);
BOOL WINAPI DJVoc_RecordFile(int trunkID,char *FileName,DWORD RecordLen);
void WINAPI DJVoc_StopRecordFile(int trunkID);
void WINAPI DJTrk_GetMfcCodeA(int trunkID,LPSTR MfcCode) ;
void WINAPI DJTrk_GetHostCodeA(int trunkID, char *HostCode );
void	WINAPI DJTrk_GetDtmfCodeA( int trunkID, char *DtmfCode);
//added by N.C.J 1999.6.14
int WINAPI DJVoc_RecordFileNew(int trunkID,char *FileName,DWORD Position,DWORD Length);
int WINAPI DJVoc_PlayFileNew(int trunkID,char *FileName,DWORD Position,DWORD Length);

//--------------------------------------------------------
// add for WHB
BOOL WINAPI DJTrk_SetDLLowBit ( int trunkID, BYTE byLowBit );

// add for WHB_new,1999.08.12
void WINAPI DJSys_DialStatusAfterKB ( int mode );

// add for lxg
int WINAPI DJVoc_StopPlayMemory ( int voiceID );
int WINAPI DJVoc_PlayMemory ( int voiceID, char *p, WORD wPlayLen);


// add for PengHu, 1999.11.23
BOOL WINAPI DJTrk_SetMoreInfo ( int trunkID, DWORD dwInfoType, int iInfoVal );

//add by N.C.J 
bool WINAPI DJTrk_RealConnect(int trunkID);
//end of add

#define CHTYPE_USER     0
#define CHTYPE_TRUNK    1
#define CHTYPE_EMPTY    2
#define CHTYPE_RECORD   3
#define CHTYPE_CS		4
#define CHTYPE_EM_CTRL	5
#define CHTYPE_EM_4VOC	6
#define CHTYPE_EM_2VOC	7

int WINAPI DJUser_CheckChType(int userID);

// v2-modify
void WINAPI DJTrk_ResetSigCheck (int trunkID);
void WINAPI DJTrk_ReadSigBuf ( int trunkID, int *piNowPoint, int *piCount, BYTE *SigBuf );
int WINAPI DJTrk_IsFaxTone ( int trunkID, int CheckLen, BYTE cbSensLevel );

int	WINAPI DJVR_SetRefreshSize ( WORD wSize );
int WINAPI DJVR_StartRecord ( int trunkID );
int WINAPI DJVR_StopRecord ( int trunkID );
int	WINAPI	DJVR_GetRecordData ( int trunkID, char *pBuffer );

// end

//for-tts
int WINAPI DJTrk_RealPlayMemory ( int trunkID,char *PlayBuf, DWORD dwLength );
int	WINAPI	IsNowSupportTTS(void);

// add for-FSK-E1, 2001.05.17
int	WINAPI E1FSK_InitForFSK(void);
void WINAPI E1FSK_ResetCallerIDBuffer(int trunkID);
WORD WINAPI E1FSK_GetFSKRawStr (int trunkID, LPSTR FSKRawStr);
// end

// SS1、DSS1、SS7混用
#define		PCM_WORK_MODE_NONE	0
#define		PCM_WORK_MODE_SS1	1
#define		PCM_WORK_MODE_DSS1	2
#define		PCM_WORK_MODE_SS7	3

BYTE WINAPI	DJSys_GetPcmMode	( int pcmID );
int	WINAPI DJSys_IsUseThisSignal ( BYTE cbCheckPcmMode );
// end

void WINAPI	DJSys_UserCall_TC08A32 	( int	flag );

//HJN ADD FOR DFM 2001.9.11
int WINAPI DJTrk_GetTimeSlot(int trunkID);
int WINAPI DJTrk_ConnectFromTS(int trunkID, int iTS);
int WINAPI DJTrk_DisconnectTS(int trunkID);
//HJN end ADD FOR DFM 2001.9.11

// add for XinTai, 2001.11.20
int	WINAPI XT_FetchOneErr ( int	*pVocID, DWORD	*pdwErrCode );

// 2002.11.19, add for "可调节的、时间准确的DTMF发送" 
int		WINAPI DJSys_SetSendPara ( int ToneLen, int SilenceLen );
int		WINAPI DJTrk_NewSendDtmfBuf ( int trunkID, LPSTR DialNum);
// end of add

//  2002.11.21, add for 模拟通道连接FAX
int WINAPI DJFax_User_SetLink ( WORD wFaxChnl, WORD userChnl );
int WINAPI DJFax_User_ClearLink ( WORD wFaxChnl, WORD userChnl );
int WINAPI DJFax_User_GetFaxChnl (WORD userChnl);
int WINAPI DJFax_User_GetUserChnl (WORD wFaxChnl);
// end of add

// 2003.03.14 add for DnlD function
bool		WINAPI DJSys_DownLoad(int SigMode, int WinHide);

// 2003.04.29 add for SHJL
int		WINAPI DJExg_ClearLinkTrunkAndUserByUser_SHJL	( int userID );

// 2004.02.05 add for 得到一号信令的MFC原始串值
#define	MAX_RAW_MFC_LEN			64

int		WINAPI DJTrk_GetRcv_Raw_MFC ( int trunkID, BYTE *pRawMFC );
int		WINAPI DJTrk_GetSend_Raw_MFC ( int trunkID, BYTE *pRawMFC );
// end of add

// add in 2004.10.10，可以在tce1_32.dll中调用模拟卡tc08a32.dll中的部分函数
void  WINAPI DJ_Tc08a32_ResetCallerIDBuffer(WORD wChnlNo);
WORD  WINAPI DJ_Tc08a32_GetCallerIDRawStr(WORD wChnlNo, LPSTR IDRawStr);
WORD  WINAPI DJ_Tc08a32_GetCallerIDStr(WORD wChnlNo, LPSTR IDStr);
// end of add in 2004.10.10

// add in 2005.01.05，可以将trunk的输出设定为某个固定的值
int		WINAPI DJTrk_SetOutputConst ( int trunkID, BYTE cbOutputVal );
// end of add in 2005.01.05，

// add in 2005.01.10，

int		WINAPI	DJSys_Get_iEnableWaveFormat ( void );			// 可以得到iEnableWaveFormat的值(即：TCE1-32.INI中WaveFormat的值)
void	WINAPI	DJSys_WF_MayBe_AddHead ( HANDLE  hfFile );		// 填写Wave文件的头
void	WINAPI	DJSys_WF_MayBe_ReFillHead ( HANDLE  hfFile );	// 填写Wave文件的头

int		WINAPI	DJVoc_GetTimeSlot(int voiceID);
int		WINAPI	DJVoc_ConnectFromTS(int voiceID, int iTS);
int		WINAPI	DJVoc_DisconnectTS(int voiceID);

// end of add in 2005.01.10，

// add for support Volume Adjust 2005.01.27
#define	VOL_ADJUST_RECORD		0
#define	VOL_ADJUST_PLAY			1

int WINAPI DJVoc_AdjustVocVol ( int voiceID, int iMode, int iVolAdjust );
// end of add 2005.01.27

#ifdef	__cplusplus
}
#endif

#endif


