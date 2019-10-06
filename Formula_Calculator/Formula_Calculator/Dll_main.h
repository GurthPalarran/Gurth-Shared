#pragma once

BOOL Test(TestClass *p);
BOOL Ansi_Call(char Buffer[], double _M, double _P, double *Result, UINT *Cal_Err);
BOOL Formula_Calculator(CString Buffer_trans_formula, double _Materrnal, double _Paternal, double *Result, UINT *Cal_Err);
BOOL Formula_Compare(double Double1, double Double2, CString CSSy, BOOL *Result);
BOOL Advance_Formula_Compare(CString Buffer_formula_compare,double A,double B,BOOL* UPPER);