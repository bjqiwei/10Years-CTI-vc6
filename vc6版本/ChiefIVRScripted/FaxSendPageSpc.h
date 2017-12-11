#if !defined(AFX_FAXSENDPAGESPC_H__501E9F8B_680B_423A_8BE3_0C81FE3A3327__INCLUDED_)
#define AFX_FAXSENDPAGESPC_H__501E9F8B_680B_423A_8BE3_0C81FE3A3327__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FaxSendPageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFaxSendPageSpc dialog
class CDrawDoc;
class CFaxSendPageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CFaxSendPageSpc)

// Construction
public:
	void InitPage(CFaxSendObj *pObj, CDrawDoc *pDoc);
	CFaxSendPageSpc();
	~CFaxSendPageSpc();

// Dialog Data
	//{{AFX_DATA(CFaxSendPageSpc)
	enum { IDD = IDD_PAGE_SENDFAX };
	CString	m_strFileName;
	CString	m_strFolder;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFaxSendPageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFaxSendPageSpc)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFaxfolder();
	afx_msg void OnButtonFax();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CFaxSendObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FAXSENDPAGESPC_H__501E9F8B_680B_423A_8BE3_0C81FE3A3327__INCLUDED_)
