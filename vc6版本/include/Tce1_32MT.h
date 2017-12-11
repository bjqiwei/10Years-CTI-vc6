#if !defined(_TCE132_MT_H_)
#define _TCE132_MT_H_

#include "tce1_32.h"

//通道的类型，来自TC08a32.h
#define CHTYPE_USER     0
#define CHTYPE_TRUNK    1
#define CHTYPE_EMPTY    2

//DTMF函数定义
#define NODTMF    -1
#define DTMF_CODE_0		10
#define DTMF_CODE_1		1
#define DTMF_CODE_2		2
#define DTMF_CODE_3		3
#define DTMF_CODE_4		4
#define DTMF_CODE_5		5
#define DTMF_CODE_6		6
#define DTMF_CODE_7		7
#define DTMF_CODE_8		8
#define DTMF_CODE_9		9
#define DTMF_CODE_STAR	11
#define DTMF_CODE_SHARP	12
#define DTMF_CODE_A		13
#define DTMF_CODE_B		14
#define DTMF_CODE_C		15
#define DTMF_CODE_D		16

#define EV_PLAY_END		0X0001
#define EV_RECORD_END	0x0002
#define EV_DTMF			0x0004
#define EV_BUSY			0x0008
#define EV_USER_HANGUP	0x0010

#define	MAX_FAX_CARD_NUM	8
#define MAX_FAX_CHANNEL_NUM	(4*MAX_FAX_CARD_NUM)
#define MAX_VOICE_CHANNEL_NUM 128
//#define	BUFFER_LEN	2048
#define HIGH_RESOLUTION	1
#define LOW_RESOLUTION	0

#define DISABLEDTMF 0
#define ENABLEDTMF  1

typedef struct {
	WORD	wCardNo;
	WORD	wMemBaseAddr;
	WORD	wIOAddr[MAX_FAX_CARD_NUM];
} FAX_SYS_INFO;




