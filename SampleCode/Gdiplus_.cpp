#include "stdafx.h"
#include "Gdiplus_.h"
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

class Gdiplus_::Impl
{
public:
	Gdiplus_ 	*_this;
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

public:
	Impl(Gdiplus_ *_this)
	{
		this->_this = _this;

		// Initialize GDI+.
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);// Shutdown
	}
	~Impl()
	{
		GdiplusShutdown(gdiplusToken);
	}
};

Gdiplus_::Gdiplus_()
:impl(nullptr)
{
	impl = new Impl(this);
}

Gdiplus_::~Gdiplus_()
{
	if (impl)
	{
		delete impl;
	}
}
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid){ UINT num = 0; UINT size = 0; ImageCodecInfo* pImageCodecInfo = NULL; GetImageEncodersSize(&num, &size); if (size == 0){ return -1; }pImageCodecInfo = (ImageCodecInfo*)(malloc(size)); if (pImageCodecInfo == NULL){ return -1; }GetImageEncoders(num, size, pImageCodecInfo); for (UINT j = 0; j< num; ++j){ if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0){ *pClsid = pImageCodecInfo[j].Clsid; free(pImageCodecInfo); return j; } }free(pImageCodecInfo); return -1; }
void Gdiplus_::doSomething()
{
	Image* pTemp;
	pTemp = Image::FromFile(L"C:\\Users\\stephen\\Pictures\\DSC02316.jpg");
	pTemp->RotateFlip(Rotate90FlipY);
	CLSID pngClsid;
	GetEncoderClsid(L"image/jpeg", &pngClsid);
	pTemp->Save(L"C:\\Users\\stephen\\Pictures\\DSC02316_rotation.jpg", &pngClsid,NULL);
	delete pTemp;
}


void countu()
{
	 LARGE_INTEGER litmp;
	 LONGLONG QPart1, QPart2;
	 double dfMinus, dfFreq, dfTime;

	 //获得计时器的时钟频率
	 QueryPerformanceFrequency(&litmp);
	 dfFreq = (double)litmp.QuadPart;
	 QueryPerformanceCounter(&litmp);
	 QPart1 = litmp.QuadPart; //开始计时

	 // Block1(); //工作模块 函数等，根据自己需要添加。
// 	 Gdiplus_ m;
// 	 m.doSomething();
	 int i =1;
	  QueryPerformanceCounter(&litmp);
	 QPart2 = litmp.QuadPart; //终止计时
	 dfMinus = (double)(QPart2 - QPart1);//计算计数器值
	 dfTime = dfMinus / dfFreq;//获得对应时间，单位为秒 你可以乘1000000精确到毫秒级（us）

}
