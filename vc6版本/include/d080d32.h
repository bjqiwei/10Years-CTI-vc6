#ifndef _D080D32_H
#define _D080D32_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************
	define value
********************************************/
#define	MAX_CARD_NO		16
//#define	MAX_CHANNEL_NO	8 * MAX_CARD_NO
#define	MAX_CHANNEL_NO	(256)

#define LEN_FILEPATH	70

// the value of every card-type
#define	CARD_TYPE_T5_64		1
#define	CARD_TYPE_T5_128	2
#define	CARD_TYPE_T5_REC	3

#define	CARD_TYPE_T5_128_NT	10
#define	CARD_TYPE_T5_REC_NT	11
#define	CARD_TYPE_T5_ID_NT	12

// new-d160a
#define	CARD_TYPE_D160A_NT		30
// d160an	
#define	CARD_TYPE_D160A_NT_NEW	31
// d080dd080d
#define CARD_TYPE_D080D_NT		40

#define CHTYPE_USER     0
#define CHTYPE_TRUNK    1
#define CHTYPE_EMPTY    2

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


#define RECORD_CHECK    01
#define PLAY_CHECK              02
#define SEND_CHECK              03
#define SEND_READY_CHECK 04

#define R_BUSY          0x21
#define R_OTHER         0x20

#define S_NORESULT      0x10
#define S_BUSY          0x11
#define S_NOBODY        0x13
#define S_CONNECT       0x14
#define S_NOSIGNAL      0x15

#define S_DIALSIG       0x30


#define PACK_64KBPS     0
#define PACK_32KBPS     1
#define PACK_16KBPS     2
#define PACK_8KBPS     3

// only in D080D, we support G723 ( 5.3K BPS & 6.3K BPS )
#define PACK_G723_53     53
#define PACK_G723_63     63

#define G723_PACKET_SIZE_63			24
#define G723_PACKET_SIZE_53			20
//

// NEW ADD for Feed and Signal

#define SIG_STOP     0
#define SIG_DIALTONE 1
#define SIG_BUSY1    2
#define SIG_BUSY2    3
#define SIG_RINGBACK 4

#define HANG_UP_FLAG_FALSE 0
#define HANG_UP_FLAG_TRUE  1
#define HANG_UP_FLAG_START 2
#define HANG_UP_FLAG_PRESS_R  3


// caller-ID
#define	ID_STEP_NONE	0
#define	ID_STEP_HEAD	1
#define	ID_STEP_ID	2
#define	ID_STEP_OK	3
#define	ID_STEP_FAIL	4
#define	ID_STEP_LEN	5

// end of caller-ID


typedef struct {
	WORD PlayFlag;
	WORD RecordFlag;
	WORD PlayCount;
	WORD RecordCount;
	WORD DtmfCount;
	WORD DialFlag;
	WORD SigCount;
	WORD SigStartPoint;
	LPSTR SigBuf;
} TPD_RPB;

typedef struct {
	BYTE Busy1Count1Low;
	BYTE Busy1Count1High;
	BYTE Busy1Count0Low;
	BYTE Busy1Count0High;

	WORD  BusySigCount;
	WORD  BusySigLen;

	WORD  SendReadyLen;

	BYTE SendBusy1Count1Low;
	BYTE SendBusy1Count1High;
	BYTE SendBusy1Count0Low;
	BYTE SendBusy1Count0High;

	BYTE SendSigCount1Low;
	BYTE SendSigCount1High;
	BYTE SendSigCount0Low;
	BYTE SendSigCount0High;

	WORD  SendNoSignalLen;

	BYTE MaxRingTimes;
} TPD_WPB;

//use play index file
typedef struct{
	char *FileName[100];
	HANDLE gHandle[100];
	DWORD FileCount;
	int FileFlag[100];
}TPD_PIF;

/*******************************************
	define struct & type
********************************************/
typedef struct {
	// get from "TC08A-V.INI"
	WORD wCardNo;
	WORD wCardType;
	WORD wConnect;
	WORD wIRQ;
	char cbDir[LEN_FILEPATH];
	WORD wAddress[MAX_CARD_NO];
	
	// get from driver
	WORD wMajorVer;
	WORD wMinorVer;
	WORD wChType[MAX_CHANNEL_NO];
//	WORD wChSubType[MAX_CHANNEL_NO];

} TC_INI_TYPE;


