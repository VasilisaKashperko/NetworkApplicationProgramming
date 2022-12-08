#define _CRT_SECURE_NO_WARNING
#include <iostream>
#include <Windows.h>
#include "ErrorHandling.h"
#include <tchar.h>

#define BUFSIZE 512

using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	system("mode con cols=80 lines=25");
	LPCWSTR lpvMessage = TEXT("Сообщение от клиента");
	DWORD  cbRead, cbToWrite, cbWritten;
	TCHAR  chBuf[BUFSIZE];
	BOOL   fSuccess = FALSE;

	HANDLE hPipe;
	try
	{
		if ((hPipe = CreateFile(
			TEXT("\\\\.\\pipe\\Example"),
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, NULL,
			NULL)) == INVALID_HANDLE_VALUE)
			throw SetPipeError("createfile:", GetLastError());

		cout << "Соединение установлено" << endl;

		int num = 0;
		cout << "Введите число: ";
		cin >> num;

		clock_t start, stop;
		start = clock();

		for (int i = num; i > 0; i--)
		{
			char hello[] = "Сообщение от клиента ";
			char numberstring[(((sizeof i) * CHAR_BIT) + 2) / 3 + 2];
			sprintf_s(numberstring, "%d", i);

			char result[BUFSIZE];
			strcpy_s(result, hello);
			strcat_s(result, numberstring);

			wchar_t* wtext = new wchar_t[BUFSIZE];

			size_t outSize;
			mbstowcs_s(&outSize, wtext, BUFSIZE, result, BUFSIZE - 1);
			lpvMessage = wtext;

			cbToWrite = (lstrlen(lpvMessage) + 1) * sizeof(TCHAR);

			fSuccess = WriteFile(
				hPipe,
				lpvMessage,
				cbToWrite,
				&cbWritten,
				NULL);

			if (!fSuccess)
			{
				throw SetPipeError("writefile:", GetLastError());
			}

			do
			{
				fSuccess = ReadFile(
					hPipe,
					chBuf,
					BUFSIZE * sizeof(TCHAR),
					&cbRead,
					NULL);

				_tprintf(TEXT("%s\n"), chBuf);
			} while (!fSuccess);

			if (!fSuccess)
			{
				throw SetPipeError("readfile:", GetLastError());
			}
		}


		stop = clock();
		cout << (stop - start) << " - скорость" << endl;

		lpvMessage = TEXT("");

		cbToWrite = (lstrlen(lpvMessage) + 1) * sizeof(TCHAR);

		fSuccess = WriteFile(
			hPipe,
			lpvMessage,
			cbToWrite,
			&cbWritten,
			NULL);

		if (!fSuccess)
		{
			throw SetPipeError("writefile:", GetLastError());
		}

		cout << "Отсоединено" << endl;
		CloseHandle(hPipe);
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}

}