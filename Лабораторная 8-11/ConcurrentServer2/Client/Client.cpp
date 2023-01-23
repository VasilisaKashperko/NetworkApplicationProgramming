#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include "Header.h"
#include <ws2tcpip.h>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
    system("chcp 65001");
    system("cls");

    WSADATA wsaData;
    char call[] = "Server";
    u_short serverPort = 2000;

    cout << "Client!" << endl << endl;

    // Set server port
    if (argc > 1)
    {
        serverPort = atoi(argv[1]);
    }
    cout << "Server port: " << serverPort << endl << endl;

    try
    {
        // Startup Winsocket
        if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
        {
            throw SetErrorMsgText("Startup:", WSAGetLastError());
        }

        // Server parameters
        sockaddr_in serv;
        int lserv;

        string pipeName;

        // Connect to server
        if (!GetServer(call, serverPort, &serv, &lserv, &pipeName))
        {
            cout << "Connection failed" << endl;
        }
        // Get server name
        char serverName[NI_MAXHOST];
        char serverInfo[NI_MAXSERV];
        DWORD dwNameInfo = getnameinfo
        (
            (struct sockaddr*)&serv, 
            sizeof(struct sockaddr), 
            serverName, NI_MAXHOST, 
            serverInfo, NI_MAXSERV, 
            NI_NUMERICSERV
        );
        cout << "Connected to server" << endl;
        cout << "Server name:\t\t" << serverName << endl;
        cout << "Server port number:\t" << serv.sin_port << endl;
        cout << "Server IP-address:\t" << inet_ntoa(serv.sin_addr) << endl;
        cout << "Pipe name:\t\t" << pipeName << endl;


        // We must use wide string to connect to named pipe
        string pipeStr = "\\\\" + string(serverName) + "\\pipe\\" + pipeName;
        wstring pipeWstr;
        for (int i = 0; i < pipeStr.length(); i++)
        {
            pipeWstr += wchar_t(pipeStr[i]);
        }
        
        // Connect to named pipe
        HANDLE hPipe;
        hPipe = CreateFile
        (
            pipeWstr.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL, OPEN_EXISTING, NULL, NULL
        );
        if (hPipe == INVALID_HANDLE_VALUE)
        {
            throw "create: invalid named pipe handle value";
        }
        cout << "Connected to pipe" << endl;
        
        while (true)
        {
            char command[50];
            cout << "> ";
            cin.getline(command, 50);

            // Send command
            unsigned long lw;
            if (!WriteFile(hPipe, command, sizeof(command), &lw, NULL))
            {
                throw "writefile error";
            }
            // Check on exit
            if (strcmp(command, "exit") == 0)
            {
                break;
            }
            // Recieve response
            char response[50];
            unsigned long lr;
            if (!ReadFile(hPipe, response, sizeof(response), &lr, NULL))
            {
                throw "readfile error";
            }
            cout << response << endl << endl;
        }

        CloseHandle(hPipe);

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
    catch (exception ex)
    {
        cout << "Something went wrong" << endl;
    }

    system("pause");
    return 0;
}
