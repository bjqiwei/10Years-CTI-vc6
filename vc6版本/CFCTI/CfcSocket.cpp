// PsgSocket.cpp: implementation of the CCFSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "PsgTransmit.h"
#include "CfcSocket.h"
//#include "DataStructTypedef.h"

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

CCFSocket::CCFSocket( const CCFSocket& s)
{
	m_nDMType		= s.m_nDMType;
	m_nDMID		= s.m_nDMType;
	m_nSMType		= s.m_nDMType;
	m_nSMID		= s.m_nDMType;
	m_nSecDMType	= s.m_nDMType;
	m_nSecDMID	= s.m_nDMType;
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
