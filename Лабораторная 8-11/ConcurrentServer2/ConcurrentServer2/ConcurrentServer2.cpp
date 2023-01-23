#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <thread>
#include <list>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include "Header.h"

using namespace std;

char serverName[] = "Server";
char hostName[50];
SOCKET UDPSocket;
SOCKET TCPSocket;
u_short port = 2000;
bool connectionsAllowed = true;
bool exitRequested = false;
list<thread> clientThreadsList;
int totalNumberOfConnections = 0;
int numberOfActiveConnections = 0;
int numberOfRefuses = 0;
thread listener;


int main(int argc, char* argv[])
{
    system("chcp 65001");
    system("cls");

    gethostname(hostName, 50);

    // Set port
    if (argc > 1)
    {
        port = atoi(argv[1]);
    }

    cout << "Concurrent server!" << endl << endl;
    cout << "Port: " << port << endl << endl;
    
    try
    {
        UDPSocket = *StartServer(port);

        listener = thread(ListenConnections);
        thread exitListener(ExitListener);
        thread console(ConsoleThread);
        
        console.join();
        exitListener.join();
        listener.join();

        // TODO : exit, shutdown

        StopServer(&UDPSocket);
    }
    catch (string errorMsgText)
    {
        cout << endl << "WSAGetLastError: " << errorMsgText << endl;
        system("pause");
        return 1;
    }

    system("pause");
    return 0;
}
