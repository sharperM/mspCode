#include "stdafx.h"
#include "RockSocket.h"

#include <stdlib.h>
#include <stdio.h>
#include "json\json.h"
#include <ios>
#include <fstream>
#include <sstream>
#include <string>


using namespace std;
#pragma comment (lib, "Ws2_32.lib")
#define DEFAULT_PORT "5150"
#define DEFAULT_BUFFER 2048
#define DATA_BUFSIZE 8192
#define  MAKESTR(s) (((std::ostringstream&)(std::ostringstream()<<std::string() << s)).str().c_str())
#define  WM_SOCKET_NOTIFY (WM_USER+1)
#define  WM_GETIPMSG (WM_USER+2)

// typedef definition

enum SocketStat{SSinvalid,SSinit,SSconnet,SSclose};
enum enClientSocketStat{ SSNoConnect, SSConnecting, SSconneted,  };
enum enServerSocketStat{ SSNobind, SSBinded};
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
	bool						bLibInit;
	SocketStat					m_stat;
	enClientSocketStat			enClientStat;
	enServerSocketStat			enServerStat;
	HWND						workWnd;
	char						serverAddr[256];
	HOSTENT						serverhost;
	vector<char>				vecSendbuf;
	SOCKET						sClient;
	SOCKET						ListenSocket;

	RockSocketSink				*sinkPtr;

	unsigned long				clientCount;
