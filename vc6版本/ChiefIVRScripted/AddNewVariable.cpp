// AddNewVariable.cpp : implementation file
//

#include "stdafx.h"
#include "chiefivrscripted.h"
#include "AddNewVariable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddNewVariable dialog


CAddNewVariable::CAddNewVariable(CWnd* pParent /*=NULL*/)
	: CDialog(CAddNewVariable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddNewVariable)
	m_strName = _T("");
	m_strValues = _T("");
	m_strDescription = _T("");
	//}}AFX_DATA_INIT
	m_strType = "Integer";
}


void CAddNewVariable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddNewVariable)
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cobType);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDV_MaxChars(pDX, m_strName, 30);
	DDX_Text(pDX, IDC_EDIT_VALUES, m_strValues);
	DDV_MaxChars(pDX, m_strValues, 256);
	DDX_Text(pDX, IDC_EDIT_DESCRIPT_VAR, m_strDescription);
	DDV_MaxChars(pDX, m_strDescription, 50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddNewVariable, CDialog)
	//{{AFX_MSG_MAP(CAddNewVariable)
	ON_EN_CHANGE(IDC_EDIT_VALUES, OnChangeEditValues)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddNewVariable message handlers



BOOL CAddNewVariable::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( "Integer" == m_strType )
	{
		m_cobType.SetCurSel(0); 
	}
	else if( "Double" == m_strType )
	{
		m_cobType.SetCurSel(1); 
	}
	else
	{
		m_cobType.SetCurSel(2); 
	}

	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddNewVariable::OnChangeEditValues() 
{
	CString strOldValues = m_strValues;
	UpdateData(TRUE);
	if( m_cobType.GetCurSel() < 2 )
	{

		if(! bIsNumber(m_strValues) )
			m_strValues = strOldValues;

	}

	UpdateData(FALSE);
}

BOOL CAddNewVariable::bIsNumber(CString str)
{
	LPCTSTR lpszText = (LPCTSTR)str ;
	if(lpszText == NULL) return FALSE;
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	TCHAR chFirst = lpszText[0];
	double d = _tcstod(lpszText, (LPTSTR*)&lpszText);
	if (d == 0.0 && chFirst != '0')
		return FALSE;   // could not convert
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	if (*lpszText != '\0')
		return FALSE;   // not terminated properly

	return TRUE;

}


void CAddNewVariable::OnButtonOk() 
{
	UpdateData(TRUE);
	
	if( "" == m_strName )
	{
		MessageBox("Please input name !");
		return;
	}

	if( "" == m_strValues )
		if( m_cobType.GetCurSel() < 2 )
			m_strValues = "0";


	UpdateData(FALSE);

	m_cobType.GetLBText( m_cobType.GetCurSel(), m_strType );
	
	CDialog::OnOK();
}

void CAddNewVariable::OnChangeEditName() 
{
	CString strOldName = m_strName;

	UpdateData(TRUE);
	if( !CheckNameIsAvail(m_strName) )
	{
		MessageBox("The name is forbid, because of using system char!");
		m_strName = strOldName;
	}


	UpdateData(FALSE);
	
}

bool CAddNewVariable::CheckNameIsAvail( CString strName )
{
	for(int i=0;i<strName.GetLength();i++)
	{
		 if(  '{' == strName.GetAt(i) )
			 return false;

		 if(  '}' == strName.GetAt(i) )
			 return false;

		 if(  '$' == strName.GetAt(i) )
			 return false;

		 if(  '"' == strName.GetAt(i) )
			 return false;
		 if(  '|' == strName.GetAt(i) )
			 return false;
		 if(  '&' == strName.GetAt(i) )
			 return false;
		 if(  '*' == strName.GetAt(i) )
			 return false;
		 if(  '/' == strName.GetAt(i) )
			 return false;
		 if(  '=' == strName.GetAt(i) )
			 return false;
		 if(  '>' == strName.GetAt(i) )
			 return false;
		 if(  '<' == strName.GetAt(i) )
			 return false;
		 if(  '@' == strName.GetAt(i) )
			 return false;
	}

	return true;
}
