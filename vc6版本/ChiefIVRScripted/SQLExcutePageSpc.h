#if !defined(AFX_SQLEXCUTEPAGESPC_H__E2E2D34C_6CD9_4E34_BF09_228CD7584352__INCLUDED_)
#define AFX_SQLEXCUTEPAGESPC_H__E2E2D34C_6CD9_4E34_BF09_228CD7584352__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SQLExcutePageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSQLExcutePageSpc dialog
class CDrawDoc;
class CSQLExcutePageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CSQLExcutePageSpc)

// Construction
public:
	CString m_strDB;
	CString m_strSQL;
	void InitPage(CSQLExcuteObj *pNodeObj, CDrawDoc * pDoc );
	CSQLExcutePageSpc();
	~CSQLExcutePageSpc();

// Dialog Data
	//{{AFX_DATA(CSQLExcutePageSpc)
	enum { IDD = IDD_PAGE_SQLEXCUTE_SPC };
	CComboBox	m_cobDB;
	CEdit	m_editSQL;
	int		m_nSQLType;
	CString	m_strRSName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSQLExcutePageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSQLExcutePageSpc)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelect();
	afx_msg void OnButtonSql();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CRecordsetVar * m_pRecsetVar;
	CSQLExcuteObj * m_pNodeObj;
	CDrawDoc * m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SQLEXCUTEPAGESPC_H__E2E2D34C_6CD9_4E34_BF09_228CD7584352__INCLUDED_)
