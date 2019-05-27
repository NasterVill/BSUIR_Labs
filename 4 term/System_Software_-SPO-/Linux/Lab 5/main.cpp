#include <iostream>
#include <dlfcn.h>
#include <string>
#include <vector>

using namespace std;

typedef void (*PerformConcatFunction)(std::vector<std::string> source_files, std::string output_file_path);

int main()
{
    int freeResult, runTimeLinkSuccess = 0;
    void* soHandle = NULL;
    PerformConcatFunction PerformConcat = NULL;

    //Load the .so and keep the handle to it
    soHandle = dlopen("libasyncrw.so", RTLD_NOW);

    // If the handle is valid, try to get the function address.
    if (NULL != soHandle)
    {
	cout << "So far... So good..." << endl;
        //Get pointer to our function using dlsym:
        PerformConcat = (PerformConcatFunction)dlsym(soHandle,
            "PerformConcat");

        // If the function address is valid, call the function.
        if (runTimeLinkSuccess = (NULL != PerformConcat))
        {          
            vector<string> file_names;
            file_names.push_back(string("read_files/first.txt"));
            file_names.push_back(string("read_files/second.txt"));
            file_names.push_back(string("read_files/third.txt"));
            file_names.push_back(string("read_files/fourth.txt"));
            file_names.push_back(string("read_files/fifth.txt"));

            cout << "Starting processing..." << endl;
            PerformConcat(file_names, "output.txt");

            cout << "The task was executed..." << endl;
            cout << "Press any key to continue..." << endl;
            cin.ignore();
        }
	    
        //Free the library:
        cout << "Unattaching .so library..." << endl;
        freeResult = dlclose(soHandle);
    }

    //If unable to call the .so function, use an alternative.
    if (!runTimeLinkSuccess)
        cout << "Unable to call .so function" << endl;

    if(soHandle == NULL)
        cout << "so sorry : " << dlerror() << endl;

    return 0;
}
