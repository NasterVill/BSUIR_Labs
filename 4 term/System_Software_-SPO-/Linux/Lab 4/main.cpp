#include <csignal>
#include <sys/wait.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <list>

using namespace std;

unsigned int last_thread = 0;

void * threadStartRoutine(void* arg)
{
    pthread_mutex_t mutex = *((pthread_mutex_t*)(arg));

    //saying to a main thread, that this subthread has started working
    kill(getpid(), SIGUSR1);

    while(true)
    {
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
        if(last_thread == (unsigned int)pthread_self())
            sleep(1);
        pthread_mutex_lock(&mutex);

        cout << "Hello! I'm subthread " << (unsigned int)pthread_self() << endl;

        last_thread = (unsigned int)pthread_self();

        pthread_mutex_unlock(&mutex);
        pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        pthread_testcancel();
    }
}

int main(int argc, char** argv)
{
    char symbol;
    void** threadReciever;
    int sigReciever;
    list<pthread_t> idList;
    pthread_t id;
    pthread_mutex_t mutex;

    sigset_t waitSet;
    sigemptyset(&waitSet);
    sigaddset(&waitSet, SIGCONT);
    sigprocmask(SIG_BLOCK, &waitSet, NULL);

    void *(*start_routine) (void *) = threadStartRoutine;
    int mutexError = pthread_mutex_init(&mutex, NULL);
    if(mutexError != 0)
    {
        cout << "Error while creating mutex (code): " << mutexError << endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    cout << "Hello! This is the parent process." << endl;
    cout << "If you want to create new process, press '+'" << endl;
    cout << "If you want to delete last process, press '-'" << endl;
    cout << "If you want to quit, press 'q'" << endl;
    cout << "Press any key to begin..." << endl;

    cin.ignore();

    while(true)
    {
        cin.get(symbol);

        switch(symbol)
        {
        case '+':
        {
                int creationResult = pthread_create(&id, NULL, start_routine, &mutex);
                if(creationResult == 0)
                {
                    idList.push_back(id);
                    sigwait(&waitSet, &sigReciever);
                }
                else
                {
                    cout << "Error while creating new thread (code): " << creationResult << endl;
                    if(!idList.empty())
                    {
                        for(auto &threadId : idList)
                        {
                            pthread_cancel(threadId);
                            pthread_join(threadId, threadReciever);
                        }
                        idList.clear();
                    }
                    pthread_mutex_destroy(&mutex);

                    system("pause");
                    exit(EXIT_FAILURE);
                }
                break;
        }
        case '-':
        {
                if(idList.empty())
                {
                    cout << "There are no subthreads to delete!" << endl;
                }
                else
                {
                    pthread_cancel(idList.back());
                    pthread_join(idList.back(), threadReciever);
                    idList.pop_back();
                }
                break;
        }
        case 'q':
        {
                if(!idList.empty())
                {
                    for(auto &threadId : idList)
                    {
                        pthread_cancel(threadId);
                        pthread_join(threadId, threadReciever);
                    }
                    idList.clear();
                }
                pthread_mutex_destroy(&mutex);

                return 0;
        }
        default:
            continue;
        }
        cin.ignore();
    }
}
