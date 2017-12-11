#if !defined(AFX_CALLHOLDPROPAGE_H__9636C438_5E10_485E_A67C_92F885634DE0__INCLUDED_)
#define AFX_CALLHOLDPROPAGE_H__9636C438_5E10_485E_A67C_92F885634DE0__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallHoldProPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCallHoldProPage dialog
class CDrawDoc;
class CCallHoldProPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCallHoldProPage)

// Construction
public:
	void EnableMyWindow( void );
	void InitPage( CCallHoldObj * pObj ,CDrawDoc *pDoc );
	CCallHoldProPage();
	~CCallHoldProPage();

// Dialog Data
	//{{AFX_DATA(CCallHoldProPage)
	enum { IDD = IDD_PAGE_CALLHOLD };
	int		m_nHold;
	CString	m_strFile;
	BOOL	m_bPlay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCallHoldProPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCallHoldProPage)
	afx_msg void OnButtonFile();
	afx_msg void OnRadioHold1();
	afx_msg void OnRadioHold2();
	afx_msg void OnCheckPlay();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDrawDoc * m_pDoc;
	CCallHoldObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLHOLDPROPAGE_H__9636C438_5E10_485E_A67C_92F885634DE0__INCLUDED_)
