#include "stdafx.h"
#include "CFCHH.h"

#define CF_CHTYPE_NOKNOW    -1 //未知通道类型 
#define CF_CHTYPE_EMPTY     0  //空通道 
#define CF_CHTYPE_DTNO1     1  //数字中继中国一号信令 
#define CF_CHTYPE_DTNO7     2  //数字中继中国七号信令(TUP)
#define CF_CHTYPE_DTDSS1    3  //数字中继ISDN PRI(30B+D)
#define CF_CHTYPE_DTNO7ISUP 4  //数字中继中国七号信令(ISUP)
#define CF_CHTYPE_MTP3		5  //七号信令MTP传输层通道
#define CF_CHTYPE_TRUNK     6  //模拟中继外线(普通市话线),接电信局
#define CF_CHTYPE_USER		7  //模拟中继内线(座席线)，接电话机
#define CF_CHTYPE_RECORD    8  //模拟中继高阻线(监控线)，与电话线/电话机相并联
#define CF_CHTYPE_VIP	    9  //VOIP通道类型

IMPLEMENT_SERIAL(CCFChnlSetObj, CObject, 0)
CCFChnlSetObj::CCFChnlSetObj()
{

	m_nIndex = -1;
	m_nType = -1;
	m_lCardKey = 12345;
	m_bEndable	= TRUE;
	m_strLocalDN = "";
	m_strScriptFile = "";
	m_nDspIndex = -1;

	//iSX4000 用的成员变量
	m_nNodeIndex = 0;                    /* iSX4000 index */
	m_nBrdIndex = 0;                     /* DTI board index */        
	m_nSpanIndex = 0;                    /* SPAN index */
}

CCFChnlSetObj::~CCFChnlSetObj()
{

}

CCFChnlSetObj::CCFChnlSetObj( const CCFChnlSetObj &s )
{
	m_nIndex			= s.m_nIndex;
	m_nType			= s.m_nType;
	m_lCardKey	= s.m_lCardKey;
	m_bEndable	= s.m_bEndable;
	m_strLocalDN = s.m_strLocalDN;
	m_strScriptFile = s.m_strScriptFile;
	m_nDspIndex = s.m_nDspIndex;

	m_nNodeIndex  = s.m_nNodeIndex ;
	m_nBrdIndex  = s.m_nBrdIndex ;    
	m_nSpanIndex  = s.m_nSpanIndex ;
	
}

CCFChnlSetObj & CCFChnlSetObj:: operator =( const CCFChnlSetObj &s )
{
	if(this == &s)
		return *this;

	m_nIndex			= s.m_nIndex;
	m_nType			= s.m_nType;
	m_lCardKey	= s.m_lCardKey;
	m_bEndable	= s.m_bEndable;
	m_strLocalDN = s.m_strLocalDN;
	m_strScriptFile = s.m_strScriptFile;
	m_nDspIndex = s.m_nDspIndex;

	m_nNodeIndex  = s.m_nNodeIndex ;
	m_nBrdIndex  = s.m_nBrdIndex ;    
	m_nSpanIndex  = s.m_nSpanIndex ;

	return *this;
}

void CCFChnlSetObj::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);
	if (ar.IsStoring())//保存数据
	{
		ar << m_nIndex;
		ar << m_nType;
		ar << m_lCardKey;
		ar << m_bEndable;
		ar << m_strLocalDN;
		ar << m_strScriptFile;
		ar << m_nDspIndex;


		ar << m_nNodeIndex ;
		ar << m_nBrdIndex ;    
		ar << m_nSpanIndex ;

	}
	else//读数据
	{
		ar >> m_nIndex;
		ar >> m_nType;
		ar >> m_lCardKey;
		ar >> m_bEndable;
		ar >> m_strLocalDN;
		ar >> m_strScriptFile;
		ar >> m_nDspIndex;

		ar >> m_nNodeIndex ;
		ar >> m_nBrdIndex ;    
		ar >> m_nSpanIndex ;
	}
	return;
}

