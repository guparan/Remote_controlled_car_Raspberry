#include "functions.h"

void initGPIO()
{
	pinMode(PIN23, OUTPUT);
	pinMode(PIN24, OUTPUT);
	pinMode(PIN8, OUTPUT);
	pinMode(PIN7, OUTPUT);
}

void avancer()
{
	digitalWrite (PIN24, 1);     // On
    digitalWrite (PIN8, 0);      // Off
    digitalWrite (PIN7, 1);      // On
    digitalWrite (PIN23, 0);     // Off
}

void reculer()
{

}

void tourner(Direction d)
{

}