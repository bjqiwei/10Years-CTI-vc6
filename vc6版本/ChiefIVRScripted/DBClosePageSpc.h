#if !defined(AFX_DBCLOSEPAGESPC_H__0FF3E6D5_AE02_46B5_B001_848043DC0ADD__INCLUDED_)
#define AFX_DBCLOSEPAGESPC_H__0FF3E6D5_AE02_46B5_B001_848043DC0ADD__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DBClosePageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDBClosePageSpc dialog
class CDrawDoc;
class CDBClosePageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CDBClosePageSpc)

// Construction
public:
	void InitPage( CDBCloseObj * pNodeObj, CDrawDoc *pDoc);
	CDBClosePageSpc();
	~CDBClosePageSpc();

// Dialog Data
	//{{AFX_DATA(CDBClosePageSpc)
	enum { IDD = IDD_PAGE_CLOSEDB };
	CComboBox	m_cobDB;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDBClosePageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDBClosePageSpc)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_strDB;
	CDBCloseObj * m_pNodeObj;
	CDrawDoc *m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBCLOSEPAGESPC_H__0FF3E6D5_AE02_46B5_B001_848043DC0ADD__INCLUDED_)
