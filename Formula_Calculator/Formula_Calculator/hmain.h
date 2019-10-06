#pragma once

#ifndef _STRING_
#define _STRING_

#include<string.h>

#endif // !_STRING_

#ifndef _MATH_
#define _MATH_

#include <math.h>

#endif // !_MATH_


#define STACK_MAX 100

class stack {
public:
	void push(CString TO_PUSH);
	CString GetTop(void);
	CString pop(void);
	int Get_Priority(CString CStr);
	BOOL Height_Priority(CString CStr1, CString CStr2);
public:
	CString Stack[STACK_MAX];
	int TOP = -1;
	UINT Cal_Err;
	BOOL Error = FALSE;
};

#include "hetc.h"