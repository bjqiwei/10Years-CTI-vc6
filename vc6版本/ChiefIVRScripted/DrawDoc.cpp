// DrawDoc.cpp : implementation of the CDrawDoc class
//

#include "stdafx.h"
#include "ChiefIVRScripted.h"
#include "SelectVariablesDlg.h"
#include "SelectConstantVarDld.h"
#include "NodeTreeFormView.h"
#include "CheckBuildFormView.h"
#include "ExpressionDlg.h"

#include "DrawDoc.h"
#include "CntrItem.h"
#include "DrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CNodeTreeFormView *g_pNodeTreeFormView ;
extern CCheckBuildFormView *g_pCheckBuildForm ;


/////////////////////////////////////////////////////////////////////////////
// CDrawDoc

IMPLEMENT_DYNCREATE(CDrawDoc, COleDocument)

BEGIN_MESSAGE_MAP(CDrawDoc, COleDocument)
	//{{AFX_MSG_MAP(CDrawDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, COleDocument::OnUpdatePasteMenu)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, COleDocument::OnUpdatePasteLinkMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_CONVERT, COleDocument::OnUpdateObjectVerbMenu)
	ON_COMMAND(ID_OLE_EDIT_CONVERT, COleDocument::OnEditConvert)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, COleDocument::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, COleDocument::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, COleDocument::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawDoc construction/destruction

CDrawDoc::CDrawDoc()
{
	m_pView = NULL;
	m_nDocType = 0;
	m_nBaseKey = 100;
	m_nVarBaseKey = 100;
	m_nParamBaseKey = 4000;//模块文件的参数的起始Key为4000
	m_bNewDocument = FALSE;
	m_size = CSize(0,0);
	m_strFileRemark = "";
	m_pEndCallBlockObj = NULL;

	EnableCompoundFile();

	ComputePageSize();
	
}

CDrawDoc::~CDrawDoc()
{
	RemoveObjects();
}

BOOL CDrawDoc::OnNewDocument()
{

	if (!COleDocument::OnNewDocument())
		return FALSE;
	CPoint ptOrg;
	ptOrg.x = GetSize().cx / 2;//取得文档的原点坐标
	ptOrg.y = GetSize().cy / 2;
	if( 0 == m_nDocType )
	{
		CPoint point = CPoint(-ptOrg.x+20,ptOrg.y+20);

		CNodeObj * pObj =  new CNodeObj(point);
		HICON hIcon = AfxGetApp( )->LoadIcon( IDI_START );

		m_nBaseKey += 1 ;//基础NodeObj的Key增加1

		pObj->SetBaseInfo("Start",hIcon,NODETYPE_START,m_nBaseKey);
		pObj->m_nIcoID = IDI_START;
		pObj->AddNextNodeObj("Next",_T(""),DATATYPE_INTEGER,NEXTTYPE_NEXT,1);
		m_objects.AddTail(pObj);
		
		/////////////////////////////////////////////////////
		CEndCallBlockObj *pECBObj = new CEndCallBlockObj(point);
		m_nBaseKey += 1;
		pECBObj->m_nKey = m_nBaseKey;
		m_objects.AddTail(pECBObj);
		m_pEndCallBlockObj = pECBObj;

		CNodeObj * pECCObj =  new CNodeObj(point);
		m_nBaseKey += 1;
		hIcon = AfxGetApp( )->LoadIcon( IDI_ENDCALLEVENT_CHECK );
		pECCObj->SetBaseInfo("EndCallCheck",hIcon,NODETYPE_ENDCALLEVENT_CHECK,m_nBaseKey);
		pECCObj->m_nIcoID = IDI_ENDCALLEVENT_CHECK;
		pECCObj->AddNextNodeObj("Next",_T(""),DATATYPE_INTEGER,NEXTTYPE_NEXT,1);
		pECBObj->m_ChildObjLst.AddTail(pECCObj);

		point.x += 200;

		CNodeObj * pECEObj =  new CNodeObj(point);
		m_nBaseKey += 1;
		hIcon = AfxGetApp( )->LoadIcon( IDI_ENDCALLEVENT_END );
		pECEObj->SetBaseInfo("EndCallEnd",hIcon,NODETYPE_ENDCALLEVENT_END,m_nBaseKey);
		pECEObj->m_nIcoID = IDI_ENDCALLEVENT_END;
		pECBObj->m_ChildObjLst.AddTail(pECEObj);
	}
	else
	{
		CPoint point = CPoint(-ptOrg.x+20,ptOrg.y+20);

		CNodeObj * pObj =  new CNodeObj(point);
		HICON hIcon = AfxGetApp( )->LoadIcon( IDI_MOUDLE_BEGIN );

		m_nBaseKey += 1 ;//基础NodeObj的Key增加1

		pObj->SetBaseInfo("ModuleBegin",hIcon,NODETYPE_MOUDLE_BEGIN,m_nBaseKey);
		pObj->m_nIcoID = IDI_MOUDLE_BEGIN;
		pObj->AddNextNodeObj("Next",_T(""),DATATYPE_INTEGER,NEXTTYPE_NEXT,1);
		m_objects.AddTail(pObj);
		
		/////////////////////////////////////////////////////

		point.x += 200;

		CNodeObj * pECEObj =  new CNodeObj(point);
		m_nBaseKey += 1;
		hIcon = AfxGetApp( )->LoadIcon( IDI_MOUDLE_END );
		pECEObj->SetBaseInfo("ModuleEnd",hIcon,NODETYPE_MOUDLE_END,m_nBaseKey);
		pECEObj->m_nIcoID = IDI_MOUDLE_END;
		m_objects.AddTail(pECEObj);
	}


	SetModifiedFlag();
	
	m_bNewDocument = TRUE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDrawDoc serialization

void CDrawDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		m_objects.Serialize(ar);
		m_RecordsetVariList.Serialize(ar);
		m_DBConnVarList.Serialize(ar);
		m_NormalVarList.Serialize(ar);
		m_DLLVariableList.Serialize(ar);
		ar << m_nBaseKey;
		ar << m_nVarBaseKey;
		ar << m_strFileRemark;
		if( 1 == m_nDocType ) 
		{
			m_ParameterLst.Serialize(ar);
			ar << m_nParamBaseKey;
		}
	}
	else
	{
		m_objects.Serialize(ar);
		m_RecordsetVariList.Serialize(ar);
		m_DBConnVarList.Serialize(ar);
		m_NormalVarList.Serialize(ar);
		m_DLLVariableList.Serialize(ar);
		ar >> m_nBaseKey;
		ar >> m_nVarBaseKey;
		ar >> m_strFileRemark;

		POSITION pos = m_objects.GetTailPosition();
		while (pos)
		{
			CNodeObj* pObj = m_objects.GetPrev(pos);
			if( NULL != pObj )
			{
				pObj->m_hIcon = AfxGetApp( )->LoadIcon( pObj->m_nIcoID );
				if(  NODETYPE_BLOCK == pObj->m_nType )
				{
					BlockLoadIcon( (CBlockObj *)pObj );
				}
				else if(  NODETYPE_ENDCALL_BLOCK == pObj->m_nType )
				{
					m_pEndCallBlockObj = (CEndCallBlockObj *)pObj ;
					EndCallBlockLoadIcon(  (CEndCallBlockObj *)pObj );
				}
			}
			
		}

		if( 1 == m_nDocType ) 
		{
			m_ParameterLst.Serialize(ar);
			ar >> m_nParamBaseKey;
		}
	}


	COleDocument::Serialize(ar);
}


void CDrawDoc::BlockLoadIcon(CBlockObj *pObj)
{
	POSITION pos = pObj->m_ChildObjLst.GetTailPosition();
	while (pos)
	{
		CNodeObj* pObj1 =pObj->m_ChildObjLst.GetPrev(pos);
		if( NULL != pObj1 )
		{
			pObj1->m_hIcon = AfxGetApp( )->LoadIcon( pObj1->m_nIcoID );
			if(  NODETYPE_BLOCK == pObj1->m_nType )
			{
				BlockLoadIcon( (CBlockObj *)pObj1 );
			}
			else if(  NODETYPE_ENDCALL_BLOCK == pObj->m_nType )
			{
				m_pEndCallBlockObj = (CEndCallBlockObj *)pObj ;
				EndCallBlockLoadIcon(  (CEndCallBlockObj *)pObj );
			}
		}
		
	}
	return;
}

void CDrawDoc::EndCallBlockLoadIcon(CEndCallBlockObj *pObj)
{
	POSITION pos = pObj->m_ChildObjLst.GetTailPosition();
	while (pos)
	{
		CNodeObj* pObj1 =pObj->m_ChildObjLst.GetPrev(pos);
		if( NULL != pObj1 )
		{
			pObj1->m_hIcon = AfxGetApp( )->LoadIcon( pObj1->m_nIcoID );
		}
		
	}
	return;
}

/////////////////////////////////////////////////////////////////////////////
// CDrawDoc diagnostics

#ifdef _DEBUG
void CDrawDoc::AssertValid() const
{
	COleDocument::AssertValid();
}

void CDrawDoc::Dump(CDumpContext& dc) const
{
	COleDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawDoc commands

void CDrawDoc::ComputePageSize()
{
	m_size = CSize(7593,11378);

}

void CDrawDoc::OnDrawBlock(CDC *pDC, CBlockObj *pObj )
{
	if( NULL == pObj ) return;
	CNodeObjList *pobjLst = &(pObj->m_ChildObjLst);
   	POSITION pos = pobjLst->GetHeadPosition();
	while (pos)
	{
		CNodeObj* pNewObj = pobjLst->GetNext(pos);
		if( NULL != pNewObj )
		{
			pNewObj->OnDraw(pDC);
		}

	}
}
void CDrawDoc::OnDrawEndCallBlock(CDC *pDC, CEndCallBlockObj *pObj)
{
	if( NULL == pObj ) return;
	CNodeObjList *pobjLst = &(pObj->m_ChildObjLst);
   	POSITION pos = pobjLst->GetHeadPosition();
	while (pos)
	{
		CNodeObj* pNewObj = pobjLst->GetNext(pos);
		if( NULL != pNewObj )
		{
			pNewObj->OnDraw(pDC);
		}

	}
}
void CDrawDoc::Draw(CDC *pDC, CDrawView *pView)
{
	CNodeObjList *pObjLst = &(m_objects);
   	POSITION pos = pObjLst->GetHeadPosition();
	while (pos)
	{
		CNodeObj* pObj = pObjLst->GetNext(pos);
		if( 0 == pView->m_nShowMode )
		{
			pObj->OnDraw(pDC);
		}
		else if(1 == pView->m_nShowMode )
		{
			OnDrawBlock( pDC,pView->m_pBlockObj );
		}
		else if(2 == pView->m_nShowMode )
		{
			OnDrawEndCallBlock( pDC,m_pEndCallBlockObj );
		}
	}

}

CNodeObj * CDrawDoc::ObjectAt(const CPoint &point)
{
	CNodeObjList *pObjLst = &(m_objects);
	CRect rect(point, CSize(1, 1));
	POSITION pos = pObjLst->GetTailPosition();
	while (pos)
	{
		CNodeObj* pObj = pObjLst->GetPrev(pos);
		if (pObj->Intersects(rect))
			return pObj;
	}

	return NULL;
}


BOOL CDrawDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COleDocument::OnSaveDocument(lpszPathName);
}

