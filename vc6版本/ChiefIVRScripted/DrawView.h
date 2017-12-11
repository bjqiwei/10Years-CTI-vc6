// DrawView.h : interface of the CDrawView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWVIEW_H__53C9369D_AE6F_483A_A82E_515EEA4FEAE8__INCLUDED_)
#define AFX_DRAWVIEW_H__53C9369D_AE6F_483A_A82E_515EEA4FEAE8__INCLUDED_



#include "ObjSelectTool.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define HINT_UPDATE_WINDOW      0
#define HINT_UPDATE_DRAWOBJ     1
#define HINT_UPDATE_SELECTION   2
#define HINT_DELETE_SELECTION   3
#define HINT_UPDATE_OLE_ITEMS   4

//对象操作常量
#define  ACTION_NO_OBJ			1//初始状态
#define  ACTION_ADD_OBJ			2//添加对象状态
#define  ACTION_SELECT_OBJ		3//对象选择状态
#define  ACTION_REMOVE_OBJ		4//删除对象状态
#define  ACTION_EDIT_OBJ		5//编辑对象状态
#define  ACTION_DRAW_LINE	    6//连线状态

//滚动方向常量
#define  SCROLL_CENTER			1//在中间
#define  SCROLL_DOWN			2//正下方
#define  SCROLL_RIGHT			3//正右方
#define  SCROLL_UP				4//正上方
#define  SCROLL_LEFT			5//正左方
#define  SCROLL_RIGHT_DOWN	    6//右下方
#define  SCROLL_RIGHT_UP	    7//右上方
#define  SCROLL_LEFT_DOWN	    8//左下方
#define  SCROLL_LEFT_UP			9//左上方
#define  SCROLL_AREA_OUT 		10//区域外


class CDrawItem;
class CDrawView : public CScrollView
{
public: // create from serialization only
	CDrawView();
protected: 
	DECLARE_DYNCREATE(CDrawView)

// Attributes
public:
	CDrawDoc* GetDocument();
	// m_pSelection holds the selection to the current CDrawItem.
	// For many applications, such a member variable isn't adequate to
	//  represent a selection, such as a multiple selection or a selection
	//  of objects that are not CDrawItem objects.  This selection
	//  mechanism is provided just to help you get started.

	// TODO: replace this selection mechanism with one appropriate to your app.
	//CDrawItem* m_pSelection;
public:
	BOOL m_bShowFocusNode;
	//用来判断是否对有焦点的节点，屏幕画圆提示
	//False不显示 TRUE显示
public:
	BOOL			m_bNextObjSelected;
	BOOL			m_bObjSelected;//是否有CNodeObj被选中
	long			m_Objwidth;//CNodeObj对象的宽度
	long			m_Objhigh;//CNodeObj对象的高度
	CNodeObjList	m_selection;
	CNodeObjList	m_selectionCopied;
public:
	int m_nShowMode;//显示内容模式，0：显示总体脚本节点；1：显示childblock的内容；2：显示EndCallBlock的内容
	CBlockObj *m_pBlockObj;//需要显示的Block对象
	CEndCallBlockObj *m_pEndCallBlockObj;
private:
	int m_nType;//添加对象的类型
	CToolTipCtrl m_ToolTip;

	CPoint			m_initialPoint;
	CPoint			m_LogicLastPoint;//鼠标最后按下时的逻辑坐标

	//拷贝时记录坐标
	CPoint          m_LogPointCopied;
	CPoint          m_LogPointInit;

	CNextNode * m_pNextNodeSelected;
	CNodeObj * m_pNodeObjSelected ;
	

