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
	void sendMsg(const char *strMsg);
	void closeSocket();

	void test(int nCount);
};

