#ifndef ONECALLCTIDLL_H
#define ONECALLCTIDLL_H

extern "C" _declspec(dllexport) bool CTI_InitDLL( unsigned int Port, const char* IP );
extern "C" _declspec(dllexport) void CTI_RegToConmmServer(void);
extern "C" _declspec(dllexport) bool CTI_SocketConnect( void);
extern "C" _declspec(dllexport) bool CTI_ExitDLL(void);
extern "C" _declspec(dllexport) bool CTI_ReadData(char *buf, int nLen);
extern "C" _declspec(dllexport) bool CTI_SendData(char *buf, int nLen);
extern "C" _declspec(dllexport) void CTI_RingingMsg(int nDN, const char *pszTel, const char *pszTel2);
extern "C" _declspec(dllexport) void CTI_HangUpMsg(int nDN );
extern "C" _declspec(dllexport) void CTI_OffHookMsg(int nDN );
extern "C" _declspec(dllexport) void CTI_MakeCallMsg(int nDN, int nResult, const char *pszTel);
extern "C" _declspec(dllexport) void CTI_MonitorMsg(int nDN, int nResult, char *pszTel);
extern "C" _declspec(dllexport) void CTI_DisMonitorMsg(int nDN, int nResult );
extern "C" _declspec(dllexport) void CTI_TransferMsg(int nDN, int nResult, char *pszTel);
extern "C" _declspec(dllexport) void CTI_AnswerCallMsg(int nDN, int nResult );
extern "C" _declspec(dllexport) void CTI_HoldMsg(int nDN, int nResult );
extern "C" _declspec(dllexport) void CTI_PickupHoldMsg(int nDN , int nResult,const char *pszCallerID, const char *pszCalledID  );
extern "C" _declspec(dllexport) void CTI_BuildConferMsg(int nDN, int nResult, int nConferNO );
extern "C" _declspec(dllexport) void CTI_ConferCall(int nDN , int nResult );
extern "C" _declspec(dllexport) void CTI_CancelTransferMsg(int nDN, int nResult, char *pszTel);
extern "C" _declspec(dllexport) void CTI_GetActiveCallMsg(int nDN , int nResult ,const char *pszCallerID, const char *pszCalledID  );
extern "C" _declspec(dllexport) void CTI_JoinConference(int nDN , int nResult );

extern "C" _declspec(dllexport) void CTI_Login(int nDN , int nResult );
extern "C" _declspec(dllexport) void CTI_LogOut(int nDN , int nResult );
extern "C" _declspec(dllexport) void CTI_Ready(int nDN , int nResult );
extern "C" _declspec(dllexport) void CTI_NotReady(int nDN , int nResult );
extern "C" _declspec(dllexport) void CTI_ValidateMsg(int nDN );
extern "C" _declspec(dllexport) void CTI_SetBusy(int nDN , int nResult );
extern "C" _declspec(dllexport) void CTI_SetInServer(int nDN , int nResult );
#endif 