	int m_nObjAction;
	BOOL m_bDown;//判断鼠标是否按下
	CObjSelectTool m_ObjSelectTool;
	CRect m_MoveSelectRect;
	

// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL DestroyWindow();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL IsSelected(const CObject* pDocItem) const;// Container support
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawCCTransfer(UINT nFlags, CPoint point);
	void DrawCCCallAgent(UINT nFlags, CPoint point);
	void EditCcTransferProperty( void );
	void EditCcCallAgentProperty( void );
	void DrawCustomNew(UINT nFlags, CPoint point);
	BOOL OnDrawCustomNew( UINT nID );
	void DrawCustomNode( UINT nFlags, CPoint point );
	void EditCustomProperty( void );
	DWORD m_dwID;
	void EditModuleBeginProperty();
	void SaveBuildModuleFile(CString strFile);
	void DrawModuleEndNode(UINT nFlags, CPoint point);
	void EditModuleProperty( void );
	void DrawModuleNode(UINT nFlags, CPoint point);
	void EditTraceProperty( void );
	void DrawTraceNode(UINT nFlags, CPoint point);
	void EditRouteCallProperty();
	void DrawRouteCallNode(UINT nFlags, CPoint point);
	void DeleteTreeNodeSelected( void );
	void DeleteBlock(CBlockObj *pObj);
	void DrawGoToNode(UINT nFlags, CPoint point);
	CNodeObj * FindNodeObj( int nKey, int &nParentKey);
	void EditExpressProperty( void );
	void EditFunctionProperty( void );
	void DrawExpressNode(UINT nFlags, CPoint point);
	void DrawFunctionNode(UINT nFlags, CPoint point);
	void ShowNodeObjTipText( CPoint point );

	void DisConnectToMeEndCallBlock( int nAimKey );
	void DeleteMemberOfEndCallBlock( CNodeObj  *pObj );
	void InsertEndCallBlockItem(CNodeObj *pObj);
	void EditChangeVoiceProperty();
	void DrawChangeVoiceNode(UINT nFlags, CPoint point);
	void EditStoreProProperty( void );
	void DrawStorePro(UINT nFlags, CPoint point);
	void EditBlockProperty( void );
	void EditEnterProperty( void );
	void DisConnectToMeBlock(int nAimKey);
	void DeleteMemberOfBlock( CNodeObj *pObj );
	void EditReturnProperty();
	void DrawReturnNode(UINT nFlags, CPoint point);
	void ShowFindObjToSelected( CNodeObj *pObj );
	void InsertBlockItem( CNodeObj *pObj );
	void DrawReturnNode(CBlockObj * pObj, int nKey );
	void DrawEnterNode(CBlockObj * pObj,  int nKey );
	void DrawBlockNode(UINT nFlags, CPoint point);
	void EditRequestConferProperty(void);
	void DrawConferRequestNode(UINT nFlags, CPoint point);
	void EditGetConferMemberProperty();
	void EditDeleteConferProperty();
	void DrawGetConferMemberNode(UINT nFlags, CPoint point);
	void DrawConferDeleteNode(UINT nFlags, CPoint point);
	void EditBuildConferProperty();
	void DrawConferBuildNode(UINT nFlags, CPoint point);
	void EditFaxReciveProperty();
	void EditFaxSendProperty();
	void DrawFaxSendNode(UINT nFlags, CPoint point);
	void DrawFaxReciveNode(UINT nFlags, CPoint point);
	void SaveBuildScriptFile( CString strFile );
	void ChangeLineColor( CPoint point );
	void EditNodeEndProperty();
	void EditModuleEndProperty();
	void EditAnswerCallProperty();
	void DrawAnswerNode(UINT nFlags, CPoint point);
	void EditSwitchProperty();
	void DrawSwitchNode(UINT nFlags, CPoint point);
	void EditCallLogRequestProperty();
	void DrawCallLogRequestNode(UINT nFlags, CPoint point);

	void CopyObj( CNodeObj * pObj );
	void EditDTMFMenuProperty();
	void DrawDTMFMenu(UINT nFlags, CPoint point);
	void SaveFileRemakToDoc( CString strRemark );
	void EditDTMFReciveProperty();
	void EditDTMFSendProperty();
	void EditDLLCallProperty();
	void EditDLLUnLoadProperty();
	void EditDLLLoadProperty();
	void EditDBBrowseProperty(void);
	void DrawDBBrowseNode(UINT nFlags, CPoint point);
	void EditSQLExcuteProperty();
	void EditDBCloseProperty();
	void EditConfLeaveNodeProperty();
	void EditConfEnterNodeProperty( void );
	void EditTransferCheckProperty();
	void EditEventCheckProperty();
	void DrawEventCheckNode(UINT nFlags, CPoint point);
	void EditDialCheckProperty();
	void DrawDTMFReciveNode(UINT nFlags, CPoint point);
	void DrawDTMFSendNode(UINT nFlags, CPoint point);
	void DrawDLLCallNode(UINT nFlags, CPoint point);
	void DrawDLLUnLoadNode(UINT nFlags, CPoint point);
	void DrawDLLLoadNode(UINT nFlags, CPoint point);
	void DrawNodeEnd(UINT nFlags, CPoint point);

