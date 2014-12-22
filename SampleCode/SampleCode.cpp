// SampleCode.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <io.h>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
#include <set>
// #include <objidl.h>
// #include <gdiplus.h>
// using namespace Gdiplus;
// #pragma comment (lib,"Gdiplus.lib")
#include <GL/gl.h>
#include <GL/GLU.h>
#pragma comment (lib,"opengl32.lib")
#pragma comment (lib,"glu32.lib")
#include "json\json.h"
#include <atlconv.h>
using namespace std;

void GetOpenGLVersion()
{
	HGLRC  hglrc;
	WNDCLASSEX wnd;
	memset(&wnd, 0, sizeof(wnd));
	wnd.cbSize = sizeof(wnd);
	wnd.lpszClassName = "MainWClass";
	wnd.lpfnWndProc = DefWindowProc;
	wnd.hInstance = GetModuleHandle(NULL);
	int result = RegisterClassEx(&wnd);

	HDC    hdc = GetDC(CreateWindow("", "",
		WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL | WS_MINIMIZEBOX, 0, 0, 0, 0,
		0, 0, GetModuleHandle(NULL), 0));
	PIXELFORMATDESCRIPTOR pfd;

	SetPixelFormat(hdc, 1, &pfd);
	// create a rendering context  
	hglrc = wglCreateContext(hdc);

	// make it the calling thread's current rendering context 
	wglMakeCurrent(hdc, hglrc);

	// call OpenGL APIs as desired ... 

	// when the rendering context is no longer needed ...   
	char a[200];
	sprintf_s(a, 200, "OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
	printf(a);
	// make the rendering context not current  
	wglMakeCurrent(NULL, NULL);

	// delete the rendering context  
	wglDeleteContext(hglrc);
}

void GetWindowsVersion()
{
	OSVERSIONINFO osvi;
	BOOL bIsWindowsXPorLater;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osvi);

	bIsWindowsXPorLater =
		((osvi.dwMajorVersion > 5) ||
		((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion >= 1)));

	if (bIsWindowsXPorLater)
		printf("The system meets the requirements.\n");
	else printf("The system does not meet the requirements.\n");

}

void GetWindowsVersion2()
{
	DWORD dwVersion = 0;
	DWORD dwMajorVersion = 0;
	DWORD dwMinorVersion = 0;
	DWORD dwBuild = 0;

	dwVersion = ::GetVersion();

	// Get the Windows version.

	dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

	// Get the build number.

	if (dwVersion < 0x80000000)
		dwBuild = (DWORD)(HIWORD(dwVersion));

	printf("Version is %d.%d (%d)\n",
		dwMajorVersion,
		dwMinorVersion,
		dwBuild);
}

void GetSystemInfo()
{
	SYSTEM_INFO siSysInfo;

	// Copy the hardware information to the SYSTEM_INFO structure. 

	GetSystemInfo(&siSysInfo);

	// Display the contents of the SYSTEM_INFO structure. 

	printf("Hardware information: \n");
	printf("  OEM ID: %u\n", siSysInfo.dwOemId);
	printf("  Number of processors: %u\n",
		siSysInfo.dwNumberOfProcessors);
	printf("  Page size: %u\n", siSysInfo.dwPageSize);
	printf("  Processor type: %u\n", siSysInfo.dwProcessorType);
	printf("  Minimum application address: %lx\n",
		siSysInfo.lpMinimumApplicationAddress);
	printf("  Maximum application address: %lx\n",
		siSysInfo.lpMaximumApplicationAddress);
	printf("  Active processor mask: %u\n",
		siSysInfo.dwActiveProcessorMask);
}

void DeleteFile(const std::string& strPath, const std::string& strFileType)
{
	string fileFullPath = strPath+"\\"+strFileType;
	_finddata_t file;
	long lf;
	if (lf = _findfirst(fileFullPath.c_str()/*"c:\\wait2delete\\*.txt"*/, &file) == -1l)
	{
	}
	else
	{
		ostringstream ostrm;
		ostrm << strPath << file.name;
		remove(ostrm.str().c_str());
		while (_findnext(lf, &file) == 0)
		{
			ostringstream ostrm;
			ostrm << strPath << file.name;
			remove(ostrm.str().c_str());
		}
		_findclose(lf);
	}
	
}

