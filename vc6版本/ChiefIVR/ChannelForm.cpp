// ChannelForm.cpp : implementation file
//

#include "stdafx.h"
#include "ChiefIVR.h"
#include "ChannelForm.h"
#include "ProgressDlg.h"
#include "SelectChannelDlg.h"
#include "UpdateChannelDlg.h"
#include "CFCHH.h"
#include "CIF5188.h"
#include "..//include//Tc08a32.h"
#include "..//include//Conf95.h"
#include "..//include//NewSig.h"
#include "..//include//NewConf.h"

#include "..//include//tce1_32.h"
#include "..//include//No7Dll.h"
#include "..//include//djH323.h"

#include "..//include//structO.h"
#include "..//include//PCIcard.h"

#include "..\include\CtiDef.h"
#include "..\include\PhonicDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CChiefIVRApp theApp;
extern CD08A *g_pD08a;
CChannelForm *g_pChannelForm = NULL;

/////////////////////////////////////////////////////////////////////////////
// CChannelForm

IMPLEMENT_DYNCREATE(CChannelForm, CFormView)

CChannelForm::CChannelForm()
	: CFormView(CChannelForm::IDD)
{
	//{{AFX_DATA_INIT(CChannelForm)
	m_strTitle = _T("");
	//}}AFX_DATA_INIT
	g_pChannelForm = this;
	m_InitialUpdate = 0;
	m_cy = 0;
	m_cx = 0;
	m_nCardType		= g_pD08a->GetCardType();//0:东进语音卡 1:Dialogic语音卡 2:长丰建业或TMS语音卡
	m_nAnalogTrunk	= g_pD08a->Get_AnalogTrunk();//是否有模拟中继(0:无模拟中继/1:有模拟中继)
	m_nDigitalTrunk = g_pD08a->Get_DigitalTrunk();//是否有数字中继以及类型(0=无数字中继;1=SS1;2=SS7 TUP;3=ISDN PRI(30B+D);4=SS7 ISUP )
	m_nVIP			= 0;
	m_strSetFile    ="";
	m_nShowType     = 0;
	m_pwndProg = NULL;

}

CChannelForm::~CChannelForm()
{
}

void CChannelForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChannelForm)
	DDX_Control(pDX, IDC_LIST_NORMALSCRIPT, m_lstNormalScript);
	DDX_Control(pDX, IDC_LIST_CONFER, m_lstConfer);
	DDX_Control(pDX, IDC_LIST_FAX, m_lstFax);
	DDX_Control(pDX, IDC_LIST_TCPIP, m_listTCPIP);
	DDX_Control(pDX, IDC_LIST_COMMLIST, m_CommList);
	DDX_Control(pDX, IDC_LIST_SCRIPT, m_listScript);
	DDX_Control(pDX, IDC_LIST_CHANNEL, m_lstChannel);
	DDX_Text(pDX, IDC_STATIC_TITLE, m_strTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChannelForm, CFormView)
	//{{AFX_MSG_MAP(CChannelForm)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_NORMALSCRIPT, OnRclickListNormalscript)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SCRIPT, OnRclickListScript)
	ON_COMMAND(ID_SELECT_ALL_CHANNEL, OnSelectAllChannel)
	ON_COMMAND(ID_CANCEL_SELECT_CHANNEL, OnCancelSelectChannel)
	ON_COMMAND(ID_SELECT_CHANNEL, OnSelectChannel)
	ON_COMMAND(ID_REVERSE_SELECT_CHANNEL, OnReverseSelectChannel)
	ON_COMMAND(ID_UPDATE_SELECT_CHANNEL, OnUpdateSelectChannel)
	ON_COMMAND(ID_RELOAD_CHANNEL_SCRIPT, OnReloadChannelScript)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_CHANNEL, OnRclickListChannel)
	ON_COMMAND(ID_SAVE_CHANNEL_CONFIG, OnSaveChannelConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelForm diagnostics

#ifdef _DEBUG
void CChannelForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CChannelForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChannelForm message handlers

void CChannelForm::OnSize(UINT nType, int cx, int cy) 
{
	m_cx=cx;
	m_cy=cy;
	if(m_InitialUpdate==1)
	{
		m_lstChannel.MoveWindow(0,20,m_cx,m_cy-20);
		m_listTCPIP.MoveWindow(0,20,m_cx,m_cy-20);
		m_listScript.MoveWindow(0,20,m_cx,m_cy-20);
		m_CommList.MoveWindow(0,20,m_cx,m_cy-20);
		m_lstConfer.MoveWindow(0,20,m_cx,m_cy-20);
		m_lstFax.MoveWindow(0,20,m_cx,m_cy-20);
		m_lstNormalScript.MoveWindow(0,20,m_cx,m_cy-20);
	}
	
}

void CChannelForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	if( 1 != m_InitialUpdate )
	{
		m_InitialUpdate=1;

		m_lstChannel.MoveWindow(0,20,m_cx,m_cy-20);
		m_listTCPIP.MoveWindow(0,20,m_cx,m_cy-20);
		m_listScript.MoveWindow(0,20,m_cx,m_cy-20);
		m_CommList.MoveWindow(0,20,m_cx,m_cy-20);
		m_lstConfer.MoveWindow(0,20,m_cx,m_cy-20);
		m_lstFax.MoveWindow(0,20,m_cx,m_cy-20);	
		m_lstNormalScript.MoveWindow(0,20,m_cx,m_cy-20);

		m_strTitle = "Channel information";
		m_lstChannel.ShowWindow(SW_SHOW);
		m_listTCPIP.ShowWindow(SW_HIDE);
		m_listScript.ShowWindow(SW_HIDE);
		m_CommList.ShowWindow(SW_HIDE);
		m_lstConfer.ShowWindow(SW_HIDE);
		m_lstFax.ShowWindow(SW_HIDE);
		m_lstNormalScript.ShowWindow(SW_HIDE);



		if( !InitChannelList() ) return;
		InitScriptList();
		InitTCPList();
		InitFaxList( );
		InitConferenceList( );
		InitNormalScriptList();

		UpdateData(FALSE);
		

	}
	
	
}

