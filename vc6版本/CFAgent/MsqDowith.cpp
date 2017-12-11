// MsqDowith.cpp: implementation of the CMsgDealWith class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <Afxmt.h>
#include "MsqDowith.h"
#include "ClientSocket.h"
#include "..//include//MSgPackDefine.h"
#include <fstream.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int MSGDEALWITH_SOCKET_STATE_BUFLEN = 300;//socket 状态事件缓冲长度

CCriticalSection section;

void WriteToLog(const char* szMsg) ;



UINT ClientRunModeSendProgress( LPVOID pParam )//Recive data thread
{

	CMsgDealWith* pMsgDealWith = ( CMsgDealWith*)pParam;
	if ( NULL == pMsgDealWith   )
	{
		return 0;   // if pMsgDealWith is not valid
	}
	section.Lock();
		pMsgDealWith->m_nThreadCount += 1;
	section.Unlock();
	
	CFC_MESSAGE_PACK MsgPackage;
	char szBuf[ CFCQUEUE_BUFSIZE ];
	int nLen = sizeof(MsgPackage);
	bool bTemp = false;

	memset( &MsgPackage, 0, sizeof(MsgPackage) );
	memset( szBuf, 0, CFCQUEUE_BUFSIZE );

	int nSendState = MSGDEALWITH_SEND_INIT_STATE;
	Client_SocketRecResult_T eSendResult;


	while(  !pMsgDealWith->m_bEndAllThread  )
	{
				
				section.Lock();
				bTemp = pMsgDealWith->m_CfcSocketClicent.m_bConnect;
				section.Unlock();
				if( !bTemp )
				{
				
					/*===========================================
					由于该socket已经断开了，其他线程就需要结束，
					同时需要关闭socket,所以需要设置线程结束开关，
					通知其他线程结束。
					============================================*/
					section.Lock();
						pMsgDealWith->m_CfcSocketClicent.Close();
						pMsgDealWith->m_nThreadCount -= 1;
					section.Unlock();
					return 0;
				}
				
				section.Lock();
				bTemp = pMsgDealWith->m_SendQueue.empty();
				section.Unlock();
				if( bTemp )
				{
					/*============================
					判断队列中是否有数据,空为真;
					等待数据去发送
					=============================*/
					Sleep(8);
					continue;
				}
				
				/*判断队列中是否有数据，如有读出数据，同时删除发送队列中的数据*/
				section.Lock();
				bTemp = pMsgDealWith->m_SendQueue.front( szBuf, nLen ) ;
				section.Unlock();
				if( !bTemp )
				{
					//读取数据失败
					Sleep(8);
					continue;
				}

				while( !pMsgDealWith->m_bEndAllThread )
				{
					section.Lock();
					eSendResult = pMsgDealWith->m_CfcSocketClicent.Send( szBuf,sizeof(MsgPackage) );
					section.Unlock();
					if( eSendResult == Unfinishing )
					{
						/*=======================================================
						Because of Send buffers overflowed, so must add the data
						sent by the thread back the queue.
						因为出现socket的发送缓冲区溢出，所以必须将该次发送的数据
						重新放回到队列，由于该数据从队列中读出时已删除，队列中至
						少还可以插入一个数据，所以一定能插入，等待下次发送。退出
						发送循环。
						========================================================*/
						
						Sleep(8);
						continue;

					}
					else if( eSendResult == Finishing )
					{	
						
						break;
					}
					else if( eSendResult == Closed )
					{
						break;
					}
					else if( eSendResult == Error )
					{	
						break;
					}
					else
					{
						break;
					}
				}//end while

				Sleep(8);

	}//end while

	section.Lock();
	pMsgDealWith->m_nThreadCount -= 1;
	section.Unlock();
	return 0;


}

