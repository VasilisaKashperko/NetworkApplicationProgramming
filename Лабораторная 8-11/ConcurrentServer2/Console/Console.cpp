#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include "Header.h"

using namespace std;

int main(int argc, char* argv[])
{
    system("chcp 65001");
    system("cls");

    WSADATA wsaData;
    u_short serverPort = 2000;

    // Set server port
    if (argc > 1)
    {
        serverPort = atoi(argv[1]);
    }

    cout << "Console!" << endl << endl;
    cout << "Server port: " << serverPort << endl << endl;

    // Server parameters
    SOCKADDR_IN serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(serverPort);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    try
    {
        // Startup Winsocket
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
        {
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        }

        SOCKET cC;
        cC = socket(AF_INET, SOCK_STREAM, NULL);
        if (cC == INVALID_SOCKET)
        {
            throw SetErrorMsgText("socket:", WSAGetLastError());
        }
        if ((connect(cC, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
        {
            throw SetErrorMsgText("connect:", WSAGetLastError());
        }
        cout << "Connected to server" << endl;

        while (true)
        {
            char command[50];
            cout << "> ";
            cin.getline(command, 50);
            // Send command
            int lobuf = send(cC, command, strlen(command) + 1, NULL);
            if (lobuf == SOCKET_ERROR)
            {
                throw  SetErrorMsgText("send:", WSAGetLastError());
            }
            string strCmd = string(command);
            // Check command
            if (strCmd == "disconnect" || strCmd == "exit" || strCmd == "shutdown")
            {
                break;
            }
            // Receive response
            char response[100];
            int lr = recv(cC, response, sizeof(response), 0);
            if (lr == SOCKET_ERROR)
            {
                throw SetErrorMsgText("recv:", WSAGetLastError());
            }
            cout << response << endl << endl;
        }

        if (closesocket(cC) == SOCKET_ERROR)
        {
            throw SetErrorMsgText("closesocket:", WSAGetLastError());
        }
        // Stop Winsocket
        if (WSACleanup() == SOCKET_ERROR)
        {
            throw SetErrorMsgText("Cleanup:", WSAGetLastError());
        }
    }
    catch (string errorMsgText)
    {
        cout << endl << "WSAGetLastError: " << errorMsgText << endl;
    }

    system("pause");
    return 0;
}