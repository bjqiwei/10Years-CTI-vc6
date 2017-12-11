// DrawView.cpp : implementation of the CDrawView class
//

#include "stdafx.h"
#include "ChiefIVRScripted.h"

#include "..//include//NodeObj.h"
#include "WaitCallSpecificProPage.h"
#include "GeneralPage.h"
#include "NodeTreeFormView.h"
#include "CheckBuildFormView.h"
#include "NodeRemarkFormView.h"
#include "FindFormView.h"
#include "SetVariableProPage.h"
#include "PlayProPage.h"
#include "ReleaseCallPage.h"
#include "CallHoldProPage.h"
#include "CallRecordProPage.h"
#include "CompareProPage.h"
#include "OpenDBPage.h"
#include "ConnectNodeDlg.h"
#include "MakeCallCheckPageSpc.h"
#include "EventCheckPageSpc.h"
#include "CallTransferCheckPage.h"
#include "ConferenceEnterPageSpc.h"
#include "ConferLeavePageSpc.h"
#include "DBClosePageSpc.h"
#include "SQLExcutePageSpc.h"
#include "BBBrowseSpc.h"
#include "DLLLoadPageSpc.h"
#include "DLLUnloadPageSpc.h"
#include "DLLCallPageSpc.h"
#include "DTMFSendPageSpc.h"
#include "DTMFRecivePageSpc.h"
#include "DTMFMenuPageSpc.h"
#include "CallLogRequestPageSpc.h"
#include "SwitchPageSpc.h"
#include "FaxRecivePageSpc.h"
#include "FaxSendPageSpc.h"
#include "ConferBuildPage.h"
#include "ConferDeletePage.h"
#include "GetConferMemberPage.h"
#include "ConferRequestPage.h"
#include "StoreProcPage.h"
#include "ChangeVoicePage.h"
#include "ExpressPage.h"
#include "FunctionPage.h"
#include "ReNameDlg.h"
#include "RouteCallProPage.h"
#include "TraceProPage.h"
#include "ModuleProPage.h"
#include "SetParameterDlg.h"
#include "FileListFormView.h"
#include "SelfDefinesPage.h"
#include "CustomTemplate.h"
#include "AddToTemplateDlg.h"
#include "CustomManageDlg.h"
#include "CcTransferPage.h"
#include "CcCallAgentPage.h"

#include "DrawDoc.h"
#include "CntrItem.h"
#include "DrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString g_strAppPath;
DWORD g_dwID = 10;//递增的视图编号

extern CNodeTreeFormView *g_pNodeTreeFormView;
extern CNodeRemarkFormView *g_pFileRemark;
extern CCheckBuildFormView *g_pCheckBuildForm ;
extern CFindFormView *g_FindFormView;
extern CFileListFormView *g_pFileListView ;
extern CObList  g_ViewList;

/////////////////////////////////////////////////////////////////////////////
// CDrawView

IMPLEMENT_DYNCREATE(CDrawView, CScrollView)

BEGIN_MESSAGE_MAP(CDrawView, CScrollView)
	//{{AFX_MSG_MAP(CDrawView)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_COMMAND(ID_OLE_INSERT_NEW, OnInsertObject)
	ON_COMMAND(ID_CANCEL_EDIT_CNTR, OnCancelEditCntr)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CANCELMODE()
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_PROPERTIES, OnEditProperties)
	ON_COMMAND(ID_EDIT_RENAME, OnEditRename)
	ON_COMMAND(ID_CONNECT_CONNECT, OnConnectConnect)
	ON_COMMAND(ID_CONNECT_DISCONNECT, OnConnectDisconnect)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PROPERTIES, OnUpdateEditProperties)
	ON_COMMAND(ID_WAITCALL, OnWaitcall)
	ON_COMMAND(ID_START_PLAY, OnStartPlay)
	ON_COMMAND(ID_START_RECORD, OnStartRecord)
	ON_COMMAND(ID_RELEASECALL, OnReleasecall)
	ON_COMMAND(ID_SETVARIABLE, OnSetvariable)
	ON_COMMAND(ID_COMPARE, OnCompare)
	ON_COMMAND(ID_CALLHOLD, OnCallhold)
	ON_COMMAND(ID_NOTSELECT, OnNotselect)
	ON_COMMAND(ID_SCRIPT_BUILD, OnScriptBuild)
	ON_COMMAND(ID_SCRIPT_CHECK, OnScriptCheck)
	ON_COMMAND(ID_CHECK_TRANSFER, OnCheckTransfer)
	ON_COMMAND(ID_CHECK_DIAL, OnCheckDial)
	ON_COMMAND(ID_ENTER_CONFERENCE, OnEnterConference)
	ON_COMMAND(ID_LEAVE_CONFERENCE, OnLeaveConference)
	ON_COMMAND(ID_OPEND_DATABASE, OnOpendDatabase)
	ON_COMMAND(ID_CLOSE_DATABASE, OnCloseDatabase)
	ON_COMMAND(ID_EXCUTE_SQL, OnExcuteSql)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE, OnUpdateEditDelete)
	ON_COMMAND(ID_DLL_CALL, OnDllCall)
	ON_COMMAND(ID_DLL_LOAD, OnDllLoad)
	ON_COMMAND(ID_DLL_UNLOAD, OnDllUnload)
	ON_COMMAND(ID_SEND_DTMFCODE, OnSendDtmfcode)
	ON_COMMAND(ID_RECIVE_DTMF, OnReciveDtmf)
	ON_COMMAND(ID_EVENT_CHECK, OnEventCheck)
	ON_COMMAND(ID_DATABASE_BROWSE, OnDatabaseBrowse)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_CHECK, OnUpdateScriptCheck)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_BUILD, OnUpdateScriptBuild)
	ON_COMMAND(ID_DTMF_MENU, OnDtmfMenu)
	ON_COMMAND(ID_CALLLOGREQUEST, OnCalllogrequest)
	ON_COMMAND(ID_NODE_END, OnNodeEnd)
	ON_COMMAND(ID_SWITCH, OnSwitch)
	ON_COMMAND(ID_ANSWER, OnAnswer)
	ON_COMMAND(ID_FAXRECIVE, OnFaxrecive)
	ON_COMMAND(ID_FAXSEND, OnFaxsend)
	ON_COMMAND(ID_BUILD_CONFERENCE, OnBuildConference)
	ON_COMMAND(ID_DELETE_CONFERENCE, OnDeleteConference)
	ON_COMMAND(ID_MEMBER_CONFERENCE, OnMemberConference)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_BLOCK, OnChildBlock)
	ON_COMMAND(ID_RETURN, OnReturn)
	ON_COMMAND(ID_STOREDPROC, OnStoredproc)
	ON_COMMAND(ID_CHANGE_VOICE, OnChangeVoice)
	ON_COMMAND(ID_FUNCTION, OnFunction)
	ON_COMMAND(ID_EXPRESS, OnExpress)
	ON_COMMAND(ID_EDIT_BLOCKOPEN, OnEditBlockopen)
	ON_COMMAND(ID_GOTO, OnGoto)
	ON_COMMAND(ID_GOTO_CONNECT, OnGotoConnect)
	ON_COMMAND(ID_GOTO_DISCONNECT, OnGotoDisconnect)
	ON_COMMAND(ID_GOTO_SHOWCONN, OnGotoShowconn)
	ON_COMMAND(ID_ROUTECALL, OnRoutecall)
	ON_COMMAND(ID_TRACE_INFO, OnTraceInfo)
	ON_COMMAND(ID_MODULE, OnModule)
	ON_COMMAND(ID_SET_PARAMETER, OnSetParameter)
	ON_COMMAND(ID_MOUDLE_END, OnMoudleEnd)
	ON_UPDATE_COMMAND_UI(ID_MOUDLE_END, OnUpdateMoudleEnd)
	ON_UPDATE_COMMAND_UI(ID_MODULE, OnUpdateModule)
	ON_COMMAND(ID_CUSTOM_NODE, OnCustomNode)
	ON_COMMAND(ID_EDIT_SAVETOTEMPLATE, OnEditSavetotemplate)
	ON_COMMAND(ID_CUSTOM_MANAGE, OnCustomManage)
	ON_COMMAND(ID_CC_CALLAGENT, OnCcCallagent)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawView construction/destruction

CDrawView::CDrawView()
{
	//m_pSelection = NULL;
	m_nType = 0;
	m_bObjSelected = FALSE;
	m_bNextObjSelected = FALSE;
	m_nObjAction = ACTION_NO_OBJ;
	m_pNextNodeSelected = NULL;
	m_pNodeObjSelected = NULL;
	m_MoveSelectRect.SetRect(0,0,0,0);

	m_hArrow = NULL;
	m_hDraw = NULL;
	m_hDrag = NULL;
	m_hStopDrag= NULL;
	m_nScrollDirect = 0;
	m_bBeginMove = TRUE;

	m_movePoint = CPoint(0,0);
	m_bCopyObj = TRUE;
	m_bPasteObj = TRUE;

	m_nShowMode = 0;
	m_pBlockObj = NULL;
	m_pEndCallBlockObj = NULL;

	m_bShowFocusNode = FALSE;
}

CDrawView::~CDrawView()
{
	m_ToolTip.DestroyWindow();
	while( m_selectionCopied.GetHeadPosition() )
	{
		delete m_selectionCopied.RemoveHead();
	}

	return;
}

BOOL CDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDrawView drawing

void CDrawView::OnDraw(CDC* pDC)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	static int k = 0;
	pDoc->Draw(pDC,this);
	if( m_bObjSelected )
	{
		pDC->Draw3dRect( &m_rect,RGB(212, 208, 200) , RGB(64,64,64));
	}
	else
	{
	}

	if(SCROLL_CENTER != m_nScrollDirect && m_nScrollDirect > 0 )
	{
		if( ACTION_DRAW_LINE == m_nObjAction )
			MouseMoveDrawLine(m_movePoint,pDC);
	}

	//画焦点矩形
    if( m_bShowFocusNode)
	{
		if( NULL == m_pNodeObjSelected ) 
		{
			m_bShowFocusNode = FALSE;
			return;
		}
		if( NODETYPE_ENDCALL_BLOCK == m_pNodeObjSelected->m_nType )
		{
			m_bShowFocusNode = FALSE;
			return;
		}

		CRect EndRect = m_pNodeObjSelected->m_rect;
		DocToClient( EndRect );

		CClientDC dc(this);
		LOGPEN logpen;
		CPen pen;
		CPen* pOldPen = NULL;
		logpen.lopnStyle = PS_SOLID;//实心的线
		logpen.lopnWidth.x = 1;
		logpen.lopnWidth.y = 1;
		logpen.lopnColor = RGB(255,0,0);


		CRect rect2;
		rect2.left = EndRect.left -20;
		rect2.top  = EndRect.top -20;
		rect2.right = EndRect.right + 20;
		rect2.bottom = EndRect.bottom  + 20;
		

		if (!pen.CreatePenIndirect(&logpen))
			return;
		pOldPen = dc.SelectObject(&pen);
		dc.SetROP2(R2_NOTXORPEN );
		int i=0;
		for( i=0; i < 4; i++ )
		{
			rect2.left += 5;
			rect2.top  += 5;
			rect2.right -= 5;
			rect2.bottom -= 5;
			
			dc.DrawFocusRect( &rect2 );
			
		}
		
		dc.DrawFocusRect( &rect2 );

		dc.SelectObject(pOldPen);
		
		m_bShowFocusNode = FALSE;
	}

	return;

}

void CDrawView::OnInitialUpdate()
{



	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	g_dwID=g_dwID+1;
	m_dwID = g_dwID;
	g_ViewList.AddHead(this);
	g_pFileListView->InsertItem( m_dwID , pDoc->m_nDocType,pDoc->GetTitle() );

	CSize sizeTotal = pDoc->GetSize();
	CSize sizePage(sizeTotal.cx / 2, sizeTotal.cy / 2);
	CSize sizeLine(sizeTotal.cx / 50, sizeTotal.cy / 50);
	SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);


	m_bDown = FALSE;
	m_nBaseKey = pDoc->m_nBaseKey ;
	m_nVarBaseKey = pDoc->m_nVarBaseKey ;

	m_hArrow = LoadCursor(NULL,IDC_ARROW);
	m_hDraw = AfxGetApp()->LoadCursor(IDC_DRAW);  
	m_hDrag = AfxGetApp()->LoadCursor(IDC_DROP);  
	m_hStopDrag= AfxGetApp()->LoadCursor(IDC_NODROP);  

	m_pEndCallBlockObj = pDoc->m_pEndCallBlockObj;
	
	m_bViewInit = TRUE;

	m_ToolTip.Create(this);
	CRect rect(1,1,1,1);
	m_ToolTip.AddTool(this, "", &rect, 1);
	m_ToolTip.Activate(TRUE);

	CScrollView::OnInitialUpdate();
	
}

void CDrawView::InsertBlockItem(CNodeObj *pObj)
{
	CBlockObj *pBlockObj = (CBlockObj*)pObj;
	POSITION pos = pBlockObj->m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj* pNewObj = pBlockObj->m_ChildObjLst.GetNext(pos);
		if( NULL != pNewObj )
		{
			
			g_pNodeTreeFormView->InsertBlockItem(pBlockObj->m_nKey,pNewObj->m_nKey,
				pNewObj->m_strCaption,pNewObj->m_nIcoID, pNewObj->m_nIcoID );
			if(  NODETYPE_BLOCK == pNewObj->m_nType )
				InsertBlockItem(pNewObj);//递归调用

		}
	}

	return;
}

//初始化节点树
void CDrawView::InitNodeTreeView()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	g_pNodeTreeFormView->DeleteAllItems();

	g_pNodeTreeFormView->InserRootItem( pDoc->m_nDocType );
	CNodeObjList* pObList = pDoc->GetObjects();
	POSITION posObj = pObList->GetHeadPosition();
	while (posObj)
	{
		CNodeObj* pObj = pObList->GetNext(posObj);
		if( NULL != pObj )
		{

			if( NODETYPE_ENDCALL_BLOCK == pObj->m_nType )
			{
				InsertEndCallBlockItem(pObj);
			}
			else if( NODETYPE_BLOCK == pObj->m_nType )
			{
				g_pNodeTreeFormView->InsertItem( 1, pObj->m_nKey, pObj->m_strCaption, pObj->m_nIcoID, pObj->m_nIcoID );
				InsertBlockItem( pObj );
				
			}
			else
			{
				g_pNodeTreeFormView->InsertItem( 1, pObj->m_nKey, pObj->m_strCaption, pObj->m_nIcoID, pObj->m_nIcoID );
			}
		}
	}		


	CNormalVarList* pNormalObList = &(pDoc->m_NormalVarList);
	posObj = pNormalObList->GetHeadPosition();
	while (posObj)
	{
		CNormalVar* pObj = pNormalObList->GetNext(posObj);
		if( NULL != pObj )
		{
			UINT nImag = 0;
			switch(pObj->m_nVarDataType)
			{

			case DATATYPE_INTEGER:
				nImag=IDI_VARIABLE_INT;
				break;
			case DATATYPE_FLOAT:
				nImag=IDI_VARIABLE_FLOAT;
				break;
			case DATATYPE_STRING:
				nImag=IDI_VARIABLE_STR;
				break;
			default:
				nImag=IDI_VARIABLE_INT;
				break;
			}

			g_pNodeTreeFormView->InsertItem( 2,  pObj->m_nKey,pObj->m_strName, nImag, nImag );

		}
	}


	CDBConnVarList* pDBConObList = &(pDoc->m_DBConnVarList);
	posObj = pDBConObList->GetHeadPosition();
	while (posObj)
	{
		CDBConnVar* pObj = pDBConObList->GetNext(posObj);
		if( NULL != pObj )
			g_pNodeTreeFormView->InsertItem( 3,  pObj->m_nKey+8000,pObj->m_strName, IDI_VARIABLE_DBCON, IDI_VARIABLE_DBCON );
	}

	CRecordsetVarList* pDBRecObList = &(pDoc->m_RecordsetVariList);
	posObj = pDBRecObList->GetHeadPosition();
	while (posObj)
	{
		CRecordsetVar* pObj = pDBRecObList->GetNext(posObj);
		if( NULL != pObj )
			g_pNodeTreeFormView->InsertItem( 3,  pObj->m_nKey,pObj->m_strName, IDI_VARIABLE_DBREC, IDI_VARIABLE_DBREC );
	}

	
	CFDLLVariableList* pDLLObList = &(pDoc->m_DLLVariableList);
	posObj = pDLLObList->GetHeadPosition();
	while (posObj)
	{
		CDLLVariable* pObj = pDLLObList->GetNext(posObj);
		if( NULL != pObj )
			g_pNodeTreeFormView->InsertItem( 4,  pObj->m_nKey+8000,pObj->m_strName, IDI_VARIABLE_DLL, IDI_VARIABLE_DLL );
	}

	

	return;
}

void CDrawView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CMainFrame *pMainFrm = (CMainFrame *)AfxGetApp( )->GetMainWnd();
	pMainFrm->ShowCustomerMenu();
	pDoc->m_pView = this;
	
	if( bActivate )
	{
		if( this != g_pNodeTreeFormView->m_pDrawView )
		{
			InitNodeTreeView();
			g_pNodeTreeFormView->m_pDrawView = this;
		}

		if( this != g_pFileRemark->m_pDrawView )
		{
			g_pFileRemark->m_pDrawView = this;
			
			g_pFileRemark->m_edtRemark.SetWindowText(pDoc->m_strFileRemark);
		}

		if( this != g_pCheckBuildForm->m_pDrawView)
		{
			g_pCheckBuildForm->m_pDrawView = this;
			g_pCheckBuildForm->m_checkTree.DeleteAllItems();
		}
		if( this != g_FindFormView->m_pDrawView)
		{
			g_FindFormView->m_pDrawView = this;
			g_FindFormView->m_ctrFindList.DeleteAllItems();
		}
		
	}
	else
	{
		g_pNodeTreeFormView->m_pDrawView = NULL;
		g_pFileRemark->m_pDrawView = NULL;
		g_pFileRemark->m_edtRemark.SetWindowText("");
		g_pCheckBuildForm->m_pDrawView = NULL;
		g_pCheckBuildForm->m_checkTree.DeleteAllItems();
		g_FindFormView->m_pDrawView = NULL;
		g_FindFormView->m_ctrFindList.DeleteAllItems();
	}
	
	


	CScrollView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


/////////////////////////////////////////////////////////////////////////////
// CDrawView printing

BOOL CDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDrawView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	POSITION pos = g_ViewList.Find(this);
	if (pos != NULL)
	{
		g_ViewList.RemoveAt(pos);
	}
	g_pFileListView->DeleteItem(m_dwID);

	DestroyCursor(m_hDrag);
	DestroyCursor(m_hDraw);
	DestroyCursor(m_hStopDrag);

   CScrollView::OnDestroy();
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }

	DestroyCursor(m_hArrow );
	DestroyCursor(m_hDraw );
	DestroyCursor(m_hDrag);
	DestroyCursor(m_hStopDrag);


   return;
}


