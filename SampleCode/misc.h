#pragma once
#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include <atldef.h>
#include <atlconv.h>
#include <atlalloc.h>

using namespace Gdiplus;

class GDIplusLib
{
private:
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GDIplusLib();
public:
	~GDIplusLib();
	static GDIplusLib& instance();
};

