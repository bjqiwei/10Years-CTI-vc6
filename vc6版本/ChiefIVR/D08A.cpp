// D08A.cpp: implementation of the CD08A class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chiefivr.h"
#include "D08A.h"
#include "StructDefine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD08A::CD08A()
{
	
	memset( m_szKey,0, sizeof(m_szKey) );
	memset( m_szUserPin,0, sizeof(m_szUserPin) );
	memset( m_szPath,0, sizeof(m_szPath) );
	sprintf( (char*)m_szUserPin,"%s","42564980");
	sprintf( m_szPath,"%s","d120");
	m_strDeviceID = "";
	m_bOpen = FALSE;
	m_lRet = 0;
}

CD08A::~CD08A()
{
	//if( m_bOpen )
	//{
		m_lRet = S4VerifyPin(&m_ctx, m_szUserPin, 8, S4_USER_PIN);
		m_lRet = S4Control( &m_ctx ,S4_RESET_DEVICE,NULL,0,NULL,0,NULL);
		m_lRet = S4Close(&m_ctx);
	//}
}

BOOL CD08A::OpenSense4()
{
	
	SENSE4_CONTEXT *pctx = NULL;
	unsigned long size = 0;
		
	S4Enum(pctx, &size);
	if (size == 0)
	{
		m_bOpen = FALSE;
		return FALSE;
	}
	
	pctx = new SENSE4_CONTEXT[size];
	if (pctx == NULL)
	{
		m_bOpen = FALSE;
		return FALSE;
	}
	
	m_lRet = S4Enum(pctx, &size);
	if (m_lRet != S4_SUCCESS)
	{
		delete []pctx;
		pctx = NULL;
		m_bOpen = FALSE;
		return FALSE;
	}

	memcpy(&m_ctx, pctx, sizeof(SENSE4_CONTEXT));
	int i = 7; 
	char szTemp[3]={0};
	do
	{
		sprintf(szTemp,"%02x",m_ctx.bID[7-i]);
		m_strDeviceID += szTemp;
		i--;
	}while (i>=0);

	delete []pctx;
	pctx = NULL;
	
	S4OPENINFO pin;
	pin.dwS4OpenInfoSize = sizeof(S4OPENINFO);
	pin.dwShareMode = S4_EXCLUSIZE_MODE;
	m_lRet = S4OpenEx(&m_ctx, &pin);
	if (m_lRet != S4_SUCCESS)
	{
		m_bOpen = FALSE;
		return FALSE;
	}

	m_lRet = S4ChangeDir(&m_ctx, "\\");
	if (m_lRet != S4_SUCCESS)
	{
		S4Control( &m_ctx ,S4_RESET_DEVICE,NULL,0,NULL,0,NULL);
		S4Close(&m_ctx);
		m_bOpen = FALSE;
		return FALSE;
	}


	m_bOpen = TRUE;
	return TRUE;
}
BOOL CD08A::CompareDayu(int nKey1, int nKey2)
{
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_DAYU;
	sprintf( (char*)input.szParam1,"%d", nKey1 );
	sprintf( (char*)input.szParam2,"%d", nKey2 );
	sprintf( (char*)input.szKey,"%.23s", m_szKey );
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );

	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CD08A::CompareXiaoYu(int nKey1, int nKey2)
{
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_XIAOYU;
	sprintf( (char*)input.szParam1,"%d", nKey1 );
	sprintf( (char*)input.szParam2,"%d", nKey2 );
	sprintf( (char*)input.szKey,"%.23s", m_szKey );
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );

	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
BOOL CD08A::CompareVarKey(int nKey1, int nKey2)
{
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_COMPVARKEY;
	sprintf( (char*)input.szParam1,"%d", nKey1 );
	sprintf( (char*)input.szParam2,"%d", nKey2 );
	sprintf( (char*)input.szKey,"%.23s", m_szKey );
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );

	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

void CD08A::SetKey(const char *pszKey,int nBufLen )
{
	if( NULL == pszKey ) return;
	memcpy(m_szKey, pszKey,nBufLen );
}

BOOL CD08A::Call_sense4(char *fid, unsigned char *buff, int len)
{
	unsigned long size = 0;
	m_lRet = S4ChangeDir(&m_ctx, "\\");
	if (m_lRet != S4_SUCCESS)
	{
		return FALSE;
	}

	m_lRet = S4VerifyPin(&m_ctx, m_szUserPin, 8, S4_USER_PIN);
	if (m_lRet != S4_SUCCESS)
	{
		return FALSE;
	}
	
	m_lRet = S4Execute(&m_ctx, fid, buff, len, buff, len, &size);
	if (m_lRet != S4_SUCCESS)
	{
		return FALSE;
	}
	
	return TRUE;

}

BOOL CD08A::IsThisKey(const char *pszKey, int nBufLen )
{
	if( NULL == pszKey || nBufLen > 23 ) return FALSE;
	if( nBufLen < 1 ) return FALSE;
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_ISTHISKEY;
	memcpy( (char*)input.szKey, pszKey, nBufLen );
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );
	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
			return TRUE;
		else
			return FALSE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

