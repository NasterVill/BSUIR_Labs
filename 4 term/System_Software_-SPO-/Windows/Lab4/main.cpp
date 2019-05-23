#include <iostream>
#include <windows.h>
#include <stdexcept>
#include <string>
#include <list>

#include "CSGuard.h"
#include "Thread.h"

using namespace std;
using namespace CustomThread;

int main(int argc, char** argv)
{
	char symbol;
	list<Thread*> threadlist;

	LPCRITICAL_SECTION critical_section = new CRITICAL_SECTION;
	InitializeCriticalSection(critical_section);
	PTHREAD_START_ROUTINE start_routine = CustomThread::ThreadRoutine;

	try
	{
		Thread::SetCriticalSection(critical_section);
		Thread::SetStartUpRoutine(start_routine);
		Thread::SetStackSize(256);

		cout << "Hello! This is the parent process." << endl;
		cout << "If you want to create new process, press '+'" << endl;
		cout << "If you want to delete last process, press '-'" << endl;
		cout << "If you want to quit, press 'q'" << endl;
		cout << "Press any key to begin..." << endl;

		cin.ignore();

		while (true)
		{
			cin.get(symbol);

			switch (symbol)
			{
			case '+':
			{
				Thread* thread = new Thread;
				thread->Invoke();
				threadlist.push_back(thread);
				break;
			}
			case '-':
			{
				if (threadlist.empty())
				{
					cout << "There are no subthreads to delete!" << endl;
				}
				else
				{
					threadlist.back()->Terminate();
					delete threadlist.back();
					threadlist.pop_back();
				}
				break;
			}
			case 'q':
			{
				if (!threadlist.empty())
				{
					for (auto &thread : threadlist)
					{
						thread->Terminate();
						delete thread;
					}
					threadlist.clear();

					DeleteCriticalSection(critical_section);
				}
				return 0;
			}
			default:
				continue;
			}
			cin.ignore();
		}
	}
	catch (exception &excepetion)
	{
		if (!threadlist.empty())
		{
			for (auto &thread : threadlist)
			{
				thread->Terminate();
				delete thread;
			}
			threadlist.clear();
		}
		DeleteCriticalSection(critical_section);

		cout << "Exception found! " << excepetion.what() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
}
