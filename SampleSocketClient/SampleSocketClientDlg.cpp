
// SampleSocketClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SampleSocketClient.h"
#include "SampleSocketClientDlg.h"
#include "afxdialogex.h"
#include "SocketWorker.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSampleSocketClientDlg 对话框



CSampleSocketClientDlg::CSampleSocketClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSampleSocketClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSampleSocketClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSampleSocketClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSampleSocketClientDlg::OnBnClickedSendMsg)
	ON_BN_CLICKED(IDC_BUTTON2, &CSampleSocketClientDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_BUTTON3, &CSampleSocketClientDlg::OnBnClickedCloseSocket)
	ON_BN_CLICKED(IDOK, &CSampleSocketClientDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSampleSocketClientDlg 消息处理程序

BOOL CSampleSocketClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSampleSocketClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSampleSocketClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSampleSocketClientDlg::OnBnClickedSendMsg()
{
	// TODO:  在此添加控件通知处理程序代码
	CString m_value;
	HWND hEditbox = ::GetDlgItem(m_hWnd, IDC_EDIT1);
	GetDlgItemText(IDC_EDIT1, m_value);
	if (m_value.GetLength()>0)
	{
		USES_CONVERSION;
		CHAR* pAnsiString = W2A(m_value.GetString());
		SocketWorker::instance().sendMsg(pAnsiString/*"hello socket"*/,strlen(pAnsiString)+1);
	}
}


void CSampleSocketClientDlg::OnBnClickedConnect()
{
	// TODO:  在此添加控件通知处理程序代码
	SocketWorker::instance().connectServer();
}


void CSampleSocketClientDlg::OnBnClickedCloseSocket()
{
	// TODO:  在此添加控件通知处理程序代码
	SocketWorker::instance().closeSocket();
}

void CSampleSocketClientDlg::OnBnClickedOk()
{
	OnBnClickedSendMsg();
}
