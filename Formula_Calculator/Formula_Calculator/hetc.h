
#pragma once

#ifndef _STRING_
#define _STRING_

#include<string.h>

#endif // !_STRING_

void stack::push(CString TO_PUSH)
{
	TOP++;
	if (TOP == STACK_MAX)
	{
		Cal_Err = MessageBox(NULL, L"Formula is too long.\nStack exploded.", L"Error", MB_RETRYCANCEL);

		return;
	}
	Stack[TOP].Empty();
	Stack[TOP] += TO_PUSH;
	return;
}
CString stack::pop(void)
{
	CString tmp;
	tmp = Stack[TOP];
	Stack[TOP].Empty();
	TOP--;
	return tmp;
}
CString stack::GetTop(void)
{
	return Stack[TOP];
}
int stack::Get_Priority(CString CStr)
{
	if (CStr == L"ln(" || CStr == L"lg(" 
		|| CStr == L"(" 
		|| CStr == L"sin(" || CStr == L"cos(" || CStr == L"tan(")
		return 1;
	else if (CStr == L"+" || CStr == L"-") return 2;
	else if (CStr == L"*" || CStr == L"/") return 3;
	else if (CStr == L"^") return 4;
	else if (CStr == L")") return 5;
	else
	{
		Cal_Err = MessageBox(NULL, L"Symbol Error.", L"ERROR", MB_RETRYCANCEL);
		Error = TRUE;
		return -1;
	}
}
BOOL stack::Height_Priority(CString CStr1, CString CStr2)
{
	if (Get_Priority(CStr1) > Get_Priority(CStr2))
		return TRUE;
	else if (Get_Priority(CStr1) <= Get_Priority(CStr2))
		return FALSE;
	return FALSE;
}