//判断鼠标是否在节点对象上
CNodeObj * CDrawDoc::HitTestNodeObj(CPoint point, CDrawView *pView )
{
	if( 0 == pView->m_nShowMode )
	{
		POSITION pos = m_objects.GetTailPosition();
		while (pos)
		{
			CNodeObj* pObj = m_objects.GetPrev(pos);
			if( NODETYPE_ENDCALL_BLOCK == pObj->m_nType)
			{
				continue;
			}
			if (pObj->HitTest(point) )
			{
				pObj->SetSelected(TRUE);
				return pObj;
			}
			else
				pObj->SetSelected(FALSE);

		}
	}
	else if( 1 == pView->m_nShowMode )
	{
		CBlockObj *pBlockObj =  pView->m_pBlockObj;
		POSITION pos = pBlockObj->m_ChildObjLst.GetTailPosition();
		while (pos)
		{
			CNodeObj* pObj = pBlockObj->m_ChildObjLst.GetPrev(pos);
			
			if (pObj->HitTest(point) )
			{
				pObj->SetSelected(TRUE);
				return pObj;
			}
			else
				pObj->SetSelected(FALSE);

		}
	}
	else if( 2 == pView->m_nShowMode )
	{
		POSITION pos = m_pEndCallBlockObj->m_ChildObjLst.GetTailPosition();
		while (pos)
		{
			CNodeObj* pObj = m_pEndCallBlockObj->m_ChildObjLst.GetPrev(pos);
			if (pObj->HitTest(point) )
			{
				pObj->SetSelected(TRUE);
				return pObj;
			}
			else
				pObj->SetSelected(FALSE);

		}

	}

	return NULL;
}

CNextNode * CDrawDoc::HitTestNextNodeObj( CPoint point, CDrawView *pView )
{
	CNodeObj *pNodeObj = HitTestNodeObj( point, pView );

	if( pNodeObj != NULL )
	{
		CNextNode *pNextNodeObj = pNodeObj->HitTestNextNode( point );
		if( pNextNodeObj != NULL )
			return pNextNodeObj;
		else
			return NULL;

	}
	else
	{
		return NULL;
	}

}

void CDrawDoc::Remove(CNodeObj *pObj)
{
	// Find and remove from document
	POSITION pos = m_objects.Find(pObj);
	if (pos != NULL)
	{
		CNodeObj * pData1 = m_objects.GetAt(pos);
		m_objects.RemoveAt(pos);
		delete pData1;
	}
	// set document modified flag
	SetModifiedFlag();

	return;
}

void CDrawDoc::Remove(CDBConnVar *pObj)
{
	// Find and remove from document
	POSITION pos = m_DBConnVarList.Find(pObj);
	if (pos != NULL)
	{
		CDBConnVar * pData1 = m_DBConnVarList.GetAt(pos);
		m_DBConnVarList.RemoveAt(pos);
		delete pData1;
	}
	// set document modified flag
	SetModifiedFlag();

	return;
}



void CDrawDoc::Remove(CRecordsetVar *pObj)
{
	// Find and remove from document
	POSITION pos = m_RecordsetVariList.Find(pObj);
	if (pos != NULL)
	{
		CRecordsetVar * pData1 = m_RecordsetVariList.GetAt(pos);
		m_RecordsetVariList.RemoveAt(pos);
		delete pData1;
	}
	// set document modified flag
	SetModifiedFlag();

	return;
}

void CDrawDoc::Remove(CNormalVar *pObj)
{
	// Find and remove from document
	POSITION pos = m_NormalVarList.Find(pObj);
	if (pos != NULL)
	{
		CNormalVar * pData1 = m_NormalVarList.GetAt(pos);
		m_NormalVarList.RemoveAt(pos);
		delete pData1;
	}
	// set document modified flag
	SetModifiedFlag();

	return;
}

void CDrawDoc::RemoveObjects()
{

	while( m_objects.GetHeadPosition() )
	{
		delete m_objects.RemoveHead();
	}

	while( m_DBConnVarList.GetHeadPosition() )
	{
		delete m_DBConnVarList.RemoveHead();
	}

	while( m_RecordsetVariList.GetHeadPosition() )
	{
		delete m_RecordsetVariList.RemoveHead();
	}

	while( m_NormalVarList.GetHeadPosition() )
	{
		delete m_NormalVarList.RemoveHead();
	}

	while( m_DLLVariableList.GetHeadPosition() )
	{
		delete m_DLLVariableList.RemoveHead();
	}


	while( m_ParameterLst.GetHeadPosition() )
	{
		delete m_ParameterLst.RemoveHead();
	}

	return;
}



void CDrawDoc::InvalidateRect(CDrawView *pView)
{
	POSITION pos = m_objects.GetTailPosition();
	CRect rect;
	while (pos)
	{
		CNodeObj* pObj = m_objects.GetPrev(pos);
		rect = pObj->m_rect;
		pView->DocToClient( rect );
		pView->InvalidateRect(rect, FALSE);

	}

	return;
}

CNodeObj * CDrawDoc::FindNodeObjByKey(int nKey)
{
	POSITION pos = m_objects.GetTailPosition();
	while (pos)
	{
		CNodeObj* pObj = m_objects.GetPrev(pos);
		
		if ( nKey == pObj->m_nKey  )
		{

			return pObj;
		}

	}

	return NULL;

}


void CDrawDoc::MoveLineBlockObj(int nNodeObjKey , CBlockObj *pBlockObj,CRect EndRect)
{
	CNodeObjList *pobjLst = &(pBlockObj->m_ChildObjLst);
	POSITION pos = pobjLst->GetTailPosition();
	while (pos)
	{
		CNodeObj* pObj = pobjLst->GetPrev(pos);
		if( NULL != pObj )
		{
			pObj->MoveLineObj( nNodeObjKey ,EndRect );
			if( NODETYPE_BLOCK == pObj->m_nType )
				MoveLineBlockObj( nNodeObjKey, (CBlockObj *)pObj,EndRect);//递归调用，直到所有
		}

	}

	return ;
}

void CDrawDoc::MoveLineEndCallBlockObj(int nNodeObjKey, CEndCallBlockObj *pBlockObj, CRect EndRect)
{
	CNodeObjList *pobjLst = &(pBlockObj->m_ChildObjLst);
	POSITION pos = pobjLst->GetTailPosition();
	while (pos)
	{
		CNodeObj* pObj = pobjLst->GetPrev(pos);
		if( NULL != pObj )
		{
			pObj->MoveLineObj( nNodeObjKey ,EndRect );
		
		}

	}

	return ;
}

void CDrawDoc::MoveLineObj(int nNodeObjKey, CRect EndRect )
{
	CNodeObjList *pObjLst = &(m_objects);
	POSITION pos = pObjLst->GetTailPosition();
	while (pos)
	{
		CNodeObj* pObj = pObjLst->GetPrev(pos);
		if( NULL != pObj )
		{
			pObj->MoveLineObj( nNodeObjKey ,EndRect );
			if( NODETYPE_BLOCK == pObj->m_nType )
			{
				MoveLineBlockObj( nNodeObjKey, (CBlockObj *)pObj,EndRect);//递归调用，直到所有
			}
			else if( NODETYPE_ENDCALL_BLOCK == pObj->m_nType )
			{
				MoveLineEndCallBlockObj( nNodeObjKey, (CEndCallBlockObj *)pObj,EndRect);
			}
		}

	}

	return ;

}

BOOL CDrawDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CString strPath=lpszPathName;
	if( "ief"==strPath.Right(3) )
		m_nDocType = 0;
	else
		m_nDocType = 1;

	if (!COleDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	m_bNewDocument = FALSE;
	return TRUE;
}

void CDrawDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	g_pNodeTreeFormView->DeleteAllItems();
	COleDocument::OnCloseDocument();
}

CNodeObj * CDrawDoc::FindNodeObj(LPCTSTR lpszName)
{
	CString str(lpszName);
	
	POSITION pos = m_objects.GetTailPosition();
	while (pos)
	{
		CNodeObj* pObj = m_objects.GetPrev(pos);
		
		if( str == pObj->m_strCaption )
		{
			return pObj;
		}

	}

	return NULL;
}

CNodeObj * CDrawDoc::FindNodeObj(int nKey)
{

	POSITION pos = m_objects.GetTailPosition();
	while (pos)
	{
		CNodeObj* pObj = m_objects.GetPrev(pos);
		if( NULL == pObj ) continue;
		if( nKey == pObj->m_nKey  )
		{
			return pObj;
		}
		else
		{
			if(  NODETYPE_BLOCK == pObj->m_nType )
			{
				CBlockObj *pObj1 = (CBlockObj*)pObj;
				CNodeObj* pObj2 = pObj1->FindObj(nKey);
				if( NULL != pObj2 )
					return pObj2;
			}
			else if(  NODETYPE_ENDCALL_BLOCK == pObj->m_nType )
			{
				CEndCallBlockObj *pObj1 = (CEndCallBlockObj*)pObj;
				CNodeObj* pObj2 = pObj1->FindObj(nKey);
				if( NULL != pObj2 )
					return pObj2;
			}
		}

	}

	return NULL;
}


void CDrawDoc::RemoveAllNormalVariable()
{
	while( m_NormalVarList.GetHeadPosition() )
	{
		CNormalVar * pObj =m_NormalVarList.RemoveHead();
		if( NULL == pObj )
			continue;
		g_pNodeTreeFormView->DeleteItem( pObj->m_nKey );
		
		delete pObj;
		pObj = NULL;

	}
	SetModifiedFlag();
}

//查看是否已存在同类型的节点
bool CDrawDoc::HaveNodeObj(int nObjType)
{
	POSITION pos = m_objects.GetHeadPosition();
	while ( pos )
	{
		CNodeObj * pObj = m_objects.GetNext(pos);
		if( NULL == pObj )
			return false;
		if( nObjType == pObj->m_nType ) 
			return true;

	}


	return false;

}

CDBConnVar * CDrawDoc::FindDBConnectVariable(int nKey)
{
	POSITION pos = m_DBConnVarList.GetHeadPosition();
	while(pos)
	{
		CDBConnVar * pVar = m_DBConnVarList.GetNext(pos);
		if( pVar != NULL )
		{
			if( nKey == pVar->m_nKey ) return pVar;
		}
	}

	return NULL;
}

CDLLVariable * CDrawDoc::FindDLLVariable(int nKey)
{
	POSITION pos = m_DLLVariableList.GetHeadPosition();
	while(pos)
	{
		CDLLVariable * pVar = m_DLLVariableList.GetNext(pos);
		if( pVar != NULL )
		{
			if( nKey == pVar->m_nKey ) return pVar;
		}
	}

	return NULL;
}

