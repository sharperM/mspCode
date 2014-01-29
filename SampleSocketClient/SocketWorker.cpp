#include "stdafx.h"
#include "SocketWorker.h"
#include <winsock2.h>
#include <string>
#include <vector>
#pragma comment (lib, "Ws2_32.lib")

class SocketWorker::Impl{
public:
	SocketWorker		*_this;

	WSADATA				wsd;

	SOCKET				sClient;

	std::vector<SOCKET> sClients;

	int					nCount;

	struct sockaddr_in	server;

	struct hostent		*host;

	char          szBuffer[2048];

	Impl(SocketWorker *_this)
	{
		this->_this = _this;
		host = NULL;
		sClient = INVALID_SOCKET;
	}

	bool initSocketlib()
	{
		return (WSAStartup(MAKEWORD(2, 2), &wsd) != 0);
	}

	bool releaseSocketlib()
	{
		return WSACleanup() == 0;
	}

	bool createSocket()
	{
		if (sClient == INVALID_SOCKET)
		{
			sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			return !(sClient == INVALID_SOCKET);
		}
		return true;
	}

	bool makeSocketAddress(int iPort, char* szServer)
	{
		server.sin_family = AF_INET;

		server.sin_port = htons(iPort);

		server.sin_addr.s_addr = inet_addr(szServer);

		if (server.sin_addr.s_addr == INADDR_NONE)
		{

			host = gethostbyname(szServer);

			if (host == NULL)
			{
				return false;
			}
			CopyMemory(&server.sin_addr, host->h_addr_list[0], host->h_length);
		}
		return true;
	}

	bool connectServer(  )
	{
		return !(connect(sClient, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR);
	}

	int sendMsg(const char * strMsg)
	{
		return send(sClient, strMsg, strlen(strMsg), 0);
	}

	bool closeSocket_()
	{
		if (sClient != INVALID_SOCKET)
		{
			if (closesocket(sClient) == 0)
			{
				sClient = INVALID_SOCKET;
				return true;
			}
			return false;
		}
		return true;
	}

	bool recviceMsg()
	{
		int ret = recv(sClient, szBuffer, 2048, 0);

		if (ret == 0)        // Graceful close

		{

			printf("It is a graceful close!\n");

		}

		else if (ret == SOCKET_ERROR)

		{

			printf("recv() failed with error code %d\n", WSAGetLastError());

		}

		szBuffer[ret] = '\0';

		printf("recv() is OK. Received %d bytes: %s\n", ret, szBuffer);
		OutputDebugStringA(std::string(szBuffer).append("\n").c_str());
		setWindowtest(std::string(szBuffer).append("\n").c_str());
		return true;
	}

	void setWindowtest(const char* str)
	{
		HWND hStaticBox = ::GetDlgItem(AfxGetMainWnd()->m_hWnd, IDC_STATIC);
		SetWindowTextA(hStaticBox, str);
	}


	void creatSockets(DWORD nCount)
	{
		SOCKET sTemp = INVALID_SOCKET;
		for (; sClients.size() < nCount; )
		{
			sTemp = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sTemp != INVALID_SOCKET)
			{
				sClients.push_back(sTemp);
				sTemp = INVALID_SOCKET;
			}
		}
	}

	void connectServerEx()
	{
		for (int i = 0, c = sClients.size(); i < c; ++i)
		{
			connect(sClients[i], (struct sockaddr *)&server, sizeof(server));
		}
	}
	
	void closeSocketEX_()
	{
		for (int i = 0, c = sClients.size(); i < c; ++i)
		{
			closesocket(sClients[i]);
		}
		sClients.clear();
	}
};
SocketWorker::SocketWorker()
{
	impl = new Impl(this);
	impl->initSocketlib();
	impl->makeSocketAddress(5150, "localhost"/*"192.168.10.5"*/);
}


SocketWorker::~SocketWorker()
{
	if (impl)
	{
		impl->closeSocket_();
		impl->releaseSocketlib();
		delete impl;
	}
}

SocketWorker& SocketWorker::instance()
{
	static SocketWorker singleton;
	return singleton;
}

void SocketWorker::connectServer()
{
	impl->createSocket();
	impl->connectServer();
}

void SocketWorker::sendMsg(const char *strMsg)
{
	impl->sendMsg(strMsg);
	impl->recviceMsg();
}

void SocketWorker::closeSocket()
{
	impl->closeSocket_();
}

void SocketWorker::test(int nCount)
{
	impl->creatSockets(nCount);
	impl->connectServerEx();
	impl->closeSocketEX_();
}
