// Fonctions
#include "functions.h"
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>


static int keepRunning = 1;


void intHandler(int sig)
{
	char  c;

	signal(sig, SIG_IGN);
	printf("Do you really want to quit? [y/n/F] ");
	c = getchar();
	if (c == 'y' || c == 'Y' || c == 'F')
	{
		keepRunning = 0;
		sleep(1);
		if(c == 'F') exit(sig);
	}
	signal(SIGINT, intHandler);
}


void *thread_runtime (void * arg)
{
    int * clients=(int *)arg; // On precise la nature de la variable arg

	int tailleTrameClient = TAILLE_INFO_TRAME + TAILLE_INFO_TRAME_CAN + TAILLE_TRAME + TAILLE_TRAME_CAN;
	
	// Declaration et allocation de la trame client
	char trameClient[TAILLE_INFO_TRAME + TAILLE_INFO_TRAME_CAN + TAILLE_TRAME + TAILLE_TRAME_CAN];
	
	// Placement des pointeurs vers les differentes parties de la trame client
	char* tailleTrameSerieLue_char = trameClient;
	char* tailleTrameCanLue_char = tailleTrameSerieLue_char + TAILLE_INFO_TRAME;
	char* buffer = tailleTrameCanLue_char + TAILLE_INFO_TRAME_CAN;
	char* bufferCan = buffer + TAILLE_TRAME;

    //char* bufferTest="Testbuffer";

    int fdSerie;
	int fdCan;

    int i, idClient;
    int ecrits=0;

	/*
	// TESTS
	int tailleTrameCanLue_int = 8;
	for(i=0 ; i<8 ; i++)
	{
		bufferCan[i] = (char)i+70;
	}
	*/
	
	int tailleTrameCanLue_int = 0;
	int tailleTrameSerieLue_int = 0;
	int lectureTramesFaite = 0;

	printf("thread cree\n");

	// Ouverture des fichiers de sauvgegarde
	FILE* logSerie = fopen("data.csv", "w");
	FILE* logCan = fopen("dataCAN.csv", "w");

	fdSerie = initLiaisonSerie();
	fdCan = initLiaisonCan();

    printf("keepRunning %d\n", keepRunning);

    while ( keepRunning )
    {
		lectureTramesFaite = 0;

        for(idClient=0 ; idClient < CLIENT_MAX ; idClient++)
        {
            //printf("check client %d\n", idClient);
            if(clients[idClient]==-1)
            {
                //printf("Pas de client a %d\n", idClient);
                continue; // on passe au client suivant
            }

			// On ne fait la lecture que si on a au moins 1 client
			// Si il ya plusieurs clients dans le tableau, la lecture des trames n'est faite qu'une fois
			if(!lectureTramesFaite) 
			{
				// LECTURE TRAME CAN
				tailleTrameCanLue_int = lectureTrameCan(fdCan, bufferCan, TAILLE_TRAME_CAN);
				if( tailleTrameCanLue_int == 0 )
				{
					// error
				}
				convertIntToChar(tailleTrameCanLue_int, tailleTrameCanLue_char, TAILLE_INFO_TRAME_CAN);
				
				// LECTURE TRAME SERIE
				tailleTrameSerieLue_int = lectureTrame(fdSerie, buffer, TAILLE_TRAME);
				if( tailleTrameSerieLue_int == 0 )
				{
					// error
				}
				convertIntToChar(tailleTrameSerieLue_int, tailleTrameSerieLue_char, TAILLE_INFO_TRAME);

				lectureTramesFaite = 1;
			}

            printf("tentative decriture sur le client %d \n", idClient);
            ecrits = write(clients[idClient], trameClient, tailleTrameClient);
            printf("code de retour du write : %d \n", ecrits);

            if(ecrits == -1)
            {
                printf("Error writing from socket ! \n");
                printf("errno = %d \n", errno);

                if(errno == EPIPE || errno == ECONNRESET)
                {
                    printf("deconnexion client\n");
                    close(clients[idClient]);
                    clients[idClient]=-1;
                }
				else
				{
					printf("Erreur inconnue \n");
					exit(errno);
				}
            }
			else // Si le write s'est bien passe
			{
				printf("Trame envoyee au client %d : ", idClient);
				for(i=0 ; i < tailleTrameClient ; i++)
				{
					if(i < TAILLE_TRAME+TAILLE_INFO_TRAME_CAN+TAILLE_INFO_TRAME) printf("%c", trameClient[i]);
					else printf("%d", trameClient[i]);
				}
				
				printf("(%d octets)\n", ecrits);

				if( saveTrameCan(logCan, bufferCan, TAILLE_TRAME_CAN) ) printf("la sauvegarde CAN a bien ete faite \n");
				if( saveTrame(logSerie, buffer, TAILLE_TRAME) ) printf("la sauvegarde SERIE a bien ete faite \n");
			}
		}
    }

    // Ce code est atteint si keepRunning == 0
	close(fdSerie); // Fermeture de la liaison Serie
    close(fdCan); // Fermeture de la liaison CAN
	
    printf("fin du thread\n");
    return 0;
}



int main()
{
    pthread_t thread;
    int socketServeur;
    int socketClient;
    int i;
    int clients[CLIENT_MAX];
    struct sockaddr_in addrServeur;
    socklen_t longueurAdresse; // Nombre d'octets de la structure sockaddr_in
    char nomDuClient[1024], portDuClient[32];

    // Gestion du signal d'interuption Ctrl+C
	//signal(SIGINT, SIG_IGN);
    signal(SIGINT, intHandler);
	
	// Gestion du signal SIGPIPE envoye par une socket Client : on l'ignore
    signal(SIGPIPE, SIG_IGN);
	
    // Initialisation du tableau des clients
    for(i=0 ; i<CLIENT_MAX ; i++)
    {
        clients[i]=-1;
    }

    // Crée un socket de communication
    //socketServeur = socket(PF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0);
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

    //memset(buffer,0x00,LG_MESSAGE*sizeof(char));
    pthread_create(&thread, NULL, thread_runtime, clients);
    printf("creation du thread\n");

	printf("Attente d'une demande de connexion (quitter avec Cltrl-C)\n\n");
		
    while(1)
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

		for(i=0 ; i<CLIENT_MAX ; i++)
		{
			if(clients[i]==-1)
			{
				clients[i]=socketClient;
				printf("Client assigne a l'indice %d\n", i);
				break;
			}
		}

		if(i == CLIENT_MAX)
		{
			printf("Table des client pleine, client rejete !\n");
			close(socketClient);
		}

		if ( getnameinfo((struct sockaddr*)&addrServeur, sizeof(addrServeur), nomDuClient, sizeof(nomDuClient), portDuClient,
					   sizeof(portDuClient), NI_NUMERICHOST | NI_NUMERICSERV) == 0)
		{
			   printf("client=%s, port=%s\n", nomDuClient, portDuClient);
		}
    }
	
	printf("CLEANING ...\n");

    // Attente de la fin du thread
	if(pthread_join(thread, NULL) != 0)
	{
		perror("pthread_join");
		exit(errno);
	}
	
	// Deconnexion des clients
    for(i=0 ; i<CLIENT_MAX ; i++)
    {
        if(clients[i] != -1)
        {
            close(clients[i]);
            clients[i] = -1;
        }
    }
	
	// Fermeture de la socket serveur
    close(socketServeur);
	
    return 0;
}
