// ProgramingWithLua.cpp : �������̨Ӧ�ó������ڵ㡣
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

class Scene
{
public:
	Scene()
	{}

	void createObject(int obj);

	void printObjects();

	void loadMap(std::string fileName);

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

void Scene::loadMap(std::string fileName)
{
	try
	{
		using namespace luabind;

		lua_State* L = lua_open();
		luaL_openlibs(L);
		luabind::open(L);
		// ������Scene  
		module(L)
			[
				// �����������ֲ�����C++�е�һ��  
				// ����Ҳ������  
				// ����Ϊ�˿��ŷ���  
				// �����������ƶ�һ����  
				class_<Scene>("Scene")
				.def(luabind::constructor<>())
				.def("createObject", &Scene::createObject)
				.def("printObject", &Scene::printObjects)
				// ע�⵽�Ҳ�û�е���loadMap()����  
			];
		// ����lua�ļ�  
		luaL_dofile(L, fileName.c_str());
		// ����lua�ļ��е�createScene����  
		luabind::call_function<void>(L, "createScene", this);
	}
	catch (luabind::error& e)
	{
		std::cout << e.what() << std::endl;
	}
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
