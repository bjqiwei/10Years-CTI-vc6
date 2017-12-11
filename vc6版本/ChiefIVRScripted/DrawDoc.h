// DrawDoc.h : interface of the CDrawDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWDOC_H__0C14A481_4E96_4961_8A4B_8E2BC35293BD__INCLUDED_)
#define AFX_DRAWDOC_H__0C14A481_4E96_4961_8A4B_8E2BC35293BD__INCLUDED_

#include "..//include//NodeObj.h"



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDrawView;

class CDrawDoc : public COleDocument
{
protected: // create from serialization only
	CDrawDoc();
	DECLARE_DYNCREATE(CDrawDoc)

// Attributes
public:
	/*************************************************************/
	//在系统创建脚本时，每个脚本文件都从在添加第一个NodeObj对象时开始记数，
	//该基数为100，每增加一个对象，该基数加一得到新的基数，
	//m_nKey = 该新的基数，用以标识该对象；所以在整个脚本文件里，所
	//有对象的该值是唯一的，同时把最后的基数存储到脚本文件中，当文件
	//从新打开编辑时，以该基数为基数递增，以此作为新增对象的m_nBaseKey值。
	//
	int     m_nBaseKey;
	/*************************************************************/
	int     m_nVarBaseKey;
	int     m_nParamBaseKey;
	BOOL m_bNewDocument;
	int m_nDocType ;//0 ief文件 1 nmf文件
// Operations
public:
	CNodeObjList* GetObjects() { return &m_objects; }
	const CSize& GetSize() const { return m_size; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	CNodeObj * FindNodeObj( int nKey );
	CNodeObj * FindNodeObj( LPCTSTR lpszName );
	void MoveLineObj( int nNodeObjKey, CRect EndRect );
	CNodeObj * FindNodeObjByKey( int nKey );
	void InvalidateRect( CDrawView *pView );
	CNextNode * HitTestNextNodeObj( CPoint point , CDrawView *pView );
	void RemoveObjects( void );
	CNodeObj * HitTestNodeObj(CPoint point, CDrawView *pView );
	void Remove(CNodeObj *pObj);
	CNodeObj * ObjectAt(const CPoint &point);
	void Draw(CDC *pDC, CDrawView *pView);
	void ComputePageSize( void );
	virtual ~CDrawDoc();
public:
	 int	GetDataType( CString strDataType ) ;
	 CString	GetDataTypeName( int nType ) ;
	void OnDrawEndCallBlock(CDC *pDC, CEndCallBlockObj*pObj );
	void CheckStorePro(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckChangeVoice(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckReturn(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckEnter(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckBlock(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckNodeObj_BlockMember( CBlockObj *pObj, int &nError, int &nWarning , HTREEITEM &hParent );
	BOOL CheckConnectToMe_BlockMember(int nMeKey, CBlockObj *pObj);
	void MoveLineBlockObj(int nNodeObjKey, CBlockObj *pBlockObj, CRect EndRect );
	void ChangeLineColor_Type(CPoint point, CDrawView *pView , CNodeObjList ObjList  );
	void BlockLoadIcon( CBlockObj * pObj  );
	void OnDrawBlock(CDC * pDC, CBlockObj *pObj );
	void CheckConferInvite(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckGetConferMember(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckDeleteConfer(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckBuildConfer(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckFaxRecive(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckFaxSend(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void ChangeLineInvalidate( COLORREF crLineColor,CPoint sbeginPoint, CRect sBeginRect, CRect EndRect,CDrawView *pView);
	void ChangeLineColor(CPoint point, CDrawView *pView );
	void CheckAnswerCall(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckSwitch(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckNodeEnd(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	bool CheckOtherVariableSet(CString strExpress, CString &strErrrDescript, int &nVarKey );
	void CheckCallLogRequest(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckDTMFMenu(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void BuildScriptFile();
	void CheckEventCheck( CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent );
	void CheckDTMFSend(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckDTMFRecive(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckDLLUnLoad(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckDLLCall(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckDLLLoad(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckSQLExcute(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	bool CheckDBRecordsetVariable(CString strExpress, CString &strErrrDescript, int &nVarKey);
	void CheckDBBrows(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent );
	bool CheckDBVariableSet(CString strExpress, CString &strErrrDescript, int &nVarKey );
	void CheckDBClose(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckDBOpen(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckCompare(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckSetVariable(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckConferLeave(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckConferEnter(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckRecordStart(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckTransferEnd(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckCallHold( CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent );
	void CheckReleaseCall(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckDialEnd(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	
	void CheckPlayStrat(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent );
	BOOL CheckConnectToMe( int nMeKey  );
	void CheckNextNode(CNodeObj *pObj, HTREEITEM &item, int &nError, int &nWarning, HTREEITEM &hParent );
	BOOL FindNormalVariable( int nKey,int *nVarType );

	bool CheckNormalVariableSet(CString strExpress, CString &strErrrDescript, int &nVarKey,int &nVarType );
	void CheckNodeObj( CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent );
	void CheckWaitCall( CNodeObj*  pObj , int &nError, int &nWarning, HTREEITEM &hParent );
	void CheckScriptFile( void );
	CString m_strFileRemark;
	CDLLVariable * FindDLLVariable( int nKey );
	void RemoveDLLVar(int nKey);
	void RemoveRecordsetVar(int nKey);
	CRecordsetVar * FindRecordsetVar( int nKey );
	BOOL GetVarFieldConstantsEtc( CWnd* pWnd , CString &str );
	void DisConnectToMe(  CNodeObj *pObj );
	BOOL JudgeNameValidity( CString strName ,int nKey );
	void RemoveDBConnVar( int nKey );
	CDBConnVar * FindDBConnectVariable(int nKey );
	bool HaveNodeObj( int nObjType );
	void RemoveAllNormalVariable(void);
	void Remove(CNormalVar *pObj);
	void Remove(CRecordsetVar *pObj);
	void Remove(CDBConnVar *pObj);
public:
	void CheckTrace(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);

	void DisConnectToMeBlock(CBlockObj *pObj );
	void DisConnectToMeEndCallBlock(CEndCallBlockObj *pObj, int nAimKey );
	void DisConnectToMeBlock(CBlockObj *pObj, int nAimKey);
	void UpDateNextObjName( CNodeObj *pObj );
	void CheckGoTo(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	CNodeObj * FindNodeObj( int nKey, int &nParentKey  );
	void CheckExpress(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckFunction(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckEndCallEventEnd(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckEndCallEventCheck(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	BOOL CheckConnectToMe_ECBlockMember(int nMeKey, CEndCallBlockObj *pObj  );
	void CheckNodeObj_EndCallBlockMember( CEndCallBlockObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void MoveLineEndCallBlockObj(int nNodeObjKey , CEndCallBlockObj *pBlockObj,CRect EndRect);
	void EndCallBlockLoadIcon( CEndCallBlockObj *pObj );
	void CheckMoudleBegin(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckOhterNode(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckModuleNode(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void RemoveAllParameter( void );
public:
	void CheckCCTransfer(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	void CheckCCCallAgent(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent);
	CDrawView * m_pView;
	CNodeObjList m_objects;
	CEndCallBlockObj *m_pEndCallBlockObj;
	CRecordsetVarList m_RecordsetVariList;
	CDBConnVarList m_DBConnVarList;
	CNormalVarList m_NormalVarList;
	CFDLLVariableList m_DLLVariableList;
	CParameterList m_ParameterLst;//模块（nmf）文件的参数列表
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	CSize m_size;
	//{{AFX_MSG(CDrawDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWDOC_H__0C14A481_4E96_4961_8A4B_8E2BC35293BD__INCLUDED_)
