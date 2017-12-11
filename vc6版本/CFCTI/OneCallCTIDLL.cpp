
#include "stdafx.h"
#include "OneCallCTIDLL.h"
#include "MsqDowith.h"
#include "..//include//MSgPackDefine.h"
#include "..//include//MessageType.h"

extern CMsgDealWith *g_pMsgDealWith;

extern "C" _declspec(dllexport) bool CTI_InitDLL( unsigned int Port, const char* IP )
{
	bool bResult = false;

	if( NULL == IP )
		return false;

	if( NULL != g_pMsgDealWith )
	{
		g_pMsgDealWith->ExitInstance();
	}

	delete g_pMsgDealWith;
	g_pMsgDealWith = NULL;
	g_pMsgDealWith = new CMsgDealWith;
	if( NULL == g_pMsgDealWith  )
		return false;

	bResult = g_pMsgDealWith->Initialize(Port , IP,  0, 0 );

	return bResult;
}

extern "C" _declspec(dllexport) bool CTI_SocketConnect( void)
{
	bool bResult = false;
	if( NULL == g_pMsgDealWith )
		return false;
	bResult = g_pMsgDealWith->SocketConnect();

	return bResult;

}

extern "C" _declspec(dllexport) bool CTI_ExitDLL(void)
{
	if( NULL == g_pMsgDealWith )
		return true;
	
	delete g_pMsgDealWith;
	g_pMsgDealWith = NULL;


	return true;
}

extern "C" _declspec(dllexport) bool CTI_ReadData(char *buf, int nLen)
{
	bool bResult = false;
	if( NULL == g_pMsgDealWith )
		return false;
	if( !g_pMsgDealWith->SocketConnect() ) return false;
	bResult = g_pMsgDealWith->ReadData( buf, nLen);

	return bResult;
}


extern "C" _declspec(dllexport) bool CTI_SendData(char *buf, int nLen)
{
	bool bResult = false;
	if( NULL == g_pMsgDealWith )
		return false;
	if( !g_pMsgDealWith->SocketConnect() ) return false;
	bResult = g_pMsgDealWith->SendData( buf, nLen);
	return bResult;
}

extern "C" _declspec(dllexport) void CTI_RingingMsg(int nDN, const char *pszTel, const char *pszTel2)
{
	if( NULL == pszTel ) return;
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_RINGING;
	msg.nDMType		=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType		=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;


	CFMSG_RINGING newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_RINGING) );
	memmove( newMsg.szTel, pszTel, strlen(pszTel) > sizeof(newMsg.szTel) ? sizeof(newMsg.szTel) :  strlen(pszTel)  );
	memmove( newMsg.szTel2, pszTel2, strlen(pszTel2) > sizeof(newMsg.szTel2) ? sizeof(newMsg.szTel2) :  strlen(pszTel2)  );

	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_RINGING) );

	CTI_SendData( (char *)&msg, sizeof(msg));


	return;

}
extern "C" _declspec(dllexport) void CTI_MakeCallMsg(int nDN, int nResult, const char *pszTel)
{
	if( NULL == pszTel ) return;
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_MCALL;//reg message type
	msg.nDMType	=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType	=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;


	CFMSG_MAKECALL newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_MAKECALL) );

	newMsg.nResult = nResult;
	memmove( newMsg.szTel, pszTel, strlen(pszTel) > sizeof(newMsg.szTel) ? sizeof(newMsg.szTel) :  strlen(pszTel)  );
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_MAKECALL) );

	CTI_SendData( (char *)&msg, sizeof(msg));


	return;

}
extern "C" _declspec(dllexport) void CTI_MonitorMsg(int nDN, int nResult, char *pszTel)
{
	if( NULL == pszTel ) return;
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_MONITOR;//reg message type
	msg.nDMType	=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType	=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;


	CFMSG_MONITOR newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_MONITOR) );

	newMsg.nResult = nResult;
	memmove( newMsg.szTel, pszTel, strlen(pszTel) > sizeof(newMsg.szTel) ? sizeof(newMsg.szTel) :  strlen(pszTel)  );
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_MONITOR) );

	CTI_SendData( (char *)&msg, sizeof(msg));


	return;

}
extern "C" _declspec(dllexport) void CTI_DisMonitorMsg(int nDN, int nResult )
{

	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_DMONITOR;
	msg.nDMType	=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType	=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;


	CFMSG_MONITOR newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_MONITOR) );

	newMsg.nResult = nResult;
	
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_MONITOR) );

	CTI_SendData( (char *)&msg, sizeof(msg));


	return;

}
extern "C" _declspec(dllexport) void CTI_TransferMsg(int nDN, int nResult, char *pszTel)
{
	if( NULL == pszTel ) return;
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_TRANSFER;
	msg.nDMType	=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType	=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;


	CFMSG_TRANSFER newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_TRANSFER) );

	newMsg.nResult = nResult;
	memmove( newMsg.szTel, pszTel, strlen(pszTel) > sizeof(newMsg.szTel) ? sizeof(newMsg.szTel) :  strlen(pszTel)  );
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_TRANSFER) );

	CTI_SendData( (char *)&msg, sizeof(msg));


	return;
}

