// ReNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "ReNameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReNameDlg dialog


CReNameDlg::CReNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReNameDlg)
	m_strNewName = _T("");
	//}}AFX_DATA_INIT
	m_pObj = NULL;
}


void CReNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReNameDlg)
	DDX_Text(pDX, IDC_EDIT_NEWNAME, m_strNewName);
	DDV_MaxChars(pDX, m_strNewName, 60);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReNameDlg, CDialog)
	//{{AFX_MSG_MAP(CReNameDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReNameDlg message handlers

void CReNameDlg::Init(CNodeObj *pObj)
{
	if( NULL != pObj )
	{
		m_pObj = pObj;
		m_strNewName = m_pObj->m_strCaption;
	}
	return;
}

void CReNameDlg::OnOK() 
{
	UpdateData(TRUE);
	if( NULL != m_pObj )
	{
		m_pObj->m_strCaption = m_strNewName;
	}
	
	CDialog::OnOK();
}