/*******************************************
	define function
********************************************/
long WINAPI LoadDRV(void);
void WINAPI FreeDRV(void);

void WINAPI GetSysInfo( TC_INI_TYPE *TmpIni);
WORD WINAPI CheckValidCh(void);
WORD WINAPI CheckChType(WORD wChnlNo);
BOOL WINAPI IsSupportCallerID(void);

long WINAPI EnableCard(WORD wUsedCh, WORD wFileBufLen);
void WINAPI DisableCard(void);
void WINAPI SetPackRate ( WORD wPackRate );


void WINAPI PUSH_PLAY (void);

//sting
BOOL WINAPI RingDetect(WORD wChnlNo);
BOOL WINAPI CheckPolarity(WORD wChnlNo);
void WINAPI OffHook(WORD wChnlNo);
void WINAPI HangUp(WORD wChnlNo);

long WINAPI SetLink ( WORD wOne, WORD wAnother );
long WINAPI ClearLink ( WORD wOne, WORD wAnother );
long WINAPI LinkOneToAnother ( WORD wOne, WORD wAnother );
long WINAPI ClearOneFromAnother ( WORD wOne, WORD wAnother );
long WINAPI LinkThree( WORD wOne, WORD wTwo, WORD wThree);
long WINAPI ClearThree( WORD wOne, WORD wTwo, WORD wThree);

void WINAPI InitDtmfBuf(WORD wChnlNo);
short WINAPI GetDtmfCode( WORD wChnlNo);
BOOL WINAPI DtmfHit(WORD wChnlNo);

void WINAPI StartSigCheck ( WORD wChnlNo );
void WINAPI StopSigCheck ( WORD wChnlNo );
WORD WINAPI ReadCheckResult(WORD wChnlNo, WORD wMode);
WORD WINAPI ReadBusyCount(void);

void WINAPI SetBusyPara( WORD BusyLen );
void WINAPI SetDialPara ( WORD RingBack1, WORD RingBack0, WORD BusyLen, WORD RingTimes );
void WINAPI ReadSigBuf ( WORD wChnlNo, WORD *pwStartPoint, WORD *pwCount, BYTE *SigBuf );

void WINAPI StartPlay ( WORD wChnlNo, char *PlayBuf, DWORD dwStartPos, DWORD dwPlayLen);
void WINAPI StopPlay ( WORD wChnlNo);
BOOL WINAPI CheckPlayEnd ( WORD wChnlNo );
void WINAPI ReadStatus ( WORD wChnlNo, TPD_RPB *TmpRead );

BOOL WINAPI StartPlayFile ( WORD wChnlNo, LPSTR FileName, DWORD StartPos );
void WINAPI StopPlayFile (WORD wChnlNo);

// index play file
void WINAPI RsetIndexPlayFile(WORD Line);
BOOL WINAPI AddIndexPlayFile(WORD Line, char *FileName);
BOOL WINAPI StartIndexPlayFile( WORD wChnlNo);
BOOL WINAPI CheckIndexPlayFile(WORD ChnlNo);
void WINAPI StopIndexPlayFile(WORD wChnlNo);

void WINAPI ResetIndex (void);
BOOL WINAPI SetIndex ( char *VocBuf, DWORD dwVocLen);
void WINAPI StartPlayIndex( WORD wChnlNo, WORD *pIndexTable, WORD wIndexLen);

void WINAPI SendDtmfBuf(WORD wChnlNo, LPSTR DialNum);
BOOL WINAPI CheckSendEnd(WORD wChnlNo);

//static WORD StopRecord(WORD wChnlNo);
BOOL WINAPI StartRecordFile ( WORD wChnlNo, LPSTR FileName, DWORD dwRecordLen );
BOOL WINAPI CheckRecordEnd ( WORD wChnlNo );
void WINAPI StopRecordFile (WORD wChnlNo);


