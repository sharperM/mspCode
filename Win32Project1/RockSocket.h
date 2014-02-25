#pragma once



class RockSocketSink
{
public:
	virtual void onAccept(){ ; }
	virtual void onRead(){ ; }
	virtual void onclose(){ ; }
};
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

	//������Ϣ
	void sendData(const char* data, unsigned long len);

	//�ر�����
	void closeSocket(bool bNotify);

	void onAccept(WPARAM wParam, LPARAM lParam);

	void onRead(WPARAM wParam, LPARAM lParam);

	void onWrite(WPARAM wParam, LPARAM lParam);

	void onClose(WPARAM wParam, LPARAM lParam);

	LRESULT wndProc(UINT uMsg, LPARAM lParam, WPARAM wParam);

	void setSink(RockSocketSink* ptr);
private:
	Impl *impl;
};

