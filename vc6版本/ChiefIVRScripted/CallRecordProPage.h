#if !defined(AFX_CALLRECORDPROPAGE_H__CDED14A3_CFDD_4AC4_A95B_AE19D8AB0F42__INCLUDED_)
#define AFX_CALLRECORDPROPAGE_H__CDED14A3_CFDD_4AC4_A95B_AE19D8AB0F42__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallRecordProPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCallRecordProPage dialog
class CDrawDoc;
class CCallRecordProPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCallRecordProPage)

// Construction
public:
	void EnableConferenceWindow( void );
	CString m_strFileName;
	void InitPage(CRecordObj *pObj, CDrawDoc *pDoc);
	CCallRecordProPage();
	~CCallRecordProPage();

// Dialog Data
	//{{AFX_DATA(CCallRecordProPage)
	enum { IDD = IDD_PAGE_CALLRECORD };
	CEdit	m_edtFileName;
	CString	m_strFolder;
	int		m_nFileLength;
	CString	m_strConferNO;
	CString	m_strEndDTMF;
	int		m_nRecordType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCallRecordProPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCallRecordProPage)
	afx_msg void OnButtonFile();
	afx_msg void OnButtonFolder();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonConfno();
	afx_msg void OnSelchangeComboRecordtype();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDrawDoc * m_pDoc;
	CRecordObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLRECORDPROPAGE_H__CDED14A3_CFDD_4AC4_A95B_AE19D8AB0F42__INCLUDED_)