void CChannelForm::ShowList(CString strItem )
{
	if( "Channel" == strItem )
	{
		m_strTitle = "Channel information";
		m_lstChannel.ShowWindow(SW_SHOW);
		m_listTCPIP.ShowWindow(SW_HIDE);
		m_listScript.ShowWindow(SW_HIDE);
		m_CommList.ShowWindow(SW_HIDE);
		m_lstConfer.ShowWindow(SW_HIDE);
		m_lstFax.ShowWindow(SW_HIDE);
		m_lstNormalScript.ShowWindow(SW_HIDE);
		m_nShowType = 0;
	}
	else if( "Communicate" == strItem )
	{
		m_strTitle = "Communicate Message";
		m_lstChannel.ShowWindow(SW_HIDE);
		m_listTCPIP.ShowWindow(SW_HIDE);
		m_listScript.ShowWindow(SW_HIDE);
		m_CommList.ShowWindow(SW_SHOW);
		m_lstConfer.ShowWindow(SW_HIDE);
		m_lstFax.ShowWindow(SW_HIDE);
		m_lstNormalScript.ShowWindow(SW_HIDE);
		m_nShowType = 1;
	}
	else if( "TCP/IP" == strItem )
	{
		m_strTitle = "TCP/IP information";
		m_lstChannel.ShowWindow(SW_HIDE);
		m_listTCPIP.ShowWindow(SW_SHOW);
		m_listScript.ShowWindow(SW_HIDE);
		m_CommList.ShowWindow(SW_HIDE);
		m_lstConfer.ShowWindow(SW_HIDE);
		m_lstFax.ShowWindow(SW_HIDE);
		m_lstNormalScript.ShowWindow(SW_HIDE);
		m_nShowType = 2;
	}
	else if( "SystemScript" == strItem )
	{
		m_strTitle = "SystemScript information";
		m_lstChannel.ShowWindow(SW_HIDE);
		m_listTCPIP.ShowWindow(SW_HIDE);
		m_listScript.ShowWindow(SW_SHOW);
		m_CommList.ShowWindow(SW_HIDE);
		m_lstConfer.ShowWindow(SW_HIDE);
		m_lstFax.ShowWindow(SW_HIDE);
		m_lstNormalScript.ShowWindow(SW_HIDE);
		m_nShowType = 3;
	}
	else if( "Fax" == strItem )
	{
		m_strTitle = "Fax information";
		m_lstChannel.ShowWindow(SW_HIDE);
		m_listTCPIP.ShowWindow(SW_HIDE);
		m_listScript.ShowWindow(SW_HIDE);
		m_CommList.ShowWindow(SW_HIDE);
		m_lstConfer.ShowWindow(SW_HIDE);
		m_lstFax.ShowWindow(SW_SHOW);
		m_lstNormalScript.ShowWindow(SW_HIDE);
		m_nShowType = 4;
	}
	else if( "Conference" == strItem )
	{
		m_strTitle = "Conference information";
		m_lstChannel.ShowWindow(SW_HIDE);
		m_listTCPIP.ShowWindow(SW_HIDE);
		m_listScript.ShowWindow(SW_HIDE);
		m_CommList.ShowWindow(SW_HIDE);
		m_lstConfer.ShowWindow(SW_SHOW);
		m_lstFax.ShowWindow(SW_HIDE);
		m_lstNormalScript.ShowWindow(SW_HIDE);
		m_nShowType = 5;
	}
	else if( "NormalScript" == strItem )
	{
		m_strTitle = "NormalScript information";
		m_lstChannel.ShowWindow(SW_HIDE);
		m_listTCPIP.ShowWindow(SW_HIDE);
		m_listScript.ShowWindow(SW_HIDE);
		m_CommList.ShowWindow(SW_HIDE);
		m_lstConfer.ShowWindow(SW_HIDE);
		m_lstFax.ShowWindow(SW_HIDE);
		m_lstNormalScript.ShowWindow(SW_SHOW);
		m_nShowType = 6;
	}
	UpdateData(FALSE);

	return;
}
void CChannelForm::OnUpdateSelectChannel() 
{
	CUpdateChannelDlg Dlg;
	if( Dlg.DoModal() == IDOK )
	{
		int nCount			= m_lstChannel.GetItemCount();
		for( int i = 0; i < nCount ;i++ )
		{
			if( m_lstChannel.GetCheck( i ) )
			{
				if( Dlg.m_bUpdateCalledID )
				{
					m_lstChannel.SetItemText( i, 9, Dlg.m_strCalledID );
				}
				if( Dlg.m_bUpdateScriptFile )
				{
					m_lstChannel.SetItemText( i, 11, Dlg.m_strScriptFile );
				}
				if( Dlg.m_bUpdateType )
				{
					m_lstChannel.SetItemText( i, 2, GetChannelTypeNew( Dlg.m_nChnlType ) );
					
				}
			}
		}	
	}
	
}
BOOL CChannelForm::InitChannelList()
{
	m_lstChannel.SetExtendedStyle(  LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT );
	m_lstChannel.InsertColumn(0,"NO",LVCFMT_LEFT,40,40);
	m_lstChannel.InsertColumn(1,"ChnlIndex",LVCFMT_LEFT,80,40);
	m_lstChannel.InsertColumn(2,"Type",LVCFMT_LEFT,80,80);
	m_lstChannel.InsertColumn(3,"BlockCaption",LVCFMT_LEFT,100,100);
	m_lstChannel.InsertColumn(4,"State",LVCFMT_LEFT,100,100);
	m_lstChannel.InsertColumn(5,"ConnetTo",LVCFMT_LEFT,80,80);
	m_lstChannel.InsertColumn(6,"Conference",LVCFMT_LEFT,80,80);
	m_lstChannel.InsertColumn(7,"CallerID",LVCFMT_LEFT,100,100);
	m_lstChannel.InsertColumn(8,"CalledID",LVCFMT_LEFT,100,100);
	m_lstChannel.InsertColumn(9,"LocalDN",LVCFMT_LEFT,100,100);//本地号码
	m_lstChannel.InsertColumn(10,"DTMF",LVCFMT_LEFT,100,100);
	m_lstChannel.InsertColumn(11,"Scriptfile",LVCFMT_LEFT,400,100);

	return InitChannelListItem();
}

