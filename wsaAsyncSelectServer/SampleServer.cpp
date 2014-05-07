#include "stdafx.h"
#include "SampleServer.h"
#include "RockSocket.h"
#include <xmemory>
#include <map>
#include <iostream>

using namespace std;
class SampleServer::Impl :public RockSocketSink
{
public:
	SampleServer			*_this;
	auto_ptr<RockSocket>	pRs;
	map<SOCKET, int>		clients;
public:
	Impl(SampleServer* ptr)
	{
		_this = ptr;
		pRs.reset(new RockSocket());
		pRs->createServer();
		pRs->setSink(this);
	}
	void onAccept(unsigned long s)
	{
		;
	}
	void onRead(char* buf, size_t len)
	{
		cout << buf << endl;
	}
	void onclose()
	{

	}
};
SampleServer::SampleServer()
{
	impl = new Impl(this);
}


SampleServer::~SampleServer()
{
	if (impl)
	{
		delete impl;
	}
}

SampleServer& SampleServer::instance()
{
	static SampleServer singleton;
	return singleton;
}
