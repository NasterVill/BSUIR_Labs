#define _CRT_SECURE_NO_WARNINGS

#define TRUE 1
#define SLEEP_TIME 500

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

void printTime(char* processName);

void main(int argc, char** argv)
{
	STARTUPINFO sturtupInfo;
	PROCESS_INFORMATION processInfo;

	char childName[] = "Child";

	memset(&sturtupInfo, 0, sizeof(sturtupInfo));
	memset(&processInfo, 0, sizeof(processInfo));
	sturtupInfo.cb = sizeof(sturtupInfo);

	if (strcmp(childName, argv[0]) == 0)
	{
		while (!_kbhit())
		{
			printTime(childName);
		}

		printf("\n%s ID : %d\n", childName, GetCurrentProcessId());
		system("pause");

		return;
	}

	if (!CreateProcess(argv[0],		// Имя исполняемого модуля
		childName,					// Передача параметра(ов) командной строки новому процессу.
		NULL,						// Установка атрибутов защиты для нового приложения.
		NULL,						// Устанвока атрибутов защиты для первого потока созданного приложением.
		FALSE,						// Флаг наследования от процесса производящего запуск.
		CREATE_NEW_CONSOLE,			// Новый процесс получает новую консоль вместо родительской. 
		NULL,						// Указывает на блок памяти с переменными окуржения(значения в виде строк с нулевым окончанием)
		NULL,						// Рабочий каталог нового процесса
		&sturtupInfo,				// Используется для настройки свойств процесса (расположения окон и заголовока)
		&processInfo)				// Структура с информацией о процессе (сюда записываются идентификторы и дескрипторы новго процесса и основного его потока)
	   )
	{
		printf("CreateProcess function failed with code : %d\n", GetLastError());
		system("pause");
		exit(EXIT_FAILURE);
	}

	while (TRUE)
	{
		if (WaitForSingleObject(processInfo.hProcess, 100) == WAIT_OBJECT_0)
		{
			printf("\nChild process was terminated\n");
			system("pause");

			CloseHandle(processInfo.hProcess);
			CloseHandle(processInfo.hThread);

			return;
		}
		printTime("Parent");
	}
}

void printTime(char* processName)
{
	time_t systemTime;
	struct tm* timeInfo;

	time(&systemTime);
	timeInfo = localtime(&systemTime);

	system("cls");
	printf("%s time : %s\r", processName, asctime(timeInfo));

	Sleep(SLEEP_TIME);
}