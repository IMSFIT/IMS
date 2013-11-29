#include "simlib.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int doba_voleb = 14*60;  //celkova doba volicu
const int celkova_doba_voleb = 24*60;  //doba casu simulace

Facility Centrum_republiky("Volebni centrum");

Stat celkovy_cas_pocitani("poctani v komisy");
Stat doba_cekani_na_centrum("doba cekani na centrum");


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
    int spoctene_hlasy;
    int prave_pocitanych_listku;
    int pocet_zvolenych_listku;
    int poc_nep_listku;
    int poc_plat_listku;
    int lide_v_mistnosti;
    Store *komise;
    Store *plenty;
    Facility *urna;

public:
    Okrsek(int,int,char*,char*,int);
    int get_cislo_okrsku();
    int get_poctu_volicu();
    int get_poctu_zvolenych_listku();
    int get_spocitano_v_okrsku();
    int get_konec_voleb();
    int get_konec_pocitani_hlasu();
    int get_lide_v_mistnosti();
    int get_prave_pocitanych_listku();
    Store* get_komise();
    Store* get_plentu();
    Facility* get_urnu();
    void increment_poc_listku();
    void decrement_poc_lisktu();
    void increment_nep_listku();
    void decrement_lidi_v_mistnosti();
    void increment_lidi_v_mistnosti();
    void decrement_prave_pocitanych_listku();
    void increment_prave_pocitanych_listku();
    void increment_plat_listku();
    void get_nul_prepoctenym_listkum();
    void konec_voleb();
    void konec_pocitani_hlasu();
};

class Volic : public Process
{
private:
    Okrsek *okrsek;
public:
    Volic(Okrsek *o) : okrsek(o){}
    void Behavior(){
        int cv = okrsek->get_poctu_zvolenych_listku();
        okrsek->increment_lidi_v_mistnosti();
        std::cout <<"V prichazi do okr: " <<okrsek->get_cislo_okrsku() <<"c_v: "<<cv <<std::endl;
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
        okrsek->decrement_lidi_v_mistnosti();
        //odchazi ze systemu
        std::cout <<"V odchazi do okr: " <<okrsek->get_cislo_okrsku() <<"c_v: "<<cv <<std::endl;
    }
};


class PocitaniKonkretnihoHlasu: public Process{
    private:
        Okrsek* okr;
    public:
        PocitaniKonkretnihoHlasu(Okrsek* o) : okr(o){}
        void Behavior(){
            int cl = okr->get_poctu_zvolenych_listku();
            std::cout <<"start poc l c.: " <<cl <<std::endl;
            okr->increment_prave_pocitanych_listku();
            Enter(*(okr->get_komise()),1); //zabere se 1 clovek pro 1 hlas
            Wait(Uniform(1/4.0,1/2.4));        //clen pocita hlas
            Leave(*(okr->get_komise()),1); //clen se uvolni
            
            if (Random() < 0.0005)      //testuje platnost a neplatnost listku
                okr->increment_nep_listku();
            else
                okr->increment_plat_listku();
            okr->decrement_prave_pocitanych_listku();
            std::cout <<"kon poc l. c.: " <<cl <<std::endl;
        }
};


class GenHlasu : public Event{
    private:
        Okrsek* okr;
    public:
        GenHlasu(Okrsek* o) : okr(o){}
        void Behavior(){
            (new PocitaniKonkretnihoHlasu(okr))->Activate();
            //snizime pocet nezpracovanych listku
            okr->decrement_poc_lisktu();
            //testuje se zdali je co pocitat
            if(okr->get_poctu_zvolenych_listku() > 0){
                //aktivovani dalsiho pocitani listku v nulovem case
                Activate(Time);
            }
            else{
                okr->konec_pocitani_hlasu();
            }
        }
};

class PocitaniHlasu : public Process{
    private:
        Okrsek* okr;
    public:
        PocitaniHlasu (Okrsek *o) : okr(o){}
        void Behavior(){           
            double cas = Time,cas1;
            int nespravne_pocitano = 1;
            //zjistujeme jeslti komise musi vysledky pocitat znovu
            while (nespravne_pocitano){
                
                (new GenHlasu(okr))->Activate();
                //testuje se co minuta jeslti je vse spocteno
                for (int i = Time ; i < celkova_doba_voleb ; ++i ){
                    if (okr->get_konec_pocitani_hlasu()){
                        i = celkova_doba_voleb;
                    }
                    else
                        Wait(1);
                }
                for (int i = Time ; i < celkova_doba_voleb ; ++i){
                    if(okr->get_prave_pocitanych_listku() == 0){
                        i = celkova_doba_voleb;
                    }
                    else
                        Wait(0.5);
                }
                Wait(Exponential(30));      //prijeti na urad a schvaleni vysledku
                
                if (Random() < 0.999) {         //vysledky souhlasi
                    nespravne_pocitano = 0;  //muze se posilat do centra
                }
                else{
                    okr->get_nul_prepoctenym_listkum();
                }
            }
            
            cas1 = Time;
            Seize(Centrum_republiky);   //posilani vysledku na centrum
            Wait(1.0/30);               //posilani do centra
            Release(Centrum_republiky); //uvolneni centra
            doba_cekani_na_centrum(Time-cas1);
            celkovy_cas_pocitani(Time - cas);
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
        //kdyz uz vyprsel cas, volici uz nesmeji vejit do volebni komise
        if (!okrsek->get_konec_voleb()){
            
            (new Volic(okrsek))->Activate();
            okrsek->increment_poc_listku();
            if (okrsek->get_poctu_volicu() != okrsek->get_poctu_zvolenych_listku())
                Activate(Time + Exponential(prum_lidi));
            else
                okrsek->konec_voleb();
        }
    }
};

