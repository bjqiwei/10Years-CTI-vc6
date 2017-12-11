#if !defined(AFX_SWITCHPAGESPC_H__0618B326_9ACB_4D0D_B4DE_C499CB22FE59__INCLUDED_)
#define AFX_SWITCHPAGESPC_H__0618B326_9ACB_4D0D_B4DE_C499CB22FE59__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SwitchPageSpc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSwitchPageSpc dialog
class CDrawDoc;
class CSwitchPageSpc : public CPropertyPage
{
	DECLARE_DYNCREATE(CSwitchPageSpc)

// Construction
public:
	void EnableMyWindow( void );
	void InitPage(CSwitchObj *pNodeObj, CDrawDoc *pDoc);
	CSwitchPageSpc();
	~CSwitchPageSpc();

// Dialog Data
	//{{AFX_DATA(CSwitchPageSpc)
	enum { IDD = IDD_PAGE_SWITCH_SPC };
	int		m_nSwitchType;
	CString	m_strAimChnl;
	int		m_nAimType;
	int		m_nWaitOption;
	CString	m_strPlayFile;
	int		m_nGetFreeChWay;
	CString	m_strBeginCH;
	CString	m_strEndCH;
	int		m_nTimeOut;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CSwitchPageSpc)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CSwitchPageSpc)
	afx_msg void OnButtonAimch();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFile();
	afx_msg void OnRadioAimchtype1();
	afx_msg void OnRadioAimchtype2();
	afx_msg void OnRadioWaitoption1();
	afx_msg void OnRadioWaitoption2();
	afx_msg void OnRadioWaitoption3();
	afx_msg void OnBtnBeginCh();
	afx_msg void OnBtnEndCh();
	afx_msg void OnSelchangeComboSwitchtype();
	afx_msg void OnSwitchGetfreech1();
	afx_msg void OnSwitchGetfreech2();
	afx_msg void OnRadioAimchtype3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CSwitchObj * m_pNodeObj;
	CDrawDoc * m_pDoc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SWITCHPAGESPC_H__0618B326_9ACB_4D0D_B4DE_C499CB22FE59__INCLUDED_)
