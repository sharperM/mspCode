// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// SampleCode.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

// TODO:  �� STDAFX.H ��
// �����κ�����ĸ���ͷ�ļ����������ڴ��ļ�������

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
