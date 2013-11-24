#include "simlib.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int doba_voleb = 50*60;  //celkova doba volicu


class GenVolicu;
class Volic;

class Okrsek
{
private:
    int end_voleb;
    int cislo_okrsku;
    int cislo_kraje;
    char kraj[20];
    char mesto[20];
    int pocet_volicu;
    int pocet_zvolenych_listku;
    Store *komise;
    Store *plenty;
    Facility *urna;

public:
    Okrsek(int,int,char*,char*,int);
    int get_cislo_okrsku();
    int get_poctu_volicu();
    int get_poctu_zvolenych_listku();
    Store* get_komise();
    Store* get_plentu();
    Facility* get_urnu();
    void increment_poc_listku();
    void konec_voleb();
};

class Volic : public Process
{
private:
    Okrsek *okrsek;
public:
    Volic(Okrsek *o) : okrsek(o){}
    void Behavior(){
        // Zabrani clena komise
        Enter(*(okrsek->get_komise()), 1);
        Wait(Exponential(0.5));     //overovani volice 
        Leave(*(okrsek->get_komise()), 1);  //uvolneni clena komise

        Enter(*(okrsek->get_plentu()), 1);  //volic zabere plentu
        Wait(Exponential(0.25));        //volic vybira kandidata
        Leave(*(okrsek->get_plentu()), 1);  //volic uvolnuje plentu
        
        Seize(*(okrsek->get_urnu()));   //zabere urnu
        Wait(Exponential(1.0/30));        //vhazuje listek do urny
        Release(*(okrsek->get_urnu())); //uvolnuje urnu
        //odchazi ze systemu
    }
};

class TimerVoleb : public Process{
    private:
        Okrsek* okr;
    public:
        TimerVoleb (Okrsek *o) : okr(o){}
        void Behavior(){
            Wait(doba_voleb);
            okr->konec_voleb();
        }
};

class GenVolicu : public Event
{
private:
    Okrsek *okrsek;
    double prum_lidi;
public:
    GenVolicu(Okrsek *o) : okrsek(o) , prum_lidi((double)doba_voleb/okrsek->get_poctu_volicu()){}

    void Behavior(){
        (new Volic(okrsek))->Activate();
        okrsek->increment_poc_listku();
        if (okrsek->get_poctu_volicu() != okrsek->get_poctu_zvolenych_listku())
            Activate(Time + Exponential(prum_lidi));
        else
            okrsek->konec_voleb();
    }
};


/************ definovani metod tridy okresek ********************/
Okrsek::Okrsek(int c,int c_o,char *k,char *m,int p_v) : end_voleb(0),cislo_okrsku(c), cislo_kraje(c_o),
    pocet_volicu(p_v),komise(new Store("Komise", 12)), plenty(new Store("Plenty", 3)),
    urna(new Facility("Urna")){
    //zkopirujeme parametry
    strcpy(kraj,k);
    strcpy(mesto,m);
    pocet_zvolenych_listku = 0;
    //aktivujeme generator volicu
    (new GenVolicu(this))->Activate();
    //aktivuje timer, ktery zjistuje jestli ubehla doba voleb
    (new TimerVoleb(this))->Activate();
    
    
}

int Okrsek::get_cislo_okrsku(){
    return cislo_okrsku;
}

int Okrsek::get_poctu_volicu(){
    return pocet_volicu;
}

int Okrsek::get_poctu_zvolenych_listku(){
    return pocet_zvolenych_listku;
}

Store* Okrsek::get_komise(){
    return komise;
}

Store* Okrsek::get_plentu(){
    return plenty;
}

Facility* Okrsek::get_urnu(){
    return urna;
}

void Okrsek::increment_poc_listku(){
    ++pocet_zvolenych_listku;
}

void Okrsek::konec_voleb(){
    end_voleb = 1;
}


/************ konec definovani metod tridy okresek *************/

class GenOkrsku : public Event
{
private:
    static int i;
public:
    void Behavior(){
        FILE *soubor;
        int test,c_k,p_v,celkovy_pocet_okresku;
        char k[20],m[20];
        soubor = fopen("okrsky.txt", "r");
        //nacteme prvni cislo..urcuje pocet okrsku
        test = fscanf(soubor,"%d",&celkovy_pocet_okresku);
        if (test != 1)
            fprintf(stderr,"chyba pri nacitani");
        //cyklus, ktery bude generovat tridy okresku
        for ( ; i < celkovy_pocet_okresku ; ++i){
            test = fscanf(soubor,"%d%s%s%d",&c_k,k,m,&p_v);
            //vynikla-li chyba pri nacitani ze souboru
            if (test == 1)
                fprintf(stderr,"chyba pri nacitani");
            //nainicialuzuje se novy okersek
            new Okrsek(i,c_k,k,m,p_v);
        }
    }
};

int GenOkrsku::i = 1;

int main(void){

    Init(0, 1);
    
    (new GenOkrsku)->Activate();

    Run();

    std::cout <<"celkovy" <<std::endl;

}