////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCFSysScriptSetObj, CObject, 0)
CCFSysScriptSetObj::CCFSysScriptSetObj()
{
	m_strRemark = "";
	m_strScriptFile = "";

}

CCFSysScriptSetObj::~CCFSysScriptSetObj()
{

}

CCFSysScriptSetObj::CCFSysScriptSetObj( const CCFSysScriptSetObj &s )
{
	m_strRemark			= s.m_strRemark;
	m_strScriptFile = s.m_strScriptFile;	
}

CCFSysScriptSetObj & CCFSysScriptSetObj:: operator =( const CCFSysScriptSetObj &s )
{
	if(this == &s)
		return *this;

	m_strRemark			= s.m_strRemark;
	m_strScriptFile = s.m_strScriptFile;	

	return *this;
}

void CCFSysScriptSetObj::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);
	if (ar.IsStoring())//保存数据
	{
		ar << m_strRemark;
		ar << m_strScriptFile;
	}
	else//读数据
	{
		ar >> m_strRemark;
		ar >> m_strScriptFile;
	}
	return;
}



/////////////////////////////////////////////////////////////////////////////
// CChiefIVRDoc

IMPLEMENT_DYNCREATE(CChiefIVRDoc, CDocument)

BEGIN_MESSAGE_MAP(CChiefIVRDoc, CDocument)
	//{{AFX_MSG_MAP(CChiefIVRDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChiefIVRDoc construction/destruction

CChiefIVRDoc::CChiefIVRDoc()
{
	m_nTotalChannel = 0;
	
}

CChiefIVRDoc::~CChiefIVRDoc()
{
	while( m_ChSetListObj.GetHeadPosition() )
	{
		delete m_ChSetListObj.RemoveHead();
	}
}

BOOL CChiefIVRDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChiefIVRDoc serialization

void CChiefIVRDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		m_ChSetListObj.Serialize(ar);
		ar << m_nTotalChannel;
	}
	else
	{
		m_ChSetListObj.Serialize(ar);
		ar >> m_nTotalChannel;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CChiefIVRDoc diagnostics

#ifdef _DEBUG
void CChiefIVRDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CChiefIVRDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

CString CCFChnlSetObj::GetChannelType(int nType)
{
	CString str = "";
	switch( nType )
	{
	case 0:
		str = "User";
		break;
	case 1:
		str = "Trunk";
		break;
	case 2:
		str = "Empty";
		break;
	case 3:
		str = "Record";
		break;
	default:
		break;

	}

	return str;
}
CString CCFChnlSetObj::GetChannelTypeNew(int nType)
{
	CString str = "";
	switch( nType )
	{
	case CF_CHTYPE_NOKNOW:
		str = "NoKnow";
		break;
	case CF_CHTYPE_EMPTY:
		str = "Empty";
		break;
	case CF_CHTYPE_USER:
		str = "User";
		break;
	case CF_CHTYPE_TRUNK:
		str = "Trunk";
		break;
	case CF_CHTYPE_RECORD:
		str = "Record";
		break;
	case CF_CHTYPE_DTNO1:
		str = "SS1";
		break;
	case CF_CHTYPE_DTNO7:
		str = "SS7";
		break;
	case CF_CHTYPE_DTDSS1:
		str = "ISDN PRI";
		break;
	case CF_CHTYPE_DTNO7ISUP:
		str = "SS7_ISUP";
		break;
	case CF_CHTYPE_MTP3:
		str = "SS7_MTP";
		break;
	case CF_CHTYPE_VIP:
		str = "VIP";
		break;
		
	default:
		break;

	}

	return str;

}
/////////////////////////////////////////////////////////////////////////////
// CCIFSysScriptDoc

IMPLEMENT_DYNCREATE(CCIFSysScriptDoc, CDocument)

CCIFSysScriptDoc::CCIFSysScriptDoc()
{
	m_lCardKey = 0;
}

BOOL CCIFSysScriptDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CCIFSysScriptDoc::~CCIFSysScriptDoc()
{
	while( m_SysScriptListObj.GetHeadPosition() )
	{
		delete m_SysScriptListObj.RemoveHead();
	}
}


BEGIN_MESSAGE_MAP(CCIFSysScriptDoc, CDocument)
	//{{AFX_MSG_MAP(CCIFSysScriptDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCIFSysScriptDoc diagnostics

#ifdef _DEBUG
void CCIFSysScriptDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCIFSysScriptDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCIFSysScriptDoc serialization

void CCIFSysScriptDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		m_SysScriptListObj.Serialize(ar);
		ar << m_lCardKey;
	}
	else
	{
		m_SysScriptListObj.Serialize(ar);
		ar >> m_lCardKey;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCIFSysScriptDoc commands
//东进设备类型转化成CIF通道类型
int CCFChnlSetObj::DJDevChTypeToCifChType(int nDJChType)
{
	switch(nDJChType)
	{
	case 0:
		return CF_CHTYPE_NOKNOW;
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



BOOL CChiefIVRDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (IsModified())
		TRACE0("Warning: OnOpenDocument replaces an unsaved document.\n");

	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		return FALSE;
	}

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			Serialize(loadArchive);     // load me
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		DeleteContents();   // remove failed contents
		return FALSE;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // start off with unmodified

	return TRUE;
	

}

BOOL CCIFSysScriptDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (IsModified())
		TRACE0("Warning: OnOpenDocument replaces an unsaved document.\n");

	CFileException fe;
	CFile* pFile = GetFile(lpszPathName,
		CFile::modeRead|CFile::shareDenyWrite, &fe);
	if (pFile == NULL)
	{
		return FALSE;
	}

	DeleteContents();
	SetModifiedFlag();  // dirty during de-serialize

	CArchive loadArchive(pFile, CArchive::load | CArchive::bNoFlushOnDelete);
	loadArchive.m_pDocument = this;
	loadArchive.m_bForceFlat = FALSE;
	TRY
	{
		CWaitCursor wait;
		if (pFile->GetLength() != 0)
			Serialize(loadArchive);     // load me
		loadArchive.Close();
		ReleaseFile(pFile, FALSE);
	}
	CATCH_ALL(e)
	{
		ReleaseFile(pFile, TRUE);
		DeleteContents();   // remove failed contents
		return FALSE;
	}
	END_CATCH_ALL

	SetModifiedFlag(FALSE);     // start off with unmodified

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CVipCallSetObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CVipCallSetObj, CObject, 0)
CVipCallSetObj::CVipCallSetObj()
{
		m_nProtocol = 1;         /*protocol type*/
		m_strIPAddress = ""; 
		m_nIPPort = 0; //端口
		m_strPbxDN = "";  
		m_strDN = "";//注册号码
		m_nDspIndex = -1;//DSP编号
}

CVipCallSetObj::~CVipCallSetObj()
{

}
CVipCallSetObj & CVipCallSetObj:: operator =( const CVipCallSetObj &s )
{
	if(this == &s)
		return *this;

		m_nProtocol = s.m_nProtocol;         /*protocol type*/
		m_strIPAddress = s.m_strIPAddress; 
		m_nIPPort = s.m_nIPPort; //端口
		m_strDN = s.m_strDN; 
		m_strPbxDN = s.m_strPbxDN;
		m_nDspIndex = s.m_nDspIndex;//DSP编号

	return *this;
}
void CVipCallSetObj::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);
	if (ar.IsStoring())//保存数据
	{
		ar << m_nProtocol;         /*protocol type*/
		ar << m_strIPAddress; 
		ar << m_nIPPort; //端口
		ar << m_strDN;  //用户名
		ar << m_strPbxDN;//注册号码
		ar << m_nDspIndex;//DSP编号

	}
	else//读数据
	{
		ar >> m_nProtocol;         /*protocol type*/
		ar >> m_strIPAddress; 
		ar >> m_nIPPort; //端口
		ar >> m_strDN;  //用户名
		ar >> m_strPbxDN;//注册号码
		ar >> m_nDspIndex;//DSP编号


	}
	return;
}
