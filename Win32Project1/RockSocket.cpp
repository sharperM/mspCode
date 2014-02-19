#include "stdafx.h"
#include "RockSocket.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "json\json.h"
#include <ios>
#include <fstream>
#include <string>
using namespace std;
#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define PORT 5150

#define DEFAULT_BUFFER 2048
#define DATA_BUFSIZE 8192



// typedef definition

enum SocketStat{SSinvalid,SSinit,SSconnet,SSclose};

enum enSocketStat{ SSNoConnect, SSConnecting, SSconneted,  };


typedef struct _SOCKET_INFORMATION {

	BOOL RecvPosted;

	CHAR Buffer[DATA_BUFSIZE];

	WSABUF DataBuf;

	SOCKET Socket;

	DWORD BytesSEND;

	DWORD BytesRECV;

	struct _SOCKET_INFORMATION *Next;

} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class RockSocketException
{
public:
	string msg;
	RockSocketException();
	~RockSocketException();
	RockSocketException(const string& msg);

private:

};

RockSocketException::RockSocketException()
{
}

RockSocketException::RockSocketException(const string& msg)
{
	this->msg = msg;
}

RockSocketException::~RockSocketException()
{
}

class RockSocket::Impl
{
private:
	RockSocket					* _this;
	static LPSOCKET_INFORMATION SocketInfoList;
	SocketStat					m_stat;
	HWND						workWnd;
	char						serverAddr[256];
	HOSTENT						serverhost;
	vector<char>				vecSendbuf;
public:
	Impl(RockSocket *_this)
	{
		this->_this = _this;
		m_stat = SSinvalid;
	}

	void loadConfig(string & strPath)
	{
		std::ifstream  f(strPath.c_str(), ios::binary | ios::in);
		Json::Value v;
		if (f.is_open())
		{
			if (Json::Reader().parse(f, v))
			{
				if (v["string"].isArray())
				{
					for (size_t i = 0; i < v["string"].size(); i++)
					{
						if (v["string"][i].isString())
						{
							OutputDebugStringA(v["string"][i].asString().append("\n").c_str());
						}
					}
				}
			}
		}
	}

