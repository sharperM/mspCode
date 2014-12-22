
// SampleSocketClientDlg.h : ͷ�ļ�
//

#pragma once

// CSampleSocketClientDlg �Ի���
class CSampleSocketClientDlg : public CDialogEx
{
// ����
public:
	CSampleSocketClientDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CSampleSocketClientDlg(){};

// �Ի�������
	enum { IDD = IDD_SAMPLESOCKETCLIENT_DIALOG };

	protected:
		class Impl;
		friend class Impl;
		Impl		*impl;
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
