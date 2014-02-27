// wsaAsyncSelectServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "RockSocket.h"

int _tmain(int argc, _TCHAR* argv[])
{
	RockSocket rs;
	rs.createServer();
	while (true)
	{
		;
	}
	return 0;
}

