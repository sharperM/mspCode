// SampleCode.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <io.h>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include "Gdiplus_.h"
using namespace std;
#define MAKESTR(a) ((ostringstream&)(ostringstream()<<string("")<<a)).str().c_str()
string MakeClass(string strClassName);
class AA
{
	int a[10];
public:
	virtual  void Said()const{ cout << "A" << endl; }
	virtual  void Said(){ cout << "A" << endl; }
	virtual  void Said(const char* szHello)
	{
		cout << "A:" << szHello << endl;;
	}
	static void Said(const AA &a)
	{
		a.Said();
	}
	virtual  int SaidAA()
	{
		cout << "SaidAA" << endl;
		return 1;
	}
};
class A 
{
	int a[10];
public:
	virtual void Said()const{ cout << "A" << endl; }
	virtual void Said(){ cout << "A" << endl; }
	virtual void Said(const char* szHello)
	{
		cout << "A:" << szHello << endl;;
	}
	static void Said(const A &a)
	{
		a.Said();
	}	
	virtual  int SaidA()
	{
		cout << "SaidA" << endl;
		return 1;
	}
};

class B :virtual public A
{
	unsigned a[10];
public:
	virtual void Said()const{ cout << "B" << endl; }
	virtual void Said(){ cout << "B" << endl; }
	virtual void Said(const char* szHello)
	{
		cout << "B:" << szHello << endl;;
	}
	virtual void donothing()
	{

	}
	virtual  int SaidB()
	{
		cout << "SaidB" << endl;
		return 1;
	}
};
class C :virtual public A
{
	unsigned a[10];
public:
	virtual void Said()const{ cout << "C" << endl; }
	virtual void Said(){ cout << "C" << endl; }
	virtual void Said(const char* szHello)
	{
		cout << "C:" << szHello << endl;;
	}
	virtual  int SaidC()
	{
		cout << "SaidC" << endl;
		return 1;
	}
};

class D :virtual public AA, virtual public B, virtual public C{
public:
	void Said()const{ cout << "D" << endl; }
	void Said(){ cout << "D" << endl; }
	void Said(const char* szHello)
	{
		cout << "D:" << szHello << endl;;
	}
// 	virtual void ____uiui()
// 	{}
	virtual  int SaidD()
	{
		cout << "SaidD" << endl;
		return 1;
	}
};


class E :virtual public D
{
	virtual  int SaidEE()
	{
		cout << "SaidEE" << endl;
		return 1;
	}
};
class v
{
public:
	virtual void Said()const = 0;
};
class _a
{
public:
	unsigned a[1];
	virtual void Said()const{ cout << "_a::Said" << endl; }
	virtual void Said(const char*a){ cout << MAKESTR("_a::Said(const char*) "<<this->a[0]) << endl; }
};
class _b 
{
public:
	unsigned a[1];
	virtual void Said()const{ cout << "_b::Said" << endl; }
	virtual void _Saidb()const{ cout << "_b::_Saidb" << endl; }
};
class _c 
{
public:
	unsigned a[1];
	virtual void Said()const{ cout << "_c::Said" << endl; }
	virtual void _Saidc()const{ cout << "_b::_Saidb" << endl; }
};
class _aa :public _a
{
public:
	unsigned a[1];
	virtual void Said()const{ cout << "_aa::Said" << endl; }
	virtual void Said2()const{ cout << "_aa::Said2" << endl; }
	virtual void Said2(const char*)const{ cout << "_aa::Said2" << endl; }
};

class _aaa :public _aa
{
public:
	unsigned a[1];
	virtual void Said()const{ cout << "_aaa::Said" << endl; }
	virtual void Said2()const{ cout << "_aaa::Said2" << endl; }
	virtual void Said3()const{ cout << "D" << endl; }

};

class _aaaa :public _aaa
{
public:
	unsigned a[1];
	virtual void Said()const{ cout << "_aaa::Said" << endl; }
	virtual void Said2()const{ cout << "_aaaa::Said2" << endl; }
	virtual void Said3()const{ cout << "D" << endl; }
	virtual void Said4()const{ cout << "D" << endl; }

};

class _va :virtual public  _a{
public:
	unsigned a[1];
	virtual void love()
	{}
};
class _aa_a :public _a, public _aa
{
public:
	unsigned a[1];
	virtual void Said()const{ cout << "_aa_a::Said" << endl; }
	virtual void Said2()const{ cout << "_aa_a::Said2" << endl; }

	virtual void s()
	{}
};

class _a_b_c :public _a, public _b, public _c
{
public:
	unsigned a[1];
	virtual void Said()const{ cout << "_a_b_c::Said" << endl; }
	virtual void Said2()const{ cout << "_a_b_c::Said2" << endl; }
	virtual void Said(const char*a){ cout << MAKESTR("_a_b_c::Said(const char*) " << this->a[0]) << endl; }
	virtual void abc()
	{}
};
class _a_b_c_d :public _a_b_c
{
public:
	virtual void abc()
	{
		int i = 0;
	}
	virtual void abcd()
	{}
};
void test(v& vv)
{
	vv.Said();
}
void test2(_a& vv)
{
	vv.Said((const char*)0);
}
class R{};
class Ir1 :public R{ char r1[16]; }; 
class Ir2 :public R{}; 
class Ir3 :public R{}; 
class Ir4 :public R{}; 
class Ir5 :public R{}; 

