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
	//lua_pushstring(lua_state, "x");
	lua_pushnumber(lua_state, x);
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
int displayLuaFunction(lua_State *L)
{
	// number of input arguments
	int argc = lua_gettop(L);
	// print input arguments
	std::cout << "[C++] Function called from Lua with " << argc
		<< " input arguments" << std::endl;
	for (int i = 0; i < argc; i++)
	{
		std::cout << " input argument #" << argc - i << ": "
			<< lua_tostring(L, lua_gettop(L)) << std::endl;
		lua_pop(L, 1);
	}
	// push to the stack the multiple return values
	std::cout << "[C++] Returning some values" << std::endl;
	lua_pushnumber(L, 12);
	lua_pushstring(L, "See you space cowboy");
	// number of return values
	return 2;
}

//ʹ�� table ��Ϊ���� �� ����ֵ
int l_map(lua_State *L) {
	int i, n;

	int argc = lua_gettop(L);

	/* 1st argument must be a table (t) */
	luaL_checktype(L, 1, LUA_TTABLE);

	/* 2nd argument must be a function (f) */
	luaL_checktype(L, 2, LUA_TFUNCTION);

	n = luaL_getn(L, 1);  /* get size of table */

	for (i = 1; i <= n; i++) {
		lua_pushvalue(L, 2);   /* push f */
		lua_rawgeti(L, 1, i);  /* push t[i] */
		lua_call(L, 1, 1);     /* call f(t[i]) */
		lua_rawseti(L, 1, i);  /* t[i] = result */
	}
	return 0;  /* no results */
}





void useCfunction()
{
	//����Lua�����
	lua_State *lua_state = luaL_newstate();
	//�������п�
	luaL_openlibs(lua_state);
	//ע���Զ��庯��
	lua_register(lua_state, "average", average);
	lua_register(lua_state, "displayLuaFunction", displayLuaFunction);
	lua_register(lua_state, "l_map", l_map);
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

// �� c++ ��
class Foo
{
	string  m_name;
public:
	Foo(const char* name){ 
		if (name)
		{
			m_name = name;
		}
		std::cout << "foo born." << std::endl; 
	}
	~Foo(){ std::cout << "foo dead." << std::endl; }
	void dosomething()
	{
		std::cout << "foo is alive." << std::endl;
	}
};

int l_Foo_constructor(lua_State* L)
{
	const char* name = luaL_checkstring(L,1);

	Foo** udata = (Foo**)lua_newuserdata(L, sizeof(Foo*));

	*udata = new Foo(name);

	return 1;
}

Foo * l_CheackFoo(lua_State* L, int n)
{
	return *(Foo **)luaL_checkudata(L, n, "luaL_Foo");
}

int l_Foo_dosomething(lua_State* L)
{
	Foo *foo = l_CheackFoo(L, 1);
	foo->dosomething();
	return 0;
}

int l_Foo_destructor(lua_State* L)
{
	Foo * foo = l_CheackFoo(L, 1);
	delete foo;
	return 0;
}

void RegisterFoo(lua_State* L)
{
//  	luaL_Reg sFooRegs[] = 
//  	{
//  		{ "new",l_Foo_constructor },
//  		{ "dosomething",l_Foo_dosomething },
//  		{ "__gc", l_Foo_destructor },
//  		{NULL,NULL}
//  	};
// 	luaL_newmetatable(L, "Foo");
// 	luaL_register(L, NULL, sFooRegs);
// 	lua_setfield(L, -1, "__index");
// 	lua_setglobal(L, "Foo");

	//1: new methods talbe for L to save functions  
    lua_newtable(L);
    int methodtable = lua_gettop(L);

    //2: new metatable for L to save "__index" "__newindex" "__gc" "__metatable" ...  
    luaL_newmetatable(L, "luaL_Foo");
    int metatable = lua_gettop(L);

    //3: metatable["__metatable"] = methodtable  
    lua_pushliteral(L, "__metatable");
    lua_pushvalue(L, methodtable);
    lua_settable(L, metatable);  // hide metatable from Lua getmetatable()  

    //4: metatable["__index"] = methodtable  
    lua_pushliteral(L, "__index");
    lua_pushvalue(L, methodtable);
    lua_settable(L, metatable);

    //5: metatable["__gc"] = gc_animal   
    lua_pushliteral(L, "__gc");
	lua_pushcfunction(L, l_Foo_destructor);
    lua_settable(L, metatable);

    lua_pop(L, 1);  // drop metatable  

    //6: for objct:  
    // name == 0 set object function to "methods"  
    //eg:Animal a = Animal("xx");  
    //a:func() in this "methods" table;  
	//luaL_openlib(L, 0, methods, 0);  // fill methodtable  
		
	lua_register(L, "new", l_Foo_constructor);
	lua_register(L, "dosomething", l_Foo_dosomething);
	lua_register(L, "__gc", l_Foo_destructor);
	lua_pop(L, 1);  // drop methodtable  

    //7.1: for Class:  
    //name = "classname" , so this set Class function to "methods_f"  
    //eg:Animal a = Animal:creat("xx");  
    //Animal:creat() in this "methods_f" tables;  
	//luaL_openlib(L, className.c_str(), methods_f, 0);  

    //7.2: for Class:  
    //add global function "Classname", so we Animal() is a global function now  
    //Animal a = Animal("xx");  
    //function Animal()in lua will call "creat" in C++  

}

void BindingClass()
{
	//1������Lua�����
	lua_State *lua_state = luaL_newstate();
	//2���������п�
	luaL_openlibs(lua_state);

	//3
	RegisterFoo(lua_state);

	//4������hello.lua�ű�  
	std::string scriptPath = "class.lua";
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

	//5. �ر�Lua�����
	lua_close(lua_state);

}

int _tmain(int argc, _TCHAR* argv[])
{
 	runLua("sp.lua");
// 	useLuaFun();
// 	useCfunction();
	BindingClass();
	return 0;
}

