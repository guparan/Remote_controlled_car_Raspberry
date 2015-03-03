#include "functions.h"

#include <stdio.h>


void initGPIO()
{
	printf("Initialisation des GPIO\n");

	if(wiringPiSetup() == -1) return;
	
	//pinMode(PIN23, OUTPUT);
	//pinMode(PIN24, OUTPUT);
	pinMode(PIN17, OUTPUT);
	pinMode(PIN27, INPUT);
	softPwmCreate(PIN23,0,1000);
	softPwmCreate(PIN24,0,1000);
	softPwmCreate(PIN8,0,1000);
	softPwmCreate(PIN7,0,1000);
}

void avancer(int speed)
{
    //digitalWrite (PIN24, 1);     // On
    //digitalWrite (PIN8, 0);      // Off
    //digitalWrite (PIN7, 1);      // On
    //digitalWrite (PIN23, 0);     // Off
   softPwmWrite(PIN24,speed);
   softPwmWrite(PIN8,0);
   softPwmWrite(PIN7,speed);
   softPwmWrite(PIN23,0);  
}

void reculer(int speed)
{
	//digitalWrite (PIN24, 0);     // On
    //digitalWrite (PIN8, 1);      // Off
    //digitalWrite (PIN7, 0);      // On
    //digitalWrite (PIN23, 1);     // Off
    softPwmWrite(PIN24,0);
    softPwmWrite(PIN8,speed);
    softPwmWrite(PIN7,0);
    softPwmWrite(PIN23,speed); 
}

void tourner(Direction d, int speed)
{
	if(d == GAUCHE)
	{
		//digitalWrite (PIN24, 0);     // Off
		//digitalWrite (PIN8, 0);      // Off
		//digitalWrite (PIN7, 1);      // On
		//digitalWrite (PIN23, 1);     // On
		softPwmWrite(PIN24,0);
		softPwmWrite(PIN8,0);
		softPwmWrite(PIN7,speed);
		softPwmWrite(PIN23,speed);
		printf("Je tourne a gauche \n");

	}
	else 
	{
		//digitalWrite (PIN24, 1);     // On
		//digitalWrite (PIN8, 1);      // On
		//digitalWrite (PIN7, 0);      // Off
		//digitalWrite (PIN23, 0);     // Off
		softPwmWrite(PIN24,speed);
		softPwmWrite(PIN8,speed);
		softPwmWrite(PIN7,0);
		softPwmWrite(PIN23,0);
		printf("Je tourne a droite \n");
	}
}

int speedChange(SpeedChange s, int speed)
{
	if(s == UP)
	{
		if(speed != 1000) speed += 100;
		printf("vitesse = %d\n", speed);
	}
	else
	{
		if(speed != 0) speed -= 100;
		printf("vitesse = %d\n", speed);
	}
	return speed;
}

int ultrason()
{
	struct timeval begin, end;
	long int elapsedTime;
	
	digitalWrite (PIN17, 1);     // On
	
	digitalWrite (PIN17, 0);      // Off
	
	// nanosecond
	gettimeofday(&begin, NULL);
	gettimeofday(&end, NULL);
	elapsedTime = elapsedTime(begin, end);
	
	
}

long int elapsedTime(struct timeval begin, struct timeval end)
{
	
	
	return (end.tv_sec*(long int)1000000+end.tv_usec) - (begin.tv_sec*(long int)1000000+begin.tv_usec);
}

