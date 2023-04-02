#ifndef OTHER_H
#define OTHER_H

#include "../inc/master.h"

using namespace std;

VOID sendUltraSafe(SOCKET &sock, wstring data); //Function for HandShake or reconnect!!! Uses XOR encryption.

string generateToken(INT length); //Function to generate a token of a certain length.
wstring XOREncryption(wstring data); //Function for XOR encryption. Allows you to know whether the function encrypts or decrypts. encrypt (TRUE) = encrypt function.
wstring getPath(); //Function to get the current path.

INT changeDirectory(wstring path); //Function to change the directory.
INT totalDestruction(); //Function for total destruction.

string ConvertWideToUtf8(const wstring &s); //Function to convert a wide string to a UTF-8 string.
wstring ConvertUtf8ToWide(const string &str); //Function to convert a UTF-8 string to a wide string. https://stackoverflow.com/questions/6693010/how-do-i-use-multibytetowidechar

#endif
