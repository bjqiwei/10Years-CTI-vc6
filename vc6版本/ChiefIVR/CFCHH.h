#ifndef CFCHH_H
#define CFCHH_H
#include <afxtempl.h>

class   CCFChnlSetObj : public CObject  
{
protected:
	DECLARE_SERIAL(CCFChnlSetObj);
public:
	CCFChnlSetObj();
	virtual ~CCFChnlSetObj();
	CCFChnlSetObj( const CCFChnlSetObj &s );
	CCFChnlSetObj & operator =( const CCFChnlSetObj &s );
	virtual void Serialize(CArchive& ar);
	
public:
	int DJDevChTypeToCifChType( int nDJChType );
	CString GetChannelTypeNew(int nType);
	CString GetChannelType(int nType);
	int m_nIndex;
	int m_nType;
	BOOL m_bEndable;
	CString m_strLocalDN;
	CString m_strScriptFile;
	long m_lCardKey;
	int	m_nDspIndex;			// 所在DSP模块的顺序号

public:
	//iSX4000 用的成员变量
	int            m_nNodeIndex;                    /* iSX4000 index */
	int            m_nBrdIndex;                     /* DTI board index */        
	int            m_nSpanIndex;                    /* SPAN index */

};

typedef  CTypedPtrList<CObList, CCFChnlSetObj*> CFChnlSetObjList;


class   CCFSysScriptSetObj : public CObject  
{
protected:
	DECLARE_SERIAL(CCFSysScriptSetObj);
public:
	CCFSysScriptSetObj();
	virtual ~CCFSysScriptSetObj();
	CCFSysScriptSetObj( const CCFSysScriptSetObj &s );
	CCFSysScriptSetObj & operator =( const CCFSysScriptSetObj &s );
	virtual void Serialize(CArchive& ar);
	
public:
	CString m_strRemark;
	CString m_strScriptFile;

};

typedef  CTypedPtrList<CObList, CCFSysScriptSetObj*> CFScriptObjList;


class  CChiefIVRDoc : public CDocument
{
protected: // create from serialization only

	DECLARE_DYNCREATE(CChiefIVRDoc)

// Attributes
public:
	CChiefIVRDoc();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChiefIVRDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	CFChnlSetObjList m_ChSetListObj;
	int m_nTotalChannel;
	virtual ~CChiefIVRDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CChiefIVRDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};



/////////////////////////////////////////////////////////////////////////////
// CCIFSysScriptDoc document

class  CCIFSysScriptDoc : public CDocument
{
protected:
	           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCIFSysScriptDoc)

// Attributes
public:
	CCIFSysScriptDoc();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCIFSysScriptDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCIFSysScriptDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCIFSysScriptDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CFScriptObjList m_SysScriptListObj;
	long m_lCardKey;
};

class  CVipCallSetObj : public CObject  
{
protected:
	DECLARE_SERIAL(CVipCallSetObj);
public:
	CVipCallSetObj & operator =( const CVipCallSetObj &s );
	UINT m_nProtocol;         /*protocol type*/
	CString m_strIPAddress; 
	UINT m_nIPPort; //端口
	CString m_strPbxDN; //交换机号码
	CString m_strDN;//号码
	int m_nDspIndex;//DSP编号
	CVipCallSetObj();
	virtual ~CVipCallSetObj();
	virtual void Serialize(CArchive& ar);
};
typedef  CTypedPtrList<CObList, CVipCallSetObj*> CVipCallSetObjList;


/////////////////////////////////////////////////////////////////////////////
// CCIFVipCallSetDoc document

class  CCIFVipCallSetDoc : public CDocument
{
protected:
	DECLARE_DYNCREATE(CCIFVipCallSetDoc)

// Attributes
public:
	CCIFVipCallSetDoc();           // protected constructor used by dynamic creation

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCIFVipCallSetDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	CVipCallSetObjList m_VipCallSetObjList;
	virtual ~CCIFVipCallSetDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCIFVipCallSetDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
#endif 

