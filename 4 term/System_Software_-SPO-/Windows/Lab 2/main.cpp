#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <Windows.h>
#include <list>

using namespace std;

const int DELAY = 100;
const int BUFF_SIZE = 10;
const int MAIN_DELAY = 500;
const int MAIN_PROCESS_ARGS = 1;

HANDLE createNewProcess(string source, int procNumber);
void printProcessSignature(string procNumber);

int main(int argc, char** argv) {
	if (argc > MAIN_PROCESS_ARGS) {	// if it is child
		HANDLE eventHdl = OpenEvent(EVENT_ALL_ACCESS, false, "writeEvent");
		string information = argv[1];

		while (true) {
			WaitForSingleObject(eventHdl, INFINITE);
			printProcessSignature(information);
			SetEvent(eventHdl);
		}
	}
	else {
		cout << "Create new process: +" << endl;
		cout << "Kill last process: -" << endl;
		cout << "Exit the programm: q" << endl;

		HANDLE writeEvent = CreateEvent(NULL, FALSE, true, "writeEvent");

		list <HANDLE> processList;

		while (true) {
			WaitForSingleObject(writeEvent, INFINITE);
			cout << endl;

			char temp;
			do {
				cin.clear();
				cin.get(temp);
			} while ( (temp != '+') && (temp != '-') && (temp != 'q') );

			switch (temp) {
			case '+':
				processList.push_front(createNewProcess(string(argv[0]), processList.size() + 1));
				break;
			case '-':
				if (!processList.empty()) {
					TerminateProcess(processList.front(), EXIT_SUCCESS);
					CloseHandle(processList.front());
					processList.pop_front();
				}
				else {
					cout << "List is empty, create process please" << endl;
				}
				break;
			case 'q':
				while (!processList.empty()) {
					TerminateProcess(processList.front(), EXIT_SUCCESS);
					CloseHandle(processList.front());
					processList.pop_front();
				}

				CloseHandle(writeEvent);
				return 0;
			}

			SetEvent(writeEvent);
			Sleep(MAIN_DELAY);
		}
	}
}

HANDLE createNewProcess(string source, int procNumber) {
	char commandArgs[BUFF_SIZE];
	sprintf(commandArgs + 1, "%d", procNumber);
	commandArgs[0] = ' ';

	STARTUPINFO sturtupInfo;
	PROCESS_INFORMATION processInfo;

	memset(&sturtupInfo, 0, sizeof(sturtupInfo));
	memset(&processInfo, 0, sizeof(processInfo));
	sturtupInfo.cb = sizeof(sturtupInfo);

	if (!CreateProcess(source.c_str(),		// Имя исполняемого модуля
		commandArgs,					// Передача параметра(ов) командной строки новому процессу.
		NULL,						// Установка атрибутов защиты для нового приложения.
		NULL,						// Устанвока атрибутов защиты для первого потока созданного приложением.
		FALSE,						// Флаг наследования от процесса производящего запуск.
		CREATE_NEW_CONSOLE,			// Новый процесс получает новую консоль вместо родительской. 
		NULL,						// Указывает на блок памяти с переменными окуржения(значения в виде строк с нулевым окончанием)
		NULL,						// Рабочий каталог нового процесса
		&sturtupInfo,				// Используется для настройки свойств процесса (расположения окон и заголовока)
		&processInfo)				// Структура с информацией о процессе (сюда записываются идентификторы и дескрипторы новго процесса и основного его потока)
		) {
		cout << "Error while creating new process!" << endl;
		Sleep(MAIN_DELAY);
		exit(EXIT_FAILURE);
	}

	return processInfo.hProcess;
}

void printProcessSignature(string procNumber) {
	for (int i = 0; i < procNumber.length(); i++) {
		cout << procNumber[i];
	}

	string message = " Process";
	for (int i = 0; i < message.length(); i++) {
		cout << message[i];
		Sleep(DELAY);
	}
	cout << endl;

	Sleep(DELAY);
}