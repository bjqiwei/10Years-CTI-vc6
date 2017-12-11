// MsqDowith.h: interface for the CMsgDealWith class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSQDOWITH_H__671048B4_E40D_4F45_AF7E_990618B15633__INCLUDED_)
#define AFX_MSQDOWITH_H__671048B4_E40D_4F45_AF7E_990618B15633__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CfcSocket.h"
#include "CfcQueue.h"
#include "iCfcComm.h"

typedef void(LPFUNCEXPORTDATA)( LPVOID lpData,int nDataType,int bufLen);


// modual reg message type const
const int MSGDEALWITH_MOUAL_REG_MSG = 0x2000;


const int MSG_MAX_SOCKET_NUM = 20;//max socket number


const int  MSGDEALWITH_REC_INIT_STATE		=	0X00;
const int  MSGDEALWITH_REC_MSG_STATE1		=	0X01;
const int  MSGDEALWITH_REC_MSG_STATE2		=	0X02;
const int  MSGDEALWITH_REC_MSG_STATE3		=	0X03;
const int  MSGDEALWITH_REC_ERROR_STATE		=	0X04;

const int  MSGDEALWITH_SEND_INIT_STATE		=	0X00;
const int  MSGDEALWITH_SEND_MSG_STATE1		=	0X01;
const int  MSGDEALWITH_SEND_MSG_STATE2		=	0X02;
const int  MSGDEALWITH_SEND_MSG_STATE3		=	0X03;
const int  MSGDEALWITH_SEND_ERROR_STATE		=	0X04;


class CMsgDealWith  
{
public:
	CMsgDealWith();
	virtual ~CMsgDealWith();


public:
	
	void ExitInstance();
	void ClearClientRegInfo(int nSocketIndex);
	bool Initialize( unsigned int Port, LPFUNCEXPORTDATA *p,	int nSMType, int nSMID );


	bool RegClientModuleInfo( int nSocketIndex, char * buf , int nLen );
	bool RegClientModuleInfo( char *buf, int nLen);
	bool SendData(char *buf, int nLen);
	bool ReadData( char *buf, int nLen );

public:
	void NetAbendDealWith( int i );
	
	CCFSocket m_cfcSocketClicent;


	sockaddr_in m_Addr;

	SOCKET m_ListenSocket ;

	bool m_bEndAllThread;

	CCFQueue m_RecQueue;
	CCFQueue m_SendQueue;
	LPFUNCEXPORTDATA *m_lpfuncExport;

	CCFSocket m_NewSocket[ MSG_MAX_SOCKET_NUM ];
	int m_nReciveState[ MSG_MAX_SOCKET_NUM ];
	int m_nSendState[ MSG_MAX_SOCKET_NUM ];
	bool m_bClientRegModule[ MSG_MAX_SOCKET_NUM ];

	int m_nSMType ;
	int m_nSMID ;
	int m_nThreadCount;
private:
	void InitVar();

	
};

#endif // !defined(AFX_MSQDOWITH_H__671048B4_E40D_4F45_AF7E_990618B15633__INCLUDED_)

