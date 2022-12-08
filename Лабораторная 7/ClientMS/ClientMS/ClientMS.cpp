#include <iostream>
#include <Windows.h>
#include "Error.h"

using namespace std;

int main()
{
	setlocale(LC_ALL, "");


	HANDLE hM;
	DWORD wb; // длина сообщения
	char wbuf[] = "Сообщение";
	LPCTSTR SlotName = TEXT("\\\\.\\mailslot\\Post");
	try
	{
		if ((hM = CreateFile(SlotName,
			GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			NULL, NULL)) == INVALID_HANDLE_VALUE)
			throw SetPipeError("Генерацияя ошибки файла:", GetLastError());
		cout << "Клиент мейлслота подключен" << endl;

		clock_t start, stop;
		start = clock();

		for (int i = 0; i < 10; i++)
			if (!WriteFile(hM,
				wbuf,
				sizeof(wbuf),
				&wb, // записано
				NULL))
				throw SetPipeError("WriteFileError:", GetLastError());


		stop = clock();
		cout << stop - start << " милисекунд" << endl;
		CloseHandle(hM);
	}
	catch (string ErrorPipeText)
	{
		cout << endl << ErrorPipeText;
	}
}