class C1{ char c1[16]; }; 
class C2{}; class C3{}; 
class C4{}; class C5{}; 
class Ir :public Ir1, public Ir2, public Ir3, public Ir4{};
class Ic :public C1, public C2, public C3, public C4{};

int _tmain(int argc, _TCHAR* argv[])
{
	A t1;
	B t2;
	C t3;
	D t4;
	A& rb = t2;
	A& rc = t3;
//  	A& rd = t4;
	A::Said(t1);
	A::Said(t2);
	A::Said(t3);
//  	A::Said(t4);
	rb.Said("--");
	rc.Said("--");
// 	rd.Said("--");
	cout << sizeof(A) << endl;
	cout << sizeof(B) << endl;
	cout << sizeof(C) << endl;
	cout << sizeof(D) << endl;
	cout << sizeof(E) << endl;
	cout << "--------" << endl;
	E e;
	e.SaidA();
	e.SaidAA();
	e.SaidB();
	e.SaidC();
	e.SaidD();
	cout << "--------" << endl;
	_a base1;
	_b base2;
	_aa base11;
	_aa_a base11_1;
	base11_1.Said2();

	_aaa base111;
	_aaaa base1111;
	base1111.a[0]= 0;
	base1111._aaa::a[0] = 0;
	base1111._aa::a[0] = 0;
	base1111._a::a[0] = 0;

	_aa& r11 = base1111;
	r11.Said2();
	_va basev1;
	cout << sizeof(_a) << endl;
	cout << sizeof(_b) << endl;
	cout << sizeof(_aa) << endl;
	cout << sizeof(_aaa) << endl;
	cout << sizeof(_aa_a) << endl;
	cout << sizeof(_va) << endl;
	cout << "--------" << endl;
	_aa_a  ______a;
	_aa& ra = ______a;
	ra.Said2();
	cout << sizeof(R) << endl;
	cout << sizeof(Ir1) << endl;
	cout << sizeof(Ir2) << endl;
	cout << sizeof(C1) << endl;
	cout << sizeof(C2) << endl;
	cout << "--------" << endl;

	cout << sizeof(Ir) << endl;
	cout << sizeof(Ic) << endl;

	_a_b_c_d abcd;
	_a ia;
	_b ib;
	_c ic;
	test2(ia);
	test2(abcd);
	countu();
	_finddata_t file;
	long lf;
	if (lf = _findfirst("c:\\wait2delete\\*.txt", &file) == -1l)
	{
	}
	else
	{
		remove(MAKESTR("c:\\wait2delete\\" << file.name));
		while (_findnext(lf, &file) == 0)
		{
			remove(MAKESTR("c:\\wait2delete\\"<<file.name));
		}
		_findclose(lf);
	}

	string s;
	ofstream ofs;
	while (cin >> s)
	{
		ofs.open("d:\\1.cpp", ios::out );
		if (ofs.is_open())
		{
			ofs << MakeClass(s);
			ofs.close();
			system("explorer d:\\");
			system("\"c:\\Program Files\\Sublime Text 2\\sublime_text.exe\" D:\\1.cpp");
		}
	}

	
	system("pause");
	return 0;
}

string MakeClass(string strClassName)
{
return	MAKESTR(
"#pragma once\n\
class " << strClassName << "\n\
{\n\
private:\n\
	class Impl;\n\
	friend class Impl;\n\
	Impl*	\timpl;\n\
\n\
public:\n\t" << strClassName << "();\n\
	~"<<strClassName<<"();\n\
};\
\n\
#include \""<<strClassName<<".h\"\n\
class " << strClassName << "::Impl\n\
{\n\
public:\n\t"	<< strClassName<<" \t*_this;\n\
public:\n\
	Impl(" << strClassName<< " *_this)\n\
	{\n\
		this->_this = _this;\n\
	}\n\
	~Impl()\n\
	{\n\
	}\n\
};\n\n"<<	strClassName << "::"<<strClassName<<"()\n\
:impl(NULL)\n\
{\n\
	impl = new Impl(this);\n\
}\n\n" <<	strClassName << "::~" << strClassName << "()\n\
{\n\
	if (impl)\n\
	{\n\
		delete impl;\n\
	}\n\
}\n");
}
#pragma once
class class1
{
private:
	class Impl;
	friend class Impl;
	Impl*	impl;

public:
	class1();
	~class1();
	/* data */
};

//#include "class1.h"
class class1::Impl
{
public:
	class1*	_this;
public:
	Impl(class1* _this)
	{
		this->_this = _this;
	}
	~Impl()
	{

	}
};

class1::class1()
:impl(NULL)
{
	impl = new Impl(this);
}

class1::~class1()
{
	if (impl)
	{
		delete impl;
	}
}

