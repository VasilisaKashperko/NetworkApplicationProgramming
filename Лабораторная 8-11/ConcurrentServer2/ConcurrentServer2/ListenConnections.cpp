#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <thread>
#include <string>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include "Header.h"

using namespace std;

void ListenConnections()
{
	while (true)
	{
        try
        {
            // Client socket parameters
            SOCKADDR_IN clnt;
            memset(&clnt, 0, sizeof(clnt));
            int lclnt = sizeof(clnt);

            // Recieve message from client
            char ibuf[50];
            int libuf = 0;
            libuf = recvfrom(UDPSocket, ibuf, sizeof(ibuf), NULL, (sockaddr*)&clnt, &lclnt);
            if (libuf == SOCKET_ERROR)
            {
                throw SetErrorMsgText("recv:", WSAGetLastError());
            }

            if (exitRequested)
            {
                return;
            }
            else if (!connectionsAllowed)
            {
                numberOfRefuses += 1;
                continue;
            }

            if (strcmp(ibuf, serverName) == 0)
            {
                cout << "Received connection request" << endl;

                // Convert client port to string
                u_short clientPort = ntohs(clnt.sin_port);
                string clientPortStr = to_string(clientPort);
                cout << "Client port: " << clientPortStr << endl;
                // Convert client IP to string
                string clientIPStr = inet_ntoa(clnt.sin_addr);
                cout << "Client IP: " << clientIPStr << endl;
                // Pipe name format is IP:port (0.0.0.0:8888)
                string pipeName = clientIPStr + ":" + clientPortStr;
                cout << "Pipe name: " << pipeName << endl << endl;
                // Send pipe name to client
                int lobuf = sendto(UDPSocket, pipeName.c_str(), strlen(pipeName.c_str()) + 1, NULL, (sockaddr*)&clnt, sizeof(clnt));
                if (lobuf == SOCKET_ERROR)
                {
                    throw SetErrorMsgText("sendto: ", WSAGetLastError());
                }

                // Run client thread
                clientThreadsList.push_back(thread(ClientThread, pipeName));
                // Detach makes thread to run completely independent
                // Need it not to get 'abort() error' message
                clientThreadsList.back().detach();

                totalNumberOfConnections += 1;
                numberOfActiveConnections += 1;
            }
        }
        catch (string errorMsgText)
        {
            cout << endl << "WSAGetLastError: " << errorMsgText << endl;
        }
	}
}