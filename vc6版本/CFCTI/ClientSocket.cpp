// ClientSocket.cpp: implementation of the CClientSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ClientSocket.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CClientSocket::CClientSocket()
{

	m_bConnect		= false;
	m_nRecPos		= 0;
	m_nRevBytes		= 0;
	m_nReciveState	= CLIENT_SOCKET_STATE1;
	m_nSendPos		= 0;
	m_nSendBytes	= 0;
	m_nSendState	= CLIENT_SOCKET_STATE1;
	m_socket		= INVALID_SOCKET;
	m_bNetAbend		= false;
	memset((char *) &m_addr, 0, sizeof(m_addr));
	memset( m_szRecBuf, 0, CLIENT_SOCKET_BUF_LEN );
	memset( m_szSendBuf, 0, CLIENT_SOCKET_BUF_LEN );
	

}

CClientSocket::~CClientSocket()
{

	closesocket(m_socket);
	m_bConnect		= false;
	m_nRecPos		= 0;
	m_nRevBytes		= 0;
	m_nReciveState	= CLIENT_SOCKET_STATE1;
	m_nSendPos		= 0;
	m_nSendBytes	= 0;
	m_nSendState	= CLIENT_SOCKET_STATE1;
	m_socket		= INVALID_SOCKET;
	m_bNetAbend		= false;
	memset((char *) &m_addr, 0, sizeof(m_addr));
	memset( m_szRecBuf, 0, CLIENT_SOCKET_BUF_LEN );
	memset( m_szSendBuf, 0, CLIENT_SOCKET_BUF_LEN );
}

CClientSocket::CClientSocket(const CClientSocket& s)
{

	m_bConnect		= s.m_bConnect;
	m_nRecPos		= s.m_nRecPos;
	m_nRevBytes		= s.m_nRevBytes;
	m_nReciveState	= s.m_nReciveState;

	m_nSendPos		= s.m_nSendPos;
	m_nSendBytes	= s.m_nSendBytes;
	m_nSendState	= s.m_nSendState;
	m_socket		= s.m_socket;
	m_bNetAbend		= s.m_bNetAbend;
	memcpy( &m_addr, &(s.m_addr), sizeof(m_addr));
	memcpy( m_szRecBuf, s.m_szRecBuf, CLIENT_SOCKET_BUF_LEN );
	memcpy( m_szSendBuf, s.m_szSendBuf, CLIENT_SOCKET_BUF_LEN );

}

void CClientSocket::UpadteSendVariable()
{ 

	m_nSendPos		= 0;
	m_nSendBytes	= 0;
	m_nSendState	= CLIENT_SOCKET_STATE1;

	memset( m_szSendBuf, 0, CLIENT_SOCKET_BUF_LEN );

	return;

}

//更新接收数据的状态
void CClientSocket::UpadteReciveVariable()
{ 

	m_nRecPos		= 0;
	m_nRevBytes		= 0;
	m_nReciveState	= CLIENT_SOCKET_STATE1;
	
	return;
}

int CClientSocket::GetLastError()
{
	return WSAGetLastError();
}