void WINAPI FeedSigFunc ( void );
void WINAPI StartTimer(WORD wChnlNo, WORD ClockType);
long WINAPI ElapseTime ( WORD wChnlNo, WORD ClockType );
void WINAPI StartPlaySignal (WORD wChnlNo, WORD SigType );
void WINAPI StartHangUpDetect ( WORD wChnlNo );
WORD WINAPI HangUpDetect ( WORD wChnlNo );
void WINAPI FeedRing(WORD wChnlNo);
void WINAPI FeedRealRing(WORD wChnlNo);
void WINAPI FeedPower(WORD wChnlNo);
BOOL WINAPI OffHookDetect(WORD wChnlNo);
BOOL WINAPI ReadGenerateSigBuf (LPSTR lpFileName);

// caller-id
void WINAPI ResetCallerIDBuffer(WORD wChnlNo);
WORD WINAPI GetCallerIDRawStr (WORD wChnlNo, LPSTR IDRawStr);
WORD WINAPI GetCallerIDStr (WORD wChnlNo, LPSTR IDStr);

BOOL WINAPI StartRecordFileNew ( WORD wChnlNo, LPSTR FileName, 
	DWORD dwRecordLen,DWORD dwRecordStartPos );
long WINAPI NewReadPass ( WORD wCardNo );
long WINAPI CheckSilence(WORD wChnlNo, WORD wCheckNum);

// add about fast send dtmf
int WINAPI SetSendPara ( int ToneLen, int SilenceLen );
void WINAPI NewSendDtmfBuf(int ChannelNo, LPSTR DialNum);
int WINAPI NewCheckSendEnd(int ChannelNo);

// add for T5-REC-NT card
#define FREQWIDTH_ALL   0
#define FREQWIDTH_WIDE  1

#define SENSVAL_1_OF_16 00
#define SENSVAL_1_OF_8  01
#define SENSVAL_1_OF_4  02
#define SENSVAL_1_OF_2  03

#define SPEAKER_VOL_NONE        0
#define SPEAKER_VOL_1           1
#define SPEAKER_VOL_2           2
#define SPEAKER_VOL_3           3
#define SPEAKER_VOL_4           4
#define SPEAKER_VOL_5           5
#define SPEAKER_VOL_6           6
#define SPEAKER_VOL_7           7
#define SPEAKER_VOL_8           8

void WINAPI LinkZeroTo ( WORD wChnlNo );
void WINAPI ClearZeroLink (void);
void WINAPI SetSpkVol ( WORD wVol );
//end of add



//----------- add for convert FAX
int WINAPI DJCvt_ClientStartConvert( WORD wFaxChnl, char *NowTxtName, char *NowFaxName, BYTE cbResolution, BYTE cbPageLineNo);
int WINAPI DJCvt_ClientStopConvert( WORD wFaxChnl );
int WINAPI DJCvt_ClientCheckCvt ( WORD wFaxChnl );

// add for new-d160a
#define	WORK_MODE_DTMF				0
	#define	DTMF_MODE_VAL_NORMAL			0
	#define	DTMF_MODE_VAL_QUICK				1
#define WORK_MODE_TEST_DTMF			1
#define	WORK_MODE_TEST_FAX			2
#define	WORK_MODE_SET_SIG_PARA		3
#define	WORK_MODE_CHECK_RING		4
	#define	CHECK_RING_MODE_VAL_NEW			0
	#define	CHECK_RING_MODE_VAL_OLD			1
#define	WORK_MODE_REC_AGC			5
	#define	REC_AGC_MODE_VAL_DISABLE		0
	#define	REC_AGC_MODE_VAL_ENABLE			1
// add for D080D
#define	WORK_MODE_AGC_PARAM_INCREASE	6
#define	WORK_MODE_AGC_PARAM_DECREASE	7

void WINAPI D_SetWorkMode  ( WORD wChnl, char cbWorkMode, char cbModeVal );

