
#include <sys/socket.h> // For sockets
#include <sys/types.h> // For sockets
#include <netinet/in.h> // For sockets
#include <netdb.h> // For sockets
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "functions.h"


int main()
{
    int socketServeur;
    int socketClient;
	int clientConnecte = 0;
    int i;
	int lu = -14; // remettre a -1
    struct sockaddr_in addrServeur;
    socklen_t longueurAdresse; // Nombre d'octets de la structure sockaddr_in
    char nomDuClient[1024], portDuClient[32];
	char commande = 'd';
	char buffer[10];
	
	// Initialisation des GPIO
	initGPIO();

    // Cree un socket de communication
	socketServeur = socket(PF_INET, SOCK_STREAM, 0);
    if(socketServeur == -1)
    {
        perror("Socket");
        exit(-1);
    }

    printf("Socket crée avec succès ! (%d)\n", socketServeur);

    addrServeur.sin_addr.s_addr = INADDR_ANY; //toutes
    addrServeur.sin_family = PF_INET;
    addrServeur.sin_port = htons(PORT);
	
	// lose the pesky "Address already in use" error message
	int yes = 1;
	if( setsockopt(socketServeur, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1 ) 
	{
		perror("setsockopt");
		exit(errno);
	}

    // Demande l'attachement local de la socket
    longueurAdresse = sizeof(addrServeur);
    if( bind(socketServeur, (struct sockaddr *)&addrServeur, longueurAdresse) == -1 )
    {
        perror("bind");
        exit(-2);
    }
    printf("Socket attachée avec succès!\n");

    if (listen(socketServeur, CLIENT_MAX) == -1)
    {
        perror("listen");
        exit(errno);
    }
    printf("Socket placée en écoute passive...\n");

	printf("Attente d'une demande de connexion (quitter avec Cltrl-C)\n\n");
		
    while(1)
    {       
		if(clientConnecte == 0)
		{
			socketClient = accept(socketServeur, (struct sockaddr *)&addrServeur, &longueurAdresse);
			
			if(socketClient == -1 )
			{
				perror("accept");
				close(socketClient);
				close(socketServeur);
				exit(errno);		
			}
			
			printf("Nouveau client !\n");

			if ( getnameinfo((struct sockaddr*)&addrServeur, sizeof(addrServeur), nomDuClient, sizeof(nomDuClient), portDuClient,
						   sizeof(portDuClient), NI_NUMERICHOST | NI_NUMERICSERV) == 0)
			{
				   printf("client=%s, port=%s\n", nomDuClient, portDuClient);
			}
			
			clientConnecte = 1;
		}
		
		//lu = read(socketClient,  buffer, 9);
		
		//printf("lu = %d\n", lu);
		
		buffer[9] = '\0';
		printf("commande = %c\n", commande);
		
		if(lu == -1)
		{
			printf("Error reading from socketClient ! \n");
            printf("errno = %d \n", errno);
			break;
		}
		
		if(lu == 0)
		{
			printf("Socket closed ! \n");
			close(socketClient);
			clientConnecte = 0;
			continue;
		}
		
		switch(commande)
		{
			case 'a' :
				avancer();
				break;
			case 'r' :
				reculer();
				break;
			case 'd' :
				tourner(DROITE);
				break;
			case 'g' :
				tourner(GAUCHE);
				break;
		}	   
    }
	
	close(socketClient);
	close(socketServeur);
	
	return 0;
}
