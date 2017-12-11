#if !defined(AFX_CALLTRANSFERCHECKPAGE_H__90F3ECBE_2A09_4D3E_A288_F643ED7D28BA__INCLUDED_)
#define AFX_CALLTRANSFERCHECKPAGE_H__90F3ECBE_2A09_4D3E_A288_F643ED7D28BA__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CallTransferCheckPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCallTransferCheckPage dialog
class CDrawDoc;
class CCallTransferCheckPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCallTransferCheckPage)

// Construction
public:
	void EnableMyWindow( void );
	void InitPage(CTransferCheckObj *pObj, CDrawDoc *pDoc);
	CCallTransferCheckPage();
	~CCallTransferCheckPage();

// Dialog Data
	//{{AFX_DATA(CCallTransferCheckPage)
	enum { IDD = IDD_PAGE_TRANSFER_CHECK };
	BOOL	m_bAfterRings;
	BOOL	m_bNotAnswer;
	UINT	m_nRings;
	UINT	m_nTimes;
	int		m_nWaitOption;
	CString	m_strPlayFile;
	int		m_nDialType;
	CString	m_strExpression;
	BOOL	m_bClearDTMF;
	int		m_nTransferAim;
	int		m_nTransferType;
	CString	m_strCancelDTMF;
	CString	m_strCompleteDTMF;
	BOOL	m_bTalkWitDN;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCallTransferCheckPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCallTransferCheckPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPlayfile();
	afx_msg void OnButtonEx();
	afx_msg void OnRadioTransfertype1();
	afx_msg void OnRadioTransfertype2();
	afx_msg void OnRadioTransfertype3();
	afx_msg void OnRadioDialtype1();
	afx_msg void OnRadioDialtype2();
	afx_msg void OnCheckNotanswer();
	afx_msg void OnCheckAfterrings();
	afx_msg void OnRadioWaitoption1();
	afx_msg void OnRadioWaitoption2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CTransferCheckObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALLTRANSFERCHECKPAGE_H__90F3ECBE_2A09_4D3E_A288_F643ED7D28BA__INCLUDED_)
