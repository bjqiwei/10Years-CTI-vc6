
#include "stdafx.h"
#include "CFAgentDLL.h"
#include "MsqDowith.h"
#include "..//include//MSgPackDefine.h"


extern CMsgDealWith *g_pMsgDealWith;

BOOL  WINAPI Agent_InitDLL( UINT Port, LPCSTR  IP )
{
	BOOL bResult = FALSE;

	if( NULL == IP )
		return FALSE;
	delete g_pMsgDealWith;
	g_pMsgDealWith = NULL;
	g_pMsgDealWith = new CMsgDealWith;
	if( NULL == g_pMsgDealWith  )
		return FALSE;

	bResult = g_pMsgDealWith->Initialize(Port , IP,  0, 0 );

	return bResult;
}

BOOL  WINAPI Agent_SocketConnect( void)
{
	BOOL bResult = FALSE;
	if( NULL == g_pMsgDealWith )
		return false;
	bResult = g_pMsgDealWith->SocketConnect();

	return bResult;

}

BOOL  WINAPI Agent_ExitDLL(void)
{
	if( NULL == g_pMsgDealWith )
		return true;
	delete g_pMsgDealWith;
	g_pMsgDealWith = NULL;

	return true;
}

BOOL  Agent_ReadData(LPSTR buf, int nLen)
{
	bool bResult = false;
	if( NULL == g_pMsgDealWith )
		return false;
	bResult = g_pMsgDealWith->ReadData( buf, nLen);

	return bResult;
}


BOOL   Agent_SendData(LPSTR buf, int nLen)
{
	bool bResult = false;
	if( NULL == g_pMsgDealWith )
		return false;
	
	bResult = g_pMsgDealWith->SendData( buf, nLen);
	return bResult;
}


void  WINAPI Agent_Reg(int nDN )
{

	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(CFC_MESSAGE_PACK) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(msg);
	msg.nMsgType	=	CFMTYPE_REG_COMM;//reg message type
	msg.nDMType	=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nDN;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	nDN;
	msg.nSecDMType=	0;
	msg.nSecDModularID	=	0;


	Agent_SendData( (char *)&msg, sizeof(msg));

	return;
}