void CChannelForm::InitTCPList()
{
	m_listTCPIP.InsertColumn(0,"NO",LVCFMT_CENTER,40,40);
	m_listTCPIP.InsertColumn(1,"Port",LVCFMT_CENTER,80,80);
	m_listTCPIP.InsertColumn(2,"IP Address",LVCFMT_CENTER,280,280);
	


	return;
}

void CChannelForm::InitScriptList()
{
	m_listScript.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_listScript.InsertColumn(0,"Script File",LVCFMT_LEFT,360,360);
	m_listScript.InsertColumn(1,"Remark",LVCFMT_LEFT,360,360);
	InitSysScriptFile();
	return;
}



void CChannelForm::OnClose() 
{
	

	
	CFormView::OnClose();
}

void CChannelForm::ShowChannelState(int nChnl, CString strState)
{
	m_lstChannel.SetItemText( nChnl,4, strState );
	return;
}

void CChannelForm::ShowChannelBlock(int nChnl, CString strBlock)
{
	m_lstChannel.SetItemText( nChnl,3, strBlock );
	return;
}

void CChannelForm::ShowChannelDTMF(int nChnl, CString strDTMF)
{
	m_lstChannel.SetItemText( nChnl,10, strDTMF );
	return;
}

void CChannelForm::DelSysScriptFile()
{
	int nIndex = m_listScript.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
	{
		AfxMessageBox("Please select the script file item then delete it ");
		return ;
	}

	m_listScript.DeleteItem(nIndex);

	return ;
}

void CChannelForm::InitSysScriptFile()
{
	CCIFSysScriptDoc doc;
	CString strfile = "";
	strfile = theApp.m_strPath + "\\CFTMS.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		CreatSysConfgFile( strfile );
		   return ;
	}



	CString str = "";
	POSITION pos = doc.m_SysScriptListObj.GetHeadPosition();
	
	while( pos )
	{
		CCFSysScriptSetObj *pObj = doc.m_SysScriptListObj.GetNext(pos);
		if( NULL != pObj )
		{
			int nCount = m_listScript.GetItemCount();
			m_listScript.InsertItem(nCount,pObj->m_strScriptFile);
			m_listScript.SetItemText(nCount,1,pObj->m_strRemark);
		}
	}
	

}

BOOL CChannelForm::SaveSysScriptFile()
{

	CCIFSysScriptDoc doc;
	CString strfile = "";
	strfile = theApp.m_strPath + "\\CFTMS.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		return FALSE;
	}
	
	while ( doc.m_SysScriptListObj.GetHeadPosition() )
	{
		delete doc.m_SysScriptListObj.RemoveHead();
	}
	int nCount = m_listScript.GetItemCount();
	for( int i = 0; i < nCount; i++ )
	{
		CCFSysScriptSetObj *pObj = new CCFSysScriptSetObj;
		pObj->m_strScriptFile = m_listScript.GetItemText(i,0);
		pObj->m_strRemark = m_listScript.GetItemText(i,1);
		doc.m_SysScriptListObj.AddTail( pObj );
	}

	doc.OnSaveDocument(strfile);
	return TRUE;
}

void CChannelForm::ShowConnectToChnl(int nChnl, int nConnectToChnl )
{
	if( nChnl < 0 ) return;
	CString str("");
	str.Format("%d", nConnectToChnl );
	m_lstChannel.SetItemText( nChnl,4, str );
	
	return;
}



void CChannelForm::InitConferenceList()
{
	m_lstConfer.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_lstConfer.InsertColumn(0,"Group Index",LVCFMT_LEFT,100,100);
	m_lstConfer.InsertColumn(1,"Conference resource",LVCFMT_LEFT,150,150);
	m_lstConfer.InsertColumn(2,"MemberNumber",LVCFMT_LEFT,150,150);
	m_lstConfer.InsertColumn(3,"Listen MemberNumber",LVCFMT_LEFT,150,150);

	return;
}

