
#include <sys/socket.h> // For sockets
#include <sys/types.h> // For sockets
#include <netinet/in.h> // For sockets
#include <netdb.h> // For sockets
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <stdint.h>

#include "functions.h"

pthread_mutex_t lock;

void *thread_distance (void * arg)
{
    int* socketClient=(int *)arg; // On precise la nature de la variable arg	
	int ecrits = 0;
	int distance;
	char id = 'd';
	int buffer[2];
	buffer[0] = htonl(0);

	
	printf("creation du thread1\n");
	
	for(;;)
	{
		//printf("Distance: %d \n", ultrason());
		distance = ultrason();
		
		if(*socketClient != -1)
		{
			buffer[1] = htonl(distance);
			pthread_mutex_lock(&lock);
			//ecrits = write(*socketClient, &id, sizeof(id));
			ecrits = write(*socketClient, buffer, sizeof(buffer));
			pthread_mutex_unlock(&lock);
		}
		delay(50);
	}

	return 0;
}

void *thread_speed (void * arg)
{
    int* socketClient=(int *)arg; // On precise la nature de la variable arg
	int encoderSpeed = 0;
	int ecrits = 0;
	char id = 's';
	int buffer[2];
	buffer[0] = htonl(1);
	
	printf("creation du thread2\n");
	
	for(;;)
	{
		encoderSpeed = codeurIncrementalD();
		printf("speed = %lu\n", encoderSpeed);
		
		if(*socketClient != -1)
		{
			buffer[1] = htonl(encoderSpeed);
			pthread_mutex_lock(&lock);
			//ecrits = write(*socketClient, &id, sizeof(id));
			ecrits = write(*socketClient, buffer, sizeof(buffer));
			pthread_mutex_unlock(&lock);
		}
		delay(100);
	}
	return 0;
}

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
	int distance = 0;
	int distance_32bits = 0;
    struct sockaddr_in addrServeur;
    socklen_t longueurAdresse; // Nombre d'octets de la structure sockaddr_in
    char nomDuClient[1024], portDuClient[32];
	char commande = 'X';
	char clientONOFF = 'o';
	pthread_t thread1;
	pthread_t thread2;
	
	// Initialisation des GPIO
	initGPIO();
	
	// Initialisation Mutex
	if(pthread_mutex_init(&lock, NULL) != 0)
	{
		perror("mutex");
		exit(-1);
		
	}

    pthread_create(&thread1, NULL, thread_distance, &socketClient);
	pthread_create(&thread2, NULL, thread_speed, &socketClient);
    printf("creation du thread1 et 2\n");
	
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
		
		//distance = ultrason();
    }
	
	close(socketClient);
	close(socketServeur);
	
	return 0;
}
