#if !defined(AFX_EVENTCHECKPAGESPC_H__33EBEDEC_2999_4F56_8730_2B6EEA466AFE__INCLUDED_)
#define AFX_EVENTCHECKPAGESPC_H__33EBEDEC_2999_4F56_8730_2B6EEA466AFE__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventCheckPageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEventCheckPageSpc dialog
class CDrawDoc;
class CEventCheckPageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CEventCheckPageSpc)

// Construction
public:
	void MyEnableWindow( void );
	void InitPage(CEventCheckObj *pObj, CDrawDoc *pDoc);
	CEventCheckPageSpc();
	~CEventCheckPageSpc();

// Dialog Data
	//{{AFX_DATA(CEventCheckPageSpc)
	enum { IDD = IDD_PAGE_EVENTCHECK_SPC };
	CEdit	m_edtPlayFile;
	CListCtrl	m_listEvent;
	int		m_nElapseTime;
	BOOL	m_bClearDTMF;
	BOOL	m_bPlay;
	int		m_nPlayType;
	CString	m_strPlayDTMF;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEventCheckPageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CEventCheckPageSpc)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPlayfile();
	afx_msg void OnCheckPlay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void EditNextNode(int nType, BOOL bEditType);
	CString m_strPlayFile;
	CDrawDoc * m_pDoc;
	CEventCheckObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTCHECKPAGESPC_H__33EBEDEC_2999_4F56_8730_2B6EEA466AFE__INCLUDED_)
