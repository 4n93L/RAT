#include "../inc/other.h"
#include "../inc/common.h"

INT changeDirectory(wstring path)
{
    if (SetCurrentDirectoryW(path.c_str()) == 0) // si SetCurrentDirectoryW renvoie 0 alors la fonction _wchdir est appelée
    {
        if (_wchdir(path.c_str()) == -1) // si _wchdir renvoie -1, il y a une erreur
        {
            // à changer : ici l'erreur devrait être gérée
            return 1;
        }
        else // sinon il n'y a pas d'erreur
        {
            return 0;
        }
    }
    else // sinon il n'y a pas d'erreur
    {
        return 0;
    }
}

wstring getPath()
{
    WCHAR path[MAX_PATH];

    if (GetCurrentDirectoryW(MAX_PATH, path) == 0) // si GetCurrentDirectoryW renvoie 0, il y a une erreur
    {
        // à changer : ici l'erreur devrait être gérée
        return (wstring)L"";
    }
    else // sinon il n'y a pas d'erreur
    {
        return (wstring)path;
    }
}

VOID sendUltraSafe(SOCKET &sock, wstring data)
{
    INT len_send = 0;
    INT len_recv = 0;
    WCHAR buffer[BUFFER_LEN] = {0};
    wstring result;
    timeval timeout;

    len_send = send(sock, (char *)data.c_str(), data.length() * sizeof(WCHAR), 0); // envoi des données

    if (len_send == SOCKET_ERROR) // si len_send est égal à SOCKET_ERROR, il y a une erreur
    {
        // à changer : ici l'erreur devrait être gérée
        ;
    }
    else
    {
        struct fd_set fds;

        FD_ZERO(&fds);
        FD_SET(sock, &fds);

        timeout.tv_sec = TIMEOUT_SOCK;
        timeout.tv_usec = 0;

        INT selectSock = select(0, &fds, 0, 0, &timeout); // attente de réponse du serveur
        if (selectSock > 0)
        {
            len_recv = recv(sock, (char *)buffer, sizeof(buffer), 0); // récupération de la réponse
            result = XOREncryption((wstring)buffer);                  // décryptage de la réponse

            if (result == L"\r\n")
            {
                // confirmation ok
            }
        }
        else if (selectSock == 0)
        {
            // timeout
        }
    }
    result.erase(); // effacement des données contenues dans result
}

wstring XOREncryption(wstring data)
{
    wstring result;
    wstring char_xor;
    const wstring key = L"" XOR_KEY; // Clé de chiffrement utilisée pour XOR

    if (data.empty()) // Si la donnée est vide, on renvoie une chaîne vide
    {
        return result;
    }

    for (INT i = 0; i < data.size(); i++) // Boucle de chiffrement XOR
    {
        char_xor = data.at(i) ^ key[i % key.size()]; // Chiffrement caractère par caractère
        result += char_xor;                          // Ajout du caractère chiffré au résultat final
    }

    return result; // Renvoi du résultat chiffré
}

string ConvertWideToUtf8(const wstring &s)
{
    string utf8 = "";                                                                      // Chaîne de caractères en UTF-8
    INT len = WideCharToMultiByte(CP_UTF8, 0, s.c_str(), s.length(), NULL, 0, NULL, NULL); // Récupération de la longueur de la chaîne UTF-8
    if (len > 0)
    {
        utf8.resize(len);                                                                  // Redimensionnement de la chaîne UTF-8
        WideCharToMultiByte(CP_UTF8, 0, s.c_str(), s.length(), &utf8[0], len, NULL, NULL); // Conversion de la chaîne wide en UTF-8
    }

    return utf8; // Renvoi de la chaîne UTF-8
}

wstring ConvertUtf8ToWide(const string &str)
{
    INT count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0); // Récupération de la longueur de la chaîne wide
    wstring wstr(count, 0);                                                          // Initialisation de la chaîne wide
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);     // Conversion de la chaîne UTF-8 en wide

    return wstr; // Renvoi de la chaîne wide
}
