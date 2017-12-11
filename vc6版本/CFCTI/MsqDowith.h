// MsqDowith.h: interface for the CMsgDealWith class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSQDOWITH_H__671048B4_E40D_4F45_AF7E_990618B15633__INCLUDED_)
#define AFX_MSQDOWITH_H__671048B4_E40D_4F45_AF7E_990618B15633__INCLUDED_
//#include <afxmt.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "CfcSocket.h"
#include "CfcQueue.h"

#define CTIDLL_REC_TREADCEASE  WM_USER


// modual reg message type const
const int MSGDEALWITH_MOUAL_REG_MSG = 0x2000;


const int MSG_MAX_SOCKET_NUM = 1000;//max socket number


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
	//CMsgDealWith(const CMsgDealWith& s); 
public:

	void ExitInstance();
	bool Initialize(  unsigned int Port, const char *IP, 	int nSMType, int nSMID );
	bool RegClientModuleInfo( char *buf, int nLen);
	bool SendData(char *buf, int nLen);
	bool ReadData( char *buf, int nLen );

public:
	bool SocketConnect(void);

	CCFSocket m_CfcSocketClicent;
	sockaddr_in m_Addr;
	bool m_bEndAllThread;
	CCFQueue m_RecQueue;
	CCFQueue m_SendQueue;
	int m_nSMType ;
	int m_nSMID ;


private:
	HANDLE m_hReciveThreadhandle;//当前接收数据的线程句柄
	HANDLE m_hSendThreadhandle;//当前发送数据的线程句柄


	
};

#endif // !defined(AFX_MSQDOWITH_H__671048B4_E40D_4F45_AF7E_990618B15633__INCLUDED_)

