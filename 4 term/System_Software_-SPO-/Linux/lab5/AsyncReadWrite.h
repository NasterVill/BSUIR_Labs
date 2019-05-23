#ifndef ASYNC_READ_WRITE_H
#define ASYNC_READ_WRITE_H

#include <aio.h>
#include <string>
#include <vector>
#include <utility>
#include <sys/types.h>
#include <sys/sem.h>

namespace async_read_write
{
	
    class AsyncReadWrite
    {
    private:
        struct ReadFileData
        {
            int offset;
            int size;
            char* data;
            int* ready;
        };

    public:
        int ConcatFiles(std::vector<std::string> source_files, std::string output_file_path);
	    
    private:
        static void* Read(void* arg);
        static void* Write(void* arg);
        static void ReadEndRoutine(sigval_t arg);
        static void WriteEndRoutine(sigval_t arg);

    private:
        void ReleaseData();
        int CreateSemaphoreSet(key_t key);
        void DeleteSemaphoreSet(int semid);

    private:
        AsyncReadWrite(const AsyncReadWrite&) = delete;
        AsyncReadWrite& operator=(const AsyncReadWrite&) = delete;

    public:
        AsyncReadWrite();
        ~AsyncReadWrite();

    private:
        std::vector<std::string> source_file_names_;
        std::string output_file_;

        std::vector<aiocb*> thread_data_vector_;
        std::vector<ReadFileData*> read_files_data_;
	int files_to_write_;

        std::vector<pthread_t> reading_threads_;
        pthread_t writing_thread_;

    private:
        int semaphoreId;
        key_t semaphoreKey;

    private:
        static const int kWriteSemaphoreIndex;
        static const int kReadSemaphoreIndex;
        static short kSetArray[];
    };

}

#endif
