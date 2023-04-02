#ifndef DESTRUCTION_H
#define DESTRUCTION_H

#include "../inc/master.h"
#include "../inc/common.h"

using namespace std;

class Destruction
{
    public:
        Destruction(); //Default constructor. Launches a process (.bat file) to delete the program and then exits the program.
        Destruction(wstring path_prog); //Constructor. Takes a path to the program.

        VOID createBatchFile(); //Function to create the batch file.
        VOID kills_all_same_process(); //Function to delete all processes that have the same name.
        VOID startBatchFile(); //Function to start the batch file.

        INT main(); //Main function of the class.

        INT testIfError(); //Function to test for errors.
        VOID delete_batch_file(); //In case of error(s), check if the batch file exists. If the file exists, then delete it.
        wstring find_name_process(); //Function to find the name of the process.

        ~Destruction(); //Destructor.

    private:
        wstring a_path_prog; //The path to the program.
        wstring a_name_file_batch = L"setup.bat"; //The name of the batch file.
        BOOL a_error = FALSE; //Whether there is an error or not.
};

#endif
