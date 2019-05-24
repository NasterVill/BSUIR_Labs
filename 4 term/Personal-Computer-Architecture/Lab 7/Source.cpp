#define _SCL_SECURE_NO_WARNINGS
#include <cstdio>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

const int kTimeout = 1000;

void Server(char* path);													// �������� ������� (COM1)
void Client();																// �������� ������� (COM2)

int main(int argc, char* argv[])
{
	switch (argc) 
	{
	case 1:																	// ���� ������� ������� �� ������
		Server(argv[0]);
		break;
	default:
		Client();
		break;
	}
}

void Server(char* path) 
{
	string name = "COM1";
	STARTUPINFO si;															// ��������� ��� �������� �������� �������.
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION childProcessInfo;									// ��������� ��� �������� �������� �������.
	ZeroMemory(&childProcessInfo, sizeof(childProcessInfo));

	HANDLE handler;
	HANDLE Semaphores[3];

	char buffer[20];
	int bufferSize = sizeof(buffer);

	string message;

	cout << "Server\n\n";

	Semaphores[0] = CreateSemaphore(NULL, 0, 1, "SEMAPHORE");				// ������� ����������.
	Semaphores[1] = CreateSemaphore(NULL, 0, 1, "SEMAPHORE_END");			// ������� ����������.
	Semaphores[2] = CreateSemaphore(NULL, 0, 1, "SEMAPHORE_EXIT");			// ������� ������.

	handler = CreateFile(
		name.c_str(),														// ��� ������������ �����.
		GENERIC_READ | GENERIC_WRITE,										// ��� ������� � �����.
		0,																	// ��������� ����������� �������.
		NULL,																// �������� ������ �����.
		OPEN_EXISTING,														// ����� ������������.
		FILE_ATTRIBUTE_NORMAL,												// ����������� ����� ������.
		NULL																// ��������� ����� �������.
	);

	CreateProcess(
		path,																// ��� ������������ ������.
		(LPSTR)" Client",												    // ��������� ��������� ������.
		NULL,																// ����������� ��������� ������ ��� ������ ����������.
		NULL,																// ����������� ��������� ������ ��� ������� ������ ���������� �����������.
		FALSE,																// ���� ������������ �� �������� ������������� ������.
		CREATE_NEW_CONSOLE,													// ����� ������� �������� ����� ������� ������ ����, ����� ������������ ������������. 
		NULL,																// ��������� �� ���� �����. ���� ����� ��� ������ ���������� ���=�������� � ���� ����� � ������� ����������.
		NULL,																// ��������� ������� ���� � �������.
		&si,																// ������������ ��� ��������� ������� ��������, �������� ������������ ���� � ���������.
		&childProcessInfo													// ��������� PROCESS_INFORMATION � ����������� � ��������. ����� ��������� Windows.
	);

	SetCommMask(handler, EV_RXCHAR);										// ������������� ����� �� ������� �����.
	SetupComm(handler, 1500, 1500);											// �������������� ���������������� ��������� ��� ��������� ���������� (����������, ����� �����-������)

	COMMTIMEOUTS CommTimeOuts;												// ���������, ��������������� ��������� ��������� ����������������� �����.
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;							// M����������� ����� ��� ��������� ����� ������������ ���� �������� �� ����� �����.
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;							// ���������, ������������, ����� ��������� ������ ������ ������� ������� ��� �������� ������.
	CommTimeOuts.ReadTotalTimeoutConstant = kTimeout;						// ���������, ������������, ����� ��������� ������ (������������) ������ ������� ������� ��� �������� ������.
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;							// ���������, ������������, ����� ��������� ������ ������ ������� ������� ��� �������� ������.
	CommTimeOuts.WriteTotalTimeoutConstant = kTimeout;						// ���������, ������������, ����� ��������� ������ ������ ������� ������� ��� �������� ������.

	if (!SetCommTimeouts(handler, &CommTimeOuts))
	{
		CloseHandle(handler);
		handler = INVALID_HANDLE_VALUE;
		return;
	}

	DCB ComDCM;																// ���������, ��������������� �������� ��������������������������� �����. 
	memset(&ComDCM, 0, sizeof(ComDCM));										// ��������� ������ ��� ���������.
	ComDCM.DCBlength = sizeof(DCB);											// ������ �����, � ������, ���������.
	GetCommState(handler, &ComDCM);											// ��������� ������ � ������� ���������� ����������� �������� ��� ���������� ����������.
	ComDCM.BaudRate = DWORD(9600);											// �������� �������� ������.
	ComDCM.ByteSize = 8;													// ���������� ����� �������������� ��� � ������������ � ����������� ������.
	ComDCM.Parity = NOPARITY;												// ���������� ����� ����� �������� �������� (��� ��������� �����������).
	ComDCM.StopBits = ONESTOPBIT;											// ������ ���������� �������� ��� (���� ���). 
	ComDCM.fAbortOnError = TRUE;											// ������ ������������� ���� �������� ������/������ ��� ������������� ������.
	ComDCM.fDtrControl = DTR_CONTROL_DISABLE;								// ������ ����� ���������� ������� ��� ������� DTR.
	ComDCM.fRtsControl = RTS_CONTROL_DISABLE;								// ������ ����� ���������� ������� ��� ������� RTS.
	ComDCM.fBinary = TRUE;													// �������� �������� ����� ������.
	ComDCM.fParity = FALSE;													// �������� ����� �������� ��������.
	ComDCM.fInX = FALSE;													// ������ ������������� XON/XOFF ���������� ������� ��� ������.
	ComDCM.fOutX = FALSE;													// ������ ������������� XON/XOFF ���������� ������� ��� ��������.
	ComDCM.XonChar = 0;														// ������ ������ XON ������������ ��� ��� ������, ��� � ��� ��������.
	ComDCM.XoffChar = (unsigned char)0xFF;									// ������ ������ XOFF ������������ ��� ��� ������, ��� � ��� ��������.
	ComDCM.fErrorChar = FALSE;												// ������ ������, �������������� ��� ������ �������� � ��������� ���������.
	ComDCM.fNull = FALSE;													// ��������� �� ������������� ������ �������� � ������� �������� �� ������ ���������� ����� ErrorChar.
	ComDCM.fOutxCtsFlow = FALSE;											// �������� ����� �������� �� �������� CTS.
	ComDCM.fOutxDsrFlow = FALSE;											// �������� ����� �������� �� �������� DSR.
	ComDCM.XonLim = 128;													// ������ ����������� ����� �������� � �������� ������ ����� �������� ������� XON.
	ComDCM.XoffLim = 128;													// ���������� ������������ ���������� ���� � �������� ������ ����� �������� ������� XOFF.

	if (!SetCommState(handler, &ComDCM))
	{
		CloseHandle(handler);
		handler = INVALID_HANDLE_VALUE;
		return;
	}

	while (true)
	{
		DWORD NumberOfBytesWritten;

		cout << "Enter message: ";
		cin.clear();
		getline(cin, message);

		if (message == "Exit")
		{																	// ������� ������ �� ���������.
			ReleaseSemaphore(Semaphores[2], 1, NULL);						// ���������� �������� ������ (������ � ���������� ���������).
			WaitForSingleObject(childProcessInfo.hProcess, INFINITE);		// �������� ������� �� ��������� ��������.
			break;
		}

		ReleaseSemaphore(Semaphores[0], 1, NULL);							// ���������� �������� ��������� (������ � ���������� ���������).

		int NumberOfBlocks = message.size() / bufferSize + 1;
		WriteFile(handler, &NumberOfBlocks, sizeof(NumberOfBlocks), &NumberOfBytesWritten, NULL);

		int size = message.size();
		WriteFile(handler, &size, sizeof(size), &NumberOfBytesWritten, NULL);

		for (int i = 0; i < NumberOfBlocks; i++) 
		{
			message.copy(buffer, bufferSize, i*bufferSize);
			if (!WriteFile(handler, buffer, bufferSize, &NumberOfBytesWritten, NULL))
				cout << "Error.";
		}
		WaitForSingleObject(Semaphores[1], INFINITE);						// �������� �������� � ������������ ���������.
	}

	CloseHandle(handler);
	CloseHandle(Semaphores[0]);
	CloseHandle(Semaphores[1]);
	cout << "\n";
	system("pause");
	return;
}

