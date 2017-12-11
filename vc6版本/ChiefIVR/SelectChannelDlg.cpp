// SelectChannelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivr.h"
#include "SelectChannelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectChannelDlg dialog


CSelectChannelDlg::CSelectChannelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectChannelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectChannelDlg)
	m_nChannelType = 10;
	m_nBeginNo = 0;
	m_nEndNo = 0;
	//}}AFX_DATA_INIT
}


void CSelectChannelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectChannelDlg)
	DDX_CBIndex(pDX, IDC_COMBO_CHNLTYPE, m_nChannelType);
	DDX_Text(pDX, IDC_EDIT_BEGINNO, m_nBeginNo);
	DDV_MinMaxUInt(pDX, m_nBeginNo, 0, 3000);
	DDX_Text(pDX, IDC_EDIT_ENDNO, m_nEndNo);
	DDV_MinMaxUInt(pDX, m_nEndNo, 0, 3000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectChannelDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectChannelDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectChannelDlg message handlers
