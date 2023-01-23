#include <iostream>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include "Header.h"
#include <string>

using namespace std;

bool GetServer(char* call, short port, struct sockaddr_in* from, int* flen, string* pipeName)
{
    // Client socket
    SOCKET cC = socket(AF_INET, SOCK_DGRAM, NULL);
    if (cC == INVALID_SOCKET)
    {
        throw SetErrorMsgText("socket: ", WSAGetLastError());
    }
    // Set socket to broadcast mode
    int optval = 1;
    if (setsockopt(cC, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
    {
        throw SetErrorMsgText("opt: ", WSAGetLastError());
    }

    // Broadcast parameters for server
    SOCKADDR_IN all;
    all.sin_family = AF_INET;
    all.sin_port = htons(port);
    all.sin_addr.s_addr = INADDR_BROADCAST;
    
    // Send call
    int sendlen = sendto(cC, call, sizeof(call), NULL, (sockaddr*)&all, sizeof(all));
    if (sendlen == SOCKET_ERROR)
    {
        throw SetErrorMsgText("sendto: ", WSAGetLastError());
    }

    // Server parameters
    SOCKADDR_IN serv;
    int lserv = sizeof(serv);
    memset(&serv, 0, lserv);
    
    // Recieve pipe name as answer
    char ibuf[50];
    int libuf = recvfrom(cC, ibuf, sizeof(ibuf), NULL, (sockaddr*)&serv, &lserv);
    if (libuf == SOCKET_ERROR)
    {
        throw SetErrorMsgText("recv:", WSAGetLastError());
    }
    else if (libuf == WSAETIMEDOUT)
    {
        if (closesocket(cC) == SOCKET_ERROR)
        {
            throw SetErrorMsgText("closesocket:", WSAGetLastError());
        }
        return false;
    }

    // Set output parameters
    *from = serv;
    *flen = lserv;
    *pipeName = string(ibuf);

    if (closesocket(cC) == SOCKET_ERROR)
    {
        throw SetErrorMsgText("closesocket: ", WSAGetLastError());
    }

    return true;
}