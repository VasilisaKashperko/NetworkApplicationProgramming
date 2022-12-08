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
	DWORD  cbRead, cbToWrite, cbWritten;
	TCHAR  chBuf[BUFSIZE] = TEXT("Сообщение от клиента NPct");
	TCHAR  zeroBuf[BUFSIZE] = TEXT("");
	LPTSTR lpvMessage = chBuf;
	BOOL   fSuccess = FALSE;

	HANDLE hPipe;
	try
	{
		cout << "Соединение успешно" << endl;

		clock_t start, stop;
		start = clock();

		cbToWrite = (lstrlen(lpvMessage) + 1) * sizeof(TCHAR);

		fSuccess = CallNamedPipe(
			TEXT("\\\\Vasilisik\\pipe\\Example"),
			lpvMessage,
			(lstrlen(lpvMessage) + 1) * sizeof(TCHAR), 
			chBuf,  
			BUFSIZE * sizeof(TCHAR), 
			&cbRead,   
			20000); 

		_tprintf(TEXT("%s\n"), chBuf);

		if (!fSuccess)
		{
			throw SetPipeError("transact:", GetLastError());
		}

		stop = clock();
		cout << (stop - start) << " - время" << endl;

		cout << "Отключено" << endl;
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}
}