BOOL CDrawDoc::FindNormalVariable(int nKey,int *nVarType )
{
	POSITION pos = m_NormalVarList.GetHeadPosition();
	while(pos)
	{
		CNormalVar * pVar = m_NormalVarList.GetNext(pos);
		if( pVar != NULL )
		{
			if( nKey == pVar->m_nKey )
			{
				*nVarType = pVar->m_nVarDataType;
				return TRUE;
			}
		}
	}
	if( 1 == m_nDocType )  ;//0 ief文件 1 nmf文件
	{
		pos = m_ParameterLst.GetHeadPosition();
		while(pos)
		{
			CParameter * pVar = m_ParameterLst.GetNext(pos);
			if( pVar != NULL )
			{
				if( nKey == pVar->m_nKey ) return TRUE;
			}
		}

	}

	return FALSE;
}

CRecordsetVar * CDrawDoc::FindRecordsetVar(int nKey)
{
	POSITION pos = m_RecordsetVariList.GetHeadPosition();
	while(pos)
	{
		CRecordsetVar * pVar = m_RecordsetVariList.GetNext(pos);
		if( pVar != NULL )
		{
			if( nKey == pVar->m_nKey ) return pVar;
		}
	}

	return NULL;
}

void CDrawDoc::RemoveDBConnVar(int nKey)
{

	POSITION pos = m_DBConnVarList.GetHeadPosition() ;
	while( pos )
	{
		
		CDBConnVar * pVar =m_DBConnVarList.GetAt(pos);
		if( pVar != NULL )
		{
			if( nKey == pVar->m_nKey )
			{
				g_pNodeTreeFormView->DeleteItem( pVar->m_nKey+8000);
				m_DBConnVarList.RemoveAt(pos);
				delete pVar;
				pVar = NULL;
				break;
			}
		}
		m_DBConnVarList.GetNext(pos) ;
	}

	SetModifiedFlag();

	return;

}

void CDrawDoc::RemoveRecordsetVar(int nKey)
{
	POSITION pos = m_RecordsetVariList.GetHeadPosition() ;
	while( pos )
	{
		
		CRecordsetVar * pVar =m_RecordsetVariList.GetAt(pos);
		if( pVar != NULL )
		{
			if( nKey == pVar->m_nKey )
			{
				g_pNodeTreeFormView->DeleteItem( pVar->m_nKey+8000 );
				m_RecordsetVariList.RemoveAt(pos);
				delete pVar;
				pVar = NULL;
				break;
			}
		}
		m_RecordsetVariList.GetNext(pos) ;
	}

	SetModifiedFlag();

	return;
}

void CDrawDoc::RemoveDLLVar(int nKey)
{
	POSITION pos = m_DLLVariableList.GetHeadPosition() ;
	while( pos )
	{
		
		CDLLVariable * pVar =m_DLLVariableList.GetAt(pos);
		if( pVar != NULL )
		{
			if( nKey == pVar->m_nKey )
			{
				g_pNodeTreeFormView->DeleteItem( pVar->m_nKey+8000);
				m_DLLVariableList.RemoveAt(pos);
				delete pVar;
				pVar = NULL;
				break;
			}
		}
		m_DLLVariableList.GetNext(pos) ;
	}

	SetModifiedFlag();

	return;
}

BOOL CDrawDoc::JudgeNameValidity(CString strName, int nKey )
{
	POSITION pos = m_objects.GetHeadPosition() ;
	while( pos )
	{
		
		CNodeObj * pVar =m_objects.GetAt(pos);
		if( pVar != NULL )
		{
			if( strName == pVar->m_strCaption && nKey != pVar->m_nKey )
			{
				return TRUE;
			}
		}
		m_objects.GetNext(pos) ;
	}

	return FALSE;
}

//断开连接到参数pObj的所有相关结点，当删除该结点时需要调用该函数
void CDrawDoc::DisConnectToMe( CNodeObj *pObj )
{
	POSITION pos = m_objects.GetHeadPosition();
	while( pos )
	{
		CNodeObj * pTempObj =m_objects.GetNext(pos);
		if( NULL == pTempObj )
			continue;
		if( NODETYPE_GOTO == pTempObj->m_nType )
		{
			CGoToObj *pGoToObj = ( CGoToObj* )pTempObj;
			pGoToObj->DisConnectToNode( pObj->m_nKey );
		}
		else
		{
			pTempObj->DisConnectToNode( pObj->m_nKey );
			if( NODETYPE_BLOCK == pTempObj->m_nType )
			{
				DisConnectToMeBlock( (CBlockObj*)pTempObj, pObj->m_nKey );
			}
			else if( NODETYPE_ENDCALL_BLOCK== pTempObj->m_nType )
			{
				DisConnectToMeEndCallBlock( (CEndCallBlockObj*)pTempObj, pObj->m_nKey );
			}
		}
		
	}

	return;
}

void CDrawDoc::DisConnectToMeBlock(CBlockObj *pObj, int nAimKey)
{
	if( NULL == pObj ) return;
	POSITION pos = pObj->m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj * pTempObj =pObj->m_ChildObjLst.GetNext(pos);
		if( NULL == pTempObj )
			continue;
		if( NODETYPE_GOTO == pTempObj->m_nType )
		{
			CGoToObj *pGoToObj = ( CGoToObj* )pTempObj;
			pGoToObj->DisConnectToNode( nAimKey );
		}
		else
		{
			pTempObj->DisConnectToNode( nAimKey );
			if( NODETYPE_BLOCK == pTempObj->m_nType )
			{
				DisConnectToMeBlock( (CBlockObj*)pTempObj,nAimKey);
			}
		}
	}

	return;
}

void CDrawDoc::DisConnectToMeBlock(CBlockObj *pObj)
{
	if( NULL == pObj ) return;
	POSITION pos = pObj->m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj * pTempObj =pObj->m_ChildObjLst.GetNext(pos);
		if( NULL == pTempObj )
			continue;
		DisConnectToMe( pTempObj);
		if( NODETYPE_BLOCK == pTempObj->m_nType )
		{
			DisConnectToMeBlock( (CBlockObj*)pTempObj );
		}
	}

	return;
}

BOOL CDrawDoc::GetVarFieldConstantsEtc(CWnd *pWnd , CString &str)
{

	CMenu menu;
	CPoint popPoint;	
	menu.LoadMenu( IDR_POP_EDIT );
	GetCursorPos(&popPoint);//得到鼠标位置
	
	
	int nSelection =	menu.GetSubMenu(4)->TrackPopupMenu(TPM_LEFTALIGN|TPM_NONOTIFY|TPM_RETURNCMD  ,popPoint.x,popPoint.y ,pWnd);
	menu.DestroyMenu();
	if( ID_SELECT_VAR ==nSelection)
	{
		CSelectVariablesDlg dlg;
		dlg.InitPage( this );
		if( dlg.DoModal() == IDOK )
		{
			str = "|" + dlg.m_strVarName + "|";
			return TRUE;
		}
	}
	else if( ID_SELECT_CONSTANT==nSelection)
	{
		CSelectConstantVarDlg dlg;
		
		if( dlg.DoModal() == IDOK )
		{
			str =   "|" + dlg.m_strVarName + "|";
			return TRUE;
		}
	}
	else if( ID_EXPRESSION_ASSISTANT==nSelection)
	{
		CExpressionDlg dlg;
		
		if( dlg.DoModal() == IDOK )
		{
			str =   "|" + dlg.m_strExpress + "|";
			return TRUE;
		}
	}
	else if( ID_SELECT_FILENAME==nSelection)
	{
		char szFilters[]="All Files (*.*)|*.*|Wave Files (*.wave)|*.wave|Vox Files (*.vox)|*.vox|Pcm Files (*.pcm)|*.pcm|Voc Files (*.voc)|*.voc|TIFF Files (*.tif)|*.tif||";
						  
		CFileDialog Dlg(TRUE,"","",OFN_FILEMUSTEXIST, szFilters, pWnd);

		if(Dlg.DoModal()==IDOK)
		{
			CString strPath = Dlg.GetFileName();
			str.Format("%s",strPath);
			return TRUE;
		}
	}
	else if( ID_SELECT_FILEPATH==nSelection)//选择文件与路径
	{
		char szFilters[]="All Files (*.*)|*.*|Wave Files (*.wave)|*.wave|Vox Files (*.vox)|*.vox|Pcm Files (*.pcm)|*.pcm|Voc Files (*.voc)|*.voc|TIFF Files (*.tif)|*.tif||";
						  
		CFileDialog Dlg(TRUE,"","",OFN_FILEMUSTEXIST, szFilters, pWnd);

		if(Dlg.DoModal()==IDOK)
		{
			CString strPath = Dlg.GetPathName();
			str.Format("%s",strPath);
			return TRUE;
		}

	}
	else if( ID_SELECT_FOLDER==nSelection)//选择文件夹
	{
		CString strPath(_T(""));
		BROWSEINFO bi;
		char szName[MAX_PATH];

		memset( szName, 0, sizeof(szName) );
		ZeroMemory( &bi, sizeof(BROWSEINFO) );


		bi.hwndOwner		= pWnd->GetSafeHwnd();
		bi.pszDisplayName	= szName;
		bi.lpszTitle		= "Select folder";
		bi.ulFlags			= BIF_EDITBOX;//|BIF_BROWSEINCLUDEFILES;
		LPITEMIDLIST idl	= SHBrowseForFolder(&bi);

		if( idl == NULL )
			return FALSE;

		SHGetPathFromIDList(idl,szName);
		CString strTemp(_T(""));
		strTemp.Format("%s",szName);
		if( strTemp.Right(1) != "\\" )
			strPath.Format("%s\\",strTemp);
		else
			strPath.Format("%s",strTemp);
		str = strPath;
		return TRUE;

	}
	else if( ID_FUNCTION_STR_LEFT == nSelection )
	{
		str = "Left( <string> ; <length> )";
		return TRUE;
	}
	else if( ID_FUNCTION_STR_LEN == nSelection )
	{
		str = "Len( <string> )";
		return TRUE;
	}
	else if( ID_FUNCTION_STR_MID == nSelection )
	{
		str = "Mid( <string> ; <start> ; <length> )";
		return TRUE;
	}
	else if( ID_FUNCTION_STR_RIGHT == nSelection )
	{
		str = "Right( <string> ; <length> )";
		return TRUE;
	}
	else if( ID_FUNCTION_SYS_GETHOLDCHNL == nSelection )
	{
		str = "GetHoldChnl()";
		return TRUE;
	}
	else if( ID_FUNCTION_SYS_AGENTISFREE == nSelection )
	{
		str = "AgentIsFree( <AgentDN> )";
		return TRUE;
	}
	else if( ID_FUNCTION_SYS_GETFREEAGENT == nSelection )
	{
		str = "GetFreeAgent()";
		return TRUE;
	}
	else if( ID_FUNCTION_SYS_GETLINKVAR == nSelection )
	{
		str = "GetLinkChnlVar( <Variable> )";
		return TRUE;
	}
	else if( ID_FUNCTION_SYS_GETLINKCHTYPE == nSelection )
	{
		str = "GetLinkChnlType()";
		return TRUE;
	}
	return FALSE;
}