/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

BOOL CDrawView::IsSelected(const CObject* pDocItem) const
{
	// The implementation below is adequate if your selection consists of
	//  only CDrawItem objects.  To handle different selection
	//  mechanisms, the implementation here should be replaced.

	// TODO: implement this function that tests for a selected OLE client item

	//return pDocItem == m_pSelection;
	return pDocItem == FALSE;
}

void CDrawView::OnInsertObject()
{
	// Invoke the standard Insert Object dialog box to obtain information
	//  for new CDrawItem object.
	COleInsertDialog dlg;
	if (dlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	CDrawItem* pItem = NULL;
	TRY
	{
		// Create new item connected to this document.
		CDrawDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pItem = new CDrawItem(pDoc);
		ASSERT_VALID(pItem);

		// Initialize the item from the dialog data.
		if (!dlg.CreateItem(pItem))
			AfxThrowMemoryException();  // any exception will do
		ASSERT_VALID(pItem);
		
        if (dlg.GetSelectionType() == COleInsertDialog::createNewItem)
			pItem->DoVerb(OLEIVERB_SHOW, this);

		ASSERT_VALID(pItem);

		// As an arbitrary user interface design, this sets the selection
		//  to the last item inserted.

		// TODO: reimplement selection as appropriate for your application

		//m_pSelection = pItem;   // set selection to last inserted item
		pDoc->UpdateAllViews(NULL);
	}
	CATCH(CException, e)
	{
		if (pItem != NULL)
		{
			ASSERT_VALID(pItem);
			pItem->Delete();
		}
		AfxMessageBox(IDP_FAILED_TO_CREATE);
	}
	END_CATCH

	EndWaitCursor();
}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the container (not the server) causes the deactivation.
void CDrawView::OnCancelEditCntr()
{
	// Close any in-place active item on this view.
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		pActiveItem->Close();
	}
	ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
}

// Special handling of OnSetFocus and OnSize are required for a container
//  when an object is being edited in-place.
void CDrawView::OnSetFocus(CWnd* pOldWnd)
{
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// need to set focus to this item if it is in the same view
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();   // don't call the base class
			return;
		}
	}

	CScrollView::OnSetFocus(pOldWnd);
}

void CDrawView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
	COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
		pActiveItem->SetItemRects();
}

/////////////////////////////////////////////////////////////////////////////
// CDrawView diagnostics

#ifdef _DEBUG
void CDrawView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDrawView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDrawDoc* CDrawView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrawDoc)));
	return (CDrawDoc*)m_pDocument;
}



#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawView message handlers



void CDrawView::OnLButtonDown(UINT nFlags, CPoint point) 
{


	CPoint local = point;
	ClientToDoc(local);
	m_LButtonUpPoint = point;
	SetCapture();

	m_LogicLastPoint = local;//保存鼠标按下时的逻辑坐标
	m_bBeginMove = TRUE;
	switch( m_nObjAction )
	{

	case  ACTION_NO_OBJ:
		m_bDown = TRUE;
		m_ObjSelectTool.OnLButtonDown( this, nFlags, point );
		if( m_bObjSelected )
		{
			if( m_bNextObjSelected )
			{
				m_pNextNodeSelected = GetDocument()->HitTestNextNodeObj( local,this );
				if( m_pNextNodeSelected != NULL )
				{
					m_nObjAction = ACTION_DRAW_LINE;
					m_pNextNodeSelected->m_LineObj.m_OldPoint = m_pNextNodeSelected->m_LineObj.m_BeginPoint;
					
				}
			}
			else
			{
				SingleSelectNodeObj( local);//判断是否进行单选移动等
				m_nObjAction = ACTION_SELECT_OBJ;
				//特别提示与经验总结：在使用自己的光标时，需要特别注意光标的
				//热点位置的设置，需要在绘制光标时，选好光标的热点位置，如选择不当
				//将影响正常工作，可能造成坐标变化的错觉，我因这个问题，折腾了2天，
				//茶点误入歧途！！！！！！！
				::SetCursor(m_hDrag);
				
				
			}
		}

		
		break;
	case  ACTION_ADD_OBJ:
		AddNewNodeObj( nFlags, point );

		break;
	case  ACTION_SELECT_OBJ:
		break;
	case  ACTION_REMOVE_OBJ:
		break;
	case  ACTION_EDIT_OBJ:
		break;
	case  ACTION_DRAW_LINE:

		break;
	default:
		break;

	
	}
	
	

	
}

void CDrawView::OnLButtonUp(UINT nFlags, CPoint point) 
{

	CPoint local = point;
	ClientToDoc(local);
	
	KillTimer(0 );

	switch( m_nObjAction )
	{

	case  ACTION_NO_OBJ:
		m_ObjSelectTool.OnLButtonUp( this, nFlags, point );
		break;
	case  ACTION_ADD_OBJ:
		m_nObjAction = ACTION_NO_OBJ;
		break;
	case  ACTION_SELECT_OBJ://对象选择状态
		MouseUPNodeObj(point);
		
		m_nObjAction = ACTION_NO_OBJ;
		
		break;
	case  ACTION_REMOVE_OBJ:
		break;
	case  ACTION_EDIT_OBJ:
		break;
	case  ACTION_DRAW_LINE:
		MouseUPDrawLine( point);
		m_nObjAction = ACTION_NO_OBJ;
		break;
	default:
		break;

	
	}

	
	
	m_bDown = FALSE;
	//m_bObjSelected = FALSE;


	
	ReleaseCapture();
	//SetCursor(m_hArrow );

	CScrollView::OnLButtonUp(nFlags, point);
}

void CDrawView::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_movePoint = point;
	CPoint local = point;
	ClientToDoc(local);
	if( ACTION_SELECT_OBJ != m_nObjAction ) ShowNodeObjTipText( local );
	ChangeLineColor(local);
	switch( m_nObjAction )
	{
	case  ACTION_NO_OBJ:
		//if( SCROLL_CENTER == BeginScroll(  point ) )
		//{
			
			m_ObjSelectTool.OnMouseMove( this, nFlags, point );
		//}
		break;
	case  ACTION_ADD_OBJ:
		break;
	case  ACTION_SELECT_OBJ:
		if( m_bDown )
		{
			if( SCROLL_CENTER == BeginScroll(  point ) )
			{
				
				MouseMoveNodeObjTo( point );
			}
			else
			{
				if( !m_bBeginMove)
				{
					ClearDragDrawRect(  );
					
					m_bBeginMove = TRUE;
				}
			}	
		}
			
		break;
	case  ACTION_REMOVE_OBJ:
		break;
	case  ACTION_EDIT_OBJ:
		break;
	case  ACTION_DRAW_LINE:
		
			if( SCROLL_CENTER == BeginScroll(  point ) )
			{
				MouseMoveDrawLine( point );
			}
			else
			{
				//MouseMoveDrawLine( point, pDc );
			}
		
		break;
	default:
		
		break;

	
	}
		


	CScrollView::OnMouseMove(nFlags, point);
}


void CDrawView::DrawStartNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CNodeObj * pObj =  new CNodeObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_START );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;

	pObj->SetBaseInfo("Start",hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_START;
	pObj->AddNextNodeObj("Next","",DATATYPE_INTEGER,NEXTTYPE_NEXT,1);
	m_nType = 0;
	pDoc->m_objects.AddTail(pObj);
	
	
	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	

	pDoc->SetModifiedFlag();

	return;
}

void CDrawView::DrawNodeEnd(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CNodeObj * pObj =  new CNodeObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_NODE_END );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("END%d",m_nBaseKey);

	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_NODE_END;


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawGoToNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CGoToObj * pObj =  new CGoToObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_GOTO );
	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str("");
	str.Format("GoTo%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_strConnPath = str + "->";
	pObj->m_nIcoID = IDI_GOTO;
	pObj->m_nType= m_nType;


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}
	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawCallHoldNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CCallHoldObj * pObj =  new CCallHoldObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_CALLHOLD );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("CallHold%d",m_nBaseKey);

	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_CALLHOLD;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );
	



	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
	
}



void CDrawView::DrawCallLogRequestNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CCallLogRequestObj * pObj =  new CCallLogRequestObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_CALLLOGREQUEST );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("CallLogRequest%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_CALLLOGREQUEST;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );



	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawTransferCheckNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CTransferCheckObj * pObj =  new CTransferCheckObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_TRANSFER_CHECK );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("Transfer%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_TRANSFER_CHECK;
	pObj->AddNextNodeObj("Next","",DATATYPE_INTEGER,NEXTTYPE_NEXT,1);
	pObj->AddNextNodeObj("Error","",DATATYPE_INTEGER,NEXTTYPE_ERROR,2);
	pObj->AddNextNodeObj("CancelTransfer","",DATATYPE_INTEGER,NEXTTYPE_CANCEL_TRANSFER,3);

	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawCompareNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	CCompareObj * pObj =  new CCompareObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_COMPARE );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("Compare%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_COMPARE;

	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "NoMatch", "", DATATYPE_INTEGER, NEXTTYPE_NOMATCH, 2 );


	
	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}
	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}


void CDrawView::DrawDialCheckNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CDialCheckObj * pObj =  new CDialCheckObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_DIAL_CHECK );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("MakeCall%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_DIAL_CHECK;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Connect", "", DATATYPE_INTEGER, NEXTTYPE_CONNECT, 2 );
	


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawPlayStartNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CPlayStartObj * pObj =  new CPlayStartObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_PLAY_START );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("Play%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_PLAY_START;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );



	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}


void CDrawView::DrawRecordStartNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CRecordObj * pObj =  new CRecordObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_RECORD_START );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("Record%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_RECORD_START;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}


void CDrawView::DrawReleaseCallNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CReleaseCallObj * pObj =  new CReleaseCallObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_RELASECALL );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("ReleaseCall%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_RELASECALL;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );
	
	
	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawSetVariableNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	//SetVariable节点每个脚本文件只能有一个
	if( pDoc->HaveNodeObj( m_nType ) )
	{
		this->MessageBox("The SetViriable is only one! you can not add new one!");
		m_nType = 0;
		return;
	}
	
	CNodeObj * pObj =  new CNodeObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_SETVARIABLE );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("SetVariable%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_SETVARIABLE;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawWaitCallNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	if( 2== m_nShowMode ) return;
	CWaitCallObj  * pObj =  new CWaitCallObj(point);

	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_WAITCALL );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("WaitCall%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_WAITCALL;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );

	

	
	pObj->m_nRings			= 3;
	pObj->m_bClearDTMF		= TRUE;
	pObj->m_bRecCalledID    = FALSE;
	pObj->m_bRecCallerID    = FALSE;



	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	CScrollView::OnPrepareDC(pDC, pInfo);
	
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	

	pDC->SetMapMode(MM_TEXT);//2005/12/10增加
	pDC->SetWindowExt(1440, 1440);
	pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX),
		pDC->GetDeviceCaps(LOGPIXELSY));
	



	// set the origin of the coordinate system to the center of the page
	CPoint ptOrg;
	ptOrg.x = pDoc->GetSize().cx / 2;
	ptOrg.y = pDoc->GetSize().cy / 2;

	// ptOrg is in logical coordinates
	pDC->OffsetWindowOrg(-ptOrg.x,ptOrg.y);
	

}

inline void CDrawView::ClientToDoc(CPoint &point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(&point);
}

BOOL CDrawView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CDrawView::HitTestNodeObj(CPoint point, BOOL &bNextObjSelected )
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	bNextObjSelected = FALSE;
	CNodeObj * pObj = pDoc->HitTestNodeObj( point,this );
	if( pObj == NULL )
	{
		m_initialPoint.x = 0;
		m_initialPoint.y = 0;
		m_Objhigh = 0;
		m_Objwidth = 0;
		
		
		return FALSE;
	}
	else
	{
		pObj->SetSelected(TRUE);
		m_Objhigh = pObj->GetHigh();
		m_Objwidth = pObj->GetWidth();
		m_initialPoint = point;
		CNextNode *pNextObj = pObj->HitTestNextNode( point );
		if( pNextObj != NULL )
			bNextObjSelected = TRUE;


		
		return TRUE;
	}
	
}

void CDrawView::MouseMoveNodeObjTo(CPoint point )
{

	CRect position;
	CClientDC dc(this);
	CPoint local = point;
	ClientToDoc(local);
	CPoint delta = (CPoint)(local - m_LogicLastPoint );


	POSITION pos = m_selection.GetHeadPosition();
	while (pos)
	{
		CNodeObj* pObj = m_selection.GetNext(pos);
		if( pObj != NULL)
		{
			//判断是否是刚开始拖动，如是，将不画开始筐，
			//如果没有该判断，将在一拖动时，在所选对象周围出现边框
			if( !m_bBeginMove)
			{
				position = pObj->m_MoveRect;
				DocToClient( position );
				dc.DrawFocusRect(position);//清除拖动时产生的上一个矩形
			}

			position = pObj->m_MoveRect;//保存拖动时产生的对象矩形
			position += delta;
			
			pObj->m_MoveRect = position;
			DocToClient( position );
			dc.DrawFocusRect(position);//拖动时产生的上一个矩形



		}

	}

	//画拖动的外框
	

	
	if( !m_bBeginMove)
	{
		position = m_MoveSelectRect;
		DocToClient( position );
		dc.DrawFocusRect(position);//清除拖动时产生的上一个矩形
	}

	position = m_MoveSelectRect;//保存拖动时产生的对象矩形
	position += delta;
	m_MoveSelectRect = position;
	DocToClient( position );
	dc.DrawFocusRect(position);//拖动时产生的上一个矩形
	

	m_LogicLastPoint = local;
	m_bBeginMove = FALSE;

}

void CDrawView::OnCancelMode() 
{
	CScrollView::OnCancelMode();
	
}


BOOL CDrawView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CScrollView::OnEraseBkgnd(pDC);

	
}



inline void CDrawView::ClientToDoc(CRect &rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.DPtoLP(rect);
}

void CDrawView::DocToClient(CRect &rect)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(rect);
	rect.NormalizeRect();
}
void CDrawView::DocToClient(CPoint &point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc, NULL);
	dc.LPtoDP(&point);
}

void CDrawView::TrackerHitTest(CPoint point)
{
	CDrawDoc* pDoc=GetDocument ();
	ASSERT_VALID (pDoc);

	if( !m_bObjSelected )
	{

	}




	 return;
}

//////////////////////////////////////////////////
//将pObj加入到选取对象的列表
//
/////////////////////////////////////////////////
void CDrawView::Select(CNodeObj *pObj, BOOL bAdd)
{
	if (!bAdd)
	{
		m_selection.RemoveAll();
	}

	if ( pObj == NULL )
		return;

	m_selection.AddTail(pObj);

	return;
}

//////////////////////////////////////////////////
//判断矩形内有那些对象，然后选取，将所有选取的pObj
//加入到选取对象的列表，但由于有2种编辑模式，
//需要分开处理矩形选择
//
/////////////////////////////////////////////////
void CDrawView::SelectWithinRect(CRect rect, BOOL bAdd)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (!bAdd)
		Select(NULL);//先清除原来的选择
	
	CRect SelectRect(0,0,0,0);

	ClientToDoc(rect);
	m_selection.RemoveAll();
	CNodeObjList* pObList	= NULL;
	CNodeObj* pObj			= NULL;
	POSITION posObj			= NULL;

	if( 0 == m_nShowMode )
	{
		//普通编辑
		pObList = pDoc->GetObjects();
		
	}
	else if( 1 == m_nShowMode )
	{
		//编辑Block成员状态
		pObList = &(m_pBlockObj->m_ChildObjLst);
	}
	else if( 2 == m_nShowMode )
	{
		//编辑Block成员状态
		pObList = &(m_pEndCallBlockObj->m_ChildObjLst);
	}
	posObj = pObList->GetHeadPosition();
	while (posObj)
	{
		pObj = pObList->GetNext(posObj);
		if (pObj->Intersects(rect))
		{
			pObj->m_MoveRect = pObj->m_rect;
			SelectRect |= pObj->m_rect;
			Select(pObj, TRUE);
			pObj->SetSelected(TRUE);

		}
		else
		{
			pObj->SetSelected(FALSE);
		}
		InvalObj( pObj );
	}
	if( pObList->GetCount() > 0 )
		m_bObjSelected = TRUE;
	else
		m_bObjSelected = FALSE;

	SelectRect.left -= 2;
	SelectRect.top -= 2;
	SelectRect.bottom += 2;
	SelectRect.right += 2;

	m_MoveSelectRect = SelectRect;
	return;


}

void CDrawView::RemoveNodeObj(CNodeObj *pObj)
{
	POSITION pos = m_selection.Find(pObj);
	if (pos != NULL)
		m_selection.RemoveAt(pos);

	return;
}

void CDrawView::InvalObj(CNodeObj *pObj)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRect rect = pObj->m_rect;
	DocToClient( rect );
		rect.left -= 4;
		rect.top -= 5;
		rect.right += 5;
		rect.bottom += 4;

	InvalidateRect(rect, FALSE);

	pDoc->SetModifiedFlag();
	return;
}

//移动节点，鼠标抬起时的操作
void CDrawView::MouseUPNodeObj(CPoint point)
{

	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	BOOL bMove = FALSE;
	CPoint local = point;
	ClientToDoc(local);
	CPoint delta = (CPoint)(local - m_LogicLastPoint );

	CRect tempRect1,uRect;
	GetClientRect( &tempRect1);
	uRect.SetRect(0,0,0,0);
	ClientToDoc(tempRect1);

	//判断拖动是否超出客户区域
	uRect =  m_MoveSelectRect;
	uRect |= tempRect1;
	if( uRect == tempRect1)
		bMove = TRUE;
	else
		bMove = FALSE;


	CClientDC dc(this);
	CRect position;
	POSITION pos = m_selection.GetHeadPosition();
	while (pos)
	{
		CNodeObj* pObj = m_selection.GetNext(pos);
		if( pObj != NULL)
		{

			
			position = pObj->m_MoveRect;
			DocToClient( position );
			dc.DrawFocusRect(position);//清除拖动时产生的矩形

			position = pObj->m_MoveRect;
			position += delta;
		
			CPoint NewPoint;
			NewPoint.x =position.left;
			NewPoint.y = position.top;
			CRect oldRect = pObj->m_rect;
			

			oldRect.left -= 4;
			oldRect.top -= 5;
			oldRect.right += 5;
			oldRect.bottom += 4;	
			DocToClient( oldRect );

			if( bMove )
			{
				//未超出客户区域，所选对象移动
				pObj->MoveTo(NewPoint);
				//下面的操作是重画连线，因为位置的变化，所以要把关联的
				//节点的连线要处理
				pDoc->MoveLineObj( pObj->m_nKey,pObj->m_rect );
			}
			
			InvalidateRect(oldRect, TRUE);//这里必须用TRUE,擦除背景
			InvalObj( pObj );

		}

	}
	


	//清除拖动时产生的外框
	position = m_MoveSelectRect;
	DocToClient( position );
	dc.DrawFocusRect(position);//清除拖动时产生的矩形


	Invalidate(TRUE);
		

}


