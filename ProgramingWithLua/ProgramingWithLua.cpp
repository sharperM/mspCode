// ProgramingWithLua.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "UsingLua.h"

#include <iostream>
using namespace std;
//////////////////////////////////////////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
 	runLua("sp.lua");
// 	useLuaFun();
// 	useCfunction();
	testStack();
	cout << "--1-----" << endl;
	BindingClass();
	cout << "--2-----" << endl;
	BindingClass2();
	BindingClass3();
	system("pause");
 	return 0;
}
