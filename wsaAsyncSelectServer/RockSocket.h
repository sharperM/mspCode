#pragma once

// typedef  unsigned int WPARAM;
// typedef  unsigned int LPARAM;
// typedef  unsigned int UINT;

class RockSocketSink
{
public:
	virtual void onSocketAccept(){ ; }
	virtual void onSocketRead(){ ; }
	virtual void onSocketclose(){ ; }
};
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

	//发送消息
	void sendData(const char* data, unsigned long len);

	//关闭连接
	void closeSocket(bool bNotify);

	void onAccept(WPARAM wParam, LPARAM lParam);

	void onRead(WPARAM wParam, LPARAM lParam);

	void onWrite(WPARAM wParam, LPARAM lParam);

	void onClose(WPARAM wParam, LPARAM lParam);

	LRESULT wndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void setSink(RockSocketSink* ptr);
private:
	Impl *impl;
};

