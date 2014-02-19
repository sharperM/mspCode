#pragma once
class RockSocket
{
public:
	friend class Impl;
	class Impl;	
	RockSocket();
	~RockSocket();

	static RockSocket& instance();

	//创建服务器
	void createServer();

	//连接服务器
	void connectSever(DWORD ipaddr, WORD port);

	//
	void connectSever(LPCSTR ipaddr, WORD port);

	//发送消息
	void sendData(const char* data, unsigned long len);

	//关闭连接
	void closeSocket(bool bNotify);

	void onAccept(WPARAM wParam, LPARAM lParam);

	void onRead(WPARAM wParam, LPARAM lParam);

	void onWrite(WPARAM wParam, LPARAM lParam);

	void onClose(WPARAM wParam, LPARAM lParam);

private:
	Impl *impl;
};

