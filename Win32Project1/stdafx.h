// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//
#if !defined(AFX_STDAFX_H__08E7CDC6_7F58_11D2_8CF1_00A0C9441E20__INCLUDED_)
#define AFX_STDAFX_H__08E7CDC6_7F58_11D2_8CF1_00A0C9441E20__INCLUDED_

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�: 
#include <windows.h>
// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#define WM_GETIPMSG (WM_USER + 1)
#define WM_SOCKET_NOTIFY (WM_USER + 2)
#include "dshowutil.h"
#include <assert.h>

#include <sstream>
#include <string>
using namespace std;
// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#endif // !defined(AFX_STDAFX_H__08E7CDC6_7F58_11D2_8CF1_00A0C9441E20__INCLUDED)

#define MAKESTR(a) ((ostringstream&)(ostringstream()<<string()<<a)).str().c_str()