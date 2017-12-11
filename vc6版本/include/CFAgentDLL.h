#ifndef CFAGENTDLL_H
#define CFAGENTDLL_H

BOOL  WINAPI Agent_InitDLL( UINT Port, LPCSTR  IP );
BOOL  WINAPI Agent_SocketConnect( void);
BOOL  WINAPI Agent_ExitDLL(void);
void  WINAPI Agent_Reg(int nDN );
BOOL  WINAPI Agent_ReadMsg( int *pnMsgType, int *pnResult, LPSTR szMsg1, LPSTR szMsg2  );
void  WINAPI Agent_MakeCall( int nDN, int nCallType, LPCSTR szTel );
void  WINAPI Agent_AnswerCall(int nDN );
void  WINAPI Agent_GetActiveCall(int nDN );
void  WINAPI Agent_HangUp(int nDN );
void  WINAPI Agent_PickupHold(int nDN, LPCSTR szTel );
void  WINAPI Agent_Hold(int nDN );
void  WINAPI Agent_Transfer(int nDN ,int nCallType , LPCSTR szTel);
void  WINAPI Agent_Monitor(int nDN , LPCSTR szTel );
void  WINAPI Agent_BuildConfer(int nDN );
void  WINAPI Agent_ConferCall( int nDN, int nCallType, LPCSTR szTel );
void  WINAPI Agent_CancelTransfer(int nDN );
void  WINAPI Agent_To_Agent(int nDN, int nAimDN, LPSTR szMsg );
void  WINAPI Agent_DisMonitor(int nDN );
#endif 