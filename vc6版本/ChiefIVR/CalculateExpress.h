// CalculateExpress.h: interface for the CCalculateExpress class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCULATEEXPRESS_H__CA1C4B90_6778_4810_8FD4_C57654EB7719__INCLUDED_)
#define AFX_CALCULATEEXPRESS_H__CA1C4B90_6778_4810_8FD4_C57654EB7719__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCalculateExpress  
{
public:
	bool calculate(char string[], double &dResult );
private:
	double calbyoper(double n1,double n2,char oper);
	int tiptop_pri(double *pri);
	double Number(char *num_string);
	void clear_data(char *array);
	void del_elementl(double *array,int index);
	void del_element(char *array,int index);
	bool insert_datal(double *array,double data, int nMaxBuf );
	bool insert_data(char *array,char data, int nMaxBuf );
	double power(double x,double y);
public:
	CCalculateExpress();
	virtual ~CCalculateExpress();

};

#endif // !defined(AFX_CALCULATEEXPRESS_H__CA1C4B90_6778_4810_8FD4_C57654EB7719__INCLUDED_)
