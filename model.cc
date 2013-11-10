/*
 * Projekt z IMS: SHO Volebná infrastruktura
 * Autori: Martin Maga, Vojtech Meca
 * Loginy: xmagam00, xmecav00
 * Datum: 15.11.2013
 */

#include "simlib.h"
#include <stdio.h>
#include <stdlib.h>

//deklaracie store, facility
const int POC_POKLADEN = 5;

Facility Pokladny[POC_POKLADEN];
Store Lahudky("Oddeleni lahudek", 2);
Store Voziky("Seradiste voziku", 50);
Histogram celk("Celkova doba v systemu", 0, 5, 20);

//definicia systemu
class Zakaznik : public Process {
void Behavior() {
double prichod = Time;

Enter(Voziky, 1);
if ( Random() <= 0.30 ) {
Enter(Lahudky, 1);
Wait(Exponential(2));
Leave(Lahudky, 1);
}
Wait(Uniform(10, 15));

int i = 0;
for (int a=1; a < POC_POKLADEN; a++)
if (Pokladny[a].QueueLen() < Pokladny[i].QueueLen())
i=a;

Seize(Pokladny[i]);
Wait( Exponential(3) );
Release(Pokladny[i]);
Leave(Voziky, 1);
celk(Time-prichod);
} // Behavior
}; // Zakaznik

class Prichody : public Event {
void Behavior() {
(new Zakaznik)->Activate();
Activate( Time + Exponential(8) );
}
};


int main() 
{
//SetOutput("Samoo.dat");
Init(0, 1000);
(new Prichody)->Activate(); 

Run();

celk.Output();
Lahudky.Output();
Voziky.Output();
for (int a=0; a < POC_POKLADEN; a++)
Pokladny[a].Output();
}