extern "C" _declspec(dllexport) void CTI_CancelTransferMsg(int nDN, int nResult, char *pszTel)
{
	if( NULL == pszTel ) return;
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_CANCEL_TRANSFER;
	msg.nDMType		=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType		=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;


	CFMSG_CANCEL_TRANSFER newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_CANCEL_TRANSFER) );
	newMsg.nResult = nResult;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_CANCEL_TRANSFER) );

	CTI_SendData( (char *)&msg, sizeof(msg));


	return;
}

extern "C" _declspec(dllexport) void CTI_ValidateMsg(int nDN )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_VALIDATE;
	msg.nDMType	=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType	=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;

	CTI_SendData( (char *)&msg, sizeof(msg));

	return;

}
extern "C" _declspec(dllexport) void CTI_HangUpMsg(int nDN )
{

	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_HANGUP;//reg message type
	msg.nDMType	=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType	=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;

	CTI_SendData( (char *)&msg, sizeof(msg));

	return;
}

extern "C" _declspec(dllexport) void CTI_OffHookMsg(int nDN )
{

	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_OFFHOOK;//reg message type
	msg.nDMType	=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType	=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;

	CTI_SendData( (char *)&msg, sizeof(msg));
	return;
}

extern "C" _declspec(dllexport) void CTI_AnswerCallMsg(int nDN, int nResult )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_ANSWER;
	msg.nDMType	=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType	=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;


	CFMSG_ANSWERCALL newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_ANSWERCALL) );
	newMsg.nResult = nResult;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_ANSWERCALL) );

	CTI_SendData( (char *)&msg, sizeof(msg));


	return;

}
extern "C" _declspec(dllexport) void CTI_HoldMsg(int nDN, int nResult )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_HOLD;
	msg.nDMType	=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType	=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;


	CFMSG_HOLD newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_HOLD) );
	newMsg.nResult = nResult;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_HOLD) );

	CTI_SendData( (char *)&msg, sizeof(msg));


	return;

}

extern "C" _declspec(dllexport) void CTI_GetActiveCallMsg(int nDN , int nResult ,const char *pszCallerID, const char *pszCalledID  )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_GETACTIVECALL;
	msg.nDMType	=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType	=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;


	CFMSG_GETACTIVECALL_RES newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_GETACTIVECALL_RES) );
	newMsg.nResult = nResult;
	memmove( newMsg.szCallerID, pszCallerID, strlen(pszCallerID) > sizeof(newMsg.szCallerID) ? sizeof(newMsg.szCallerID) :  strlen(pszCallerID)  );
	memmove( newMsg.szCalledID, pszCalledID, strlen(pszCalledID) > sizeof(newMsg.szCalledID) ? sizeof(newMsg.szCalledID) :  strlen(pszCalledID)  );

	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_GETACTIVECALL_RES) );

	CTI_SendData( (char *)&msg, sizeof(msg));


	return;

}
extern "C" _declspec(dllexport) void CTI_PickupHoldMsg(int nDN , int nResult,const char *pszCallerID, const char *pszCalledID  )
{

	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_PICKUPHOLD;
	msg.nDMType	=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType	=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;


	CFMSG_PICKUPHOLD_RES newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_PICKUPHOLD_RES) );
	newMsg.nResult = nResult;
	memmove( newMsg.szCallerID, pszCallerID, strlen(pszCallerID) > sizeof(newMsg.szCallerID) ? sizeof(newMsg.szCallerID) :  strlen(pszCallerID)  );
	memmove( newMsg.szCalledID, pszCalledID, strlen(pszCalledID) > sizeof(newMsg.szCalledID) ? sizeof(newMsg.szCalledID) :  strlen(pszCalledID)  );

	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_PICKUPHOLD_RES) );

	CTI_SendData( (char *)&msg, sizeof(msg));


	return;
}

