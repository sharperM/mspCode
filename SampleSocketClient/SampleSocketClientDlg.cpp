
// SampleSocketClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SampleSocketClient.h"
#include "SampleSocketClientDlg.h"
#include "afxdialogex.h"
#include "SocketWorker.h"
#include <string>
#include <xmemory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSampleSocketClientDlg �Ի���

class CSampleSocketClientDlg::Impl
{
public:
	Impl(CSampleSocketClientDlg * _this);
	~Impl();
	CSampleSocketClientDlg * _this;
	std::auto_ptr<SocketWorker> pSW;
private:

};

CSampleSocketClientDlg::Impl::Impl(CSampleSocketClientDlg * _this)
{
	this->_this = _this;
	pSW.reset(new SocketWorker);
}

CSampleSocketClientDlg::Impl::~Impl()
{
}

CSampleSocketClientDlg::CSampleSocketClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSampleSocketClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	impl = new Impl(this);

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


// CSampleSocketClientDlg ��Ϣ�������

BOOL CSampleSocketClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSampleSocketClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSampleSocketClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSampleSocketClientDlg::OnBnClickedSendMsg()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString m_value;
	HWND hEditbox = ::GetDlgItem(m_hWnd, IDC_EDIT1);
	GetDlgItemText(IDC_EDIT1, m_value);
	if (m_value.GetLength()>0)
	{
		USES_CONVERSION;
		CHAR* pAnsiString = W2A(m_value.GetString());
		impl->pSW->sendMsg(pAnsiString/*"hello socket"*/,strlen(pAnsiString)+1);
	}
}


void CSampleSocketClientDlg::OnBnClickedConnect()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	impl->pSW->connectServer();
}


void CSampleSocketClientDlg::OnBnClickedCloseSocket()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	impl->pSW->closeSocket();
}

void CSampleSocketClientDlg::OnBnClickedOk()
{
	OnBnClickedSendMsg();
}

CSampleSocketClientDlg::~CSampleSocketClientDlg()
{
	if (impl)
	{
		delete impl;
	}
	
}
