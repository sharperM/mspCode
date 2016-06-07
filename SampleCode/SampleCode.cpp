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
	wnd.lpszClassName = TEXT("MainWClass");
	wnd.lpfnWndProc = DefWindowProc;
	wnd.hInstance = GetModuleHandle(NULL);
	int result = RegisterClassEx(&wnd);

	HDC    hdc = GetDC(CreateWindow(TEXT(""), TEXT(""),
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
	::CreateProcessA(scrtextfile.c_str(), NULL,
		NULL, NULL, true, 0,
		NULL, NULL, (LPSTARTUPINFOA)&si, &pi);
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
	hmouse = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)mouseProc, GetModuleHandle(TEXT("KeyMonitor.dll")), 0);//挂上鼠标钩子  
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

using namespace Gdiplus;

// HRESULT SaveImage(BITMAPINFO *pbi, void *pBits, LPCTSTR pszFileName, LONG lQuality)
// {
// 	assert(pbi != NULL && pBits != NULL);
// 	assert(pszFileName != NULL);
// 	if (pbi == NULL || pBits == NULL || pszFileName == NULL)
// 		return E_FAIL;
// 
// 	GUID guidImageFormat = Gdiplus::ImageFormatJPEG;
// 
// 	Gdiplus::EncoderParameters eps;
// 	eps.Count = 1;
// 	eps.Parameter[0].Guid = Gdiplus::EncoderQuality;
// 	eps.Parameter[0].NumberOfValues = 1;
// 	eps.Parameter[0].Type = Gdiplus::EncoderParameterValueTypeLong;
// 	eps.Parameter[0].Value = &lQuality;
// 
// 	UINT nEncoders;
// 	UINT nBytes;
// 	Gdiplus::Status status;
// 
// 	status = Gdiplus::GetImageEncodersSize(&nEncoders, &nBytes);
// 	if (status != Gdiplus::Ok)
// 	{
// 		return (E_FAIL);
// 	}
// 
// 	USES_CONVERSION_EX;
// 	Gdiplus::ImageCodecInfo* pEncoders = static_cast<Gdiplus::ImageCodecInfo*>(_ATL_SAFE_ALLOCA(nBytes, _ATL_SAFE_ALLOCA_DEF_THRESHOLD));
// 
// 	status = Gdiplus::GetImageEncoders(nEncoders, nBytes, pEncoders);
// 	if (status != Gdiplus::Ok)
// 	{
// 		return(E_FAIL);
// 	}
// 
// 	CLSID clsidEncoder = CLSID_NULL;
// 	// Determine clsid from file type
// 	for (UINT iCodec = 0; iCodec < nEncoders; iCodec++)
// 	{
// 		if (pEncoders[iCodec].FormatID == guidImageFormat)
// 		{
// 			clsidEncoder = pEncoders[iCodec].Clsid;
// 			break;
// 		}
// 	}
// 
// 	if (clsidEncoder == CLSID_NULL)
// 	{
// 		return(E_FAIL);
// 	}
// 
// 	LPCWSTR pwszFileName = T2CW_EX(pszFileName, _ATL_SAFE_ALLOCA_DEF_THRESHOLD);
// 
// 	Gdiplus::Bitmap bm(pbi, pBits);
// 	status = bm.Save(pwszFileName, &clsidEncoder, &eps);
// 	if (status != Gdiplus::Ok)
// 	{
// 		return(E_FAIL);
// 	}
// 
// 	return S_OK;
// }

void testEraseVector()
{
	vector<int> v;
	v.assign(10, 0);
	for (unsigned i = 0; i < 10; ++i)
	{
		v[i] = i+1000;
	}
	for (vector<int>::iterator it = v.begin(); it != v.end();)
	{
		if (*it == 1000 || *it == 1005)
		{
			it = v.erase(it);
		}
		else
		{
			it++;
		}
	}
	cout << v.size() << endl;
	for each (auto i in v)
	{
		cout << i << endl;
	}
}
#include "_md5/md5.h"

void PrintMD5(const string& str, MD5& md5) {
	cout << "MD5(\"" << str << "\") = " << md5.toString() << endl;
}