//接收指定包头标志以及指定长度的数据，同时将数据返回，但不清空接收数据的缓冲区，
//可以使用ReadBuf成员函数去读数据，同时清空接收数据的缓冲区。
Client_SocketRecResult_T CClientSocket::Recive(char *Buf, char cIdentifier, int nBufLen)
{
	if( NULL == Buf )
		return Error;
	if( nBufLen <= 0 )
		return Error;
	if( nBufLen > CLIENT_SOCKET_BUF_LEN )
		return Error;

	int nerror = 0;
	
	switch( m_nReciveState )
	{
	case CLIENT_SOCKET_STATE1:
		//开始接收数据，初试化接收缓冲区
		UpadteReciveVariable();
		memset( m_szRecBuf, 0, CLIENT_SOCKET_BUF_LEN );
		m_nReciveState = CLIENT_SOCKET_STATE2;
		
		break;
	case CLIENT_SOCKET_STATE2:
		//正式开始接收数据
		m_nRevBytes = recv( m_socket, m_szRecBuf, 1 , 0 );
		if( 0 == m_nRevBytes )//the connection has been gracefully closed
		{
			Close();
			return Closed;
		}

		nerror =  WSAGetLastError();

		if ( nerror == WSAEWOULDBLOCK ) 
		{
			/*=====================================================================/
            We have already received the data. This error is returned from 
			operations on non-blocking sockets that cannot be completed immediately, 
			for example recv when no data is queued to be read from the socket.
			It is a non-fatal error
			=====================================================================*/
			m_nReciveState = CLIENT_SOCKET_STATE2;
            return Unfinishing;
		}

		if ( m_nRevBytes == SOCKET_ERROR )
		{
			Close();
			return Closed;

		}

		/*判断是否收到包头标志字符*/
		m_nRecPos += m_nRevBytes;
		if ( cIdentifier == m_szRecBuf[0] )
		{
			//收到后，进入下一状态
			m_nReciveState = CLIENT_SOCKET_STATE3;
			return Unfinishing;
		}
		else
		{
			//没有收到重新收
			UpadteReciveVariable();
		}
		break;
	case CLIENT_SOCKET_STATE3:
		m_nRevBytes = recv( m_socket, &m_szRecBuf[ m_nRecPos ], nBufLen - m_nRecPos , 0 );
		if( 0 == m_nRevBytes )//the connection has been gracefully closed
		{
			Close();
			return Closed;
		}
		
		nerror =  WSAGetLastError();

		if ( nerror == WSAEWOULDBLOCK ) 
		{
			/*=====================================================================/
            We have already received the data. This error is returned from 
			operations on non-blocking sockets that cannot be completed immediately, 
			for example recv when no data is queued to be read from the socket.
			It is a non-fatal error
			=====================================================================*/
			m_nReciveState = CLIENT_SOCKET_STATE3;
            return Unfinishing;
		}

		if ( m_nRevBytes == SOCKET_ERROR )
		{
			Close();
			return Closed;

		}

		//判断是否收完
		m_nRecPos += m_nRevBytes;
		if( m_nRecPos >= nBufLen )
		{
			memcpy( Buf, m_szRecBuf, nBufLen );
			UpadteReciveVariable();

			return Finishing;
		}
		break;
	default:
		UpadteReciveVariable();
		return Error;

	}//end switch

	return Unfinishing;
}

Client_SocketRecResult_T CClientSocket::Recive(char *Buf, int nBufLen)
{
	if( NULL == Buf )
		return Error;
	if( nBufLen <= 0 )
		return Error;
	if( nBufLen > CLIENT_SOCKET_BUF_LEN )
		return Error;

	int nerror = 0;

	switch( m_nReciveState )
	{
	case CLIENT_SOCKET_STATE1:
		//开始接收数据，初试化接收缓冲区
		UpadteReciveVariable();
		memset( m_szRecBuf, 0, CLIENT_SOCKET_BUF_LEN );
		m_nReciveState = CLIENT_SOCKET_STATE2;

		break;
	case CLIENT_SOCKET_STATE2:
		//正式开始接收数据
		m_nRevBytes = recv( m_socket, m_szRecBuf, nBufLen - m_nRecPos , 0 );
		if( 0 == m_nRevBytes )//the connection has been gracefully closed
		{
			Close();
			return Closed;
		}
		
		nerror =  WSAGetLastError();

		if ( nerror == WSAEWOULDBLOCK ) 
		{
			/*=====================================================================
            We have already received the data. This error is returned from 
			operations on non-blocking sockets that cannot be completed immediately, 
			for example recv when no data is queued to be read from the socket.
			It is a non-fatal error
			=====================================================================*/
			m_nReciveState = CLIENT_SOCKET_STATE2;
            return Unfinishing;
		}

		if ( m_nRevBytes == SOCKET_ERROR )
		{
			Close();
			return Closed;

		}


		//判断是否收完
		m_nRecPos += m_nRevBytes;
		if( m_nRecPos >= nBufLen )
		{
			memcpy( Buf, m_szRecBuf, nBufLen );
			UpadteReciveVariable();

			return Finishing;
		}

		break;
	case CLIENT_SOCKET_STATE3:
		//继续接收剩余的数据
		m_nRevBytes = recv( m_socket, &m_szRecBuf[ m_nRecPos ], nBufLen - m_nRecPos , 0 );
		if( 0 == m_nRevBytes )//the connection has been gracefully closed
		{
			Close();

			return Closed;
		}
		
		nerror =  WSAGetLastError();

		if ( nerror == WSAEWOULDBLOCK ) 
		{
			/*=====================================================================
            We have already received the data. This error is returned from 
			operations on non-blocking sockets that cannot be completed immediately, 
			for example recv when no data is queued to be read from the socket.
			It is a non-fatal error
			=====================================================================*/
			m_nReciveState = CLIENT_SOCKET_STATE2;
            return Unfinishing;
		}

		if ( m_nRevBytes == SOCKET_ERROR )
		{
			Close();
			return Closed;

		}

		//判断是否收完
		m_nRecPos += m_nRevBytes;
		if( m_nRecPos >= nBufLen )
		{
			m_nReciveState = CLIENT_SOCKET_STATE1;
			memcpy( Buf, m_szRecBuf, nBufLen );
			UpadteReciveVariable();

			return Finishing;
		}
		break;

	default:
		UpadteReciveVariable();

		return Error;
	}//end switch


	return Unfinishing;
}

