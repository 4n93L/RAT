#ifndef EXEC_H
#define EXEC_H

#include "../inc/master.h"

using namespace std;

class Exec
{
    /*Executes a command and returns the result. The only drawback of this class is that if the process executing is waiting for user input (stdin), then the program becomes a zombie.
    The only solution I found is to put a timeout in CreateProcess (in createChildProcess) then list all processes with the same name and kill them all.
    If you have ideas to make this code less brutal, don't hesitate to contact me.
    */

    public:
        Exec(); //Default constructor.

        vector<DWORD> returnPid(wstring stringTargetProcessName); //Function to list all PIDs and return PIDs via process name.

        VOID setupAllPipe(); //Function to set up all pipes.
        PROCESS_INFORMATION createChildProcess(wstring &command); //Function to create a child process and execute a command.

        vector<string> readFromPipe(PROCESS_INFORMATION piProcInfo); //Function to read from pipe and return the result as a string.

        vector<string> executeCommand(wstring command); //Function to execute a command and return the result.

        VOID spawnSHELL(SOCKET &sock,WCHAR *prog); //Function to spawn a shell.

        ~Exec(); //Destructor.

    private:
        HANDLE a_hChildStd_OUT_Rd = NULL; //The read handle for stdout.
        HANDLE a_hChildStd_OUT_Wr = NULL; //The write handle for stdout.

        HANDLE a_hChildStd_ERR_Rd = NULL; //The read handle for stderr.
        HANDLE a_hChildStd_ERR_Wr = NULL; //The write handle for stderr.
        SECURITY_ATTRIBUTES security; //The security attributes for the pipes.

        BOOL a_error = FALSE; //Whether there is an error or not.
        BOOL a_timeout = FALSE; //Whether there is a timeout in CreateProcess.
};

/*
Inspired by:
- https://docs.microsoft.com/en-us/windows/win32/procthread/creating-a-child-process-with-redirected-input-and-output
- https://stackoverflow.com/questions/14147138/capture-output-of-spawned-process-to-string
*/

#endif