	void DrawExcuteSQLNode(UINT nFlags, CPoint point);
	void DrawDataBaseCloseNode(UINT nFlags, CPoint point);
	void DrawDataBaseOpenNode(UINT nFlags, CPoint point);
	void DrawConferenceLeaveNode(UINT nFlags, CPoint point);
	void DrawConferenceEnterNode(UINT nFlags, CPoint point);
	void DrawTransferCheckNode(UINT nFlags, CPoint point);
	void DrawDialCheckNode(UINT nFlags, CPoint point);
	void EditCompareProperty();
	void EditRecordStartProperty();
	void EditCallHoldProperty();
	void EditReleaseCallProperty();
	void EditPlayStartNodeProperty();
	void EditDBOpenProperty();
	inline void FindNodeObj(LPCTSTR lpszName);
	CNodeObj *  FindNodeObj(int nKey);
	void InitNodeTreeView( void );


	void EditSetVariableProperty( void );
	CString GetChiefScriptFile( void );
	CString GetFolders( void );
	CString GetFoldersAndFile( void );
	void EditWaitCallNodeProperty( void );
	void EditNodeObjProperty( void );
	void DrawStartNode(UINT nFlags, CPoint point);
	void MouseMoveDrawLine(CPoint point, CDC *pDc );
	void ResetDragDraw( CPoint point );
	void ClearDragDrawRect(  );
	void StopScroll( void );
	int  BeginScroll( CPoint point );
	void SingleSelectNodeObj( CPoint local );
	void MouseUPDrawLine(CPoint point);
	void DocToClient(CPoint& point);
	void MouseMoveDrawLine( CPoint point );
	void AddNewNodeObj(UINT nFlags, CPoint point);
	void MouseUPNodeObj(CPoint point );
	void InvalObj( CNodeObj *pObj );
	void RemoveNodeObj(CNodeObj *pObj);
	void SelectWithinRect(CRect rect,BOOL bAdd );
	void Select(CNodeObj* pObj,BOOL bAdd  = FALSE );
	void TrackerHitTest( CPoint point );
	void DocToClient(CRect& rect);
	void ClientToDoc(CRect& rect);
	void MouseMoveNodeObjTo(CPoint point  );
	BOOL HitTestNodeObj(CPoint point,BOOL & bNextObjSelected);
	inline void ClientToDoc(CPoint& point);
	void DrawWaitCallNode(UINT nFlags, CPoint point);
	void DrawSetVariableNode(UINT nFlags, CPoint point);
	void DrawReleaseCallNode(UINT nFlags, CPoint point);
	void DrawRecordStartNode(UINT nFlags, CPoint point);
	void DrawPlayStartNode(UINT nFlags, CPoint point);
	void DrawCompareNode(UINT nFlags, CPoint point);
	void DrawCallHoldNode(UINT nFlags, CPoint point);
	void PasteReplaceNextKey( CNodeObjList *pNOlst,int nNodeKey , int nNewNodeKey );
	void PasteReplaceNext(CNodeObjList *pNOlst, CNodeObj *pObj );
	void PasteQueryReplaceNextKey( CNodeObjList *pNOlst,CNextNode *pNObj  );
	virtual ~CDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CRect m_rect;
	/*************************************************************/
	//在系统创建脚本时，每个脚本文件都从在添加第一个NodeObj对象时开始记数，
	//该基数为100，每增加一个对象，该基数加一得到新的基数，
	//m_nKey = 该新的基数，用以标识该对象；所以在整个脚本文件里，所
	//有对象的该值是唯一的，同时把最后的基数存储到脚本文件中，当文件
	//从新打开编辑时，以该基数为基数递增，以此作为新增对象的m_nBaseKey值。
	//
	int     m_nBaseKey;
	int     m_nVarBaseKey;
	/*************************************************************/
	


// Generated message map functions
protected:
	CPoint m_movePoint;
	int m_nScrollDirect;//滚动方向
	int m_nScrollType;//滚动类型
	CPoint m_OldScrollPoint;

