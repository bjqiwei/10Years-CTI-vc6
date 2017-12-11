#ifndef _FAXAPI32_H
#define _FAXAPI32_H


#define	MAX_FAX_CARD_NUM	16
#define MAX_FAX_CHANNEL_NUM	(4*MAX_FAX_CARD_NUM)

// modify by H.J.N when write DFM-PCI, 2001.10.31
//#define MAX_VOICE_CHANNEL_NUM 128
#define MAX_VOICE_CHANNEL_NUM 256
// end
//#define	BUFFER_LEN	2048
#define HIGH_RESOLUTION	1
#define LOW_RESOLUTION	0
#define FILE_RESOLUTION 2

#define FILE_TYPE_VALID 1
#define FILE_TYPE_PASS 2
#define FILE_TYPE_LAST 0
#define FILE_TYPE_COMPAGES 3



typedef struct {
	WORD	wCardNo;
	WORD	wMemBaseAddr;
	WORD	wIOAddr[MAX_FAX_CARD_NUM];
} FAX_SYS_INFO;

typedef struct tagDJ_FAX_CTRL{
	char FileName[256];
	int StartPage;
	int EndPage;
	int Resolution;
	int Type;  		// view FILE_TYPE_VALID .....
}DJ_FAX_CTRL;


#ifdef __cplusplus
extern "C" {
#endif

int WINAPI DJFax_DriverReady(WORD wBuffSize);
void WINAPI DJFax_DisableCard(void);
void WINAPI DJFax_GetSysInfo(FAX_SYS_INFO *TmpInfo);
int WINAPI DJFax_GetTotalFaxChnl(void);

int WINAPI DJFax_SetLocalID ( WORD wChnl, char far *s );
int WINAPI DJFax_GetLocalID ( WORD wChnl, char far *s );
int WINAPI DJFax_SetDialNo(WORD wChnl,char *DialNo);
int WINAPI DJFax_SetValue(WORD wChnl, void far *s, WORD Position, WORD Count);
int WINAPI DJFax_GetValue(WORD wChnl, void far *s, WORD Position, WORD Count);

int WINAPI DJFax_SetLink(WORD wFaxChnl,WORD trunkID);
int WINAPI DJFax_ClearLink ( WORD wFaxChnl, WORD trunkID);
int WINAPI DJFax_SelfCheckSetLink(WORD wFaxChnl);
int WINAPI DJFax_SelfCheckBreakLink(WORD wFaxChnl);

long WINAPI DJFax_GetRcvBytes(WORD wChnl);
long WINAPI DJFax_GetSendBytes(WORD wChnl);

int WINAPI DJFax_GetOneFreeFaxChnl(void);
int WINAPI DJFax_GetFaxChnlOfVoiceChnl(WORD trunkID);
int WINAPI DJFax_GetVoiceChnlOfFaxChnl(WORD trunkID);
int WINAPI DJFax_GetMiddleStatus(WORD wChnl);
int WINAPI DJFax_GetErrCode(WORD wChnl);
int WINAPI DJFax_GetErrPhase(WORD wChnl);
int WINAPI DJFax_GetErrSubst(WORD wChnl);

void WINAPI DJFax_StopFax(WORD wChnl);


int WINAPI DJFax_SendFaxFile(WORD wChnl,char *FileName);
int WINAPI DJFax_SendFaxFileEx(WORD wChnl,char *FileName,int StartPage,int EndPage);
int WINAPI DJFax_Send(WORD wChnl,DJ_FAX_CTRL* pFaxCtrl);
int WINAPI DJFax_CheckTransmit(WORD wChnl);
int WINAPI DJFax_RcvFaxFile(WORD wChnl,char *FileName);

int WINAPI DJFax_SetResolution(WORD wChnl,int ResolutionFlag);

int WINAPI DJFax_SetDCSCtrlByte(WORD wChnl,BYTE cbDCSCtrlByte);
WORD WINAPI DJFax_GetDCSCtrlWord(WORD wChnl);

// syl add ,for get current send pages
int WINAPI DJFax_GetCurPage(WORD chnl);

// add for hai-nan, 1999.03.08
int WINAPI DJFax_GetRemoteID ( WORD wChnl, char far *s );
int WINAPI DJFax_ClearRemoteID ( WORD wChnl );

//--------------------------------------------------------------------------
//Add for MODEM, 1998.11.26

int WINAPI DJFax_SetModemCtrol ( WORD wChnl);
int WINAPI DJFax_GetModemCtrol ( WORD wChnl);
int WINAPI DJFax_GetLineStatus( WORD wChnl);
int WINAPI DJFax_GetModemStatus( WORD wChnl);
int WINAPI DJFax_GetSendBufSize( WORD wChnl);

int WINAPI DJFax_ReadModemBuf(WORD wChnl,char *DataBuf);
int WINAPI DJFax_WriteModemBuf(WORD wChnl,char *DataBuf,int DataLen);

// add for check fax tone, modified by yezm, 2004.08.18
void WINAPI DJFax_StartCheckFaxTone(WORD wChnlNo);
int  WINAPI DJFax_FaxToneCheckResult(WORD wChnlNo);
// and for check fax tone.


//--------------------------------------------------------------------------
//Add for Convert
#define DOT_0_IS_WHITE      0
#define DOT_1_IS_WHITE      1

int WINAPI DJCvt_InitConvert(void);
void WINAPI DJCvt_DisableConvert(void);
int WINAPI DJCvt_Open(WORD wChnl,char *cbFaxFileName,BYTE cbResolution,
            WORD wPageLineNo);
int WINAPI DJCvt_Close(WORD wChnl);
int WINAPI DJCvt_DotLine(WORD wChnl,char *cbDotStr,WORD wDotSize,WORD wDotFlag);
int WINAPI DJCvt_TextLine(WORD wChnl,char *cbTextStr);
int WINAPI DJCvt_BmpImage(WORD wChnl,char *cbImageStr);
int WINAPI DJCvt_BmpFile(WORD wChnl,char *cbBmpFileName);

int WINAPI DJCvt_LeftLine(WORD wChnl);
int WINAPI DJCvt_BmpFileA(WORD wChnl,char *cbBmpFileName, int DoubleBitFlag);
int WINAPI DJCvt_TextLineA(WORD wChnl,char *cbTextStr, int DoubleBitFlag, int DoubleLineFlag,int FontSize);


// add for jin-zheng, 1999.07.05
int WINAPI DJCvt_TextLine1(WORD wChnl,char *cbTextStr);


// add for good-fax, 1999.12.20
int WINAPI DJFax_GetOneFreeFaxChnlOld(void);

#define	GF_TYPE_CHANNEL_FLAG		0
#define	GF_TYPE_CARD_FLAG			1
#define	GF_TYPE_RESET_COUNT			2
#define	GF_TYPE_CARD_VERSION		3
#define	GF_TYPE_CONNECT_FLAG		4
int WINAPI GF_GetVal ( int No, int type );
// end of good-fax



#ifdef __cplusplus
}
#endif

#endif	//_FAXAPI32_H