// more info function
typedef struct {
	WORD wMemAddr;

	WORD wCardNum;
	BYTE cbCardType[MAX_CARD_NO];		// add for D160A, also mean Channel of this card type
	BYTE cbCardNeiWai[MAX_CARD_NO];		// add for D160A, TRUNK or USER

	WORD wChnlNum;
	BYTE cbChType[MAX_CHANNEL_NO];
	BYTE cbChnlCardNo[MAX_CHANNEL_NO];
	BYTE cbChnlInternal[MAX_CHANNEL_NO];
	BYTE cbConnectChnl[MAX_CHANNEL_NO];
	BYTE cbConnectStream[MAX_CHANNEL_NO];
	BYTE cbDtmfModeVal[MAX_CHANNEL_NO];
	BYTE cbIsSupportCallerID[MAX_CHANNEL_NO];
} TC_INI_TYPE_MORE;

#define	CARD_TYPE_D160A		16
#define	CARD_TYPE_D080A		8


WORD WINAPI D_GetSysInfoMore( TC_INI_TYPE_MORE *TmpMore);
// end of add

#define	VOL_ADJUST_RECORD		0
#define	VOL_ADJUST_PLAY			1
int WINAPI D_AdjustVocVol ( WORD wChnl, char cMode, char cVolAdjust );
int WINAPI D_AdjustVocVol_ForVB ( WORD wChnl, WORD wMode, WORD wVolAdjust );


// add for D160A or D080A card
void WINAPI ReadSigBufAll ( WORD wChnlNo, WORD *pwStartPoint, WORD *pwCount, BYTE *SigBuf );
void WINAPI SetSigFreqAll ( WORD wIndex, WORD wFreq );
// end

// add for-vr
int	WINAPI VR_SetRefreshSize ( WORD wSize );
void WINAPI VR_StartRecord ( WORD wChnlNo );
void WINAPI VR_StopRecord(WORD wChnlNo);
int	WINAPI	VR_GetRecordData ( WORD wChnlNo, char *pBuffer );
void WINAPI VR_ReadSigBuf ( WORD wChnlNo, WORD *pwStartPoint, WORD *pwCount, BYTE *SigBuf );
void WINAPI VR_SetEcrMode ( WORD wChnl, BYTE cbEnableFlag, WORD wParam1, WORD wParam2 );
// end

//for-tts
int	WINAPI	IsNowSupportTTS(void);

// for hn, dyq, 2000.09.06
void WINAPI NewSetBusyPara( WORD Busy1Low, WORD Busy1High,WORD Busy0Low, WORD Busy0High );

// add for zyb, 2000.09.07
BOOL WINAPI AddIndexPlayFile_ZYB(WORD Line, char *FileName, int DNum );

// add for HZARKJ, 2000.10.19
int WINAPI HZ_inp (WORD wPort);
void WINAPI HZ_outp(WORD wPort, int DataByte );
// end


// ------------- HJN start add -----------------------------
int		WINAPI	D080D_HandleVoltageTable(void);

BYTE	WINAPI	D080D_GetVoltageValue (WORD wChnlNo);
short	WINAPI	D080D_RingCount(WORD wChnlNo);	//返回振铃次数

BOOL  WINAPI D080D_GetUserHookParam(WORD wChnlNo, WORD* pParam);	//得到用户摘挂机参数
BOOL  WINAPI D080D_SetUserHookParam(WORD wChnlNo, WORD param);	//得到用户摘挂机参数

short	WINAPI D080D_HookDetect(WORD wChnlNo);

void	WINAPI D080D_StartPreLoopRecord(WORD wChnlNo);
int		WINAPI D080D_StartRecordFile ( WORD wChnlNo, LPSTR FileName, DWORD dwRecordLen, int ForwardLen );


#define	D080D_MAX_CONF_CHNL		4
#define	D080D_CONF_GROUP_ID		1

int		WINAPI D080D_Extra_StartPlayFile ( int PlayID, LPSTR FileName, DWORD StartPos, int ChnlAtte );
BOOL	WINAPI D080D_Extra_CheckPlayEnd ( int PlayID );
void	WINAPI D080D_Extra_StopPlayFile ( int PlayID );


void WINAPI SetSigParaOneChnl ( WORD wChnlNo, WORD wFreqWidth, WORD wSensVal );

#ifdef __cplusplus
}
#endif


#endif