//添加节点函数
void CDrawView::AddNewNodeObj(UINT nFlags, CPoint point)
{
	CPoint local = point;
	ClientToDoc(local);

	switch( m_nType )
	{
	case  NODETYPE_WAITCALL:
		DrawWaitCallNode( nFlags, local);
		break;
	case  NODETYPE_START_PLAY:
		DrawPlayStartNode( nFlags, local);
		break;
	case NODETYPE_CHECK_DIAL:
		DrawDialCheckNode( nFlags, local);
		break;
	case  NODETYPE_RELEASECALL:
		DrawReleaseCallNode( nFlags, local);
		break;
	case  NODETYPE_CALLHOLD:
		DrawCallHoldNode( nFlags, local);
		break;
	case NODETYPE_CHECK_TRANSFER:
		DrawTransferCheckNode( nFlags, local);
		break;
	case  NODETYPE_START_RECORD:
		DrawRecordStartNode( nFlags, local);
		break;
	case  NODETYPE_SETVARIABLE:
		DrawSetVariableNode( nFlags, local);
		break;
	case  NODETYPE_COMPARE:
		DrawCompareNode( nFlags, local);
		break;
	case  NODETYPE_ENTER_CONFERENCE:
		DrawConferenceEnterNode( nFlags, local);
		break;
	case  NODETYPE_LEAVE_CONFERENCE:
		DrawConferenceLeaveNode( nFlags, local);
		break;
	case  NODETYPE_OPEN_DATABASE:
		DrawDataBaseOpenNode( nFlags, local);
		break;
	case  NODETYPE_CLOSE_DATABASE:
		DrawDataBaseCloseNode( nFlags, local);
		break;
	case  NODETYPE_EXCUTE_SQL:
		DrawExcuteSQLNode( nFlags, local);
		break;
	case  NODETYPE_DATABASE_BROWSE:
		DrawDBBrowseNode( nFlags, local);
		break;
	case  NODETYPE_DLL_LOAD:
		DrawDLLLoadNode( nFlags, local);
		break;
	case  NODETYPE_DLL_UNLOAD:
		DrawDLLUnLoadNode( nFlags, local);
		break;
	case  NODETYPE_DLL_CALL:
		DrawDLLCallNode( nFlags, local);
		break;
	case  NODETYPE_SEND_DTMFCODE:
		DrawDTMFSendNode( nFlags, local);
		break;
	case  NODETYPE_RECIVE_DTMF:
		DrawDTMFReciveNode( nFlags, local);
		break;
	case  NODETYPE_EVENT_CHECK:
		DrawEventCheckNode( nFlags, local);
		break;
	case NODETYPE_DTMF_MENU:
		DrawDTMFMenu( nFlags, local);
		break;
	case NODETYPE_CALLLOGREQUEST:
		DrawCallLogRequestNode( nFlags, local);
		break;
	case NODETYPE_NODE_END:
		DrawNodeEnd( nFlags, local);
		break;
	case NODETYPE_SWITCH:
		DrawSwitchNode( nFlags, local);
		break;
	case NODETYPE_ANSWERCALL:
		DrawAnswerNode( nFlags, local);
		break;
	case NODETYPE_FAXRECIVE:
		DrawFaxReciveNode( nFlags, local);
		break;
	case NODETYPE_FAXSEND:
		DrawFaxSendNode( nFlags, local);
		break;
	case NODETYPE_CONFERENCE_BUILD:
		DrawConferBuildNode( nFlags, local);
		break;
	case NODETYPE_CONFERENCE_DELETE:
		DrawConferDeleteNode( nFlags, local);
		break;
	case NODETYPE_GET_CONFERENCE_MEMBER:
		DrawGetConferMemberNode( nFlags, local);
		break;
	case NODETYPE_CONFERENCE_REQUEST:
		DrawConferRequestNode( nFlags, local);
		break;
	case NODETYPE_BLOCK:
		DrawBlockNode( nFlags, local);
		break;
	case NODETYPE_ENTER:
		//DrawEnterNode( nFlags, local);
		break;
	case NODETYPE_RETURN:
		DrawReturnNode( nFlags, local);
		break;
	case NODETYPE_STOREPRO:
		DrawStorePro( nFlags, local);
		break;
	case NODETYPE_CHANGEVOICE:
		DrawChangeVoiceNode( nFlags, local);
		break;
	case NODETYPE_EXPRESS:
		DrawExpressNode( nFlags, local);
		break;
	case NODETYPE_FUNCTION:
		DrawFunctionNode( nFlags, local);
		break;
	case NODETYPE_GOTO:
		DrawGoToNode( nFlags, local);
		break;
	case NODETYPE_ROUTE_CALL:
		DrawRouteCallNode( nFlags, local);
		break;
	case NODETYPE_TRACE_INFO:
		DrawTraceNode( nFlags, local);
		break;
	case NODETYPE_MOUDLE:
		DrawModuleNode( nFlags, local);
		break;
	case NODETYPE_MOUDLE_BEGIN:
		break;
	case NODETYPE_MOUDLE_END:
		DrawModuleEndNode( nFlags, local);
		break;
	case NODETYPE_CUSTOM:
		DrawCustomNode( nFlags, local);
		break;
	case NODETYPE_START://0
		break;
	case NODETYPE_CC_CALLAGENT:
		DrawCCCallAgent( nFlags, local);
		break;
	case NODETYPE_CC_TRANSFER:
		DrawCCTransfer( nFlags, local);
		break;
	default:
		{
			//自定义节点
			DrawCustomNew( nFlags, local);
		}
		break;
	
	}


	::SetCursor(m_hArrow);

	return;
}

//当NextNode被选中，鼠标移动时用来画线
void CDrawView::MouseMoveDrawLine(CPoint point)
{

	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	if( !m_bNextObjSelected )
		return;
	if( NULL == m_pNextNodeSelected )
		return;

	//将实际鼠标拖动点再延长，检测是否靠近某个结点，同时连接
	CPoint BeginPoint = m_pNextNodeSelected->m_LineObj.m_BeginPoint;
	CPoint OldPoint = m_pNextNodeSelected->m_LineObj.m_OldPoint;

	CClientDC dc(this);
	
	LOGPEN logpen;
	CPen pen;
	CPen* pOldPen = NULL;
	logpen.lopnStyle = PS_SOLID;//实心的线
	logpen.lopnWidth.x = 1;
	logpen.lopnWidth.y = 1;
	logpen.lopnColor = RGB(255,0,0);

	


	if (!pen.CreatePenIndirect(&logpen))
		return;
	pOldPen = dc.SelectObject(&pen);
	BeginPoint = m_pNextNodeSelected->m_LineObj.m_BeginPoint;
	OldPoint = m_pNextNodeSelected->m_LineObj.m_OldPoint;
	DocToClient( BeginPoint );
	DocToClient( OldPoint );

	//画连接线
	dc.SetROP2(R2_NOTXORPEN );
	dc.MoveTo(BeginPoint);
	dc.LineTo(OldPoint);
	dc.MoveTo(BeginPoint);
	dc.LineTo(point);

	OldPoint = point;
	ClientToDoc( OldPoint );
	m_pNextNodeSelected->m_LineObj.m_OldPoint = OldPoint;

	dc.SelectObject(pOldPen);




	return;
}

//用于滚动条滚动时的连线
void CDrawView::MouseMoveDrawLine(CPoint point, CDC *pDc)
{

	CPoint local = point;
	ClientToDoc(local);

	if( !m_bNextObjSelected )
		return;
	if( NULL == m_pNextNodeSelected )
		return;
	
	LOGPEN logpen;
	CPen pen;
	CPen* pOldPen = NULL;
	logpen.lopnStyle = PS_SOLID;//实心的线
	logpen.lopnWidth.x = 1;
	logpen.lopnWidth.y = 1;
	logpen.lopnColor = RGB(255,0,0);

	

	if (!pen.CreatePenIndirect(&logpen))
		return;
	pOldPen = pDc->SelectObject(&pen);
	CPoint BeginPoint = m_pNextNodeSelected->m_LineObj.m_BeginPoint;
	CPoint OldPoint = m_pNextNodeSelected->m_LineObj.m_OldPoint;

	//画连接线
	pDc->MoveTo(BeginPoint);
	pDc->LineTo(local);

	OldPoint = local;
	m_pNextNodeSelected->m_LineObj.m_OldPoint = OldPoint;




	pDc->SelectObject(pOldPen);

	return;
}

void CDrawView::MouseUPDrawLine(CPoint point)
{
	if( !m_bNextObjSelected )
		return;
	if( NULL == m_pNextNodeSelected )
		return;

	CClientDC dc(this);
	
	LOGPEN logpen;
	CPen pen;
	CPen* pOldPen = NULL;
	logpen.lopnStyle = PS_SOLID;//实心的线
	logpen.lopnWidth.x = 1;
	logpen.lopnWidth.y = 1;
	logpen.lopnColor = RGB(255,0,0);

	

	if (!pen.CreatePenIndirect(&logpen))
		return;
	pOldPen = dc.SelectObject(&pen);
	CPoint BeginPoint = m_pNextNodeSelected->m_LineObj.m_BeginPoint;
	CPoint OldPoint = m_pNextNodeSelected->m_LineObj.m_OldPoint;
	DocToClient( BeginPoint );
	DocToClient( OldPoint );

	//画连接线
	dc.SetROP2(R2_NOTXORPEN );
	dc.MoveTo(BeginPoint);
	dc.LineTo(OldPoint);


	OldPoint = point;
	ClientToDoc( OldPoint );
	m_pNextNodeSelected->m_LineObj.m_OldPoint = OldPoint;

	dc.SelectObject(pOldPen);

	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	CNodeObj * pObj = pDoc->HitTestNodeObj( OldPoint,this );
	if( pObj != NULL )
	{
		if( NULL == pObj->HitTestNextNode(OldPoint)   )//判断是否落在起点上
		{
			
			m_pNextNodeSelected->ConnectToNodeObj( pObj->m_strCaption, pObj->m_nType, pObj->m_nKey, 
											pObj->m_rect );
			

		}
	}

	Invalidate(TRUE);
	
	return;
}


void CDrawView::SingleSelectNodeObj(CPoint local)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
				
	CNodeObj * pObj = pDoc->HitTestNodeObj(  local, this  );
	if( NULL != pObj )
	{
		//如果单选时
		if( m_selection.IsEmpty()  )
		{
			m_selection.AddTail(pObj);
			pObj->m_MoveRect = pObj->m_rect;
			m_MoveSelectRect = pObj->m_rect;
			m_MoveSelectRect.left -= 2;
			m_MoveSelectRect.top -= 2;
			m_MoveSelectRect.bottom += 2;
			m_MoveSelectRect.right += 2;
		}
		else if( m_selection.GetCount() == 1 )
		{

				POSITION pos = m_selection.GetTailPosition();
				while (pos)
				{
					if( pObj == m_selection.GetPrev(pos))
					{
						pObj->m_MoveRect = pObj->m_rect;
						m_MoveSelectRect = pObj->m_rect;
						m_MoveSelectRect.left -= 2;
						m_MoveSelectRect.top -= 2;
						m_MoveSelectRect.bottom += 2;
						m_MoveSelectRect.right += 2;
						return;
					}

				}
				m_selection.RemoveAll();
				m_selection.AddTail(pObj);
				pObj->m_MoveRect = pObj->m_rect;
				m_MoveSelectRect = pObj->m_rect;
				m_MoveSelectRect.left -= 2;
				m_MoveSelectRect.top -= 2;
				m_MoveSelectRect.bottom += 2;
				m_MoveSelectRect.right += 2;


		}


	}
	

	return;


}

BOOL CDrawView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CScrollView::OnScroll(nScrollCode, nPos, bDoScroll);
}

void CDrawView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CDrawView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}

/*********************************************************
该函数用在当鼠标左键按下，选中对象开始拖动对象时，判断是
否需要滚动，同时相应处理；函数根据上下左右组合方向判断。


**********************************************************/
int CDrawView::BeginScroll( CPoint point )
{
	CPoint local = point;
	CRect tempRect1;
	GetClientRect( &tempRect1);
	

	int y		= GetScrollPos(SB_VERT);
	int x		= GetScrollPos(SB_HORZ);
	int y_max	= GetScrollLimit( SB_VERT );
	int x_max	= GetScrollLimit( SB_HORZ );

	if( (tempRect1.right   - local.x >= 10 && local.x - tempRect1.left >= 10) && 
			( local.y - tempRect1.top >= 10 && tempRect1.bottom - local.y >=10  )  )
	{
		KillTimer(0 );
		SetCursor(m_hDrag);
		m_nScrollDirect = SCROLL_CENTER;//在中间
		
		return SCROLL_CENTER;


	}
	
	else if( (tempRect1.bottom  - local.y <= 10 && tempRect1.bottom - local.y > 0 ) &&
				(local.x - tempRect1.left > 10  && tempRect1.right - local.x >10  ) )
	{
		SetCursor(m_hDrag);
		m_nScrollDirect = SCROLL_DOWN;//正下方
		if( y < y_max )//当水平滚动条不在最正下方时，需要启动时钟自动滚动
			SetTimer(0,50,NULL);

		return SCROLL_DOWN;
	}

	else if( (tempRect1.bottom  - local.y <= 10 && tempRect1.bottom - local.y > 0 ) && 
				(tempRect1.right - local.x <= 10 && tempRect1.right - local.x > 0 ) )
	{
		SetTimer(0,50,NULL);
		SetCursor(m_hDrag);
		m_nScrollDirect = SCROLL_RIGHT_DOWN;//右下方

		return SCROLL_RIGHT_DOWN;

	}
	else if( (tempRect1.right - local.x  <=10  && tempRect1.right - local.x  > 0 ) && 
				( local.y  - tempRect1.top > 10  && tempRect1.bottom - local.y > 10 ))
	{

		SetCursor(m_hDrag);
		m_nScrollDirect = SCROLL_RIGHT;//正右方
		if( x < x_max )//当水平滚动条不在最右方时，需要启动时钟自动滚动
			SetTimer(0,50,NULL);


		return SCROLL_RIGHT;
		
	}
	else if( (tempRect1.right - local.x  <= 10  && tempRect1.right - local.x  > 0 ) && 
		( local.y -  tempRect1.top <= 10 && local.y  -  tempRect1.top > 0) )
	{
		
		SetTimer(0,50,NULL);

		SetCursor(m_hDrag);
		m_nScrollDirect = SCROLL_RIGHT_UP;//右上方


		return SCROLL_RIGHT_UP;

	}
	else if( (local.y  -  tempRect1.top <= 10 && local.y  -  tempRect1.top > 0) && 
		( local.x - tempRect1.left > 10  && tempRect1.right - local.x > 10   )  )
	{
		SetCursor(m_hDrag);
		m_nScrollDirect = SCROLL_UP;//正上方
		if( y > 0 )//当垂直滚动条不在最上端时，需要启动时钟自动滚动
			SetTimer(0,50,NULL);


		return SCROLL_UP;

	}
	else if( (local.y  -  tempRect1.top <= 10 && local.y  -  tempRect1.top > 0) 
			&& (local.x - tempRect1.left <= 10 && local.x - tempRect1.left > 0)  )
	{
		SetTimer(0,50,NULL);
		SetCursor(m_hDrag);
		m_nScrollDirect = SCROLL_LEFT_UP;//左上方

		return SCROLL_LEFT_UP;

	}
	else if( (local.x   -  tempRect1.left  <= 10 && local.x   -  tempRect1.left > 0) &&
			 ( local.y - tempRect1.top > 10 && tempRect1.bottom  - local.y > 10  ) )
	{
		SetCursor(m_hDrag);
		m_nScrollDirect = SCROLL_LEFT;//正左方
		if( x > 0 )//当水平滚动条不在最左端时，需要启动时钟自动滚动
		   SetTimer(0,50,NULL);

		return SCROLL_LEFT;

	}
	else if( (local.x   -  tempRect1.left  <= 10 && local.x   -  tempRect1.left > 0) && 
		(tempRect1.bottom - local.y <= 10 && tempRect1.bottom - local.y > 0) )
	{
		SetTimer(0,50,NULL);
		SetCursor(m_hDrag);
		m_nScrollDirect = SCROLL_LEFT_DOWN;//左下方

		return SCROLL_LEFT_DOWN;

	}
	
	else 
	{

		SetCursor(m_hStopDrag);
		m_nScrollDirect = SCROLL_AREA_OUT;//区域外
		KillTimer(0 );
		
		return SCROLL_AREA_OUT;
	}

}

void CDrawView::OnTimer(UINT nIDEvent) 
{
	
	
	int y		= GetScrollPos(SB_VERT);
	int x		= GetScrollPos(SB_HORZ);
	int y_max	= GetScrollLimit( SB_VERT );
	int x_max	= GetScrollLimit( SB_HORZ );
	 
	if( 0 == nIDEvent )
	{
		switch( m_nScrollDirect )//判断滚动方向
		{
		case  SCROLL_DOWN://正下方
			if(y == y_max) StopScroll();	
			y += 100;
			SetScrollPos(SB_VERT,y,TRUE); 

			break;
		case  SCROLL_RIGHT://正右方
			if(x == x_max) StopScroll();
			x += 100;
			SetScrollPos(SB_HORZ,x,TRUE); 
			break;
		case  SCROLL_UP://正上方
			if(y == 0) StopScroll();
			y -= 100;
			SetScrollPos(SB_VERT,y,TRUE); 
			break;
		case  SCROLL_LEFT://正左方
			if(x == 0) StopScroll();
			x -= 100;
			SetScrollPos(SB_HORZ,x,TRUE); 
			break;
		case  SCROLL_RIGHT_DOWN://右下方
			if(y == y_max && x == x_max ) StopScroll();
			y += 100;
			x += 100;
			SetScrollPos(SB_HORZ,x,TRUE); 
			SetScrollPos(SB_VERT,y,TRUE); 
			break;
		case  SCROLL_RIGHT_UP://右上方
			if(y == 0 && x == x_max ) StopScroll();
			y -= 100;
			x += 100;
			SetScrollPos(SB_HORZ,x,TRUE); 
			SetScrollPos(SB_VERT,y,TRUE); 
			break;
		case  SCROLL_LEFT_DOWN://左下方
			if(x == 0 && y == y_max ) StopScroll();
			y += 100;
			x -= 100;
			SetScrollPos(SB_HORZ,x,TRUE); 
			SetScrollPos(SB_VERT,y,TRUE); 
			break;
		case  SCROLL_LEFT_UP://左上方
			if(x == 0 && y == 0 ) StopScroll();
			y -= 100;
			x -= 100;
			SetScrollPos(SB_HORZ,x,TRUE); 
			SetScrollPos(SB_VERT,y,TRUE); 
			break;
		default:
			break;


		}

		Invalidate(TRUE);
	}
	else
	{


	}

	CScrollView::OnTimer(nIDEvent);
}

