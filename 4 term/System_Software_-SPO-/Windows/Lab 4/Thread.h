#ifndef THREAD_H
#define THREAD_H
#include <cstring>
#include <string>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <list>
#include <windows.h>

namespace CustomThread
{
	class Thread
	{
	public:
		static const std::string kSynchronizeEveventName;
		static const int kSwitchDelay;
		static const int kPrintDelay;

	public:
		struct ThreadData
		{
			LPCRITICAL_SECTION critical_section;
			const char* event_name;
			int* run_flag;
			int* last_thread;
			DWORD thread_id;
		};

	public:
		static void SetCriticalSection(LPCRITICAL_SECTION critical_section);
		static void SetStartUpRoutine(PTHREAD_START_ROUTINE start_routine);
		static void SetStackSize(size_t stack_size);

		void Invoke();
		void Terminate();

		Thread();
		Thread(const Thread&) = delete;
		Thread& operator=(const Thread&) = delete;
		~Thread();

	private:
		void InitializeData();
		void DeleteData();

	private:
		HANDLE thread_handle_;
		ThreadData* data_;

	private:
		static int last_thread_;
		static int stack_size_;
		static LPCRITICAL_SECTION critical_section_;
		static PTHREAD_START_ROUTINE start_routine_;
	};

	DWORD WINAPI ThreadRoutine(PVOID pvParam);
};

#endif THREAD_H

