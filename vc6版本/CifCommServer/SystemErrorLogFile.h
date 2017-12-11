// SystemErrorLogFile.h: interface for the CSystemErrorLogFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMERRORLOGFILE_H__77F4C576_1FFA_48B2_A476_834CC9EFA640__INCLUDED_)
#define AFX_SYSTEMERRORLOGFILE_H__77F4C576_1FFA_48B2_A476_834CC9EFA640__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



class CSystemErrorLogFile  
{
public:
	
	void ClearFileContent();
	DWORD GetLength( );
	void Close();
	void WriteString( LPCTSTR lpsz );
	bool OpenLogFile( LPCTSTR lpszFileName );
	CSystemErrorLogFile();
	virtual ~CSystemErrorLogFile();

private:
	CString m_csFileName;
	CStdioFile m_ErrorLogFile;
};

#endif // !defined(AFX_SYSTEMERRORLOGFILE_H__77F4C576_1FFA_48B2_A476_834CC9EFA640__INCLUDED_)
