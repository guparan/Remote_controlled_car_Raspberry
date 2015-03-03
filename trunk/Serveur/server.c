
#include <sys/socket.h> // For sockets
#include <sys/types.h> // For sockets
#include <netinet/in.h> // For sockets
#include <netdb.h> // For sockets
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#include "functions.h"


int main()
{
    int socketServeur;
    int socketClient = -1;
	int clientConnecte = 0;
    int i;
	int ecrit = 0;
	int lu = -1;
	int speed = 500;
	int stopCount = 0;
    struct sockaddr_in addrServeur;
    socklen_t longueurAdresse; // Nombre d'octets de la structure sockaddr_in
    char nomDuClient[1024], portDuClient[32];
	char commande = 'X';
	char clientONOFF = 'o';
	
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
			socketClient = accept4(socketServeur, (struct sockaddr *)&addrServeur, &longueurAdresse, SOCK_NONBLOCK);
			
			if(socketClient == -1 )
			{
				printf("errno : %d\n", errno);
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
			//ecrits = write(socketClient, &clientONOFF, 1);
		}
		
		// nanosleep((struct timespec[]){{0, 10000000}}, NULL);
		lu = read(socketClient, &commande, 1);
		
		if(lu == -1)
		{
			if(errno == EAGAIN) // Nothing to read
			{
				/*if(etat != REPOS) //&& stopCount > 150)
				{
					etat = REPOS;
					avancer(0); // stop motors
					printf("STOP\n");
					//stopCount = 0;
				}*/
				//stopCount++;
				continue;
			}
			else
			{
				printf("Error reading from socketClient ! \n");
				printf("errno = %d \n", errno);
				break;
			}
		}
		
		if(lu == 0)
		{
			printf("Socket closed ! \n");
			speed = 500;
			avancer(0);
			commande = 's';
			close(socketClient);
			clientConnecte = 0;
			continue;
		}
		
		printf("lu = %d\n", lu);
		printf("commande = %c\n", commande);
		
		switch(commande)
		{
			case 'a' :
				//printf("AVANCE\n");
				avancer(speed);
				break;
			case 'r' :
				//printf("RECULE\n");
				reculer(speed);
				break;
			case 'd' :
				//printf("DROITE\n");
				tourner(DROITE, speed);
				break;
			case 'g' :
				//printf("GAUCHE\n");
				tourner(GAUCHE, speed);
				break;
			case '+' :
				speed = speedChange(UP, speed);
				break;
			case '-' :
				speed = speedChange(DOWN, speed);
				break;
			case 's' :
				//printf("REPOS\n");
				avancer(0);
		}
    }
	
	close(socketClient);
	close(socketServeur);
	
	return 0;
}
