// ProgramingWithLua.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "UsingLua.h"

#include <iostream>
using namespace std;

#include <iostream>
#include "luabind/luabind.hpp"

#ifdef _DEBUG
#pragma comment(lib,"libluabindd.lib")
#else
#pragma comment(lib,"libluabind.lib")
#endif // _DEBUG


void greet()
{
	std::cout << "hello world!\n";
}

extern "C" int init(lua_State* L)
{
	using namespace luabind;

	open(L);

	module(L)
		[
			def("greet", &greet)
		];
	return 0;
}
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
