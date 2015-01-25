#include "stdafx.h"
#include "misc.h"
#pragma comment (lib,"Gdiplus.lib")
void InitGDIplus()
{
}

GDIplusLib::GDIplusLib()
{
	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

GDIplusLib::~GDIplusLib()
{
	GdiplusShutdown(gdiplusToken);
}

GDIplusLib& GDIplusLib::instance()
{
	static GDIplusLib singleton;
	return singleton;
}


