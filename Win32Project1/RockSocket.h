#pragma once
class RockSocket
{
public:
	friend class Impl;
	class Impl;	
	RockSocket();
	~RockSocket();

	static RockSocket& instance();

	//����������
	void createServer();

	//���ӷ�����
	void connectSever(DWORD ipaddr, WORD port);

	//
	void connectSever(LPCSTR ipaddr, WORD port);

	//������Ϣ
	void sendData(const char* data, unsigned long len);

	//�ر�����
	void closeSocket(bool bNotify);

	void onAccept(WPARAM wParam, LPARAM lParam);

	void onRead(WPARAM wParam, LPARAM lParam);

	void onWrite(WPARAM wParam, LPARAM lParam);

	void onClose(WPARAM wParam, LPARAM lParam);

private:
	Impl *impl;
};

