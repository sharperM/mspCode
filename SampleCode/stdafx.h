// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")

class GDIplusLib
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GDIplusLib();
public:
	~GDIplusLib();
	static GDIplusLib& instance();
};
// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