int CD08A::GetChnlCount()
{
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_GET_CHNLCOUNT;
	sprintf( (char*)input.szKey,"%.23s", m_szKey );
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );

	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
			return atoi((const char *)Output.szParam1);
		else
			return -1;
	}
	else
	{
		return -1;
	}
	return -1;
}

BOOL CD08A::GetKey(char *pszKey, int buflen)
{
	if( NULL == pszKey ) return FALSE;
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_GET_KEY;
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );

	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
		{
			memcpy( pszKey, Output.szKey, buflen );
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

int CD08A::IsFax()
{
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_GET_ISFAX;
	sprintf( (char*)input.szKey,"%.23s", m_szKey );
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );
	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
			return Output.szParam1[0];
		else
			return Output.szParam1[0];
	}
	else
	{
		return Output.szParam1[0];
	}
	return Output.szParam1[0];
}

int CD08A::IsConfer()
{
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_GET_ISCONF;
	sprintf( (char*)input.szKey,"%.23s", m_szKey );
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );
	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
			return Output.szParam1[0];
		else
			return Output.szParam1[0];
	}
	else
	{
		return Output.szParam1[0];
	}
	return Output.szParam1[0];
}

int CD08A::GetSysType()
{
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_GET_SYSTYPE;
	sprintf( (char*)input.szKey,"%.23s", m_szKey );
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );
	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
			return Output.szParam1[0];
		else
			return Output.szParam1[0];
	}
	else
	{
		return Output.szParam1[0];
	}
	return Output.szParam1[0];
}

int CD08A::GetCardType()
{
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_GET_CARDTYPE;
	sprintf( (char*)input.szKey,"%.23s", m_szKey );
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );
	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
			return Output.szParam1[0];
		else
			return Output.szParam1[0];
	}
	else
	{
		return Output.szParam1[0];
	}
	return Output.szParam1[0];
}

char CD08A::GetCodeAscii(int code)
{
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_GET_DTMF;
	input.szParam1[0] = code;
	sprintf( (char*)input.szKey,"%.23s", m_szKey );
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );

	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
		{

			if( 'F' == Output.szParam1[0] )
			{
				return -1;
			}
			else
			{
				return Output.szParam1[0];
			}
		}
		else
			return -1;
	}
	else
	{
		return -1;
	}
	return Output.szParam1[0];
}

int CD08A::Get_AnalogTrunk()
{
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_ANALOGTRUNK;
	sprintf( (char*)input.szKey,"%.23s", m_szKey );
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );
	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
			return Output.szParam1[0];
		else
			return Output.szParam1[0];
	}
	else
	{
		return Output.szParam1[0];
	}
	return Output.szParam1[0];
}

int CD08A::Get_DigitalTrunk()
{
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_DIGITALTRUNK;
	sprintf( (char*)input.szKey,"%.23s", m_szKey );
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );
	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
			return Output.szParam1[0];
		else
			return Output.szParam1[0];
	}
	else
	{
		return Output.szParam1[0];
	}
	return Output.szParam1[0];
}

int CD08A::IsChangeVoice()
{
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_CHANGEVOICE;
	sprintf( (char*)input.szKey,"%.23s", m_szKey );
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );
	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
			return Output.szParam1[0];
		else
			return Output.szParam1[0];
	}
	else
	{
		return Output.szParam1[0];
	}
	return Output.szParam1[0];
}

long CD08A::GetVoiceCardKey()
{
	char kk[20];
	GetKey(kk,20);
	return atol( kk );
}



int CD08A::GetSS7PCMType()
{
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_SS7PCMTYPE;
	sprintf( (char*)input.szKey,"%.23s", m_szKey );
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );
	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
			return Output.szParam1[0];
		else
			return Output.szParam1[0];
	}
	else
	{
		return Output.szParam1[0];
	}
	return Output.szParam1[0];
}



BOOL CD08A::GetCompany( CString &strCompany )
{
	strCompany = "";
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_COMPANY_NAME;
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );

	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
		{
			strCompany = Output.szKey;
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CD08A::GetHeTongBianHao(CString &strHeTong)
{
	strHeTong = "";
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_HETONG_BIANHAO;
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );

	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
		{
			strHeTong = Output.szKey;
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CD08A::GetProuctID(CString &strID)
{
	strID = "";
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_GET_KEY;
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );

	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
		{
			strID = Output.szKey;
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CD08A::GetLicenceDate(CString &strData)
{
	strData = "";
	IO_PACKAGE input = {0};
	IO_PACKAGE Output = {0};
	input.cmd = CMD_DBL_XUKE_RIQI;
	unsigned char szBuf[IO_PACKAGE_BUF_SIZE];
	memset( szBuf, 0, sizeof(szBuf) );
	memcpy( szBuf, (unsigned char *)&input, IO_PACKAGE_BUF_SIZE );

	if( Call_sense4(m_szPath, szBuf, IO_PACKAGE_BUF_SIZE ) )
	{
		memcpy( &Output, ( char *)szBuf, IO_PACKAGE_BUF_SIZE );
		if( 1 == Output.szResult[0] )
		{
			strData = Output.szKey;
			return TRUE;
		}
		else
			return FALSE;
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}