#if defined(__cplusplus)
extern "C"{
#endif 
//...............初始化部分............................
int	WINAPI t_DJSys_EnableCard ( const char * configFile, const char * promptVoiceIdxFile);
void WINAPI t_DJSys_DisableCard(void);
void WINAPI t_DJSys_AutoApplyDtmf(int DtmfMode);


///*//////////////////////////////////////////////////////////////
/*add from chinesechar*/
void	WINAPI	t_DJSys_DisableCard	(	void	);
int	WINAPI	t_DJTrk_GetTotalTrunkNum	(	void	);
int	WINAPI	t_DJTrk_GetPcmTrunkNum	(	void	);
UINT	WINAPI	t_DJTrk_GetWaitTime	(	int	trunkID	);
UINT	WINAPI	t_DJTrk_GetConnectVoiceTime	(	int	trunkID	);
UINT	WINAPI	t_DJTrk_GetConnectTime	(	int	trunkID	);
UINT	WINAPI	t_DJTrk_GetDtmfTime	(	int	trunkID	);
DWORD	WINAPI	t_DJTrk_GetTrunkConnectTime	(	int	trunkID	);
int	WINAPI	t_DJTrk_GetPcmID	(	int	trunkID	);
int	WINAPI	t_DJTrk_GetConnectTrunkID	(	int	trunkID	);
int	WINAPI	t_DJTrk_GetConnectUserID	(	int	trunkID	);
int	WINAPI	t_DJTrk_GetListenUserID	(	int	trunkID	);
int	WINAPI	t_DJTrk_GetListenTrunkID	(	int	trunkID	);
int	WINAPI	t_DJTrk_GetTrunkPlayID	(	int	trunkID	);
int	WINAPI	t_DJTrk_GetTrunkRecordID	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_CheckReady	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_CheckConnect	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_CheckWait	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_CheckForwardHangUp	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_CheckTrunkIn	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_CheckTrunkFree	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_CheckTrunkEnable	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_BackwardHangUp	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_DisableTrunk	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_EnableTrunk	(	int	trunkID	);
int	WINAPI	t_DJTrk_GetFreeTrunkIDForDial	(	int	pcmID);
BOOL	WINAPI	t_DJTrk_StartDial	(	int	trunkID,	const char * szPhoneNum,const char * szCallerStr);
void	WINAPI	t_DJTrk_StartDial_SetParam	(	BYTE	foreKD,	BYTE	foreKA	);
BOOL	WINAPI	t_DJTrk_AppendTelNum	(	int	trunkID,	char	phoneCode	);//ADD
DialStatus	WINAPI	t_DJTrk_GetDialStatus	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_CheckApplyDtmf	(	int	trunkID	);	//these	is	a	dtmf	resource	link	to	this	trunk
BOOL	WINAPI	t_DJTrk_ApplyDtmf	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_FreeDtmf	(	int	trunkID	);
char*	WINAPI	t_DJTrk_GetMfcCode	(	int	trunkID	);
char*	WINAPI	t_DJTrk_GetHostCode	(	int	trunkID	);
char*	WINAPI	t_DJTrk_GetDtmfCode	(	int	trunkID	);
int	WINAPI	t_DJTrk_GetReciveMfcNum	(	int	trunkID	);
int	WINAPI	t_DJTrk_GetReciveCallerNum	(	int	trunkID	);
int	WINAPI	t_DJTrk_GetReciveDtmfNum	(	int	trunkID	);
int	WINAPI	t_DJTrk_GetReciveDtmfNumNew	(	int	trunkID	);
char	WINAPI	t_DJTrk_GetFirstDtmfCode	(	int	trunkID	);
char	WINAPI	t_DJTrk_GetLastDtmfCode	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_InitDtmfBuf	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_SetTrunkType	(	int	trunkID,	TrunkType	type	);
int	WINAPI	t_DJTrk_GetTrunkStatus	(	int	trunkID	);
BYTE	WINAPI	t_DJTrk_GetTrunkKD	(	int	trunkID	);
BYTE	WINAPI	t_DJTrk_GetTrunkKB	(	int	trunkID	);
BYTE	WINAPI	t_DJTrk_GetTrunkForwardKA	(	int	trunkID	);
BYTE	WINAPI	t_DJTrk_GetTrunkBackwardA	(	int	trunkID	);
BYTE	WINAPI	t_DJTrk_GetTrunkForwardMFN	(	int	trunkID	);
BYTE	WINAPI	t_DJTrk_GetTrunkForwardDL	(	int	trunkID	);
BYTE	WINAPI	t_DJTrk_GetTrunkBackwardDL	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_WaitBackwardA3	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_SetTrunkKB	(	int	trunkID,	BYTE	backKB/*,	BYTE	backA=MFC_BACK_A3*/	);
void	WINAPI	t_DJTrk_SetTrunkKB_SetParam	(	BYTE	backA	);
BOOL	WINAPI	t_DJTrk_InitDtmfBufNew	(	int	trunkID	);
int	WINAPI	t_DJTrk_GetReceiveDtmfNumNew	(	int	trunkID	);
char	WINAPI	t_DJTrk_GetDtmfCodeNew	(	int	trunkID	);
char	WINAPI	t_DJTrk_GetDtmfHitNew	(	int	trunkID	);
int	WINAPI	t_DJTrk_SendDtmfStr	(	int	trunkID,	const	char*	pcDtmf	);
bool	WINAPI	t_DJTrk_CheckDtmfSendEnd	(	int	trunkID	);
int	WINAPI	t_DJTrk_PlayPromptStr	(	int	trunkID,	const	char*	pcPromptStr	);
bool	WINAPI	t_DJTrk_CheckPlayPromptStrEnd	(	int	trunkID	);
BOOL	WINAPI	t_DJTrk_SetDtmfWorkMode	(	int	trunkID,	BYTE	byDtmfMode	);

int	WINAPI	t_DJUser_GetTotalUserNum	(	void	);
void	WINAPI	t_DJUser_SetPowerON		(	int	userID	);	//default
void	WINAPI	t_DJUser_SetPowerOFF	(	int	userID	);
BOOL	WINAPI	t_DJUser_CheckPowerStatus	(	int	userID	);	//if	Power	is	ON	then	return	TRUE
void	WINAPI	t_DJUser_SetUserHalfPower	(	int	userID	);
void	WINAPI	t_DJUser_SetUserFullPower	(	int	userID	);
BOOL	WINAPI	t_DJUser_RingDetect	(	int	userID	);
BOOL	WINAPI	t_DJUser_StartRing	(	int	userID	);
BOOL	WINAPI	t_DJUser_StopRing	(	int	userID	);
BOOL	WINAPI	t_DJUser_EnableDialSound	(	int	userID	);	//default
BOOL	WINAPI	t_DJUser_DisableDialSound	(	int	userID	);
BOOL	WINAPI	t_DJUser_CheckHookOFF	(	int	userID	);
int	WINAPI	t_DJUser_GetDialNum	(	int	userID	);
char*	WINAPI	t_DJUser_GetDialCode	(	int	userID	);
char	WINAPI	t_DJUser_GetFirstDialCode	(	int	userID	);
char	WINAPI	t_DJUser_GetLastDialCode	(	int	userID	);
BOOL	WINAPI	t_DJUser_InitDialBuf	(	int	userID	);
int	WINAPI	t_DJUser_GetConnectTrunkID	(	int	userID	);
int	WINAPI	t_DJUser_GetConnectUserID	(	int	userID	);
int	WINAPI	t_DJUser_GetListenUserID	(	int	userID	);
int	WINAPI	t_DJUser_GetListenTrunkID	(	int	userID	);
int	WINAPI	t_DJUser_GetPlayChannel	(	int	userID	);
int	WINAPI	t_DJUser_GetRecordChannel	(	int	userID	);
BOOL	WINAPI	t_DJUser_SetSendSoundValue	(	int	userID,	int	value	);
BOOL	WINAPI	t_DJUser_SetReciveSoundValue	(	int	userID,	int	value	);
int	WINAPI	t_DJUser_GetSendSoundValue	(	int	userID	);
int	WINAPI	t_DJUser_GetReciveSoundValue	(	int	userID	);
UINT	WINAPI	t_DJUser_GetPreStopDialToNowTime	(	int	userID	);
PstnErr	WINAPI	t_DJUser_SearchFreeTrunkAndDial	(	int	userID,	int pcmID,const char * szPhoneNum,const char * szCallerStr );
BOOL	WINAPI	t_DJUser_DialByTrunk	(	int	userID,	int trunkID,const char * szPhoneNum,const char * szCallerStr );
int	WINAPI	t_DJUser_GetDialTrunkID	(	int	userID	);
DialStatus	WINAPI	t_DJUser_GetDialStatus	(	int	userID	);
int	WINAPI	t_DJUser_PlayFileNew	(	int	userID,	char	*FileName,	DWORD	Position,	DWORD	Length	);
void	WINAPI	t_DJUser_StopPlayFile	(	int	userID	);
int	WINAPI	t_DJUser_RecordFileNew	(	int	userID,	char	*FileName,	DWORD	Position,	DWORD	Length	);
void	WINAPI	t_DJUser_StopRecordFile	(	int	userID	);
BOOL	WINAPI	t_DJUser_InitDialBufNew	(	int	userID	);
int	WINAPI	t_DJUser_GetDialNumNew	(	int	userID	);
char	WINAPI	t_DJUser_GetDialCodeNew	(	int	userID	);
char	WINAPI	t_DJUser_GetDialHitNew	(	int	userID	);
int	WINAPI	t_DJUser_SendDtmfStr	(	int	userID,	const	char*	pcDtmf	);
bool	WINAPI	t_DJUser_CheckDtmfSendEnd	(	int	userID	);
int	WINAPI	t_DJUser_PlayPromptStr	(	int	userID,	const	char*	pcPromptStr	);
bool	WINAPI	t_DJUser_CheckPlayPromptStrEnd	(	int	userID	);
////*/
int	WINAPI	t_DJVoc_GetTotalVoiceChannel	(	void	);
int	WINAPI	t_DJVoc_VoiceStart	(	int iVoiceID, int 	voiceResHandle,DWORD	voiceResSize,DWORD	voiceResOffset,VoiceResourcesType	voiceResType,VoiceOperatorType	voiceOpType );
int	WINAPI	t_DJVoc_PlayNextVoice	(	int	voiceID,	int 	voiceResHandle,DWORD	voiceResSize,DWORD	voiceResOffset );
BOOL	WINAPI	t_DJVoc_FromHeadRePlay	(	int	voiceID	);
int	WINAPI	t_DJVoc_PlayPromptFile	( int	voiceID,	const	char*	voiceName	);
int	WINAPI	t_DJVoc_LoopPlayPromptFile	(int	voiceID,	const	char*	voiceName	);
BOOL	WINAPI	t_DJVoc_PlayPromptID	(	int	voiceID,	int	voiceResID	);
BOOL	WINAPI	t_DJVoc_LoopPlayPromptID	(	int	voiceID,	int	voiceResID	);
BOOL	WINAPI	t_DJVoc_PauseChannelVoiceOp	(	int	voiceID	);
BOOL	WINAPI	t_DJVoc_ContinueChannelVoiceOp	(	int	voiceID	);
BOOL	WINAPI	t_DJVoc_CheckChannelVoicePause	(	int	voiceID	);
BOOL	WINAPI	t_DJVoc_VoiceStop	(	int	voiceID	);
BOOL	WINAPI	t_DJVoc_CheckVoiceEnd	(	int	voiceID	);
BOOL	WINAPI	t_DJVoc_CheckVoiceChannelOp	(	int	voiceID	);
long	WINAPI	t_DJVoc_GetVoiceDataLength	(	int	voiceID	);
int	WINAPI	t_DJVoc_GetVoiceHandle	(	int	voiceID	);
int	WINAPI	t_DJVoc_GetVoiceErr	(	int	voiceID	);
VoiceResourcesType	WINAPI	t_DJVoc_GetVoiceResourceType	(	int	voiceID	);
VoiceOperatorType	WINAPI	t_DJVoc_GetVoiceOperateType	(	int	voiceID	);
BOOL	WINAPI	t_DJVoc_GetPromptInfoByName	(	const	char*	voiceName,	int   * voiceHandle,DWORD * voiceSize,DWORD * voiceOffset );
BOOL	WINAPI	t_DJVoc_GetPromptInfoByHandle	(	int	voiceResID,	int   * voiceHandle,DWORD * voiceSize,DWORD * voiceOffset );
int	WINAPI	t_DJVoc_SearchFreeVoiceChannelForPlay	(	void	);
int	WINAPI	t_DJVoc_SearchFreeVoiceChannelForRecord	(	void	);
int	WINAPI	t_DJVoc_SFVC_ForPlay	(	int	trunkID	);
int	WINAPI	t_DJVoc_SFVC_ForRecord	(	int	trunkID	);
int	WINAPI	t_DJVoc_GetRecordTrunkID	(	int	voiceID	);
int	WINAPI	t_DJVoc_GetRecordUserID	(	int	voiceID	);
BOOL	WINAPI	t_DJExg_SetListenUserToUser	(	int	trgUserID,	int	srcUserID	);
BOOL	WINAPI	t_DJExg_ClearListenUserFromUser	(	int	trgUserID	);
BOOL	WINAPI	t_DJExg_SetListenUserToTrunk	(	int	userID,	int	trunkID	);
BOOL	WINAPI	t_DJExg_ClearListenUserFromTrunk	(	int	userID	);
BOOL	WINAPI	t_DJExg_SetListenTrunkToUser	(	int	trunkID,	int	userID	);
BOOL	WINAPI	t_DJExg_ClearListenTrunkFromUser	(	int	trunkID	);
BOOL	WINAPI	t_DJExg_SetListenTrunkToTrunk	(	int	trgTrunkID,	int	srcTrunkID	);
BOOL	WINAPI	t_DJExg_ClearListenTrunkFromTrunk	(	int	trgTrunkID	);
BOOL	WINAPI	t_DJExg_SetLinkTrunkAndUser		(	int	trunkID,	int	userID	);
BOOL	WINAPI	t_DJExg_ClearLinkTrunkAndUserByTrunk	(	int	trunkID	);
BOOL	WINAPI	t_DJExg_ClearLinkTrunkAndUserByUser	(	int	userID	);
BOOL	WINAPI	t_DJExg_SetLinkTrunkAndTrunk	(	int	trgTrunkID,	int	srcTrunkID	);
BOOL	WINAPI	t_DJExg_ClearLinkTrunkAndTrunk	(	int	trunkID	);
BOOL	WINAPI	t_DJExg_LinkUserAndUser	(	int	trgUserID,	int	srcUserID	);
BOOL	WINAPI	t_DJExg_ClearLinkUserAndUser	(	int	userID	);
BOOL	WINAPI	t_DJExg_SetLinkPlayVoiceToTrunk	(	int	trunkID,	int	voiceID/*rem	by	H.J.N*//*,	BOOL	conn=TRUE*/	);
void	WINAPI	t_DJExg_VoiceToTrunk_SetParam	(	BOOL	conn	);
BOOL	WINAPI	t_DJExg_ClearLinkPlayVoiceFromTrunk	(	int	trunkID	);
BOOL	WINAPI	t_DJExg_SetLinkPlayVoiceToUser	(	int	userID,	int	voiceID	);
BOOL	WINAPI	t_DJExg_ClearLinkPlayVoiceFromUser	(	int	userID	);
BOOL	WINAPI	t_DJExg_SetLinkRecordVoiceToUser	(	int	userID,	int	voiceID	);
BOOL	WINAPI	t_DJExg_ClearLinkRecordVoiceFromUser	(	int	userID	);
BOOL	WINAPI	t_DJExg_SetLinkRecordVoiceToTrunk	(	int	trunkID,	int	voiceID	);
BOOL	WINAPI	t_DJExg_ClearLinkRecordVoiceFromTrunk	(	int	trunkID	);
int	WINAPI	t_DJSys_GetPcmNum	(	void	);
UINT	WINAPI	t_DJSys_GetImpExpNum	(	int	ImpExpType	);
DWORD	WINAPI	t_DJSys_GetTotalImpExpNum	(	int	ImpExpType	);
UINT	WINAPI	t_DJSys_GetPcmImpExpNum	(	int	pcmID,	int	ImpExpType	);
DWORD	WINAPI	t_DJSys_GetPcmTotalImpExpNum	(	int	pcmID,	int	ImpExpType	);
BYTE	WINAPI	t_DJSys_GetPcmStatus	(	int	pcmID	);
void	WINAPI	t_DJSys_StopWarning	(	void	);
void	WINAPI	t_DJSys_EnableWarn	(	void	);
void	WINAPI	t_DJSys_DisableWarn	(	void	);
void	WINAPI	t_DJSys_GetSysRunTime	(	DWORD*	hours,	BYTE*	minutes,	BYTE*	seconds	);
void	WINAPI	t_DJSys_GetSysTime	(	BYTE*	hours,	BYTE*	minutes,	BYTE*	seconds	);
void	WINAPI	t_DJSys_SetSysTime	(	BYTE	hours,	BYTE	minutes,	BYTE	seconds	);
PstnErr	WINAPI	t_DJSys_GetErrCode	(	void	);
int	WINAPI	t_DJSys_FreeMfcNum	(	void	);
int	WINAPI	t_DJSys_FreePlayVoiceNum	(	void	);
int	WINAPI	t_DJSys_FreeDtmfNum	(	void	);
int	WINAPI	t_DJSys_FreeMfcNumNew	(	int	trunkID	);
int	WINAPI	t_DJSys_GetCardType	(	void	);
char*	WINAPI	t_DJSys_GetInstallDir	(	void	);
void	WINAPI	t_DJSys_AutoApplyDtmf	(	int	DtmfMode	);
BOOL	WINAPI	t_DJSys_IsAutoApplyDtmf	(	void	);
void	WINAPI	t_DJSys_EnableAutoKB	(	void	);
void	WINAPI	t_DJSys_DisableAutoKB	(	void	);
bool	WINAPI	t_DJSys_EnableDtmfSend	(	);
BOOL	WINAPI	t_DJVoc_PlayFile	(	int	trunkID,	char	*FileName	);
void	WINAPI	t_DJVoc_StopPlayFile	(	int	trunkID	);
void	WINAPI	t_DJVoc_InitIndexPlayFile	(	int	trunkID	);
BOOL	WINAPI	t_DJVoc_AddIndexPlayFile	(	int	trunkID,	char	*FileName	);
BOOL	WINAPI	t_DJVoc_StartIndexPlayFile	(	int	trunkID	);
void	WINAPI	t_DJVoc_StopIndexPlayFile	(	int	trunkID	);
BOOL	WINAPI	t_DJVoc_CheckIndexPlayEnd	(	int	trunkID	);
BOOL	WINAPI	t_DJVoc_RecordFile	(	int	trunkID,	char	*FileName,	DWORD	RecordLen	);
void	WINAPI	t_DJVoc_StopRecordFile	(	int	trunkID	);
void	WINAPI	t_DJTrk_GetMfcCodeA	(	int	trunkID,	LPSTR	MfcCode	)	;
void	WINAPI	t_DJTrk_GetHostCodeA	(	int	trunkID,	char	*HostCode	);
void	WINAPI	t_DJTrk_GetDtmfCodeA	(	int	trunkID,	char	*DtmfCode	);
int	WINAPI	t_DJVoc_RecordFileNew	(	int	trunkID,	char	*FileName,	DWORD	Position,	DWORD	Length	);
int	WINAPI	t_DJVoc_PlayFileNew	(	int	trunkID,	char	*FileName,	DWORD	Position,	DWORD	Length	);
BOOL	WINAPI	t_DJTrk_SetDLLowBit	(	int	trunkID,	BYTE	byLowBit	);
void	WINAPI	t_DJSys_DialStatusAfterKB	(	int	mode	);
int	WINAPI	t_DJVoc_StopPlayMemory	(	int	voiceID	);
int	WINAPI	t_DJVoc_PlayMemory	(	int	voiceID, char	*p,	WORD	wPlayLen	);
/////////////////////////////////////

//................传真函数......................................................
//初始化函数
int   WINAPI t_DJFax_DriverReady(WORD wBuffSize);
void  WINAPI t_DJFax_DisableCard(void);
void  WINAPI t_DJFax_GetSysInfo(FAX_SYS_INFO far* TmpInfo);
int   WINAPI t_DJFax_GetTotalFaxChnl(void);

//自环的连通函数
int  WINAPI t_DJFax_SelfCheckSetLink(WORD wChnl);
int  WINAPI t_DJFax_SelfCheckBreakLink(WORD wChnl);

//与模拟语音卡的连通函数
int  WINAPI t_DJFax_SetLink(WORD wFaxChnl,WORD wVoiceChnl);
int  WINAPI t_DJFax_ClearLink(WORD wFaxChnl,WORD wVoiceChnl);
int  WINAPI t_DJFax_GetVoiceChnlOfFaxChnl(WORD wFaxChnl);
int  WINAPI t_DJFax_GetFaxChnlOfVoiceChnl(WORD wVoiceChnl);

//发送及接收函数
int   WINAPI t_DJFax_GetOneFreeFaxChnl(void);
int   WINAPI t_DJFax_GetOneFreeFaxChnlOld(void);
int   WINAPI t_DJFax_SetResolution(WORD wChnl,int ResolutionFlag);
int   WINAPI t_DJFax_SendFaxFile(WORD wChnl,char *FileName);
int   WINAPI t_DJFax_RcvFaxFile(WORD wChnl,char* FileName);
int   WINAPI t_DJFax_CheckTransmit(WORD wChnl);
void  WINAPI t_DJFax_StopFax(WORD wChnl);
int   WINAPI t_DJFax_SetLocalID(WORD wChnl,char far *s);
int   WINAPI t_DJFax_GetLocalID(WORD wChnl,char far *s);
long  WINAPI t_DJFax_GetRcvBytes(WORD wChnl);
long  WINAPI t_DJFax_GetSendBytes(WORD wChnl);
int   WINAPI t_DJFax_SetDialNo(WORD wChnl,char* DialNo);
int   WINAPI t_DJFax_GetErrCode(WORD wChnl);
int   WINAPI t_DJFax_GetErrPhase(WORD wChnl);
int   WINAPI t_DJFax_GetErrSubst(WORD wChnl);

//有关转换的函数
int   WINAPI t_DJCvt_InitConvert(void);
void  WINAPI t_DJCvt_DisableConvert(void);
int   WINAPI t_DJCvt_Open(WORD wChnl,char *cbFaxFileName,BYTE cbResolution,WORD wPageLineNo);
int   WINAPI t_DJCvt_Close(WORD wChnl);
int   WINAPI t_DJCvt_DotLine(WORD wChnl,char *cbDotStr,WORD wDotSize,WORD wDotFlag);
int   WINAPI t_DJCvt_LeftLine(WORD wChnl);
int   WINAPI t_DJCvt_TextLineA(WORD wChnl,char* cbTextStr,int DoubleBitFlag,int DoubleLineFlag,int FontSize);
int   WINAPI t_DJCvt_TextLine(WORD wChnl,char *cbTextStr);
int   WINAPI t_DJCvt_BmpFileA(WORD wChnl,char *cbBmpFileName,int DoubleBitFlag);
int   WINAPI t_DJCvt_BmpFile(WORD wChnl,char *cbBmpFileName);

/////////////////////////////////////////////////////////////////
//Add for control.
//int ConvertTrunkIDFromVoiceID(int VoiceID);




#if defined(__cplusplus)
}

#endif 



#endif