void CDrawDoc::CheckNodeObj_BlockMember(CBlockObj *pObj, int &nError, int &nWarning , HTREEITEM &hParent )
{
   	POSITION pos = pObj->m_ChildObjLst.GetHeadPosition();
	while (pos)
	{
		CNodeObj* pNewObj = pObj->m_ChildObjLst.GetNext(pos);
		if( NULL != pNewObj )
		{
			CheckNodeObj( pNewObj, nError, nWarning,hParent );
			if( NODETYPE_BLOCK == pNewObj->m_nType )
			{
				CheckNodeObj_BlockMember( (CBlockObj *)pNewObj, nError,nWarning,hParent);
			}
		}
	}
	hParent = g_pCheckBuildForm->m_checkTree.GetParentItem(hParent);
	
	return;
}

void CDrawDoc::CheckNodeObj_EndCallBlockMember(CEndCallBlockObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	hParent = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
   	POSITION pos = pObj->m_ChildObjLst.GetHeadPosition();
	while (pos)
	{
		CNodeObj* pNewObj = pObj->m_ChildObjLst.GetNext(pos);
		if( NULL != pNewObj )
		{
			CheckNodeObj( pNewObj, nError, nWarning ,hParent);

		}
	}
	hParent = g_pCheckBuildForm->m_checkTree.GetParentItem(hParent);
	return;
}

void CDrawDoc::CheckScriptFile()
{
	int nError = 0;
	int nWarning = 0;
	g_pCheckBuildForm->InserRootItem();
	HTREEITEM hParent = NULL;
   	POSITION pos = m_objects.GetHeadPosition();
	while (pos)
	{
		CNodeObj* pObj = m_objects.GetNext(pos);
		if( NULL != pObj )
		{
			CheckNodeObj( pObj, nError, nWarning,hParent );
			if( NODETYPE_BLOCK == pObj->m_nType )
			{
				CheckNodeObj_BlockMember( (CBlockObj *)pObj, nError,nWarning,hParent);
			}
			else if( NODETYPE_ENDCALL_BLOCK == pObj->m_nType )
			{
				CheckNodeObj_EndCallBlockMember(  (CEndCallBlockObj *)pObj, nError,nWarning,hParent);
			}
		}
	}

	CString str ="";
	str.Format(" %d error(s)",nError);
	g_pCheckBuildForm->InsertNotifyItem(str);
	str.Format(" %d Warning(s)",nWarning);
	g_pCheckBuildForm->InsertNotifyItem(str);
	g_pCheckBuildForm->InsertNotifyItem("End Checking");

	return;
}
void CDrawDoc::CheckWaitCall(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent )
{

	if( NULL == pObj ) return;
	if( NODETYPE_WAITCALL != pObj->m_nType ) return;

	CWaitCallObj *pNodeObj = ( CWaitCallObj *)pObj;

	//开始检查项目
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;
	if( pNodeObj->m_nRings < 0 ) 
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Rings < 0" , item );
		nError += 1;
	}

	
	if( pNodeObj->m_bRecCallerID ) 
	{
		if( "" == pNodeObj->m_CallerID.m_strName )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey, hParent);
			g_pCheckBuildForm->InsertErrorItem( "Error : Not input Save CallerID Variable " , item );
			nError += 1;
		}
		else
			if( !CheckNormalVariableSet( pNodeObj->m_CallerID.m_strName , strErrrDescript, nVarKey,nVarType ) )
			{
				if( NULL == item )
					item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey, hParent);
				g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
				nError += 1;
			}

	}


	if( pNodeObj->m_bRecCalledID ) 
	{
		if( "" == pNodeObj->m_CalledID.m_strName )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey, hParent);
			g_pCheckBuildForm->InsertErrorItem( "Error : Not input Save CalledID Variable < 0" , item );
			nError += 1;
		}
		else
			if( !CheckNormalVariableSet( pNodeObj->m_CalledID.m_strName , strErrrDescript, nVarKey ,nVarType ) )
			{
				if( NULL == item )
					item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey, hParent);
				g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
				nError += 1;
			}

	}


	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey, hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}

	return;
}

