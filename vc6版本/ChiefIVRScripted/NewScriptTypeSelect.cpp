// NewScriptTypeSelect.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "NewScriptTypeSelect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewScriptTypeSelect dialog


CNewScriptTypeSelect::CNewScriptTypeSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CNewScriptTypeSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewScriptTypeSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nDocType = 0;
}


void CNewScriptTypeSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewScriptTypeSelect)
	DDX_Control(pDX, IDC_LIST_DOCTYPE, m_lstDocType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewScriptTypeSelect, CDialog)
	//{{AFX_MSG_MAP(CNewScriptTypeSelect)
	ON_NOTIFY(NM_CLICK, IDC_LIST_DOCTYPE, OnClickListDoctype)
	ON_NOTIFY(HDN_ITEMCLICK, IDC_LIST_DOCTYPE, OnItemclickListDoctype)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_DOCTYPE, OnDblclkListDoctype)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewScriptTypeSelect message handlers

BOOL CNewScriptTypeSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int i=0;
	HICON hIcon[3];
	
	m_ImageList.Create(32,32,TRUE,3,3);
	hIcon[0] = AfxGetApp()->LoadIcon(IDR_CHIEFITYPE);
	hIcon[1] = AfxGetApp()->LoadIcon(IDR_MODULE);
	for(i = 0 ; i < 3 ; i++)
	{  
		m_ImageList.Add(hIcon[i]);
	}
	m_lstDocType.SetImageList(&m_ImageList,LVSIL_NORMAL);

	m_lstDocType.InsertItem( 0, "Normal", 0 );
	m_lstDocType.InsertItem( 1, "Module", 1 );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewScriptTypeSelect::OnClickListDoctype(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	POSITION pos = m_lstDocType.GetFirstSelectedItemPosition();
	if( pos )
		m_nDocType = m_lstDocType.GetNextSelectedItem(pos);

	*pResult = 0;
}

void CNewScriptTypeSelect::OnItemclickListDoctype(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CNewScriptTypeSelect::OnDblclkListDoctype(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION pos = m_lstDocType.GetFirstSelectedItemPosition();
	if( pos )
		m_nDocType = m_lstDocType.GetNextSelectedItem(pos);
    OnOK();
	*pResult = 0;
}
