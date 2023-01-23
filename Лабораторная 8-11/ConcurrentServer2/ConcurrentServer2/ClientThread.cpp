#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#include "Header.h"
#include <string>
#include <ctime>
#include <iostream>

using namespace std;

void ClientThread(string pipeName)
{
    // We must use wide string to connect to named pipe
    string pipeStr = "\\\\.\\pipe\\" + pipeName;
    wstring pipeWstr;
    for (int i = 0; i < pipeStr.length(); i++)
    {
        pipeWstr += wchar_t(pipeStr[i]);
    }
    
    HANDLE hPipe;
    
    try
    {
        // Create named pipe
        hPipe = CreateNamedPipe
        (
            pipeWstr.c_str(),
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_WAIT,
            1, NULL, NULL,
            INFINITE, NULL
        );
        // Connect to named pipe
        if (!ConnectNamedPipe(hPipe, NULL))
        {
            throw "connect: unable to connect to named pipe";
        }
        if (hPipe == INVALID_HANDLE_VALUE)
        {
            throw "create: invalid named pipe handle value";
        }

        // For 'rand' command:
        srand((unsigned)time(NULL));
        while (true)
        {
            // Recieve command
            char rbuf[50];
            unsigned long lr;
            if (!ReadFile(hPipe, rbuf, sizeof(rbuf), &lr, NULL))
            {
                throw to_string(GetLastError());
            }
            string command = string(rbuf);
            // Check
            string response;
            if (command.find("echo") == 0)
            {
                response = command.substr(command.find(" ") + 1);
            }
            else if (command == "rand")
            {
                response = to_string(rand());
            }
            else if (command == "time")
            {
                time_t rawTime;
                tm* currentTime;
                char formatedTime[80];

                time(&rawTime);
                currentTime = localtime(&rawTime);
                strftime(formatedTime, sizeof(formatedTime), "%d.%m.%Y/%H:%M:%S", currentTime);

                response = string(formatedTime);
            }
            else if (command == "exit")
            {
                break;
            }
            else
            {
                response = "Unknown command";
            }
            // Writing
            unsigned long lw;
            if (!WriteFile(hPipe, response.c_str(), sizeof(response), &lw, NULL))
            {
                throw "writefile error";
            }
        }
    }
    catch (string errm)
    {
        cout << "Pipe " << pipeName << " disconnected" << endl;
    }
    catch (exception err)
    {
        cout << "Pipe " << pipeName << " disconnected" << endl;
    }

    //DisconnectNamedPipe(hPipe);
    //CloseHandle(hPipe);
    numberOfActiveConnections -= 1;
    return;
}