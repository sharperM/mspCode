#pragma once
class RockSocket
{
	RockSocket();
public:
	friend class Impl;
	class Impl;
	~RockSocket();
	static RockSocket& instance();

	void createServer();

	void onAccept(WPARAM wParam, LPARAM lParam);

	void onRead(WPARAM wParam, LPARAM lParam);

	void onWrite(WPARAM wParam, LPARAM lParam);

	void onClose(WPARAM wParam, LPARAM lParam);

private:
	Impl *impl;
};