string FileDigest(const string& file) {

	ifstream in(file.c_str(), ios::binary);
	if (!in) {
		return "";
	}

	MD5 md5;
	std::streamsize length;
	char buffer[1024];
	while (!in.eof()) {
		in.read(buffer, 1024);
		length = in.gcount();
		if (length > 0) {
			md5.update(buffer, length);
		}
	}
	in.close();
	return md5.toString();
}

void testMD5()
{
	{
		MD5 md5;
		md5.update(ifstream("D:\\Program Files\\lk78\\userface\\41364145.jpg", ios::binary));
		//md5.update(buffer, size);
		PrintMD5("", md5);
	}


	MD5 md5;
	md5.update("");
	PrintMD5("", md5);

	md5.update("a");
	PrintMD5("a", md5);

	md5.update("bc");
	PrintMD5("abc", md5);

	md5.update("defghijklmnopqrstuvwxyz");
	PrintMD5("abcdefghijklmnopqrstuvwxyz", md5);

	md5.reset();
	md5.update("message digest");
	PrintMD5("message digest", md5);
}

//for (unsigned char left = 0x81; left < 0x82; left++)
//{
//	for (unsigned char right = 0x40; right < 0xff; right++)
//	{
//		char s[] = { (char)left, (char)right, (char)0x00 };  //GB18030

//		if (right != 0x7f)
//		{
//			vector<utf8_char> v = Utf16LEToUtf8(&(GB18030ToUtf16LE((uint8_t *)s))[0]);

//			char * x = (char *)&v[0];
//			int l = v.size();

//			ss = COMBINE_WSTR(ss << L" ma  0x" << std::hex << (unsigned char)(s[0]) << (unsigned char)(s[1]) << L", 0x");
//			for (size_t i = 0; i < v.size() - 1; i++)
//			{
//				ss = COMBINE_WSTR(ss << std::hex << (unsigned char)v[i]);
//			}
//			ss = COMBINE_WSTR(ss << L"\r\n");
//		}

//	}
//}


//
#include <stdint.h>
vector<wchar_t> GB18030ToUtf16LE(const uint8_t * s)
{
	int nUtf16WChars = ::MultiByteToWideChar(54936, 0, (char *)s, -1, NULL, 0);
	vector<wchar_t> utf16WChars(nUtf16WChars);
	::MultiByteToWideChar(54936, 0, (char *)s, -1, &utf16WChars[0], (int32_t)utf16WChars.size());
	return utf16WChars;
}
typedef unsigned char utf8_char;
vector<utf8_char> Utf16LEToUtf8(const wchar_t * s)
{
	int nUtf8Chars = ::WideCharToMultiByte(CP_UTF8, 0, s, -1, NULL, 0, NULL, NULL);
	vector<utf8_char> utf8Chars(nUtf8Chars);
	::WideCharToMultiByte(CP_UTF8, 0, s, -1, (char *)&utf8Chars[0], nUtf8Chars, NULL, NULL);
	return utf8Chars;
}
#include <sstream>
#define COMBINE_WSTR(s) (((std::wostringstream&)(std::wostringstream()<<std::wstring() << s)).str().c_str())
#define  MAKESTR(s) (((std::ostringstream&)(std::ostringstream()<<std::string() << s)).str().c_str())