BOOL CDrawView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CScrollView::OnScrollBy(sizeScroll, bDoScroll);
}

void CDrawView::StopScroll()
{

	KillTimer(0 );
	//Invalidate();
	return;
}

BOOL CDrawView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	POINT ptCursor;
	CRect tempRect1;


	::GetCursorPos( &ptCursor );//取得屏幕上的坐标，不是客户区的坐标
	ScreenToClient( &ptCursor );//将屏幕坐标转换为客户区坐标
	GetClientRect( &tempRect1);//取得客户区矩形


	if( ACTION_ADD_OBJ == m_nObjAction && tempRect1.PtInRect(ptCursor)  )
	{
		::SetCursor(m_hDraw);
		return TRUE;
	}

	
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}


void CDrawView::ResetDragDraw(CPoint point)
{
	CRect position(0,0,0,0);
	CPoint local = point;
	ClientToDoc(local);
	CPoint delta = (CPoint)(local - m_LogicLastPoint );

	CRect SelectRect(0,0,0,0);
	POSITION pos = m_selection.GetHeadPosition();
	while (pos)
	{
		CNodeObj* pObj = m_selection.GetNext(pos);
		if( pObj != NULL)
		{
			position = pObj->m_MoveRect ;//保存拖动时产生的对象矩形
			position += delta;
			pObj->m_MoveRect = position;
			SelectRect |= pObj->m_MoveRect;
		}

	}

	SelectRect.left -= 2;
	SelectRect.top -= 2;
	SelectRect.bottom += 2;
	SelectRect.right += 2;

	m_MoveSelectRect = SelectRect;

	return;
}

void CDrawView::ClearDragDrawRect(  )
{
	CRect position(0,0,0,0);
	CClientDC dc(this);

	POSITION pos = m_selection.GetHeadPosition();
	while (pos)
	{
		CNodeObj* pObj = m_selection.GetNext(pos);
		if( pObj != NULL)
		{
			position = pObj->m_MoveRect;
			DocToClient( position );
			dc.DrawFocusRect(position);//清除拖动时产生的上一个矩形
		}

	}

	position = m_MoveSelectRect;
	DocToClient( position );
	dc.DrawFocusRect(position);//清除拖动时产生的上一个矩形



	return;
}

void CDrawView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
				
	CPoint local = point;
	ClientToDoc(local);
	m_pNodeObjSelected = pDoc->HitTestNodeObj(  local, this  );
	if( NULL == m_pNodeObjSelected  )  
	{
		return;//没有有对象被选中
	}

	m_pNextNodeSelected = GetDocument()->HitTestNextNodeObj( local,this );
	if( m_pNextNodeSelected == NULL )
	{
		EditNodeObjProperty();
	}
	else
	{
		//当双击NodeObj对象的NextNodeObj时，将连接的对象自动显示在屏幕左上角
		if( m_pNextNodeSelected->m_nNodeObjKey < 100 )//判断是否连接下一个NodeObj
			return;

		SetCapture();
		int y		= GetScrollPos(SB_VERT);
		int x		= GetScrollPos(SB_HORZ);
		CPoint endPoint;
		CRect EndRect = m_pNextNodeSelected->m_LineObj.m_EndRect;

		DocToClient( EndRect );

		CRect cRect;
		GetClientRect( &cRect);

		endPoint.x = EndRect.left  ;
		endPoint.y = EndRect.top ;//+ 8;
		SetScrollPos(SB_HORZ, x  + EndRect.left - cRect.left,TRUE); 
		SetScrollPos(SB_VERT, y + EndRect.top - cRect.top,TRUE); 		

		ReleaseCapture();
		Invalidate(TRUE);
		
	}
	

	
	CScrollView::OnLButtonDblClk(nFlags, point);
}

void CDrawView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
				
	

	CPoint local = point;
	ClientToDoc(local);
	
	

	CMenu menu;
	CPoint popPoint;	
	menu.LoadMenu( IDR_POP_EDIT );
	GetCursorPos(&popPoint);//得到鼠标位置
	

	
	m_pNodeObjSelected = pDoc->HitTestNodeObj(  local, this  );
	if( NULL == m_pNodeObjSelected  )  
	{
		m_bObjSelected = FALSE;
		m_bCopyObj = TRUE;
		m_LogPointCopied = local;
		menu.GetSubMenu(3)->TrackPopupMenu(TPM_LEFTALIGN ,popPoint.x,popPoint.y ,AfxGetMainWnd());
		menu.DestroyMenu();
		return;//没有有对象被选中
	}

	m_bObjSelected = TRUE;
	m_pNextNodeSelected = GetDocument()->HitTestNextNodeObj( local,this );
	if( m_pNextNodeSelected != NULL )//点击到Next Node
	{
		menu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN ,popPoint.x,popPoint.y ,AfxGetMainWnd());
	}
	else//没有点击到Next Node
	{
		m_LogPointInit= local;
		m_bCopyObj = FALSE;
		if( NODETYPE_BLOCK == m_pNodeObjSelected->m_nType )
			menu.GetSubMenu(7)->TrackPopupMenu(TPM_LEFTALIGN ,popPoint.x,popPoint.y ,AfxGetMainWnd());
		else if( NODETYPE_GOTO == m_pNodeObjSelected->m_nType )
		{
			menu.GetSubMenu(8)->TrackPopupMenu(TPM_LEFTALIGN ,popPoint.x,popPoint.y ,AfxGetMainWnd());
		}
		else if( NODETYPE_CUSTOM == m_pNodeObjSelected->m_nType )
		{
			menu.GetSubMenu(9)->TrackPopupMenu(TPM_LEFTALIGN ,popPoint.x,popPoint.y ,AfxGetMainWnd());
		}
		else
			menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN ,popPoint.x,popPoint.y ,AfxGetMainWnd());
		
	}
	menu.DestroyMenu();
	CScrollView::OnRButtonDown(nFlags, point);
}

void CDrawView::OnEditCopy() 
{
	while( m_selectionCopied.GetHeadPosition() )
	{
		delete m_selectionCopied.RemoveHead();
	}

	POSITION pos = m_selection.GetHeadPosition();
	while ( pos )
	{
		CNodeObj * pObj =m_selection.GetNext(pos) ;
		if( pObj != NULL )
		{
			//判断是否是SetVariable结点，如果是，就不拷贝
			if( NODETYPE_SETVARIABLE == pObj->m_nType ) continue;
			if( NODETYPE_ENTER == pObj->m_nType ) continue;
			//if( NODETYPE_BLOCK == pObj->m_nType ) continue;
			CopyObj( pObj );
		}
		
	}


	m_bCopyObj = FALSE;
	m_bPasteObj  = FALSE;
	
}

void CDrawView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bCopyObj);
	
	
}

void CDrawView::OnEditCut() 
{
    
	
}

void CDrawView::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}




void CDrawView::OnEditProperties() 
{
	EditNodeObjProperty();
	
	return;
	
}

void CDrawView::OnUpdateEditProperties(CCmdUI* pCmdUI) 
{
	//pCmdUI->Enable(m_bObjSelected);
	
}

void CDrawView::OnEditRename() 
{
	CReNameDlg dlg;
	dlg.Init( m_pNodeObjSelected );
	dlg.DoModal();
	
	return;
}

void CDrawView::OnConnectConnect() 
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	CConnectNodeDlg dlg;
	dlg.InitDlg( pDoc, m_pNextNodeSelected ,this, m_nShowMode );
	if( dlg.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}

	
}

void CDrawView::OnConnectDisconnect() 
{
	if( NULL != m_pNextNodeSelected )
	{
		m_pNextNodeSelected->DisConnectToNodeObj();
		Invalidate(TRUE);
	}
	
}

CString CDrawView::GetFoldersAndFile()
{
	CString str(_T(""));
	BROWSEINFO bi;
	char name[MAX_PATH];
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner=GetSafeHwnd();
	bi.pszDisplayName=name;
	bi.lpszTitle="Select folder";
	bi.ulFlags=BIF_EDITBOX|BIF_BROWSEINCLUDEFILES;
	LPITEMIDLIST idl=SHBrowseForFolder(&bi);
	if(idl==NULL)
		return _T("");
	SHGetPathFromIDList(idl,str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer();
	return str;
}

CString CDrawView::GetFolders()
{
	CString str(_T("")), strPath(_T(""));
	BROWSEINFO bi;
	char name[MAX_PATH];
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner=GetSafeHwnd();
	bi.pszDisplayName=name;
	bi.lpszTitle="Select folder";
	//bi.ulFlags=BIF_USENEWUI;
	bi.ulFlags=BIF_EDITBOX;//|BIF_BROWSEINCLUDEFILES;
	LPITEMIDLIST idl=SHBrowseForFolder(&bi);
	if(idl==NULL)
		return _T("");
	SHGetPathFromIDList(idl,str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer();
	strPath=str;
	if(str.GetAt(str.GetLength()-1)!='\\')
		strPath+="\\";
	return strPath;
}

CString CDrawView::GetChiefScriptFile()
{
	
    char szFilters[]="Chief IVR execute Script file (*.cfs)|*.cfs||";     

	CFileDialog Dlg(TRUE,"pef","*.pef",OFN_FILEMUSTEXIST, szFilters, this);
	if(Dlg.DoModal()==IDOK)
	{
		return Dlg.GetPathName();
	}
	else
		return _T("");

}

void CDrawView::EditNodeObjProperty()
{
	if( NULL == m_pNodeObjSelected )
		return;

	switch( m_pNodeObjSelected->m_nType )
	{
	case  NODETYPE_WAITCALL:
		EditWaitCallNodeProperty(  );
		break;
	case  NODETYPE_START_PLAY:
		EditPlayStartNodeProperty();
		break;
	case NODETYPE_CHECK_DIAL:
		EditDialCheckProperty();
		break;
	case  NODETYPE_RELEASECALL:
		EditReleaseCallProperty();
		break;
	case  NODETYPE_CALLHOLD:
		EditCallHoldProperty();
		break;
	case NODETYPE_CHECK_TRANSFER:
		EditTransferCheckProperty();
		break;
	case  NODETYPE_START_RECORD:
		EditRecordStartProperty();
		break;
	case  NODETYPE_SETVARIABLE:
		EditSetVariableProperty();
		break;
	case  NODETYPE_COMPARE:
		EditCompareProperty();
		break;
	case  NODETYPE_ENTER_CONFERENCE:
		EditConfEnterNodeProperty();
		break;
	case  NODETYPE_LEAVE_CONFERENCE:
		EditConfLeaveNodeProperty();
		break;
	case NODETYPE_OPEN_DATABASE:
		EditDBOpenProperty();
		break;
	case  NODETYPE_CLOSE_DATABASE:
		EditDBCloseProperty();
		break;
	case  NODETYPE_EXCUTE_SQL:
		EditSQLExcuteProperty();
		break;
	case  NODETYPE_DATABASE_BROWSE:
		EditDBBrowseProperty();
		break;
	case  NODETYPE_DLL_LOAD:
		EditDLLLoadProperty();
		break;
	case  NODETYPE_DLL_UNLOAD:
		EditDLLUnLoadProperty();
		break;
	case  NODETYPE_DLL_CALL:
		EditDLLCallProperty();
		break;
	case  NODETYPE_SEND_DTMFCODE:
		EditDTMFSendProperty();
		break;
	case  NODETYPE_RECIVE_DTMF:
		EditDTMFReciveProperty();
		break;

	case NODETYPE_EVENT_CHECK:
		EditEventCheckProperty();
		break;
	case NODETYPE_DTMF_MENU:
		EditDTMFMenuProperty();
		break;
	case NODETYPE_CALLLOGREQUEST:
		EditCallLogRequestProperty();
		break;	
	case NODETYPE_SWITCH:
		EditSwitchProperty();
		break;
	case NODETYPE_NODE_END:
		EditNodeEndProperty();
		break;
	case NODETYPE_ANSWERCALL:
		EditAnswerCallProperty();
		break;	
	case NODETYPE_FAXRECIVE:
		EditFaxReciveProperty();
		break;
	case NODETYPE_FAXSEND:
		EditFaxSendProperty();
		break;
	case NODETYPE_CONFERENCE_BUILD:
		EditBuildConferProperty();
		break;
	case NODETYPE_CONFERENCE_DELETE:
		EditDeleteConferProperty();
		break;
	case NODETYPE_GET_CONFERENCE_MEMBER:
		EditGetConferMemberProperty();
		break;
	case NODETYPE_CONFERENCE_REQUEST:
		EditRequestConferProperty();
		break;
	case NODETYPE_BLOCK:
		EditBlockProperty();
		break;
	case NODETYPE_ENTER:
		EditEnterProperty();
		break;
	case NODETYPE_RETURN:
		EditReturnProperty();
		break;
	case NODETYPE_STOREPRO:
		EditStoreProProperty();
		break;
	case NODETYPE_CHANGEVOICE:
		EditChangeVoiceProperty();
		break;
	case NODETYPE_EXPRESS:
		EditExpressProperty();
		break;
	case NODETYPE_FUNCTION:
		EditFunctionProperty();
		break;
	case NODETYPE_GOTO:
		OnGotoConnect();
		break;
	case NODETYPE_ROUTE_CALL:
		EditRouteCallProperty();
		break;
	case NODETYPE_TRACE_INFO:
		EditTraceProperty();
		break;
	case NODETYPE_MOUDLE:
		EditModuleProperty();
		break;
	case NODETYPE_MOUDLE_BEGIN:
		EditModuleBeginProperty();
		break;
	case NODETYPE_MOUDLE_END:
		EditModuleEndProperty();
		break;
	case NODETYPE_CUSTOM:
		EditCustomProperty();
		break;
	case NODETYPE_CC_CALLAGENT:
		EditCcCallAgentProperty();
		break;
	case NODETYPE_CC_TRANSFER:
		EditCcTransferProperty();
		break;
	default:
		break;
	
	}

	return;
}

void CDrawView::EditRecordStartProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Record properties",this,0);
	CCallRecordProPage   specificPage;
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected, pDoc );
	specificPage.InitPage( ( CRecordObj *)m_pNodeObjSelected, pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	sheet.DoModal();
}




void CDrawView::EditTransferCheckProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Transfer properties",this,0);
	CCallTransferCheckPage    specificPage;
	CGeneralPage		generalPage;

	generalPage.InitPage( m_pNodeObjSelected ,pDoc );
	specificPage.InitPage( ( CTransferCheckObj * )m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if( sheet.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}

	return;
}
void CDrawView::EditCallHoldProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Call hold properties",this,0);
	CCallHoldProPage   specificPage;
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected ,pDoc );
	specificPage.InitPage( (CCallHoldObj*)m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	sheet.DoModal();
}

void CDrawView::EditCallLogRequestProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("CallLog Request properties",this,0);
	CCallLogRequestPageSpc   specificPage;
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected ,pDoc );
	specificPage.InitPage( (CCallLogRequestObj*)m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	sheet.DoModal();

	return;
}

void CDrawView::EditReleaseCallProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("ReleaseCall properties",this,0);
	CReleaseCallPage   specificPage;
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected,pDoc );
	specificPage.InitPage( ( CReleaseCallObj *)m_pNodeObjSelected );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	sheet.DoModal();

	return;
}

void CDrawView::EditCompareProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Compare properties",this,0);
	CCompareProPage    specificPage;
	CGeneralPage		generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected, pDoc );
	specificPage.InitPage( (CCompareObj *)m_pNodeObjSelected, pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if( sheet.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}

	return;
}


void CDrawView::EditDTMFSendProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("DTMF Send properties",this,0);
	CDTMFSendPageSpc    specificPage;
	CGeneralPage		generalPage;

	generalPage.InitPage( m_pNodeObjSelected ,pDoc );
	specificPage.InitPage( (CDTMFSendObj*)m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if( sheet.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}

	return;
}

void CDrawView::EditDTMFReciveProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("DTMF Recive properties",this,0);
	CDTMFRecivePageSpc    specificPage;
	CGeneralPage		generalPage;

	generalPage.InitPage( m_pNodeObjSelected ,pDoc );
	specificPage.InitPage( (CDTMFReciveObj *)m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW|PSH_HASHELP;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if( sheet.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}

	return;
}

void CDrawView::EditDTMFMenuProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("DTMF Menu properties",this,0);
	CDTMFMenuPageSpc    specificPage;
	CGeneralPage		generalPage;

	generalPage.InitPage( m_pNodeObjSelected ,pDoc );
	specificPage.InitPage( (CDTMFMenuObj*)m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if( sheet.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}

	return;
}

void CDrawView::EditEventCheckProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Event Check properties",this,0);
	CEventCheckPageSpc    specificPage;
	CGeneralPage		generalPage;

	generalPage.InitPage( m_pNodeObjSelected ,pDoc );
	specificPage.InitPage( (CEventCheckObj*)m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if( sheet.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}

	return;


}

void CDrawView::EditDialCheckProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("MakeCall properties",this,0);
	CMakeCallCheckPageSpc    specificPage;
	CGeneralPage		generalPage;

	generalPage.InitPage( m_pNodeObjSelected ,pDoc );
	specificPage.InitPage( ( CDialCheckObj * )m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if( sheet.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}

	return;
}

void CDrawView::EditConfEnterNodeProperty()
{

	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Enter Conference properties",this,0);
	CConferenceEnterPageSpc   specificPage;
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected,pDoc );
	specificPage.InitPage( (CEnterConferObj*)m_pNodeObjSelected,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	sheet.DoModal();

	return;
}

void CDrawView::EditConfLeaveNodeProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Leave Conference properties",this,0);
	CConferLeavePageSpc   specificPage;
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected, pDoc );
	specificPage.InitPage( (CLeaveConferObj*)m_pNodeObjSelected, pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	sheet.DoModal();
}

void CDrawView::EditPlayStartNodeProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Play properties",this,0);
	CPlayProPage   specificPage;
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected,pDoc );
	specificPage.InitPage( ( CPlayStartObj*)m_pNodeObjSelected,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	sheet.DoModal();

	return;
}



void CDrawView::EditWaitCallNodeProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("WaitCall properties",this,0);
	CWaitCallSpecificProPage   specificPage;
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected,pDoc );
	specificPage.InitPage( (CWaitCallObj*)m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if(sheet.DoModal() ==IDOK)
	{
		
		return;
	}

	return;
}



