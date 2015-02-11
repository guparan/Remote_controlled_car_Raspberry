#include "functions.h"

#include <stdio.h>

void initGPIO()
{
	printf("Initialisation des GPIO\n");

	if(wiringPiSetup() == -1) return;
	
	//pinMode(PIN23, OUTPUT);
	//pinMode(PIN24, OUTPUT);
	//pinMode(PIN8, OUTPUT);
	//pinMode(PIN7, OUTPUT);
	softPwmCreate(PIN23,0,1000);
	softPwmCreate(PIN24,0,1000);
	softPwmCreate(PIN8,0,1000);
	softPwmCreate(PIN7,0,1000);
}

void avancer()
{
    //digitalWrite (PIN24, 1);     // On
    //digitalWrite (PIN8, 0);      // Off
    //digitalWrite (PIN7, 1);      // On
    //digitalWrite (PIN23, 0);     // Off
   softPwmWrite(PIN24,500);
   softPwmWrite(PIN8,0);
   softPwmWrite(PIN7,500);
   softPwmWrite(PIN23,0);  
}

void reculer()
{
	digitalWrite (PIN24, 0);     // On
    digitalWrite (PIN8, 1);      // Off
    digitalWrite (PIN7, 0);      // On
    digitalWrite (PIN23, 1);     // Off
}

void tourner(Direction d)
{
	if(d == GAUCHE)
	{
		digitalWrite (PIN24, 0);     // Off
		digitalWrite (PIN8, 0);      // Off
		digitalWrite (PIN7, 1);      // On
		digitalWrite (PIN23, 1);     // On
		printf("Je tourne a gauche \n");

	}
	else 
	{
		digitalWrite (PIN24, 1);     // On
		digitalWrite (PIN8, 1);      // On
		digitalWrite (PIN7, 0);      // Off
		digitalWrite (PIN23, 0);     // Off
		printf("Je tourne a droite \n");
	}
}