void Client()
{
	HANDLE hMyPipe;
	HANDLE Semaphores[3];

	char buffer[20];
	int bufferSize = sizeof(buffer);

	string message;
	string name = "COM2";

	bool successFlag;
	Semaphores[0] = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, "SEMAPHORE");
	Semaphores[1] = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, "SEMAPHORE_END");
	Semaphores[2] = OpenSemaphore(SEMAPHORE_ALL_ACCESS, TRUE, "SEMAPHORE_EXIT");

	cout << "Client\n\n";

	hMyPipe = CreateFile(name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	while (true)
	{
		successFlag = TRUE;
		DWORD NumberOfBytesRead;
		message.clear();

		int index = WaitForMultipleObjects(3, Semaphores, FALSE, INFINITE) - WAIT_OBJECT_0;
		if (index == 2)
		{
			break;
		}

		int NumberOfBlocks;
		if (!ReadFile(hMyPipe, &NumberOfBlocks, sizeof(NumberOfBlocks), &NumberOfBytesRead, NULL)) break;

		int size;
		if (!ReadFile(hMyPipe, &size, sizeof(size), &NumberOfBytesRead, NULL)) break;

		for (int i = 0; i < NumberOfBlocks; i++)
		{
			successFlag = ReadFile(hMyPipe, buffer, bufferSize, &NumberOfBytesRead, NULL);
			if (!successFlag)
			{
				break;
			}
			message.append(buffer, bufferSize);
		}

		if (!successFlag)
		{
			break;
		}

		message.resize(size);

		for (int i = 0; i < size; i++)
		{
			cout << message[i];
		}
		cout << endl;
		ReleaseSemaphore(Semaphores[1], 1, NULL);
	}
	CloseHandle(hMyPipe);
	CloseHandle(Semaphores[0]);
	CloseHandle(Semaphores[1]);
	return;
}