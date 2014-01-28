#pragma once
class RockSocket
{
	RockSocket();
public:
	friend class Impl;
	class Impl;
	static RockSocket& instance();
	~RockSocket();
	void createServer();
private:
	Impl *impl;
};

