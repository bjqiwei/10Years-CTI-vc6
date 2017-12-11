#ifndef CFCQUEUE_H
#define CFCQUEUE_H


const int CFCQUEUE_BUFSIZE = 250;	//队列存储空间
/*======================================================
note: CFCQUEUE_BUFSIZE > sizeof(CFC_MESSAGE_PACK) + 1
=======================================================*/
const int CFCQUEUE_SIZE = 100;		//队列长度>=2

class CMsgRecord//消息内容
{
public:
	void Clear( void ){memset(m_szBuf,0,CFCQUEUE_BUFSIZE);
				m_nMsgLen=0;};
	CMsgRecord(){memset(m_szBuf,0,CFCQUEUE_BUFSIZE);
				m_nMsgLen=0;	};
	virtual ~CMsgRecord(){};
	//CMsgRecord( const CMsgRecord &s ){ memcpy( m_szBuf, s.m_szBuf, CFCQUEUE_BUFSIZE ); m_nMsgLen = s.m_nMsgLen; };
	char m_szBuf[CFCQUEUE_BUFSIZE];//消息缓冲区
	int	m_nMsgLen;//消息实际的长度
};

class CMsgNode
{
public:
	void Clear( void ){m_MsgRecord.Clear();
				pNext=NULL;pPrev=NULL;bIfFree=1;};
	CMsgNode(){pNext=NULL;pPrev=NULL;bIfFree=1;};
	virtual ~CMsgNode(){};
	//CMsgNode( const CMsgNode &s ){ pNext = s.pNext; pPrev = s.pPrev; bIfFree = s.bIfFree;  m_MsgRecord = s.m_MsgRecord; };
	CMsgNode *pNext;
	CMsgNode *pPrev;
	bool	   bIfFree;//标志此节点是否有实际内容，1表示没有实际内容，0表示有实际内容
	CMsgRecord m_MsgRecord;//消息内容
	
};


class CCFQueue  
{
public:
	CCFQueue();
	virtual ~CCFQueue();
	//CCFQueue( const CCFQueue &s);
public:
	void Clear( void );
	bool front(char *pszBuf, int nBufLen );//取走队列的第一个
	bool push( const char * pszBuf ,int nBufLen );//往队列的后面加消息内容
	bool empty();//判断队列是否为空
	CMsgNode *pFree;//空节点指针
	CMsgNode *pHead;
	CMsgNode *pLast;
	CMsgNode m_Queue[CFCQUEUE_SIZE];//队列节点的数组

};

#endif 
