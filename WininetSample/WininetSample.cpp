// WininetSample.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#pragma comment( lib, "wininet" )

// int _tmain(int argc, _TCHAR* argv[])
// {
// 	return 0;
// }

#include<windows.h>
#include<wininet.h>
#include<iostream>
using namespace std;
HANDLE hConnectedEvent, hRequestOpenedEvent, hRequestCompleteEvent;
HINTERNET hInstance, hConnect, hRequest;
_TCHAR *lpszUrl, *lpszServer;

BOOL bAllDone = FALSE;
BOOL bVerbose = FALSE;

void __stdcall Callback(HINTERNET hInternet,
	DWORD dwContext,
	DWORD dwInternetStatus,
	LPVOID lpStatusInfo,
	DWORD dwStatusInfoLen);

int _tmain(int argc, _TCHAR *argv[])
{
	if (argc != 3)
	{
		if ((argc == 4) && (argv[3][0] == TEXT('v')))
			bVerbose = TRUE;
		else
		{
			cout << "Usage: asynchttp <server> <url> [v]" << endl;
			cout << "   <server> is the hostname of the http server" << endl;
			cout << "   <url> is the url of the object you are requesting (without the hostname)"<<  endl;
			cout << "   'v' for verbose output" << endl << endl;
			cout << "   Example: asynchttp www.domain.com /docs/readme.htm v" << endl;
			return -1;
		}
	}

	lpszServer = argv[1];
	lpszUrl = argv[2];

	hConnectedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hRequestOpenedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	hRequestCompleteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	hInstance = InternetOpen(TEXT("asynchttp"),
		INTERNET_OPEN_TYPE_PRECONFIG,
		NULL,
		NULL,
		INTERNET_FLAG_ASYNC); // ASYNC Flag

	if (hInstance == NULL)
	{
		cout << "InternetOpen failed, error " << GetLastError();
		return -1;
	}

	// Setup callback function
	if (InternetSetStatusCallback(hInstance,
		(INTERNET_STATUS_CALLBACK)&Callback) == INTERNET_INVALID_STATUS_CALLBACK)
	{
		cout << "InternetSetStatusCallback failed, error " << GetLastError();
		return -1;
	}

	// First call that will actually complete asynchronously even
	// though there is no network traffic
	hConnect = InternetConnect(hInstance,
		lpszServer,
		INTERNET_DEFAULT_HTTP_PORT,
		NULL,
		NULL,
		INTERNET_SERVICE_HTTP,
		0,
		1); // Connection handle's Context
	if (hConnect == NULL)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			cout << "InternetConnect failed, error " << GetLastError();
			return -1;
		}
		// Wait until we get the connection handle
		WaitForSingleObject(hConnectedEvent, INFINITE);
	}


	// Open the request
	hRequest = HttpOpenRequest(hConnect,
		TEXT("GET"),
		lpszUrl,
		NULL,
		NULL,
		NULL,
		INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE,
		2);  // Request handle's context 
	if (hRequest == NULL)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			cout << "HttpOpenRequest failed, error " << GetLastError();
			return -1;
		}
		// Wait until we get the request handle
		WaitForSingleObject(hRequestOpenedEvent, INFINITE);
	}

	if (!HttpSendRequest(hRequest,
		NULL,
		0,
		NULL,
		0))
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			cout << "HttpSendRequest failed, error " << GetLastError();
			return -1;
		}
	}

	if (bVerbose)
	{
		cout << "HttpSendRequest called successfully" << endl;
		cout.flush();
	}

	WaitForSingleObject(hRequestCompleteEvent, INFINITE);

	cout << "------------------- Read the response -------------------" << endl;
	char lpReadBuff[256];

	do
	{
		INTERNET_BUFFERS InetBuff;
		FillMemory(&InetBuff, sizeof(InetBuff), 0);
		InetBuff.dwStructSize = sizeof(InetBuff);
		InetBuff.lpvBuffer = lpReadBuff;
		InetBuff.dwBufferLength = sizeof(lpReadBuff)-1;

		if (bVerbose)
		{
			cout << "Calling InternetReadFileEx" << endl;
			cout.flush();
		}

		if (!InternetReadFileEx(hRequest,
			&InetBuff,
			0, 2))
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				if (bVerbose)
				{
					cout << "Waiting for InternetReadFileEx to complete" << endl;
					cout.flush();
				}
				WaitForSingleObject(hRequestCompleteEvent, INFINITE);
			}
			else
			{
				cout << "InternetReadFileEx failed, error " << GetLastError();
				cout.flush();
				return -1;
			}
		}

		lpReadBuff[InetBuff.dwBufferLength] = 0;
		cout << lpReadBuff;
		cout.flush();

		if (InetBuff.dwBufferLength == 0)
			bAllDone = TRUE;

	} while (bAllDone == FALSE);

	cout << endl << endl << "------------------- Request Complete ----------------" << endl;
	return 0;
}

