#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <string.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <limits>

namespace constants
{
    const int kKeyIdSem = 40;
    const int kKeyIdShMem = 41;
    const int kSize = 10;
    const int kSemaphoreAmount = 4;
    const int kServerSemaphoreIndex = 0;
    const int kClientSemaphoreIndex = 1;
    const int kKillSemaphoreIndex = 2;
    const int kClientErrorSemaphoreIndex = 3;
    const short kSetArray[kSemaphoreAmount] = { 0 };
    const char kInitialPath[] = "/dev/null";
}

int createSemaphoreSet(key_t key)
{
    int id;
    int check = 0;

    id = semget(key, constants::kSemaphoreAmount, IPC_CREAT | SHM_R | SHM_W);
    if (id != -1)
    {
        check = semctl(id, 0, SETALL,
                       const_cast<short*>(constants::kSetArray)); // SETALL ignores
                                                                  // semnum (second) argument
    }

    return (check == -1) ? check : id;
}

void deleteSemaphoreSet(int semid)
{
    semctl(semid, 0, IPC_RMID, NULL);
}

void* mapSharedMemory(int shmId)
{
    void* memoryAddress;

    memoryAddress = shmat(shmId, NULL, 0);

    return memoryAddress;
}

using namespace std;

int main()
{
    pid_t pid;
    int clientStatus;
    int semaphoreId, shMemoryId;
    key_t semaphoreKey, shMemoryKey;
    void *shMemoryAddress;
    struct sembuf semaphoreSet;
    struct shmid_ds shMemoryStruct;

    semaphoreKey = ftok(constants::kInitialPath, constants::kKeyIdSem);
    if (semaphoreKey == (key_t)-1)
    {
        cerr << "Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    shMemoryKey = ftok(constants::kInitialPath, constants::kKeyIdShMem);
    if (shMemoryKey == (key_t)-1)
    {
        cerr << "Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    semaphoreId = createSemaphoreSet(semaphoreKey);
    if (semaphoreId == -1)
    {
        cerr << "Error: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    shMemoryId = shmget(shMemoryKey, constants::kSize, IPC_CREAT | SHM_R | SHM_W);
    if (shMemoryId == -1)
    {
        cerr << "Error: " << strerror(errno) << endl;

        deleteSemaphoreSet(semaphoreId);

        exit(EXIT_FAILURE);
    }

    shMemoryAddress = (char* )mapSharedMemory(shMemoryId);
    if (shMemoryAddress == NULL)
    {
        cerr << "Error: " << strerror(errno) << endl;

        deleteSemaphoreSet(semaphoreId);
        shmctl(shMemoryId, IPC_RMID, &shMemoryStruct);

        exit(EXIT_FAILURE);
    }

    pid = fork();
    switch(pid)
    {
    case -1:
    {
        cerr << "Error: " << strerror(errno) << endl;

        deleteSemaphoreSet(semaphoreId);
        shmdt(shMemoryAddress);
        shmctl(shMemoryId, IPC_RMID, &shMemoryStruct);

        exit(EXIT_FAILURE);
    }
    case 0:
    {
        while(true)
        {
            // Wait for server to get the input
            semaphoreSet.sem_num = constants::kServerSemaphoreIndex;
            semaphoreSet.sem_op  = -1;
            semaphoreSet.sem_flg = SEM_UNDO;
            semop(semaphoreId, &semaphoreSet, 1);

            // If there was a kill-command
            if(semctl(semaphoreId, constants::kKillSemaphoreIndex, GETVAL) == 1)
            {
                shmdt(shMemoryAddress);
                break;
            }

            shMemoryAddress = (char* )mapSharedMemory(shMemoryId);
            if (shMemoryAddress == NULL)
            {
                cerr << "Error: " << strerror(errno) << endl;

                // Set error semaphore
                semaphoreSet.sem_num = constants::kClientErrorSemaphoreIndex;
                semaphoreSet.sem_op  = 1;
                semaphoreSet.sem_flg = SEM_UNDO;
                semop(semaphoreId, &semaphoreSet, 1);

                //Say, that client has stopped working
                semaphoreSet.sem_num = constants::kClientSemaphoreIndex;
                semaphoreSet.sem_op  = 1;
                semaphoreSet.sem_flg = SEM_UNDO;
                semop(semaphoreId, &semaphoreSet, 1);

                exit(EXIT_FAILURE);
            }

            cout << "Client got: " << (char*)shMemoryAddress << endl;

            // Say, that client has read the memory
            semaphoreSet.sem_num = constants::kClientSemaphoreIndex;
            semaphoreSet.sem_op  = 1;
            semaphoreSet.sem_flg = SEM_UNDO;
            semop(semaphoreId, &semaphoreSet, 1);
        }
        exit(EXIT_SUCCESS);
    }
    default:
    {
        int currPos = 0;
        bool readyForInput = true;
        string buffStr;
        buffStr.resize(constants::kSize, '\0');

        while(true)
        {
            memset(shMemoryAddress, '\0', 1);

            if(readyForInput)
            {
                currPos = 0;
                cout << "Server: Please, enter the string" << endl;
                getline(cin, buffStr);
                readyForInput = false;
            }

            string tempBuff;
            int newLength = 0;
            tempBuff.append(buffStr, 0, constants::kSize - 1);
            currPos = tempBuff.length();
            strcpy((char*)shMemoryAddress, const_cast<char*>(tempBuff.c_str()));

            tempBuff.clear();
            newLength = buffStr.length() - currPos;
            if (newLength > 0)
            {
                tempBuff.append(buffStr, currPos, newLength);
            }
            buffStr = tempBuff;

            // Say, that server has accepted the input
            semaphoreSet.sem_num = constants::kServerSemaphoreIndex;
            semaphoreSet.sem_op  = 1;
            semaphoreSet.sem_flg = SEM_UNDO;
            semop(semaphoreId, &semaphoreSet, 1);

            // Wait for client to read
            semaphoreSet.sem_num = constants::kClientSemaphoreIndex;
            semaphoreSet.sem_op  = -1;
            semaphoreSet.sem_flg = SEM_UNDO;
            semop(semaphoreId, &semaphoreSet, 1);

            // Check, whether the client had an error
            if(semctl(semaphoreId, constants::kClientErrorSemaphoreIndex, GETVAL) > 0)
            {
                break;
            }

            if(buffStr.empty())
            {
                readyForInput = true;
                cout << "Press '0' to exit the programm, or press any other key to continue..." << endl;

                if(cin.get() == '0')
                {
                    semaphoreSet.sem_num = constants::kKillSemaphoreIndex;
                    semaphoreSet.sem_op  = 1;
                    semaphoreSet.sem_flg = SEM_UNDO;
                    semop(semaphoreId, &semaphoreSet, 1);

                    semaphoreSet.sem_num = constants::kServerSemaphoreIndex;
                    semaphoreSet.sem_op  = 1;
                    semaphoreSet.sem_flg = SEM_UNDO;
                    semop(semaphoreId, &semaphoreSet, 1);

                    waitpid(pid, &clientStatus, 0);
                    if (WIFEXITED(clientStatus))
                    {
                        cout << "Client has exited with status value: " << WEXITSTATUS(clientStatus) << endl;
                    }
                    break;
            }
            buffStr.clear();

            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << endl;
            }
        }
    }
    }

    deleteSemaphoreSet(semaphoreId);
    shmdt(shMemoryAddress);
    shmctl(shMemoryId, IPC_RMID, &shMemoryStruct);

    return 0;
}
