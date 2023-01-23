#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include "Header.h"
#include <thread>

void ExitListener()
{
    while (true)
    {
        if (exitRequested)
        {
            SOCKET cC = socket(AF_INET, SOCK_DGRAM, NULL);
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
            int sendlen = sendto(cC, "shutup", sizeof("shutup"), NULL, (sockaddr*)&all, sizeof(all));
            if (sendlen == SOCKET_ERROR)
            {
                throw SetErrorMsgText("sendto: ", WSAGetLastError());
            }
            
            return;
        }
    }
}