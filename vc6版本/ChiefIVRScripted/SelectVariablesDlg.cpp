// SelectVariablesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "DrawDoc.h"
#include "SelectVariablesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectVariablesDlg dialog


CSelectVariablesDlg::CSelectVariablesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectVariablesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectVariablesDlg)
	m_nShowType = 0;
	//}}AFX_DATA_INIT
	m_pDoc = NULL;
	m_strVarName = _T("");
}


void CSelectVariablesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectVariablesDlg)
	DDX_Control(pDX, IDC_LIST_VAR, m_varList);
	DDX_Radio(pDX, IDC_RADIO_SHOW1, m_nShowType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectVariablesDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectVariablesDlg)
	ON_BN_CLICKED(IDC_RADIO_SHOW1, OnRadioShow1)
	ON_BN_CLICKED(IDC_RADIO_SHOW2, OnRadioShow2)
	ON_BN_CLICKED(IDC_RADIO_SHOW3, OnRadioShow3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectVariablesDlg message handlers

BOOL CSelectVariablesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_varList.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_varList.InsertColumn(0,"Key",LVCFMT_LEFT,80,100);
	m_varList.InsertColumn(1,"Name",LVCFMT_LEFT,120,100);
	m_varList.InsertColumn(2,"Type",LVCFMT_LEFT,120,100);
	m_varList.InsertColumn(3,"Values",LVCFMT_LEFT,150,100);
	
	
	InitNormalVarialeList();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectVariablesDlg::InitSystemVarialeList()
{
	if( NULL == m_pDoc ) return;
	m_varList.DeleteAllItems();
	//系统变量Key范围: key >=51 , key <= 99
	int i = 0;
	CString str("");
	//Database system variable
	for( i = 51; i < 59; i++ )
	{
		str.Format("%d",i );
		m_varList.InsertItem( i-51,	str );
		str.Format("SysDatabase%d",i-50 );
		m_varList.SetItemText(i-51, 1, str);
		m_varList.SetItemText( i-51, 2, "Database");
		m_varList.SetItemText( i-51, 3, "" );
		
	}

	//DLLHandle system variable
	for( i = 59; i < 67; i++ )
	{
		str.Format("%d",i );
		m_varList.InsertItem( i-51,	str );
		str.Format("SysDLLHandle%d",i-58 );
		m_varList.SetItemText(i-51, 1, str);
		m_varList.SetItemText( i-51, 2, "DLLHandle");
		m_varList.SetItemText( i-51, 3, "" );
	}

	//Time system variable
	for( i = 67; i < 75; i++ )
	{
		str.Format("%d",i );
		m_varList.InsertItem( i-51,	str );
		str.Format("SysString%d",i-66 );
		m_varList.SetItemText(i-51, 1, str);
		m_varList.SetItemText( i-51, 2, "String");
		m_varList.SetItemText( i-51, 3, "" );
	}
	
	//Double system variable
	for( i = 75; i < 79; i++ )
	{
		str.Format("%d",i );
		m_varList.InsertItem( i-51,	str );
		str.Format("SysDouble%d",i-74 );
		m_varList.SetItemText(i-51, 1, str);
		m_varList.SetItemText( i-51, 2, "Double");
		m_varList.SetItemText( i-51, 3, "" );
	}


	//Integer system variable
	for( i = 79; i < 87; i++ )
	{
		str.Format("%d",i );
		m_varList.InsertItem( i-51,	str );
		str.Format("SysInteger%d",i-78 );
		m_varList.SetItemText(i-51, 1, str);
		m_varList.SetItemText( i-51, 2, "Integer");
		m_varList.SetItemText( i-51, 3, "" );
	}
	
	//TCPSocket system variable
	for( i = 87; i < 91; i++ )
	{
		str.Format("%d",i );
		m_varList.InsertItem( i-51,	str );
		str.Format("SysTCPSocket%d",i-86 );
		m_varList.SetItemText(i-51, 1, str);
		m_varList.SetItemText( i-51, 2, "TCPSocket");
		m_varList.SetItemText( i-51, 3, "" );
	}


	//UDPSocket system variable
	for( i = 91; i < 95; i++ )
	{
		str.Format("%d",i );
		m_varList.InsertItem( i-51,	str );
		str.Format("SysUDPSocket%d",i-90 );
		m_varList.SetItemText(i-51, 1, str);
		m_varList.SetItemText( i-51, 2, "UDPSocket");
		m_varList.SetItemText( i-51, 3, "" );
	}



	return;
}

void CSelectVariablesDlg::OnOK() 
{
	UpdateData(FALSE);
	int nIndex = m_varList.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return;

	if( 1 == m_nShowType )
	{
		m_strVarName.Format("@%s:%s",m_varList.GetItemText( nIndex,0),m_varList.GetItemText( nIndex,1) );
	}
	else
	{
		m_strVarName.Format("#%s:%s",m_varList.GetItemText( nIndex,0),m_varList.GetItemText( nIndex,1) );
	}

	CDialog::OnOK();
}

void CSelectVariablesDlg::InitPage( CDrawDoc *pDoc )
{
	if( NULL == pDoc ) return;
	m_pDoc = pDoc;

	return;
}

void CSelectVariablesDlg::InitNormalVarialeList()
{
	if( NULL == m_pDoc ) return;
	m_varList.DeleteAllItems();


	CString str(_T(""));
	POSITION pos = m_pDoc->m_NormalVarList.GetHeadPosition();
	int i = 0;
	while( pos )
	{
		CNormalVar * pObj = m_pDoc->m_NormalVarList.GetNext(pos);
		if( NULL == pObj )
			continue;
		str.Format("%d",pObj->m_nKey);
		m_varList.InsertItem(i,	str );
		m_varList.SetItemText(i,1,pObj->m_strName);
		m_varList.SetItemText(i,2,pObj->GetDataTypeName(pObj->m_nVarDataType));
		m_varList.SetItemText(i,3,pObj->m_strVariable );
		
		i++;
	}


	return;
}

void CSelectVariablesDlg::InitParameterList()
{
	
	if( NULL == m_pDoc ) return;
	m_varList.DeleteAllItems();

	if( 0 == m_pDoc->m_nDocType ) return;//0 ief文件 1 nmf文件
	CString str(_T(""));
	POSITION pos = m_pDoc->m_ParameterLst.GetHeadPosition();
	int i = 0;
	while( pos )
	{
		CParameter * pObj = m_pDoc->m_ParameterLst.GetNext(pos);
		if( NULL == pObj )
			continue;
		str.Format("%d",pObj->m_nKey);
		m_varList.InsertItem(i,	str );
		m_varList.SetItemText(i,1,pObj->m_strName);
		m_varList.SetItemText(i,2,"");
		m_varList.SetItemText(i,3,"" );
		
		i++;
	}


	return;
}



void CSelectVariablesDlg::OnRadioShow1() 
{
	m_nShowType = 0;
	InitNormalVarialeList();
}

void CSelectVariablesDlg::OnRadioShow2() 
{
	InitSystemVarialeList();
	m_nShowType = 1;
}

void CSelectVariablesDlg::OnRadioShow3() 
{
	InitParameterList();
	m_nShowType = 2;
}


