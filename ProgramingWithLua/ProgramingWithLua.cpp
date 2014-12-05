// ProgramingWithLua.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <string>
#ifdef __cplusplus
extern "C" {
#endif
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#ifdef __cplusplus
}
#endif
using namespace std;


void error(lua_State *L, const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}

void load(lua_State *L, const char *fname, int *w, int *h) {
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run config. file: %s", lua_tostring(L, -1));
	lua_getglobal(L, "width");
	lua_getglobal(L, "height");
	if (!lua_isnumber(L, -2))
		error(L, "'width' should be a number\n");
	if (!lua_isnumber(L, -1))
		error(L, "'height' should be a number\n");
	*w = lua_tointeger(L, -2);
	*h = lua_tointeger(L, -1);
}

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
// 	const luaL_Reg *lib = lualibs;
// 	for (; lib->func != NULL; lib++)
// 	{
// 		lib->func(lua_state);
// 		lua_settop(lua_state, 0);
// 	}
	//ע�� ��׼�⣨math,Bitwise,table,string,io,operating system,debug��
	luaL_openlibs(lua_state);
	//4������hello.lua�ű�  
	luaL_dofile(lua_state, filePath.c_str());
	//��ȡȫ�ֱ���
	int w = 0, h = 0;
	load(lua_state, "RECT.lua", &w, &h);
	cout << "width = " << w << ", height = "<<h << endl;
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
	//���� ��׼�⣨math,Bitwise,table,string,io,operating system,debug��
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
	//���� ��׼�⣨math,Bitwise,table,string,io,operating system,debug��
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
public:
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

static int l_Foo_index(lua_State* L)
{
	Foo* pT = *(Foo**)lua_topointer(L, 1);

	if (strcmp(lua_tostring(L, 2), "m_name") == 0)
	{
		lua_pushstring(L, pT->m_name.c_str());
	}
	else if (strcmp(lua_tostring(L, 2), "dosomething") == 0)
	{
		lua_pushcfunction(L, l_Foo_dosomething);
	}
	else
	{
		return 0;
	}
	return 1;
}

static int l_Foo_newindex(lua_State* L)
{
	Foo* pT = *(Foo**)lua_topointer(L, 1);

	if (strcmp(lua_tostring(L, 2), "m_name") == 0)
	{
		pT->m_name = (string)lua_tostring(L, 3);
	}
	return 0;
}

