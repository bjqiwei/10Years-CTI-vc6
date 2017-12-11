#ifndef __FIHO_PHONIC_H__
#define __FIHO_PHONIC_H__

#include "ctidef.h"

#ifdef	__cplusplus
extern "C"{
#endif


	
typedef enum enumPhonicEvent
{
	eventState	= 0,		//通道状态,iParam1=新状态,iParam2=旧状态
	eventDeviceTimer,		//通道定时器
	eventSignal,			//已废弃
	eventAlarm,				//已废弃
	eventIdle,				//通道空闲
	eventCallIn,			//呼入,iParam1=指向号码部分的字符指针,iParam2=指向曰期部分的字符指针
	eventAnswer,			//外线应答或高阻摘机
	eventCallOutFinish,		//外线呼叫完成
	eventCallFail,			//线路呼叫失败
	eventHangup,			//线路挂机
	eventDTMF,				//检测到DTMF,iParam1=DTMF的ASCII的值
	eventSignalMonitor,		//已废弃
	eventPlayEnd,			//放音结束,iParam1=已放完的字节数
	eventRecordEnd,			//录音结束,iParam1=已录完的字节数
	eventFaxError,			//传真失败,iParam1=传真失败原因
	eventOneFaxPageOver,	//传真完成,iParam1=0,完成一页;iParam1=1,全部完成
	eventFlash,				//检测到内线拍了叉簧
	eventSendCallerIDEnd,   //已废弃
	eventSendV23End,        //已废弃
	eventV23DataReady,      //已废弃
	eventRing,              //振铃 
	eventVoice,             //检测到语音 
	eventSilence,           //检测到静音 
	eventHanging,			//高阻悬空
	eventDetectedFaxTone,   //检测到传真音 	  
	eventFaxToneMissing,    //已废弃
	eventRealTimeData,      //已废弃
	eventAnalogSignal,      //已废弃
	eventDtmfOn,            //检测到按键按下,iParam1=DTMF的ASCII的值
	eventDtmfOff,           //检测到按键弹起,iParam1=DTMF的ASCII的值 
	eventPolarity,          //检测到外线极性变化,iParam1极性
	eventEnergy,            //已废弃
	eventVad,               //已废弃
	eventIPArrive,          //已废弃
	eventIPCallFail,        //已废弃
	eventIPAnswer,          //已废弃
	eventIPHangup,          //已废弃   
	eventIPRegisterResult,  //已废弃
	eventFlashOver,         //表示外线拍叉簧的动作(调用tpi_FlashEx())完成
	eventRingBack,          //检测到外线的回铃音
	eventV21Data,           //已废弃  
	eventFaxModulationOn,   //已废弃
	eventFaxModulationOff,  //已废弃 
	eventFaxSpeed,          //报告当前的传真速率,iParam1=传真速率
	eventFaxResolution,     //报告当前的传真分辨率,iParam1=传真分辨率
	eventFaxRemoteID,       //报告本地的传真标识(),iParam1=指向标识的字符指针
	eventFaxTCFOK,          //传真训练结束 
	eventRecordStreamReady, //流式语音准备好了,iParam1=指向流式语音数据的字符指针,iParam2=数据大小
	eventDialTone,          //报告拨号音检测结果,iParam1=0,没有检测到拨号音;iParam1=1,检测到了拨号音
	eventTTSDataReady,
	eventMax
}EPhonicEvent;

                                    
typedef void (CALLBACK * ProcPhonicFireEventCallBack)(
				EPhonicEvent	eventType,
				int				channelType,	
				int				channelID, 
				int				iParam1,
				int				iParam2);

//事件通知回调函数
void WINAPI tpi_SetEventNotifyCallBackProc( ProcPhonicFireEventCallBack procFireEvent );

int WINAPI tpi_GetPhonicEvent( 
				EPhonicEvent *	pEventType,
				int			 *	pChannelType,	
				int			 *	pChannelID, 
				int			 *	pParam1,
				int			 *	pParam2);

//系统
int  WINAPI tpi_OpenDevice();
void WINAPI tpi_CloseDevice();


int  WINAPI tpi_GetCardCount(int lCardType);
int  WINAPI tpi_GetChannelCount(int channelType);
int  WINAPI tpi_GetLineCount(void);
int  WINAPI tpi_GetLineID(int channelType, int channelID);
int  WINAPI tpi_GetChannelType(int lineID);
int  WINAPI tpi_GetChannelID(int lineID);



//定时器
int  WINAPI tpi_SetDeviceTimer(int channelType, int channelID, int timer);
int  WINAPI tpi_ClearDeviceTimer(int channelType, int channelID);
int  WINAPI tpi_GetChannelState(int channelType, int channelID);

//呼叫接续
int  WINAPI tpi_GetChannelCallerID(int channelType, int channelID,char * callerIDBuf,char * dateTimeBuf);
int  WINAPI tpi_MakeCall(int channelType, int channelID, const char * callerID, const char * phoneNumber, int overtime);
int  WINAPI tpi_MakeCallEx(int channelType, int channelID, const char * callerID, const char * phoneNumber,int toneTime, int silenceTime, int scale,int overtime);
int  WINAPI tpi_EnableHookOffDetect(int channelType, int channelID, BOOL bEnable);
int  WINAPI tpi_Answer(int channelType, int channelID, int bConn);
int  WINAPI tpi_Hangup(int channelType, int channelID, int cause);

//交换
int  WINAPI tpi_TalkWith(int destType, int destID, int srcType, int srcID);
int  WINAPI tpi_BreakTalk(int channelType, int channelID);
int  WINAPI tpi_ListenTo(int destType, int destID, int srcType, int srcID);
int  WINAPI tpi_BreakListen(int destChannelType, int destChannelID);
int  WINAPI tpi_SetLineListenVolume(int channelType, int channelID, int volume);


//录放音
int  WINAPI tpi_SetVoiceVolume(int channelType, int channelID, int volume, int type);
int  WINAPI tpi_SetVoiceCoder(int channelType, int channelID, int lCoder);
int  WINAPI tpi_GetVoiceCoder(int channelType, int channelID);

int  WINAPI tpi_AddPromptVoiceFromFile(const char * fileName);
int  WINAPI tpi_AddPromptVoiceFromMemory(char* pVoiceBuffer, int voiceSize);
void WINAPI tpi_ResetPromptVoice(void);
int  WINAPI tpi_PlayPromptVoice(int channelType, int channelID, int pmtIndex, int loopCount);
int  WINAPI tpi_PlayPromptVoiceArray(int channelType, int channelID, int* pPmtIndexArray, int pmtCount, int loopCount);

int  WINAPI tpi_PlayFile(int channelType, int channelID, const char * fileName, int offset, int voiceSize);
int  WINAPI tpi_PlayMemory(int channelType, int channelID, char* pVoiceBuffer, int voiceSize);
int  WINAPI tpi_PlayTone(int channelType, int channelID, float fFrequency, int dwTimeLength, int dwScale, const char * attrib);
int  WINAPI tpi_SendDtmf(int channelType, int channelID, const char * dtmfString, int toneTime, int silenceTime, int scale);
int  WINAPI tpi_PlayPause(int channelType, int channelID, int bPause);
int  WINAPI tpi_StopPlay(int channelType, int channelID);
int  WINAPI tpi_StopPlayEx(int channelType, int channelID,int voiceType);

int  WINAPI tpi_RecordFile(int channelType, int channelID, const char * fileName, int offset, int voiceSize);
int  WINAPI tpi_RecordMemory(int channelType, int channelID, char* pVoiceBuffer, int voiceSize);
int  WINAPI tpi_RecordPause(int channelType, int channelID, int bPause);
int  WINAPI tpi_StopRecord(int channelType, int channelID);
int  WINAPI tpi_StopRecordEx(int channelType, int channelID,int voiceType);

//流式录放音
int	 WINAPI   tpi_StartRecordStream(int channelType,int channelID, long blocksize);
int	 WINAPI   tpi_PlayStream(int channelType,int channelID,char* pVoiceData, long size);

//TTS
int  WINAPI tpi_PlayText(int channelType, int channelID, const char * text, int isMale, int speed);
int  WINAPI tpi_PlayTextFile(int channelType, int channelID, char* text, int isMale, int speed);
int  WINAPI tpi_ConvertTextToVoiceFile(char *pText, int isMale, int speed, int iTextIsFileName, char* pVocName);

//电话会议
int  WINAPI   tpi_CreateConference();//返回会议句柄
int  WINAPI	  tpi_JoinConference(int channelType, int channelID, int conferResourceGrpID, int conferSetID, int Attenuation, int NoiseSuppression);
int  WINAPI   tpi_ListenConference(int channelType, int channelID, int conferResourceGrpID, int conferSetID);
int  WINAPI   tpi_ExitConference(int channelType, int channelID);
int  WINAPI   tpi_GetChannelConferenceHandle(int channelType, int channelID);//返回电话通道所在的会议句柄
int  WINAPI   tpi_SetConferenceMemberVolumeEx(int channelType,int channelID,int iVolume,int Type);//Type==0为会议成员Mic音量,Type==1会议成员Speaker音量
int  WINAPI   tpi_ReleaseConference(int conferenceHandle);



//FAX
int  WINAPI tpi_faxSetResolution( int channelType,int channelID,int res);
int  WINAPI tpi_faxSetLocalID(int channelType, int channelID, const char * IDString);
int  WINAPI tpi_faxSendFax(int channelType, int channelID, const char * fileName);
int  WINAPI tpi_faxReceiveFax(int channelType, int channelID, const char * fileName);
void WINAPI tpi_faxStopFax(int channelType, int channelID);

int  WINAPI tpi_faxOpenConvert();
void WINAPI tpi_faxCloseConvert();
int  WINAPI tpi_faxCreateFaxFile(int cvtChannelID, const char * fileName, int Resolution, int scanLineNum);
void WINAPI tpi_faxCloseFaxFile(int cvtChannelID);
int  WINAPI tpi_faxAddTextLine(int cvtChannelID, const char * strText, int horiDoubleFlag, int vertiDoubleFlag, int FontSize);
int  WINAPI tpi_faxAddBmpFile(int cvtChannelID, const char * BmpFileName, int DoubleBitFlag);
int  WINAPI tpi_faxAddDotLine(int cvtChannelID, const char * szDot, int DotSize, int DotFlag);
int  WINAPI tpi_faxLeftLine(int cvtChannelID);


// 系列号
int  WINAPI  tpi_GetCardPass(int iCardNo);
int  WINAPI  tpi_GetCardUserNo(int iCardNo);
int  WINAPI  tpi_SetVoiceThreshold(int channelType, int channelID,int lVoiceThreshold,int lVoiceCount);
int  WINAPI  tpi_SetSilenceThreshold(int channelType, int channelID,int lSilenceThreshold);
int  WINAPI  tpi_SetSilenceThresholdEx(int channelType, int channelID,int lSilenceThreshold,int lSilenceTime,BOOL bDisableEvent);


int  WINAPI  tpi_Flash(int channelType, int channelID);
int  WINAPI  tpi_FlashEx(int channelType, int channelID,int lFlashTime);
int  WINAPI  tpi_EnableDtmfPredetect(int channelType, int channelID, LONG iEnable);
int  WINAPI  tpi_SetDtmfDetectTime(int channelType, int channelID, int time);
int  WINAPI  tpi_StartDetectDialTone(int channelType, int channelID,int iThreshold=25,int judgeTime=3000);
int  WINAPI  tpi_PowerDown(int channelType, int channelID);
int  WINAPI  tpi_LinePolarity(int channelType, int channelID,int value);
int  WINAPI  tpi_SetRingParameter(int channelType, int channelID,int OnTime,int OffTime);
int  WINAPI  tpi_SetCallerIDType(int channelType, int channelID,int type);
int  WINAPI  tpi_MeasureVoltage(int channelType, int channelID);       //测量被监控线路的电压
int  WINAPI  tpi_SetVthr1(int channelType, int channelID,int voltage); //设置被监控线路摘、挂机电压的平均值
int  WINAPI  tpi_SetVthr0(int channelType, int channelID,int voltage); //设置被监控线路悬空的电压，一般设置低于线路摘机电压就行了.
int  WINAPI  tpi_GetVthr1(int channelType, int channelID); //取得摘、挂机电压的设置值
int  WINAPI  tpi_GetVthr0(int channelType, int channelID); //取得线路悬空的电压的设置值
int  WINAPI  tpi_MuteEncoderWhenDtmf(long channelType, long channelID, long Mec) ;
int  WINAPI  tpi_PlayFile2(int channelType, int channelID, const char * fileName, int offset, int voiceSize);
#ifdef	__cplusplus
};
#endif

#endif //__FIHO_PHONIC_H__