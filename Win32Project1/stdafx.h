// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//
#if !defined(AFX_STDAFX_H__08E7CDC6_7F58_11D2_8CF1_00A0C9441E20__INCLUDED_)
#define AFX_STDAFX_H__08E7CDC6_7F58_11D2_8CF1_00A0C9441E20__INCLUDED_

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
#include <windows.h>
// C 运行时头文件
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
// TODO:  在此处引用程序需要的其他头文件
#endif // !defined(AFX_STDAFX_H__08E7CDC6_7F58_11D2_8CF1_00A0C9441E20__INCLUDED)

#define MAKESTR(a) ((ostringstream&)(ostringstream()<<string()<<a)).str().c_str()