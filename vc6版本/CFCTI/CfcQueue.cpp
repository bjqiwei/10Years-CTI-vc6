// PsgQueue.cpp: implementation of the CCFQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "ListTest.h"
#include "CfcQueue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCFQueue::CCFQueue()
{

	pFree = NULL;//指向的节点当前是空的
	pHead = NULL;
	pLast = NULL;
	pHead = &m_Queue[0];
	pFree = &m_Queue[0];
	pHead->pPrev = NULL;
	pHead->pNext = &m_Queue[1];
	for( int i=1; i < CFCQUEUE_SIZE-1; i++ )
	{
		m_Queue[i].pPrev=&m_Queue[i-1];
		m_Queue[i].pNext=&m_Queue[i+1];
	}
	this->pLast=&m_Queue[CFCQUEUE_SIZE-1];
	this->pLast->pNext=NULL;
	this->pLast->pPrev=&m_Queue[CFCQUEUE_SIZE-2];
}

CCFQueue::~CCFQueue()
{
	pFree = NULL;//指向的节点当前是空的
	pHead = NULL;
	pLast = NULL;
	

}

/*
CCFQueue::CCFQueue( const CCFQueue &s)
{
	pFree = new 
	pFree = s.pFree;
	pHead = s.pHead;
	pLast = s.pLast;
	memcpy( m_Queue , s.m_Queue, CFCQUEUE_SIZE) ;

}
*/
/***************************************************
功能：取走队列的第一个
输入参数：
	无
输出参数：
	pszBuf，消息内容
	nBufLen，消息长度
返回值：成功返回真，失败返回假
***************************************************/
bool CCFQueue::front(char *pszBuf, int nBufLen )//
{
	CMsgNode *ptempNode=NULL;//临时节点指针
	if(pHead)
	{
		if(!pHead->bIfFree)//不为空
		{

			ptempNode=pHead;

			if( nBufLen > pHead->m_MsgRecord.m_nMsgLen )//new add  by ruize
			{
				memcpy(pszBuf,pHead->m_MsgRecord.m_szBuf,pHead->m_MsgRecord.m_nMsgLen);
			}
			else
			{
				memcpy(pszBuf,pHead->m_MsgRecord.m_szBuf, nBufLen );
			}
			//memcpy(pszBuf,pHead->m_MsgRecord.m_szBuf, nBufLen );
			//memcpy(pszBuf,pHead->m_MsgRecord.m_szBuf,pHead->m_MsgRecord.m_nMsgLen);
			//*nBufLen=pHead->m_MsgRecord.m_nMsgLen;
			pHead=pHead->pNext;//移动pHead指针，指向下一个节点
			pHead->pPrev=NULL;//使该节点成为头节

			//复位取走的节点内容，并把它移动到链表后面
			ptempNode->bIfFree=true;
			ptempNode->m_MsgRecord.m_nMsgLen=0;
			memset(ptempNode->m_MsgRecord.m_szBuf,0,CFCQUEUE_BUFSIZE);
			pLast->pNext=ptempNode;
			ptempNode->pPrev=pLast;//
			ptempNode->pNext=NULL;
			pLast=ptempNode;//移动pLast指针，指向最后一个节点

			//取走一个消息后，看看pFree指针是否为NULL(为NULL表示是在队列满的情况下取走的消息)
			if(!pFree)
				pFree=pLast;//指向最后一个节点
			
		}
		else
			return false;
	}
	else
		return false;
	return true;
}
/***************************************************
功能：往队列的后面加消息内容
输入参数：
	pszBuf，消息内容
	nBufLen，消息长度
输出参数：
返回值：成功为真，失败为假
***************************************************/

bool CCFQueue::push( const char * pszBuf ,int nBufLen )//
{
	if(nBufLen > CFCQUEUE_BUFSIZE)
		return false;
	if(pFree)//
	{
		if(pFree->bIfFree)
		{//保存消息到节点中
			pFree->bIfFree=false;//置为非空
			memcpy(pFree->m_MsgRecord.m_szBuf,pszBuf,nBufLen);
			pFree->m_MsgRecord.m_nMsgLen=nBufLen;
			pFree=pFree->pNext;//移动pPree指针到下一个节点
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}
/***************************************************
功能：判断队列是否为空
输入参数：无
输出参数：无
返回值：队列为空为真，不为空为假
***************************************************/

bool CCFQueue::empty()//
{
	if(pHead->bIfFree)//第一个节点都为空，该队列即为空
		return true;
	else
		return false;
}

void CCFQueue::Clear()
{
	pFree = NULL;//指向的节点当前是空的
	pHead = NULL;
	pLast = NULL;
	pHead = &m_Queue[0];
	pFree = &m_Queue[0];
	pHead->pPrev = NULL;
	pHead->pNext = &m_Queue[1];
	int i=1;
	for(  i=1; i < CFCQUEUE_SIZE-1; i++ )
	{
		m_Queue[i].Clear();
	}
	for(  i=1; i < CFCQUEUE_SIZE-1; i++ )
	{
		m_Queue[i].pPrev=&m_Queue[i-1];
		m_Queue[i].pNext=&m_Queue[i+1];
	}
	this->pLast=&m_Queue[CFCQUEUE_SIZE-1];
	this->pLast->pNext=NULL;
	this->pLast->pPrev=&m_Queue[CFCQUEUE_SIZE-2];
}