void GetFileRaw(string &filename, vector<char> &s)
{
	USES_CONVERSION;
	ifstream fs = ifstream(filename.c_str());//A2W(filename.c_str()));
	if (fs.is_open())
	{
		//s.at(0);
		fs.seekg(0, ios::end);
		int flen = (int)fs.tellg();
		fs.seekg(0, ios::beg);
		s.resize(++flen, 0);
		fs.read(&s[0], flen);
	}
	fs.close();
}

void ExecuteCmdline(string &scrtextfile)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	ofstream fs = ofstream(scrtextfile.c_str(), fstream::out | ios::binary);
	fs << "ping www.baidu.com\r\nping www.163.com\r\n";
	fs.close();
	::CreateProcess(scrtextfile.c_str(), NULL,
		NULL, NULL, true, 0,
		NULL, NULL, &si, &pi);
}

void JsonTestCode()
{
	vector<char> vec1;
	//string sss = vec1.data();

	Json::Value v1, v2;
	Json::Reader().parse("{\"wo\":12}", v1);
	bool isb2 = false;
	bool isb1 = false;
	if (Json::Reader().parse("{\"unsigned int\":3000000000}", v2))
	{
		isb2 = v2["unsigned int"].isUInt();
		isb1 = v2["unsigned int"].isInt();
	}
	Json::Value v3;
	v3["unsigned int"] = Json::UInt(10);
	string stroutput;
	stroutput =
		v3.toStyledString();


	Json::Value vCrash;
	if (Json::Reader().parse("                                {\"\":null}mjlkjlkjlkj;l", vCrash))
	{
		if (vCrash.isObject())
		{
			vCrash["1231"];
		}
	}

}

HHOOK hmouse;

LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	//call back  
	if (nCode < 0)//若code〈0，则直接调用callnexthookex返回  
		return CallNextHookEx(hmouse, nCode, wParam, lParam);
	else if (nCode == HC_ACTION)
	{
		if (wParam == WM_MOUSEMOVE)
		{//离开主窗体   &&((PMOUSEHOOKSTRUCT)lParam)->hwnd!=main->Handle  
			int x = ((PMSLLHOOKSTRUCT)lParam)->pt.x;
			int y = ((PMSLLHOOKSTRUCT)lParam)->pt.y;
			printf("x = %d, y = %d \r\n", x, y);
		}
	}
	return CallNextHookEx(hmouse, nCode, wParam, lParam);

}
void HookMouseLLMessage()
{
	hmouse = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)mouseProc, GetModuleHandle("KeyMonitor.dll"), 0);//挂上鼠标钩子  
	if (!hmouse)
	{
		DWORD error = GetLastError();
		printf("SetWindowsHookEx()_ error code %d\r\n", error);
	}

}


void referenceTest()
{
	struct sd
	{
		int i;
		char a[5];
	};
	sd a1 = { 0 };
	sd& a2 = a1;
	memset(&a2, 255, sizeof(a2));
	cout << "" << endl;

	set<int*> m_int;
	int n100[100] = {0};
	for (unsigned i = 0; i < 100; ++i)
	{
		m_int.insert(&(n100[i]));
		n100[i] = i;
	}
	if (m_int.begin() == m_int.find(&n100[0]))
	{
	}
	;
	
}

void tryCrash()
{
	const char* text[3] = { 0 };
	text[0] = "好高兴";
	text[1] = "好高兴_1";
	text[2] = "好高兴_2";

	const char* t = text[-1231];


	system("pause");
}
int _tmain(int argc, _TCHAR* argv[])
{
	tryCrash();
	referenceTest();
	GetOpenGLVersion();
	GetWindowsVersion2();
	GetWindowsVersion();
	GetSystemInfo();
	JsonTestCode();
	string scrtextfile = "F:\\Documents and Settings\\Administrator\\桌面\\测试\\";
	ExecuteCmdline(scrtextfile+"test_scr.bat");
	vector<char> s;
	GetFileRaw(scrtextfile + "gameoption.ini", s);
	DeleteFile("c:\\wait2delete\\","*.txt");
	//HookMouseLLMessage();



	MSG msg;
	BOOL bRet;

	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		if (bRet == -1)
		{
			// Handle Error
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}