	int InitSocket()throw (RockSocketException)
	{
		if (m_stat != SSinvalid) return 0;
		WSADATA wsaData;
		int iResult;
		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			throw RockSocketException();
			//printf("WSAStartup failed with error: %d\n", iResult);
			return 1;
		}
		return 0;
	}

	int setListenAddr(string& strPort, struct addrinfo **ppresult) throw(RockSocketException)
	{
		int iResult;
		struct addrinfo hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		iResult = getaddrinfo(NULL, strPort.c_str()/*DEFAULT_PORT*/, &hints, ppresult);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			throw RockSocketException();

// 			WSACleanup();
			return 1;
		}
		return 0;
	}

	int createServer()
	{
		if (m_stat == SSinvalid)
		{		
			SOCKADDR_IN InternetAddr;
			struct addrinfo *result = NULL;
			int iResult;
			SOCKET ListenSocket = INVALID_SOCKET;

			try
			{
				InitSocket();
				setListenAddr(string(DEFAULT_PORT), &result);

				// Create a SOCKET for connecting to server
				ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
				if (ListenSocket == INVALID_SOCKET) {
	// 				printf("socket failed with error: %ld\n", WSAGetLastError());
					freeaddrinfo(result);
					throw RockSocketException();
				}
				workWnd = MakeWorkerWindow();

				WSAAsyncSelect(ListenSocket, workWnd, WM_SOCKET_NOTIFY, FD_ACCEPT | FD_CLOSE);

				InternetAddr.sin_family = AF_INET;
				InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
				InternetAddr.sin_port = htons(atoi(DEFAULT_PORT));

				bind(ListenSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr));

				freeaddrinfo(result);

				iResult = listen(ListenSocket, SOMAXCONN);
				if (iResult == SOCKET_ERROR) {
					//printf("listen failed with error: %d\n", WSAGetLastError());
					closesocket(ListenSocket);
					throw RockSocketException();
				}
				m_stat = SSinit;
			}
			catch (RockSocketException )
			{
				WSACleanup();
				m_stat = SSinvalid;
				return 1;
			}
			
		}
		return 0;
	}

	HWND MakeWorkerWindow(void)
	{
		WNDCLASS wndclass;
		TCHAR *ProviderClass = _T("AsyncSelect");
		HWND Window;

		wndclass.style = CS_HREDRAW | CS_VREDRAW |WS_VISIBLE;
		wndclass.lpfnWndProc = (WNDPROC)WindowProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = NULL;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = (LPCSTR)ProviderClass;

		if (RegisterClass(&wndclass) == 0)
		{
			printf("RegisterClass() failed with error %d\n", GetLastError());
			return NULL;
		}
		else
			printf("RegisterClass() is OK!\n");

		// Create a window

		if ((Window = CreateWindow(
			(LPCSTR)ProviderClass,
			TEXT(""),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			NULL,
			NULL,
			NULL,
			NULL)) == NULL)
		{
			printf("CreateWindow() failed with error %d\n", GetLastError());
			return NULL;
		}
		else
			printf("CreateWindow() is OK!\n");

		return Window;
	}

	static void CreateSocketInformation(SOCKET s)
	{
		LPSOCKET_INFORMATION SI;

		if ((SI = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL)
		{
			printf("GlobalAlloc() failed with error %d\n", GetLastError());
			return;
		}
		else
			printf("GlobalAlloc() for SOCKET_INFORMATION is OK!\n");

		// Prepare SocketInfo structure for use

		SI->Socket = s;
		SI->RecvPosted = FALSE;
		SI->BytesSEND = 0;
		SI->BytesRECV = 0;
		SI->Next = SocketInfoList;
		SocketInfoList = SI;
	}

	static LPSOCKET_INFORMATION GetSocketInformation(SOCKET s)
	{
		SOCKET_INFORMATION *SI = SocketInfoList;

		while (SI)
		{
			if (SI->Socket == s)
				return SI;

			SI = SI->Next;
		}

		return NULL;
	}

	static void FreeSocketInformation(SOCKET s)
	{
		SOCKET_INFORMATION *SI = SocketInfoList;
		SOCKET_INFORMATION *PrevSI = NULL;

		while (SI)
		{
			if (SI->Socket == s)
			{
				if (PrevSI)
					PrevSI->Next = SI->Next;
				else
					SocketInfoList = SI->Next;

				closesocket(SI->Socket);
				GlobalFree(SI);
				return;
			}

			PrevSI = SI;
			SI = SI->Next;
		}
	}

	void senddata(const char * data, unsigned long len)
	{

	}

	void closeSocket(bool bNotify)
	{

	}

	void onAccept(WPARAM wParam, LPARAM lParam)
	{
		SOCKET Accept = INVALID_SOCKET;

		if ((Accept = accept(wParam, NULL, NULL)) == INVALID_SOCKET)
		{
			printf("accept() failed with error %d\n", WSAGetLastError());
		}
		else
			printf("accept() is OK!\n");
		// Create a socket information structure to associate with the socket for processing I/O

		RockSocket::Impl::CreateSocketInformation(Accept);

		printf("Socket number %d connected\n", Accept);

		WSAAsyncSelect(Accept, workWnd, WM_SOCKET_NOTIFY, FD_READ | FD_WRITE | FD_CLOSE);
	}

	int onRead(WPARAM wParam, LPARAM lParam)
	{
		DWORD RecvBytes;
		LPSOCKET_INFORMATION SocketInfo;
		DWORD Flags;

		SocketInfo = RockSocket::Impl::GetSocketInformation(wParam);

		// Read data only if the receive buffer is empty

		if (SocketInfo->BytesRECV != 0)
		{
			SocketInfo->RecvPosted = TRUE;
			return 0;
		}
		else
		{
			SocketInfo->DataBuf.buf = SocketInfo->Buffer;
			SocketInfo->DataBuf.len = DATA_BUFSIZE;

			Flags = 0;

			if (WSARecv(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes,
				&Flags, NULL, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					printf("WSARecv() failed with error %d\n", WSAGetLastError());
					RockSocket::Impl::FreeSocketInformation(wParam);
					return 0;
				}
			}
			else // No error so update the byte count
			{
				printf("WSARecv() is OK!\n");
				SocketInfo->BytesRECV = RecvBytes;
			}
		}

	}

	int onWrite(WPARAM wParam, LPARAM lParam)
	{
		LPSOCKET_INFORMATION SocketInfo;
		DWORD SendBytes;

		SocketInfo = RockSocket::Impl::GetSocketInformation(wParam);

		if (SocketInfo->BytesRECV > SocketInfo->BytesSEND)
		{
			SocketInfo->DataBuf.buf = SocketInfo->Buffer + SocketInfo->BytesSEND;
			SocketInfo->DataBuf.len = SocketInfo->BytesRECV - SocketInfo->BytesSEND;

			if (WSASend(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &SendBytes, 0,
				NULL, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					printf("WSASend() failed with error %d\n", WSAGetLastError());
					RockSocket::Impl::FreeSocketInformation(wParam);
					return 0;
				}
			}
			else // No error so update the byte count
			{
				printf("WSASend() is OK!\n");
				SocketInfo->BytesSEND += SendBytes;
			}
			return 1;
		}

		if (SocketInfo->BytesSEND == SocketInfo->BytesRECV)
		{
			SocketInfo->BytesSEND = 0;
			SocketInfo->BytesRECV = 0;

			// If a RECV occurred during our SENDs then we need to post an FD_READ notification on the socket
			if (SocketInfo->RecvPosted == TRUE)
			{
				SocketInfo->RecvPosted = FALSE;
				PostMessage(workWnd, WM_SOCKET_NOTIFY, wParam, FD_READ);
			}
		}
	}

	void onClose(WPARAM wParam, LPARAM lParam)
	{

	}	

	void onConnect(WPARAM wParam, LPARAM lParam)
	{

	}


	void connectSever(DWORD ipaddr, WORD port)
	{
		struct sockaddr_in	server;
		struct hostent		*host = NULL;
		int					ret;

		try
		{
			InitSocket();
			if (ipaddr == 0)
			{
				if (WSAAsyncGetHostByName(workWnd, 
					WM_GETIPMSG, serverAddr,(char*)&serverhost,sizeof(serverhost)) == 0)
				{
					throw RockSocketException(string("½âÎöÓòÃû³ö´í"));
				}
			}
			SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sClient == INVALID_SOCKET)
			{
				throw RockSocketException();
			}

			server.sin_family = AF_INET;
			server.sin_port = htons(port);
			server.sin_addr.S_un.S_addr = ipaddr;

			ret = WSAAsyncSelect(sClient, workWnd, WM_SOCKET_NOTIFY, FD_READ | FD_CONNECT | FD_CLOSE | FD_WRITE);

			if (ret == SOCKET_ERROR)
			{
				throw RockSocketException(string("°ó¶¨´°¿Ú´íÎó"));
			}
			ret = connect(sClient,(struct sockaddr*)&server, sizeof(server));
			
			if (ret == SOCKET_ERROR)
			{
				ret = WSAGetLastError();
				if (ret != WSAEWOULDBLOCK)
				{
					throw RockSocketException(string(""));
				}
			}
			m_stat = SSconnet;
		}
		catch (RockSocketException e)
		{
			return;
		}

	}

	void connectSever(LPCTSTR pszServerAddr, WORD port)
	{
		struct hostent		*host = NULL;

		DWORD dwServerIP = inet_addr(pszServerAddr);
 		if (dwServerIP == INADDR_NONE)
 		{

//  		host = gethostbyname(pszServerAddr);
//  		if (host == NULL)
//  		{
//  			throw RockSocketException(string("½âÎöÓòÃûÊ§°Ü"));
//  		}
//  		else
//  		{
//  			dwServerIP = ((LPIN_ADDR)host->h_addr_list[0])->s_addr;
//  		}
			dwServerIP = 0;
 		}
		connectSever(dwServerIP, port);
	}
};
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_SOCKET_NOTIFY)
	{
		if (WSAGETSELECTERROR(lParam))
		{
			printf("Socket failed with error %d\n", WSAGETSELECTERROR(lParam));
			RockSocket::Impl::FreeSocketInformation(wParam);
		}
		else
		{
			printf("Socket looks fine!\n");

			switch (WSAGETSELECTEVENT(lParam))
			{
			case FD_ACCEPT:
				RockSocket::instance().onAccept(wParam, lParam);
				break;

			case FD_READ:
				RockSocket::instance().onRead(wParam, lParam);

				// DO NOT BREAK HERE SINCE WE GOT A SUCCESSFUL RECV. Go ahead

				// and begin writing data to the client

			case FD_WRITE:
				RockSocket::instance().onWrite(wParam, lParam);
				break;

			case FD_CLOSE:
				RockSocket::instance().onClose(wParam, lParam);
				printf("Closing socket %d\n", wParam);
				RockSocket::Impl::FreeSocketInformation(wParam);
				break;
			}
		}
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);

}

RockSocket::RockSocket()
{
	impl = new Impl(this);
	impl->loadConfig(string("C:/Users/stephen/Desktop/1.json"));
	createServer();
}


RockSocket::~RockSocket()
{
	if (impl)
	{
		delete impl;
	}
}

RockSocket& RockSocket::instance()
{
	static RockSocket singleton;
	return singleton;
}

void RockSocket::createServer()
{
	impl->createServer();
}

void RockSocket::onAccept(WPARAM wParam, LPARAM lParam)
{
	impl->onAccept(wParam, lParam);
}

void RockSocket::onRead(WPARAM wParam, LPARAM lParam)
{
	impl->onRead(wParam, lParam);
}

void RockSocket::onWrite(WPARAM wParam, LPARAM lParam)
{
	impl->onWrite(wParam, lParam);
}

void RockSocket::onClose(WPARAM wParam, LPARAM lParam)
{
	impl->onClose(wParam, lParam);
}

void RockSocket::connectSever(DWORD ipaddr, WORD port)
{
	impl->connectSever(ipaddr, port);
}


LPSOCKET_INFORMATION RockSocket::Impl::SocketInfoList = NULL;