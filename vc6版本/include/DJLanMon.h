#ifndef _DJLANMON_H
#define _DJLANMON_H

#ifndef DJLANMON_API 
#define DJLANMON_API __declspec(dllexport)
#endif

#include "windows.h"

typedef struct
{
	int TotalCount;
	char IPAddr[16][15];
	char MonStartTime[16][20];
	int MonDura[16];
	BOOL IsMonStatus[16];
}DJLM_CHANMONINFO,*PDJLM_CHANMONINFO; 

int WINAPI DJLM_Svr_Init(char *ServerIp,int BuffSize);
int WINAPI DJLM_Clt_Init(char *ServerIp,int BuffSize1,char *ClientIp,int BuffSize);
void WINAPI  DJLM_Svr_Quit();
void WINAPI  DJLM_Clt_Quit();
int WINAPI DJLM_Clt_SetMonSign(int Channel,int sign);
int WINAPI DJLM_Svr_GetMonInfo(int Channel,PDJLM_CHANMONINFO MonInfo);

int WINAPI DJLM_Svr_GetMonInfo_MoniterClientCount(int Channel);
char * WINAPI DJLM_Svr_GetMonInfo_MoniterClientIP(int Channel, int ClientIndex);
char *  WINAPI DJLM_Svr_GetMonInfo_MonStartTime(int Channel, int ClientIndex);
int  WINAPI DJLM_Svr_GetMonInfo_MonDura(int Channel, int ClientIndex);
int  WINAPI DJLM_Svr_GetMonInfo_IsMonStatus(int Channel, int ClientIndex);



#endif//