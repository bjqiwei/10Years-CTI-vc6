
#ifndef CLENTSOCKET_H
#define CLENTSOCKET_H


const int CLIENT_SOCKET_BUF_LEN = 241;//socket接收和发送数据缓冲区大小
/*==================================================================
note: CLIENT_SOCKET_BUF_LEN > sizeof(CFC_MESSAGE_PACK) + 1
===================================================================*/

//recive STATE const define
const int CLIENT_SOCKET_STATE1 = 0;//bengin recive
const int CLIENT_SOCKET_STATE2 = 1;
const int CLIENT_SOCKET_STATE3 = 2;

//define socket recive buf result enum type
typedef enum
{
	Unfinishing,
	Finishing,
	Closed,
	Error

}Client_SocketRecResult_T;


//==========================================================================//
//CClientSocket是一个非阻塞模式的Socket类，当使用Ioctlsocket(long cmd, u_long *argp)
//设置阻塞模式模式时一定得设为非阻塞模式
//
//==========================================================================//
class CClientSocket  
{
public:

	Client_SocketRecResult_T Recive(char *Buf, int nBufLen);
	Client_SocketRecResult_T Recive( char *Buf, char cIdentifier , int nBufLen );
	CClientSocket();
	virtual ~CClientSocket();
	CClientSocket(const CClientSocket& s); 
public:
	bool m_bNetAbend;
	SOCKET Create(int af, int type, int protocol );
	bool ReadBuf( char *buf, int bufLen );
	Client_SocketRecResult_T Send(char *Buf, int nBufLen);
	void SetAddr( sockaddr_in addr);
	int Close();
	int Ioctlsocket(long cmd, u_long *argp);
	bool m_bConnect;
	SOCKET m_socket;
	sockaddr_in m_addr;
private:
	void UpadteSendVariable();
	void UpadteReciveVariable();

	int GetLastError( );
private:
	int m_nRecPos ;
	int m_nRevBytes;
	int m_nReciveState;
	char m_szRecBuf[ CLIENT_SOCKET_BUF_LEN ];

	int m_nSendPos ;
	int m_nSendBytes;
	int m_nSendState;
	char m_szSendBuf[ CLIENT_SOCKET_BUF_LEN ];
	
};

#endif 
