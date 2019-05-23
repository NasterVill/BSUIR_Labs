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
		/*Доступ к объекту семафора. Функция не работает, если дескриптор безопасности указанного
		объекта не разрешает запрашиваемый доступ для вызывающего процесса*/
		SEMAPHORE_ALL_ACCESS,
		/*Если это значение TRUE, процессы, созданные этим процессом, наследуют дескриптор. В противном случае процессы не наследуют этот дескриптор.*/
		FALSE,
		"Work");
	HANDLE Close = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "Close");
	HANDLE WINAPI FileProjection = OpenFileMapping(
		/*Доступ к объекту сопоставления файлов. Этот доступ проверяется на любой дескриптор безопасности в объекте сопоставления целевого файла.*/
		FILE_MAP_ALL_ACCESS,
		/*Не наследует дескриптор*/
		FALSE,
		/*Имя объекта сопоставления файлов, который нужно открыть. Если с этим именем есть открытый дескриптор объекта
		сопоставления файлов, а дескриптор безопасности в объекте сопоставления не
		конфликтует с параметром dwDesiredAccess, операция открытия успешно завершается.*/
		"FileProjection");

	LPVOID pointer;
	pointer = MapViewOfFile(FileProjection,
		/*Отображается представление чтения / записи файла. Объект сопоставления файлов должен быть
		создан с защитой PAGE_READWRITE или PAGE_EXECUTE_READWRITE.*/
		FILE_MAP_ALL_ACCESS,
		/*DWORD верхнего порядка смещения файла, где начинается представление.*/
		0,
		/*DWORD младшего порядка для смещения файла, где должно начинаться представление.
		Комбинация высоких и низких смещений должна указывать смещение в сопоставлении файлов.
		Они также должны соответствовать гранулярности распределения памяти в системе.
		То есть, смещение должно быть кратным гранулярности распределения.*/
		0,
		/*Число байтов сопоставления файлов для сопоставления с представлением.
		Все байты должны быть в максимальном размере, указанном CreateFileMapping.
		Если этот параметр равен 0 (ноль), отображение продолжается от указанного смещения до конца сопоставления файлов.*/
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
		/*Элемент lpSecurityDescriptor структуры указывает дескриптор безопасности для нового семафора.
		Если этот параметр равен NULL, семафор получает дескриптор безопасности по умолчанию.*/
		NULL,
		/*Начальный счет для объекта семафора. Это значение должно быть больше или равно нулю и меньше или равно lMaximumCount.
		Состояние семафора сигнализируется, когда его количество больше нуля и не имеет значения, когда оно равно нулю.
		Счет уменьшается на единицу, когда функция ожидания освобождает поток, ожидающий семафор.
		Счет увеличивается на указанное количество, вызывая функцию ReleaseSemaphore.*/
		0,
		/*Максимальный счетчик для объекта семафора. Это значение должно быть больше нуля.*/
		1,
		/*Если lpName соответствует имени существующего объекта семафора, эта функция запрашивает право доступа SEMAPHORE_ALL_ACCESS.
		В этом случае параметры lInitialCount и lMaximumCount игнорируются, поскольку они уже были установлены процессом создания.
		Если параметр lpSemaphoreAttributes не равен NULL, он определяет, можно ли наследовать дескриптор, но его элемент дескриптора безопасности игнорируется.*/
		"Work");

	HANDLE Close = CreateSemaphore(NULL, 0, 1, "Close");

	HANDLE WINAPI FileProjection = CreateFileMapping(
		/*Дескриптор файла, из которого можно создать объект сопоставления файлов.
		Если hFile является INVALID_HANDLE_VALUE, вызывающий процесс должен также указать размер для объекта сопоставления файлов
		в параметрах dwMaximumSizeHigh и dwMaximumSizeLow. В этом случае CreateFileMapping создает объект сопоставления файлов
		указанного размера, который поддерживается файлом подкачки системы, а не файлом в файловой системе.*/
		INVALID_HANDLE_VALUE,
		/*Указатель на структуру SECURITY_ATTRIBUTES, которая определяет, может ли возвращенный дескриптор наследоваться дочерними процессами.
		Член lpSecurityDescriptor структуры SECURITY_ATTRIBUTES указывает дескриптор безопасности для нового объекта сопоставления файлов.
		Если lpAttributes NULL, дескриптор не может быть унаследован, а объект сопоставления файлов получает дескриптор безопасности по умолчанию.*/
		NULL,
		/*Указывает защиту страницы объекта сопоставления файлов. Все сопоставленные виды объекта должны быть совместимы с этой защитой.
		PAGE_READWRITE позволяет отображать представления для чтения только для чтения, копирования на запись или доступа для чтения / записи.*/
		PAGE_READWRITE,
		/*DWORD максимального размера объекта сопоставления файлов.*/
		constants::kDwMaximumSizeLow,
		/*Минимальное значение DWORD для максимального размера объекта сопоставления файлов.
		Если этот параметр и dwMaximumSizeHigh равны 0 (ноль), максимальный размер объекта сопоставления файлов равен текущему размеру файла,
		который hFile идентифицирует.*/
		constants::kDwMaximumSizeHigh,
		/*Имя объекта сопоставления файлов.
		Если этот параметр соответствует имени существующего объекта сопоставления,
		функция запрашивает доступ к объекту с защитой, которую определяет функция flProtect.*/
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
				ReleaseSemaphore(Close, 1, NULL);	//Увеличивает количество указанного объекта семафора на определенное значение.
				ReleaseSemaphore(Work, 1, NULL);
				WaitForSingleObject(pi.hProcess, INFINITE);
				UnmapViewOfFile(memoryMap);			//Отключает сопоставленный вид файла из адресного пространства вызывающего процесса.
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

