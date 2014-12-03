// ProgramingWithLua.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <string>
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
using namespace std;
void runLua(string filePath)
{
	//---------------在程序中使用  脚本---------------------
	//1. 初始化Lua虚拟机
	lua_State *lua_state;
	lua_state = luaL_newstate();

	//2.设置待注册的Lua标准库，这个库是给你的Lua脚本用的
	//因为接下来我们只想在Lua脚本里面输出hello world，所以只引入基本库就可以了
	static const luaL_Reg lualibs[] =
	{
		{ "base", luaopen_base },
		{ NULL, NULL }
	};

	//3.注册Lua标准库并清空栈
	const luaL_Reg *lib = lualibs;
	for (; lib->func != NULL; lib++)
	{
		lib->func(lua_state);
		lua_settop(lua_state, 0);
	}

	//4、运行hello.lua脚本  
	luaL_dofile(lua_state, filePath.c_str());

	//5. 关闭Lua虚拟机
	lua_close(lua_state);

}

//C / C++调用Lua函数
int luaAdd(lua_State *lua_state, int x, int y)
{
	int sum;
	//获取lua里面的add函数并把它放到lua的栈顶
	lua_getglobal(lua_state, "add");
	//往lua栈里面压入两个参数
	lua_pushstring(lua_state, "x");
	//lua_pushnumber(lua_state, x);
	lua_pushnumber(lua_state, y);
	//调用lua函数,这里的2是参数的个数，1是返回值的个数
	lua_call(lua_state, 2, 1);
	//从栈顶读取返回值,注意这里的参数是-1
	sum = lua_tointeger(lua_state, -1);
	//最后我们把返回值从栈顶拿掉
	lua_pop(lua_state, 1);
	return sum;
}

void useLuaFun()
{
	//创建Lua虚拟机
	lua_State *lua_state = luaL_newstate();
	//加载所有库
	luaL_openlibs(lua_state);
	//运行Lua脚本
	std::string scriptPath = "add_.lua";
	int status = luaL_loadfile(lua_state, scriptPath.c_str());
	std::cout << " return: " << status << std::endl;
	int result = 0;
	if (status == 0)
	{
		result = lua_pcall(lua_state, 0, LUA_MULTRET, 0);
	}
	else
	{
		std::cout << " Could not load the script." << std::endl;
	}
	std::cout << "4 + 1= " << luaAdd(lua_state, 4, 1) << std::endl;
	lua_close(lua_state);
}

//操作Lua全局变量
void getLuaGlobalVar(lua_State* lua_state)
{
	lua_getglobal(lua_state, "height");
	std::string myname = lua_tostring(lua_state, -1);
	lua_pop(lua_state, 1);
	std::cout << "get height: " << myname << std::endl;

}

//C++里面修改Lua全局变量的值
void setLuaGlobal(lua_State* lua_state,string var)
{
	lua_pushstring(lua_state, "1920");
	lua_setglobal(lua_state, "height");
}

//C++传递Table给Lua
void setLuaTable(lua_State* lua_state)
{
	lua_createtable(lua_state, 2, 0);
	lua_pushnumber(lua_state, 1);
	lua_pushnumber(lua_state, 49);
	//    lua_settable(lua_state, -3);
	lua_rawset(lua_state, -3);
	lua_pushnumber(lua_state, 2);
	lua_pushstring(lua_state, "Life is a beach");
	//    lua_settable(lua_state, -3);
	lua_rawset(lua_state, -3);
	lua_setglobal(lua_state, "arg");
}


//定义C++函数指针
int average(lua_State *L)
{
	// get number of arguments 
	int n = lua_gettop(L);
	double sum = 0;
	int i;
	// loop through each argument 
	for (i = 1; i <= n; i++)
	{
		// total the arguments 
		sum += lua_tonumber(L, i);
	}
	// push the average 
	lua_pushnumber(L, sum / n);
	// push the sum 
	lua_pushnumber(L, sum);
	// return the number of results 
	return 2;
}

//定义C++函数
int displayLuaFunction(lua_State *l)
{
	// number of input arguments
	int argc = lua_gettop(l);
	// print input arguments
	std::cout << "[C++] Function called from Lua with " << argc
		<< " input arguments" << std::endl;
	for (int i = 0; i < argc; i++)
	{
		std::cout << " input argument #" << argc - i << ": "
			<< lua_tostring(l, lua_gettop(l)) << std::endl;
		lua_pop(l, 1);
	}
	// push to the stack the multiple return values
	std::cout << "[C++] Returning some values" << std::endl;
	lua_pushnumber(l, 12);
	lua_pushstring(l, "See you space cowboy");
	// number of return values
	return 2;
}

void useCfunction()
{
	//创建Lua虚拟机
	lua_State *lua_state = luaL_newstate();
	//加载所有库
	luaL_openlibs(lua_state);
	//注册函数
	lua_register(lua_state, "average", average);
	lua_register(lua_state, "displayLuaFunction", displayLuaFunction);
	//运行Lua脚本
	std::string scriptPath = "useCfun.lua";
	int status = luaL_loadfile(lua_state, scriptPath.c_str());
	std::cout << " return: " << status << std::endl;
	int result = 0;
	if (status == 0)
	{//执行
		result = lua_pcall(lua_state, 0, LUA_MULTRET, 0);
	}
	else
	{
		std::cout << " Could not load the script." << std::endl;
	}
	lua_close(lua_state);
}

int _tmain(int argc, _TCHAR* argv[])
{

	runLua("sp.lua");
	useLuaFun();
	useCfunction();
	return 0;
}