UINT ClientRunModeReciveProgress( LPVOID pParam )//Recive data thread
{
	CMsgDealWith* pMsgDealWith = ( CMsgDealWith*)pParam;
	if ( NULL == pMsgDealWith   )
	{
		return 0;   // if pMsgDealWith is not valid
	}
	section.Lock();
		pMsgDealWith->m_nThreadCount += 1;
	section.Unlock();

	int i = 0;
	int nRevBytes = 0;
	char szTemp[1] = {'0'};
	bool bConnect = false;
	int nError = 0;

	char szRecMsg[ CFCQUEUE_BUFSIZE ];	//注意: 这个缓冲区的大小设为与队列的数据缓冲区的大小一直
	sockaddr_in tempSockAddr;			//临时sockaddr_in变量
	memset( szRecMsg, 0, CFCQUEUE_BUFSIZE);
	memset((char *) &tempSockAddr, 0, sizeof(tempSockAddr));

	int nReciveState = MSGDEALWITH_REC_INIT_STATE;
	bool bClientRegModule = false;
	while(  !pMsgDealWith->m_bEndAllThread  )
	{
		section.Lock();
		
		if( !pMsgDealWith->m_CfcSocketClicent.m_bConnect )
		{
		
			//设置线程结束开关，通知其他线程结束
			pMsgDealWith->m_CfcSocketClicent.Close();
			pMsgDealWith->m_nThreadCount -= 1;
			section.Unlock();
			
			return 0;
			
		}
		switch(nReciveState)
		{
		case MSGDEALWITH_REC_INIT_STATE:
			/***********************************************************************************************************************/
								switch( pMsgDealWith->m_CfcSocketClicent.Recive( szRecMsg, '\xFF', sizeof( CFC_MESSAGE_PACK ) ) )
								{
								case Unfinishing:
									nReciveState = MSGDEALWITH_REC_INIT_STATE;
									break;
								case Finishing:
									//将数据插入接收队列中
									if( pMsgDealWith->m_RecQueue.push( szRecMsg, CFCQUEUE_BUFSIZE ) )
									{
										nReciveState = MSGDEALWITH_REC_INIT_STATE;
									}
									else
									{
										
										//队列已满，进入下一状态，等待插入
										nReciveState = MSGDEALWITH_REC_MSG_STATE1;
									}
									//send socket state event
								
									//判断是不是来的模块注册消息，如是就注册
									if( !bClientRegModule )
									{
										if( pMsgDealWith->RegClientModuleInfo( szRecMsg, sizeof( CFC_MESSAGE_PACK ) ) )
										{
											//注册成功
											bClientRegModule = true;
										
										}
										else
										{
											bClientRegModule = true;
											
										}

									}

									
									break;
								case Closed:
									//send socket state event

									break;
								case Error:

									break;
								default:
									break;

								}//end switch
			/***********************************************************************************************************************/
			
			break;
		case MSGDEALWITH_REC_MSG_STATE1:
			/***********************************************************************************************************************/
								//将数据插入接收队列中,直到成功插入，然后开始下一次接收数据
								pMsgDealWith->m_CfcSocketClicent.ReadBuf( szRecMsg, CFCQUEUE_BUFSIZE );
								if( pMsgDealWith->m_RecQueue.push( szRecMsg, CFCQUEUE_BUFSIZE ) )
								{
									//插入成功
									nReciveState = MSGDEALWITH_REC_INIT_STATE;
								}

								//判断是不是来的模块注册消息，如是就注册
								if( !bClientRegModule )
								{
									if( pMsgDealWith->RegClientModuleInfo( szRecMsg, sizeof( CFC_MESSAGE_PACK ) ) )
									{
										bClientRegModule = true;
										
									}
									else
									{
										bClientRegModule = false;

									}

								}
			/***********************************************************************************************************************/
			
			break;
		default:
			
			break;

		}//end switch
		section.Unlock();
		Sleep(8);

	}//end while
	
	section.Lock();
		pMsgDealWith->m_nThreadCount -= 1;
	section.Unlock();
	return 0;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgDealWith::CMsgDealWith()
{
	m_bEndAllThread = false;
	m_nThreadCount  = 0;


	m_nSMType = DMODULARTYPE_BROKER;
	m_nSMID = 0;
	memset(&m_Addr, 0, sizeof(m_Addr));

}


CMsgDealWith::~CMsgDealWith()
{
	ExitInstance();

}


//Client run mode initalize
bool CMsgDealWith::Initialize(unsigned int Port, const char *IP, 	int nSMType, int nSMID )
{
	bool bResult = false;

	if (!AfxSocketInit())
	{

		return FALSE;
	}
	


	m_nSMID = nSMID;
	m_nSMType = nSMType;

	
	//Creat client socket
	m_Addr.sin_family = AF_INET;
    m_Addr.sin_port = (unsigned short)Port ;
    m_Addr.sin_addr.S_un.S_addr = inet_addr(IP);

	SOCKET cli_sock = INVALID_SOCKET;
	sockaddr_in  cli_addr;

	memset(&cli_addr,0,sizeof(cli_addr));
	cli_addr.sin_family=AF_INET;
	cli_addr.sin_addr.s_addr=INADDR_ANY;        
	cli_addr.sin_port=0;    

	cli_sock = socket(PF_INET,SOCK_STREAM,0);
	if (cli_sock==INVALID_SOCKET)
	{
		//AfxMessageBox("Create Socket Error !");
		return false;
	}

	if (bind(cli_sock,(LPSOCKADDR)&cli_addr,sizeof(cli_addr))==SOCKET_ERROR)
	{
		//AfxMessageBox("Bind socket Error!");
		closesocket(cli_sock);
		return false;
	}

	if (connect(cli_sock,(LPSOCKADDR)&m_Addr,sizeof(m_Addr))==SOCKET_ERROR)
	{ 

		//AfxMessageBox("connect false");
		closesocket(cli_sock);
		return false;
	}

	memcpy( &(m_CfcSocketClicent.m_addr), &cli_addr ,sizeof(cli_addr) );
	m_CfcSocketClicent.m_socket = cli_sock;

	/********************************************************/
	//注意：一定得设为异步的Socket，否则就会出现意想不到的错误
	unsigned long ioctl_opt = 1;
	m_CfcSocketClicent.Ioctlsocket(FIONBIO , &ioctl_opt );
	/******************************************************/

	m_CfcSocketClicent.m_bConnect = true;



	m_bEndAllThread = false;
	m_nThreadCount = 0;
	//Creat clent Recive data thread
	AfxBeginThread( ClientRunModeReciveProgress, this, NULL );

	//Creat clent Send data thread
	AfxBeginThread( ClientRunModeSendProgress, this, NULL );

	return true;
}

void CMsgDealWith::ExitInstance()
{

	m_CfcSocketClicent.Close();

	m_bEndAllThread = true;
	while(1)
	{
		if(m_nThreadCount<=0)
		{
			break;
		}
		Sleep(100);
	}

	memset(&m_Addr, 0, sizeof(m_Addr));
	

	m_nSMType = DMODULARTYPE_BROKER;
	m_nSMID = 0;

}



bool CMsgDealWith::ReadData(char *buf, int nLen)
{
	bool bHaveData = false;

	section.Lock();
	
	if( m_RecQueue.empty() )
	{
		//没有数据可读
		bHaveData = false;
	}
	else
	{
		if( m_RecQueue.front( buf, nLen ) )
		{
			//成功读取数据
			bHaveData =  true;
		}
		else
		{
			//没有读到数据
			bHaveData = false;
		}
	}


	section.Unlock();

	return bHaveData;
}

bool CMsgDealWith::SendData(char *buf, int nLen)
{
	bool bResult = false;

	section.Lock();

	if( m_SendQueue.push( buf,nLen) )
	{
		bResult = true;
	}
	else
	{
		bResult = false;
	}

	section.Unlock();


	return bResult;

}


bool CMsgDealWith::RegClientModuleInfo( char *buf, int nLen)
{

	if( buf == NULL )
		return false;


	bool bResult = true;
	CFC_MESSAGE_PACK MsgPackage;
	
	memcpy( &MsgPackage, buf, sizeof(MsgPackage));
	if( MSGDEALWITH_MOUAL_REG_MSG == MsgPackage.nMsgType )//判断是不是注册消息
	{

		m_CfcSocketClicent.m_nDMType = MsgPackage.nDMType;
		m_CfcSocketClicent.m_nDMID = MsgPackage.nDMID;
		m_CfcSocketClicent.m_nSMType = MsgPackage.nSMType;
		m_CfcSocketClicent.m_nSMID = MsgPackage.nSMID;
		m_CfcSocketClicent.m_nSecDMType  = MsgPackage.nSecDMType;
		m_CfcSocketClicent.m_nSecDMID  = MsgPackage.nSecDModularID;
		bResult = true;
	}
	else
	{
		bResult = false;
	}

	
	return bResult;
}

bool CMsgDealWith::SocketConnect()
{
	bool bResult = false;
	bResult = m_CfcSocketClicent.m_bConnect;
	return bResult;
}
