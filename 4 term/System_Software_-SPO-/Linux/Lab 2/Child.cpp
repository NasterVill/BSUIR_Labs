#include <iostream>
#include <cstdio>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

const int DELAY = 1;

using namespace std;

int main()
{
	int reciver = 0;

	string message = "Hello, i'm child! My pid is ";
	string pidStr = patch::to_string((int)getpid());
	message += pidStr;
	
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGKILL);
	sigprocmask(SIG_BLOCK, &set, NULL);

	while (true)
	{
		sigwait(&set, &reciver);
		
		cout << message.c_str() << endl;;

		kill(getppid(), SIGUSR2);
	}
	return 0;
}
