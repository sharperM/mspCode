// CommandHttpClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinInet.h>

using namespace std;


#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock.h>
#include <wininet.h>
#include <shellapi.h>
#include <mmsystem.h>
#pragma comment(lib, "wininet.lib")

typedef struct vs {
	char host[128];
	char referer[128];
	char chan[128];
	SOCKET sock;
	BOOL silent;
	BOOL gotinfo;
} vs;

HANDLE ih;
#define _CRT_SECURE_NO_WARNINGS
VOID visit(LPVOID param)
{
	URL_COMPONENTSA url;
	HINTERNET ch, req;
	DWORD dwRead;
	// char szBuffer[1024];
	BYTE szBuffer = 0;
	LPVOID lpMsgBuf;
	const char *accept = "*/*";
	char *error = "error visiting URL.";
	vs visit;
	char vhost[128];
	int vport;
	char vuser[128];
	char vpass[128];
	char vpath[256];

	visit = *((vs *)param);
	vs *vsp = (vs *)param;
	vsp->gotinfo = TRUE;

	// zero out string varaiables
	memset(vhost, 0, sizeof(vhost));
	memset(vuser, 0, sizeof(vuser));
	memset(vpass, 0, sizeof(vpass));
	memset(vpath, 0, sizeof(vpath));

	// zero out url structure and set options
	memset(&url, 0, sizeof(url));
	url.dwStructSize = sizeof(url);
	url.dwHostNameLength = 1;
	url.dwUserNameLength = 1;
	url.dwPasswordLength = 1;
	url.dwUrlPathLength = 1;

	do {
		// crack the url (break it into its main parts)
		if (!InternetCrackUrlA(visit.host, strlen(visit.host), 0, &url)) {
			if (!visit.silent)
				printf("%s\n", "invalid url");
			// irc_privmsg(visit.sock, visit.chan, "invalid URL.", FALSE);
			break;
		}

		// copy url parts into variables
		if (url.dwHostNameLength > 0) strncpy(vhost, url.lpszHostName, url.dwHostNameLength);
		vport = url.nPort;
		if (url.dwUserNameLength > 0) strncpy(vuser, url.lpszUserName, url.dwUserNameLength);
		if (url.dwPasswordLength > 0) strncpy(vpass, url.lpszPassword, url.dwPasswordLength);
		if (url.dwUrlPathLength > 0) strncpy(vpath, url.lpszUrlPath, url.dwUrlPathLength);

		ch = InternetConnectA(ih, vhost, vport, vuser, vpass, INTERNET_SERVICE_HTTP, 0, 0);
		if (ch == NULL) {
			if (!visit.silent)
				printf("%s\n", "error InternetConnect");
			// irc_privmsg(visit.sock, visit.chan, error, FALSE);
			break;
		}

		req = HttpOpenRequestA(ch, NULL, vpath, NULL, visit.referer, &accept, INTERNET_FLAG_NO_UI, 0);
		if (req == NULL) {
			if (!visit.silent)
				printf("%s\n", "error HttpOpenRequest");
			// irc_privmsg(visit.sock, visit.chan, error, FALSE);
			break;
		}

		if (HttpSendRequest(req, NULL, 0, NULL, 0)) {
			if (!visit.silent)
				printf("%s\n", "success HttpSendRequest");
			//   if ( InternetReadFile(req, &szBuffer, 1024, &dwRead) ) {
			//                printf("%s\n", &szBuffer);
			//   } else {

			// FormatMessage( 
			//     FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			//     FORMAT_MESSAGE_FROM_SYSTEM | 
			//     FORMAT_MESSAGE_IGNORE_INSERTS,
			//     NULL,
			//     GetLastError(),
			//     0, // Default language
			//     (LPTSTR) &lpMsgBuf,
			//     0,
			//     NULL 
			// );
			// // Process any inserts in lpMsgBuf.
			// // ...
			// // Display the string.
			// printf("%s\n", lpMsgBuf);
			// // Free the buffer.
			// LocalFree( lpMsgBuf );

			//   }

			while (InternetReadFile(req, &szBuffer, 1, &dwRead) == TRUE) {
				if (dwRead == 0)
				{
					break;
				}
				printf("%s", &szBuffer);
			}

			// irc_privmsg(visit.sock, visit.chan, "url visited.", FALSE);
		}
		else {
			if (!visit.silent)
				printf("%s\n", "error HttpSendRequest");
			// irc_privmsg(visit.sock, visit.chan, error, FALSE);
		}
	} while (0); // always false, so this never loops, only helps make error handling easier

	InternetCloseHandle(ch);
	InternetCloseHandle(req);
	// return 0;
}

// int main(int argc, char const *argv[])
// {
// 	vs vis;
// 
// 	ih = InternetOpenA("Mozilla/4.0 (compatible)", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
// 	if (ih == NULL)
// 	{
// 		printf("%s\n", "null");
// 	}
// 
// 	strncpy(vis.host, "http://translate.google.cn/#zh-CN/en/%E5%B9%B3%E5%8F%B0", sizeof(vis.host) - 1);
// 	vis.silent = FALSE;
// 
// 	visit(&vis);
// 
// 	return 0;
// }