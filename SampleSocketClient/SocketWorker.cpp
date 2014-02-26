#include "stdafx.h"
#include "SocketWorker.h"
#include <winsock2.h>
#include <string>
#include <vector>
#include <sstream>
#include <memory>

#pragma comment (lib, "Ws2_32.lib")
#define  MAKESTR(s) (((std::ostringstream&)(std::ostringstream()<<std::string() << s)).str().c_str())

using namespace std;
enum enClientSocketStat{ SSNoConnect, SSConnecting, SSconneted, };
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
class SocketWorker::Impl{
public:
	SocketWorker		*_this;

	WSADATA				wsd;

	SOCKET				sClient;

	struct sockaddr_in	server;

	struct hostent		*host;

	char				szBuffer[2048];

	HWND						hWorkWnd;

	WSABUF						DataBuf;

	DWORD BytesSEND;

	DWORD BytesRECV;

	char						serverAddr[256];

	HOSTENT						serverhost;

	vector<char>				vecSendbuf;

	volatile enClientSocketStat			enClientStat;

	bool bSendReady;

	Impl(SocketWorker *_this)
	{
		this->_this = _this;
		host = NULL;
		sClient = INVALID_SOCKET;
		enClientStat = SSNoConnect;
		bSendReady = false;
		BytesSEND = 0;
		BytesRECV = 0;
	}

	bool initSocketlib()
	{
		return (WSAStartup(MAKEWORD(2, 2), &wsd) != 0);
	}

	bool releaseSocketlib()
	{
		return WSACleanup() == 0;
	}

// 	bool createSocket()
// 	{
// 		if (sClient == INVALID_SOCKET)
// 		{
// 			sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
// 			return !(sClient == INVALID_SOCKET);
// 		}
// 		return true;
// 	}

	bool makeSocketAddress(const char* szServer, WORD iPort )
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

	bool connectServer(const char* szServer, WORD port)
	{
		if (enClientStat != SSNoConnect) return true;

		makeSocketAddress(szServer, port);
		
		try
		{
			sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sClient == INVALID_SOCKET)
			{
				throw "socket() ERROR";
			}
			if (WSAAsyncSelect(sClient, hWorkWnd, WM_SOCKET_NOTIFY_,
				FD_READ | FD_CONNECT | FD_CLOSE | FD_WRITE) == SOCKET_ERROR)
			{
				throw "绑定窗口错误";
			}
			if (connect(sClient, (struct sockaddr*)&server,
				sizeof(server)) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					throw ("connect()");
				}
				enClientStat = SSConnecting;
			}
		}
		catch (const char * e)
		{
			OutputDebugStringA(MAKESTR("error: " << e << "\n"));
			return false;
		}
		return true;
		//return !(connect(sClient, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR);
	}

	void connectSever(DWORD ipaddr, WORD port)
	{
		struct sockaddr_in	server;
		struct hostent		*host = NULL;
		//hWorkWnd = MakeWorkerWindow(_this);
		try
		{
			initSocketlib();
			if (ipaddr == 0)
			{
				if (WSAAsyncGetHostByName(hWorkWnd,
					WM_GETIPMSG, serverAddr, (char*)&serverhost, sizeof(serverhost)) == 0)
				{
					throw "解析域名出错";
				}
				return;
			}
			sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sClient == INVALID_SOCKET)
			{
				throw "socket() ERROR";
			}

			server.sin_family = AF_INET;
			server.sin_port = htons(port);
			server.sin_addr.S_un.S_addr = ipaddr;

			

			if (WSAAsyncSelect(sClient, hWorkWnd, WM_SOCKET_NOTIFY_, 
					FD_READ | FD_CONNECT | FD_CLOSE | FD_WRITE) == SOCKET_ERROR)
			{
				throw "绑定窗口错误";
			}
			WSABUF CallerData;
			WSABUF CalleeData;
			char CallerBuf = 'q';
			CallerData.buf = &CallerBuf;
			CallerData.len = 1;

			char CalleeBuf[12] = { 0 };
			CalleeData.buf = CalleeBuf;
			CalleeData.len = 12;
// 
// 			if (connect(sClient, (struct sockaddr*)&server,
// 				sizeof(server)) == SOCKET_ERROR)
			if (WSAConnect(sClient, (struct sockaddr*)&server,
				sizeof(server), &CallerData, &CalleeData, NULL, NULL) == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					throw ("connect()");
				}
				enClientStat = SSConnecting;
			}
			//enClientStat = SSconneted;
		}
		catch (const char * e)
		{
			OutputDebugStringA(MAKESTR("error: "<<e <<"\n"));
			return;
		}

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
				enClientStat = SSNoConnect;
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


	HWND MakeWorkerWindow(SocketWorker * _this)
	{
		WNDCLASSEX wndclass;
		TCHAR *ProviderClass = TEXT("AsyncSelect");
		HWND Window;
		wndclass.cbSize = sizeof(wndclass);
		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = (WNDPROC)WindowProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = NULL;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = (PTCHAR)ProviderClass;
		wndclass.hIconSm = NULL;
		if (RegisterClassEx(&wndclass) == 0)
		{
			OutputDebugStringA(MAKESTR("RegisterClass() failed with error " << GetLastError() << "\n"));
			return NULL;
		}
		else
			OutputDebugStringA("RegisterClass() is OK!\n");

		// Create a window

		if ((Window = CreateWindow(
			(PTCHAR)ProviderClass,
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
			OutputDebugStringA(MAKESTR("CreateWindow() failed with error " << GetLastError() << "\n"));
			return NULL;
		}
		else
			OutputDebugStringA("CreateWindow() is OK!\n");
		SetWindowLong(Window, GWLP_USERDATA, (long)_this);
		
		return Window;
	}

	void onConnect(WPARAM wParam, LPARAM lParam)
	{
		enClientStat = SSconneted;
	}

	void onRead(WPARAM wParam, LPARAM lParam)
	{
		DWORD Flags =0;
		SOCKET s = wParam;
		ZeroMemory(szBuffer, 2048);
		DataBuf.buf = szBuffer;
		DataBuf.len = 2048;
		DWORD RecvBytes=0;
		//DWORD SendBytes;
		if (WSARecv(s, &DataBuf, 1, &RecvBytes,	&Flags, NULL/*&RecvOverlapped*/, NULL) == SOCKET_ERROR)
		//if (recv(s, szBuffer, 2048, 0) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				OutputDebugStringA(MAKESTR("WSARecv() failed with error " << WSAGetLastError() << "\n"));
				return ;
			}	
		}
		else // No error so update the byte count
		{
			OutputDebugStringA("WSARecv() is OK!\n");
			OutputDebugStringA(MAKESTR("recv:\n"<<DataBuf.buf<<"\n"));
			BytesRECV = RecvBytes;
		}
	}

	void onWrite(WPARAM wParam, LPARAM lParam)
	{
		//当socket可以发送的时候，会发一个 FD_WRITE（FD_Connect/FD_accept之后马上 有 FD_write,）
		//除非 wsasend 返回 wsawouldblock  表示不能再发送。
		//当再可以发送的时候，会再发一个FD_WRITE
		
		bSendReady = true;
		sendData();
		return;
	}

	void sendData()
	{
		WSABUF sendBuf;
		DWORD SendBytes;
		if (vecSendbuf.empty()||(bSendReady == false))
		{
			return;
		}

		sendBuf.buf = vecSendbuf.data();
		sendBuf.len = vecSendbuf.size();

		if (WSASend(sClient, &(sendBuf), 1, &SendBytes, 0,
			NULL, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				OutputDebugStringA(MAKESTR("WSASend() failed with error " << WSAGetLastError() << "\n"));
				return;
			}
			else
			{
				bSendReady = false;
			}
		}
		else // No error so update the byte count
		{
			OutputDebugStringA("WSASend() is OK!\n");
			BytesSEND += SendBytes;
			vecSendbuf.erase(vecSendbuf.begin(), vecSendbuf.begin() + SendBytes);
		}
		if (vecSendbuf.empty())
		{
			BytesSEND = 0;
		}
	}
	
	void onClose(WPARAM wParam, LPARAM lParam)
	{
		closesocket(wParam);
		enClientStat = SSNoConnect;
	}
	
	void sendMsg(const char *strMsg, size_t len)
	{
		if (enClientStat == SSconneted)
		{

			//auto_ptr<char> p;
			//p.reset(new char[1024 * 1024]);
			//vecSendbuf.insert(vecSendbuf.end(), p.get(), p.get() + 1024 * 1024);
			vecSendbuf.insert(vecSendbuf.end(), strMsg, strMsg + len);
			sendData();			
		}
	}
};
SocketWorker::SocketWorker()
{
	impl = new Impl(this);
	impl->initSocketlib();
	impl->hWorkWnd = impl->MakeWorkerWindow(this);
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
	//impl->createSocket();
	impl->connectServer("localhost",5150);
}