BOOL  WINAPI Agent_ReadMsg( int *pnMsgType, int *pnResult, LPSTR szMsg1, LPSTR szMsg2  )
{
	if( NULL == szMsg1 || NULL == szMsg2 )
	{
		return FALSE;
	}

	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(CFC_MESSAGE_PACK) );
	sprintf( szMsg1,"%s","");
	sprintf( szMsg2,"%s","");
	
	if( !Agent_ReadData( (char*)&msg, sizeof(CFC_MESSAGE_PACK) ) ) 
		return FALSE;
	
	*pnMsgType = msg.nMsgType;
	switch( *pnMsgType )
	{
		case CFMTYPE_MONITOR://Monitoring
			{
				CFMSG_MONITOR recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_MONITOR) );
				*pnResult = recMsg.nResult;
			}
			break;
		case CFMTYPE_DMONITOR://Disable Monitoring
			{
				CFMSG_MONITOR recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_MONITOR) );
				*pnResult = recMsg.nResult;
			}
			break;
		case CFMTYPE_MCALL://Make Call
			{
				CFMSG_MAKECALL recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_MAKECALL) );
				*pnResult = recMsg.nResult;
			}
			break;
		case CFMTYPE_INCONFER://In Conference
			{

			}
			break;
		case CFMTYPE_LCONFER://Leave Conference 
			{

			}
			break;
		case CFMTYPE_TRANSFER://Transfer
			{
				CFMSG_TRANSFER recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_TRANSFER) );
				*pnResult = recMsg.nResult;
			}
			break;
		case CFMTYPE_CANCEL_TRANSFER:
			{
				CFMSG_CANCEL_TRANSFER recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_CANCEL_TRANSFER) );
				*pnResult = recMsg.nResult;
			}
			break;
		case CFMTYPE_ANSWER://Answer
			{
				CFMSG_ANSWERCALL recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_ANSWERCALL) );
				*pnResult = recMsg.nResult;
			}
			break;
		case CFMTYPE_RELEASECALL://Release Call
			{

			}
			break;
		case CFMTYPE_HOLD://Hold Request
			{
				CFMSG_HOLD recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_HOLD) );
				*pnResult = recMsg.nResult;
			}
			break;
		case CFMTYPE_PICKUPHOLD://Pickup Hold Request
			{
				CFMSG_PICKUPHOLD recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_PICKUPHOLD) );
				*pnResult = recMsg.nResult;
			}
			break;
		case CFMTYPE_SENDFAX://Send Fax Request
			{

			}
			break;
		case CFMTYPE_LOGIN://Set Feature Login
			{

			}
			break;
		case CFMTYPE_LOGOUT://Set Feature Logout
			{

			}
			break;
		case CFMTYPE_MSETBBUSY://Set Feature MSB
			{

			}
			break;
		case CFMTYPE_MSI://Set Feature MSI
			{

			}
			break;
		case CFMTYPE_SFNOTR://Set Feature Not Ready
			{

			}
			break;
		case CFMTYPE_SFR://Set Feature  Ready
			{

			}
			break;
		case CFMTYPE_RINGING://Ringing Response
			{
				CFMSG_RINGING recMsg;
				memset(&recMsg, '\0', sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_RINGING) );
				memmove( szMsg1, recMsg.szTel , 32 );
				memmove( szMsg2, recMsg.szTel2 , 32 );

			}
			break;
		case CFMTYPE_HANGUP://Hangup event
			break;
		case CFMTYPE_OFFHOOK://OffHook event
			break;
		case CFMTYPE_REG_COMM://reg to Comm and response
			break;
		case CFMTYPE_NET_ABEND://Net abend message type
			break;
		case CFMTYPE_CONFER_CALL:
			{
				
				CFMSG_CONFER_CALL recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_CONFER_CALL) );
				

			}
			break;
		case CFMTYPE_BUILD_CONFER:
			{
				
				CFMSG_BUILD_CONFER recMsg;
				memset(&recMsg, 0, sizeof(recMsg) );
				memmove( &recMsg, msg.szMessageBody ,sizeof(CFMSG_RINGING) );
				sprintf( szMsg1, "%d", recMsg.nConferNO );

			}
			break;
		default:
			return  FALSE;
			break;
	}
	
	return TRUE;
}


void  WINAPI Agent_MakeCall( int nDN, int nCallType, LPCSTR szTel )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(msg);
	msg.nMsgType	=	CFMTYPE_MCALL;//reg message type
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	nDN;


	CFMSG_MAKECALL recMsg;
	memset(&recMsg, 0, sizeof(recMsg) );
	recMsg.nCallType = nCallType;
	memmove( recMsg.szTel, szTel, strlen(szTel) > sizeof(recMsg.szTel) ? sizeof(recMsg.szTel) :  strlen(szTel)  );
	memmove( msg.szMessageBody , &recMsg, sizeof(CFMSG_MAKECALL) );

	Agent_SendData( (char *)&msg, sizeof(msg));


	return;

}

void  WINAPI Agent_GetActiveCall(int nDN )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_GETACTIVECALL;
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	nDN;


	CFMSG_GETACTIVECALL newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_ANSWERCALL) );
	newMsg.nResult = 0;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_ANSWERCALL) );

	Agent_SendData( (char *)&msg, sizeof(msg));


	return;

}

void  WINAPI Agent_AnswerCall(int nDN )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_ANSWER;//reg message type
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	nDN;


	CFMSG_ANSWERCALL newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_ANSWERCALL) );
	newMsg.nResult = 0;
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_ANSWERCALL) );

	Agent_SendData( (char *)&msg, sizeof(msg));


	return;

}

void  WINAPI Agent_HangUp(int nDN )
{

	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_HANGUP;
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	nDN;

	Agent_SendData( (char *)&msg, sizeof(msg));

	return;
}
void  WINAPI Agent_Hold(int nDN )
{

	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_HOLD;
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	nDN;

	
	CFMSG_HOLD newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_HOLD) );
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_HOLD) );

	Agent_SendData( (char *)&msg, sizeof(msg));

	return;
}