void CDrawDoc::CheckNodeObj( CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent  )
{
	if( NULL == pObj ) return ;
	
	switch( pObj->m_nType )
	{
	case  NODETYPE_WAITCALL:
		CheckWaitCall( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_START_PLAY:
		CheckPlayStrat( pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_CHECK_DIAL:
		CheckDialEnd( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_RELEASECALL:
		CheckReleaseCall( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_CALLHOLD:
		CheckCallHold( pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_CHECK_TRANSFER:
		CheckTransferEnd( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_START_RECORD:
		CheckRecordStart( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_SETVARIABLE:
		CheckSetVariable( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_COMPARE:
		CheckCompare( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_ENTER_CONFERENCE:
		CheckConferEnter( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_LEAVE_CONFERENCE:
		CheckConferLeave( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_OPEN_DATABASE:
		CheckDBOpen( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_EXCUTE_SQL:
		CheckSQLExcute( pObj, nError, nWarning,hParent );
		break;	
	case  NODETYPE_CLOSE_DATABASE:
		CheckDBClose( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_DATABASE_BROWSE:
		CheckDBBrows( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_DLL_LOAD:
		CheckDLLLoad( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_DLL_CALL:
		CheckDLLCall( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_DLL_UNLOAD:
		CheckDLLUnLoad( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_SEND_DTMFCODE:
		CheckDTMFSend( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_RECIVE_DTMF:
		CheckDTMFRecive( pObj, nError, nWarning,hParent );
		break;
	case  NODETYPE_EVENT_CHECK:
		CheckEventCheck( pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_DTMF_MENU:
		CheckDTMFMenu( pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_CALLLOGREQUEST:
		CheckCallLogRequest( pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_NODE_END:
		CheckNodeEnd( pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_SWITCH:
		CheckSwitch( pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_ANSWERCALL:
		CheckAnswerCall( pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_FAXRECIVE:
		CheckFaxRecive( pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_FAXSEND:
		CheckFaxSend( pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_CONFERENCE_BUILD:
		CheckBuildConfer( pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_CONFERENCE_DELETE:
		CheckDeleteConfer( pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_GET_CONFERENCE_MEMBER:
		CheckGetConferMember( pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_CONFERENCE_REQUEST:
		CheckConferInvite( pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_BLOCK:
		CheckBlock(pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_ENTER:
		CheckEnter(pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_RETURN:
		CheckReturn(pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_STOREPRO:
		CheckStorePro(pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_CHANGEVOICE:
		CheckChangeVoice(pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_ENDCALLEVENT_CHECK:
		CheckEndCallEventCheck(pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_ENDCALLEVENT_END:
		CheckEndCallEventEnd(pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_EXPRESS:
		CheckExpress(pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_FUNCTION:
		CheckFunction(pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_GOTO:
		CheckGoTo(pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_TRACE_INFO:
		CheckTrace(pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_MOUDLE:
		CheckModuleNode(pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_MOUDLE_BEGIN:
		CheckMoudleBegin(pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_MOUDLE_END:
		CheckOhterNode(pObj, nError, nWarning,hParent );
		break;
	case NODETYPE_CUSTOM:
		break;
	case NODETYPE_CC_CALLAGENT:
		{
			CheckCCCallAgent(pObj, nError, nWarning,hParent );
		}
		break;
	case NODETYPE_CC_TRANSFER:
		{
			CheckCCTransfer(pObj, nError, nWarning,hParent );
		}
		break;
	default:
		break;
	
	}


	return ;
}

bool CDrawDoc::CheckNormalVariableSet(CString strExpress, CString &strErrrDescript, int &nVarKey,int &nVarType)
{
	if( "" == strExpress ) 
	{
		strErrrDescript = "Express is empty";
		nVarKey = -1;
		return false;
	}

	strExpress.Replace("|",NULL);
	if( "@" != strExpress.Left(1) && "$" != strExpress.Left(1) && "#" != strExpress.Left(1))
	{
		//判断是否是常量，如果是常量也返回true
		nVarKey = -1;
		return true;
	}
	
	int n = strExpress.Find(':');
	if(  n < 2)
	{
		strErrrDescript = "Express is Error, have not " + strExpress + "the Var";
		nVarKey = -1;
		return false;
	}

	nVarKey = -1;
	nVarKey = atoi( strExpress.Mid(1,n) );
	if( nVarKey > 100 )// 普通变量的Key>100
	{
		if( !FindNormalVariable( nVarKey ,&nVarType) )
		{
			strErrrDescript = "Express is Error,  have not " + strExpress + "the Var";
			nVarKey = -1;
			return false;
		}

	}

	strErrrDescript = "";

	return true;
}



void CDrawDoc::CheckNextNode(CNodeObj *pObj, HTREEITEM &item, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CString strError ="";

	if( NODETYPE_BLOCK == pObj->m_nType || NODETYPE_ENDCALL_BLOCK == pObj->m_nType  )
	{
		item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		hParent = item;
	}
	POSITION pos = pObj->m_NextNodeList.GetHeadPosition();
	while( pos )
	{
		CNextNode * pNextObj =pObj->m_NextNodeList.GetNext(pos);
		if( pNextObj != NULL )
		{
			CNodeObj *pTemNode = FindNodeObj( pNextObj->m_nNodeObjKey );
			if( NULL == pTemNode )
			{
				strError.Format( "Error: Output \" %s \"  Not connected ",  pNextObj->m_strName );
				if( NULL == item )
				{
					if( NODETYPE_BLOCK != pObj->m_nType && NODETYPE_ENDCALL_BLOCK != pObj->m_nType  )
						item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);

				}
				g_pCheckBuildForm->InsertErrorItem( strError , item );
				nError += 1;
			}

		}

	}

	return;
}

//判断是否有结点连接到自己
BOOL CDrawDoc::CheckConnectToMe(int nMeKey)
{
	
	POSITION pos = m_objects.GetHeadPosition();
	while( pos )
	{
		CNodeObj * pTempObj =m_objects.GetNext(pos);
		if( NULL == pTempObj )
			continue;
		if ( pTempObj->CheckConnectToThis( nMeKey ) )
			return TRUE;
		if( NODETYPE_BLOCK == pTempObj->m_nType )
		{
			if( CheckConnectToMe_BlockMember( nMeKey, (CBlockObj *)pTempObj ) )
				return TRUE;
		}
		else if( NODETYPE_ENDCALL_BLOCK == pTempObj->m_nType )
		{
			if( CheckConnectToMe_ECBlockMember( nMeKey, (CEndCallBlockObj *)pTempObj ) )
				return TRUE;

		}
	}
	return FALSE;
}

BOOL CDrawDoc::CheckConnectToMe_BlockMember(int nMeKey, CBlockObj *pObj )
{
	POSITION pos = pObj->m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj * pTempObj = pObj->m_ChildObjLst.GetNext(pos);
		if( NULL == pTempObj )
			continue;
		if ( pTempObj->CheckConnectToThis( nMeKey ) )
			return TRUE;
		if( NODETYPE_BLOCK == pTempObj->m_nType )
		{
			if( CheckConnectToMe_BlockMember( nMeKey, (CBlockObj *)pTempObj ) )//递归调用
				return TRUE;
		}
	}
	return FALSE;
}
BOOL CDrawDoc::CheckConnectToMe_ECBlockMember(int nMeKey, CEndCallBlockObj *pObj)
{
	POSITION pos = pObj->m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj * pTempObj = pObj->m_ChildObjLst.GetNext(pos);
		if( NULL == pTempObj )
			continue;
		if ( pTempObj->CheckConnectToThis( nMeKey ) )
			return TRUE;
	}
	return FALSE;
}
void  CDrawDoc::CheckPlayStrat(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{

	CPlayStartObj *pNodeObj = (CPlayStartObj *)pObj;

	
	//开始检查项目
	HTREEITEM item = NULL;
	int nVarKey = -1;
	int nVarType = 0;
	CString strErrrDescript = "";

	if( pNodeObj->m_bRecDTMF ) 
	{
		if( "" == pNodeObj->m_strSaveDTMF )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey, hParent);
			g_pCheckBuildForm->InsertErrorItem( "Error : Not input Save DTMF Variable" , item );
			nError += 1;
		}
		else
			if( !CheckNormalVariableSet( pNodeObj->m_strSaveDTMF , strErrrDescript, nVarKey,nVarType ) )
			{
				if( NULL == item )
					item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey, hParent);
				g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
				nError += 1;
			}

	}

	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey, hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}

	return;
}



void CDrawDoc::CheckDialEnd(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{

	HTREEITEM item = NULL;

	CheckNextNode( pObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}


	return;
}

void CDrawDoc::CheckReleaseCall(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	HTREEITEM item = NULL;

	CheckNextNode( pObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}


	return;
}

void CDrawDoc::CheckCallHold(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CCallHoldObj *pNodeObj = (CCallHoldObj *)pObj;
	HTREEITEM item = NULL; 

	if( pNodeObj->m_bPlayVox ) 
	{
		if( "" == pNodeObj->m_strPlayFile )
		{
			if( NULL == item )
			{
				item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey, hParent);
			}
			g_pCheckBuildForm->InsertErrorItem( "Error : Not input file" , item );
			nError += 1;
		}

	}
	
	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
		{
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey, hParent);
		}
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}


	return;
}

void CDrawDoc::CheckEndCallEventCheck(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	//开始检查项目
	HTREEITEM item = NULL;
	CheckNextNode( pObj, item, nError, nWarning,hParent);

	return;
}

void CDrawDoc::CheckEndCallEventEnd(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	//开始检查项目
	HTREEITEM item = NULL;

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}

	return;
}
void CDrawDoc::CheckCallLogRequest(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CCallLogRequestObj *pNodeObj = (CCallLogRequestObj *)pObj;

	
	//开始检查项目
	HTREEITEM item = NULL;
	int nVarKey = -1;
	CString strErrrDescript = "";



	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey, hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}

	return;
}


void CDrawDoc::CheckTransferEnd(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CTransferCheckObj *pNodeObj = (CTransferCheckObj *)pObj;
	HTREEITEM item = NULL;

	if( 1 == pNodeObj->m_nDialType )
	{
		if( "" == pNodeObj->m_strExpression )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( "Error : Please Input Express string" , item );
			nError += 1;
		}
	}

	if( 1 == pNodeObj->m_nWaitOption )
	{
		if( "" == pNodeObj->m_strPlayFile )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( "Error : Please Input play File" , item );
			nError += 1;
		}
	}

	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}


	return;
}

void CDrawDoc::CheckRecordStart(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CRecordObj *pNodeObj = (CRecordObj *)pObj;
	//下面开始初始化数据
	HTREEITEM item = NULL;

	CString strErrrDescript = "";
	int nVarKey = -1;

	if( "" == pNodeObj->m_strFileName )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input File" , item );
		nError += 1;
	}


	CheckNextNode( pObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}



	return;
}



void CDrawDoc::CheckConferEnter(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CEnterConferObj *pNodeObj = (CEnterConferObj *)pObj;
	//下面开始初始化数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;


	if( "" == pNodeObj->m_strChnlIndex )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Channel Index" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strChnlIndex , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}


	if( "" == pNodeObj->m_strConferNO )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Conference NO" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strConferNO , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}


	CheckNextNode( pObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}



	return;
}

void CDrawDoc::CheckConferLeave(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CLeaveConferObj *pNodeObj = (CLeaveConferObj *)pObj;
	//下面开始初始化数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;

	if( "" == pNodeObj->m_strChnlIndex )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Channel Index" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strChnlIndex , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}


	if( "" == pNodeObj->m_strConferNO )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Conference NO" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strConferNO , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}


	CheckNextNode( pObj, item, nError, nWarning,hParent);
	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}

	return;
}

void CDrawDoc::CheckSetVariable(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	//下面开始检查数据
	HTREEITEM item = NULL;

	CString strErrrDescript = "";
	int nVarKey = -1;

	CheckNextNode( pObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	return;
}

void CDrawDoc::CheckCompare(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	//下面开始检查数据
	HTREEITEM item = NULL;

	CString strErrrDescript = "";
	int nVarKey = -1;
	
	CCompareObj *pComObj = ( CCompareObj * )pObj;
	if( -1 == pComObj->m_nVarKey || "" == pComObj->m_strVarName )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pComObj->m_strCaption,pComObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Have not the Var" , item );
		nError += 1;
	}

	CheckNextNode( pComObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pComObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pComObj->m_strCaption,pComObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	return;
}

void CDrawDoc::CheckDBOpen(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	//下面开始检查数据
	HTREEITEM item = NULL;

	CString strErrrDescript = "";
	int nVarKey = -1;
	
	CDBOpenObj *pDB = ( CDBOpenObj * )pObj;

	if( 0 == pDB->m_nDBType )
	{
		CDBConnVar *pDBConnVar = FindDBConnectVariable( pDB->m_nDBKey);

		if( NULL == pDBConnVar ) 
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pDB->m_strCaption,pDB->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( "Error : have not DB Open Variable" , item );
			nError += 1;
			return;
		}

	}
	
	CString strDBIdentify	= pDB->m_strDBName;

	if( "" == strDBIdentify )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pDB->m_strCaption,pDB->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not Input Database Identify" , item );
		nError += 1;
	}

	if( "" == pDB->m_strConn )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pDB->m_strCaption,pDB->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not Input Database ConnectionString" , item );
		
		nError += 1;
	}

	CheckNextNode( pDB, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pDB->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pDB->m_strCaption,pDB->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}

	return;
}

void CDrawDoc::CheckDBClose(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CDBCloseObj *pNodeObj = (CDBCloseObj*)pObj;

	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;

	if( "" == pNodeObj->m_strDB )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : Not Select Database" , item );
		nError += 1;
	}
	else
		if( 0 == pNodeObj->m_nDBType )
		if( !CheckDBVariableSet( pNodeObj->m_strDB , strErrrDescript, nVarKey ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}

	

	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}

	return;

}

bool CDrawDoc::CheckDBVariableSet(CString strExpress, CString &strErrrDescript, int &nVarKey)
{

	if( "" == strExpress ) 
	{
		strErrrDescript = "Database variable Name is not exit";
		nVarKey = -1;
		return false;
	}

	strExpress.Replace("|",NULL);
	if( "@" != strExpress.Left(1) )
	{
		strErrrDescript = "Input Database variable is error";
		nVarKey = -1;
		return false;
	}
	
	int n = strExpress.Find(':');
	if(  n < 2)
	{
		strErrrDescript = "Database variable is Error, have not the variable";
		nVarKey = -1;
		return false;
	}

	nVarKey = -1;
	nVarKey = atoi( strExpress.Mid(1,n) );
	
	if( nVarKey < 100 )
	{
		strErrrDescript =  "Database variable is Error, have not the variable";
		nVarKey = -1;
		return false;
	}

	
	CDBConnVar * pVar = FindDBConnectVariable( nVarKey );

	if( NULL == pVar )
	{
		strErrrDescript =  "Database variable is Error, have not the variable";
		nVarKey = -1;
		return false;
	}

	strErrrDescript = "";

	return true;
}

void CDrawDoc::CheckDBBrows(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CDBBrowseObj *pNodeObj = (CDBBrowseObj*)pObj;
	
	
	//下面开始检查数据
	//开始检查项目
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;
	
	if( !CheckDBRecordsetVariable( pNodeObj->m_strRecordset, strErrrDescript, nVarKey  ) ) 
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
		nError += 1;
	}


	if( -1 == pNodeObj->m_nBrowseType  )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		strErrrDescript = "Please select Database browse type!";
		g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
		nError += 1;
	}


	CNodeDataList *pDadaList = &(pNodeObj->m_NodeDataList);
	POSITION pos = pDadaList->GetHeadPosition( );
	BOOL bInputField = FALSE;
	
	while( pos )
	{
		nVarType = 0;
		CNodeData *pData = pDadaList->GetNext(pos);
		if( NULL == pData ) continue;
		
		bInputField = TRUE;
		if( "" == pData->m_strData )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( "Error : Not input Save field Variable" , item );
			nError += 1;
		}
		else
		{
			if( !CheckNormalVariableSet( pData->m_strData , strErrrDescript, nVarKey,nVarType ) )
			{
				if( NULL == item )
					item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
				g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
				nError += 1;
			}
		}
		pData->m_nDataType = nVarType;

	}
	
	if( !bInputField && pNodeObj->m_nBrowseType < 5 )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input  field and Variable" , item );
		nError += 1;
	}

	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}


	return;
}

bool CDrawDoc::CheckDBRecordsetVariable(CString strExpress, CString &strErrrDescript, int &nVarKey)
{

	if( "" == strExpress ) 
	{
		strErrrDescript = "Database Recordset variable Name is not exit";
		nVarKey = -1;
		return false;
	}

	strExpress.Replace("|",NULL);
	if( "@" != strExpress.Left(1) )
	{
		strErrrDescript = "Input Database Recordset variable is error";
		nVarKey = -1;
		return false;
	}
	
	int n = strExpress.Find(':');
	if(  n < 2)
	{
		strErrrDescript = "Database Recordset variable is Error, have not the variable";
		nVarKey = -1;
		return false;
	}

	nVarKey = -1;
	nVarKey = atoi( strExpress.Mid(1,n) );
	
	if( nVarKey < 100 )
	{
		strErrrDescript =  "Database Recordset variable is Error, have not the variable";
		nVarKey = -1;
		return false;
	}

	
	CRecordsetVar * pVar = FindRecordsetVar( nVarKey );

	if( NULL == pVar )
	{
		strErrrDescript =  "Database Recordset variable is Error, have not the variable";
		nVarKey = -1;
		return false;
	}

	strErrrDescript = "";

	return true;
}

void CDrawDoc::CheckSQLExcute(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{

	
	CRecordsetVar *pRecsetVar  = FindRecordsetVar( pObj->m_nKey );
	if( NULL == pRecsetVar ) return;

	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;

	int nSQLType	= pRecsetVar->m_nSQLType;
	CString strSQL	= pRecsetVar->m_strSQL;
	CString strDB	= pRecsetVar->m_strDataBase;
	CString strName	= pRecsetVar->m_strName;

	if( "" == strDB )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not Select Database" , item );
		nError += 1;
	}
	else
	{
		if( "@" == strDB.Left(1) )
		if( !CheckDBVariableSet( strDB , strErrrDescript, nVarKey ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}
	}

	if( "" == strName )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not Input Recordset Name" , item );
		nError += 1;
	}

	if( NULL == item )
		item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
	g_pCheckBuildForm->InsertWarningItem( "Warning : Please check carefully SQL string Input " , item );
	nWarning += 1;

	CheckNextNode( pObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}


	return;
}

void CDrawDoc::CheckDLLLoad(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CDLLLoadObj *pDllObj = (CDLLLoadObj *)pObj;
	CDLLVariable *pDLLVar = FindDLLVariable( pDllObj->m_nDLLKey );

	if( NULL == pDLLVar ) return;

	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;

	CString strFile	= pDLLVar->m_strDLLFileName;
	CString strHandle= pDLLVar->m_strName;

	if( "" == strHandle )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pDllObj->m_strCaption,pDllObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Dll handle" , item );
		nError += 1;
	}

	if( "" == strFile )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pDllObj->m_strCaption,pDllObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input dll file " , item );
		nError += 1;
	}

	CheckNextNode( pDllObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pDllObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pDllObj->m_strCaption,pDllObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}


	return;

}

void CDrawDoc::CheckDLLCall(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{

	CDLLCallObj *pDLLObj = (CDLLCallObj *)pObj;

	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;


	CString strDLL = pDLLObj->m_strDLL;
	if( "" == strDLL )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pDLLObj->m_strCaption,pDLLObj->m_nKey, hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input dll Handle " , item );
		nError += 1;
	}

	if( NULL == item )
		item = g_pCheckBuildForm->InsertNodeItem( pDLLObj->m_strCaption, pDLLObj->m_nKey,hParent);
	g_pCheckBuildForm->InsertWarningItem( "Warning : Please check dll Handle " , item );
	nWarning += 1;

	CString strFunction = "";
	strFunction = pDLLObj->m_strFunction ;
	if( "" == strFunction )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pDLLObj->m_strCaption,pDLLObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input dll function " , item );
		nError += 1;
	}


	CheckNextNode( pDLLObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pDLLObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pDLLObj->m_strCaption,pDLLObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}


	return;
}

void CDrawDoc::CheckDLLUnLoad(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CDLLUnLoadObj *pDLL = (CDLLUnLoadObj*)pObj;

	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;

	CString strDLLHandle = "";

	if( "" == pDLL->m_strDLL )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pDLL->m_strCaption,pDLL->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input dll Handle " , item );
		nError += 1;
	}
	if( -1 == pDLL->m_nDLLKey )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pDLL->m_strCaption,pDLL->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input dll Handle " , item );
		nError += 1;
	}
	

	if( NULL == item )
		item = g_pCheckBuildForm->InsertNodeItem( pDLL->m_strCaption,pDLL->m_nKey,hParent);
	g_pCheckBuildForm->InsertWarningItem( "Warning : Please check dll Handle " , item );
	nWarning += 1;


	CheckNextNode( pDLL, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pDLL->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pDLL->m_strCaption, pDLL->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}


	return;
}

void CDrawDoc::CheckDTMFRecive(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CDTMFReciveObj *pNodeObj = ( CDTMFReciveObj * )pObj;

		
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;


	if( "" == pNodeObj->m_SaveDTMF.m_strName )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Save DTMF Variable" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_SaveDTMF.m_strName , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}

	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	
	return;
}

void CDrawDoc::CheckDTMFSend(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CDTMFSendObj *pNodeObj = ( CDTMFSendObj * )pObj;	
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;


	if( "" == pNodeObj->m_strDTMF )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Send DTMF Variable" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strDTMF , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}

	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	
	return;
}

void CDrawDoc::CheckEventCheck(CNodeObj *pObj1, int &nError, int &nWarning, HTREEITEM &hParent)
{
	if( NULL == pObj1 ) return;
	CEventCheckObj *pObj = (CEventCheckObj *)pObj1;
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;

	if( pObj->m_bPlay )
	{
		if( "" == pObj->m_strPlayFile )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( "Error : Not Input Play File for this node" , item );
			nError += 1;
		}
		if( "" == pObj->m_strPlayDTMF )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( "Error : Not Input the DTMF that is used to Stop and Start Play File for this node" , item );
			nError += 1;
		}
	}

	CheckNextNode( pObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	
	return;
}

void CDrawDoc::BuildScriptFile()
{
	int nError = 0;
	int nWarning = 0;
	g_pCheckBuildForm->InserRootItem();
	HTREEITEM hParent = NULL;
   	POSITION pos = m_objects.GetHeadPosition();
	while (pos)
	{
		CNodeObj* pObj = m_objects.GetNext(pos);
		if( NULL != pObj )
		{
			CheckNodeObj( pObj, nError, nWarning ,hParent);
			if( NODETYPE_BLOCK == pObj->m_nType )
			{
				CheckNodeObj_BlockMember( (CBlockObj *)pObj, nError,nWarning,hParent);
			}
			else if( NODETYPE_ENDCALL_BLOCK == pObj->m_nType )
			{
				CheckNodeObj_EndCallBlockMember( (CEndCallBlockObj *)pObj, nError,nWarning,hParent);
			}
		}
	}

	CString str ="";
	str.Format(" %d error(s)",nError);
	g_pCheckBuildForm->InsertNotifyItem(str);
	str.Format(" %d Warning(s)",nWarning);
	g_pCheckBuildForm->InsertNotifyItem(str);
	g_pCheckBuildForm->InsertNotifyItem("End Checking");

	return;
}

void CDrawDoc::CheckDTMFMenu(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;

	CheckNextNode( pObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	
	return;
}


bool CDrawDoc::CheckOtherVariableSet(CString strExpress, CString &strErrrDescript, int &nVarKey)
{
	if( "" == strExpress ) 
	{
		strErrrDescript = "Express is empty";
		nVarKey = -1;
		return false;
	}

	strExpress.Replace("|",NULL);
	if( "@" != strExpress.Left(1) && "$" != strExpress.Left(1) && "#" != strExpress.Left(1))
	{
		nVarKey = -1;
		return true;
	}
	
	int n = strExpress.Find(':');
	if(  n < 2)
	{
		strErrrDescript = "Express is Error, have not " + strExpress + "the Var";
		nVarKey = -1;
		return false;
	}

	nVarKey = -1;
	nVarKey = atoi( strExpress.Mid(1,n) );
	
	

	return true;
}

void CDrawDoc::CheckNodeEnd(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	//下面开始检查数据
	HTREEITEM item = NULL;

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}	
	return;
}

void CDrawDoc::CheckSwitch(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CSwitchObj *pNodeObj = (CSwitchObj*)pObj;

	//下面开始检查数据
	//开始检查项目
	HTREEITEM item = NULL;

	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;

	if(  1 == pNodeObj->m_nAimType )
	{
		if( "" == pNodeObj->m_strAimChnl  )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
			g_pCheckBuildForm->InsertErrorItem( "Error : Not input Aim channel " , item );
			nError += 1;
		}
	}

	if( 2 == pNodeObj->m_nWaitOption )//播放语音
	{
		if( "" == pNodeObj->m_strPlayFile )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
			g_pCheckBuildForm->InsertErrorItem( "Error : Not input Playfile channel " , item );
			nError += 1;

		}

	}
	

	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}


	return;
}

void CDrawDoc::CheckAnswerCall(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	//下面开始检查数据
	HTREEITEM item = NULL;

	CheckNextNode( pObj, item, nError, nWarning,hParent);
	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}	
	return;
}


void CDrawDoc::ChangeLineColor_Type(CPoint point, CDrawView *pView, CNodeObjList ObjList)
{
	POSITION pos1 = NULL;
	CRect rect(0,0,0,0);
	CPoint endPoint(0,0) ;
	CClientDC dc(pView);

	POSITION pos = ObjList.GetTailPosition();
	while (pos)
	{
		CNodeObj* pObj = ObjList.GetPrev(pos);
		
		if( NULL != pObj)
		{
				//-----------------------------------------
  				pos1 = pObj->m_NextNodeList.GetHeadPosition();
				while (pos1 != NULL)
				{
					CNextNode *pNextObj =  pObj->m_NextNodeList.GetNext(pos1);	
					if( pNextObj->HitTest( point ) )
					{
						//选中
						if( 0 != pNextObj->m_nNodeObjKey )
						{
							pNextObj->m_LineObj.m_crLineColor = RGB(255, 0, 0);
							if( pNextObj->m_LineObj.m_crLineColor != pNextObj->m_LineObj.m_crOldLineColor )
							{
								CPoint BeginPoing = pNextObj->m_LineObj.m_BeginPoint;
								pView->DocToClient(BeginPoing);
								CRect BeginRect = pNextObj->m_LineObj.m_BeginRect;
								pView->DocToClient(BeginRect);
								CRect EndRect = pNextObj->m_LineObj.m_EndRect;
								pView->DocToClient(EndRect);

								ChangeLineInvalidate( RGB(255, 0, 0), BeginPoing, BeginRect, EndRect, pView);
								pNextObj->m_LineObj.m_crOldLineColor = RGB(255, 0, 0);
							}
						}
						
					}
					else
					{
						if( 0 != pNextObj->m_nNodeObjKey )
						{
							
							pNextObj->m_LineObj.m_crLineColor = RGB(0, 0, 255);
							if( pNextObj->m_LineObj.m_crLineColor != pNextObj->m_LineObj.m_crOldLineColor )
							{
								CPoint BeginPoing = pNextObj->m_LineObj.m_BeginPoint;
								pView->DocToClient(BeginPoing);
								CRect BeginRect = pNextObj->m_LineObj.m_BeginRect;
								pView->DocToClient(BeginRect);
								CRect EndRect = pNextObj->m_LineObj.m_EndRect;
								pView->DocToClient(EndRect);
								

								ChangeLineInvalidate( RGB(0, 0, 255), BeginPoing, BeginRect, EndRect, pView);
								pNextObj->m_LineObj.m_crOldLineColor = RGB(0, 0, 255);
							}
						}
						
					}
				}
				//-----------------------------------------
		}
	}

	return;
}

void CDrawDoc::ChangeLineColor(CPoint point, CDrawView *pView)
{


	POSITION pos1 = NULL;
	CRect rect(0,0,0,0);
	CPoint endPoint(0,0) ;
	CClientDC dc(pView);

	POSITION pos = NULL;
	CBlockObj *pBlockObj = NULL;
	CNodeObj* pObj = NULL;
	if( 0 == pView->m_nShowMode )
	{
		pos = m_objects.GetTailPosition();
	}
	else if( 1 == pView->m_nShowMode )
	{
		pBlockObj =  pView->m_pBlockObj;
		pos = pBlockObj->m_ChildObjLst.GetTailPosition();
	}
	else
	{
		pos = m_pEndCallBlockObj->m_ChildObjLst.GetTailPosition();
	}

	
	while (pos)
	{
		if( 0 == pView->m_nShowMode )
		{
			pObj = m_objects.GetPrev(pos);
		}
		else if( 1 == pView->m_nShowMode )
		{
			pBlockObj =  pView->m_pBlockObj;
			pObj = pBlockObj->m_ChildObjLst.GetPrev(pos);
		}
		else 
		{
			pObj = m_pEndCallBlockObj->m_ChildObjLst.GetPrev(pos);
		}
	
		if( NULL != pObj)
		{
				//-----------------------------------------
  				pos1 = pObj->m_NextNodeList.GetHeadPosition();
				while (pos1 != NULL)
				{
					CNextNode *pNextObj =  pObj->m_NextNodeList.GetNext(pos1);	
					if( pNextObj->HitTest( point ) )
					{
						//选中
						if( 0 != pNextObj->m_nNodeObjKey )
						{
							pNextObj->m_LineObj.m_crLineColor = RGB(255, 0, 0);
							if( pNextObj->m_LineObj.m_crLineColor != pNextObj->m_LineObj.m_crOldLineColor )
							{
								CPoint BeginPoing = pNextObj->m_LineObj.m_BeginPoint;
								pView->DocToClient(BeginPoing);
								CRect BeginRect = pNextObj->m_LineObj.m_BeginRect;
								pView->DocToClient(BeginRect);
								CRect EndRect = pNextObj->m_LineObj.m_EndRect;
								pView->DocToClient(EndRect);

								ChangeLineInvalidate( RGB(255, 0, 0), BeginPoing, BeginRect, EndRect, pView);
								pNextObj->m_LineObj.m_crOldLineColor = RGB(255, 0, 0);
							}
						}
						
					}
					else
					{
						if( 0 != pNextObj->m_nNodeObjKey )
						{
							
							pNextObj->m_LineObj.m_crLineColor = RGB(0, 0, 255);
							if( pNextObj->m_LineObj.m_crLineColor != pNextObj->m_LineObj.m_crOldLineColor )
							{
								CPoint BeginPoing = pNextObj->m_LineObj.m_BeginPoint;
								pView->DocToClient(BeginPoing);
								CRect BeginRect = pNextObj->m_LineObj.m_BeginRect;
								pView->DocToClient(BeginRect);
								CRect EndRect = pNextObj->m_LineObj.m_EndRect;
								pView->DocToClient(EndRect);
								

								ChangeLineInvalidate( RGB(0, 0, 255), BeginPoing, BeginRect, EndRect, pView);
								pNextObj->m_LineObj.m_crOldLineColor = RGB(0, 0, 255);
							}
						}
						
					}
				}
				//-----------------------------------------
		}
	}


	return;
}

void CDrawDoc::ChangeLineInvalidate( COLORREF crLineColor, CPoint sbeginPoint, CRect sBeginRect, CRect EndRect,CDrawView *pView)
{

	LOGPEN logpen;
	CPen pen;
	CPoint TempPoint=CPoint(0,0);
	CPen* pOldPen = NULL;
	logpen.lopnStyle = PS_SOLID;//实心的线
	logpen.lopnWidth.x = 1;
	logpen.lopnWidth.y = 1;
	logpen.lopnColor = crLineColor;

	CClientDC dc( pView );
	//dc.SetROP2(R2_MERGEPENNOT);
	

	if (!pen.CreatePenIndirect(&logpen))
		return;
	pOldPen = dc.SelectObject(&pen);


	//画连接线

	CPoint Point1;
	CPoint Point2;
	CPoint Point3;
	CPoint Point4;
	CPoint Point5;
	CPoint Point6;
	CPoint Point7;
	CPoint Point8;
	CPoint BeginPoint = sbeginPoint;
	CPoint endPoint(0,0) ;

	endPoint.x = EndRect.left  ;
	endPoint.y = EndRect.top + 8;

	if(  BeginPoint.y > endPoint.y )//结束点在开始点的下端
	{
		if( BeginPoint.x > endPoint.x )//结束点在开始点的左端
		{
				if( sBeginRect.top - EndRect.bottom > 10  )
				{
					Point1.x = BeginPoint.x + 10;
					Point1.y = BeginPoint.y ;

					Point2.x = Point1.x + 2;
					Point2.y = Point1.y - 2;

					Point3.x = Point2.x;
					Point3.y = EndRect.bottom + 7;

					Point4.x = Point1.x;
					Point4.y = Point3.y  - 2;

					Point5.x = EndRect.left - 12;
					Point5.y = Point4.y ;

					Point6.x = Point5.x  - 2;
					Point6.y = Point5.y - 2 ;

					Point7.x = Point6.x;
					Point7.y = endPoint.y + 2 ;

					Point8.x = Point7.x + 2 ;
					Point8.y = endPoint.y ;

				}
				else
				{
					Point1.x = BeginPoint.x + 10;
					Point1.y = BeginPoint.y ;
					Point2.x = Point1.x + 2;
					Point2.y = Point1.y - 2;

					Point3.x = Point2.x;
					Point3.y = EndRect.top  - 5;

					Point4.x = Point1.x;
					Point4.y = Point3.y  - 2;

					Point5.x = EndRect.left - 12;
					Point5.y = Point4.y ;

					Point6.x = Point5.x  - 2;
					Point6.y = Point5.y + 2 ;

					Point7.x = Point6.x;
					Point7.y = endPoint.y - 2 ;

					Point8.x = Point7.x + 2 ;
					Point8.y = endPoint.y ;
				}

		}
		else//结束点在开始点的右端
		{
			Point1.x = BeginPoint.x + 10;
			Point1.y = BeginPoint.y ;
			Point2.x = Point1.x + 2;
			Point2.y = Point1.y - 2;

			Point3 = Point2;
			Point4 = Point2;
			Point5 = Point2;
			Point6 = Point2;

			Point7.x = Point6.x;
			Point7.y = endPoint.y + 2 ;

			Point8.x = Point7.x + 2 ;
			Point8.y = endPoint.y ;
		}

	}
	else//在开始结点下方
	{
		if( BeginPoint.x > endPoint.x )//结束点在开始点的左端
		{
			if( EndRect.top - sBeginRect.bottom > 10  )
			{
				Point1.x = BeginPoint.x + 10;
				Point1.y = BeginPoint.y ;
				Point2.x = Point1.x + 2;
				Point2.y = Point1.y + 2;

				Point3.x = Point2.x;
				Point3.y = EndRect.top  - 7;

				Point4.x = Point1.x;
				Point4.y = Point3.y  + 2;

				Point5.x = EndRect.left - 12;
				Point5.y = Point4.y ;

				Point6.x = Point5.x  - 2;
				Point6.y = Point5.y + 2 ;

				Point7.x = Point6.x;
				Point7.y = endPoint.y - 2 ;

				Point8.x = Point7.x + 2 ;
				Point8.y = endPoint.y ;

			}
			else
			{
				
				Point1.x = BeginPoint.x + 10;
				Point1.y = BeginPoint.y ;
				Point2.x = Point1.x + 2;
				Point2.y = Point1.y - 2;

				Point3.x = Point2.x;
				Point3.y = sBeginRect.top  - 5;

				Point4.x = Point1.x;
				Point4.y = Point3.y  - 2;

				Point5.x = EndRect.left - 12;
				Point5.y = Point4.y ;

				Point6.x = Point5.x  - 2;
				Point6.y = Point5.y + 2 ;

				Point7.x = Point6.x;
				Point7.y = endPoint.y - 2 ;

				Point8.x = Point7.x + 2 ;
				Point8.y = endPoint.y ;
				
			}
		}
		else
		{
			Point1.x = BeginPoint.x + 10;
			Point1.y = BeginPoint.y ;
			Point2.x = Point1.x + 2;
			Point2.y = Point1.y + 2;

			Point3 = Point2;
			Point4 = Point2;
			Point5 = Point2;
			Point6 = Point2;

			Point7.x = Point6.x;
			Point7.y = endPoint.y - 2 ;

			Point8.x = Point7.x + 2 ;
			Point8.y = endPoint.y ;
		}

	}

	dc.MoveTo(BeginPoint);
	dc.LineTo(Point1);
	dc.MoveTo(Point1);
	dc.LineTo(Point2);
	dc.MoveTo(Point2);
	dc.LineTo(Point3);
	dc.MoveTo(Point3);
	dc.LineTo(Point4);
	dc.MoveTo(Point4);
	dc.LineTo(Point5);
	dc.MoveTo(Point5);
	dc.LineTo(Point6);
	dc.MoveTo(Point6);
	dc.LineTo(Point7);
	dc.MoveTo(Point7);
	dc.LineTo(Point8);
	dc.MoveTo(Point8);
	dc.LineTo(endPoint);

	//画箭头

	CPoint ptop,pbottom;
	ptop.x = endPoint.x - 4;
	ptop.y = endPoint.y - 4;

	pbottom.x = endPoint.x - 4;
	pbottom.y = endPoint.y + 4;

	dc.MoveTo(endPoint);
	dc.LineTo(ptop);
	dc.MoveTo(ptop);
	dc.LineTo(pbottom);
	dc.MoveTo(pbottom);
	dc.LineTo(endPoint);

	return;

}


void CDrawDoc::CheckFaxSend(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CFaxSendObj *pNodeObj = ( CFaxSendObj * )pObj;
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;

	if( "" == pNodeObj->m_strFileName )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input  Fax file name" , item );
		nError += 1;
	}


	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	
	return;
}

void CDrawDoc::CheckFaxRecive(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CFaxReciveObj *pNodeObj = ( CFaxReciveObj * )pObj;	
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;

	if( "" == pNodeObj->m_strFileName )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Save Fax File name" , item );
		nError += 1;
	}


	if( "" == pNodeObj->m_strPage )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption, pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Save Page number Variable" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strPage , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption, pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}

	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	
	return;
}

void CDrawDoc::CheckBuildConfer(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CConferenceBulidObj *pNodeObj = ( CConferenceBulidObj * )pObj;	
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;

	if( 1 == pNodeObj->m_nBuildType )
	{
		if( "" == pNodeObj->m_strConferNo  )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( "Error : Not input Conference NO" , item );
			nError += 1;
		}
		else
			if( !CheckNormalVariableSet( pNodeObj->m_strConferNo , strErrrDescript, nVarKey,nVarType ) )
			{
				if( NULL == item )
					item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
				g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
				nError += 1;
			}

	}

	if( "" == pNodeObj->m_strMaxMember )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Conference Max Member" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strMaxMember , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}

	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	
	return;
}

void CDrawDoc::CheckChangeVoice(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CChangeVoiceObj *pNodeObj = ( CChangeVoiceObj * )pObj;	
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";


	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	
	return;
}

void CDrawDoc::CheckStorePro(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CStoreProObj *pNodeObj = ( CStoreProObj * )pObj;	
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;

	if( "" == pNodeObj->m_strStoreProName  )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Stored Procedures Name" , item );
		nError += 1;
	}


	if( "" == pNodeObj->m_strRecordset  )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not Select Recordset" , item );
		nError += 1;
	}

	if( "" == pNodeObj->m_strReturn  )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Return Variable" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strReturn , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}

	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	
	return;

}

void CDrawDoc::CheckDeleteConfer(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CConferenceDeleteObj *pNodeObj = ( CConferenceDeleteObj * )pObj;	
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;

	if( "" == pNodeObj->m_strConferNo  )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Conference NO" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strConferNo , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}

	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	
	return;
}

void CDrawDoc::CheckGetConferMember(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CGetConferMemberObj *pNodeObj = ( CGetConferMemberObj * )pObj;	
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;

	
	if( "" == pNodeObj->m_strConferNo  )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Conference NO" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strConferNo , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption, pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}

		
	if( -1 == pNodeObj->GetVarKey( pNodeObj->m_strMemberCount )  )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Conference MemberCount" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strMemberCount , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption, pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}

	if( -1 == pNodeObj->GetVarKey( pNodeObj->m_strMaxMemberNum )  )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Conference MaxMemberNum" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strMaxMemberNum , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption, pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}

	if( -1 == pNodeObj->GetVarKey( pNodeObj->m_strMaxSpeakerNum )  )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Conference MaxSpeakerNum" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strMaxSpeakerNum , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption, pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}

	if( -1 == pNodeObj->GetVarKey( pNodeObj->m_strMaxSpeakingNum )  )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Conference MaxSpeakingNum" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strMaxSpeakingNum , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption, pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}

	if( -1 == pNodeObj->GetVarKey( pNodeObj->m_strListenNum )  )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Conference ListenNum" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strListenNum , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption, pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}

	if( -1 == pNodeObj->GetVarKey( pNodeObj->m_strSpeakerNum )  )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Conference  SpeakerNum" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strSpeakerNum , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption, pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}

	if( -1 == pNodeObj->GetVarKey( pNodeObj->m_strSpeakingNum )  )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Conference  SpeakingNum" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strSpeakingNum , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption, pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}

	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);
	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	
	return;
}

void CDrawDoc::CheckConferInvite(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CConferRequestObj *pNodeObj = ( CConferRequestObj * )pObj;	
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;

	if( "" == pNodeObj->m_strDialTel  )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Dial Tel" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strDialTel , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}


	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	
	return;
}


void CDrawDoc::CheckBlock(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	
	CBlockObj *pBlkObj = ( CBlockObj * )pObj;
	
	CheckNextNode( pBlkObj, item, nError, nWarning,hParent);
	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pBlkObj->m_nKey ) )
	{
		/*if( NULL == item )
		{
			item = g_pCheckBuildForm->InsertNodeItem( pBlkObj->m_strCaption,pBlkObj->m_nKey,hParent);
			hParent = item;
		}*/
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}

	return ;
}

void CDrawDoc::CheckEnter(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	//下面开始检查数据
	HTREEITEM item = NULL;
	CheckNextNode( pObj, item, nError, nWarning,hParent);
	return;
}

void CDrawDoc::CheckReturn(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	//下面开始检查数据
	HTREEITEM item = NULL;
	
	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}

	return;
}




void CDrawDoc::CheckFunction(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CFunctionObj *pNodeObj = (CFunctionObj *)pObj;
	//下面开始初始化数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;

	if( "" == pNodeObj->m_strFunction )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input function" , item );
		nError += 1;
	}

	if( "" == pNodeObj->m_strReturnValues )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input return values variable" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strReturnValues , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}


	CheckNextNode( pObj, item, nError, nWarning,hParent);
	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}

	return;
}

