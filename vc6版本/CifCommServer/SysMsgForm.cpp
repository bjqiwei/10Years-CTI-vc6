// SysMsgForm.cpp : implementation file
//

#include "stdafx.h"
#include "CifCommServer.h"
#include "SysMsgForm.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSysMsgForm  *g_pSysMsgFrom = NULL;
extern CMainFrame 	*g_pMainFram;

UINT Progress_AutoRun( LPVOID pParam )
{
	if( g_pMainFram->m_nRunMode != 0 )//auto run
	{
		g_pMainFram->SysRun();
	}
	else
	{
		g_pMainFram->m_bSysRun = TRUE;
	}

	return 0;

}

/////////////////////////////////////////////////////////////////////////////
// CSysMsgForm

IMPLEMENT_DYNCREATE(CSysMsgForm, CFormView)

CSysMsgForm::CSysMsgForm()
	: CFormView(CSysMsgForm::IDD)
{
	//{{AFX_DATA_INIT(CSysMsgForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	g_pSysMsgFrom = this;
}

CSysMsgForm::~CSysMsgForm()
{
}

void CSysMsgForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysMsgForm)
	DDX_Control(pDX, IDC_SYSMSG_LIST, m_lstSysMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysMsgForm, CFormView)
	//{{AFX_MSG_MAP(CSysMsgForm)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysMsgForm diagnostics

#ifdef _DEBUG
void CSysMsgForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CSysMsgForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSysMsgForm message handlers

void CSysMsgForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	if( 1 != m_InitialUpdate )
	{
		m_InitialUpdate=1;
		AfxBeginThread( Progress_AutoRun, this, NULL );
		m_lstSysMsg.MoveWindow(0,20,m_cx,m_cy-20);
		CreateSysLogFile();
	}
	
}

void CSysMsgForm::OnSize(UINT nType, int cx, int cy) 
{
	m_cx=cx;
	m_cy=cy;
	if(m_InitialUpdate==1)
	{

		m_lstSysMsg.MoveWindow(0,20,m_cx,m_cy-20);

	}
	
}

void CSysMsgForm::WriteStringToLogFile(LPCTSTR lpsz)
{
	if( m_nIsSaveSysLog > 0  )
	{
		if(m_SystemErrorLogFile.GetLength() > 402400 )
		{
			m_SystemErrorLogFile.Close();
			CreateSysLogFile();
		}
		CString csTemp = "";
		csTemp.Format("\t\n%s",lpsz);
		m_SystemErrorLogFile.WriteString( (LPCTSTR) csTemp );
	}
}

void CSysMsgForm::OnClose() 
{
	if( m_nIsSaveSysLog > 0  )
	{
		m_SystemErrorLogFile.Close();
	}
	
	CFormView::OnClose();
}

void CSysMsgForm::CreateSysLogFile()
{
	m_nIsSaveSysLog = GetPrivateProfileInt("SystemOption","IsSaveSysLog",0,"System\\SystemSet.ini");
	
	if( m_nIsSaveSysLog > 0  )
	{
		CString csFileName = "";
		csFileName = "CommSlog\\Sys" + (CTime::GetCurrentTime()).Format("%Y%m%d%H%M%S") + ".txt" ;

		if( !m_SystemErrorLogFile.OpenLogFile( (LPCTSTR )csFileName ) )
		{
			AfxMessageBox("Creat Logfile False ! ");
			m_nIsSaveSysLog = 0;
		}
	}
}