void __stdcall Callback(HINTERNET hInternet,
	DWORD dwContext,
	DWORD dwInternetStatus,
	LPVOID lpStatusInfo,
	DWORD dwStatusInfoLen)
{
	if (bVerbose)
	{
		cout << "Callback dwInternetStatus: " << dwInternetStatus << " Context: " << dwContext << endl;
		cout.flush();
	}

	switch (dwContext)
	{
	case 1: // Connection handle
		if (dwInternetStatus == INTERNET_STATUS_HANDLE_CREATED)
		{
			INTERNET_ASYNC_RESULT *pRes = (INTERNET_ASYNC_RESULT *)lpStatusInfo;
			hConnect = (HINTERNET)pRes->dwResult;
			if (bVerbose)
			{
				cout << "Connect handle created" << endl;
				cout.flush();
			}
			SetEvent(hConnectedEvent);
		}
		break;
	case 2: // Request handle
		switch (dwInternetStatus)
		{
		case INTERNET_STATUS_HANDLE_CREATED:
		{
											   INTERNET_ASYNC_RESULT *pRes = (INTERNET_ASYNC_RESULT *)lpStatusInfo;
											   hRequest = (HINTERNET)pRes->dwResult;
											   if (bVerbose)
											   {
												   cout << "Request handle created" << endl;
												   cout.flush();
											   }
											   SetEvent(hRequestOpenedEvent);
		}
			break;
		case INTERNET_STATUS_REQUEST_SENT:
		{
											 DWORD *lpBytesSent = (DWORD*)lpStatusInfo;
											 if (bVerbose)
											 {
												 cout << "Bytes Sent: " << *lpBytesSent << endl;
												 cout.flush();
											 }
		}
			break;
		case INTERNET_STATUS_REQUEST_COMPLETE:
		{
												 INTERNET_ASYNC_RESULT *pAsyncRes = (INTERNET_ASYNC_RESULT *)lpStatusInfo;
												 if (bVerbose)
												 {
													 cout << "Function call finished" << endl;
													 cout << "dwResult: " << pAsyncRes->dwResult << endl;
													 cout << "dwError:  " << pAsyncRes->dwError << endl;
													 cout.flush();
												 }
												 SetEvent(hRequestCompleteEvent);
		}
			break;
		case INTERNET_STATUS_RECEIVING_RESPONSE:
			if (bVerbose)
			{
				cout << "Receiving Response" << endl;
				cout.flush();
			}
			break;
		case INTERNET_STATUS_RESPONSE_RECEIVED:
		{
												  DWORD *dwBytesReceived = (DWORD*)lpStatusInfo;
												  if (*dwBytesReceived == 0)
													  bAllDone = TRUE;
												  if (bVerbose)
												  {
													  cout << "Received " << *dwBytesReceived << endl;
													  cout.flush();
												  }
		}

		}

	}

}