void CChannelForm::InitFaxList()
{
	m_lstFax.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_lstFax.InsertColumn(0,"Fax Index",LVCFMT_LEFT,100,100);
	m_lstFax.InsertColumn(1,"State",LVCFMT_LEFT,160,160);
	m_lstFax.InsertColumn(2,"page",LVCFMT_LEFT,60,60);
	m_lstFax.InsertColumn(3,"FileName",LVCFMT_LEFT,360,360);

	return;
}

void CChannelForm::ReadSystemSetFile()
{
	
	//m_nCardType		= GetPrivateProfileInt("CardOption","CardType",0,"IvrSystem\\ChiefIVRSet.ini");
	//m_nTrunkType	= GetPrivateProfileInt("CardOption","TrunkType",0,"IvrSystem\\ChiefIVRSet.ini");
	//m_nFax			= GetPrivateProfileInt("CardOption","Fax",0,"IvrSystem\\ChiefIVRSet.ini");

	return;
}

void CChannelForm::InitNormalScriptFile()
{
	CCIFSysScriptDoc doc;
	CString strfile = "";
	strfile = theApp.m_strPath + "\\CFSynWay.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		CreatSysConfgFile(strfile);
		return ;
	}
	
	
	CString str = "";
	POSITION pos = doc.m_SysScriptListObj.GetHeadPosition();
	
	while( pos )
	{
		CCFSysScriptSetObj *pObj = doc.m_SysScriptListObj.GetNext(pos);
		if( NULL != pObj )
		{
			int nCount = m_lstNormalScript.GetItemCount();
			m_lstNormalScript.InsertItem(nCount,pObj->m_strScriptFile);
			m_lstNormalScript.SetItemText(nCount,1,pObj->m_strRemark);
		}
	}
	
}

void CChannelForm::InitNormalScriptList()
{
	m_lstNormalScript.SetExtendedStyle(  LVS_EX_FULLROWSELECT );
	m_lstNormalScript.InsertColumn(0,"Script File",LVCFMT_LEFT,360,360);
	m_lstNormalScript.InsertColumn(1,"Remark",LVCFMT_LEFT,360,360);
	InitNormalScriptFile();
	return;
}

BOOL CChannelForm::SaveNormalScriptFile()
{

	CCIFSysScriptDoc doc;
	CString strfile = "";
	strfile = theApp.m_strPath + "\\CFSynWay.DLL";
	if( ! doc.OnOpenDocument(strfile) )
	{
		return FALSE;
	}
	
	while ( doc.m_SysScriptListObj.GetHeadPosition() )
	{
		delete doc.m_SysScriptListObj.RemoveHead();
	}
	int nCount = m_lstNormalScript.GetItemCount();
	for( int i = 0; i < nCount; i++ )
	{
		CCFSysScriptSetObj *pObj = new CCFSysScriptSetObj;
		pObj->m_strScriptFile = m_lstNormalScript.GetItemText(i,0);
		pObj->m_strRemark = m_lstNormalScript.GetItemText(i,1);
		doc.m_SysScriptListObj.AddTail( pObj );
	}

	doc.OnSaveDocument(strfile);
	return TRUE;

}

void CChannelForm::DelNormalScriptFile()
{
	int nIndex = m_lstNormalScript.GetNextItem( -1, LVNI_SELECTED ) ;
	if( -1 == nIndex )
	{
		AfxMessageBox("Please select the script file item then delete it ");
		return ;
	}

	m_lstNormalScript.DeleteItem(nIndex);

	return ;
}

void CChannelForm::OnRclickListNormalscript(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu menu;
	CPoint popPoint;	
	menu.LoadMenu( IDR_POPTOOL );
	GetCursorPos(&popPoint);//得到鼠标位置
	

	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN ,popPoint.x,popPoint.y ,AfxGetMainWnd());
	menu.DestroyMenu();

	*pResult = 0;
}

void CChannelForm::OnRclickListScript(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu menu;
	CPoint popPoint;	
	menu.LoadMenu( IDR_POPTOOL );
	GetCursorPos(&popPoint);//得到鼠标位置
	

	menu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN ,popPoint.x,popPoint.y ,AfxGetMainWnd());
	menu.DestroyMenu();	

	*pResult = 0;
}



void CChannelForm::OnSelectAllChannel() 
{
	this->UpdateData(TRUE);
	int nCount = m_lstChannel.GetItemCount();
	for( int i = 0; i < nCount ;i++ )
	{
		m_lstChannel.SetCheck( i, TRUE );
	}
	
}

void CChannelForm::OnCancelSelectChannel() 
{
	this->UpdateData(TRUE);
	int nCount = m_lstChannel.GetItemCount();
	for( int i = 0; i < nCount ;i++ )
	{
		m_lstChannel.SetCheck( i, FALSE );
	}
	
	
}

void CChannelForm::OnSelectChannel() 
{
	CSelectChannelDlg Dlg;
	if( Dlg.DoModal() == IDOK )
	{
		int nCount			= m_lstChannel.GetItemCount();
		int nBeginNo		= Dlg.m_nBeginNo;
		int nEndNo			= Dlg.m_nEndNo;
		int nChannelType	= Dlg.m_nChannelType;

		for( int i = 0; i < nCount ;i++ )
		{
			if( i >= nBeginNo &&  i <= nEndNo)
			{
				if( 10 != nChannelType )
				{
					if( nChannelType == GetChannelTypeNew( m_lstChannel.GetItemText( i, 2 ) ) )
						m_lstChannel.SetCheck( i, TRUE );
					else
						m_lstChannel.SetCheck( i, FALSE );
				}
				else
					m_lstChannel.SetCheck( i, TRUE );
			}
			else
			{
					m_lstChannel.SetCheck( i, FALSE );
			}
		}	
	}
	
}

