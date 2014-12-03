// ProgramingWithLua.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//---------------�ڳ�����ʹ��  �ű�---------------------
	//1. ��ʼ��Lua�����
	lua_State *lua_state;
	lua_state = luaL_newstate();

	//2.���ô�ע���Lua��׼�⣬������Ǹ����Lua�ű��õ�
	//��Ϊ����������ֻ����Lua�ű��������hello world������ֻ���������Ϳ�����
	static const luaL_Reg lualibs[] =
	{
		{ "base", luaopen_base },
		{ NULL, NULL }
	};

	//3.ע��Lua��׼�Ⲣ���ջ
	const luaL_Reg *lib = lualibs;
	for (; lib->func != NULL; lib++)
	{
		lib->func(lua_state);
		lua_settop(lua_state, 0);
	}

	//4������hello.lua�ű�  
	luaL_dofile(lua_state, filePath.c_str());

	//5. �ر�Lua�����
	lua_close(lua_state);

}

//C / C++����Lua����
int luaAdd(lua_State *lua_state, int x, int y)
{
	int sum;
	//��ȡlua�����add�����������ŵ�lua��ջ��
	lua_getglobal(lua_state, "add");
	//��luaջ����ѹ����������
	lua_pushstring(lua_state, "x");
	//lua_pushnumber(lua_state, x);
	lua_pushnumber(lua_state, y);
	//����lua����,�����2�ǲ����ĸ�����1�Ƿ���ֵ�ĸ���
	lua_call(lua_state, 2, 1);
	//��ջ����ȡ����ֵ,ע������Ĳ�����-1
	sum = lua_tointeger(lua_state, -1);
	//������ǰѷ���ֵ��ջ���õ�
	lua_pop(lua_state, 1);
	return sum;
}

void useLuaFun()
{
	//����Lua�����
	lua_State *lua_state = luaL_newstate();
	//�������п�
	luaL_openlibs(lua_state);
	//����Lua�ű�
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

//����Luaȫ�ֱ���
void getLuaGlobalVar(lua_State* lua_state)
{
	lua_getglobal(lua_state, "height");
	std::string myname = lua_tostring(lua_state, -1);
	lua_pop(lua_state, 1);
	std::cout << "get height: " << myname << std::endl;

}

//C++�����޸�Luaȫ�ֱ�����ֵ
void setLuaGlobal(lua_State* lua_state,string var)
{
	lua_pushstring(lua_state, "1920");
	lua_setglobal(lua_state, "height");
}

//C++����Table��Lua
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


//����C++����ָ��
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

//����C++����
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
	//����Lua�����
	lua_State *lua_state = luaL_newstate();
	//�������п�
	luaL_openlibs(lua_state);
	//ע�ắ��
	lua_register(lua_state, "average", average);
	lua_register(lua_state, "displayLuaFunction", displayLuaFunction);
	//����Lua�ű�
	std::string scriptPath = "useCfun.lua";
	int status = luaL_loadfile(lua_state, scriptPath.c_str());
	std::cout << " return: " << status << std::endl;
	int result = 0;
	if (status == 0)
	{//ִ��
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

