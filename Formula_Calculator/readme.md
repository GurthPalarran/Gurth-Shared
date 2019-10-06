## 1. 库的用法：

在程序中使用
<pre>Loadlibrary("(path of Formula_Calculator.dll)") </pre>
进行显式调用，或用
<pre>
#include"Dll_main.h"
#progma comment(lib,"(path of Formula_Calculator.lib)")</pre>
进行隐式调用;

## 2. 库函数用法：
<pre>
BOOL Formula_Calculator(
	CString Buffer_trans_formula,
	double _Materrnal,
	double _Paternal,
	double *Result,
	UINT *Cal_Err);</pre>

* <i> Buffer_trans_formula</i>: 字符串形式的表达式，支持代入M与P两个变量值，表达式支持ln()与lg()，支持使用^表示乘方；
* <i> _Materrnal</i>: 代入M变量的值;
* <i> _Paternal</i>: 代入P变量的值;
* <i> Result</i>: 计算所得结果的指针;
* <i> Cal_Err</i>: 发生错误时弹窗选择值的指针。

<pre>
BOOL Advance_Formula_Compare(
	CString Buffer_formula_compare,
	double A,
	double B,
	BOOL* UPPER);
</pre>

* <i>Buffer_formula_compare</i>: 字符串形式的比较表达式,支持代入M与P两个变量值；
* <i>A</i>: 代入M变量的值;
* <i>B</i>: 代入P变量的值;
* <i>UPPER</i>: 比较结果的指针；

上述函数正常执行均返回TRUE, <i>Advance_Formula_Compare()</i>未经过广泛的测试，可能有bug;

## 3. 联系方式：

Email: gurthpalarran@outlook.com