// void SocketWorker::sendMsg(const char *strMsg)
// {
//  	impl->sendMsg(strMsg);
//  	impl->recviceMsg();
// }

void SocketWorker::sendMsg(const char *strMsg, size_t len)
{
	impl->sendMsg(strMsg, len);
}

void SocketWorker::closeSocket()
{
	impl->closeSocket_();
}

void SocketWorker::onConnect(WPARAM wParam, LPARAM lParam)
{
	impl->onConnect(wParam, lParam);
}

void SocketWorker::onRead(WPARAM wParam, LPARAM lParam)
{
	impl->onRead(wParam, lParam);
}

void SocketWorker::onWrite(WPARAM wParam, LPARAM lParam)
{
	impl->onWrite(wParam, lParam);
}

void SocketWorker::onClose(WPARAM wParam, LPARAM lParam)
{
	impl->onClose(wParam, lParam);
}

LRESULT SocketWorker::wmProc(UINT uMsg, LPARAM lParam, WPARAM wParam)
{
	if (uMsg == WM_SOCKET_NOTIFY_)
	{
		if (WSAGETSELECTERROR(lParam))
		{
			OutputDebugStringA(MAKESTR("Socket failed with error " << WSAGETSELECTERROR(lParam) << "\n"));
			return 1;
		}
		else
		{
			OutputDebugStringA("Socket looks fine!\n");

			switch (WSAGETSELECTEVENT(lParam))
			{
			case FD_CONNECT:
				
				onConnect(wParam, lParam);
				break;

			case FD_READ:
				onRead(wParam, lParam);
				break;
				// DO NOT BREAK HERE SINCE WE GOT A SUCCESSFUL RECV. Go ahead

				// and begin writing data to the client

			case FD_WRITE:
				onWrite(wParam, lParam);
				break;

			case FD_CLOSE:
				onClose(wParam, lParam);
				OutputDebugStringA(MAKESTR("Closing socket " << wParam << "\n"));
				break;
			}
		}
	}
	if (uMsg == WM_GETIPMSG)
	{

	}
	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_SOCKET_NOTIFY_ || uMsg == WM_GETIPMSG)
	{
		SocketWorker * pWork = (SocketWorker *)GetWindowLong(hwnd, GWL_USERDATA);
		pWork->wmProc(uMsg, lParam, wParam);
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
