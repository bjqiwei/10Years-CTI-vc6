#if !defined(AFX_DLLLOADPAGESPC_H__D8588C3C_6782_44A8_AE01_2CAEE812E181__INCLUDED_)
#define AFX_DLLLOADPAGESPC_H__D8588C3C_6782_44A8_AE01_2CAEE812E181__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLLLoadPageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDLLLoadPageSpc dialog
class CDrawDoc;
class CDLLLoadPageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CDLLLoadPageSpc)

// Construction
public:
	void EnableMyWindow( void );
	void InitPage(CDLLLoadObj *pNodeObj, CDrawDoc *pDoc);
	CDLLLoadPageSpc();
	~CDLLLoadPageSpc();

// Dialog Data
	//{{AFX_DATA(CDLLLoadPageSpc)
	enum { IDD = IDD_PAGE_DLLLOAD_SPC };
	CString	m_strHandle;
	CString	m_strFile;
	int		m_nHandleType;
	int		m_nDLLHandle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDLLLoadPageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDLLLoadPageSpc)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFile();
	afx_msg void OnRadioHandle1();
	afx_msg void OnRadioHandle2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDLLVariable *m_pDLLVar;
	CDLLLoadObj * m_pNodeObj;
	CDrawDoc *m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLLLOADPAGESPC_H__D8588C3C_6782_44A8_AE01_2CAEE812E181__INCLUDED_)