void CChannelForm::OnReverseSelectChannel() 
{
	this->UpdateData(TRUE);
	int nCount = m_lstChannel.GetItemCount();
	for( int i = 0; i < nCount ;i++ )
	{
		
		m_lstChannel.SetCheck( i, !m_lstChannel.GetCheck(i) );
	}
	
	
}



void CChannelForm::OnReloadChannelScript() 
{
	// TODO: Add your command handler code here
	
}

void CChannelForm::OnRclickListChannel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CMenu menu;
	CPoint popPoint;	
	menu.LoadMenu( IDR_POPTOOL );
	GetCursorPos(&popPoint);//得到鼠标位置
	

	menu.GetSubMenu(2)->TrackPopupMenu(TPM_LEFTALIGN ,popPoint.x,popPoint.y ,AfxGetMainWnd());
	menu.DestroyMenu();	

	*pResult = 0;
}



int CChannelForm::GetChannelTypeNew( CString strType )
{
	if( strType == "NoKnow" )
		return -1;
	else if( strType == "Empty")
		return 0;
	else if( strType == "User")
		return 7;
	else if( strType == "Trunk")
		return 6;
	else if( strType == "Record")
		return 8;
	else if( strType == "SS1")
		return 1;
	else if( strType == "SS7")
		return 2;
	else if( strType == "ISDN PRI")
		return 3;
	else if( strType == "SS7_ISUP")
		return 4;
	else if( strType == "SS7_MTP")
		return 5;
	else if( strType == "VIP")
		return 9;
	else
		return -3;

}

CString CChannelForm::GetChannelTypeNew( int nType )
{
	CString str = "";
	switch( nType )
	{
	case -1:
		str = "NoKnow";
		break;
	case 0:
		str = "Empty";
		break;
	case 7:
		str = "User";
		break;
	case 6:
		str = "Trunk";
		break;
	case 8:
		str = "Record";
		break;
	case 1:
		str = "SS1";
		break;
	case 2:
		str = "SS7";
		break;
	case 3:
		str = "ISDN PRI";
		break;
	case 4:
		str = "SS7_ISUP";
		break;
	case 5:
		str = "SS7_MTP";
		break;
	case 9:
		str = "VIP";
		break;
		
	default:
		break;

	}

	return str;

}

void CChannelForm::AutoCheck() 
{
	CProgressDlg Dlg;
	Dlg.Create(IDD_DIALOG_PROGRESS,this);
	Dlg.ShowWindow(SW_SHOW);

	m_pwndProg = &(Dlg.m_wndProg);
	if( 0 == m_nCardType )
	{
		//东进语音卡
		DJ_InitChannelSet();
	}
	else if( 1 == m_nCardType )
	{
		//Dialogic语音卡

	}
	else if( 2 == m_nCardType )
	{
		//2:长丰建业语音卡
		TMS_InitChannelSet();
	}
	else if( 3 == m_nCardType )
	{
		//3:飞环语音卡
		Nor32_InitChannelSet();
	}

	InitChannelListItem();
	m_pwndProg = NULL;
	Dlg.DestroyWindow();
	return;
	
}

int CChannelForm::DJDevChTypeToCifChType(int nDJChType)
{
	switch(nDJChType)
	{
	case 0:
		return CF_CHTYPE_USER;
	case 1:
		return CF_CHTYPE_TRUNK;
	case 2:
		return CF_CHTYPE_EMPTY;
	case 3:
		return CF_CHTYPE_RECORD;
	//case CHTYPE_CS:
	//case CHTYPE_EM_CTRL:
	//case CHTYPE_EM_4VOC:
	//case CHTYPE_EM_2VOC:
	default:
		return CF_CHTYPE_NOKNOW;
		
	}
}

