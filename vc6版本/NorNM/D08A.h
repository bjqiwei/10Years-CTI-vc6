// D08A.h: interface for the CD08A class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_D08A_H__6B5CBE03_FA3A_404B_A912_39677347B9E6__INCLUDED_)
#define AFX_D08A_H__6B5CBE03_FA3A_404B_A912_39677347B9E6__INCLUDED_

#include "sense4.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CD08A  
{
public:
	BOOL GetLicenceDate( CString &strData );
	BOOL GetProuctID( CString &strID );
	BOOL GetHeTongBianHao( CString &strHeTong );
	BOOL GetCompany( CString &strCompany );
	BOOL CompareXiaoYu( int nKey1,int nKey2);
	BOOL CompareDayu( int nKey1,int nKey2);
	int GetSS7PCMType( void );
	long GetVoiceCardKey( void );
	int IsChangeVoice( void );
	int Get_DigitalTrunk( void );
	int Get_AnalogTrunk( void );
	char GetCodeAscii( int code );
	int GetCardType();
	int GetSysType( void );
	int IsConfer( void );
	int IsFax( void );
	BOOL GetKey( char *pszKey, int buflen );
	int GetChnlCount( void );
	BOOL IsThisKey( const char *pszKey , int nBufLen );
	BOOL Call_sense4(char *fid, unsigned char *buff, int len);
	void SetKey( const char *pszKey ,int nBufLen );
	BOOL CompareVarKey( int nKey1,int nKey2);
	BOOL OpenSense4( void );
	CD08A();
	virtual ~CD08A();

private:
	SENSE4_CONTEXT m_ctx;
	BOOL m_bOpen;
	CString m_strDeviceID;
	char m_szKey[24];
	char m_szPath[5];
	unsigned char m_szUserPin[9];
	unsigned long m_lRet;

};

#endif // !defined(AFX_D08A_H__6B5CBE03_FA3A_404B_A912_39677347B9E6__INCLUDED_)