void GBK_Utf8()
{
	wstring ss;
	int nIndex = 0;
	//FILE * pFile;
	//pFile = fopen("D:/Program Files/lk780000/LKRoom/lkplaza/dist.config", "w+");
	FILE * pFile;
	FILE * pFile2;
	string _ss = MAKESTR("{}");
	pFile = fopen("myfile.bin", "wb");
	pFile2 = fopen("myfile2.bin", "wb");

	if (pFile == NULL)
	{
		return;
	}
	for (unsigned char left = 0x81; left < 0xff; left++)
	{
		for (unsigned char right = 0x40; right < 0xff; right++)
		{
			char s[] = { (char)left, (char)right, (char)0x00 };  //GB18030

			if (right != 0x7f)
			{
				vector<utf8_char> v = Utf16LEToUtf8(&(GB18030ToUtf16LE((uint8_t *)s))[0]);

				char * x = (char *)&v[0];
				int l = v.size();
				if (l != 4)
				{
					if (l == 3)
						ss = L"2";
					else
					{
						ss = L"1";
						
					}
					ss = L"";
				}

				string _s;
				if (v.size() == 3)
				{
					_s = _s + "0x00,";
				}
				for (size_t i = 0; i <v.size()-1 ; i++)
				{
					_s = MAKESTR(_s << "0x" << std::hex << (int)v[v.size() - 2- i] << ",");
					if (v[i]<16)
					{
						continue;
					}
				}
		
				//_s = _s + ",";
				fwrite(_s.c_str(), sizeof(char), _s.length(), pFile);

				nIndex = nIndex + 1;
			}

		}
	}
	fclose(pFile);
	fclose(pFile2);




	for (unsigned char left = 0x81; left < 0xff; left++)
	{
		for (unsigned char right = 0x40; right < 0xff; right++)
		{
			if (right != 0x7f)
			{
				char s[] = { (char)left, (char)right, (char)0x00 };
				vector<utf8_char> v = Utf16LEToUtf8(&(GB18030ToUtf16LE((uint8_t *)s))[0]);
				unsigned int index = (left - 0x81)*(0xff - 0x40 - 1) + right - 0x40-(right>0x7f?1:0);
				string _s = "";
				for (size_t i = 0; i < v.size() - 1; i++)
				{
					_s = MAKESTR(_s << std::hex << (int)v[i]);
				}
				//if (_s != MAKESTR(std::hex << utf8[index]))
				//{
				//	continue;
				//}
			}
		}
	}
	int adfadf = 1;
}

#include"gbk2utf8map.h"

string GBK2utf8(const char* inbuf)
{
	unsigned int length = strlen(inbuf);
	char* outbuf = new char[length * 3 / 2 + 1];
	ZeroMemory(outbuf, length * 3 / 2+1);
	unsigned char* p = NULL;
	unsigned int offset = 0;
	for (size_t i = 0; i < length; )
	{
		p = (unsigned char*)inbuf+i;
		unsigned char left = *p, right = *(p + 1);
		if (left < 0x80)//asnii 0~127
		{
			memcpy_s(outbuf + offset, 1, &left, 1);
			offset++;
			i++;
		}
		else if(left == 0xff || right<0x40 || right == 0x7f || right == 0xff)
		{
			return "";
		}
		else //gbk
		{
			unsigned int index = (left - 0x81)*(0xff - 0x40 - 1) + right - 0x40 - (right>0x7f ? 1 : 0);
			byte copysize = utf8_2[index * 3+2] > 0x00 ? 3 : 2;
			memcpy_s(outbuf + offset, copysize, ((unsigned char*)(utf8_2 + 3 * index)), copysize);
			offset = offset + copysize;
			i = i + 2;
		}
		
	}
	string ret = (outbuf);
	delete(outbuf);
	return ret;
}
int _tmain(int argc, _TCHAR* argv[])
{
	int tint[2] = { 0xffffff00, 0xff000000 };
	byte tttt[8] = { 0 };
	memcpy_s(tttt, 8, &tint, 8);

	//GBK_Utf8();
	string allGBKChar;
	for (byte down = 0x81; down < 0xfe; down++)
	{
		for (byte up = 0x40; up < 0xff; up++)
		{
			if (up != 0x7f)
			{
				allGBKChar.append(1,(char)down);
				allGBKChar.append(1,(char)up);
			}
		}
	}
	allGBKChar.append("\0");
	char sssssss1[8] = { 0x61, 0x81, 0x41, 0x63, 0x81, 0x42, 0x62, 0x0 };
	string sssssss2 = GBK2utf8(allGBKChar.c_str());
	//string sssssss2 = GBK2utf8(sssssss1);

	vector<utf8_char> v = Utf16LEToUtf8(&(GB18030ToUtf16LE((uint8_t *)allGBKChar.c_str()))[0]);
	string sssssss3 = (char*)v.data();
	if (sssssss2 == sssssss3)
	{

		return 1;
	}
	else
	{
		return 1;
	}
	testMD5();


	GDIplusLib::instance();
	Gdiplus::Image a(L"");
	testEraseVector();
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
	HookMouseLLMessage();

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