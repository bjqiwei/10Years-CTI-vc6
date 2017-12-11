#if !defined(AFX_DTMFMENUPAGESPC_H__F6ECAE7F_0BAB_48C4_8ABB_0875B5354A76__INCLUDED_)
#define AFX_DTMFMENUPAGESPC_H__F6ECAE7F_0BAB_48C4_8ABB_0875B5354A76__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DTMFMenuPageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDTMFMenuPageSpc dialog
class CDrawDoc;
class CDTMFMenuPageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CDTMFMenuPageSpc)

// Construction
public:
	void InitPage(CDTMFMenuObj *pObj, CDrawDoc *pDoc);
	void EditNextNode(int nType, BOOL bEditType);
	CDTMFMenuPageSpc();
	~CDTMFMenuPageSpc();

// Dialog Data
	//{{AFX_DATA(CDTMFMenuPageSpc)
	enum { IDD = IDD_PAGE_DTMFMENU_SPC };
	CListCtrl	m_listEvent;
	BOOL	m_bClearDTMF;
	int		m_nTimeOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDTMFMenuPageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDTMFMenuPageSpc)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CDTMFMenuObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DTMFMENUPAGESPC_H__F6ECAE7F_0BAB_48C4_8ABB_0875B5354A76__INCLUDED_)
