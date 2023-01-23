#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include "Header.h"
#include <string>
#include <thread>

using namespace std;

void ConsoleThread()
{
	while (true)
	{
		try
		{
			// New TCP socket
			TCPSocket = socket(AF_INET, SOCK_STREAM, NULL);
			if (TCPSocket == INVALID_SOCKET)
			{
				throw SetErrorMsgText("socket:", WSAGetLastError());
			}
			// TCP socket parameters
			SOCKADDR_IN serv;
			serv.sin_family = AF_INET;
			serv.sin_port = htons(port);
			serv.sin_addr.s_addr = INADDR_ANY;
			// Bind socket to parameters
			if (bind(TCPSocket, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			{
				throw SetErrorMsgText("bind:", WSAGetLastError());
			}
			// Set TCP socket to listen mode
			if (listen(TCPSocket, SOMAXCONN) == SOCKET_ERROR)
			{
				throw SetErrorMsgText("listen:", WSAGetLastError());
			}
			// Console client socket
			SOCKET cS;
			SOCKADDR_IN clnt;
			memset(&clnt, 0, sizeof(clnt));
			int lclnt = sizeof(clnt);
			// Accept console connection
			cS = accept(TCPSocket, (sockaddr*)&clnt, &lclnt);
			if (cS == INVALID_SOCKET)
			{
				throw SetErrorMsgText("accept:", WSAGetLastError());
			}

			while (true)
			{
				// Receive console command
				char ibuf[50];
				int libuf = recv(cS, ibuf, sizeof(ibuf), NULL);
				if (libuf == SOCKET_ERROR)
				{
					throw SetErrorMsgText("recv:", WSAGetLastError());
				}
				string command = string(ibuf);

				// Check command
				string response;
				if (command == "start")
				{
					connectionsAllowed = true;
					response = "Connections allowed";
				}
				else if (command == "stop")
				{
					connectionsAllowed = false;
					response = "Connections prohibited";
				}
				else if (command == "wait")
				{
					connectionsAllowed = false;
					while (true)
					{
						if (numberOfActiveConnections == 0)
						{
							break;
						}
					}
					connectionsAllowed = true;
				}
				else if (command == "statistics")
				{
					response = "Total connections: " + to_string(totalNumberOfConnections) + "\n";
					response += "Active connections: " + to_string(numberOfActiveConnections) + "\n";
					response += "Refuses: " + to_string(numberOfRefuses);
				}
				else if (command == "shutdown")
				{
					connectionsAllowed = false;
					while (true)
					{
						if (numberOfActiveConnections == 0)
						{
							break;
						}
					}
					exitRequested = true;
					return;
				}
				else if (command == "exit")
				{
					exitRequested = true;
					return;
				}
				else if (command == "disconnect")
				{
					break;
				}
				else
				{
					response = "Unknown command";
				}

				// Send response
				int lobuf = send(cS, response.c_str(), strlen(response.c_str()) + 1, NULL);
				if (lobuf == SOCKET_ERROR)
				{
					throw  SetErrorMsgText("send:", WSAGetLastError());
				}
			}
			// Close sockets
			if (closesocket(cS) == SOCKET_ERROR)
			{
				throw SetErrorMsgText("closesocket:", WSAGetLastError());
			}
			if (closesocket(TCPSocket) == SOCKET_ERROR)
			{
				throw SetErrorMsgText("closesocket:", WSAGetLastError());
			}
			if (exitRequested)
			{
				break;
			}
		}
		catch (string errorMsgText)
		{
			cout << endl << "WSAGetLastError: " << errorMsgText << endl;
			break;
		}
	}
}