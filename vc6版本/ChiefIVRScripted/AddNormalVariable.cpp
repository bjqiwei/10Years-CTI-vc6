// AddNormalVariable.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "AddNormalVariable.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddNormalVariable dialog


CAddNormalVariable::CAddNormalVariable(CWnd* pParent /*=NULL*/)
	: CDialog(CAddNormalVariable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddNormalVariable)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAddNormalVariable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddNormalVariable)
	DDX_Control(pDX, IDC_LIST_VAR, m_varList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddNormalVariable, CDialog)
	//{{AFX_MSG_MAP(CAddNormalVariable)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, OnButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddNormalVariable message handlers

void CAddNormalVariable::OnButtonNew() 
{
	// TODO: Add your control notification handler code here
	
}

void CAddNormalVariable::OnButtonEdit() 
{
	// TODO: Add your control notification handler code here
	
}

void CAddNormalVariable::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	
}