void CDrawDoc::CheckExpress(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CExpressObj *pNodeObj = (CExpressObj *)pObj;
	//下面开始初始化数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;


	if( "" == pNodeObj->m_strExpress )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input express" , item );
		nError += 1;
	}

	if( "" == pNodeObj->m_strExpressValues )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input express values variable" , item );
		nError += 1;
	}
	else
		if( !CheckNormalVariableSet( pNodeObj->m_strExpressValues , strErrrDescript, nVarKey,nVarType ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( strErrrDescript , item );
			nError += 1;
		}


	CheckNextNode( pObj, item, nError, nWarning,hParent);
	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}

	return;
}

CNodeObj * CDrawDoc::FindNodeObj( int nKey, int &nParentKey )
{
	
	POSITION pos = m_objects.GetTailPosition();
	while (pos)
	{
		CNodeObj* pObj = m_objects.GetPrev(pos);
		if( NULL == pObj ) continue;
		if( nKey == pObj->m_nKey  )
		{
			return pObj;
		}
		else
		{
			if(  NODETYPE_BLOCK == pObj->m_nType )
			{
				CBlockObj *pObj1 = (CBlockObj*)pObj;
				CNodeObj* pObj2 = pObj1->FindNodeObj(nKey,nParentKey);
				if( NULL != pObj2 )
				{
					return pObj2;
				}
			}
			else if(  NODETYPE_ENDCALL_BLOCK == pObj->m_nType )
			{
				CEndCallBlockObj *pObj1 = (CEndCallBlockObj*)pObj;
				CNodeObj* pObj2 = pObj1->FindNodeObj(nKey,nParentKey);
				if( NULL != pObj2 )
				{
					return pObj2;
				}
			}

		}

	}
	
	return NULL;
}

