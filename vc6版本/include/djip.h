/************************************************************************

        (c) COPYRIGHT 2000-2008 by Oriental Communicaton System, Inc.
                          All rights reserved.

This software is confidential and proprietary to Oriental Communicaton 
System, Inc.  No part of this software may be reproduced,stored, 
transmitted, disclosed or used in any form or by any means other 
than as expressly provided by the written license agreement between 
Oriental Communicaton System and its licensee.

FileName:       
Author:          
Version:         
Date:             
Description:   
Others:
History:
1.	Date:   
Author:        
Modification:
                                                     
************************************************************************/
#ifndef	__DJ_IP_H__
#define	__DJ_IP_H__

#include <windows.h>

#define    DJ_OK                          0
#define    GET_INT_INFO_ERROR             -1     /*获取初始化信息错误*/
#define    SYS_CHECK_READY_ERROR          -2     /*检测DSp是否Ready错误*/
#define    VAD_LEVEL_ERROR                -3     /*获取VAD Level错误*/
#define    DTMF_ON_TIME_ERROR             -4      /*获取DTMF上升沿时间错误*/
#define    DTMF_OFF_TIME_ERROR            -5      /*获取DTMF下降沿时间错误*/
#define    PACKET_LOSS_RECOVERY_ERROR     -6      /*获取数据包丢失恢复时使用的帧的个数错误*/
#define    CODER_TYPE_ERROR               -7      /*获取编码类型错误*/
#define    FRAME_SIZE_ERROR               -8      /*获取打包时长错误*/
#define    CHANNEL_ERROR                  -9      /*通道号错误*/
#define    NOT_VALID_DATA                 -10     /*非法数据*/
#define    NEW_CHANNEL_ERROR              -11     /*分配全局通道变量错误*/
#define    CHANNEL_INIT_ERROR             -12     /*通道初始化错误*/
#define    INIT_SOCKET_ERROR              -13     /*初始化Socket错误*/
#define    CREATE_SOCKET_ERROR            -14     /*创建Socket错误*/
#define    SOCKET_BIND_ERROR              -15     /*绑定Socket错误*/
#define    SET_SEND_BUFFER_ERROR          -16     /*设置发送Scoket Buffer错误*/
#define    SET_IP_HDRINCL_ERROR           -17     /*设置自己填IP头错误*/
#define    CLOSE_SOCKET_ERROR             -18     /*关闭Socket错误*/
#define    CREATE_THREAD_ERROR            -20     /*创建线程错误*/
#define    PARAMETER_ERROR                -21     /*参数错误*/
#define    CAN_NOT_WRITE                  -22     /*不能写*/
#define    SYS_INIT_ERROR                 -23     /*初始化失败*/
#define    NO_SYS_INIT_ERROR              -24     /*没有初始化就调用函数*/
#define    BUFFER_SIZE_ERROR              -25
#define    CLOSE_CHANNEL_ERROR            -26
#define    D081A4P_LOAD_DEV_ERROR         -27
#define    VOICE_BUFFER_NULL              -30

// 定义DJIP.INI的结构
typedef struct 
{
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
	short    wSendVolumeSize;
	short    wRecvVolumeSize;

} TYPE_DJIP_INI_PARAMETER;

// 定义系统参数的结构
typedef struct 
{
	int		iTotalNum;
} TYPE_DJIP_SYS_PARAMETER;

//int iTotalIPNum; 
	
#ifdef __cplusplus
extern "C" {
#endif

// ------------------
int		WINAPI	DJIP_SetRtpFlag(unsigned short wRtpFlag);
int		WINAPI	DJIP_Sys_Init(void);   /*系统初始化*/
int		WINAPI	DJIP_Sys_Release(void);
void	WINAPI	DJIP_Sys_GetIniInfo ( TYPE_DJIP_INI_PARAMETER *pIniInfo );
void	WINAPI	DJIP_Sys_GetSysInfo ( TYPE_DJIP_SYS_PARAMETER *pSysInfo );

int		WINAPI	DJIP_Ch_GetTimeSlot ( int iChID );
int		WINAPI	DJIP_Ch_ConnectFromTS ( int iChID, int iTS );
int		WINAPI	DJIP_Ch_DisconnectTS ( int iChID );

//int		WINAPI  DJIP_GetVocFromDsp(int channel,char* buffer);
int		WINAPI  DJIP_GetVocFromDsp(int channel,char* buffer,int BufferLen);
int		WINAPI  DJIP_GetVocFromDspEx(int channel,char* buffer,int BufferLen);
int		WINAPI  DJIP_PutVocToDsp(int channel,char* buffer);
int		WINAPI  DJIP_PutSilenceToDsp(int channel);
int		WINAPI  DJIP_IsSilenceBuf(char*);

short   WINAPI  DJIP_RtpDecoder( unsigned short CID, char *pBuffer, unsigned short BufferLen );
short   WINAPI  DJIP_JitterBufferDispatcher( unsigned short CID );

int     WINAPI  DJIP_Channel_Init(int Channel);
int     WINAPI  DJIP_OpenChannel(int CID);
int     WINAPI  DJIP_CloseChannel(int CID);
int     WINAPI DJIP_OpenChannelEx(int CID,
							 unsigned long LocalIP,
							 unsigned long RemoteIP,
							 unsigned short LocalPort,
							 unsigned short RemotePort);
int     WINAPI DJIP_CloseChannelEx(int CID);

int 	WINAPI  DJIP_SetChannelCoderType(short Channel,int CodeType,short SingleOrDouble);
int	    WINAPI  DJIP_GetChannelCodeType ( short Channel,unsigned short RecvAndSendFlag );

int     WINAPI  DJIP_SetChannelEC(int CID,unsigned short ECValue);
int     WINAPI  DJIP_GetChannelEC(int CID);         /*获取EC是否使能*/

int     WINAPI  DJIP_SetChannelJitterSize(int CID,unsigned char JitterBufferSize);
int     WINAPI  DJIP_GetChannelJitterSize(int CID);  /*获取Jitter buffer的大小*/

int     WINAPI  DJIP_SetChannelVadAndCng( int CID, unsigned char VadAndCngEnable );
int     WINAPI  DJIP_GetChannelVadAndCng(int CID);  /*获取VAD是否使能*/

int     WINAPI  DJIP_SetChannelVadLevel( int CID, short VadLevel );
int     WINAPI  DJIP_GetChannelVadLevel(int CID);   /*获取通道的VAD门限*/

int     WINAPI DJIP_SetChannelSendVolume( int CID, short VolumeLevel );
int     WINAPI DJIP_SetChannelRecvVolume( int CID, short VolumeLevel );
int     WINAPI DJIP_GetChannelSendVolume( int CID );
int     WINAPI DJIP_GetChannelRecvVolume( int CID );
int     WINAPI DJIP_GetWriteFlag(int channel);     /*获取写标志*/
unsigned long WINAPI DJIP_GetRecvOctets(int CID);
unsigned long WINAPI DJIP_GetRecvPacket(int CID);
unsigned long WINAPI DJIP_GetLostPacket(int CID);
WORD WINAPI DJIP_GetStartTimeSlot(int CID);

//

//
#ifdef __cplusplus
}
#endif

#endif

