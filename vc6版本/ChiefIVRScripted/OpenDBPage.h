#if !defined(AFX_OPENDBPAGE_H__3498557B_6A56_4581_BE02_8FA38B24A7B8__INCLUDED_)
#define AFX_OPENDBPAGE_H__3498557B_6A56_4581_BE02_8FA38B24A7B8__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenDBPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpenDBPage dialog
class CDrawDoc;
class COpenDBPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COpenDBPage)

// Construction
public:
	void EnableMyWindow( void );
	
	void InitPage( CDBOpenObj * pNodeObj, CDrawDoc *pDoc);
	COpenDBPage();
	~COpenDBPage();
private:
// Dialog Data
	//{{AFX_DATA(COpenDBPage)
	enum { IDD = IDD_PAGE_DBOPEN_SPC };
	int		m_nDBType;
	int		m_nDB;
	CString	m_strConn;
	CString	m_strDBIdentify;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COpenDBPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COpenDBPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBuildconn();
	afx_msg void OnRadioType1();
	afx_msg void OnRadioType2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDBOpenObj * m_pNodeObj;
	CDrawDoc *m_pDoc;
	CDBConnVar * m_pDBConnVar;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENDBPAGE_H__3498557B_6A56_4581_BE02_8FA38B24A7B8__INCLUDED_)
