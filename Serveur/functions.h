#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <wiringPi.h>

#define PORT 8080
#define CLIENT_MAX 1
#define PIN7 11
#define PIN8 10
#define PIN23 4
#define PIN24 5

typedef enum {GAUCHE, DROITE} Direction;


void avancer();
void reculer();
void tourner(Direction d);
void arretUrgence(); // si la file de commande est rempli on arrete le systeme
void initGPIO();

#endif

