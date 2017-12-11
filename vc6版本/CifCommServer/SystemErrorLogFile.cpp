// SystemErrorLogFile.cpp: implementation of the CSystemErrorLogFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SystemErrorLogFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSystemErrorLogFile::CSystemErrorLogFile()
{

}

CSystemErrorLogFile::~CSystemErrorLogFile()
{

}

bool CSystemErrorLogFile::OpenLogFile(LPCTSTR lpszFileName)
{

	TRY
	{
		int nResult = 0;
		m_csFileName.Format("%s",lpszFileName);
		nResult = m_ErrorLogFile.Open(lpszFileName,CFile::modeNoTruncate|CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone);
		if( 0 == nResult )
			return false;
		else
			return true;
	}
	CATCH( CFileException, e )
	{
		return false;
	}
	END_CATCH
}

void CSystemErrorLogFile::WriteString(LPCTSTR lpsz)
{
	TRY
	{
		m_ErrorLogFile.WriteString(lpsz);
		m_ErrorLogFile.Flush();
	}
	CATCH( CFileException, e )
	{
		return ;
	}
	END_CATCH
}

void CSystemErrorLogFile::Close()
{
	TRY
	{
		m_ErrorLogFile.Close();
	}
	CATCH( CFileException, e )
	{
		return ;
	}
	END_CATCH
}

DWORD CSystemErrorLogFile::GetLength()
{
	TRY
	{
		return m_ErrorLogFile.GetLength();
	}
	CATCH( CFileException, e )
	{
		return 0;
	}
	END_CATCH
}

void CSystemErrorLogFile::ClearFileContent()
{
	TRY
	{
		m_ErrorLogFile.SetLength(0);
	}
	CATCH( CFileException, e )
	{
		return ;
	}
	END_CATCH
}