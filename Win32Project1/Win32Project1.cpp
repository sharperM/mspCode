// Win32Project1.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Win32Project1.h"
#include <CommCtrl.h>
#include "RockSocket.h"
#include <Vfw.h>
#include <stdio.h>
#include <windowsx.h>
#include <dshow.h>
#include "DStest.h"
#include <iomanip>
#include <sstream>
#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "Quartz.lib")
#pragma comment(lib, "Vfw32.lib")
//#pragma comment(lib, "gdiplus.lib")
//using namespace Gdiplus;
#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	std::ostringstream ostrm;
	ostrm << std::setfill('0') << std::setw(3) << 1;
	std::string s = ostrm.str();
 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

//	ULONG_PTR m_gdiplusToken;
//	GdiplusStartupInput gdiplusStartupInput;
//	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	// 主消息循环: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WIN32PROJECT1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

unsigned long er = 0;
static LRESULT WINAPI FrameCallbackProc(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	unsigned char *data;
	data = lpVHdr->lpData;//获得视频数据首地址，并将数据存入data数组中以便处理
	char temp[128];
	er++;
	sprintf_s(temp, 128, "%ld\n", er);
	OutputDebugString(temp);
	//lpVHdr->dwBufferLength
	//Gdiplus::Bitmap bitmap()

	//lpVHdr->dwBytesUsed;
	//而大小保存在lpVHdr->dwBytesUsed
	return 0;
}

static LRESULT WINAPI FrameCallbackProc2(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	unsigned char *data;
	data = lpVHdr->lpData;//获得视频数据首地址，并将数据存入data数组中以便处理
	char temp[128];
	er++;
	sprintf_s(temp, 128, "2__%ld\n", er);
	OutputDebugString(temp);
	//lpVHdr->dwBufferLength
	//Gdiplus::Bitmap bitmap()

	//lpVHdr->dwBytesUsed;
	//而大小保存在lpVHdr->dwBytesUsed
	return 0;
}

//
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   HWND hotkeyControl = ::CreateWindow(HOTKEY_CLASS, TEXT("test"), WS_VISIBLE|WS_CHILD|WS_BORDER, 0, 0, 590, 230, hWnd, NULL, hInstance, NULL);
   HFONT font = CreateFont(19, 12, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
	   CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("宋体"));
 //  (_In_ int cHeight, _In_ int cWidth, _In_ int cEscapement, _In_ int cOrientation, _In_ int cWeight, _In_ DWORD bItalic,
//	   _In_ DWORD bUnderline, _In_ DWORD bStrikeOut, _In_ DWORD iCharSet, _In_ DWORD iOutPrecision, _In_ DWORD iClipPrecision,
//	   _In_ DWORD iQuality, _In_ DWORD iPitchAndFamily, _In_opt_ LPCWSTR pszFaceName);   
   SendMessage(hotkeyControl, WM_SETFONT, (WPARAM)font, NULL);

   HWND testButton = ::CreateWindow(TEXT("button"), TEXT("test"), WS_VISIBLE | WS_CHILD, 0, 240, 80, 30, hWnd, (HMENU)1, hInstance, NULL);
   HWND startPreviewButton = ::CreateWindow(TEXT("button"), TEXT("start preview"), WS_VISIBLE | WS_CHILD, 90, 240, 80, 30, hWnd, (HMENU)2, hInstance, NULL);
   HWND stopPreviewButton = ::CreateWindow(TEXT("button"), TEXT("stop preview"), WS_VISIBLE | WS_CHILD, 180, 240, 80, 30, hWnd, (HMENU)3, hInstance, NULL);
   HWND testStatic = CreateWindow(_T("STATIC"), "", WS_CHILD | WS_VISIBLE, 0, 300, 250, 250, hWnd, NULL, hInstance, NULL);

   DStest ddtest;
//    HWND hWndC = ddtest.vfwCapture(testStatic);
//    capSetCallbackOnFrame(hWndC, &FrameCallbackProc);
//    capSetCallbackOnVideoStream(hWndC, &FrameCallbackProc2);
   DStest::instance().test2(testStatic);
   //ShowWindow(hWndC, SW_HIDE);


   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   
   return TRUE;
}

//
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case 1:
			if (wmEvent == BN_CLICKED)
			{
				RockSocket::instance();
			}

			break;
		case 2:
				if (wmEvent == BN_CLICKED)
				{
					DStest::instance().StartPreview();
				}
				break;		
		case 3:
				if (wmEvent == BN_CLICKED)
				{
					DStest::instance().StopPreview();
				}

				break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
