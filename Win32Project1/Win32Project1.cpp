// Win32Project1.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Win32Project1.h"
#include <CommCtrl.h>
#include "RockSocket.h"
#include <objidl.h>
#include <gdiplus.h>
#include <sstream>
#define  MAKESTR(s) (((std::ostringstream&) (std::ostringstream("") << s)).str().c_str())

#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������: 
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

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WIN32PROJECT1, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32PROJECT1));

	// ����Ϣѭ��: 
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	GdiplusShutdown(gdiplusToken);
	return (int) msg.wParam;
}



//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW ;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= 0/* LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32PROJECT1))*/;
	wcex.hCursor = NULL;// LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.lpszMenuName = NULL;// MAKEINTRESOURCE(IDC_WIN32PROJECT1);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm = NULL;// LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//

float size = 1;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
   //WINUSERAPI HWND WINAPI CreateWindowExA( _In_ DWORD dwExStyle, _In_opt_ LPCSTR lpClassName,
   //_In_opt_ LPCSTR lpWindowName, _In_ DWORD dwStyle, _In_ int X, _In_ int Y, _In_ int nWidth, _In_ int nHeight, _In_opt_ HWND hWndParent, _In_opt_ HMENU hMenu, _In_opt_ HI
   hWnd = CreateWindowEx(WS_EX_TRANSPARENT, szWindowClass, szTitle, WS_VISIBLE | WS_POPUP,
	   1000, 100, 630, 893, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }
   HWND hotkeyControl = ::CreateWindow(HOTKEY_CLASS, TEXT("test"), WS_VISIBLE|WS_CHILD, 0, 0, 200, 24, hWnd, NULL, hInstance, NULL);
   HFONT font = CreateFont(12, 6, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
	   CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("����"));
 //  (_In_ int cHeight, _In_ int cWidth, _In_ int cEscapement, _In_ int cOrientation, _In_ int cWeight, _In_ DWORD bItalic,
//	   _In_ DWORD bUnderline, _In_ DWORD bStrikeOut, _In_ DWORD iCharSet, _In_ DWORD iOutPrecision, _In_ DWORD iClipPrecision,
//	   _In_ DWORD iQuality, _In_ DWORD iPitchAndFamily, _In_opt_ LPCWSTR pszFaceName);   
   SendMessage(hotkeyControl, WM_SETFONT, (WPARAM)font, NULL);

   HWND testButton = ::CreateWindow(TEXT("button"), TEXT("��test"), WS_VISIBLE | WS_CHILD, 0, 24, 80, 24, hWnd, (HMENU)1, hInstance, NULL);
//    SendMessage(testButton, WM_SETFONT, (WPARAM)font, NULL);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   
   return TRUE;
}

void onPaint(HDC hdc)
{
	Graphics graphics(hdc);
	Image image(L"nice.jpg");
	graphics.DrawImage(&image, 0, 0, image.GetWidth() / 2, image.GetHeight() / 2);
	Pen opaquePen(Color(255, 0, 0, 255), 15);
	Pen semiTransPen(Color(128, 0, 0, 255), 15);
	graphics.DrawLine(&opaquePen, 0, 40, 100, 40);
	graphics.DrawLine(&semiTransPen, 0, 60, 100, 60);
	graphics.SetCompositingQuality(CompositingQualityGammaCorrected);
	graphics.DrawLine(&semiTransPen, 0, 80, 100, 80);
	if (FALSE)
	{
		Bitmap        bitmap(L"beatuiful.jpg");
		UINT          width = bitmap.GetWidth();
		UINT          height = bitmap.GetHeight();
		CachedBitmap  cBitmap(&bitmap, &graphics);
		int           j, k;
		for (j = 0; j < 300; j += 10)
			for (k = 0; k < 100; ++k)
				graphics.DrawImage(&bitmap, j, j / 2, width / 10, height / 10);
		for (j = 0; j < 300; j += 10)
			for (k = 0; k < 100; ++k)
				graphics.DrawCachedBitmap(&cBitmap, j, 150 + j / 2);
	}
}

//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	static HDC hdc;
	static HDC hdcMem;
	Bitmap bitmap(TEXT("icon.jpg"));
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
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
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
	{

		break;
	}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  �ڴ���������ͼ����...
		{
			onPaint(hdc);

		}
		
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_NCHITTEST:
	{
		int y = GET_Y_LPARAM(lParam);
		int x = GET_X_LPARAM(lParam);
		POINT m_point;
		m_point.x = x;
		m_point.y
			= y;
		ScreenToClient(hWnd, &m_point);
		if (m_point.y<20)
		{
			SetCursor(LoadCursor(NULL, IDC_HAND));
			return HTCAPTION;
		}
		
		break;
	}
	case WM_NCMOUSEHOVER:
	{
		break;
	}
	case WM_NCMOUSEMOVE:
	{
		break;
	}
	case WM_MOUSEMOVE:
	{
		if (wParam == MK_LBUTTON)
		{
			if (HIWORD(lParam) < 20)
			{
				MSG msg;
				msg.hwnd = hWnd;
				msg.lParam = lParam;
				msg.message = WM_NCHITTEST;
				DispatchMessage(&msg);
			}
		}
		break;

		
	}
	case  WM_MOUSEWHEEL:
	{
		int i = 0;
		i =
		HIWORD(wParam);
		//((std::ostringstream&) (std::ostringstream("") << 1)).str().c_str();
		OutputDebugStringA(MAKESTR("HIWORD: "<<i<<std::endl));
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
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
