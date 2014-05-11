// SampleCode.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <io.h>
#include <sstream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	_finddata_t file;
	long lf;
	if (lf = _findfirst("c:\\wait2delete\\*.txt", &file) == -1l)
	{
	}
	else
	{
		ostringstream ostrm;
		ostrm << "c:\\wait2delete\\" << file.name;
 		remove(ostrm.str().c_str());
		while (_findnext(lf, &file) == 0)
		{
			ostringstream ostrm;
			ostrm << "c:\\wait2delete\\" << file.name;
 			remove(ostrm.str().c_str());
		}
		_findclose(lf);
	}

	return 0;
}

