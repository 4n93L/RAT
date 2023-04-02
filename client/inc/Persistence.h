#ifndef PERSI_H
#define PERSI_H

#include "../inc/master.h"

using namespace std;

class Persistence
{
    /*
    Changes the registry key according to the user's permission.
    */
    public:
        Persistence(); //Default constructor.
        Persistence(bool is_admin, wstring path_prog); //Constructor with parameters.
        INT customPersi(); //Function to customize persistence (coming soon).

        VOID main(); //Main function.

    private:
        BOOL a_is_admin; //Whether the user is an admin or not.
        wstring a_path_prog; //The path of the program.
};
#endif
