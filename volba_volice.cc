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
const int POC_STOLU = 5;

Facility volna_plenta("volna plenta");
Facility volna_urna("volna urna");
Facility volne_stoly[POC_STOLU];
Histogram celk("Celkova doba v systemu", 0, 5, 20);

//definicia systemu
class Volic : public Process {
    void Behavior() {
        double prichod = Time;
        
        int i = 0;
        for (int a=1; a < POC_STOLU; a++)
            if (volne_stoly[a].QueueLen() < volne_stoly[i].QueueLen())
                i=a;

        Seize(volne_stoly[i]);         //zabereme jeden stul
        if (Random() <= 0.005)         //nema obcanku
            Release(volne_stoly[i]);   //odchazi ze systemu
        Wait( Uniform(25,35) );       //overovani totoznosti
        if (Random() <= 0.1){           //volic nema volebni listky
            Wait(Uniform(10,11));       //volic dostava volebni listky
        }
        Release(volne_stoly[i]);
        
        Wait(Uniform(8,12));            //jde k plente
        
        Seize(volna_plenta);           //zabira si volnou plentu
        Wait(Uniform(20,21));            //skrta sve kandidaty
        Release(volna_plenta);            //uvolnuje plentu
        
        Wait(Uniform(8,12));            //jde k urne
        
        Seize(volna_urna);                //zabira volnou urnu
        Wait(Exponential(2));            //vhazuje hlasovaci listek
        Release(volna_urna);
        
        Wait(Exponential(10));            //odchazi z mistnosti        
        
        
        celk(Time-prichod);
    } // Behavior
}; // Volic

class Prichody : public Event {
    void Behavior() {
        (new Volic)->Activate();
        Activate( Time + Exponential(60) ); //co deset minut prijde novy
    }
};


int main() 
{
//SetOutput("Samoo.dat");
    Init(0, 8000);
    (new Prichody)->Activate(); 

    Run();

    celk.Output();
    volna_plenta.Output();
    volna_urna.Output();
    for (int a=0; a < POC_STOLU; a++)
        volne_stoly[a].Output();
}

