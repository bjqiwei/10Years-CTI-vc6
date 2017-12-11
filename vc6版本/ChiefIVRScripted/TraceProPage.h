#if !defined(AFX_TRACEPROPAGE_H__07419B9F_580B_435F_814F_CEA0CCF875C9__INCLUDED_)
#define AFX_TRACEPROPAGE_H__07419B9F_580B_435F_814F_CEA0CCF875C9__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TraceProPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTraceProPage dialog
class CDrawDoc;
class CTraceProPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CTraceProPage)

// Construction
public:
	void InitPage(CTraceObj *pObj, CDrawDoc *pDoc);
	CTraceProPage();
	~CTraceProPage();

// Dialog Data
	//{{AFX_DATA(CTraceProPage)
	enum { IDD = IDD_PPAGE_TRACE_SPC };
	CEdit	m_ctrTrace;
	//}}AFX_DATA

	CString	m_strTrace;
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTraceProPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CTraceProPage)
	afx_msg void OnButtonTrace();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CTraceObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACEPROPAGE_H__07419B9F_580B_435F_814F_CEA0CCF875C9__INCLUDED_)
