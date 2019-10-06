#include "stdafx.h"
#include "hmain.h"
#include "TestClass.h"
#include "Dll_main.h"

BOOL Test(TestClass *p)
{
	return TRUE;
}

BOOL Ansi_Call(
	char Buffer[],
	double _M,
	double _P,
	double *Result,
	UINT *Cal_Err)
{
	CString BufferTmp;
	USES_CONVERSION;
	BufferTmp = A2T(Buffer);
	BufferTmp.Replace(L'x', L'M');
	if (Formula_Calculator(BufferTmp, _M, _P, Result, Cal_Err))
	{
		return TRUE;
	}
	else return TRUE;
}

BOOL Formula_Calculator(
	CString Buffer_trans_formula,
	double _Materrnal,
	double _Paternal,
	double *Result,
	UINT *Cal_Err)
{
	stack stack;
	if (Buffer_trans_formula.Mid(0, 1) == L"-")
	{
		Buffer_trans_formula.Insert(0, L"0");
	}
	int Buffer_Len = Buffer_trans_formula.GetLength();
	CString _M, _P;
	CString Formula_Right[STACK_MAX];
	BOOL Neg_First = FALSE;
	int Items = 0;
	_M.Format(L"%f", _Materrnal);
	_P.Format(L"%f", _Paternal);
	for (int i = 0; i < Buffer_Len; i++)
	{
		CString CStmp;
		CStmp.Empty();
		CStmp = Buffer_trans_formula.Mid(i, 1);
		/*
		if (CStmp == L"-" && i == 0)
		{
			Neg_First = TRUE;
			CStmp = Buffer_trans_formula.Mid(++i, 1);
		}
		*/
		if (CStmp.SpanIncluding(L"0123456789") == CStmp)
		{
			CString tmp2 = Buffer_trans_formula.Mid(i + 1, 1);
			while ((i + 1) < Buffer_Len && tmp2.SpanIncluding(L"0123456789") == tmp2 || tmp2 == L".")
			{
				CStmp += tmp2;
				i++;
				tmp2.Empty();
				tmp2 = Buffer_trans_formula.Mid(i + 1, 1);
			}
			if (tmp2 == L"M" || tmp2 == L"P")
			{
				Buffer_trans_formula.Insert(i+1, L"*");
				Buffer_Len += 1;
			}
			Formula_Right[Items].Empty();
			Formula_Right[Items] += CStmp;
			Items++;
			continue;
		}
		else if (CStmp == L"M")
		{
			Formula_Right[Items].Empty();
			Formula_Right[Items] += _M;
			Items++;
			continue;
		}
		else if (CStmp == L"P")
		{
			Formula_Right[Items].Empty();
			Formula_Right[Items] += _P;
			Items++;
			continue;
		}
		else if (CStmp == L"l")
		{
			CStmp += Buffer_trans_formula.Mid(++i, 2);
			i++;
		}
		else if (CStmp == L"s" || CStmp == L"c" || CStmp == L"t")
		{
			CStmp += Buffer_trans_formula.Mid(++i, 3);
			i += 2;
		}
		if (stack.TOP == -1) stack.push(CStmp);
		else
		{
			if (CStmp == L")")
			{
				do
				{
					if (stack.GetTop() == L"(")
					{
						stack.pop();
						break;
					}
					else if (stack.GetTop() == L"ln(")
					{
						stack.pop();
						Formula_Right[Items].Empty();
						Formula_Right[Items] += L"n";
						Items++;
						break;
					}
					else if (stack.GetTop() == L"lg(")
					{
						stack.pop();
						Formula_Right[Items].Empty();
						Formula_Right[Items] += L"g";
						Items++;
						break;
					}
					else if (stack.GetTop() == L"sin(")
					{
						stack.pop();
						Formula_Right[Items].Empty();
						Formula_Right[Items] += L"s";
						Items++;
						break;
					}
					else if (stack.GetTop() == L"cos(")
					{
						stack.pop();
						Formula_Right[Items].Empty();
						Formula_Right[Items] += L"c";
						Items++;
						break;
					}
					else if (stack.GetTop() == L"tan(")
					{
						stack.pop();
						Formula_Right[Items].Empty();
						Formula_Right[Items] += L"t";
						Items++;
						break;
					}
					Formula_Right[Items].Empty();
					Formula_Right[Items] += stack.pop();
					Items++;
				} while (1);
			}
			else
			{
				if (   CStmp == L"("
					|| CStmp == L"ln(" || CStmp == L"lg"
					|| CStmp == L"sin(" || CStmp == L"cos(" || CStmp == L"tan()")
				{
					stack.push(CStmp);
				}
				else
				{
					if (stack.Height_Priority(CStmp, stack.GetTop()))
					{
						if (stack.Error)
						{
							return FALSE;
						}
						else
							stack.push(CStmp);
					}
					else if (!stack.Height_Priority(CStmp, stack.GetTop()))
					{
						if (stack.Error)
						{
							return FALSE;
						}
						else
						{
							do
							{
								Formula_Right[Items].Empty();
								Formula_Right[Items] += stack.pop();
								Items++;
								if (stack.TOP >= 0 && !stack.Height_Priority(CStmp, stack.GetTop()))
									continue;
								else
								{
									stack.push(CStmp);
									break;
								}
							} while (1);
						}
					}
				}
			}

		}
	}
	while (stack.TOP > -1)
	{
		Formula_Right[Items].Empty();
		Formula_Right[Items] += stack.pop();
		Items++;
	}
	/*
	if (Neg_First)
	{
		CString Negtmp;
		double doutmp;
		Negtmp = Formula_Right[0];
		doutmp = _ttof(Negtmp);
		doutmp = -doutmp;
		Formula_Right[0].Format(L"%f", doutmp);
	}
	*/
	if (stack.TOP == -1)
	{
		stack.push(Formula_Right[0]);
	}
	else
	{
		*Cal_Err = MessageBox(NULL, L"Error stack", L"ERROR", MB_RETRYCANCEL);
		return FALSE;
	}
	for (int i = 1; i < Items; i++)
	{
		if (Formula_Right[i].SpanIncluding(L"0123456789.") == Formula_Right[i])
		{
			stack.push(Formula_Right[i]);
		}
		else if (Formula_Right[i].Mid(0, 1) == L"-"
			&& !Formula_Right[i].Mid(1).IsEmpty()
			&& Formula_Right[i].Mid(1).SpanIncluding(L"0123456789.") == Formula_Right[i].Mid(1))
		{
			stack.push(Formula_Right[i]);
		}
		else
		{
			CString CStmp;
			if (Formula_Right[i] == L"+")
			{
				CStmp.Empty();
				double doub1, doub2;
				doub1 = _ttof(stack.pop());
				doub2 = _ttof(stack.pop());
				CStmp.Format(L"%f", doub1 + doub2);
				stack.push(CStmp);
			}
			else if (Formula_Right[i] == L"-")
			{
				CStmp.Empty();
				double doub1, doub2;
				doub1 = _ttof(stack.pop());
				doub2 = _ttof(stack.pop());
				CStmp.Format(L"%f", doub2 - doub1);
				stack.push(CStmp);
			}
			else if (Formula_Right[i] == L"*")
			{
				CStmp.Empty();
				double doub1, doub2;
				doub1 = _ttof(stack.pop());
				doub2 = _ttof(stack.pop());
				CStmp.Format(L"%f", doub2 * doub1);
				stack.push(CStmp);
			}
			else if (Formula_Right[i] == L"/")
			{
				CStmp.Empty();
				double doub1, doub2;
				doub1 = _ttof(stack.pop());
				if (doub1 == 0)
				{
					*Cal_Err = MessageBox(NULL, L"Divided by 0", L"ERROR", MB_RETRYCANCEL);
					return FALSE;
				}
				doub2 = _ttof(stack.pop());
				CStmp.Format(L"%f", doub2 / doub1);
				stack.push(CStmp);
			}
			else if (Formula_Right[i] == L"n")
			{
				CStmp.Empty();
				double doub;
				doub = _ttof(stack.pop());
				CStmp.Format(L"%f", log(doub));
				stack.push(CStmp);
			}
			else if (Formula_Right[i] == L"g")
			{
				CStmp.Empty();
				double doub;
				doub = _ttof(stack.pop());
				CStmp.Format(L"%f", log10(doub));
				stack.push(CStmp);
			}
			else if (Formula_Right[i] == L"s")
			{
				CStmp.Empty();
				double doub;
				doub = _ttof(stack.pop());
				CStmp.Format(L"%f", sin(doub));
				stack.push(CStmp);
			}
			else if (Formula_Right[i] == L"c")
			{
				CStmp.Empty();
				double doub;
				doub = _ttof(stack.pop());
				CStmp.Format(L"%f", cos(doub));
				stack.push(CStmp);
			}
			else if (Formula_Right[i] == L"t")
			{
				CStmp.Empty();
				double doub;
				doub = _ttof(stack.pop());
				CStmp.Format(L"%f", tan(doub));
				stack.push(CStmp);
			}
			else if (Formula_Right[i] == L"^")
			{
				CStmp.Empty();
				double doub1, doub2;
				doub1 = _ttof(stack.pop());
				doub2 = _ttof(stack.pop());
				CStmp.Format(L"%f", pow(doub2, doub1));
				stack.push(CStmp);
			}
		}
	}
	if (stack.TOP == 0)
	{
		CString Stmp;
		Stmp = stack.pop();
		*Result = _ttof(Stmp);
		*Cal_Err = stack.Cal_Err;
		return TRUE;
	}
	*Cal_Err = MessageBox(NULL, L"Unknown Error.", L"ERROR", MB_RETRYCANCEL);
	return FALSE;
}
BOOL Formula_Compare(
	double Double1,
	double Double2,
	CString CSSy,
	BOOL *Result)
{
	if (CSSy == L">")
	{
		if (Double1 > Double2)
		{
			*Result = TRUE;
		}
		else
		{
			*Result = FALSE;
		}
		return TRUE;
	}
	else if (CSSy == L"<")
	{
		if (Double1 < Double2)
		{
			*Result = TRUE;
		}
		else
		{
			*Result = FALSE;
		}
		return TRUE;
	}
	else if (CSSy == L"<=")
	{
		if (Double1 <= Double2)
		{
			*Result = TRUE;
		}
		else
		{
			*Result = FALSE;
		}
		return TRUE;
	}
	else if (CSSy == L">=")
	{
		if (Double1 >= Double2)
		{
			*Result = TRUE;
		}
		else
		{
			*Result = FALSE;
		}
		return TRUE;
	}
	else if (CSSy == L"==")
	{
		if (Double1 == Double2)
		{
			*Result = TRUE;
		}
		else
		{
			*Result = FALSE;
		}
		return TRUE;
	}
	else if (CSSy == L"!=")
	{
		if (Double1 != Double2)
		{
			*Result = TRUE;
		}
		else
		{
			*Result = FALSE;
		}
		return TRUE;
	}
	else
	{
		MessageBox(NULL, L"Unknow Error !", L"Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}
}

BOOL Advance_Formula_Compare(
	CString Buffer_formula_compare,
	double A,
	double B,
	BOOL* UPPER)
{
	int Buffer_Len = Buffer_formula_compare.GetLength();
	CString CS_1, CS_2, CS_3;
	CString CS_Sy1, CS_Sy2;
	int now = 1;
	CS_1.Empty();
	CS_2.Empty();
	CS_3.Empty();
	for (int i = 0; i < Buffer_Len; i++)
	{
		CString CStmp;
		CStmp.Empty();
		CStmp = Buffer_formula_compare.Mid(i, 1);
		if (CStmp == L" ") continue;
		if (CStmp != L"<" && CStmp != L">" && CStmp != L"=" && CStmp != L"!")
		{
			if (now == 1) CS_1 += CStmp;
			else if (now == 2) CS_2 += CStmp;
			else if (now == 3) CS_3 += CStmp;
		}
		else if (CStmp == L">" || CStmp == L"<")
		{
			CString CStmp1;
			CStmp1.Empty();
			CStmp1 = Buffer_formula_compare.Mid(i + 1, 1);
			if (CStmp1 == L"=")
			{
				i++;
				CStmp += CStmp1;
			}
			if (now == 1) CS_Sy1 += CStmp;
			else if (now == 2) CS_Sy2 += CStmp;

			now++;
		}
		else if (CStmp == L"=" || CStmp == L"!")
		{
			CStmp += Buffer_formula_compare.Mid(i + 1, 1);
			if (now == 1) CS_Sy1 += CStmp;
			else if (now == 2) CS_Sy2 += CStmp;

			now++;
		}
	}
	double DOU_1, DOU_2, DOU_3;
	UINT nSE_1, nSE_2, nSE_3;
	for (int j = 1; j <= 3; j++)
	{
		if (j == 1 && !CS_1.IsEmpty())
		{
			while (1)
			{
				if (!Formula_Calculator(CS_1, A, B, &DOU_1, &nSE_1))
				{
					if (nSE_1 == IDCANCEL)
					{
						return FALSE;
						break;
					}
					else if (nSE_1 == IDRETRY)
					{
						continue;
					}
				}
				else
				{
					break;
				}
			}
		}
		else if (j == 2 && !CS_2.IsEmpty())
		{
			while (1)
			{
				if (!Formula_Calculator(CS_2, A, B, &DOU_2, &nSE_2))
				{
					if (nSE_2 == IDCANCEL)
					{
						return FALSE;
						break;
					}
					else if (nSE_2 == IDRETRY)
					{
						continue;
					}
				}
				else
				{
					break;
				}
			}
		}
		else if (j == 3 && !CS_3.IsEmpty())
		{
			while (1)
			{
				if (!Formula_Calculator(CS_3, A, B, &DOU_3, &nSE_3))
				{
					if (nSE_3 == IDCANCEL)
					{
						return FALSE;
						break;
					}
					else if (nSE_3 == IDRETRY)
					{
						continue;
					}
				
				}
				else
				{
					break;
				}
			}
		}
	}
	BOOL Boo1=TRUE, Boo2=TRUE;
	while (1)
	{
		if (!CS_2.IsEmpty() && !CS_1.IsEmpty())
		{
			if (!Formula_Compare(DOU_1, DOU_2, CS_Sy1, &Boo1) && !CS_2.IsEmpty() && !CS_1.IsEmpty())
			{
				UINT nSELE = MessageBox(NULL, L"Compare Error !", L"Error", MB_RETRYCANCEL | MB_ICONSTOP);
				if (nSELE == IDCANCEL)
				{
					return FALSE;
					break;
				}
				else if (nSELE == IDRETRY) continue;
			}
			else
			{
				break;
			}
		}
		else break;
	}
	while (1)
	{
		if (!CS_2.IsEmpty() && !CS_3.IsEmpty())
		{
			if (!Formula_Compare(DOU_2, DOU_3, CS_Sy2, &Boo2))
			{
				UINT nSELE = MessageBox(NULL, L"Compare Error !", L"Error", MB_RETRYCANCEL | MB_ICONSTOP);
				if (nSELE == IDCANCEL)
				{
					return FALSE;
					break;
				}
				else if (nSELE == IDRETRY) continue;
			}
			else
			{
				break;
			}
		}
		else break;
	}
	if (!CS_2.IsEmpty() && !CS_3.IsEmpty())
	{
		if (Boo1 && Boo2)
		{
			MessageBox(NULL, L"Success", L"Tips", MB_OK);
			*UPPER = TRUE;
		}
		else
		{
			MessageBox(NULL, L"Success", L"Tips", MB_OK);
			*UPPER = FALSE;
		}
	}
	else if (CS_3.IsEmpty())
	{
		MessageBox(NULL,L"Success",L"Tips",MB_OK);
		return Boo1;
	}
	return TRUE;
}