void CDrawDoc::CheckGoTo(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CGoToObj *pNodeObj = (CGoToObj *)pObj;
	//下面开始初始化数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;


	int nKey = pNodeObj->m_nNextObjKey;
	if( 100 > nKey )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : Not Select Aim Node " , item );
		nError += 1;
	}
	else
	{
		CNodeObj * pGoTo = FindNodeObj( nKey );
		if( NULL == pGoTo )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
			g_pCheckBuildForm->InsertErrorItem( "Error : Not Select Aim Node " , item );
			nError += 1;

		}

	}

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}

	return;
}

void CDrawDoc::UpDateNextObjName(CNodeObj *pObj)
{

}


void CDrawDoc::DisConnectToMeEndCallBlock(CEndCallBlockObj *pObj, int nAimKey )
{
	if( NULL == pObj ) return;
	POSITION pos = pObj->m_ChildObjLst.GetHeadPosition();
	while( pos )
	{
		CNodeObj * pTempObj =pObj->m_ChildObjLst.GetNext(pos);
		if( NULL == pTempObj )
			continue;
		if( NODETYPE_GOTO == pTempObj->m_nType )
		{
			CGoToObj *pGoToObj = ( CGoToObj* )pTempObj;
			pGoToObj->DisConnectToNode( nAimKey );
		}
		else
		{
			pTempObj->DisConnectToNode( nAimKey );
		}

	}

	return;
}




