#include <csignal>
#include <cstring>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdio>
#include <list>

const int PROC_DELAY = 1;

using namespace std;

int main(int argc, char** argv)
{
    int reciver = 0;
    char symbol;
    pid_t pid;
    list<pid_t> pidList;

    sigset_t waitSet;
    sigemptyset(&waitSet);
    sigaddset(&waitSet, SIGUSR2);
    sigprocmask(SIG_BLOCK, &waitSet, NULL);

    cout << "Hello! This is the parent process." << endl;
    cout << "If you want to create new process, please press '+'" << endl;
    cout << "If you want to delete last process, please press '-'" << endl;
    cout << "If you want to quit, please press 'q'" << endl;

    while(true)
    {
        cin.get(symbol);
        switch(symbol)
        {
            case '+':
                pid = fork();
                switch(pid)
                {
                case -1:
                    cout << "Erorr while creating child process! (fork)" << endl << endl;
                    exit(EXIT_FAILURE);
                case 0:
                    //specify the location of your child.o file
                    execv("/home/nastervill/Documents/QtProjects/Lab_2_SPOVM/child", argv);
                    cout << "Error while loading child process (excec)!" << endl << endl;
                    exit(127);
                default:
                    pidList.push_back(pid);
                    sleep(PROC_DELAY);
                    break;
                }
                break;

            case '-':
                if(pidList.empty())
                {
                    cout << "There are no chidren to delete!" << endl;
                }
                else
                {
                    kill(pidList.back(), SIGKILL);
                    pidList.pop_back();
                }
                break;

            case 'q':
                if(!pidList.empty())
                {
                    for(auto &childPid : pidList)
                    {
                        kill(childPid, SIGKILL);
                    }
                    pidList.clear();
                }
                return 0;

            default:
                continue;
        }
        cin.ignore();

        for(auto &childPid: pidList)
        {
            kill(childPid, SIGUSR1);
            sigwait(&waitSet, &reciver);
        }
    }
}
