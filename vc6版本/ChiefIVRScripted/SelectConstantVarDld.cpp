// SelectConstantVarDld.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "SelectConstantVarDld.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectConstantVarDlg dialog


CSelectConstantVarDlg::CSelectConstantVarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectConstantVarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectConstantVarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strVarName = "";
}


void CSelectConstantVarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectConstantVarDlg)
	DDX_Control(pDX, IDC_LIST_VAR, m_varList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectConstantVarDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectConstantVarDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectConstantVarDlg message handlers

void CSelectConstantVarDlg::InitVarialeList()
{
	m_varList.DeleteAllItems();

	//³£Á¿Key·¶Î§: key >=1 , key <= 50
	m_varList.InsertItem(0,"$1:Time");
	m_varList.SetItemText(0,1,"Time(HH:mm:ss)");

	m_varList.InsertItem(1,"$2:Data");
	m_varList.SetItemText(1,1,"Data(YYYY-MM-DD)");

	m_varList.InsertItem(2,"$3:DataTime");
	m_varList.SetItemText(2,1,"DataTime(YYYY-MM-DD HH:mm:ss)");

	m_varList.InsertItem(3,"$4:ChIndex");
	m_varList.SetItemText(3,1,"Integer");

	m_varList.InsertItem(4,"$5:ChType");
	m_varList.SetItemText(4,1,"Integer");

	m_varList.InsertItem(5,"$6:ScriptName");
	m_varList.SetItemText(5,1,"String");

	m_varList.InsertItem(6,"$7:DTMF");
	m_varList.SetItemText(6,1,"String");

	m_varList.InsertItem(7,"$8:ConferIndex");
	m_varList.SetItemText(7,1,"Integer");

	m_varList.InsertItem(8,"$9:LinkChIndex");
	m_varList.SetItemText(8,1,"Integer");

	m_varList.InsertItem(9,"$10:LinkChType");
	m_varList.SetItemText(9,1,"Integer");

	m_varList.InsertItem(10,"$11:CallBegiTime");
	m_varList.SetItemText(10,1,"String");

	m_varList.InsertItem(11,"$12:CallEndTime");
	m_varList.SetItemText(11,1,"String");
	
	m_varList.InsertItem(12,"$13:CallerID");
	m_varList.SetItemText(12,1,"String");

	m_varList.InsertItem(13,"$14:CalledID");
	m_varList.SetItemText(13,1,"String");

	m_varList.InsertItem(14,"$15:Time");
	m_varList.SetItemText(14,1,"Time(HHmmss)");

	m_varList.InsertItem(15,"$16:Data");
	m_varList.SetItemText(15,1,"Data(YYYYMMDD)");

	m_varList.InsertItem(16,"$17:DataTime");
	m_varList.SetItemText(16,1,"DataTime(YYYYMMDDHHmmss)");

	m_varList.InsertItem(17,"$18:HoldChIndex");
	m_varList.SetItemText(17,1,"Integer");

	m_varList.InsertItem(18,"$19:DialType");
	m_varList.SetItemText(18,1,"Integer");

	m_varList.InsertItem(19,"$20:DialTel");
	m_varList.SetItemText(19,1,"String");

	m_varList.InsertItem(20,"$21:RecordFile");
	m_varList.SetItemText(20,1,"String");

	m_varList.InsertItem(21,"$22:ReciveFaxFile");
	m_varList.SetItemText(21,1,"String");

	m_varList.InsertItem(22,"$23:SendFaxFile");
	m_varList.SetItemText(22,1,"String");

	m_varList.InsertItem(23,"$24:DialChIndex");
	m_varList.SetItemText(23,1,"Integer");

	m_varList.InsertItem(24,"$25:LocalDN");
	m_varList.SetItemText(24,1,"String");

	m_varList.InsertItem(25,"$26:RingTime");
	m_varList.SetItemText(25,1,"String");

	m_varList.InsertItem(26,"$27:HangUpType");
	m_varList.SetItemText(26,1,"Integer");

	m_varList.InsertItem(27,"$28:PickUpType");
	m_varList.SetItemText(27,1,"Integer");

	m_varList.InsertItem(28,"$29:PickUpTel");
	m_varList.SetItemText(28,1,"String");

	m_varList.InsertItem(29,"$30:ActiveType");
	m_varList.SetItemText(29,1,"Integer");

	m_varList.InsertItem(30,"$31:ActiveTel");
	m_varList.SetItemText(30,1,"String");

	m_varList.InsertItem(31,"$32:InboundCallPK");
	m_varList.SetItemText(31,1,"String");

	m_varList.InsertItem(32,"$33:OutboundCallPK");
	m_varList.SetItemText(32,1,"String");

	m_varList.InsertItem(33,"$34:CallType");
	m_varList.SetItemText(33,1,"String");
	
	m_varList.InsertItem(34,"$35:AgentPwd");
	m_varList.SetItemText(34,1,"String");

	m_varList.InsertItem(35,"$36:AgentID");
	m_varList.SetItemText(35,1,"String");

	m_varList.InsertItem(36,"$37:Group");
	m_varList.SetItemText(36,1,"Integer");

	m_varList.InsertItem(37,"$38:LoginTime");
	m_varList.SetItemText(37,1,"String");

	m_varList.InsertItem(38,"$39:LogOutTime");
	m_varList.SetItemText(38,1,"String");

	m_varList.InsertItem(39,"$40:ReadyTime");
	m_varList.SetItemText(39,1,"String");

	m_varList.InsertItem(40,"$41:NotReadyTime");
	m_varList.SetItemText(40,1,"String");

	m_varList.InsertItem(41,"$42:AgentName");
	m_varList.SetItemText(41,1,"String");

	m_varList.InsertItem(42,"$43:GroupName");
	m_varList.SetItemText(42,1,"String");

	m_varList.InsertItem(43,"$44:SetBusyTime");
	m_varList.SetItemText(43,1,"String");

	m_varList.InsertItem(44,"$45:SetInServerTime");
	m_varList.SetItemText(44,1,"String");
	return;
}

BOOL CSelectConstantVarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_varList.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_varList.InsertColumn(1,"Name",LVCFMT_LEFT,120,120);
	m_varList.InsertColumn(2,"Type",LVCFMT_LEFT,300,300);

	
	
	InitVarialeList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectConstantVarDlg::OnOK() 
{
	int nIndex = m_varList.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
		return;

	
	m_strVarName = m_varList.GetItemText( nIndex,0);
	
	
	CDialog::OnOK();
}