class TimerVoleb : public Process{
    private:
        Okrsek* okr;
    public:
        TimerVoleb (Okrsek *o) : okr(o){}
        void Behavior(){
            int i;
            
            /*****zacatek prichodu volicu****/
            //Time = 0
            //po kazde minute se testuje jestli neprisli vsichni volici
            for (i = 0 ; i < doba_voleb ; ++i){
                //prisli vsichni volici cyklus bude koncit
                if (okr->get_konec_voleb()){
                    //std::cout <<"tu jsem este nebyl" <<std::endl;
                    i = doba_voleb;
                }
                else
                    Wait(1);
            }
            okr->konec_voleb();
            //ceka se az vsichni volici vyjdou z mistnosti co minutu se na ne ceka
            while (okr->get_lide_v_mistnosti()){
                Wait(1);
            }
            /************konec prichodu volicu*******/
            std::cout <<"konec chozeni volicu" <<std::endl;
            //rozjede se pocitani v komisi okrsku
            (new PocitaniHlasu(okr))->Activate();
        }
};


/************ definovani metod tridy okresek ********************/
Okrsek::Okrsek(int c,int c_o,char *k,char *m,int p_v) : end_voleb(0), 
    cislo_okrsku(c), cislo_kraje(c_o), pocet_volicu(p_v),spoctene_hlasy(0),prave_pocitanych_listku(0),
    lide_v_mistnosti(0),komise(new Store("Komise", 12)),
    plenty(new Store("Plenty", 3)),urna(new Facility("Urna")){
    
    //zkopirujeme parametry
    strcpy(kraj,k);
    strcpy(mesto,m);
    pocet_zvolenych_listku = 0;
    //aktivujeme generator volicu
    (new GenVolicu(this))->Activate();
    //zacina prepocitavani halsu v tomto okrsku
    (new TimerVoleb(this))->Activate();
    //konec veskereho ziti okrsku....spocitane halasy a jsou zkontrolovany a vyslany do onech mist Centra voleb
}

int Okrsek::get_cislo_okrsku(){
    return cislo_okrsku;
}

int Okrsek::get_spocitano_v_okrsku(){
    return spoctene_hlasy;
}

int Okrsek::get_poctu_volicu(){
    return pocet_volicu;
}

int Okrsek::get_poctu_zvolenych_listku(){
    return pocet_zvolenych_listku;
}

int Okrsek::get_konec_voleb(){
    return end_voleb;
}

int Okrsek::get_konec_pocitani_hlasu(){
    return spoctene_hlasy;
}

int Okrsek::get_lide_v_mistnosti(){
    return lide_v_mistnosti;
}

int Okrsek::get_prave_pocitanych_listku(){
    return prave_pocitanych_listku;
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

void Okrsek::decrement_poc_lisktu(){
    --pocet_zvolenych_listku;
}

void Okrsek::increment_lidi_v_mistnosti(){
    ++lide_v_mistnosti;
}

void Okrsek::decrement_lidi_v_mistnosti(){
    --lide_v_mistnosti;
}

void Okrsek::increment_prave_pocitanych_listku(){
    ++prave_pocitanych_listku;
}

void Okrsek::decrement_prave_pocitanych_listku(){
    --prave_pocitanych_listku;
}

void Okrsek::increment_nep_listku(){
    ++poc_nep_listku;
}

void Okrsek::get_nul_prepoctenym_listkum(){
    //nastavime hodnotu nezpocitanych zvolenych listku na puvodni stav
    pocet_zvolenych_listku = poc_nep_listku + poc_plat_listku;
    poc_nep_listku = 0;
    poc_plat_listku = 0;
}


void Okrsek::increment_plat_listku(){
    ++poc_plat_listku;
}


void Okrsek::konec_voleb(){
    end_voleb = 1;
}

void Okrsek::konec_pocitani_hlasu(){
    spoctene_hlasy = 1;
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
        //celkovy_pocet_okresku = 1;
        //cyklus, ktery bude generovat tridy okresku
        for ( ; i <= celkovy_pocet_okresku ; ++i){
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

    Init(0, celkova_doba_voleb);
    
    (new GenOkrsku)->Activate();

    Run();
    
    doba_cekani_na_centrum.Output();
    celkovy_cas_pocitani.Output();
    Centrum_republiky.Output();

}