Client_SocketRecResult_T CClientSocket::Send(char *Buf, int nBufLen)
{
	if( NULL == Buf )
		return Error;
	if( nBufLen <= 0 )
		return Error;
	if( nBufLen > CLIENT_SOCKET_BUF_LEN )
		return Error;
	int nerror = 0;
	
	
	switch( m_nSendState )
	{
	case CLIENT_SOCKET_STATE1:

		UpadteSendVariable();
		memcpy( m_szSendBuf, Buf ,nBufLen);
		m_nSendState = CLIENT_SOCKET_STATE2;

		break;
	case CLIENT_SOCKET_STATE2:
		m_nSendBytes = send( m_socket, &m_szSendBuf[ m_nSendPos ], nBufLen - m_nSendPos, 0 );
		
		if (m_nSendBytes == SOCKET_ERROR) 
		{
			nerror =  WSAGetLastError();
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
					/*============================================
                    Send buffers overflowed. 
					This error is returned from operations on non-blocking 
					sockets that cannot be completed immediately, 
					for example recv when no data is queued to be 
					read from the socket. It is a non-fatal error, 
					and the operation should be retried later. It 
					is normal for WSAEWOULDBLOCK to be reported as 
					the result from calling connect on a non-blocking 
					SOCK_STREAM socket, since some time must elapse 
					for the connection to be established. 
					==============================================*/
					m_nSendState = CLIENT_SOCKET_STATE2;

					return Unfinishing;
                    break;
             } 
			else
			{
				UpadteSendVariable();
				return Closed;
			}
		}

	
		/*判断是否发完*/
		m_nSendPos += m_nSendBytes;
		if( m_nSendPos >= nBufLen )
		{
			UpadteSendVariable();
			return Finishing;
		}
		break;
	default:
		UpadteSendVariable();
		break;

	}
		
	return Unfinishing;
}


int CClientSocket::Ioctlsocket(long cmd, u_long *argp)
{
	return ioctlsocket( m_socket,  cmd,  argp );
}

int CClientSocket::Close()
{
	m_bConnect = false;
	UpadteReciveVariable();
	UpadteSendVariable();

	return closesocket(m_socket);
}

void CClientSocket::SetAddr(sockaddr_in addr)
{
	memcpy( &m_addr, &(addr), sizeof(m_addr));
	return;
}

//从socket中读去收到的数据，不清空
bool CClientSocket::ReadBuf(char *buf, int bufLen)
{
	if( bufLen > CLIENT_SOCKET_BUF_LEN )
		return false;
	memcpy( buf, m_szRecBuf, bufLen );
	
	
	return true;
}

SOCKET CClientSocket::Create(int af, int type, int protocol)
{
	return socket(af,type,protocol);
}
