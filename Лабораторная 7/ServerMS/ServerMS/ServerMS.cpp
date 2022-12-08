#include <iostream>
#include <Windows.h>
#include "Error.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, "");

	HANDLE hM;
	DWORD rb;
	char rbuf[100];
	LPCTSTR SlotName = TEXT("\\\\.\\mailslot\\Post");
	try
	{
		if ((hM = CreateMailslot(SlotName,
			300,
			//MAILSLOT_WAIT_FOREVER,
			180000,// 5 минуты
			NULL)) == INVALID_HANDLE_VALUE)
			throw SetPipeError("Ошибка мейлслота:", GetLastError());
		cout << "Мейлслот-сервер подключен:" << endl;
		cout << "Чтение доступно в течение 5 минут.." << endl;
		while (true)
		{
			if (!ReadFile(hM,
				rbuf,
				sizeof(rbuf),
				&rb, // прочитано
				NULL))
				throw SetPipeError("ReadFileError:", GetLastError());
			cout << rbuf << endl;
		}
		CloseHandle(hM);
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}
}