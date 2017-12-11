
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CFComm.h"
#include "CfcSocket.h"
#include "MSgPackDefine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCFSocket::CCFSocket()
{
	m_nDMType = 0;
	m_nDMID = 0;
	m_nSMType = 0;
	m_nSMID = 0;
	m_nSecDMType = 0 ;
	m_nSecDMID = 0 ; 

}

CCFSocket::~CCFSocket()
{

}



void CCFSocket::Initialize( )
{

	m_nDMType = 0;
	m_nDMID = 0;
	m_nSMType = 0;
	m_nSMID = 0;
	m_nSecDMType = 0 ;
	m_nSecDMID = 0 ; 
}
