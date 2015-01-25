// ProgramingWithLua.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "UsingLua.h"

#include <iostream>
using namespace std;

#include <iostream>
#include "luabind/luabind.hpp"
#include <cmath>
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

class PP
{
public:
	string str;
	int x;
	int y;
	int z;
	string strone;
	string strtwo;
	string strthree;
	string strfour;
	string strfive;
	string strsix;
	string strseven;
	string streight;
	string strnine;
	int weight;
	int hight;
	int dsx;
	int dsy;
	int dsw;
	int dsh;
	int r;
	int ang;

};

class Scene
{
	string name;
public:
	Scene()
	{}	
	Scene(const string& name)
		:name(name)
	{}

	void createObject(int obj);

	void printObjects();

	void loadMap(std::string fileName);

	//void testFunc(PP x);
	int testFunc(PP x);

private:
	std::vector<int> mObjects;
};
void Scene::createObject(int obj)
{
	mObjects.push_back(obj);
}
void Scene::printObjects()
{
	for (auto iter = mObjects.begin(); iter != mObjects.end(); ++iter)
		std::cout << (*iter) << std::endl;
}
class testclass
{
public:
	testclass(const std::string& s) : m_string(s) {}
	void print_string() { std::cout << m_string << "\n"; }

private:
	std::string m_string;
};
void Scene::loadMap(std::string fileName)
{
	try
	{
		using namespace luabind;

		lua_State* L = lua_open();
		luaL_openlibs(L);
		luabind::open(L);
		// 导出类Scene  
		module(L)
			[
				// 导出的类名字不必与C++中的一样  
				// 方法也是这样  
				// 但是为了看着方便  
				// 我让它们名称都一样了  
				class_<Scene>("Scene")
				.def(luabind::constructor<>())
				.def(luabind::constructor<const std::string&>())
				.def("createObject", &Scene::createObject)
				.def("printObject", &Scene::printObjects)
				.def("testFunc",&Scene::testFunc)
				, class_<PP>("PP")
				.def(luabind::constructor<>())
				.def_readwrite("x", &PP::x)
				.def_readwrite("y", &PP::y)
				.def_readwrite("z", &PP::z)
				.def_readwrite("str", &PP::str)
				.def_readwrite("strone", &PP::strone)
				.def_readwrite("strtwo", &PP::strtwo)
				.def_readwrite("strthree", &PP::strthree)
				.def_readwrite("strfour", &PP::strfour)
				.def_readwrite("strfive", &PP::strfive)
				.def_readwrite("strsix", &PP::strsix)
				.def_readwrite("strseven", &PP::strseven)
				.def_readwrite("streight", &PP::streight)
				.def_readwrite("strnine", &PP::strnine)
				, def("sin", &sin)
				,class_<testclass>("testclass")
				.def(constructor<const std::string&>())
				.def("print_string", &testclass::print_string)
				// 注意到我并没有导出loadMap()方法  
			];
		// 加载lua文件  
		luaL_dofile(L, fileName.c_str());

		// 调用lua文件中的createScene方法  
// 		luabind::call_function<void>(L, "createScene", this);
// 		luabind::call_function<void>(L, "tepp",this);
}
	catch (luabind::error& e)
	{
		std::cout << e.what() << std::endl;
	}
}



int Scene::testFunc(PP x)
{
	cout << x.x << endl;
	cout << x.y << endl;
	cout << x.z << endl;
	cout << x.str << endl;
	cout << x.strone << endl;
	cout << x.strtwo << endl;
	cout << x.strthree << endl;
	cout << x.strfour << endl;
	cout << x.strfive << endl;
	cout << x.strsix << endl;
	cout << x.strseven << endl;
	cout << x.streight << endl;
	cout << x.strnine << endl;
	return x.x;
}

int _tmain(int argc, _TCHAR* argv[])
{
	Scene s;
	s.loadMap("test.lua");
	s.printObjects();
	system("pause");

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
