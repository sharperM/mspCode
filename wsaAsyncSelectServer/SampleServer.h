#pragma once
class SampleServer
{
private:
	friend class Impl;
	class Impl;
	Impl *impl;

private:
	SampleServer();
public:
	~SampleServer();
	static  SampleServer& instance();
};