void RegisterFoo(lua_State* L)
{
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


void RegisterFoo2(lua_State* L)
{
	lua_pushcfunction(L, l_Foo_constructor);    // ע�����ڴ������ȫ�ֺ���
	lua_setglobal(L, "Foo");

	luaL_newmetatable(L, "Foo");           // ����һ��Ԫ��

	lua_pushstring(L, "__gc");               // ��������
	lua_pushcfunction(L, l_Foo_destructor);
	lua_settable(L, -3);                     // �����������õ�ʵ�־��ڴ˰�

	lua_pushstring(L, "__index");
	lua_pushcfunction(L, l_Foo_index);
	lua_settable(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, l_Foo_newindex);
	lua_settable(L, -3);

	lua_pop(L, 1);
}
void BindingClass()
{
	//1������Lua�����
	lua_State *lua_state = luaL_newstate();
	//2������ ��׼�⣨math,Bitwise,table,string,io,operating system,debug��
	luaL_openlibs(lua_state);

	//3
	RegisterFoo2(lua_state);

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

//����ջ �� ��ӡ�� ����
static void stackDump(lua_State *L) {
	int i;
	int top = lua_gettop(L); /* depth of the stack */
	for (i = 1; i <= top; i++) { /* repeat for each level */
		int t = lua_type(L, i);
		switch (t) {
		case LUA_TSTRING: { /* strings */
							  printf("'%s'", lua_tostring(L, i));
							  break;
		}
		case LUA_TBOOLEAN: { /* booleans */
							   printf(lua_toboolean(L, i) ? "true" : "false");
							   break;
		}
		case LUA_TNUMBER: { /* numbers */
							  printf("%g", lua_tonumber(L, i));
							  break;
		}
		default: { /* other values */
					 printf("%s", lua_typename(L, t));
					 break;
		}
		}
		printf(" "); /* put a separator */
	}
	printf("\n"); /* end the listing */
}

//����  ��ջ ����
void testStack()
{
	cout << "-----------------" << endl;
	lua_State *L = luaL_newstate();
	lua_pushboolean(L, 1);
	lua_pushnumber(L, 10);
	lua_pushnil(L);
	lua_pushstring(L, "hello");
	stackDump(L);
	/* true 10 nil 'hello' */
	lua_pushvalue(L, -4); stackDump(L);//���� ָ������ֵ��ջ�� ��������ջ����ʼ�� ������ջ�׿�ʼ��
	/* true 10 nil 'hello' true */
	lua_replace(L, 3); stackDump(L);//��ջ��ֵ�滻ָ������ֵ ����ջ
	/* true 10 true 'hello' */
	lua_settop(L, 6); stackDump(L);//����ջ�ĸ߶ȣ���ȣ�
	/* true 10 true 'hello' nil nil */
	lua_remove(L, -3); stackDump(L);//�Ƴ�ָ��������ֵ
	/* true 10 true nil nil */
	lua_settop(L, -5); stackDump(L);//����ջ�ĸ߶ȣ���ȣ� ������ʾ����
	/* true */
	lua_close(L);
}




////////////////////ʹ�� ��//////////////////////////////////////////////////////
class CTest
{
public:
	CTest()
	{
		m_x = 1234;
		cout << "construct" << endl;
	}
	virtual ~CTest()
	{
		cout << "deconstruct" << endl;
	}

	int Add(int x, int y)
	{
		return x + y;
	};
	int m_x;
};

static int CreateCTest(lua_State* L)
{
	// ����һ��Ԫ��ΪCTest��Table����Lua����
	CTest** pData = (CTest**)lua_newuserdata(L, sizeof(CTest*));
	*pData = new CTest();
	luaL_getmetatable(L, "CTest");
	lua_setmetatable(L, -2);
	return 1;
}

static int DestoryCTest(lua_State* L)
{
	// �ͷŶ���
	delete *(CTest**)lua_topointer(L, 1);
	return 0;
}

static int CallAdd(lua_State* L)
{
	// ����C++�෽�������庯����
	CTest* pT = *(CTest**)lua_topointer(L, 1);
	lua_pushnumber(L, pT->Add(lua_tonumber(L, 2), lua_tonumber(L, 3)));
	return 1;
}

static int lua_index(lua_State* L)
{
	CTest* pT = *(CTest**)lua_topointer(L, 1);

	if (strcmp(lua_tostring(L, 2), "m_x") == 0)
	{
		lua_pushnumber(L, pT->m_x);
	}
	else if (strcmp(lua_tostring(L, 2), "Add") == 0)
	{
		lua_pushcfunction(L, CallAdd);
	}
	else
	{
		return 0;
	}
	return 1;
}

static int lua_newindex(lua_State* L)
{
	CTest* pT = *(CTest**)lua_topointer(L, 1);

	if (strcmp(lua_tostring(L, 2), "m_x") == 0)
	{
		pT->m_x = (int)lua_tonumber(L, 3);
	}
	return 0;
}


void BindingClass2()
{
	lua_State *L = lua_open();
	luaopen_base(L);

	// ��lua��ע����
	lua_pushcfunction(L, CreateCTest);    // ע�����ڴ������ȫ�ֺ���
	lua_setglobal(L, "CTest");

	luaL_newmetatable(L, "CTest");           // ����һ��Ԫ��

	lua_pushstring(L, "__gc");               // ��������
	lua_pushcfunction(L, DestoryCTest);
	lua_settable(L, -3);                     // �����������õ�ʵ�־��ڴ˰�

	lua_pushstring(L, "__index");
	lua_pushcfunction(L, lua_index);
	lua_settable(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcfunction(L, lua_newindex);
	lua_settable(L, -3);

	lua_pop(L, 1);

	luaL_dofile(L, "LuaUseClass.lua");

	lua_close(L);

}

//////////////////////////////////////////////////////////////////////////

int _tmain(int argc, _TCHAR* argv[])
{
 	runLua("sp.lua");
// 	useLuaFun();
// 	useCfunction();
	testStack();
	BindingClass2();

 	return 0;
}

