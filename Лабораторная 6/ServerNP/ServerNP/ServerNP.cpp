#include <iostream>
#include <Windows.h>
#include "ErrorHandling.h"
#include <tchar.h>

#define BUFSIZE 512

using namespace std;

int main()
{
	system("mode con cols=80 lines=25");
	LPCWSTR lpvMessage = TEXT("Сообщение от сервера");
	setlocale(LC_ALL, "Russian");
	DWORD  cbRead, cbToWrite, cbWritten;
	TCHAR  chBuf[BUFSIZE];
	TCHAR checkBuf[BUFSIZE] = TEXT("");
	BOOL   fSuccess = FALSE;

	HANDLE hPipe;

	bool multi = true;

	try
	{
		do
		{
			if ((hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Example"),
				PIPE_ACCESS_DUPLEX,
				PIPE_TYPE_MESSAGE | PIPE_WAIT,
				1, NULL, NULL,
				INFINITE, NULL)) == INVALID_HANDLE_VALUE)
				throw SetPipeError("create:", GetLastError());

			cout << "Ожидание клиента\n" << endl;

			if (!ConnectNamedPipe(hPipe, NULL))
				throw SetPipeError("connect:", GetLastError());

			cout << "Подключение успешно" << endl;

			do
			{
				do
				{
					fSuccess = ReadFile(
						hPipe,
						chBuf,
						BUFSIZE * sizeof(TCHAR),
						&cbRead,
						NULL);
				} while (!fSuccess);

				if (!fSuccess)
				{
					throw SetPipeError("readfile:", GetLastError());
				}

				if (!wcscmp(chBuf, checkBuf)) {
					break;
				}

				_tprintf(TEXT("%s\n"), chBuf);

				cbToWrite = (lstrlen(chBuf) + 1) * sizeof(TCHAR);

				fSuccess = WriteFile(
					hPipe,
					chBuf,
					cbToWrite,
					&cbWritten,
					NULL);

				if (!fSuccess)
				{
					throw SetPipeError("writefile:", GetLastError());
				}

			} while (wcscmp(chBuf, checkBuf) && multi);

			cout << "Клиент отсоединен" << endl;

			DisconnectNamedPipe(hPipe);
			CloseHandle(hPipe);
		} while (true);
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
		DisconnectNamedPipe(hPipe);
		CloseHandle(hPipe);
	}

}