void CDrawDoc::CheckTrace(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CTraceObj *pNodeObj = (CTraceObj *)pObj;
	//下面开始初始化数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;


	if( "" == pNodeObj->m_strTrace )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent );
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input Trace information" , item );
		nError += 1;
	}

	

	CheckNextNode( pObj, item, nError, nWarning,hParent);
	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}

	return;
}

int	CDrawDoc::GetDataType( CString strDataType ) 
{
	strDataType.TrimRight();
	strDataType.TrimLeft();

	int nType = 0;

	if( strDataType =="Integer" )
		nType = DATATYPE_INTEGER;
	else if( strDataType =="Double" )
		nType = DATATYPE_FLOAT;
	else if( strDataType =="String" )
		nType = DATATYPE_STRING;
	else if( strDataType =="Date" )
		nType = DATATYPE_DATE;
	else if( strDataType =="Time" )
		nType = DATATYPE_TIME;
	else if( strDataType =="DateTime" )
		nType = DATATYPE_DATATIME;

	return nType;
}
CString CDrawDoc::GetDataTypeName( int nType ) 
{

	CString str="";
	switch(nType)
	{

	case DATATYPE_INTEGER:
		str="Integer";
		break;
	case DATATYPE_FLOAT:
		str="Double";
		break;
	case DATATYPE_STRING:
		str="String";
		break;
	case DATATYPE_DATE:
		str="Date";
		break;
	case DATATYPE_TIME:
		str="Time";
		break;
	case DATATYPE_DATATIME:
		str="DateTime";
		break;
	default:
		break;
	}

	return str;
}

void CDrawDoc::RemoveAllParameter()
{
	while( m_ParameterLst.GetHeadPosition() )
	{
		CParameter * pObj =m_ParameterLst.RemoveHead();
		if( NULL == pObj )
			continue;
		
		delete pObj;
		pObj = NULL;

	}
	SetModifiedFlag();
}

void CDrawDoc::CheckModuleNode(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CModuleObj *pNodeObj = (CModuleObj*)pObj;

	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;

	if( "" == pNodeObj->m_strFile )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not Select Module file" , item );
		nError += 1;
	}
	else
	{
		CModuleDocument doc;
		if( !doc.OnOpenDocument( pNodeObj->m_strFile ) )
		{
			if( NULL == item )
				item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
			g_pCheckBuildForm->InsertErrorItem( "Error : it is error that open Module file" , item );
			nError += 1;
		}
	}


	CheckNextNode( pObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}


	return;
}



void CDrawDoc::CheckOhterNode(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;


	CheckNextNode( pObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pObj->m_strCaption,pObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	
	return;
}

void CDrawDoc::CheckMoudleBegin(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;


	CheckNextNode( pObj, item, nError, nWarning,hParent);

	
	return;

}

void CDrawDoc::CheckCCCallAgent(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CCCCallAgentObj *pNodeObj = ( CCCCallAgentObj * )pObj;	
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;

	if( "" == pNodeObj->m_strAimDN )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input aim DN" , item );
		nError += 1;
	}
	if( pNodeObj->m_nTimeOut <= 0 )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : input time out must > 0" , item );
		nError += 1;
	}
	if( 2 == pNodeObj->m_nWaitType && "" == pNodeObj->m_strWaitPlayFile )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input wait play file name" , item );
		nError += 1;
	}



	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	
	return;
}

void CDrawDoc::CheckCCTransfer(CNodeObj *pObj, int &nError, int &nWarning, HTREEITEM &hParent)
{
	CCCTransferObj *pNodeObj = ( CCCTransferObj * )pObj;	
	//下面开始检查数据
	HTREEITEM item = NULL;
	CString strErrrDescript = "";
	int nVarKey = -1;
	int nVarType = 0;

	if( "" == pNodeObj->m_strAimDN )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input aim DN" , item );
		nError += 1;
	}
	if( "" == pNodeObj->m_strCancelDTMF )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input cancel transfer dtmf" , item );
		nError += 1;
	}
	if( 1 == pNodeObj->m_nWaitType && "" == pNodeObj->m_strPlayFile )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input play file name" , item );
		nError += 1;
	}


	if(  pNodeObj->m_bTalkWitDN && "" == pNodeObj->m_strCompleteDTMF )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption, pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : Not input complete transfer dtmf" , item );
		nError += 1;
	}

	CheckNextNode( pNodeObj, item, nError, nWarning,hParent);

	//判断是否有结点连接自己
	if ( !CheckConnectToMe( pNodeObj->m_nKey ) )
	{
		if( NULL == item )
			item = g_pCheckBuildForm->InsertNodeItem( pNodeObj->m_strCaption,pNodeObj->m_nKey,hParent);
		g_pCheckBuildForm->InsertErrorItem( "Error : No Connect to this node" , item );
		nError += 1;
	}
	
	return;
}
