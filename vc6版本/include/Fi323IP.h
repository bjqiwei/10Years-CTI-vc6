#ifndef  __FI323IP_DLL_H__

#ifdef	__cplusplus
extern "C"{
#endif


typedef  enum 
{
	event_IPArrive   = 33,
	event_IPCallFail,
	event_IPAnswer,
	event_IPHangup,
	event_IPRegisterResult,
	event_IPDTMF
};


typedef  void (CALLBACK * ProcVoipEventCallBack)(
				int         	eventType,
				int				channelType,	
				int				channelID, 
				int				iParam1,
				int				iParam2);

//事件通知回调函数

void WINAPI tpi_SetVoipEventCallBackProc( ProcVoipEventCallBack pCallBack);

int  WINAPI tpi_H323Open();
void WINAPI tpi_H323Close();
int  WINAPI tpi_IPCall(int channelType,int channelID,char* DestIp,char* DestNumber,char* srcNumber);
int  WINAPI tpi_IPHangup(int channelType,int channelID);
int  WINAPI tpi_IPAssignVoip(int CallID , int channelType,int channelID);
int  WINAPI tpi_IPAnswer(  int channelType,int channelID);
int  WINAPI tpi_IPRegister(char* GKIP,char* H323ID,char* prefix);
int  WINAPI tpi_IPUnregister();
int  WINAPI tpi_IPSetRtpPortRange(int iStart,int iEnd);
int  WINAPI tpi_IPGetVoipCurrentState(int channelType,int channelID);
int  WINAPI tpi_IPRefuse(int CallID);
BOOL WINAPI tpi_IPEnableLog(BOOL bEnable, char* fileName);

int  WINAPI tpi_IPSetFastStart(BOOL bFastStart);
int  WINAPI tpi_IPSetCallSignalingPort(int Port);
int  WINAPI tpi_IPSendDtmf(int channelType,int channelID,char* Dtmf);



#ifdef	__cplusplus
};
#endif


#endif  // __FI323IP_DLL_H__