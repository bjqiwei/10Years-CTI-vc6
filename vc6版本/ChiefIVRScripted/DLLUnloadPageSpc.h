#if !defined(AFX_DLLUNLOADPAGESPC_H__3DF185CB_3A4A_4F5E_A9D3_1F8C967E2673__INCLUDED_)
#define AFX_DLLUNLOADPAGESPC_H__3DF185CB_3A4A_4F5E_A9D3_1F8C967E2673__INCLUDED_


#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLLUnloadPageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLLUnloadPageSpc dialog
class CDrawDoc;
class CDLLUnloadPageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CDLLUnloadPageSpc)

// Construction
public:
	CString m_strDLLHandle;
	void InitPage(CDLLUnLoadObj *pNodeObj, CDrawDoc *pDoc);
	CDLLUnloadPageSpc();
	~CDLLUnloadPageSpc();

// Dialog Data
	//{{AFX_DATA(CDLLUnloadPageSpc)
	enum { IDD = IDD_PAGE_DLLUNLOAD };
	CComboBox	m_cobDLLHandle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDLLUnloadPageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDLLUnloadPageSpc)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDLLUnLoadObj * m_pNodeObj;
	CDrawDoc *m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLUNLOADPAGESPC_H__3DF185CB_3A4A_4F5E_A9D3_1F8C967E2673__INCLUDED_)
