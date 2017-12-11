#if !defined(AFX_CHANGEVOICEPAGE_H__1D305F97_649E_490B_B8AB_F31B0F206A72__INCLUDED_)
#define AFX_CHANGEVOICEPAGE_H__1D305F97_649E_490B_B8AB_F31B0F206A72__INCLUDED_

#include "..//include//NodeObj.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangeVoicePage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChangeVoicePage dialog

class CChangeVoicePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CChangeVoicePage)

// Construction
public:
	void InitPage(CChangeVoiceObj *pObj, CDrawDoc *pDoc);
	CChangeVoicePage();
	~CChangeVoicePage();

// Dialog Data
	//{{AFX_DATA(CChangeVoicePage)
	enum { IDD = IDD_PAGE_CHANGEVOICE };
	int		m_nChangeVoiceType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CChangeVoicePage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CChangeVoicePage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CDrawDoc * m_pDoc;
	CChangeVoiceObj * m_pNodeObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGEVOICEPAGE_H__1D305F97_649E_490B_B8AB_F31B0F206A72__INCLUDED_)
