#include "stdafx.h"
#include "testCode.h"
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")
testCode::testCode()
{
	SYSTEM_INFO ss;
	GetNativeSystemInfo(&ss);
	std::string processType;
	if (ss.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
	{
		processType = "x64 (AMD or Intel)";
	}
	else if (ss.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ARM)
	{
		processType = "ARM";
	}
	else if (ss.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
	{
		processType = "Intel Itanium-based";
	}
	else if (ss.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
	{
		processType = "x86";
	}
	else if (ss.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_UNKNOWN)
	{
		processType = "Unknown architecture";
	}
	OutputDebugString(MAKESTR("processType:"<< processType<<"\r\n"));

}


testCode::~testCode()
{
}

testCode& testCode::instance()
{
	static testCode singleton;
	return singleton;
}

bool testCode::download()
{
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	BOOL  bResults = FALSE;
	HINTERNET  hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"WinHTTP Example/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.//http://lkdownload.lkgame.com//GamePlaza/RoomAddCount.json
	if (hSession)
		hConnect = WinHttpConnect(hSession, L"lkdownload.lkgame.com",
		INTERNET_DEFAULT_HTTP_PORT, 0);

	// Create an HTTP request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/GamePlaza/RoomAddCount.json",
		L"HTTP/1.1", WINHTTP_NO_REFERER,
		WINHTTP_DEFAULT_ACCEPT_TYPES,
		0);

	// Send a request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
		WINHTTP_NO_ADDITIONAL_HEADERS, 0,
		WINHTTP_NO_REQUEST_DATA, 0,
		0, 0);


	// End the request.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	// Keep checking for data until there is nothing left.
	if (bResults)
	{
		do
		{
			// Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				printf("Error %u in WinHttpQueryDataAvailable.\n",
				GetLastError());

			// Allocate space for the buffer.
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				dwSize = 0;
			}
			else
			{
				// Read the data.
				ZeroMemory(pszOutBuffer, dwSize + 1);

				if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
					dwSize, &dwDownloaded))
					printf("Error %u in WinHttpReadData.\n", GetLastError());
				else
					printf("%s", pszOutBuffer);

				// Free the memory allocated to the buffer.
				delete[] pszOutBuffer;
			}
		} while (dwSize > 0);
	}


	// Report any errors.
	if (!bResults)
		printf("Error %d has occurred.\n", GetLastError());

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);	return true;
}

bool r = testCode::instance().download();