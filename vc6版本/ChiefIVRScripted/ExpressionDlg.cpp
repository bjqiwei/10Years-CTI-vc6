// ExpressionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "ExpressionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExpressionDlg dialog


CExpressionDlg::CExpressionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExpressionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExpressionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CExpressionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExpressionDlg)
	DDX_Control(pDX, IDC_TREE_SELECT, m_treeSelect);
	DDX_Control(pDX, IDC_EDIT_EXPRESS, m_edtExpress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExpressionDlg, CDialog)
	//{{AFX_MSG_MAP(CExpressionDlg)
	ON_BN_CLICKED(IDC_BUTTON_XIAOYUDENGYU, OnButtonXiaoyudengyu)
	ON_BN_CLICKED(IDC_BUTTON_XIAOYU, OnButtonXiaoyu)
	ON_BN_CLICKED(IDC_BUTTON_STRJIA, OnButtonStrjia)
	ON_BN_CLICKED(IDC_BUTTON_JIAN, OnButtonJian)
	ON_BN_CLICKED(IDC_BUTTON_JIA, OnButtonJia)
	ON_BN_CLICKED(IDC_BUTTON_DENGYU, OnButtonDengyu)
	ON_BN_CLICKED(IDC_BUTTON_DAYUDENDYU, OnButtonDayudendyu)
	ON_BN_CLICKED(IDC_BUTTON_DAYU, OnButtonDayu)
	ON_BN_CLICKED(IDC_BUTTON_CHU, OnButtonChu)
	ON_BN_CLICKED(IDC_BUTTON_CHENG, OnButtonCheng)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExpressionDlg message handlers

void CExpressionDlg::OnButtonXiaoyudengyu() 
{
	// TODO: Add your control notification handler code here
	
}

void CExpressionDlg::OnButtonXiaoyu() 
{
	// TODO: Add your control notification handler code here
	
}

void CExpressionDlg::OnButtonStrjia() 
{
	// TODO: Add your control notification handler code here
	
}

void CExpressionDlg::OnButtonJian() 
{
	// TODO: Add your control notification handler code here
	
}

void CExpressionDlg::OnButtonJia() 
{
	// TODO: Add your control notification handler code here
	
}

void CExpressionDlg::OnButtonDengyu() 
{
	// TODO: Add your control notification handler code here
	
}

void CExpressionDlg::OnButtonDayudendyu() 
{
	// TODO: Add your control notification handler code here
	
}

void CExpressionDlg::OnButtonDayu() 
{
	// TODO: Add your control notification handler code here
	
}

void CExpressionDlg::OnButtonChu() 
{
	// TODO: Add your control notification handler code here
	
}

void CExpressionDlg::OnButtonCheng() 
{
	// TODO: Add your control notification handler code here
	
}