void CDrawView::EditSetVariableProperty()
{

	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("SetVariable properties",this,0);
	CSetVariableProPage   specificPage;
	CGeneralPage   generalPage;


	generalPage.InitPage( m_pNodeObjSelected,pDoc);
	
	specificPage.InitPage( m_pNodeObjSelected, pDoc, &(pDoc->m_nVarBaseKey));
	

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );
	
	if(sheet.DoModal() ==IDOK)
	{
		InitNodeTreeView();
	}

	return;
}
void CDrawView::EditDBOpenProperty()
{

	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("DBOpen properties",this,0);
	COpenDBPage   specificPage;
	CGeneralPage   generalPage;


	generalPage.InitPage( m_pNodeObjSelected,pDoc);
	
	specificPage.InitPage( (CDBOpenObj*)m_pNodeObjSelected, pDoc);
	

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );
	
	if(sheet.DoModal() ==IDOK)
	{

	}

	return;
}

void CDrawView::EditDBBrowseProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("DBBrowse properties",this,0);
	CBBBrowseSpc   specificPage;
	CGeneralPage   generalPage;


	generalPage.InitPage( m_pNodeObjSelected,pDoc);
	
	specificPage.InitPage( (CDBBrowseObj * )m_pNodeObjSelected, pDoc);
	

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );
	
	if(sheet.DoModal() ==IDOK)
	{

	}

	return;
}
void CDrawView::EditSQLExcuteProperty()
{

	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("SQL Excute properties",this,0);
	CSQLExcutePageSpc   specificPage;
	CGeneralPage   generalPage;


	generalPage.InitPage( m_pNodeObjSelected,pDoc);
	
	specificPage.InitPage( (CSQLExcuteObj*)m_pNodeObjSelected, pDoc);
	

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );
	
	if(sheet.DoModal() ==IDOK)
	{

	}

	return;
}

void CDrawView::EditDBCloseProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("DB Close properties",this,0);
	CDBClosePageSpc   specificPage;
	CGeneralPage   generalPage;


	generalPage.InitPage( m_pNodeObjSelected,pDoc);
	
	specificPage.InitPage( (CDBCloseObj *)m_pNodeObjSelected, pDoc);
	

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );
	
	if(sheet.DoModal() ==IDOK)
	{

	}

	return;
}


void CDrawView::EditDLLLoadProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("DLL Load properties",this,0);
	CDLLLoadPageSpc   specificPage;
	CGeneralPage   generalPage;


	generalPage.InitPage( m_pNodeObjSelected,pDoc);
	
	specificPage.InitPage( (CDLLLoadObj *)m_pNodeObjSelected, pDoc);
	

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );
	
	if(sheet.DoModal() ==IDOK)
	{

	}

	return;
}

void CDrawView::EditDLLUnLoadProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	CPropertySheet sheet("DLL UnLoad properties",this,0);
	CDLLUnloadPageSpc   specificPage;
	CGeneralPage   generalPage;


	generalPage.InitPage( m_pNodeObjSelected,pDoc);
	
	specificPage.InitPage( (CDLLUnLoadObj*)m_pNodeObjSelected, pDoc);
	

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );
	
	if(sheet.DoModal() ==IDOK)
	{

	}

	return;
}

void CDrawView::EditDLLCallProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	CPropertySheet sheet("DLL Call properties",this,0);
	CDLLCallPageSpc   specificPage;
	CGeneralPage   generalPage;


	generalPage.InitPage( m_pNodeObjSelected,pDoc);
	
	specificPage.InitPage( (CDLLCallObj*)m_pNodeObjSelected, pDoc);
	

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );
	
	if(sheet.DoModal() ==IDOK)
	{

	}

	return;
}

CNodeObj *  CDrawView::FindNodeObj(int nKey)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CNodeObj * pObj = pDoc->FindNodeObj( nKey );
	return pObj;
}


////////////////////////////////////////////////////
//让选中的节点在视图中显示
//
///////////////////////////////////////////////////
void CDrawView::ShowFindObjToSelected(CNodeObj *pObj)
{
	if( NULL == pObj ) return;
	SetCapture();
	int y		= GetScrollPos(SB_VERT);
	int x		= GetScrollPos(SB_HORZ);
	CPoint endPoint;
	CRect EndRect = pObj->m_rect;

	DocToClient( EndRect );

	CRect cRect;
	GetClientRect( &cRect);
	CRect local = cRect;
	ClientToDoc(local);
	if( !pObj->Intersects(local) )//判断该节点是否显示
	{
		//如果没显示，让其显示
		endPoint.x = EndRect.left  ;
		endPoint.y = EndRect.top ;//+ 8;
		SetScrollPos(SB_HORZ, x  + EndRect.left - cRect.left-300,TRUE); 
		SetScrollPos(SB_VERT, y + EndRect.top - cRect.top-300,TRUE); 
	}
	
	m_pNodeObjSelected = pObj;
    m_bShowFocusNode = TRUE;

	ReleaseCapture();
	Invalidate(TRUE);
}

void CDrawView::FindNodeObj(LPCTSTR lpszName)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CNodeObj * pObj = pDoc->FindNodeObj( lpszName );
	if( NULL == pObj ) return;

	pObj->SetSelected(TRUE);
	SetCapture();
	int y		= GetScrollPos(SB_VERT);
	int x		= GetScrollPos(SB_HORZ);
	CPoint endPoint;
	CRect EndRect = pObj->m_rect;

	DocToClient( EndRect );

	CRect cRect;
	GetClientRect( &cRect);

	endPoint.x = EndRect.left  ;
	endPoint.y = EndRect.top ;//+ 8;
	SetScrollPos(SB_HORZ, x  + EndRect.left - cRect.left,TRUE); 
	SetScrollPos(SB_VERT, y + EndRect.top - cRect.top,TRUE); 		

	ReleaseCapture();
	Invalidate(TRUE);

	return;
}





void CDrawView::OnWaitcall() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_WAITCALL;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::OnStartPlay() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_START_PLAY;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::OnStartRecord() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_START_RECORD;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::OnReleasecall() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_RELEASECALL;
	m_nObjAction = ACTION_ADD_OBJ;
}

void CDrawView::OnSetvariable() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_SETVARIABLE;
	m_nObjAction = ACTION_ADD_OBJ;
	
}


void CDrawView::OnCheckDial() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_CHECK_DIAL;
	m_nObjAction = ACTION_ADD_OBJ;
	
}
void CDrawView::OnCompare() 
{

	::SetCursor(m_hDraw);
	m_nType = NODETYPE_COMPARE;
	m_nObjAction = ACTION_ADD_OBJ;
	
}
void CDrawView::OnCalllogrequest() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_CALLLOGREQUEST;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::OnCheckTransfer() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_CHECK_TRANSFER;
	m_nObjAction = ACTION_ADD_OBJ;	
	
}
void CDrawView::OnCallhold() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_CALLHOLD;
	m_nObjAction = ACTION_ADD_OBJ;	
	
}

void CDrawView::OnDllCall() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_DLL_CALL;
	m_nObjAction = ACTION_ADD_OBJ;	
	
}


void CDrawView::OnDllLoad() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_DLL_LOAD;
	m_nObjAction = ACTION_ADD_OBJ;	
	
}
void CDrawView::OnEventCheck() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_EVENT_CHECK;
	m_nObjAction = ACTION_ADD_OBJ;
	
	
}

void CDrawView::OnDllUnload() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_DLL_UNLOAD;
	m_nObjAction = ACTION_ADD_OBJ;	
	
}

void CDrawView::OnSendDtmfcode() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_SEND_DTMFCODE;
	m_nObjAction = ACTION_ADD_OBJ;	
	
}

void CDrawView::OnReciveDtmf() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_RECIVE_DTMF;
	m_nObjAction = ACTION_ADD_OBJ;	
	
}

void CDrawView::OnDtmfMenu() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_DTMF_MENU;
	m_nObjAction = ACTION_ADD_OBJ;	

}

void CDrawView::OnNotselect() 
{
	// TODO: Add your command handler code here
	
}

void CDrawView::OnScriptBuild() 
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->BuildScriptFile();//开始检查脚本文件

	if( 1 == pDoc->m_nDocType )
	{
		char szFilters[]="Module Excute script(*.cmf)|*.cmf||";	  
		CFileDialog Dlg(FALSE,"","",OFN_FILEMUSTEXIST, szFilters, this);

		if(Dlg.DoModal()==IDOK)
		{
			CString strPath = Dlg.GetPathName();
			SaveBuildModuleFile( strPath );
		}

	}
	else
	{
		char szFilters[]="Normal Excute script(*.cef)|*.cef||";		  
		CFileDialog Dlg(FALSE,"","",OFN_FILEMUSTEXIST, szFilters, this);

		if(Dlg.DoModal()==IDOK)
		{
			CString strPath = Dlg.GetPathName();
			SaveBuildScriptFile( strPath );
		}

	}

	return;
	
}

void CDrawView::OnUpdateScriptBuild(CCmdUI* pCmdUI) 
{

	
}

void CDrawView::OnScriptCheck() 
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	pDoc->CheckScriptFile();//开始检查脚本文件



	return;
	
	
}
void CDrawView::OnUpdateScriptCheck(CCmdUI* pCmdUI) 
{

	
}


void CDrawView::OnEnterConference() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_ENTER_CONFERENCE ;
	m_nObjAction = ACTION_ADD_OBJ;	
	
}

void CDrawView::OnLeaveConference() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_LEAVE_CONFERENCE  ;
	m_nObjAction = ACTION_ADD_OBJ;	
	
}

void CDrawView::OnOpendDatabase() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_OPEN_DATABASE   ;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::OnCloseDatabase() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_CLOSE_DATABASE  ;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::OnExcuteSql() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_EXCUTE_SQL;
	m_nObjAction = ACTION_ADD_OBJ;
	
}
void CDrawView::OnDatabaseBrowse() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_DATABASE_BROWSE;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::DrawConferenceEnterNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CEnterConferObj * pObj =  new CEnterConferObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_CONFERENCE_ENTER );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("EnterConference%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_CONFERENCE_ENTER;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}
void CDrawView::DrawConferDeleteNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CConferenceDeleteObj * pObj =  new CConferenceDeleteObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_CONFER_DELETE );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("DeleteConference%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_CONFER_DELETE;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawGetConferMemberNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CGetConferMemberObj * pObj =  new CGetConferMemberObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_CONFER_MEMBER );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("ConferenceInfo%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_CONFER_MEMBER;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}
void CDrawView::DrawConferenceLeaveNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CLeaveConferObj * pObj =  new CLeaveConferObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_CONFERENCE_LEAVE );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("LeaveConference%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_CONFERENCE_LEAVE;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawDataBaseOpenNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	CDBOpenObj * pObj =  new CDBOpenObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_DATABASE_OPEN );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("DBOpen%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_DATABASE_OPEN;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );
	pObj->m_nDBKey = m_nBaseKey;
	pObj->m_strDBName.Format("DataBase%d",m_nBaseKey);


	CDBConnVar *pDBConVar =  new CDBConnVar;
	pDBConVar->m_nKey = m_nBaseKey;
	pDBConVar->m_nVarDataType = DATATYPE_DBCONNECT;
	pDBConVar->m_strName.Format("DataBase%d",m_nBaseKey);
	pDoc->m_DBConnVarList.AddTail( pDBConVar );//将其保存
	g_pNodeTreeFormView->InsertItem(3, pDBConVar->m_nKey+8000,pDBConVar->m_strName, IDI_VARIABLE_DBCON , IDI_VARIABLE_DBCON );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawDBBrowseNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	CDBBrowseObj * pObj =  new CDBBrowseObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_DATABASE_BROWSE );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("DBBrowse%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_DATABASE_BROWSE;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "EOF==TRUE", "", DATATYPE_INTEGER, NEXTTYPE_EOF_FALSE, 2 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 3 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}
	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawDataBaseCloseNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	CDBCloseObj * pObj =  new CDBCloseObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_DATABASE_CLOASE );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("DBClose%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_DATABASE_CLOASE;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );



	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawEventCheckNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	//if( 2== m_nShowMode ) return;
	CEventCheckObj * pObj =  new CEventCheckObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_EVENT_CHECK );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("EventCheck%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_EVENT_CHECK;
	pObj->AddNextNodeObj("Error","",DATATYPE_INTEGER,NEXTTYPE_ERROR,1);


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}
	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawExcuteSQLNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	CSQLExcuteObj * pObj =  new CSQLExcuteObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_EXCUTE_SQL );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str("");
	str.Format("ExcuteSQL%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_EXCUTE_SQL;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );
	pObj->m_nRecordsetKey = m_nBaseKey;
	pObj->m_nDBType = 0;


	CRecordsetVar *pRset =  new CRecordsetVar;
	pRset->m_nKey = m_nBaseKey;
	pRset->m_nVarDataType = DATATYPE_DBRECORDSET;
	pRset->m_strName.Format("Recordset%d",m_nBaseKey);
	pDoc->m_RecordsetVariList.AddTail( pRset );//将其保存
	g_pNodeTreeFormView->InsertItem(3,pRset->m_nKey+8000,pRset->m_strName, IDI_VARIABLE_DBREC , IDI_VARIABLE_DBREC );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}
	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawDLLLoadNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	CDLLLoadObj * pObj =  new CDLLLoadObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_DLL_LOAD );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("DLLLoad%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_DLL_LOAD;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );
	pObj->m_nHandleType = 0;
	pObj->m_nDLLKey = m_nBaseKey;

	CDLLVariable *pDLLVar =  new CDLLVariable;
	pDLLVar->m_nKey = m_nBaseKey;
	pDLLVar->m_nVarDataType = DATATYPE_DLL;
	pDLLVar->m_strName.Format("DLL%d",m_nBaseKey);
	pDoc->m_DLLVariableList.AddTail( pDLLVar );//将其保存
	g_pNodeTreeFormView->InsertItem(4,pDLLVar->m_nKey+8000,pDLLVar->m_strName, IDI_VARIABLE_DLL , IDI_VARIABLE_DLL );
	


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}
	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawDLLUnLoadNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	CDLLUnLoadObj * pObj =  new CDLLUnLoadObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_DLL_UNLOAD );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str("");
	str.Format("DLLUnLoad%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_DLL_UNLOAD;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );



	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}
	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawDLLCallNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	CDLLCallObj * pObj =  new CDLLCallObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_DLL_CALL );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str("");
	str.Format("DLLCall%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_DLL_CALL;
	pObj->m_nHandleType = 0;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );




	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}
	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;

}

void CDrawView::DrawDTMFSendNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CDTMFSendObj * pObj =  new CDTMFSendObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_DTMF_SEND );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str("");
	str.Format("DTMFSend%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_DTMF_SEND;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawDTMFReciveNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CDTMFReciveObj * pObj =  new CDTMFReciveObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_DTMF_RECIVE );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str("");
	str.Format("DTMFRecive%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_DTMF_RECIVE;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );



	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawDTMFMenu(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CDTMFMenuObj * pObj =  new CDTMFMenuObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_DTMF_MENU );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("DTMFMenu%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_DTMF_MENU;
	pObj->AddNextNodeObj("TimeOut","",DATATYPE_INTEGER,NEXTTYPE_DTMF_TIMEOUT,1);
	pObj->AddNextNodeObj("Error","",DATATYPE_INTEGER,NEXTTYPE_ERROR,2);


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bPasteObj);
	
	return;
}


void CDrawView::PasteReplaceNextKey( CNodeObjList *pNOlst,int nNodeKey , int nNewNodeKey )
{

	POSITION pos = pNOlst->GetHeadPosition();
	while( pos )
	{
		CNodeObj *pObj =  pNOlst->GetNext(pos);
		if( pObj == NULL ) continue;

		POSITION pos2 = pObj->m_NextNodeList.GetHeadPosition();
		while( pos2 )
		{
			CNextNode *pNObj =  pObj->m_NextNodeList.GetNext(pos2);
			if( pNObj == NULL ) continue;
			
			pNObj->m_LineObj.m_BeginPoint = pNObj->GetLinePoint();
			if( nNodeKey == pNObj->m_nNodeObjKey )
			{
				pNObj->m_nNodeObjKey = nNewNodeKey;
			}
			else
			{
				PasteQueryReplaceNextKey( pNOlst, pNObj);
			}

		}
	}
	return ;

}

void CDrawView::PasteQueryReplaceNextKey( CNodeObjList *pNOlst,CNextNode *pNObj  )
{
	POSITION pos = pNOlst->GetHeadPosition();
	int i = 0;
	while( pos )
	{
		CNodeObj *pObj =  pNOlst->GetNext(pos);
		if( pObj == NULL ) continue;
		if( pNObj->m_nNodeObjKey == pObj->m_nKey )
			i = 1;

	}

	if( 0 == i )
	{
		pNObj->m_nNodeObjKey = 0;
		pNObj->m_strNodeObjName = "";
	}
	return ;

}

void CDrawView::PasteReplaceNext(CNodeObjList *pNOlst, CNodeObj *pObj )
{
	POSITION pos = pNOlst->GetHeadPosition();
	while( pos )
	{
		CNodeObj *pNewObj =  pNOlst->GetNext(pos);
		if( pNewObj == NULL ) continue;

		POSITION pos2 = pNewObj->m_NextNodeList.GetHeadPosition();
		while( pos2 )
		{
			CNextNode *pNObj =  pNewObj->m_NextNodeList.GetNext(pos2);
			if( pNObj == NULL ) continue;
			
			pNObj->m_LineObj.m_BeginPoint = pNObj->GetLinePoint();
			if( pObj->m_nKey == pNObj->m_nNodeObjKey )
			{
				pNObj->ConnectToNodeObj( pObj->m_strCaption, pObj->m_nType,pObj->m_nKey,pObj->m_rect);
				
			}

		}
	}
	return ;

}

void CDrawView::OnEditPaste() 
{

	
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPoint delta(0,0);
	CPoint NewPoint(0,0);
	CPoint tempPoint(0,0);
	CRect  newRect(0,0,0,0);
	

	delta = (CPoint)(m_LogPointCopied - m_LogPointInit );
	
	POSITION pos = m_selectionCopied.GetHeadPosition();
	while ( pos )
	{
		CNodeObj * pObj =m_selectionCopied.GetNext(pos) ;
		if( pObj != NULL )
		{
			
			m_nBaseKey +=1;
			PasteReplaceNextKey( &m_selectionCopied,pObj->m_nKey,m_nBaseKey);

			pObj->m_nKey = m_nBaseKey;
			//pObj->m_strCaption.Format("%d",m_nBaseKey);
			
			pDoc->m_nBaseKey =m_nBaseKey;
			
			
			if( NODETYPE_OPEN_DATABASE == pObj->m_nType )
			{
				CDBOpenObj *pDB = (CDBOpenObj*)pObj;
				pDB->m_nDBKey = m_nBaseKey;
				if( 0 == pDB->m_nDBType )
				{
					CDBConnVar *pDBConVar =  new CDBConnVar;
					pDBConVar->m_nKey = m_nBaseKey;
					pDBConVar->m_nVarDataType = DATATYPE_DBCONNECT;
					pDBConVar->m_strName.Format("DataBase%d",m_nBaseKey);
					pDB->m_strDBName.Format("DataBase%d",m_nBaseKey);
					pDoc->m_DBConnVarList.AddTail( pDBConVar );
				
					g_pNodeTreeFormView->InsertItem(3,pDBConVar->m_nKey+8000,pDBConVar->m_strName, IDI_VARIABLE_DBCON , IDI_VARIABLE_DBCON );
				}
			}
			else if( NODETYPE_DLL_LOAD == pObj->m_nType )
			{
				CDLLLoadObj *pDLL = (CDLLLoadObj *)pObj;
				pDLL->m_nDLLKey = m_nBaseKey;
				if( 0 == pDLL->m_nHandleType )
				{
					CDLLVariable *pDLLVar =  new CDLLVariable;
					pDLLVar->m_nKey = m_nBaseKey;
					pDLLVar->m_nVarDataType = DATATYPE_DLL;
					pDLLVar->m_strName.Format("DLL%d",m_nBaseKey);
					
					pDoc->m_DLLVariableList.AddTail( pDLLVar );
					g_pNodeTreeFormView->InsertItem(4,pDLLVar->m_nKey+8000,pDLLVar->m_strName, IDI_VARIABLE_DLL , IDI_VARIABLE_DLL );
				}
			}
			else if( NODETYPE_BLOCK == pObj->m_nType )
			{
				CBlockObj * pNewObj =  (CBlockObj *)pObj;
				m_nBaseKey= pNewObj->PasteNodeObj(&(pDoc->m_DBConnVarList), &(pDoc->m_DLLVariableList), &(pDoc->m_RecordsetVariList), pNewObj, m_nBaseKey );
				pDoc->m_nBaseKey = m_nBaseKey;
				
			}
			
			newRect = pObj->m_rect;
			newRect += delta;
			NewPoint.x = newRect.left ;
			NewPoint.y = newRect.top ;

			pObj->MoveTo( NewPoint );
			
			if( 0 == m_nShowMode )
			{
				//根据当前的编辑模式来判断
				pDoc->m_objects.AddTail(pObj);
				g_pNodeTreeFormView->InsertItem(1,pObj->m_nKey,pObj->m_strCaption, pObj->m_nIcoID , pObj->m_nIcoID );
				if( NODETYPE_BLOCK == pObj->m_nType )
				{
					CBlockObj * pNewObj =  (CBlockObj *)pObj;
					InsertBlockItem(pNewObj);
				}

					
			}
			else if( 1 == m_nShowMode )
			{
				//Block编辑状态
				m_pBlockObj->m_ChildObjLst.AddTail(pObj);
				g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey ,pObj->m_strCaption, pObj->m_nIcoID, pObj->m_nIcoID);
				if( NODETYPE_BLOCK == pObj->m_nType )
				{
					CBlockObj * pNewObj =  (CBlockObj *)pObj;
					InsertBlockItem(pNewObj);
				}
			}
			else if( 2 == m_nShowMode )
			{
				m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
				g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey ,pObj->m_strCaption, pObj->m_nIcoID, pObj->m_nIcoID);
				if( NODETYPE_BLOCK == pObj->m_nType )
				{
					CBlockObj * pNewObj =  (CBlockObj *)pObj;
					InsertBlockItem(pNewObj);
				}
			}
			
			
	
			CRect rect = pObj->m_rect;
			DocToClient( rect );
			InvalidateRect(rect, FALSE);
		}
		
	}


	POSITION pos3 = m_selectionCopied.GetHeadPosition();
	while ( pos3 )
	{
		CNodeObj * pObj =m_selectionCopied.GetNext(pos3) ;
		if( pObj != NULL )
		{
			PasteReplaceNext( &m_selectionCopied, pObj );
		}

	}

	Invalidate(FALSE);
	m_selectionCopied.RemoveAll();

	pDoc->SetModifiedFlag();


}


