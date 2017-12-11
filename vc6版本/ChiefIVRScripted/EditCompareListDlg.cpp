// EditCompareListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DrawDoc.h"
#include "chiefivrscripted.h"
#include "EditCompareListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditCompareListDlg dialog


CEditCompareListDlg::CEditCompareListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditCompareListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditCompareListDlg)
	m_strDscription = _T("");
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_strExpression = _T("");
}


void CEditCompareListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditCompareListDlg)
	DDX_Control(pDX, IDC_COMBO_TESTTYPE, m_cobType);
	DDX_Control(pDX, IDC_EDIT_EXPRESSION, m_edtExpression);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_strDscription);
	DDV_MaxChars(pDX, m_strDscription, 120);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditCompareListDlg, CDialog)
	//{{AFX_MSG_MAP(CEditCompareListDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditCompareListDlg message handlers

void CEditCompareListDlg::OnOK() 
{
	m_nType = m_cobType.GetCurSel() + 50;

	m_edtExpression.GetWindowText( m_strExpression );

	CDialog::OnOK();
}

BOOL CEditCompareListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cobType.SetCurSel(m_nType-50); 
	m_edtExpression.SetWindowText(m_strExpression);

	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditCompareListDlg::OnButton1() 
{
	UpdateData(TRUE);
	if(NULL == m_pDoc ) return;
	CString str(_T(""));
	if( m_pDoc->GetVarFieldConstantsEtc( this, str))
	{
	
		m_edtExpression.SetSel(-1,0);
		m_edtExpression.ReplaceSel(str);

		UpdateData(FALSE);
	}

	return;
	
}
