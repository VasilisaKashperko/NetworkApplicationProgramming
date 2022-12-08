#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <ctime>
#include "Error.h"
#include "Winsock2.h"  
#pragma comment(lib, "WS2_32.lib")

using namespace std;

bool GetRequestFromClient(char*, short, struct sockaddr*, int*);
bool AnswerToClient(char*, short, struct sockaddr*, int*);
void SearchServer(char*);


int main()
{
	setlocale(LC_ALL, "");
	system("color B0");

	WSADATA wsaData;
	SOCKET sS;
	SOCKADDR_IN serv;
	SOCKADDR_IN from;

	char name[50] = "Mine";
	serv.sin_family = AF_INET;
	serv.sin_port = htons(2000);
	serv.sin_addr.s_addr = INADDR_ANY;

	memset(&from, 0, sizeof(from)); // обнуление памяти
	int lfrom = sizeof(from);

	char hostname[25];
	hostent* clntInfo;
	memset(&clntInfo, 0, sizeof(clntInfo));

	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw  SetErrorMsgText("Запуск:\n", WSAGetLastError());

		cout << "Поиск серверов...\n" << endl;
		SearchServer(name);

		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Очистка:", WSAGetLastError());

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw  SetErrorMsgText("\nЗапуск:", WSAGetLastError());
		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("Сокет:", WSAGetLastError());
		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw  SetErrorMsgText("Бинд:", WSAGetLastError());
		if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
			throw SetErrorMsgText("Получение host name: ", WSAGetLastError());
		cout << "Host name сервера: " << hostname << endl;
		do
		{
			if (GetRequestFromClient(name, sS, (sockaddr*)&from, &lfrom))
			{
				clntInfo = gethostbyaddr((const char*)&from.sin_addr, 4, AF_INET);

				cout << endl;
				cout << "Socket клиента:" << endl;
				cout << "Hostname: " << clntInfo->h_name << endl;
				cout << "IP: " << inet_ntoa(from.sin_addr) << endl;
				cout << "Порт: " << htons(from.sin_port) << endl;
				cout << endl;
				if (AnswerToClient(name, sS, (sockaddr*)&from, &lfrom))
				{
					cout << "Успех" << endl;
				}
			}
			else
				cout << "Неправильный вызов имени" << endl;
		} while (true);

		if (closesocket(sS) == SOCKET_ERROR)
			throw  SetErrorMsgText("Закрыть socket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw  SetErrorMsgText("Очистка:", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << "\n" << errorMsgText << endl;
	}
}

bool GetRequestFromClient(char* name, short port, struct sockaddr* from, int* flen)
{
	int lbuf;
	char ibuf[50] = "";
	cout << "Ожидание сообщения..." << endl;

	while (true)
	{
		if (lbuf = recvfrom(port, ibuf, sizeof(ibuf), NULL, from, flen) == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSAETIMEDOUT)
				return false;
			else
				throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
		}
		if (strcmp(ibuf, name) == 0)
			return true;
		else
			return false;
	}
}

bool AnswerToClient(char* name, short port, struct sockaddr* to, int* lto)
{
	int lbuf;
	if (lbuf = sendto(port, name, strlen(name) + 1, NULL, to, *lto) == SOCKET_ERROR)
		throw SetErrorMsgText("Отправить кому: ", WSAGetLastError());

	return true;
}

void SearchServer(char* name)
{
	SOCKADDR_IN from;
	memset(&from, 0, sizeof(from));
	int lfrom = sizeof(from);
	SOCKET sock;
	int lbuf;
	int optval = 3000;

	if ((sock = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
	{
		throw SetErrorMsgText("socket: ", WSAGetLastError());
	}
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(optval)) == SOCKET_ERROR)
		throw SetErrorMsgText("opt: ", WSAGetLastError());

	SOCKADDR_IN all;
	all.sin_family = AF_INET;
	all.sin_port = htons(2000);
	all.sin_addr.s_addr = INADDR_BROADCAST;
	char ibuf[50];
	if (lbuf = sendto(sock, name, strlen(name) + 1, NULL, (sockaddr*)&all, sizeof(all)) == SOCKET_ERROR)
	{
		throw SetErrorMsgText("Отправить кому: ", WSAGetLastError());
	}
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&optval, sizeof(optval)) == SOCKET_ERROR)
		throw SetErrorMsgText("opt: ", WSAGetLastError());
	if (lbuf = recvfrom(sock, ibuf, sizeof(ibuf), NULL, (sockaddr*)&from, &lfrom) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAETIMEDOUT)
			throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
	}
	if (lbuf == 0)
	{
		cout << "Сервер socket: " << endl;
		cout << "IP: " << inet_ntoa(from.sin_addr) << endl;
		cout << "Порт: " << htons(from.sin_port) << endl;
	}
	if (closesocket(sock) == SOCKET_ERROR)
	{
		throw SetErrorMsgText("Закрыть socket: ", WSAGetLastError());
	}
}