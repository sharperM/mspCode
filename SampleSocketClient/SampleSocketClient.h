
// SampleSocketClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSampleSocketClientApp: 
// �йش����ʵ�֣������ SampleSocketClient.cpp
//

class CSampleSocketClientApp : public CWinApp
{
public:
	CSampleSocketClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSampleSocketClientApp theApp;