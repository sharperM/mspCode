#pragma once
class RockSocket
{
	RockSocket();
public:
	friend class Impl;
	class Impl;
	~RockSocket();
	static RockSocket& instance();

	//创建服务器
	void createServer();

	void onAccept(WPARAM wParam, LPARAM lParam);

	void onRead(WPARAM wParam, LPARAM lParam);

	void onWrite(WPARAM wParam, LPARAM lParam);

	void onClose(WPARAM wParam, LPARAM lParam);

	//链接服务器
	void connectSever(DWORD ipaddr, WORD port);
private:
	Impl *impl;
};

