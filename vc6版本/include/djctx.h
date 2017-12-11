#ifndef __DJCTX_H__
#define __DJCTX_H__


#ifdef __cplusplus
extern "C" {
#endif



enum CHANNEL_TYPE
{
	TYPE_TRUNK=0,
	TYPE_USER,
	TYPE_FAX,
	TYPE_CONF,
};


struct DJCTX_EXCH_FORUSER
{
	CHANNEL_TYPE ChnType;
	WORD wLocalChnNo;
	WORD wLocalStreamNo;
	WORD wLocalSlotNo;
	WORD wRemoteStreamNo;
	WORD wRemoteSlotNo;

};

struct DJExchSys
{
	int serverCount;
	int LocalID;
	int Flag;
	int CT_BusType;
	int DriveType;//0:ANALOG;1:DIGITAL;
};

struct DJExchInfo
{
	char Ip[20];
	int port;
	WORD wClockMode;
	int StartStream;
	int UseStream;
	int ExchTrunk;
	int ExchUser;
	int ExchFax;
	int ExchConf;
};

struct DJCTx_CONFIG_FILE
{
	DJExchSys SysInfo;
	DJExchInfo AllExchInfo[16];//16:系统最大机器数量
};


int WINAPI  DJCTx_InitExch();
BOOL WINAPI  DJCTx_FreeExch();
BOOL WINAPI  DJCTx_RecvData(char* Source, char* Data, int &Size);
int  WINAPI  DJCTx_SendData(int TargetID,char* data,int size);
void WINAPI DJCTx_GetEvent();
BOOL WINAPI DJCTx_CheckSendSockEnd(int paramRefCount);
WORD WINAPI DJCTx_GetChnInfo(CHANNEL_TYPE ChnType,WORD LocalChnNo,WORD* LocalStreamNo,WORD* LocalSlotNo);
WORD WINAPI DJCTx_ConnectFromCTBus(CHANNEL_TYPE ChnType,WORD LocalChnNo,WORD RemoteStreamNo,WORD RemoteSlotNo);
DJCTx_CONFIG_FILE WINAPI  DJCTx_QueryExchConfig();


#ifdef __cplusplus
}
#endif


#endif