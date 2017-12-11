#include "stdafx.h"
#include "iCfcComm.h"
#include "MsqDowith.h"

CMsgDealWith g_MsgDealWith;

extern "C" _declspec(dllexport) bool InitCFCommDLL( unsigned int Port, const char* IP, LPFUNCEXPORTDATA1 *p,  int nSMType, int nSMID )
{
	bool bResult = false;

	if( NULL == IP )
		return bResult;
	if( NULL == p )
		return bResult;
	bResult = g_MsgDealWith.Initialize(Port , p, nSMType, nSMID );
	return bResult;
}

extern "C" _declspec(dllexport) bool ExitCFCommDLL(void)
{

	g_MsgDealWith.ExitInstance();
	return true;
}

extern "C" _declspec(dllexport) bool ReadCommData(char *buf, int nLen)
{
	if( buf == NULL )
		return false;
	return g_MsgDealWith.ReadData( buf, nLen);
}


extern "C" _declspec(dllexport) bool SendCommData(char *buf, int nLen)
{
	if( buf == NULL )
		return false;
	return g_MsgDealWith.SendData( buf, nLen);
}