public:
	Impl(RockSocket *_this)
	{
		this->_this		= _this;
		m_stat			= SSinvalid;
		bLibInit = false;
		ListenSocket	= INVALID_SOCKET;
		sClient = INVALID_SOCKET;
		enClientStat = SSNoConnect;
		enServerStat = SSNobind;
		workWnd = NULL;
		sinkPtr = nullptr;
		clientCount = 0;
	}

	~Impl()
	{
		if (m_stat != SSinvalid)
		{
			if (ListenSocket != INVALID_SOCKET)
			{	
				closesocket(ListenSocket);
			}
			if (enClientStat != SSNoConnect)
			{
				closesocket(sClient);
			}
			if (bLibInit)
			{
				WSACleanup();
			}
			if (workWnd)
			{
				SendMessage(workWnd, WM_CLOSE,NULL,NULL);
			}
		}
		while (SocketInfoList)
		{
			OutputDebugStringA(MAKESTR(SocketInfoList));
			FreeSocketInformation(SocketInfoList->Socket);
		}
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

	bool InitSocketLib()throw(RockSocketException)
	{
		if (bLibInit) return true;
		WSADATA wsaData;
		int iResult;
		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			OutputDebugStringA(MAKESTR("WSAStartup failed with error: " << iResult << "\n"));
			throw RockSocketException();
			return false;
		}
		bLibInit = true;
		return true;
	}

	bool setListenAddr(string& strPort, struct addrinfo **ppresult) throw(RockSocketException)
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
			OutputDebugStringA(MAKESTR("getaddrinfo failed with error: "<<iResult<<"\n"));
			throw RockSocketException();
			return false;
		}
		return true;
	}

	bool createServer()
	{
		if (enServerStat == SSNobind)
		{		
			SOCKADDR_IN InternetAddr;
			struct addrinfo *result = NULL;
			int iResult;

			try
			{
				InitSocketLib();
						
				setListenAddr(string(DEFAULT_PORT), &result);

				// Create a SOCKET for connecting to server
				ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
				if (ListenSocket == INVALID_SOCKET) {
					OutputDebugStringA(MAKESTR("socket failed with error: " << WSAGetLastError() << "\n"));
					freeaddrinfo(result);
					throw RockSocketException();
				}
				OutputDebugStringA("socket() is pretty fine!\n");

				workWnd = MakeWorkerWindow(_this);

				if (WSAAsyncSelect(ListenSocket, workWnd, WM_SOCKET_NOTIFY, FD_ACCEPT | FD_CLOSE))
				{
					OutputDebugStringA(MAKESTR("WSAAsyncSelect() failed with error code "<< WSAGetLastError()<<"\n"));
					throw RockSocketException(string("°ó¶¨´°¿Ú´íÎó"));
				}
				OutputDebugStringA("WSAAsyncSelect() is OK lol!\n");

				InternetAddr.sin_family = AF_INET;
				InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
				InternetAddr.sin_port = htons(atoi(DEFAULT_PORT));

				if (bind(ListenSocket, (PSOCKADDR)&InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
				{
					OutputDebugStringA(MAKESTR("bind() failed with error "<<WSAGetLastError()<<"\n"));
					throw RockSocketException(string("bind() err"));
				}

				freeaddrinfo(result);

				iResult = listen(ListenSocket, SOMAXCONN);
				if (iResult == SOCKET_ERROR) {
					OutputDebugStringA(MAKESTR("listen failed with error: "<< WSAGetLastError()<<"\n"));
					closesocket(ListenSocket);
					throw RockSocketException();
				}
				OutputDebugStringA("Server Start successful");
				enServerStat = SSBinded;
			}
			catch (RockSocketException )
			{
				return false;
			}
			
		}
		return true;
	}

	HWND MakeWorkerWindow(RockSocket* ptr)
	{
		WNDCLASS wndclass;
		TCHAR *ProviderClass = TEXT("AsyncSelect");
		HWND Window;

		wndclass.style = CS_HREDRAW | CS_VREDRAW ;
		wndclass.lpfnWndProc = (WNDPROC)WindowProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = NULL;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = (LPCTSTR)ProviderClass;

		if (RegisterClass(&wndclass) == 0)
		{
			OutputDebugStringA(MAKESTR("RegisterClass() failed with error " << GetLastError()<<"\n"));
			return NULL;
		}
		else
			OutputDebugStringA("RegisterClass() is OK!\n");

		// Create a window

		if ((Window = CreateWindow(
			(LPCTSTR)ProviderClass,
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
		SetWindowLong(Window, GWLP_USERDATA, (long)ptr);
		return Window;
	}

	static void CreateSocketInformation(SOCKET s)
	{
		LPSOCKET_INFORMATION SI;

		if ((SI = (LPSOCKET_INFORMATION)GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL)
		{
			OutputDebugStringA(MAKESTR("GlobalAlloc() failed with error " << GetLastError() << "\n"));
			return;
		}
		else
			OutputDebugStringA("GlobalAlloc() for SOCKET_INFORMATION is OK!\n");

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
		if (data!=NULL&&len>0)
		{
			vecSendbuf.insert(vecSendbuf.end(), data, data + len);
		}
	}

	void closeSocket(bool bNotify)
	{
		closesocket(sClient);
	}

	void onAccept(WPARAM wParam, LPARAM lParam)
	{
		SOCKET Accept = INVALID_SOCKET;

		if ((Accept = accept(wParam, NULL, NULL)) == INVALID_SOCKET)
		{
			OutputDebugStringA(MAKESTR("accept() failed with error " << WSAGetLastError() << "\n"));
			return;
		}
		else
			OutputDebugStringA("accept() is OK!\n");
		// Create a socket information structure to associate with the socket for processing I/O
		CreateSocketInformation(Accept);

		OutputDebugStringA(MAKESTR("Socket number "<< Accept<<" connected\n"));

		WSAAsyncSelect(Accept, workWnd, WM_SOCKET_NOTIFY, FD_READ | FD_WRITE | FD_CLOSE);

		++clientCount;

		if (sinkPtr)
		{
			sinkPtr->onAccept(Accept);
		}
		
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
					OutputDebugStringA(MAKESTR("WSARecv() failed with error "<< WSAGetLastError()<<"\n"));
					RockSocket::Impl::FreeSocketInformation(wParam);
					return 0;
				}
			}
			else // No error so update the byte count
			{
				OutputDebugStringA("WSARecv() is OK!\n");
				SocketInfo->BytesRECV = RecvBytes;
			}
		}

		if (sinkPtr)
		{
			sinkPtr->onRead(SocketInfo->DataBuf.buf, RecvBytes);
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

					OutputDebugStringA(MAKESTR("WSASend() failed with error "<<WSAGetLastError()<<"\n"));
					RockSocket::Impl::FreeSocketInformation(wParam);
					return 0;
				}
			}
			else // No error so update the byte count
			{
				OutputDebugStringA("WSASend() is OK!\n");
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
		RockSocket::Impl::FreeSocketInformation(wParam);
		std::cout<< clientCount-- << std::endl;
		if (sinkPtr)
		{
			sinkPtr->onclose();
		}
	}	

	void setSink(RockSocketSink* ptr)
	{
		sinkPtr = ptr;
	}


};


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_GETIPMSG||uMsg == WM_SOCKET_NOTIFY)
	{
		((RockSocket*)GetWindowLong(hwnd, GWLP_USERDATA))->wndProc(uMsg, wParam, lParam);
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


void RockSocket::sendData(const char* data, unsigned long len)
{

}

LRESULT RockSocket::wndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_SOCKET_NOTIFY)
	{
		if (WSAGETSELECTERROR(lParam))
		{
			OutputDebugStringA(MAKESTR("Socket failed with error " << WSAGETSELECTERROR(lParam) << "\n"));
			RockSocket::Impl::FreeSocketInformation(wParam);
			return 1;
		}
		else
		{
			OutputDebugStringA("Socket looks fine!\n");

			switch (WSAGETSELECTEVENT(lParam))
			{
			case FD_ACCEPT:
				onAccept(wParam, lParam);
				break;

			case FD_READ:
				onRead(wParam, lParam);
				//break;

			case FD_WRITE:
				onWrite(wParam, lParam);
				break;

			case FD_CLOSE:
				onClose(wParam, lParam);
				OutputDebugStringA(MAKESTR("Closing socket " << wParam << "\n"));
				break;
			}
		}
		return 0;
	}
	if (uMsg == WM_GETIPMSG)
	{
		return 0;
	}
	return 0;
}

void RockSocket::setSink(RockSocketSink* ptr)
{
	impl->setSink(ptr);
}
LPSOCKET_INFORMATION RockSocket::Impl::SocketInfoList = NULL;