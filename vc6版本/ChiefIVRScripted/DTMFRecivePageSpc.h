#if !defined(AFX_DTMFRECIVEPAGESPC_H__DB4F95E3_3B1A_46E6_B1D9_1F44B1BCED68__INCLUDED_)
#define AFX_DTMFRECIVEPAGESPC_H__DB4F95E3_3B1A_46E6_B1D9_1F44B1BCED68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DTMFRecivePageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDTMFRecivePageSpc dialog
class CDrawDoc;
class CDTMFRecivePageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CDTMFRecivePageSpc)

// Construction
public:
	void EnableMyWindow( void );
	void InitPage(CDTMFReciveObj *pObj, CDrawDoc *pDoc);
	CDTMFRecivePageSpc();
	~CDTMFRecivePageSpc();

// Dialog Data
	//{{AFX_DATA(CDTMFRecivePageSpc)
	enum { IDD = IDD_PAGE_DTMFRECIVE_SPC };
	BOOL	m_bClearDTMF;
	CString	m_strDTMF;
	CString	m_strSaveTo;
	int		m_nEnd;
	int		m_nDTMFCount;
	int		m_nTimeOut;
	int		m_nDTMFType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDTMFRecivePageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDTMFRecivePageSpc)
	afx_msg void OnButtonSave();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRadioEdn1();
	afx_msg void OnRadioEdn2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CDTMFReciveObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DTMFRECIVEPAGESPC_H__DB4F95E3_3B1A_46E6_B1D9_1F44B1BCED68__INCLUDED_)