extern "C" _declspec(dllexport) void CTI_RegToConmmServer()
{

	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(CFC_MESSAGE_PACK) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(msg);
	msg.nMsgType	=	CFMTYPE_REG_COMM;//reg message type

	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;
	msg.nSecDMType=	0;
	msg.nSecDModularID	=	0;


	CTI_SendData( (char *)&msg, sizeof(msg));


	return;

}

extern "C" _declspec(dllexport) void CTI_BuildConferMsg(int nDN, int nResult, int nConferNO )
{

	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_BUILD_CONFER;
	msg.nDMType		=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType		=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;


	CFMSG_BUILD_CONFER newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_BUILD_CONFER) );
	newMsg.nResult = nResult;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_BUILD_CONFER) );

	CTI_SendData( (char *)&msg, sizeof(msg));

	return;

}

extern "C" _declspec(dllexport) void CTI_JoinConference(int nDN , int nResult )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_INCONFER;
	msg.nDMType		=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType		=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;

	
	CFMSG_IN_CONFER newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_IN_CONFER) );
	newMsg.nResult		= nResult;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_IN_CONFER) );

	CTI_SendData( (char *)&msg, sizeof(msg));

	return;

}
extern "C" _declspec(dllexport) void CTI_ConferCall(int nDN , int nResult )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_CONFER_CALL;
	msg.nDMType		=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType		=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;

	
	CFMSG_CONFER_CALL newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_CONFER_CALL) );
	newMsg.nResult		= nResult;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_CONFER_CALL) );

	CTI_SendData( (char *)&msg, sizeof(msg));

	return;
}

extern "C" _declspec(dllexport) void CTI_Login(int nDN , int nResult )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_LOGIN;
	msg.nDMType		=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType		=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;

	
	CFMSG_LOGIN newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_LOGIN) );
	newMsg.nResult		= nResult;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_LOGIN) );

	CTI_SendData( (char *)&msg, sizeof(msg));

	return;
}
extern "C" _declspec(dllexport) void CTI_LogOut(int nDN , int nResult )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_LOGOUT;
	msg.nDMType		=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType		=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;

	
	CFMSG_LOGOUT newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_LOGOUT) );
	newMsg.nResult		= nResult;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_LOGOUT) );

	CTI_SendData( (char *)&msg, sizeof(msg));

	return;
}
extern "C" _declspec(dllexport) void CTI_Ready(int nDN , int nResult )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_SFR;
	msg.nDMType		=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType		=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;

	
	CFMSG_READY newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_READY) );
	newMsg.nResult		= nResult;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_READY) );

	CTI_SendData( (char *)&msg, sizeof(msg));

	return;
}
extern "C" _declspec(dllexport) void CTI_NotReady(int nDN , int nResult )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_SFNOTR;
	msg.nDMType		=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType		=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;

	
	CFMSG_NOT_READY newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_NOT_READY) );
	newMsg.nResult		= nResult;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_NOT_READY) );

	CTI_SendData( (char *)&msg, sizeof(msg));

	return;

}

extern "C" _declspec(dllexport) void CTI_SetBusy(int nDN , int nResult )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_MSETBBUSY;
	msg.nDMType		=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType		=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;

	
	CFMSG_SET_BUSY newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_SET_BUSY) );
	newMsg.nResult		= nResult;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_SET_BUSY) );

	CTI_SendData( (char *)&msg, sizeof(msg));

	return;
}

extern "C" _declspec(dllexport) void CTI_SetInServer(int nDN , int nResult )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_MSI;
	msg.nDMType		=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType		=	DMODULARTYPE_CTI;
	msg.nSMID		=	0;

	
	CFMSG_INSERVER newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_INSERVER) );
	newMsg.nResult		= nResult;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_INSERVER) );

	CTI_SendData( (char *)&msg, sizeof(msg));

	return;

}