#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include "Header.h"

SOCKET* StartServer(u_short port)
{
	// Startup Winsocket
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
	{
		throw SetErrorMsgText("Startup:", WSAGetLastError());
	}
	// Server socket
	SOCKET sS;
	sS = socket(AF_INET, SOCK_DGRAM, NULL);
	if (sS == INVALID_SOCKET)
	{
		throw SetErrorMsgText("socket:", WSAGetLastError());
	}
	// Server socket parameters
	SOCKADDR_IN serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	serv.sin_addr.s_addr = INADDR_ANY;
	// Bind socket parameters
	if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
	{
		throw SetErrorMsgText("bind:", WSAGetLastError());
	}
	return &sS;
}