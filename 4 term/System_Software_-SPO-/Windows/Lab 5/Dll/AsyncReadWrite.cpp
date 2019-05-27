#include "stdafx.h"
#include "AsyncReadWrite.h"
#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
using namespace async_read_write;

AsyncReadWrite::AsyncReadWrite()
{
}

AsyncReadWrite::~AsyncReadWrite()
{
}

void AsyncReadWrite::Release()
{
	delete this;
}

vector<string> AsyncReadWrite::GetFilesFromSource(const string& source_folder)
{
	vector<string> file_names;

	const std::string path = ".\\" + source_folder;
	for (auto & p : std::experimental::filesystem::directory_iterator(path))
		file_names.emplace_back(p.path().string());
	return file_names;
}

int AsyncReadWrite::ConcatFiles(std::string source_folder_path, std::string output_file_path)
{
	this->files_to_write_ = 0;
	this->output_file_ = output_file_path;
	this->source_file_names_ = GetFilesFromSource(source_folder_path);

	int number = 0;
	int offset = 0;
	for (auto &file_name : this->source_file_names_)
	{
		this->thread_data_vector_.push_back(new ThreadData);
		this->thread_data_vector_.back()->file = CreateFileA(file_name.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
		
		this->read_files_data_.push_back(new ReadFileData);
		this->read_files_data_.back()->size = GetFileSize(thread_data_vector_.back()->file, NULL);
		if (this->read_files_data_.back()->size == 0)
		{
			CloseHandle(thread_data_vector_.back()->file);

			delete this->read_files_data_.back();
			this->read_files_data_.pop_back();

			delete thread_data_vector_.back();
			this->thread_data_vector_.pop_back();
		}
		else
		{
			this->read_events_.push_back(CreateEvent(NULL, FALSE, FALSE, NULL));

			//end the ReadFileData struct initialization
			this->read_files_data_.back()->data = new char[this->read_files_data_.back()->size];
			memset(this->read_files_data_.back()->data, 0, sizeof(char));
			this->read_files_data_.back()->offset = offset;

			//end the ThreadData struct initialization
			this->thread_data_vector_.back()->caller_instance = this;
			this->thread_data_vector_.back()->file_number = number;
			OVERLAPPED empty = { 0 };
			this->thread_data_vector_.back()->overlapped = empty;
			this->thread_data_vector_.back()->overlapped.hEvent = this->read_events_.back();

			this->reading_threads_.push_back(CreateThread(NULL, NULL, Read, reinterpret_cast<LPVOID>(this->thread_data_vector_.back()), NULL, NULL));

			offset += this->read_files_data_.back()->size;
			this->files_to_write_++;
			number++;
		}
	}
	this->writing_thread_ = CreateThread(NULL, NULL, Write, static_cast<LPVOID>(this), NULL, NULL);
	WaitForSingleObject(this->writing_thread_, INFINITE);

	this->ReleaseData();

	return this->thread_data_vector_.size();
}

void AsyncReadWrite::ReleaseData()
{
	for (auto &thread_data : this->thread_data_vector_)
	{
		CloseHandle(thread_data->file);
		delete thread_data;
	}
	this->thread_data_vector_.clear();

	this->read_files_data_.clear();

	for (auto &read_data : this->read_files_data_)
	{
		delete read_data->data;
		delete read_data;
	}
	this->read_files_data_.clear();

	for (auto &read_event : this->read_events_)
	{
		CloseHandle(read_event);
	}
	this->read_events_.clear();

	for (auto &thread : this->reading_threads_)
	{
		CloseHandle(thread);
	}
	this->reading_threads_.clear();
	CloseHandle(this->writing_thread_);
}

DWORD WINAPI AsyncReadWrite::Write(LPVOID param)
{
	AsyncReadWrite* caller_instance = reinterpret_cast<AsyncReadWrite*>(param);

	vector<HANDLE> writing_events;
	vector<OVERLAPPED*> overlappdes;

	HANDLE output_file = CreateFileA(caller_instance->output_file_.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, NULL);

	while (caller_instance->files_to_write_)
	{
		WaitForMultipleObjects(caller_instance->read_events_.size(), caller_instance->read_events_.data(), FALSE, INFINITE);
		for (auto &element : caller_instance->read_files_data_)
		{
			if (element->data[0] != '\0')
			{
				writing_events.push_back(CreateEvent(NULL, FALSE, FALSE, NULL));

				overlappdes.push_back(new OVERLAPPED);
				OVERLAPPED empty = { 0 };
				*overlappdes.back() = empty;
				overlappdes.back()->Offset = element->offset;
				overlappdes.back()->hEvent = writing_events.back();

				WriteFile(output_file, element->data, element->size, NULL, overlappdes.back());

				caller_instance->files_to_write_--;
			}
		}
	}
	WaitForMultipleObjects(writing_events.size(), writing_events.data(), TRUE, INFINITE);

	for (auto &write_event : writing_events)
	{
		CloseHandle(write_event);
	}

	for (auto &overlapped : overlappdes)
	{
		delete overlapped;
	}

	CloseHandle(output_file);

	return 0;
}

DWORD WINAPI AsyncReadWrite::Read(LPVOID param)
{
	ThreadData* thisData = reinterpret_cast<ThreadData*>(param);

	int size = thisData->caller_instance->read_files_data_[thisData->file_number]->size;
	char* buffer = thisData->caller_instance->read_files_data_[thisData->file_number]->data;

	ReadFile(thisData->file, buffer, size, NULL, &thisData->overlapped);

	return 0;
}

IAsyncReadWrite* __cdecl AsyncReadWriteFactory()
{
	return new AsyncReadWrite;
}