void CChannelForm::DJ_InitChannelSet()
{
	if( NULL !=m_pwndProg ) m_pwndProg->StepIt();//进度条显示
	CFChnlSetObjList  csoList;
	int i = 0;
	if( 1 == m_nAnalogTrunk && 0 == m_nDigitalTrunk )//有模拟卡但没有数字卡
	{

		if( LoadDRV ( ) ) return;
		int nTotalChannel = CheckValidCh();
		if ( EnableCard( nTotalChannel, 1024*32 ) != (long)0)
		{
			FreeDRV();
			return ;
		}
		

		for( i = 0; i < nTotalChannel; i++ )
		{
			//if( CHTYPE_EMPTY != CheckChTypeNew(i) )
			//{
				CCFChnlSetObj *pNewObj = new CCFChnlSetObj;
				pNewObj->m_nIndex = i;
				pNewObj->m_nType = DJDevChTypeToCifChType( CheckChTypeNew(i) );
				csoList.AddTail( pNewObj );
			//}
			if( NULL !=m_pwndProg ) m_pwndProg->StepIt();//进度条显示
		}

		
		DisableCard();
		FreeDRV();

	}
	else if( 1 == m_nAnalogTrunk && 0 != m_nDigitalTrunk )//有模拟卡也有数字卡
	{
		//数字模拟混合
		DJSys_UserCall_TC08A32( true );
		int ret = DJSys_EnableCard( "", "PROMPT.INI" ) ;
		if ( ret != 0 ) {
			return ;
		}


		int nTotalTrunk = DJTrk_GetTotalTrunkNum() ;
		int nTotalUser = CheckValidCh();

		for( i = 0; i < nTotalUser; i++ )
		{
			if( CHTYPE_EMPTY != CheckChTypeNew(i) )
			{
				CCFChnlSetObj *pNewObj = new CCFChnlSetObj;
				pNewObj->m_nIndex = i;
				pNewObj->m_nType = DJDevChTypeToCifChType( CheckChTypeNew(i) );
				csoList.AddTail( pNewObj );
			}
		}
		for( i = 0; i < nTotalTrunk; i++ )
		{
			CCFChnlSetObj *pNewObj = new CCFChnlSetObj;
			pNewObj->m_nIndex = i;
			BYTE cb = DJSys_GetPcmMode ( nTotalTrunk / 30 );
			switch ( cb ) {
			case PCM_WORK_MODE_NONE:		// "7号(无信令)"
			case PCM_WORK_MODE_SS7:			// "7号"
				pNewObj->m_nType = CF_CHTYPE_DTNO7;
				break;
			case PCM_WORK_MODE_SS1:			// "1号"
				pNewObj->m_nType = CF_CHTYPE_DTNO1;
				break;
			case PCM_WORK_MODE_DSS1:		// "数字1号(ISDN PRI)"
				pNewObj->m_nType = CF_CHTYPE_DTDSS1;
				break;
			}

			csoList.AddTail( pNewObj );
			if( NULL !=m_pwndProg ) m_pwndProg->StepIt();//进度条显示
		}


		DJSys_DisableCard() ;

	}
	else if( 0 == m_nAnalogTrunk && 0 != m_nDigitalTrunk )//只有数字卡
	{
		//数字模拟混合
		int ret = DJSys_EnableCard( "", "PROMPT.INI" ) ;
		if ( ret != 0 ) {
			return ;
		}


		int nTotalTrunk = DJTrk_GetTotalTrunkNum() ;
		for( i = 0; i < nTotalTrunk; i++ )
		{
			CCFChnlSetObj *pNewObj = new CCFChnlSetObj;
			pNewObj->m_nIndex = i;
			BYTE cb = DJSys_GetPcmMode ( nTotalTrunk / 30 );
			switch ( cb ) {
			case PCM_WORK_MODE_NONE:		// "7号(无信令)"
			case PCM_WORK_MODE_SS7:			// "7号"
				pNewObj->m_nType = CF_CHTYPE_DTNO7;
				break;
			case PCM_WORK_MODE_SS1:			// "1号"
				pNewObj->m_nType = CF_CHTYPE_DTNO1;
				break;
			case PCM_WORK_MODE_DSS1:		// "数字1号(ISDN PRI)"
				pNewObj->m_nType = CF_CHTYPE_DTDSS1;
				break;
			}
			csoList.AddTail( pNewObj );
			if( NULL !=m_pwndProg ) m_pwndProg->StepIt();//进度条显示
		}

		DJSys_DisableCard() ;

	}
	m_strSetFile = theApp.m_strPath + "\\CFCHDJ.DLL";
	CChiefIVRDoc doc;
	if( ! doc.OnOpenDocument(m_strSetFile) )
		return;
	int nTotalChannel = doc.m_nTotalChannel;
	POSITION pos1 = doc.m_ChSetListObj.GetHeadPosition();
	POSITION pos2 = csoList.GetHeadPosition();
	int m = 0;
	for( i = 0 ; i < nTotalChannel; i++ )
	{
		if( NULL != pos1 ){
			CCFChnlSetObj *pObj1 = doc.m_ChSetListObj.GetNext(pos1);
			if( NULL != pObj1 )
			{
				if( NULL != pos2 )
				{
					CCFChnlSetObj *pObj2 = csoList.GetNext(pos2);
					if( NULL != pObj2)
					{
						//if( CF_CHTYPE_EMPTY != pObj2->m_nType ){
						pObj1->m_nIndex = pObj2->m_nIndex;
						pObj1->m_nType = pObj2->m_nType;
						//}
					}
				}
				else
				{
					pObj1->m_nIndex = m;
					pObj1->m_nType = CF_CHTYPE_EMPTY;
					m++;
				}
			}
		}
		if( NULL !=m_pwndProg ) m_pwndProg->StepIt();//进度条显示
	}

	doc.OnSaveDocument(m_strSetFile);

	while( csoList.GetHeadPosition() )
	{
		delete csoList.RemoveHead();
	}

	return;
}


