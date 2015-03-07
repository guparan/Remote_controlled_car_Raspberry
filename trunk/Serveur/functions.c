#include "functions.h"

#include <stdio.h>
//long int elapsedTime2(struct timeval begin, struct timeval end);


void initGPIO()
{
	printf("Initialisation des GPIO\n");

	if(wiringPiSetup() == -1) return;
	
	//pinMode(PIN23, OUTPUT);
	//pinMode(PIN24, OUTPUT);
	pinMode(PIN17, OUTPUT);	//	For TRIG envoyer
	pinMode(PIN27, INPUT);	//	For ECHO recevoir
	
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
	int distance = 0;
	long startTime = 0;
	long travelTime = 0;
	
	digitalWrite(PIN17, 0);  //   TRIG pin must start off
	delay(30);
	
	//	Send trig pulse
	digitalWrite (PIN17, 1);      // On
	delayMicroseconds(20);
	digitalWrite (PIN17, 0);     // Off	
	
	//	Wait for echo start
	while(digitalRead(PIN27) == 0);
	
	//	Wait for echo end
	startTime = micros();		
	
	
	while(digitalRead(PIN27) == 1);
	
	travelTime = micros() - startTime;
	
	 //Get distance in cm
	distance = travelTime / 58;

	return distance;
}
/*
long int elapsedTime2(struct timeval begin, struct timeval end)
{
	int i;
	
	return (end.tv_sec*(long int)1000000+end.tv_usec) - (begin.tv_sec*(long int)1000000+begin.tv_usec);
}
*/