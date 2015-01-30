#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#define CLIENT_MAX 1
typedef enum {GAUCHE, DROITE} Orientation;


void avancer();
void reculer();
void tourner(Orientation direction);
void arretUrgence(); // si la file de commande est rempli on arrete le systeme


#endif

