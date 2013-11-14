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
const int POC_PREPAZEK = 5;

Facility prepazky[POC_PREPAZEK];
Histogram celk("Celkova doba v systemu", 0, 5, 20);

//definicia systemu
class Prepocet : public Process {
    void Behavior() {
        double prichod = Time;
        
        bool spocteno_nespravne = 1;
        for (;spocteno_nespravne;){  //zjistuje se jestli je prepocet volebni komise spravny
            
            Wait(Exponential(60*120));  //ceka se nez jsou zpoctene hlasy
            
            Wait(Exponential(30*60));    //cesta na urad
            
            int i = 0;
            for (int a=1; a < POC_PREPAZEK; a++)
                if (prepazky[a].QueueLen() < prepazky[i].QueueLen())
                    i=a;
            
            Seize(prepazky[i]);
            Wait(Uniform(300,600));         //kontrola vysledku
            Release(prepazky[i]);
            if (Random() <= 0.9)            //vysledky jsou spravne
                spocteno_nespravne = 0;
            
        }
        
        //tady by se to nejak melo dale schazet a pocitat
        
        
        celk(Time-prichod);
    } // Behavior
}; // Volic