void CDrawView::DeleteBlock(CBlockObj *pObj)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	POSITION pos = pObj->m_ChildObjLst.GetHeadPosition();
	while(pos)
	{
		CNodeObj * pData1 = pObj->m_ChildObjLst.GetAt(pos);
		if( NULL == pData1 ) return;
		if( NODETYPE_SETVARIABLE == pData1->m_nType )// 当是设置变量节点
		{
			pDoc->RemoveAllNormalVariable();
		}
		else if( NODETYPE_OPEN_DATABASE == pData1->m_nType )
		{
			CDBOpenObj *pDB = (CDBOpenObj*)pData1;
			pDoc->RemoveDBConnVar( pDB->m_nDBKey);
		}
		else if( NODETYPE_DLL_LOAD == pData1->m_nType )
		{
			CDLLLoadObj *pDll = (CDLLLoadObj*)pData1;
			pDoc->RemoveDLLVar( pDll->m_nDLLKey );
		}
		else if( NODETYPE_EXCUTE_SQL == pData1->m_nType )
		{
			pDoc->RemoveRecordsetVar( pData1->m_nKey );
		}
		else if( NODETYPE_STOREPRO == pData1->m_nType )
		{
			pDoc->RemoveRecordsetVar( pData1->m_nKey );
		}
		else if(  NODETYPE_BLOCK == pData1->m_nType )
		{
			DeleteBlock( (CBlockObj *)pData1 );
		}
		else if( NODETYPE_RETURN == pData1->m_nType )
		{
			CNextNode *pNextObj = pObj->FindNextNodeObj(pData1->m_nKey);
			if( NULL != pNextObj )
			{
				pObj->RemoveNextNodeObj( pNextObj );
			}
		}

		pObj->m_ChildObjLst.RemoveAt(pos);
		delete pData1;
		pos = pObj->m_ChildObjLst.GetHeadPosition();
		
	}
	return;
}
///////////////////////////////////////
//从中删除指定的节点对象，如果是
//////////////////////////////////////
void CDrawView::DeleteMemberOfBlock(CNodeObj *pObj)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	//Find and remove
	int nKey = pObj->m_nKey;
	int nType = pObj->m_nType;
	POSITION pos = m_pBlockObj->m_ChildObjLst.Find(pObj);
	if (pos != NULL)
	{
		CNodeObj * pData1 = m_pBlockObj->m_ChildObjLst.GetAt(pos);
		if( NULL == pData1 ) return;

		if( NODETYPE_SETVARIABLE == pData1->m_nType )// 当是设置变量节点
		{
			pDoc->RemoveAllNormalVariable();
		}
		else if( NODETYPE_OPEN_DATABASE == pData1->m_nType )
		{
			CDBOpenObj *pDB = (CDBOpenObj*)pData1;
			pDoc->RemoveDBConnVar( pDB->m_nDBKey);
		}
		else if( NODETYPE_DLL_LOAD == pData1->m_nType )
		{
			CDLLLoadObj *pDll = (CDLLLoadObj*)pData1;
			pDoc->RemoveDLLVar( pDll->m_nDLLKey );
		}
		else if( NODETYPE_EXCUTE_SQL == pData1->m_nType )
		{
			pDoc->RemoveRecordsetVar( pData1->m_nKey );
		}
		else if( NODETYPE_STOREPRO == pData1->m_nType )
		{
			pDoc->RemoveRecordsetVar( pData1->m_nKey );
		}
		else if(  NODETYPE_BLOCK == pData1->m_nType )
		{
			DeleteBlock( (CBlockObj *)pData1 );
		}
		m_pBlockObj->m_ChildObjLst.RemoveAt(pos);
		delete pData1;

		
	}
	if( NODETYPE_RETURN == nType )
	{
		CNextNode *pNextObj = m_pBlockObj->FindNextNodeObj(nKey);
		if( NULL != pNextObj )
		{
			m_pBlockObj->RemoveNextNodeObj( pNextObj );
		}
	}
	

	return;
}

void CDrawView::DeleteMemberOfEndCallBlock(CNodeObj *pObj)
{
	//Find and remove
	int nKey = pObj->m_nKey;
	int nType = pObj->m_nType;
	POSITION pos = m_pEndCallBlockObj->m_ChildObjLst.Find(pObj);
	if (pos != NULL)
	{
		CNodeObj * pData1 = m_pEndCallBlockObj->m_ChildObjLst.GetAt(pos);
		m_pEndCallBlockObj->m_ChildObjLst.RemoveAt(pos);
		delete pData1;
	}


	return;
}

void CDrawView::DisConnectToMeBlock( int nAimKey )
{
	
	POSITION pos = m_pBlockObj->m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj * pTempObj =m_pBlockObj->m_ChildObjLst.GetNext(pos);
		if( NULL == pTempObj )
			continue;
		pTempObj->DisConnectToNode( nAimKey);
	}

	return;
}

void CDrawView::DisConnectToMeEndCallBlock(int nAimKey )
{

	POSITION pos = m_pEndCallBlockObj->m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj * pTempObj =m_pEndCallBlockObj->m_ChildObjLst.GetNext(pos);
		if( NULL == pTempObj )
			continue;
		pTempObj->DisConnectToNode( nAimKey);
	}

	return;
}

void CDrawView::OnEditDelete() 
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	POSITION pos = m_selection.GetHeadPosition();
	while( pos )
	{
		pos =m_selection.GetTailPosition( ) ;
		if( NULL == pos )
			break;
		CNodeObj * pObj =m_selection.GetAt(pos);
		if( NULL == pObj )
			break;
		if( 1 == m_nShowMode  )
		{
			if( NODETYPE_RETURN == pObj->m_nType ){
				int nCount = m_pBlockObj->m_NextNodeList.GetCount();
				if( nCount < 2 ) return;
			}
			else if( NODETYPE_ENTER == pObj->m_nType ){
				return;
			}
				
		}
		else if( 2 == m_nShowMode  )
		{
			if( NODETYPE_ENDCALLEVENT_CHECK == pObj->m_nType ){
				int nCount = m_pEndCallBlockObj->m_NextNodeList.GetCount();
				if( nCount < 2 ) return;
			}
			else if( NODETYPE_ENDCALLEVENT_END == pObj->m_nType ){
				return;
			}
			
		}

		g_pNodeTreeFormView->DeleteItem( pObj->m_nKey );
		if( NODETYPE_SETVARIABLE == pObj->m_nType )// 当是设置变量节点
		{
			pDoc->RemoveAllNormalVariable();
		}
		else if( NODETYPE_OPEN_DATABASE == pObj->m_nType )
		{
			CDBOpenObj *pDB = (CDBOpenObj*)pObj;
			pDoc->RemoveDBConnVar( pDB->m_nDBKey);
		}
		else if( NODETYPE_DLL_LOAD == pObj->m_nType )
		{
			CDLLLoadObj *pDll = (CDLLLoadObj*)pObj;
			pDoc->RemoveDLLVar( pDll->m_nDLLKey );
		}
		else if( NODETYPE_EXCUTE_SQL == pObj->m_nType )
		{
			pDoc->RemoveRecordsetVar( pObj->m_nKey );
		}
		else if( NODETYPE_STOREPRO == pObj->m_nType )
		{
			pDoc->RemoveRecordsetVar( pObj->m_nKey );
		}

		
		if( 0 == m_nShowMode )
		{
			pDoc->DisConnectToMe( pObj );
			if( NODETYPE_BLOCK == pObj->m_nType )
			{
				DeleteBlock((CBlockObj*)pObj );
			}
			m_selection.RemoveAt( pos );
			pDoc->Remove( pObj );
		}
		else if( 1 == m_nShowMode )
		{
			if( NODETYPE_BLOCK == pObj->m_nType )
			{
				pDoc->DisConnectToMeBlock( (CBlockObj *)pObj );
			}
			else
			{
				pDoc->DisConnectToMe( pObj );
			}
			DisConnectToMeBlock(pObj->m_nKey);
			m_selection.RemoveAt( pos );
			DeleteMemberOfBlock(pObj);

		}
		else if( 2 == m_nShowMode )
		{
			pDoc->DisConnectToMe( pObj );
			DisConnectToMeEndCallBlock(pObj->m_nKey);
			m_selection.RemoveAt( pos );
			DeleteMemberOfEndCallBlock(pObj);

		}
	}



	Invalidate();

	pDoc->SetModifiedFlag();
	m_bCopyObj = TRUE;
}


void CDrawView::OnUpdateEditDelete(CCmdUI* pCmdUI) 
{
	if( m_selection.GetCount() > 0 )
		m_bObjSelected = TRUE;
	else
		m_bObjSelected = FALSE;
	
	pCmdUI->Enable( m_bObjSelected );
	
	
}



void CDrawView::SaveFileRemakToDoc(CString strRemark)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_strFileRemark = strRemark;

	return;
}

