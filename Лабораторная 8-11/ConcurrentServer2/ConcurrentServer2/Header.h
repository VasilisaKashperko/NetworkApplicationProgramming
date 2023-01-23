#pragma once
#include <iostream>
#include<list>
#include<thread>

using namespace std;

extern int totalNumberOfConnections;
extern int numberOfActiveConnections;
extern int numberOfRefuses;

// Server variables
extern char serverName[];
extern char hostName[50];
extern SOCKET UDPSocket;
extern SOCKET TCPSocket;
extern u_short port;
extern bool connectionsAllowed;
extern bool exitRequested;
extern list<thread> clientThreadsList;
extern thread listener;


SOCKET* StartServer(u_short port);
void StopServer(SOCKET* sS);
void ExitListener();
void ConsoleThread();
void ListenConnections();
void ClientThread(string pipeName);

// Error handling
string GetErrorMsgText(int code);
string SetErrorMsgText(string msgText, int code);