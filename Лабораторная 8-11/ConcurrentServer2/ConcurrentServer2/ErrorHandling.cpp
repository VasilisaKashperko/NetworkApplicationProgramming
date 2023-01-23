#include <iostream>
#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include "Header.h"

using namespace std;

string GetErrorMsgText(int code)
{
    string msgText;
    switch (code)
    {
    case WSAEINTR:
        msgText = "WSAEINTR";
        break;
    case WSAEACCES:
        msgText = "WSAEACCES";
        break;
    case WSAEFAULT:
        msgText = "WSAEFAULT";
        break;
    case WSAEINVAL:
        msgText = "WSAEINVAL";
        break;
    case WSAENOTSOCK:
        msgText = "WSAENOTSOCK";
        break;
    case WSAEMSGSIZE:
        msgText = "WSAEMSGSIZE";
        break;
    case WSAEPROTOTYPE:
        msgText = "WSAEPROTOTYPE";
        break;
    case WSAENOPROTOOPT:
        msgText = "WSAENOPROTOOPT";
        break;
    case WSAEADDRINUSE:
        msgText = "WSAEADDRINUSE";
        break;
    case WSAEADDRNOTAVAIL:
        msgText = "WSAEADDRNOTAVAIL";
        break;
    case WSAEOPNOTSUPP:
        msgText = "WSAEOPNOTSUPP";
        break;
    case WSAENETDOWN:
        msgText = "WSAENETDOWN";
        break;
    case WSAENETUNREACH:
        msgText = "WSAENETUNREACH";
        break;
    case WSAENETRESET:
        msgText = "WSAENETRESET";
        break;
    case WSAECONNRESET:
        msgText = "WSAECONNRESET";
        break;
    case WSAEISCONN:
        msgText = "WSAEISCONN";
        break;
    case WSAENOTCONN:
        msgText = "WSAENOTCONN";
        break;
    case WSAESHUTDOWN:
        msgText = "WSAESHUTDOWN";
        break;
    case WSAETIMEDOUT:
        msgText = "WSAETIMEDOUT";
        break;
    case WSAECONNREFUSED:
        msgText = "WSAECONNREFUSED";
        break;
    case WSASYSNOTREADY:
        msgText = "WSASYSNOTREADY";
        break;
    case WSANOTINITIALISED:
        msgText = "WSANOTINITIALISED";
        break;
    case WSASYSCALLFAILURE:
        msgText = "WSASYSCALLFAILURE";
        break;
    case WSAEDISCON:
        msgText = "WSAEDISCON";
        break;
    case WSATYPE_NOT_FOUND:
        msgText = "WSATYPE_NOT_FOUND";
        break;
    case WSAHOST_NOT_FOUND:
        msgText = "WSAHOST_NOT_FOUND";
        break;
    case WSA_IO_INCOMPLETE:
        msgText = "WSA_IO_INCOMPLETE";
        break;
    case WSA_IO_PENDING:
        msgText = "WSA_IO_PENDING";
        break;
    case WSA_OPERATION_ABORTED:
        msgText = "WSA_OPERATION_ABORTED";
        break;
    default:
        msgText = "***ERROR***";
        break;
    }
    return msgText;
}

string SetErrorMsgText(string msgText, int code)
{
    return msgText + " " + GetErrorMsgText(code);
}