void CDrawView::CopyObj(CNodeObj *pObj)
{
	if(  NODETYPE_ENDCALLEVENT_CHECK == pObj->m_nType )
	{
		return;
	}
	if(  NODETYPE_ENDCALLEVENT_END == pObj->m_nType )
	{
		return;
	}
	else if( NODETYPE_START == pObj->m_nType )
	{
		return;
	}
	else if( NODETYPE_MOUDLE_BEGIN == pObj->m_nType )
	{
		return;
	}
	else if(  NODETYPE_WAITCALL == pObj->m_nType )
	{
		CWaitCallObj * pNewObj = new CWaitCallObj;
		*pNewObj = *(CWaitCallObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_START_PLAY == pObj->m_nType )
	{
		CPlayStartObj * pNewObj = new CPlayStartObj;
		*pNewObj = *(CPlayStartObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_CHECK_DIAL == pObj->m_nType )
	{
		CDialCheckObj * pNewObj = new CDialCheckObj;
		*pNewObj = *(CDialCheckObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_RELEASECALL == pObj->m_nType )
	{
		CReleaseCallObj * pNewObj = new CReleaseCallObj;
		*pNewObj = *(CReleaseCallObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_CALLHOLD == pObj->m_nType )
	{
		CCallHoldObj * pNewObj = new CCallHoldObj;
		*pNewObj = *(CCallHoldObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_CHECK_TRANSFER == pObj->m_nType )
	{
		CTransferCheckObj * pNewObj = new CTransferCheckObj;
		*pNewObj = *(CTransferCheckObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_COMPARE == pObj->m_nType )
	{
		CCompareObj * pNewObj = new CCompareObj;
		*pNewObj = *(CCompareObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_START_RECORD == pObj->m_nType )
	{
		CRecordObj * pNewObj = new CRecordObj;
		*pNewObj = *(CRecordObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);

	}
	else if(  NODETYPE_ENTER_CONFERENCE == pObj->m_nType )
	{
		CEnterConferObj * pNewObj = new CEnterConferObj;
		*pNewObj = *(CEnterConferObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_LEAVE_CONFERENCE == pObj->m_nType )
	{
		CLeaveConferObj * pNewObj = new CLeaveConferObj;
		*pNewObj = *(CLeaveConferObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_OPEN_DATABASE == pObj->m_nType )
	{
		CDBOpenObj * pNewObj = new CDBOpenObj;
		*pNewObj = *(CDBOpenObj *)pObj;
		if( 0 == pNewObj->m_nDBType )
		{
			m_selectionCopied.AddTail(pNewObj);
		}
		else
		{
			delete pNewObj;
			pNewObj = NULL;
		}
		
	}
	else if(  NODETYPE_DATABASE_BROWSE == pObj->m_nType )
	{
		CDBBrowseObj * pNewObj = new CDBBrowseObj;
		*pNewObj = *(CDBBrowseObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_CLOSE_DATABASE == pObj->m_nType )
	{
		CDBCloseObj * pNewObj = new CDBCloseObj;
		*pNewObj = *(CDBCloseObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_EXCUTE_SQL == pObj->m_nType )
	{
		//CSQLExcuteObj * pNewObj = new CSQLExcuteObj;
		//*pNewObj = *(CSQLExcuteObj *)pObj;
		//m_selectionCopied.AddTail(pNewObj);
		return;
	}
	else if(  NODETYPE_DLL_LOAD == pObj->m_nType )
	{
		CDLLLoadObj * pNewObj = new CDLLLoadObj;
		*pNewObj = *(CDLLLoadObj *)pObj;
		if( 0 == pNewObj->m_nHandleType )
		{
			m_selectionCopied.AddTail(pNewObj);
		}
		else
		{
			delete pNewObj;
			pNewObj = NULL;
		}
	}
	else if(  NODETYPE_DLL_CALL == pObj->m_nType )
	{
		CDLLCallObj * pNewObj = new CDLLCallObj;
		*pNewObj = *(CDLLCallObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_DLL_UNLOAD == pObj->m_nType )
	{
		CDLLUnLoadObj * pNewObj = new CDLLUnLoadObj;
		*pNewObj = *(CDLLUnLoadObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_SEND_DTMFCODE == pObj->m_nType )
	{
		CDTMFSendObj * pNewObj = new CDTMFSendObj;
		*pNewObj = *(CDTMFSendObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_RECIVE_DTMF == pObj->m_nType )
	{
		CDTMFReciveObj * pNewObj = new CDTMFReciveObj;
		*pNewObj = *(CDTMFReciveObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_EVENT_CHECK == pObj->m_nType )
	{
		CEventCheckObj * pNewObj = new CEventCheckObj;
		*pNewObj = *(CEventCheckObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if( NODETYPE_DTMF_MENU == pObj->m_nType )
	{
		CDTMFMenuObj * pNewObj = new CDTMFMenuObj;
		*pNewObj = *(CDTMFMenuObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if( NODETYPE_CALLLOGREQUEST == pObj->m_nType )
	{
		CCallLogRequestObj * pNewObj = new CCallLogRequestObj;
		*pNewObj = *(CCallLogRequestObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_SWITCH == pObj->m_nType )
	{
		CSwitchObj * pNewObj =  new CSwitchObj;
		*pNewObj = *(CSwitchObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_FAXRECIVE == pObj->m_nType )
	{
		CFaxReciveObj * pNewObj =  new CFaxReciveObj;
		*pNewObj = *(CFaxReciveObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_FAXSEND == pObj->m_nType )
	{
		CFaxSendObj * pNewObj =  new CFaxSendObj;
		*pNewObj = *(CFaxSendObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_CONFERENCE_BUILD == pObj->m_nType )
	{
		CConferenceBulidObj * pNewObj =  new CConferenceBulidObj;
		*pNewObj = *(CConferenceBulidObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_CONFERENCE_DELETE == pObj->m_nType )
	{
		CConferenceDeleteObj * pNewObj =  new CConferenceDeleteObj;
		*pNewObj = *(CConferenceDeleteObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_GET_CONFERENCE_MEMBER == pObj->m_nType )
	{
		CGetConferMemberObj * pNewObj =  new CGetConferMemberObj;
		*pNewObj = *(CGetConferMemberObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if(  NODETYPE_CONFERENCE_REQUEST == pObj->m_nType )
	{
		CConferRequestObj * pNewObj =  new CConferRequestObj;
		*pNewObj = *(CConferRequestObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}

	else if( NODETYPE_BLOCK == pObj->m_nType )
	{

		CBlockObj * pNewObj =  new CBlockObj;
		*pNewObj = *(CBlockObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);

		return;

	}
	else if( NODETYPE_STOREPRO == pObj->m_nType )
	{
		//CStoreProObj * pNewObj =  new CStoreProObj;
		//*pNewObj = *(CStoreProObj *)pObj;
		//m_selectionCopied.AddTail(pNewObj);
		return;
	}
	else if( NODETYPE_CHANGEVOICE == pObj->m_nType )
	{
		CChangeVoiceObj * pNewObj =  new CChangeVoiceObj;
		*pNewObj = *(CChangeVoiceObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if( NODETYPE_EXPRESS == pObj->m_nType )
	{
		CExpressObj * pNewObj =  new CExpressObj;
		*pNewObj = *(CExpressObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if( NODETYPE_FUNCTION == pObj->m_nType )
	{
		CFunctionObj * pNewObj =  new CFunctionObj;
		*pNewObj = *(CFunctionObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if( NODETYPE_TRACE_INFO == pObj->m_nType )
	{
		CTraceObj * pNewObj =  new CTraceObj;
		*pNewObj = *(CTraceObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if( NODETYPE_MOUDLE == pObj->m_nType )
	{
		CModuleObj * pNewObj =  new CModuleObj;
		*pNewObj = *(CModuleObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if( NODETYPE_CUSTOM == pObj->m_nType )
	{
		CCustomObj * pNewObj =  new CCustomObj;
		*pNewObj = *(CCustomObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if( NODETYPE_CC_CALLAGENT == pObj->m_nType )
	{
		CCCCallAgentObj * pNewObj =  new CCCCallAgentObj;
		*pNewObj = *(CCCCallAgentObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else if( NODETYPE_CC_TRANSFER == pObj->m_nType )
	{

		CCCTransferObj * pNewObj =  new CCCTransferObj;
		*pNewObj = *(CCCTransferObj *)pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	else 
	{
		CNodeObj * pNewObj =  new CNodeObj;
		*pNewObj = *pObj;
		m_selectionCopied.AddTail(pNewObj);
	}
	return;
}

void CDrawView::OnNodeEnd() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_NODE_END;
	m_nObjAction = ACTION_ADD_OBJ;	
	
}

void CDrawView::DrawSwitchNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CSwitchObj * pObj =  new CSwitchObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_SWITCH );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("Switch%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_SWITCH;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::OnSwitch() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_SWITCH;
	m_nObjAction = ACTION_ADD_OBJ;	
}

void CDrawView::EditSwitchProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Switch properties",this,0);
	CSwitchPageSpc    specificPage;
	CGeneralPage		generalPage;

	generalPage.InitPage( m_pNodeObjSelected ,pDoc );
	specificPage.InitPage( ( CSwitchObj * )m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if( sheet.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}

	return;
}

void CDrawView::OnAnswer() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_ANSWERCALL;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::DrawAnswerNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CNodeObj * pObj =  new CNodeObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_ANSWER );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("AnswerCall%d",m_nBaseKey);

	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_ANSWER;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );

	m_nType = 0;

	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::EditAnswerCallProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("AnswerCall properties",this,0);
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );

	sheet.DoModal();
}

void CDrawView::EditNodeEndProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("ModuleEnd properties",this,0);
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );

	sheet.DoModal();
}

void CDrawView::EditModuleEndProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("NodeEnd properties",this,0);
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );

	sheet.DoModal();
}


//当鼠标移动时改变节点连线的颜色
void CDrawView::ChangeLineColor(CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->ChangeLineColor( point, this);
	
	return;
	
}
//必须重载这个消息才能正常
BOOL CDrawView::PreTranslateMessage(MSG* pMsg) 
{
	m_ToolTip.RelayEvent(pMsg);
	return CScrollView::PreTranslateMessage(pMsg);
}
/////////////////////////////////////////
//当鼠标停留在节点上时，显示浮动的提示框
//////////////////////////////////////////
void CDrawView::ShowNodeObjTipText(CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	CNodeObj * pObj = pDoc->HitTestNodeObj(  point, this  );
	if( NULL != pObj )
	{
			CNextNode * pNextNodeObj = pDoc->HitTestNextNodeObj( point,this );
			if( NULL != pNextNodeObj )
			{
				CRect rect = pNextNodeObj->m_rect;
				DocToClient( rect);
				m_ToolTip.SetToolRect( this, 1, &rect );
				m_ToolTip.UpdateTipText( pNextNodeObj->m_strNodeObjName, this, 1 );

			}
			else if( NODETYPE_GOTO == pObj->m_nType )
			{
				CGoToObj *pGoToObj = (CGoToObj*)pObj;
				CRect rect = pObj->m_rect;
				DocToClient( rect);
				m_ToolTip.SetToolRect( this, 1, &rect );
				m_ToolTip.UpdateTipText( pGoToObj->m_strConnPath, this, 1 );
			}
			else
			{
				CRect rect = pObj->m_rect;
				DocToClient( rect);
				m_ToolTip.SetToolRect( this, 1, &rect );
				m_ToolTip.UpdateTipText( pObj->m_strDescription, this, 1 );
			}


	}
	else
	{
		m_ToolTip.Pop();
	}

}

void CDrawView::SaveBuildScriptFile(CString strFile)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	CCFDocument NewDoc;
	NewDoc.OnNewDocument();
	
	AssignsObjList( pDoc->m_objects, &(NewDoc.m_objects),1 );
	
	POSITION pos = pDoc->m_DBConnVarList.GetHeadPosition();
	while (pos)
	{
		CDBConnVar * pNewObj = new CDBConnVar;
		*pNewObj = *( pDoc->m_DBConnVarList.GetNext(pos) );
		NewDoc.m_DBConnVarList.AddTail(pNewObj);
	}

	pos = pDoc->m_DLLVariableList.GetHeadPosition();
	while (pos)
	{
		CDLLVariable * pNewObj = new CDLLVariable;
		*pNewObj = *( pDoc->m_DLLVariableList.GetNext(pos) );
		NewDoc.m_DLLVariableList.AddTail(pNewObj);
	}

	pos = pDoc->m_NormalVarList.GetHeadPosition();
	while (pos)
	{
		CNormalVar * pNewObj = new CNormalVar;
		*pNewObj = *( pDoc->m_NormalVarList.GetNext(pos) );
		NewDoc.m_NormalVarList.AddTail(pNewObj);
	}

	pos = pDoc->m_RecordsetVariList.GetHeadPosition();
	while (pos)
	{
		CRecordsetVar * pNewObj = new CRecordsetVar;
		*pNewObj = *( pDoc->m_RecordsetVariList.GetNext(pos) );
		NewDoc.m_RecordsetVariList.AddTail(pNewObj);
	}

	
	pos = pDoc->m_ParameterLst.GetHeadPosition();
	while (pos)
	{
		CParameter * pNewObj = new CParameter;
		*pNewObj = *( pDoc->m_ParameterLst.GetNext(pos) );
		NewDoc.m_ParameterLst.AddTail(pNewObj);
	}

	NewDoc.m_nDocType = pDoc->m_nDocType;
	NewDoc.m_nBaseKey = pDoc->m_nBaseKey;
	NewDoc.m_nVarBaseKey = pDoc->m_nVarBaseKey;
	NewDoc.OnSaveDocument(strFile);

	return;
}


void CDrawView::OnFaxrecive() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_FAXRECIVE;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::OnFaxsend() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_FAXSEND;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::DrawFaxReciveNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CFaxReciveObj * pObj =  new CFaxReciveObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_FAXRECIVE );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("FaxRecive%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_FAXRECIVE;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawFaxSendNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CFaxSendObj * pObj =  new CFaxSendObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_FAXSEND );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("FaxSend%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_FAXSEND;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::EditFaxSendProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("FaxSend properties",this,0);
	CFaxSendPageSpc   specificPage;
	CGeneralPage   generalPage;


	generalPage.InitPage( m_pNodeObjSelected,pDoc);
	
	specificPage.InitPage( (CFaxSendObj*)m_pNodeObjSelected, pDoc);
	

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );
	
	if(sheet.DoModal() ==IDOK)
	{

	}
}

void CDrawView::EditFaxReciveProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("FaxRecive properties",this,0);
	CFaxRecivePageSpc   specificPage;
	CGeneralPage   generalPage;


	generalPage.InitPage( m_pNodeObjSelected,pDoc);
	
	specificPage.InitPage( (CFaxReciveObj*)m_pNodeObjSelected, pDoc);
	

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );
	
	if(sheet.DoModal() ==IDOK)
	{

	}
}

void CDrawView::OnBuildConference() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_CONFERENCE_BUILD;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::OnDeleteConference() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_CONFERENCE_DELETE;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::OnMemberConference() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_GET_CONFERENCE_MEMBER;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::DrawConferBuildNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CConferenceBulidObj * pObj =  new CConferenceBulidObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_CONFER_BUILD );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("BuildConference%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_CONFER_BUILD;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::EditBuildConferProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("BuildConference properties",this,0);
	CConferBuildPage    specificPage;
	CGeneralPage		generalPage;

	generalPage.InitPage( m_pNodeObjSelected ,pDoc );
	specificPage.InitPage( ( CConferenceBulidObj * )m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if( sheet.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}

	return;
}



void CDrawView::EditDeleteConferProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("DeleteConference properties",this,0);
	CConferDeletePage    specificPage;
	CGeneralPage		generalPage;

	generalPage.InitPage( m_pNodeObjSelected ,pDoc );
	specificPage.InitPage( ( CConferenceDeleteObj * )m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if( sheet.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}

	return;
}

void CDrawView::EditGetConferMemberProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Conference Info properties",this,0);
	CGetConferMemberPage    specificPage;
	CGeneralPage		generalPage;

	generalPage.InitPage( m_pNodeObjSelected ,pDoc );
	specificPage.InitPage( ( CGetConferMemberObj * )m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if( sheet.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}

	return;
}


void CDrawView::DrawConferRequestNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CConferRequestObj * pObj =  new CConferRequestObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_CONFER_REQUEST );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("ConferCall%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_CONFER_REQUEST;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::EditRequestConferProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("ConferenceCall properties",this,0);
	CConferRequestPage    specificPage;
	CGeneralPage		generalPage;

	generalPage.InitPage( m_pNodeObjSelected ,pDoc );
	specificPage.InitPage( ( CConferRequestObj * )m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if( sheet.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}

	return;
}

void CDrawView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar) {
	case VK_HOME:
		OnVScroll(SB_TOP, 0, NULL);
		OnHScroll(SB_LEFT, 0, NULL);
		break;
	case VK_END:
		OnVScroll(SB_BOTTOM, 0, NULL);
		OnHScroll(SB_RIGHT, 0, NULL);
		break;
	case VK_UP:
		OnVScroll(SB_LINEUP, 0, NULL);
		break;
	case VK_DOWN:
		OnVScroll(SB_LINEDOWN, 0, NULL);
		break;
	case VK_PRIOR:
		OnVScroll(SB_PAGEUP, 0, NULL);
		break;
	case VK_NEXT:
		OnVScroll(SB_PAGEDOWN, 0, NULL);
		break;
	case VK_LEFT:
		OnHScroll(SB_LINELEFT, 0, NULL);
		break;
	case VK_RIGHT:
		OnHScroll(SB_LINERIGHT, 0, NULL);
		break;
	default:
		break;
	}
}

void CDrawView::OnChildBlock() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_BLOCK;
	m_nObjAction = ACTION_ADD_OBJ;	
	
}
void CDrawView::OnGoto() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_GOTO;
	m_nObjAction = ACTION_ADD_OBJ;	
	
}
void CDrawView::OnReturn() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_RETURN;
	m_nObjAction = ACTION_ADD_OBJ;	
	
}

void CDrawView::OnRoutecall() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_ROUTE_CALL;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::DrawBlockNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CBlockObj * pObj =  new CBlockObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_BLOCK );
	
	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	
	CString str("");
	CString str2("");
	str.Format("Block%d",m_nBaseKey);
	str2.Format("Return%d",m_nBaseKey+2);
	pObj->m_nIcoID		= IDI_BLOCK;
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->AddNextNodeObj( str2, "", DATATYPE_INTEGER, m_nBaseKey + 2, 1 );
	DrawEnterNode(pObj, m_nBaseKey + 1 );
	DrawReturnNode(pObj, m_nBaseKey + 2 );
	



	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);

	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			//如果是CBlockObj的编辑状态下，
			//需要把新的CBlockObj加到当前CBlockObj
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			pObj->m_nBlockKey = m_pBlockObj->m_nKey;
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, m_nBaseKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	
	str2.Format("Enter%d",m_nBaseKey+1);
	g_pNodeTreeFormView->InsertBlockItem(pObj->m_nKey, m_nBaseKey + 1, str2, IDI_ENTER, IDI_ENTER);
	str2.Format("Return%d",m_nBaseKey+2);
	g_pNodeTreeFormView->InsertBlockItem(pObj->m_nKey, m_nBaseKey + 2, str2, IDI_RETURN, IDI_RETURN);

	m_nType = 0;
	pDoc->m_nBaseKey =m_nBaseKey+2;
	m_nBaseKey += 2;
	pDoc->SetModifiedFlag();
	return;

}

void CDrawView::DrawEnterNode(CBlockObj * pObj, int nKey )
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CPoint ptOrg;
	ptOrg.x = pDoc->GetSize().cx / 2;//取得文档的原点坐标
	ptOrg.y = pDoc->GetSize().cy / 2;

	CPoint point = CPoint(-ptOrg.x+20,ptOrg.y+20);
	CString str("");
	str.Format("Enter%d", nKey );
	CNodeObj * pBeginObj =  new CNodeObj(point);
	pBeginObj->m_nClassType = 1;
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_ENTER );
	pBeginObj->SetBaseInfo( str,hIcon,NODETYPE_ENTER,nKey);
	pBeginObj->m_nIcoID = IDI_ENTER;
	pBeginObj->m_nBlockKey = pObj->m_nKey;
	pBeginObj->AddNextNodeObj("Next",_T(""),DATATYPE_INTEGER,NEXTTYPE_NEXT,1);
	pObj->m_ChildObjLst.AddTail(pBeginObj);
	return;
}

void CDrawView::DrawReturnNode(CBlockObj * pObj, int nKey)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CPoint ptOrg;
	ptOrg.x = pDoc->GetSize().cx / 2;//取得文档的原点坐标
	ptOrg.y = pDoc->GetSize().cy / 2;

	CPoint point = CPoint(-ptOrg.x+300,ptOrg.y+20);
	CString str("");
	str.Format("Return%d", nKey );
	CNodeObj * pEndObj =  new CNodeObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_RETURN );
	pEndObj->SetBaseInfo( str ,hIcon,NODETYPE_RETURN,nKey);
	pEndObj->m_nIcoID = IDI_RETURN;
	pEndObj->m_nBlockKey = pObj->m_nKey;
	pObj->m_ChildObjLst.AddTail(pEndObj);

	return;
}


void CDrawView::DrawReturnNode(UINT nFlags, CPoint point)
{
	if( 2== m_nShowMode ) return;
	if( 0 == m_nShowMode || NULL == m_pBlockObj)
	{
		AfxMessageBox("The Return Node is only used in the Block !");
		return;
	}

	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	CNodeObj * pObj =  new CNodeObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_RETURN );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("Return%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_RETURN;
	pObj->m_nBlockKey = m_pBlockObj->m_nKey;

	m_nType = 0;
	m_pBlockObj->m_ChildObjLst.AddTail(pObj);
	int nNextCount =  m_pBlockObj->m_ChildObjLst.GetCount();
	m_pBlockObj->AddNextNodeObj( str, "", DATATYPE_INTEGER, m_nBaseKey, nNextCount+1 );
	g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);


	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}


void CDrawView::EditReturnProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Return properties",this,0);
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );

	if( sheet.DoModal() == IDOK )
	{
		//CBlockObj的CNextNode的类型值等于属于他成员的
		//类型为NODETYPE_RETURN的CNodeObj的Key值
		CNextNode *pNextObj = m_pBlockObj->FindNextNodeObj( m_pNodeObjSelected->m_nKey );
		if( NULL != pNextObj )
		{
			pNextObj->m_strName = m_pNodeObjSelected->m_strCaption;
		}


	}

	return;
}

void CDrawView::EditModuleBeginProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("ModuleBegin",this,0);
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );

	if( sheet.DoModal() == IDOK )
	{

	}

	return;
}

void CDrawView::EditEnterProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Enter properties",this,0);
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );

	if( sheet.DoModal() == IDOK )
	{

	}

	return;
}

void CDrawView::EditBlockProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Block properties",this,0);
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );

	if( sheet.DoModal() == IDOK )
	{

	}

	return;
}

void CDrawView::OnStoredproc() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_STOREPRO;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::DrawStorePro(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	CStoreProObj * pObj =  new CStoreProObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_STOREDPROC );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("StoredPro%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_STOREDPROC;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );
	pObj->m_nRecordsetKey = m_nBaseKey;
	pObj->m_nDBType = 0;


	CRecordsetVar *pRset =  new CRecordsetVar;
	pRset->m_nKey = m_nBaseKey;
	pRset->m_nVarDataType = DATATYPE_DBRECORDSET;
	pRset->m_strName.Format("Recordset%d",m_nBaseKey);
	pDoc->m_RecordsetVariList.AddTail( pRset );//将其保存
	g_pNodeTreeFormView->InsertItem(3,pRset->m_nKey+8000,pRset->m_strName, IDI_VARIABLE_DBREC , IDI_VARIABLE_DBREC );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}
	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::EditStoreProProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("StoredPro properties",this,0);
	CStoreProcPage   specificPage;
	CGeneralPage   generalPage;


	generalPage.InitPage( m_pNodeObjSelected,pDoc);
	specificPage.InitPage( (CStoreProObj * )m_pNodeObjSelected, pDoc);
	

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );
	
	if(sheet.DoModal() ==IDOK)
	{

	}

	return;
}

void CDrawView::OnChangeVoice() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_CHANGEVOICE;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::DrawChangeVoiceNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if( 2== m_nShowMode ) return;
	CChangeVoiceObj * pObj =  new CChangeVoiceObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_CHANGE_VOICE );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("ChangeVoice%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_CHANGE_VOICE;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}
void CDrawView::DrawRouteCallNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CRouteCallObj * pObj =  new CRouteCallObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_ROUTECALL );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("RouteCall%d",m_nBaseKey);

	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_ROUTECALL;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );

	m_nType = 0;

	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}
void CDrawView::EditChangeVoiceProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("ChangeVoice properties",this,0);
	CChangeVoicePage    specificPage;
	CGeneralPage		generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected, pDoc );
	specificPage.InitPage( (CChangeVoiceObj *)m_pNodeObjSelected, pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if( sheet.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}

	return;
}

void CDrawView::InsertEndCallBlockItem(CNodeObj *pObj)
{
	CEndCallBlockObj *pEndCalBlockObj = (CEndCallBlockObj*)pObj;
	POSITION pos = pEndCalBlockObj->m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj* pNewObj = pEndCalBlockObj->m_ChildObjLst.GetNext(pos);
		if( NULL != pNewObj )
		{
			
			g_pNodeTreeFormView->InsertEndCallBlockItem(pNewObj->m_nKey,
				pNewObj->m_strCaption,pNewObj->m_nIcoID, pNewObj->m_nIcoID );

		}
	}

	return;
}



