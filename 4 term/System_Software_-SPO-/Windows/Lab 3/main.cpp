#include "Header.h"

void main(int argc, char* argv[]) {
	if (argc > 1) {
		Child();
	}
	else {
		Parent(argv[0]);
	}
}

void Child() {
	HANDLE Work = OpenSemaphore(
		/*������ � ������� ��������. ������� �� ��������, ���� ���������� ������������ ����������
		������� �� ��������� ������������� ������ ��� ����������� ��������*/
		SEMAPHORE_ALL_ACCESS,
		/*���� ��� �������� TRUE, ��������, ��������� ���� ���������, ��������� ����������. � ��������� ������ �������� �� ��������� ���� ����������.*/
		FALSE,
		"Work");
	HANDLE Close = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Close");
	HANDLE WINAPI FileProjection = OpenFileMapping(
		/*������ � ������� ������������� ������. ���� ������ ����������� �� ����� ���������� ������������ � ������� ������������� �������� �����.*/
		FILE_MAP_ALL_ACCESS,
		/*�� ��������� ����������*/
		FALSE,
		/*��� ������� ������������� ������, ������� ����� �������. ���� � ���� ������ ���� �������� ���������� �������
		������������� ������, � ���������� ������������ � ������� ������������� ��
		����������� � ���������� dwDesiredAccess, �������� �������� ������� �����������.*/
		"FileProjection");

	LPVOID pointer;
	pointer = MapViewOfFile(FileProjection,
		/*������������ ������������� ������ / ������ �����. ������ ������������� ������ ������ ����
		������ � ������� PAGE_READWRITE ��� PAGE_EXECUTE_READWRITE.*/
		FILE_MAP_ALL_ACCESS,
		/*DWORD �������� ������� �������� �����, ��� ���������� �������������.*/
		0,
		/*DWORD �������� ������� ��� �������� �����, ��� ������ ���������� �������������.
		���������� ������� � ������ �������� ������ ��������� �������� � ������������� ������.
		��� ����� ������ ��������������� ������������� ������������� ������ � �������.
		�� ����, �������� ������ ���� ������� ������������� �������������.*/
		0,
		/*����� ������ ������������� ������ ��� ������������� � ��������������.
		��� ����� ������ ���� � ������������ �������, ��������� CreateFileMapping.
		���� ���� �������� ����� 0 (����), ����������� ������������ �� ���������� �������� �� ����� ������������� ������.*/
		0);

	char* stringRepresentation = (char*)pointer;

	while (true)
	{
		WaitForSingleObject(Work, INFINITE);
		if (WaitForSingleObject(Close, constants::kDelay) == WAIT_OBJECT_0) {
			CloseHandle(Work);
			CloseHandle(Close);
			UnmapViewOfFile(pointer);
			CloseHandle(FileProjection);
			return;
		}

		cout << "Client got: " << stringRepresentation << endl;

		ReleaseSemaphore(Work, 1, NULL);
	}
}

