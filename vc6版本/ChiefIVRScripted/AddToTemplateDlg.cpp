// AddToTemplateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "AddToTemplateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddToTemplateDlg dialog


CAddToTemplateDlg::CAddToTemplateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddToTemplateDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddToTemplateDlg)
	m_strName = _T("");
	//}}AFX_DATA_INIT
}


void CAddToTemplateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddToTemplateDlg)
	DDX_Control(pDX, IDC_EDIT_EXPRESS, m_ctrExpress);
	DDX_Control(pDX, IDC_COMBOBOXEX_ICO, m_cbexICO);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddToTemplateDlg, CDialog)
	//{{AFX_MSG_MAP(CAddToTemplateDlg)
	ON_CBN_SELENDCANCEL(IDC_COMBOBOXEX_ICO, OnSelendcancelComboboxexIco)
	ON_CBN_SELENDOK(IDC_COMBOBOXEX_ICO, OnSelendokComboboxexIco)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddToTemplateDlg message handlers

BOOL CAddToTemplateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	m_ctrExpress.SetWindowText( m_strExpress );
	SetNodeImageList( &m_ImageList );

	m_cbexICO.SetImageList(&m_ImageList);
	CString str("");
	
	
	for(int i = 0 ; i < 60 ; i++)
	{
		COMBOBOXEXITEM           cbi;//扩展组合框单元  
		ZeroMemory( &cbi, sizeof(cbi) );
		/*当不加清零的语句是在debug状态下能显示图标和文字，但在release下图标和文字均回不可见，
		当加了后，就显示正常了*/
		cbi.mask   =   CBEIF_IMAGE|CBEIF_INDENT|CBEIF_OVERLAY|CBEIF_SELECTEDIMAGE|CBEIF_TEXT ;   
		cbi.iItem   =   i;   
		cbi.iImage   =   i;   
		cbi.iSelectedImage   =   i;   
		str.Format( "Custom%d", i );
		cbi.pszText   =   (LPTSTR)(LPCTSTR)str;   
		m_cbexICO.InsertItem(&cbi);//插入单元   
	}	

	m_cbexICO.SetCurSel( m_nIco );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddToTemplateDlg::OnSelendcancelComboboxexIco() 
{

	
}

void CAddToTemplateDlg::OnSelendokComboboxexIco() 
{
    m_nIco  = m_cbexICO.GetCurSel();	
}

void CAddToTemplateDlg::OnOK() 
{
	m_ctrExpress.GetWindowText(m_strExpress);
	
	CDialog::OnOK();
}
