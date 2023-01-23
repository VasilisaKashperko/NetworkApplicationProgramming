#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include "Header.h"

void StopServer(SOCKET* sS)
{
	if (closesocket(*sS) == SOCKET_ERROR)
	{
		throw SetErrorMsgText("closesocket:", WSAGetLastError());
	}
	if (WSACleanup() == SOCKET_ERROR)
	{
		throw SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
}