	//{{AFX_MSG(CDrawView)
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnInsertObject();
	afx_msg void OnCancelEditCntr();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnCancelMode();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnEditDelete();
	afx_msg void OnEditProperties();
	afx_msg void OnEditRename();
	afx_msg void OnConnectConnect();
	afx_msg void OnConnectDisconnect();
	afx_msg void OnUpdateEditProperties(CCmdUI* pCmdUI);
	afx_msg void OnWaitcall();
	afx_msg void OnStartPlay();
	afx_msg void OnStartRecord();
	afx_msg void OnReleasecall();
	afx_msg void OnSetvariable();
	afx_msg void OnCompare();
	afx_msg void OnCallhold();
	afx_msg void OnNotselect();
	afx_msg void OnScriptBuild();
	afx_msg void OnScriptCheck();
	afx_msg void OnCheckTransfer();
	afx_msg void OnCheckDial();
	afx_msg void OnEnterConference();
	afx_msg void OnLeaveConference();
	afx_msg void OnOpendDatabase();
	afx_msg void OnCloseDatabase();
	afx_msg void OnExcuteSql();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDelete(CCmdUI* pCmdUI);
	afx_msg void OnDllCall();
	afx_msg void OnDllLoad();
	afx_msg void OnDllUnload();
	afx_msg void OnSendDtmfcode();
	afx_msg void OnReciveDtmf();
	afx_msg void OnEventCheck();
	afx_msg void OnDatabaseBrowse();
	afx_msg void OnUpdateScriptCheck(CCmdUI* pCmdUI);
	afx_msg void OnUpdateScriptBuild(CCmdUI* pCmdUI);
	afx_msg void OnDtmfMenu();
	afx_msg void OnCalllogrequest();
	afx_msg void OnNodeEnd();
	afx_msg void OnSwitch();
	afx_msg void OnAnswer();
	afx_msg void OnFaxrecive();
	afx_msg void OnFaxsend();
	afx_msg void OnBuildConference();
	afx_msg void OnDeleteConference();
	afx_msg void OnMemberConference();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChildBlock();
	afx_msg void OnReturn();
	afx_msg void OnStoredproc();
	afx_msg void OnChangeVoice();
	afx_msg void OnFunction();
	afx_msg void OnExpress();
	afx_msg void OnEditBlockopen();
	afx_msg void OnGoto();
	afx_msg void OnGotoConnect();
	afx_msg void OnGotoDisconnect();
	afx_msg void OnGotoShowconn();
	afx_msg void OnRoutecall();
	afx_msg void OnTraceInfo();
	afx_msg void OnModule();
	afx_msg void OnSetParameter();
	afx_msg void OnMoudleEnd();
	afx_msg void OnUpdateMoudleEnd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateModule(CCmdUI* pCmdUI);
	afx_msg void OnCustomNode();
	afx_msg void OnEditSavetotemplate();
	afx_msg void OnCustomManage();
	afx_msg void OnCcCallagent();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bCopyObj;
	BOOL m_bPasteObj;
	BOOL m_bViewInit;
	BOOL m_bBeginMove;
	HCURSOR m_hArrow;
	HCURSOR m_hDraw;
	HCURSOR m_hDrag;
	HCURSOR m_hStopDrag;

	CPoint m_LButtonUpPoint;
	CPoint m_LButtonDownPoint;
	

};

#ifndef _DEBUG  // debug version in DrawView.cpp
inline CDrawDoc* CDrawView::GetDocument()
   { return (CDrawDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWVIEW_H__53C9369D_AE6F_483A_A82E_515EEA4FEAE8__INCLUDED_)
