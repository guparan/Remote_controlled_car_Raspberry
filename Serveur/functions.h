#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <wiringPi.h>
//#include <softPwm.h>

#define PORT 8080
#define CLIENT_MAX 1
#define PIN7 11
#define PIN8 10
#define PIN23 4
#define PIN24 5

typedef enum {GAUCHE, DROITE} Direction;
typedef enum {UP, DOWN} SpeedChange;


void avancer(int speed);
void reculer(int speed);
void tourner(Direction d, int speed);
int speedChange(SpeedChange s, int speed); 
void arretUrgence(); // si la file de commande est rempli on arrete le systeme
void initGPIO();

#endif
