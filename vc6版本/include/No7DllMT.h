#if !defined(_TCNO7_MT_)
#define _TCNO7_MT_

#include "No7Dll.h"

int		WINAPI t_DJNo7_InitSystem();
void	WINAPI t_DJNo7_ExitSystem();
//void	WINAPI t_DJNo7_GetEvent();

BYTE	WINAPI t_DJNo7_GetChnState( BYTE mo_num, BYTE dt_num );
void	WINAPI t_DJNo7_SetChnState( BYTE mo_num, BYTE dt_num, BYTE state );
void	WINAPI t_DJNo7_SetChnStateA( BYTE nPCM, BYTE nCHN, BYTE state, BYTE param ) ;

void	WINAPI t_DJNo7_GetCalleeNum( BYTE mo_num, BYTE dt_num, char* call_id );
void	WINAPI t_DJNo7_GetCallerNum( BYTE mo_num, BYTE dt_num, char* call_id );
char*	WINAPI t_DJNo7_GetCallerNumA(BYTE nPCM, BYTE nCHN);
char*	WINAPI t_DJNo7_GetCalleeNumA(BYTE nPCM, BYTE nCHN);

bool	WINAPI t_DJNo7_GetCalloutChn( BYTE* mo_num, BYTE* dt_num );
bool	WINAPI t_DJNo7_Callout( BYTE mo_num, BYTE dt_num, char* callee, char* caller = "" );
bool	WINAPI t_DJNo7_CalloutB( BYTE nPCM, BYTE nCHN, char* callee, char *caller );
BYTE	WINAPI t_DJNo7_CheckCalloutResult( BYTE mo_num, BYTE dt_num );
DWORD	WINAPI t_DJNo7_CheckCalloutSubResult( BYTE nPCM, BYTE nCHN );
bool	WINAPI t_DJNo7_CalloutA( BYTE mo_num, BYTE dt_num, BYTE* data, int len );
bool	WINAPI t_DJNo7_CalloutC( BYTE mo_num, BYTE dt_num, char * s_callee);
void	WINAPI t_DJNo7_GetCallInfo( BYTE mo_num, BYTE dt_num, char * info, int len );

void	WINAPI t_DJNo7_SetSystemMode( int mode , int param=0 );

void	WINAPI t_DJNo7_SetCalloutAParam( BYTE nPCM, BYTE nCHN, char* caller);

int		WINAPI t_DJNo7_GetRawFrame(int nLinkId, BYTE * strRawFrame);
bool	WINAPI t_DJNo7_SendRawFrame(BYTE nPCM, BYTE nCHN, BYTE* pData, int nLen);

void	WINAPI t_DJNo7_StartTimer(BYTE nPCM, BYTE nCHN, int nTimerValue);
int		WINAPI t_DJNo7_TimerElapsed(BYTE nPCM, BYTE nCHN);






#endif