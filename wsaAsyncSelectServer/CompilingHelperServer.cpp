#include "stdafx.h"
#include "CompilingHelperServer.h"
#include "RockSocket.h"
#include <xmemory>
#include <map>

using namespace std;
class CompilingHelperServer::Impl :public RockSocketSink
{
public:
	CompilingHelperServer		*_this;
	std::auto_ptr<RockSocket>	pRs;
	map<SOCKET, int>			clients;
public:
	Impl(CompilingHelperServer* ptr)
	{
		_this = ptr;
		pRs.reset(new RockSocket());
		pRs->createServer();
	}
	void onAccept(unsigned long s)
	{
		;
	}
	void onRead(char* buf, size_t len)
	{
		OutputDebugStringA(buf);
	}
	void onclose()
	{

	}
};
CompilingHelperServer::CompilingHelperServer()
{
	impl = new Impl(this);
}


CompilingHelperServer::~CompilingHelperServer()
{
	if (impl)
	{
		delete impl;
	}
}

CompilingHelperServer& CompilingHelperServer::instance()
{
	static CompilingHelperServer singleton;
	return singleton;
}
