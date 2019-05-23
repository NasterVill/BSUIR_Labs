#include "Thread.h"
#include "CSGuard.h"
#include "InvalidSectionException.h"
#include "InvalidStartRoutineException.h"
#include "InvalidStackSizeException.h"
#include "ThreadCreationException.h"

using namespace CustomThread;

const std::string Thread::kSynchronizeEveventName = "setUpEndEvent";
const int Thread::kSwitchDelay = 200;
const int Thread::kPrintDelay = 1000;

int Thread::stack_size_ = 0;
int Thread::last_thread_ = 0;
LPCRITICAL_SECTION Thread::critical_section_ = nullptr;
PTHREAD_START_ROUTINE Thread::start_routine_ = nullptr;

void Thread::SetCriticalSection(LPCRITICAL_SECTION critical_section)
{
	if (critical_section == nullptr)
		throw InvalidSectionException("The value of param criticalSection is invalid!");
	else
		critical_section_ = critical_section;
}

void Thread::SetStartUpRoutine(PTHREAD_START_ROUTINE start_routine)
{
	if (start_routine == nullptr)
		throw InvalidStartRoutineException("The value of param startRoutine is invalid!");
	else
		start_routine_ = start_routine;
}

void Thread::SetStackSize(size_t stack_size)
{
	if (stack_size <= 0)
		throw InvalidStackSizeException("The value of param stackSize is invalid!");
	else
		stack_size_ = stack_size;
}

void Thread::Invoke()
{
	if (stack_size_ == 0)
		throw InvalidStackSizeException("The value of param stackSize is invalid!");
	if (start_routine_ == nullptr)
		throw InvalidStartRoutineException("The value of param startRoutine is invalid!");
	if (critical_section_ == nullptr)
		throw InvalidSectionException("The value of param stackSize is invalid!");

	if (*this->data_->run_flag != 0)
		return;
	else
		*this->data_->run_flag = 1;

	HANDLE synchronizeEvent = CreateEvent(NULL, FALSE, false, kSynchronizeEveventName.c_str());

	this->thread_handle_ = CreateThread(NULL, this->stack_size_, start_routine_, data_, NULL, &(this->data_->thread_id));
	if (this->thread_handle_ != NULL)
	{
		WaitForSingleObject(synchronizeEvent, INFINITE);
		CloseHandle(synchronizeEvent);
	}
	else
		throw ThreadCreationException("CreateEvent returned NULL");
}

void Thread::Terminate()
{
	if (*(this->data_->run_flag) != 0)
	{
		*this->data_->run_flag = 0;
		WaitForSingleObject(this->thread_handle_, INFINITE);
		CloseHandle(this->thread_handle_);
		this->data_->thread_id = 0;
	}
}

void Thread::DeleteData()
{
	if (this->data_ != nullptr)
	{
		delete this->data_->run_flag;
		delete this->data_;

		this->data_ = nullptr;
	}
}

void Thread::InitializeData()
{
	if (this->data_ != nullptr)
	{
		this->DeleteData();
	}

	this->data_ = new ThreadData;
	this->data_->critical_section = critical_section_;
	this->data_->event_name = kSynchronizeEveventName.c_str();
	this->data_->run_flag = new int(0);
	this->data_->last_thread = &last_thread_;
	this->data_->thread_id = 0;
}

Thread::Thread()
{
	this->InitializeData();
}

Thread::~Thread()
{
	this->Terminate();
	this->DeleteData();
}

DWORD WINAPI CustomThread::ThreadRoutine(PVOID pvParam)
{
	Thread::ThreadData* threadData = reinterpret_cast<Thread::ThreadData*>(pvParam);

	//saying to a main thread, that this subthread has started working
	HANDLE eventHandle = OpenEvent(EVENT_ALL_ACCESS, false, threadData->event_name);

	CSGuard guard(threadData->critical_section);

	SetEvent(eventHandle);
	CloseHandle(eventHandle);

	while (*(threadData->run_flag) != 0)
	{
		if (*(threadData->last_thread) == threadData->thread_id)
		{
			Sleep(Thread::kSwitchDelay);
		}
		guard.enterSection();

		std::cout << "Hello! I'm subthread " << threadData->thread_id << std::endl;
		Sleep(Thread::kPrintDelay);

		guard.leaveSection();
		*(threadData->last_thread) = threadData->thread_id;
	}
	return 0;
}
