#if !defined(AFX_FAXRECIVEPAGESPC_H__042DAEC5_201A_4F0F_AF59_A2C38C7B3A14__INCLUDED_)
#define AFX_FAXRECIVEPAGESPC_H__042DAEC5_201A_4F0F_AF59_A2C38C7B3A14__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FaxRecivePageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFaxRecivePageSpc dialog
class CDrawDoc;
class CFaxRecivePageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CFaxRecivePageSpc)
public:
	CString m_strFileName;
// Construction
public:
	void InitPage(CFaxReciveObj *pObj, CDrawDoc *pDoc);
	CFaxRecivePageSpc();
	~CFaxRecivePageSpc();

// Dialog Data
	//{{AFX_DATA(CFaxRecivePageSpc)
	enum { IDD = IDD_PAGE_FAXRECIVE };
	CEdit	m_edtFaxfile;
	CString	m_strFolder;
	CString	m_strPage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFaxRecivePageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFaxRecivePageSpc)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFaxfile();
	afx_msg void OnButtonFloder();
	afx_msg void OnButtonPagenumber();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CFaxReciveObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAXRECIVEPAGESPC_H__042DAEC5_201A_4F0F_AF59_A2C38C7B3A14__INCLUDED_)