void  WINAPI Agent_PickupHold(int nDN, LPCSTR szTel )
{

	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_PICKUPHOLD;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	nDN;

	
	CFMSG_PICKUPHOLD newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_PICKUPHOLD) );
	memmove( newMsg.szTel, szTel, strlen(szTel) > sizeof(newMsg.szTel) ? sizeof(newMsg.szTel) :  strlen(szTel)  );
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_PICKUPHOLD) );

	Agent_SendData( (char *)&msg, sizeof(msg));

	return;
}
void  WINAPI Agent_Transfer(int nDN ,int nCallType , LPCSTR szTel  )
{
	if( NULL == szTel ) return;
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_TRANSFER;
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	nDN;

	
	CFMSG_TRANSFER newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_TRANSFER) );
	newMsg.nCallType = nCallType;
	memmove( newMsg.szTel, szTel, strlen(szTel) > sizeof(newMsg.szTel) ? sizeof(newMsg.szTel) :  strlen(szTel)  );
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_TRANSFER) );
	Agent_SendData( (char *)&msg, sizeof(msg));

	return;
}

void  WINAPI Agent_CancelTransfer(int nDN )
{

	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_CANCEL_TRANSFER;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	nDN;

	
	CFMSG_CANCEL_TRANSFER newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_CANCEL_TRANSFER) );
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_CANCEL_TRANSFER) );
	Agent_SendData( (char *)&msg, sizeof(msg));

	return;

}
void  WINAPI Agent_Monitor(int nDN , LPCSTR szTel )
{
	if( NULL == szTel ) return;
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_MONITOR;
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	nDN;

	
	CFMSG_MONITOR newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_MONITOR) );
	memmove( newMsg.szTel, szTel, strlen(szTel) > sizeof(newMsg.szTel) ? sizeof(newMsg.szTel) :  strlen(szTel)  );
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_MONITOR) );

	Agent_SendData( (char *)&msg, sizeof(msg));

	return;
}


void  WINAPI Agent_BuildConfer(int nDN  )
{

	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier		=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_BUILD_CONFER;
	msg.nDMType	=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType	=	DMODULARTYPE_AGENT;
	msg.nSMID		=	nDN;



	CFMSG_BUILD_CONFER newMsg;
	memset(&newMsg, 0, sizeof(CFMSG_BUILD_CONFER) );
	memmove( msg.szMessageBody , &newMsg, sizeof(CFMSG_BUILD_CONFER) );

	Agent_SendData( (char *)&msg, sizeof(msg));

	return;
}




void  WINAPI Agent_ConferCall( int nDN, int nCallType, LPCSTR szTel )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(msg);
	msg.nMsgType	=	CFMTYPE_CONFER_CALL;
	msg.nDMType		=	DMODULARTYPE_CTI;
	msg.nDMID		=	0;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	nDN;


	CFMSG_CONFER_CALL recMsg;
	memset(&recMsg, 0, sizeof(recMsg) );
	recMsg.nCallType = nCallType;
	memmove( recMsg.szTel, szTel, strlen(szTel) > sizeof(recMsg.szTel) ? sizeof(recMsg.szTel) :  strlen(szTel)  );
	memmove( msg.szMessageBody , &recMsg, sizeof(CFMSG_CONFER_CALL) );

	Agent_SendData( (char *)&msg, sizeof(msg));


	return;

}
void  WINAPI Agent_To_Agent(int nDN, int nAimDN, LPSTR szMsg )
{
	CFC_MESSAGE_PACK msg;
	memset(&msg, 0, sizeof(msg) );

	msg.cIdentifier	=	'\xFF';
	msg.nMsgLen		=	sizeof(CFC_MESSAGE_PACK);
	msg.nMsgType	=	CFMTYPE_AGENT_TO_AGENT;
	msg.nDMType		=	DMODULARTYPE_AGENT;
	msg.nDMID		=	nAimDN;
	msg.nSMType		=	DMODULARTYPE_AGENT;
	msg.nSMID		=	nDN;

	memmove( msg.szMessageBody, szMsg, strlen(szMsg) > sizeof(msg.szMessageBody) ? sizeof(msg.szMessageBody) :  strlen(szMsg)  );
	Agent_SendData( (char *)&msg, sizeof(msg));
	return;
}
