// wsaAsyncSelectServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SampleServer.h"

int _tmain(int argc, _TCHAR* argv[])
{
	MSG msg;
	int Ret;
	SampleServer::instance();
	while (Ret = GetMessage(&msg, NULL, 0, 0))
	{
		if (Ret == -1)
		{

			printf("\nGetMessage() failed with error %d\n", GetLastError());

			return 1;
		}
		else
			OutputDebugStringA("\nGetMessage() is pretty fine!\n");
// 			printf("\nGetMessage() is pretty fine!\n");

// 		printf("Translating a message...\n");	
		TranslateMessage(&msg);
// 		printf("Dispatching a message...\n");
		DispatchMessage(&msg);
	}
}