void CChannelForm::TMS_InitChannelSet(void)
{
	if( NULL !=m_pwndProg ) m_pwndProg->StepIt();//进度条显示
	CFChnlSetObjList  csoList;
	int i = 0;
	int nTB			= PCIGETLOGICCHNUM(CHTYPE_TB);//TB模拟外线的逻辑通道数目
	for( i = 0; i < nTB; i++ )
	{
		CCFChnlSetObj *pNewObj = new CCFChnlSetObj;
		pNewObj->m_nIndex = i;
		pNewObj->m_nType = CF_CHTYPE_TRUNK;
		csoList.AddTail( pNewObj );
	}
	int nAgent		= PCIGETLOGICCHNUM(CHTYPE_AGENT);//AGENT模拟内线的逻辑通道数目
	for( i = 0; i < nAgent; i++ )
	{
		CCFChnlSetObj *pNewObj = new CCFChnlSetObj;
		pNewObj->m_nIndex = i;
		pNewObj->m_nType = CF_CHTYPE_USER;
		csoList.AddTail( pNewObj );
	}
	int nHB			= PCIGETLOGICCHNUM(CHTYPE_HB);//HB高阻模块的逻辑通道数目
	for( i = 0; i < nHB; i++ )
	{
		CCFChnlSetObj *pNewObj = new CCFChnlSetObj;
		pNewObj->m_nIndex = i;
		pNewObj->m_nType = CF_CHTYPE_RECORD;
		csoList.AddTail( pNewObj );
	}
	int nDTNO1		= PCIGETLOGICCHNUM(CHTYPE_DTNO1);//中国一号信令数字线的逻辑通道数
	for( i = 0; i < nDTNO1; i++ )
	{
		CCFChnlSetObj *pNewObj = new CCFChnlSetObj;
		pNewObj->m_nIndex = i;
		pNewObj->m_nType = CF_CHTYPE_DTNO1;
		csoList.AddTail( pNewObj );
	}
	int nDTNO7		= PCIGETLOGICCHNUM(CHTYPE_DTNO7);//中国七号信令数字线的逻辑通道数目
	for( i = 0; i < nDTNO7; i++ )
	{
		CCFChnlSetObj *pNewObj = new CCFChnlSetObj;
		pNewObj->m_nIndex = i;
		pNewObj->m_nType = CF_CHTYPE_DTNO7;
		csoList.AddTail( pNewObj );
	}
	int nDTDSS1		= PCIGETLOGICCHNUM(CHTYPE_DTDSS1);//DSS1 ISDN  信令数字线的逻辑通道数目
	for( i = 0; i < nDTDSS1; i++ )
	{
		CCFChnlSetObj *pNewObj = new CCFChnlSetObj;
		pNewObj->m_nIndex = i;
		pNewObj->m_nType = CF_CHTYPE_DTDSS1;
		csoList.AddTail( pNewObj );
	}
	int nDTNO7ISUP	= PCIGETLOGICCHNUM(CHTYPE_DTNO7ISUP);//数字中继中国七号信令(ISUP)
	for( i = 0; i < nDTNO7ISUP; i++ )
	{
		CCFChnlSetObj *pNewObj = new CCFChnlSetObj;
		pNewObj->m_nIndex = i;
		pNewObj->m_nType = CF_CHTYPE_DTNO7ISUP;
		csoList.AddTail( pNewObj );
	}

	int nVIP	= PCIGETLOGICCHNUM(CHTYPE_DTNO7ISUP);//数字中继中国七号信令(ISUP)
	for( i = 0; i < nVIP; i++ )
	{
		CCFChnlSetObj *pNewObj = new CCFChnlSetObj;
		pNewObj->m_nIndex = i;
		pNewObj->m_nType = CF_CHTYPE_VIP;
		csoList.AddTail( pNewObj );
	}


	m_strSetFile = theApp.m_strPath + "\\CFCHDJ.DLL";
	CChiefIVRDoc doc;
	if( ! doc.OnOpenDocument(m_strSetFile) )
		return;
	int nTotalChannel = doc.m_nTotalChannel;
	POSITION pos1 = doc.m_ChSetListObj.GetHeadPosition();
	POSITION pos2 = csoList.GetHeadPosition();
	int m = 0;
	for( i = 0 ; i < nTotalChannel; i++ )
	{
		if( NULL != pos1 )
		{
			CCFChnlSetObj *pObj1 = doc.m_ChSetListObj.GetNext(pos1);
			if( NULL != pObj1 )
			{
				if( NULL != pos2 )
				{
					CCFChnlSetObj *pObj2 = csoList.GetNext(pos2);
					if( NULL != pObj2)
					{
						pObj1->m_nIndex = pObj2->m_nIndex;
						pObj1->m_nType = pObj2->m_nType;
					}
				}
				else
				{
					pObj1->m_nIndex = m;
					pObj1->m_nType = CF_CHTYPE_EMPTY;
					m++;
				}

			}
		}
		if( NULL !=m_pwndProg ) m_pwndProg->StepIt();//进度条显示
	}

	doc.OnSaveDocument(m_strSetFile);

	while( csoList.GetHeadPosition() )
	{
		delete csoList.RemoveHead();
	}

	return;
}

BOOL CChannelForm::InitChannelListItem()
{
	CString strfile = "";
	strfile = theApp.m_strPath + "\\CFCHDJ.DLL";
	CChiefIVRDoc doc;
	

	if( ! doc.OnOpenDocument(strfile) )
	{
		CreatChnlConfgFile( strfile );
		   return FALSE;
	}

	
	m_lstChannel.DeleteAllItems();
	CString str = "";
	POSITION pos = doc.m_ChSetListObj.GetHeadPosition();
	int i = 0;
	while( pos )
	{
		CCFChnlSetObj *pObj = doc.m_ChSetListObj.GetNext(pos);
		if( NULL != pObj )
		{
			str.Format("%d",i);
			m_lstChannel.InsertItem(i,str);
			str.Format("%d",pObj->m_nIndex);
			m_lstChannel.SetItemText(i,1, str );
			m_lstChannel.SetItemText(i,2, pObj->GetChannelTypeNew(pObj->m_nType) );

			m_lstChannel.SetItemText(i,3, "Start" );
			m_lstChannel.SetItemText(i,4, "Close" );
			m_lstChannel.SetItemText(i,5, "-1" );
			m_lstChannel.SetItemText(i,6, "-1" );
			m_lstChannel.SetItemText(i,7, "" );
			m_lstChannel.SetItemText(i,8, "" );
			m_lstChannel.SetItemText(i,9, pObj->m_strLocalDN  );
			m_lstChannel.SetItemText(i,10, "" );
			m_lstChannel.SetItemText(i,11, pObj->m_strScriptFile );
			i++;
		}
		if( NULL !=m_pwndProg ) m_pwndProg->StepIt();//进度条显示
	}





	return TRUE;
}

