#pragma once
class SocketWorker
{
private:
	friend class Impl;
	class Impl;
	Impl *impl;
	SocketWorker();
public:
	static SocketWorker& instance();
	~SocketWorker();
	void connectServer();
	void sendMsg(const char*strMsg, size_t len);
	void closeSocket();

	void onConnect(WPARAM wParam, LPARAM lParam);

	void onRead(WPARAM wParam, LPARAM lParam);

	void onWrite(WPARAM wParam, LPARAM lParam);

	void onClose(WPARAM wParam, LPARAM lParam);
};

