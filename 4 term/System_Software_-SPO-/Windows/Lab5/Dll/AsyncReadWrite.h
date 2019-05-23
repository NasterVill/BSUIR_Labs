#ifndef ASYNC_READ_WRITE_H
#define ASYNC_READ_WRITE_H

#ifndef RWDLL_H
#define RWDLL_H

//is done to import/export only public methods
#ifdef ASYNC_READ_WRITE_EXPORTS
//Enabled as "export" while compiling the dll project
#define ASYNC_READ_WRITE_API __declspec(dllexport)
#else
//Enabled as "import" in the Client side for using already created dll file
#define ASYNC_READ_WRITE_API __declspec(dllimport)
#endif

#endif

#include <string>
#include <vector>
#include <utility>
#include <Windows.h>
#include "IAsyncReadWrite.h"

namespace async_read_write
{

	class AsyncReadWrite : public IAsyncReadWrite
	{
	private:
	private:
		struct ThreadData
		{
			AsyncReadWrite* caller_instance;
			HANDLE file;
			int file_number;
			OVERLAPPED overlapped;
		};

		struct ReadFileData
		{
			int offset;
			int size;
			char* data;
		};

	public:
		int ConcatFiles(std::string source_folder_path, std::string output_file_path);
		void Release();

	private:
		static DWORD WINAPI Read(LPVOID param);
		static DWORD WINAPI Write(LPVOID param);

	private:
		void ReleaseData();
		std::vector<std::string> GetFilesFromSource(const std::string& source_folder);

	private:
		AsyncReadWrite(const AsyncReadWrite&) = delete;
		AsyncReadWrite& operator=(const AsyncReadWrite&) = delete;

	public:
		AsyncReadWrite();
		~AsyncReadWrite();

	private:
		int files_to_write_;
		std::vector<std::string> source_file_names_;
		std::string output_file_;

		std::vector<ThreadData*> thread_data_vector_;
		std::vector<ReadFileData*> read_files_data_;
		std::vector<HANDLE> read_events_;

		std::vector<HANDLE> reading_threads_;
		HANDLE writing_thread_;
	};

}

extern "C" ASYNC_READ_WRITE_API IAsyncReadWrite* __cdecl AsyncReadWriteFactory();

#endif