void CChannelForm::OnSaveChannelConfig() 
{
	m_strSetFile = theApp.m_strPath + "\\CFCHDJ.DLL";
	CChiefIVRDoc doc;
	if( ! doc.OnOpenDocument(m_strSetFile) )
		return;
    
	POSITION pos = doc.m_ChSetListObj.GetHeadPosition();
	int i = 0;
	while( pos )
	{
		CCFChnlSetObj *pObj = doc.m_ChSetListObj.GetNext(pos);
		if( NULL != pObj )
		{
			pObj->m_nType  = GetChannelTypeNew( m_lstChannel.GetItemText( i,2) );
			pObj->m_strLocalDN  = m_lstChannel.GetItemText( i,9);
			pObj->m_strScriptFile = m_lstChannel.GetItemText(i,11);
			i++;
		}
	}

	if( doc.OnSaveDocument(m_strSetFile) )
	{
		AfxMessageBox("it is successful to config channel!");
	}
	else
	{
		AfxMessageBox("it is failful to config channel!");
	}
	return;
}



void CChannelForm::Nor32_InitChannelSet()
{
	if( NULL !=m_pwndProg ) m_pwndProg->StepIt();//进度条显示
	CFChnlSetObjList  csoList;
	int i = 0;

	if( tpi_OpenDevice() < 0 )
	{
		return ;
	}

	int nTotalChannel = tpi_GetLineCount();
	for( i = 0; i < nTotalChannel; i++ )
	{

		CCFChnlSetObj *pNewObj = new CCFChnlSetObj;
		pNewObj->m_nIndex = tpi_GetChannelID(i);
		pNewObj->m_nType = Nor32DevChTypeToCifChType( tpi_GetChannelType(i) );
		csoList.AddTail( pNewObj );

		if( NULL !=m_pwndProg ) m_pwndProg->StepIt();//进度条显示
	}

	
	tpi_CloseDevice();


	m_strSetFile = theApp.m_strPath + "\\CFCHDJ.DLL";
	CChiefIVRDoc doc;
	if( ! doc.OnOpenDocument(m_strSetFile) )
		return;
	nTotalChannel = doc.m_nTotalChannel;
	POSITION pos1 = doc.m_ChSetListObj.GetHeadPosition();
	POSITION pos2 = csoList.GetHeadPosition();
	int m = 0;
	for( i = 0 ; i < nTotalChannel; i++ )
	{
		if( NULL != pos1 )
		{
			CCFChnlSetObj *pObj1 = doc.m_ChSetListObj.GetNext(pos1);
			if( NULL != pObj1 )
			{
				if( NULL != pos2 )
				{
					CCFChnlSetObj *pObj2 = csoList.GetNext(pos2);
					if( NULL != pObj2)
					{
						pObj1->m_nIndex = pObj2->m_nIndex;
						pObj1->m_nType = pObj2->m_nType;
					}
				}
				else
				{
					pObj1->m_nIndex = m;
					pObj1->m_nType = CF_CHTYPE_EMPTY;
					m++;
				}

			}
		}
		if( NULL !=m_pwndProg ) m_pwndProg->StepIt();//进度条显示
	}

	doc.OnSaveDocument(m_strSetFile);

	while( csoList.GetHeadPosition() )
	{
		delete csoList.RemoveHead();
	}

	return;
}

int CChannelForm::Nor32DevChTypeToCifChType(int nChnlType)
{
	switch(nChnlType)
	{
	case 0:
		return CF_CHTYPE_NOKNOW;
	case 1:
		return CF_CHTYPE_USER;
	case 2:
		return CF_CHTYPE_TRUNK;
	case 9:
		return CF_CHTYPE_RECORD;
	default:
		return CF_CHTYPE_NOKNOW;
		
	}

}

BOOL CChannelForm::CreatChnlConfgFile(LPCTSTR lpszPathName)
{
	CChiefIVRDoc NewDoc;
	NewDoc.OnNewDocument();
	 
	int nCount =  g_pD08a->GetChnlCount();
	NewDoc.m_nTotalChannel = nCount;
	long lKey = CountPlay( g_pD08a->GetVoiceCardKey() );
	for( int i = 0; i < nCount; i++ )
	{
		CCFChnlSetObj *pObj = new CCFChnlSetObj;
		pObj->m_nIndex = i;
		pObj->m_lCardKey  = lKey;
		NewDoc.m_ChSetListObj.AddTail(pObj);
		
	}
	NewDoc.OnSaveDocument(lpszPathName);
	return TRUE;
}

BOOL CChannelForm::CreatSysConfgFile(LPCTSTR lpszPathName)
{
	CCIFSysScriptDoc NewDoc;
	NewDoc.OnNewDocument();
	NewDoc.m_lCardKey = CountPlay( g_pD08a->GetVoiceCardKey() );
	NewDoc.OnSaveDocument( lpszPathName);

	return TRUE;
}

long CChannelForm::CountPlay(long lDTMF)
{
	long kk = 0;
	kk = (lDTMF-100)*10+20-30*10-(700+30-3) + ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 ))) - ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 ))) + ((12*2 - (12-12) + (10*10- 10*10 ))-(12*2 - (12-12) + (10*10- 10*10 )));
	return kk;
}
