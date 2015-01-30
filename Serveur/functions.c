#include "functions.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


int initLiaisonSerie()
{
	int fdSerie = -1;
    struct termios termios_p;
	FILE* file;

	if ( (file = fopen("/dev/ttyAMA0", "r")) == NULL )
	{
		printf("error on open");
		exit(-1);
	}

	//setvbuf(file, NULL, _IOFBF, TAILLE_TRAME);
	fdSerie = fileno(file);

	/* Lecture des parametres courants */
	tcgetattr(fdSerie,&termios_p);
	/* On ignore les BREAK et les caracteres avec erreurs de parite */
	termios_p.c_iflag = IGNBRK | IGNPAR;
	/* Pas de mode de sortie particulier */
	termios_p.c_oflag = 0;
	/* Liaison a 9600 bps avec 7 bits de donnees et une parite paire */
	termios_p.c_cflag = B9600 | CS7 | PARENB;
	/* Mode non-canonique avec echo */
	termios_p.c_lflag = ECHO;
	/* Caracteres immediatement disponibles */
	termios_p.c_cc[VMIN] = 1;
	termios_p.c_cc[VTIME] = 0;
	/* Sauvegarde des nouveaux parametres */
	tcsetattr(fdSerie,TCSANOW,&termios_p);

	return fdSerie;
}


int initLiaisonCan()
{
	int fdCan = -1;
    struct ifreq ifr;
	char *ifname = "can0";
	struct sockaddr_can addr;
	//int n = TAILLE_TRAME_CAN;
	//int flag = 0;

	if((fdCan = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
	{
		perror("Error while opening socket");
		return -1;
	}
/*
	if ( setsockopt(fdCan, SOL_SOCKET, SO_RCVBUF, &n, sizeof(n)) == -1 )
	{
		return -1;
	}
	//setsockopt(fdCan, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
*/
	printf("socket canbus cree avec sucees\n");

	strcpy(ifr.ifr_name, ifname);
	ioctl(fdCan, SIOCGIFINDEX, &ifr);

	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	printf("%s at index %d\n", ifname, ifr.ifr_ifindex);

	if(bind(fdCan, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("Error in socket bind");
		return -2;
	}
	printf("socket attache avec succes\n");
	//shutdown(fdCan, SHUT_RDWR);

	return fdCan;
}


ssize_t lectureTrame(int liaisonSerie, char *buffer, size_t tailleBuffer)
{
    ssize_t nbLus;
	int totalLus = 0;
    char data;
    int lecture = 0;
	
	// On vide le buffer de lecture pour effacer les anciennes trames lues
	tcflush(liaisonSerie, TCIFLUSH);

    while( totalLus < tailleBuffer )
    {
        printf("je rentre dans la liaison\n");
        nbLus = read(liaisonSerie, &data, sizeof data);
        if( nbLus <= 0)
        {
            printf("erreur : nbLus = %d \n", nbLus);
            return nbLus;     /*erreur*/
        }

        printf("%c\n", data);

        if(lecture)
        {
            printf("ajout d'une data au buffer : %c\n", data);
            buffer[totalLus] = data;
			totalLus++;
        }

        if( data == '+' )
        {
            printf("caractere de debut de trame detecte\n");
            if(lecture == 0)
            {
                lecture = 1;
                printf("mode lecture active\n");
            }
           // if(lecture == 1) return;
        }
    }

    return totalLus;
}


ssize_t lectureTrameCan(int fdCan, char *buffer, size_t tailleBuffer)
{
	int i;
	int nbytes;
	int totalLus = 0;
	struct can_frame frame;

	// TODO : stop loop after 1000 negative frames ?
	while(1)
	{
		// La fonction read remplit frame avec le resultat de la lecture sur le port CAN
		nbytes = read( fdCan, &frame, sizeof(struct can_frame));

		if(nbytes == 0)
		{
			// error
		}

		printf("Identifiant: %x [%d] ", frame.can_id, frame.can_dlc);
		for(i=0; i<frame.can_dlc; i++)
		{
			printf("%x ",frame.data[i]);
		}
		printf("\n");

		if(frame.can_id == 0x11)
		{
			printf("je detecte bien le message de l identifiant 11\n");
			printf("Identifiant: %x [%d]\n", frame.can_id, frame.can_dlc);
			for(i=0; i<frame.can_dlc; i++)
			{
				//printf("%d\n",frame.data[i]);
				buffer[i] = frame.data[i];
				printf("%d\n", buffer[i]);
				totalLus++;
			}
			
			return totalLus;
		}
	}

	return 0; // never reached
}


int saveTrame(FILE* fptr, char *buffer, int sizeofbuffer)
{
	static int static_ligne = 0; // L'entier static_ligne est init a 0 et n'est jamais detruit. On peut le modifier.
    int i;

	// TODO try this
    //snprintf("%s\n", sizeofbuffer, buffer);

	fprintf(fptr, "%d;", static_ligne);
	for(i=0 ; i<TAILLE_TRAME ; i++)
	{
		if(buffer[i] == ' ') fprintf(fptr, ";");
		else fprintf(fptr, "%c", buffer[i]);
	}
	fprintf(fptr, "\n");

	static_ligne++;

	return 1;
}


int saveTrameCan(FILE* fptr, char *bufferCan, int sizeofbuffercan)
{
	static int static_ligne = 0; // L'entier static_ligne est init a 0 et n'est jamais detruit. On peut le modifier.
	int i = 0;

	printf("Je suis dans la fonction save tramecan\n");
	printf("%d, %d, %d, %d, %d, %d, %d, %d\n", *bufferCan, *(bufferCan+1), *(bufferCan+2), *(bufferCan+3), *(bufferCan+4), *(bufferCan+5), *(bufferCan+6), *(bufferCan+7));

	/*
	fprintf( fptr, "%d;%d;%d;%d;%d;%d;%d;%d;%d\n", static_ligne,
		*bufferCan, *(bufferCan+1), *(bufferCan+2), *(bufferCan+3), *(bufferCan+4), *(bufferCan+5), *(bufferCan+6), *(bufferCan+7)
	);
	*/

	fprintf(fptr, "%d;", static_ligne);
	for(i=0 ; i<TAILLE_TRAME_CAN-1 ; i++) // Pour toutes les valeurs de la trame sauf la derniere
	{
		fprintf(fptr, "%d;", bufferCan[i]); // On écrit la valeur puis un point-virgule dans le fichier 
	}
	fprintf(fptr, "%d", bufferCan[i]); // Pour la dernière valeur, on écrit la valeur sans point-virgule
	fprintf(fptr, "\n"); // et on revient à la ligne

	static_ligne++;

	return 1;
}


int numberOfEncodingDigits(int number)
{
	if( number < 0 ) return 0;
	if( number < 10 ) return 1;
	if( number < 100) return 2;
	if( number < 1000) return 3;
	else return 0;
}


void convertIntToChar(int value, char* result, int resultSize)
{
	char* buffer;
	int digits = 0;
	int i, j;

	printf("conversion du nombre %d : ", value);

	digits = numberOfEncodingDigits(value);
	if( digits == 0 )
	{
		// error
		exit(0);
	}
	buffer = (char*)malloc(digits+1); // +1 for \n
	sprintf(buffer, "%d", value);

	for(i = 0, j = 0 ; i < resultSize ; i++)
	{
		if( i < resultSize-digits ) result[i] = '0';
		else
		{
			result[i] = buffer[j];
			j++;
		}
		printf("%c", result[i]);
	}

	printf("\n");

	free(buffer);
}