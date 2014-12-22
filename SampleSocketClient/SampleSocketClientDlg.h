
// SampleSocketClientDlg.h : 头文件
//

#pragma once

// CSampleSocketClientDlg 对话框
class CSampleSocketClientDlg : public CDialogEx
{
// 构造
public:
	CSampleSocketClientDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CSampleSocketClientDlg(){};

// 对话框数据
	enum { IDD = IDD_SAMPLESOCKETCLIENT_DIALOG };

	protected:
		class Impl;
		friend class Impl;
		Impl		*impl;
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSendMsg();
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedCloseSocket();
	afx_msg void OnBnClickedOk();
};