void Parent(char* input) {
	HANDLE Work = CreateSemaphore(
		/*������� lpSecurityDescriptor ��������� ��������� ���������� ������������ ��� ������ ��������.
		���� ���� �������� ����� NULL, ������� �������� ���������� ������������ �� ���������.*/
		NULL,
		/*��������� ���� ��� ������� ��������. ��� �������� ������ ���� ������ ��� ����� ���� � ������ ��� ����� lMaximumCount.
		��������� �������� ���������������, ����� ��� ���������� ������ ���� � �� ����� ��������, ����� ��� ����� ����.
		���� ����������� �� �������, ����� ������� �������� ����������� �����, ��������� �������.
		���� ������������� �� ��������� ����������, ������� ������� ReleaseSemaphore.*/
		0,
		/*������������ ������� ��� ������� ��������. ��� �������� ������ ���� ������ ����.*/
		1,
		/*���� lpName ������������� ����� ������������� ������� ��������, ��� ������� ����������� ����� ������� SEMAPHORE_ALL_ACCESS.
		� ���� ������ ��������� lInitialCount � lMaximumCount ������������, ��������� ��� ��� ���� ����������� ��������� ��������.
		���� �������� lpSemaphoreAttributes �� ����� NULL, �� ����������, ����� �� ����������� ����������, �� ��� ������� ����������� ������������ ������������.*/
		"Work");

	HANDLE Close = CreateSemaphore(NULL, 0, 1, "Close");

	HANDLE WINAPI FileProjection = CreateFileMapping(
		/*���������� �����, �� �������� ����� ������� ������ ������������� ������.
		���� hFile �������� INVALID_HANDLE_VALUE, ���������� ������� ������ ����� ������� ������ ��� ������� ������������� ������
		� ���������� dwMaximumSizeHigh � dwMaximumSizeLow. � ���� ������ CreateFileMapping ������� ������ ������������� ������
		���������� �������, ������� �������������� ������ �������� �������, � �� ������ � �������� �������.*/
		INVALID_HANDLE_VALUE,
		/*��������� �� ��������� SECURITY_ATTRIBUTES, ������� ����������, ����� �� ������������ ���������� ������������� ��������� ����������.
		���� lpSecurityDescriptor ��������� SECURITY_ATTRIBUTES ��������� ���������� ������������ ��� ������ ������� ������������� ������.
		���� lpAttributes NULL, ���������� �� ����� ���� �����������, � ������ ������������� ������ �������� ���������� ������������ �� ���������.*/
		NULL,
		/*��������� ������ �������� ������� ������������� ������. ��� �������������� ���� ������� ������ ���� ���������� � ���� �������.
		PAGE_READWRITE ��������� ���������� ������������� ��� ������ ������ ��� ������, ����������� �� ������ ��� ������� ��� ������ / ������.*/
		PAGE_READWRITE,
		/*DWORD ������������� ������� ������� ������������� ������.*/
		constants::kDwMaximumSizeLow,
		/*����������� �������� DWORD ��� ������������� ������� ������� ������������� ������.
		���� ���� �������� � dwMaximumSizeHigh ����� 0 (����), ������������ ������ ������� ������������� ������ ����� �������� ������� �����,
		������� hFile ��������������.*/
		constants::kDwMaximumSizeHigh,
		/*��� ������� ������������� ������.
		���� ���� �������� ������������� ����� ������������� ������� �������������,
		������� ����������� ������ � ������� � �������, ������� ���������� ������� flProtect.*/
		"FileProjection");

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi;

	if (!CreateProcess(input, (LPSTR)"child process", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		cout << "Create Process failed" << GetLastError() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	LPVOID memoryMap;
	memoryMap = MapViewOfFile(FileProjection, FILE_MAP_ALL_ACCESS, 0, 0, 0);

	char* stringRepresentation = (char*)memoryMap;

	int currPos = 0;
	bool readyForInput = true;
	string buffString;
	buffString.resize(constants::kMaxSize, '\0');

	while (true)
	{
		if (readyForInput)
		{
			currPos = 0;
			cout << "Server: Please, enter the string" << endl;
			getline(cin, buffString);
			readyForInput = false;
		}

		string tempBuff;
		int newLength = 0;
		tempBuff.append(buffString, 0, constants::kMaxSize - 1);
		currPos = tempBuff.length();

		strcpy(stringRepresentation, const_cast<char*>(tempBuff.c_str()));
		stringRepresentation[tempBuff.length()] = '\0';

		tempBuff.clear();
		newLength = buffString.length() - currPos;
		if (newLength > 0)
		{
			tempBuff.append(buffString, currPos, newLength);
		}
		buffString = tempBuff;

		ReleaseSemaphore(Work, 1, NULL);
		WaitForSingleObject(Work, INFINITE);

		if (buffString.empty())
		{
			readyForInput = true;
			cout << "\nExit (0)" << endl;
			if (cin.get() == '0')
			{
				ReleaseSemaphore(Close, 1, NULL);	//����������� ���������� ���������� ������� �������� �� ������������ ��������.
				ReleaseSemaphore(Work, 1, NULL);
				WaitForSingleObject(pi.hProcess, INFINITE);
				UnmapViewOfFile(memoryMap);			//��������� �������������� ��� ����� �� ��������� ������������ ����������� ��������.
				CloseHandle(pi.hThread);
				CloseHandle(pi.hProcess);
				CloseHandle(Close);
				CloseHandle(Work);
				CloseHandle(FileProjection);
				return;
			}

			buffString.clear();

			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << endl;
		}
	}
}