void CDrawView::OnFunction() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_FUNCTION;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::OnExpress() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_EXPRESS;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::DrawFunctionNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	CFunctionObj * pObj =  new CFunctionObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_FUNCTION );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("Function%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_FUNCTION;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawExpressNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	CExpressObj * pObj =  new CExpressObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_EXPRESS );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("Express%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_EXPRESS;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::EditFunctionProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Function properties",this,0);
	CFunctionPage   specificPage;
	CGeneralPage   generalPage;


	generalPage.InitPage( m_pNodeObjSelected,pDoc);
	
	specificPage.InitPage( (CFunctionObj * )m_pNodeObjSelected, pDoc);
	

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );
	
	if(sheet.DoModal() ==IDOK)
	{

	}

	return;
}

void CDrawView::EditExpressProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Express properties",this,0);
	CExpressPage   specificPage;
	CGeneralPage   generalPage;


	generalPage.InitPage( m_pNodeObjSelected,pDoc);
	specificPage.InitPage( (CExpressObj * )m_pNodeObjSelected, pDoc);
	

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );
	
	if(sheet.DoModal() ==IDOK)
	{

	}

	return;
}

CNodeObj * CDrawView::FindNodeObj( int nKey, int &nParentKey )
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	return 	pDoc->FindNodeObj( nKey,nParentKey );
}

void CDrawView::OnEditBlockopen() 
{
	if( NULL == m_pNodeObjSelected ) return;
	if( NODETYPE_BLOCK == m_pNodeObjSelected->m_nType )
	{

		m_nShowMode = 1;
		m_pBlockObj = (CBlockObj *)m_pNodeObjSelected;

		SetScrollPos(SB_HORZ,0,TRUE);
		SetScrollPos(SB_VERT,0,TRUE);
		Invalidate(TRUE);
	}
	return;
}





void CDrawView::OnGotoConnect() 
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CGoToObj *pObj = (CGoToObj*)m_pNodeObjSelected;
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CGoToObj) ) )
	{
		return ;   
	}
	
	CConnectNodeDlg dlg;
	dlg.InitGoToDlg( pDoc, pObj ,this, m_nShowMode );
	if( dlg.DoModal() == IDOK )
	{
		//Invalidate(TRUE);
	}
	
}

void CDrawView::OnGotoDisconnect() 
{
	CGoToObj *pObj = (CGoToObj*)m_pNodeObjSelected;
    if (pObj == NULL || !pObj->IsKindOf( RUNTIME_CLASS(CGoToObj) ) )
	{
		return ;   
	}
	pObj->m_nNextObjKey = -1;
	pObj->m_strConnPath = pObj->m_strCaption + "->";
	return;
}

void CDrawView::OnGotoShowconn() 
{

	if( NULL == m_pNodeObjSelected ) return;
	if( NODETYPE_GOTO != m_pNodeObjSelected->m_nType ) return;
	CGoToObj *pGoToObj = (CGoToObj*)m_pNodeObjSelected;
	int nParentKey = -1;
	int nKey = pGoToObj->m_nNextObjKey;
	if( 0 >= nKey ) return;
	
	CNodeObj *pObj = FindNodeObj(nKey, nParentKey );
	CNodeObj *pParentNode = FindNodeObj(nParentKey);
	if( NULL == pObj ) return;
	if( NULL != pParentNode ){
		if(  NODETYPE_BLOCK == pParentNode->m_nType ){
			m_nShowMode = 1;
			m_pBlockObj = (CBlockObj *)pParentNode;
			SetScrollPos(SB_HORZ,0,TRUE);
			SetScrollPos(SB_VERT,0,TRUE);
			Invalidate(TRUE);
			ShowFindObjToSelected(pObj);
		}
		else if( NODETYPE_ENDCALL_BLOCK == pParentNode->m_nType ){
			
			m_nShowMode = 2;
			SetScrollPos(SB_HORZ,0,TRUE);
			SetScrollPos(SB_VERT,0,TRUE);
			Invalidate(TRUE);
			ShowFindObjToSelected(pObj);
		}
		else{
			ShowFindObjToSelected(pObj);
			m_nShowMode = 0;
			m_pBlockObj = NULL;
		}
	}
	else{
		ShowFindObjToSelected(pObj);
		m_nShowMode = 0;
		m_pBlockObj = NULL;
	}
	 
	return;
}





void CDrawView::DeleteTreeNodeSelected()
{
	OnEditDelete();
}


void CDrawView::EditRouteCallProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("RouteCall properties",this,0);
	CRouteCallProPage    specificPage;
	CGeneralPage		generalPage;

	generalPage.InitPage( m_pNodeObjSelected ,pDoc );
	specificPage.InitPage( ( CRouteCallObj * )m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if( sheet.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}

	return;
}

void CDrawView::OnTraceInfo() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_TRACE_INFO;
	m_nObjAction = ACTION_ADD_OBJ;
	
}

void CDrawView::DrawTraceNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	CTraceObj * pObj =  new CTraceObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_TRACE_INFO );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("Trace%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_TRACE_INFO;
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 1 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::EditTraceProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Trace properties",this,0);
	CTraceProPage   specificPage;
	CGeneralPage   generalPage;


	generalPage.InitPage( m_pNodeObjSelected,pDoc);
	specificPage.InitPage( (CTraceObj * )m_pNodeObjSelected, pDoc);
	

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );
	
	if(sheet.DoModal() ==IDOK)
	{

	}

	return;
}

void CDrawView::OnModule() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_MOUDLE;
	m_nObjAction = ACTION_ADD_OBJ;
	

}

void CDrawView::DrawModuleNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);


	CModuleObj * pObj =  new CModuleObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_MODULE );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("Module%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_MODULE;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::EditModuleProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Module properties",this,0);
	CModuleProPage   specificPage;
	CGeneralPage   generalPage;


	generalPage.InitPage( m_pNodeObjSelected,pDoc);
	specificPage.InitPage( (CModuleObj * )m_pNodeObjSelected, pDoc);
	

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );
	
	if(sheet.DoModal() ==IDOK)
	{

	}

	return;
}

void CDrawView::OnSetParameter() 
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CSetParameterDlg dlg;
	dlg.Init(pDoc);
	dlg.DoModal() ;


	return;
}


void CDrawView::DrawModuleEndNode(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CNodeObj * pObj =  new CNodeObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_MOUDLE_END );
	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str("");
	str.Format("ModuleEnd%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_MOUDLE_END;
	pObj->m_nType= m_nType;


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}
	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::OnMoudleEnd() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_MOUDLE_END;
	m_nObjAction = ACTION_ADD_OBJ;	
	return;
}

void CDrawView::OnUpdateMoudleEnd(CCmdUI* pCmdUI) 
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	BOOL b=FALSE;
	if( 1 == pDoc->m_nDocType )
	{
		b=TRUE;
	}
	pCmdUI->Enable(b);
	
}

void CDrawView::SaveBuildModuleFile(CString strFile)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CModuleObj NewMdlObj;
	AssignsObjList( pDoc->m_objects,&(NewMdlObj.m_objects),0);

	POSITION pos = pDoc->m_DBConnVarList.GetHeadPosition();
	while (pos)
	{
		CDBConnVar * pNewObj = new CDBConnVar;
		*pNewObj = *( pDoc->m_DBConnVarList.GetNext(pos) );
		NewMdlObj.m_DBConnVarList.AddTail(pNewObj);
	}

	pos = pDoc->m_DLLVariableList.GetHeadPosition();
	while (pos)
	{
		CDLLVariable * pNewObj = new CDLLVariable;
		*pNewObj = *( pDoc->m_DLLVariableList.GetNext(pos) );
		NewMdlObj.m_DLLVariableList.AddTail(pNewObj);
	}

	pos = pDoc->m_NormalVarList.GetHeadPosition();
	while (pos)
	{
		CNormalVar * pNewObj = new CNormalVar;
		*pNewObj = *( pDoc->m_NormalVarList.GetNext(pos) );
		NewMdlObj.m_NormalVarList.AddTail(pNewObj);
	}

	pos = pDoc->m_RecordsetVariList.GetHeadPosition();
	while (pos)
	{
		CRecordsetVar * pNewObj = new CRecordsetVar;
		*pNewObj = *( pDoc->m_RecordsetVariList.GetNext(pos) );
		NewMdlObj.m_RecordsetVariList.AddTail(pNewObj);
	}


	pos = pDoc->m_ParameterLst.GetHeadPosition();
	while (pos)
	{
		CParameter * pNewObj = new CParameter;
		*pNewObj = *( pDoc->m_ParameterLst.GetNext(pos) );
		NewMdlObj.m_ParameterLst.AddTail(pNewObj);
	}

	
	CModuleDocument NewDoc;
	NewDoc.OnNewDocument();
	NewDoc.m_ModuleObj = NewMdlObj;
	NewDoc.OnSaveDocument(strFile);

	return;
}

BOOL CDrawView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{



	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CDrawView::DestroyWindow() 
{
	
	return CScrollView::DestroyWindow();
}



void CDrawView::OnUpdateModule(CCmdUI* pCmdUI) 
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	BOOL b=TRUE;
	if( 1 == pDoc->m_nDocType )
	{
		b=FALSE;
	}
	pCmdUI->Enable(b);
	
}



void CDrawView::OnCustomNode() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_CUSTOM;
	m_nObjAction = ACTION_ADD_OBJ;		
}

void CDrawView::EditCustomProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("Custom properties",this,0);
	CSelfDefinesPage    specificPage;
	CGeneralPage		generalPage;

	generalPage.InitPage( m_pNodeObjSelected ,pDoc );
	specificPage.InitPage( (CCustomObj*) m_pNodeObjSelected ,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	if( sheet.DoModal() == IDOK )
	{
		Invalidate(TRUE);
	}
	

	return;
}

//画自定义节点
void CDrawView::DrawCustomNew(UINT nFlags, CPoint point)
{

	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	int nType = m_nType;
	
	CCustomTemplateDoc doc;
	CString strFile =  g_strAppPath + "\\Template.nor";
	if( !doc.OnOpenDocument( strFile ) )
	{
		return;
	}
	CCustomObj * pObj = NULL;
	POSITION pos = doc.m_ObjLst.GetHeadPosition();
	while (pos != NULL)
	{
		CCustomObj  *pNewObj = (CCustomObj *)doc.m_ObjLst.GetNext(pos);
		if( NULL == pNewObj ) continue;
		if( m_nType == pNewObj->m_nKey )
		{
			pObj = new CCustomObj(point);
			*pObj = *pNewObj;
			break;

		}
		
	}//end while
	
	if( NULL == pObj ){
		return;
	}
	pObj->MoveTo(point);
	//pObj->RemoveNextNodeObj

	UINT  nIcoID = pObj->m_nIcoID;
	HICON hIcon = AfxGetApp( )->LoadIcon( nIcoID );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("%s%d",pObj->m_strCaption, m_nBaseKey);

	pObj->SetBaseInfo(str,hIcon,NODETYPE_CUSTOM,m_nBaseKey);
	//pObj->m_nIcoID = IDI_CUSTOM_NODE;


	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	
	return;

}

void CDrawView::DrawCustomNode(UINT nFlags, CPoint point)
{
	
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	
	CCustomObj * pObj =  new CCustomObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_CUSTOM_NODE );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("Custom%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_CUSTOM_NODE;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );

	

	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}
	else
	{
		if( NULL != m_pEndCallBlockObj )
		{
			m_pEndCallBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertEndCallBlockItem( pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}
	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	
	return;
}

BOOL CDrawView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if( 0 == nCode )
	{
		if (NULL == pHandlerInfo)
		{
			if( OnDrawCustomNew( nID ) )
			{
				return TRUE;
			}
		}
		else
			return true;
	}
	
	return CScrollView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CDrawView::OnEditSavetotemplate() 
{
	
	if( NULL == m_pNodeObjSelected )
		return;
	if( NODETYPE_CUSTOM == m_pNodeObjSelected->m_nType )
	{
		CAddToTemplateDlg dlg;
		dlg.m_nIco = GetImageIndex( m_pNodeObjSelected->m_nIcoID );
		dlg.m_strName = m_pNodeObjSelected->m_strCaption;
		dlg.m_strExpress = m_pNodeObjSelected->m_strDescription;
		if( IDOK == dlg.DoModal() )
		{

			CCustomTemplateDoc doc;
			CString strFile =  g_strAppPath + "\\Template.nor";
			if( !doc.OnOpenDocument( strFile ) )
			{
				return;
			}

			CCustomObj *pNewObj = new CCustomObj;
			*pNewObj = *(CCustomObj *)m_pNodeObjSelected;
			doc.m_nBaseKey += 1;
			pNewObj->m_nKey = doc.m_nBaseKey;
			pNewObj->m_strCaption = dlg.m_strName;
			pNewObj->m_nIcoID = GetImageIcoID( dlg.m_nIco );
			
			POSITION pos = pNewObj->m_NextNodeList.GetHeadPosition();
			while (pos != NULL)
			{
				CNextNode *pObj = pNewObj->m_NextNodeList.GetNext(pos);
				if( NULL != pObj ){
					pObj->DisConnectToNodeObj();
				}
			}

			doc.m_ObjLst.AddTail( pNewObj );
			doc.OnSaveDocument(strFile);
		

			CMainFrame *pMainFrm = (CMainFrame *)AfxGetApp( )->GetMainWnd();
			pMainFrm->ShowCustomerMenu();
		}
		else
		{
			return;
		}
	}
	
	AfxMessageBox("It is successful to add the custom node to the template!");
	return;
}

BOOL CDrawView::OnDrawCustomNew(UINT nID)
{
	BOOL bResult = FALSE;
	CCustomTemplateDoc doc;
	CString strFile =  g_strAppPath + "\\Template.nor";
	if( !doc.OnOpenDocument( strFile ) )
	{
		return FALSE;
	}
	
	POSITION pos = doc.m_ObjLst.GetHeadPosition();
	while (pos != NULL)
	{
		CCustomObj *pObj = (CCustomObj *)doc.m_ObjLst.GetNext(pos);
		if( NULL == pObj ) continue;
		if( nID == (UINT)pObj->m_nKey  )
		{
			::SetCursor(m_hDraw);
			m_nType = nID   ;
			m_nObjAction = ACTION_ADD_OBJ;
			bResult = TRUE;
			break;

		}
		
	}//end while
	
	return bResult;
}



void CDrawView::OnCustomManage() 
{
	CCustomManageDlg dlg;

	if( IDOK == dlg.DoModal() )
	{



	}

	return;
}

void CDrawView::OnCcCallagent() 
{
	::SetCursor(m_hDraw);
	m_nType = NODETYPE_CC_CALLAGENT;
	m_nObjAction = ACTION_ADD_OBJ;	
	
}



void CDrawView::EditCcCallAgentProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("CallAgent properties",this,0);
	CCcCallAgentPage   specificPage;
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected,pDoc );
	specificPage.InitPage( ( CCCCallAgentObj*)m_pNodeObjSelected,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	sheet.DoModal();

	return;
}

void CDrawView::EditCcTransferProperty()
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPropertySheet sheet("CCTransfer properties",this,0);
	CCcTransferPage   specificPage;
	CGeneralPage   generalPage;

	
	generalPage.InitPage( m_pNodeObjSelected,pDoc );
	specificPage.InitPage( ( CCCTransferObj*)m_pNodeObjSelected,pDoc );

	sheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	sheet.AddPage( &generalPage );
	sheet.AddPage( &specificPage );

	sheet.DoModal();

	return;
}

void CDrawView::DrawCCCallAgent(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CCCCallAgentObj * pObj =  new CCCCallAgentObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_CC_CALLAGENT );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("CallAgent%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_CC_CALLAGENT;
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 1 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 2 );



	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}

void CDrawView::DrawCCTransfer(UINT nFlags, CPoint point)
{
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if( 2== m_nShowMode ) return;
	CCCTransferObj * pObj =  new CCCTransferObj(point);
	HICON hIcon = AfxGetApp( )->LoadIcon( IDI_CC_TRANSFER );

	m_nBaseKey += 1 ;//基础NodeObj的Key增加1
	pDoc->m_nBaseKey =m_nBaseKey;
	CString str(_T(""));
	str.Format("CCTransfer%d",m_nBaseKey);
	pObj->SetBaseInfo(str,hIcon,m_nType,m_nBaseKey);
	pObj->m_nIcoID = IDI_CC_TRANSFER;
	pObj->AddNextNodeObj( "CancelTransfer", "", DATATYPE_INTEGER,NEXTTYPE_CANCEL_TRANSFER, 1 );
	pObj->AddNextNodeObj( "Error", "", DATATYPE_INTEGER, NEXTTYPE_ERROR, 2 );
	pObj->AddNextNodeObj( "Next", "", DATATYPE_INTEGER, NEXTTYPE_NEXT, 3 );



	m_nType = 0;
	if( 0 == m_nShowMode )
	{
		pDoc->m_objects.AddTail(pObj);
		g_pNodeTreeFormView->InsertItem(1, pObj->m_nKey,str, pObj->m_nIcoID , pObj->m_nIcoID );
	}
	else if( 1== m_nShowMode )
	{
		if( NULL != m_pBlockObj )
		{
			m_pBlockObj->m_ChildObjLst.AddTail(pObj);
			g_pNodeTreeFormView->InsertBlockItem(m_pBlockObj->m_nKey, pObj->m_nKey , str, pObj->m_nIcoID, pObj->m_nIcoID);
		}

	}

	CRect rect = pObj->m_rect;
	DocToClient( rect );
	InvalidateRect(rect, FALSE);
	pDoc->SetModifiedFlag();
	return;
}
