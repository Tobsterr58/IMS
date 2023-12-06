#include "simlib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>


using namespace std;

int width = 50;

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DEFINICIA KONSTANT A CASOV  ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////// OPENING HOURS //////////////////////////////////////////////////

int START = 0;
int END = 900;
#define WEEKDAYS 900
#define WEEKEND 780
#define WEEKDAYS_PEEKHOURS_START 420
#define WEEKEND_PEEKHOURS_START 360

// Pole pre dni v tyzdni
string days[] = {"Pondelok", "Utorok", "Streda", "Stvrtok", "Piatok", "Sobota", "Nedela"};

///////////////////////////////////////////////////////////////////////////////////////////////////

#define MIN_TIME_BOX 60
#define MAX_TIME_BOX 90
#define MIN_TIME_KARDIO 45
#define MAX_TIME_KARDIO 75
#define SQUASH_TIME 60
#define BEFORE_CHANGE_ROOM_TIME_MIN 3
#define BEFORE_CHANGE_ROOM_TIME_MAX 10
#define CHANGE_ROOM_AFTER_TRAINING 5
#define SHOWER_TIME_AFTER_TRAINING 20
#define NESPOKOJNY_ZAKAZNIK_GYM 20
#define NESPOKOJNY_ZAKAZNIK_SQUASH 10
#define CAKANIE_NA_KURT 30

//////////////////////////////// LEGS EXERCISES //////////////////////////////////////////////////

#define LEGS_EXERCISE_1_AVERAGE 15
#define LEGS_EXERCISE_2_AVERAGE 10
#define LEGS_EXERCISE_3_AVERAGE 10
#define LEGS_EXERCISE_4_AVERAGE 10
#define LEGS_EXERCISE_5_AVERAGE 10
#define LEGS_EXERCISE_6_AVERAGE 10
#define LEGS_EXERCISE_7_AVERAGE 10
#define LEGS_EXERCISE_8_AVERAGE 8

int LEGS_EXERCISES[8] = {LEGS_EXERCISE_1_AVERAGE, LEGS_EXERCISE_2_AVERAGE, LEGS_EXERCISE_3_AVERAGE, LEGS_EXERCISE_4_AVERAGE, LEGS_EXERCISE_5_AVERAGE, LEGS_EXERCISE_6_AVERAGE, LEGS_EXERCISE_7_AVERAGE, LEGS_EXERCISE_8_AVERAGE};

//////////////////////////////// PUSH EXERCISES //////////////////////////////////////////////////

#define PUSH_EXERCISE_1_AVERAGE 15
#define PUSH_EXERCISE_2_AVERAGE 10
#define PUSH_EXERCISE_3_AVERAGE 10
#define PUSH_EXERCISE_4_AVERAGE 10
#define PUSH_EXERCISE_5_AVERAGE 10

int PUSH_EXERCISES[5] = {PUSH_EXERCISE_1_AVERAGE, PUSH_EXERCISE_2_AVERAGE, PUSH_EXERCISE_3_AVERAGE, PUSH_EXERCISE_4_AVERAGE, PUSH_EXERCISE_5_AVERAGE};

//////////////////////////////// PULL EXERCISES //////////////////////////////////////////////////

#define PULL_EXERCISE_1_AVERAGE 15
#define PULL_EXERCISE_2_AVERAGE 10
#define PULL_EXERCISE_3_AVERAGE 10
#define PULL_EXERCISE_4_AVERAGE 10
#define PULL_EXERCISE_5_AVERAGE 10

int PULL_EXERCISES[5] = {PULL_EXERCISE_1_AVERAGE, PULL_EXERCISE_2_AVERAGE, PULL_EXERCISE_3_AVERAGE, PULL_EXERCISE_4_AVERAGE, PULL_EXERCISE_5_AVERAGE};

///////////////////////////////////////////////////////////////////////////////////////////////////

enum typeOfWait {
  GymKeys,
  SquashKeys,
  Court,
  };

// Definicia premennych pre pocet strojov pre jednotlive cvicenia a cviky a pocet klucov

int kluce_gym = 120;
int kluce_squash = 20;
int stroje_pull = 22;
int stroje_push = 22;
int stroje_legs = 34;
int box_room = 7;
int kardio_stroje = 15;
int squash_kurt = 5;

int pocet_zakaznikov = 0;
int pocet_zakaznikov_squash = 0;
int pocet_zakaznikov_gym = 0;
int pocet_zakaznikov_box = 0;
int pocet_zakaznikov_kardio = 0;
int pocet_zakaznikov_legs = 0;
int pocet_zakaznikov_pull = 0;
int pocet_zakaznikov_push = 0;

// Definicia premennych pre pocet nespokojných zakaznikov a ID zakaznika
int customer_id = 0;
int nespokojny_zakaznik = 0;

// Definicia premenných pre výpis času stráveného čakaním na niečo
double cakanie_na_kurt = 0.0;
double cakanie_na_kluc_gym = 0.0;
double cakanie_na_kluc_squash = 0.0;
double cakanie_na_stroj_pull = 0.0;
double cakanie_na_stroj_push = 0.0;
double cakanie_na_stroj_legs = 0.0;
double cakanie_na_box = 0.0;
double cakanie_na_kardio = 0.0;

////////////////////////////////////////////////////////////////////

////////////////////////////// WEEK REPORT STATS //////////////////////////////////////////////////

int celkovy_pocet_zakaznikov = 0;
int celkovy_pocet_zakaznikov_squash = 0;
int celkovy_pocet_zakaznikov_gym = 0;
int celkovy_pocet_zakaznikov_box = 0;
int celkovy_pocet_zakaznikov_kardio = 0;
int celkovy_pocet_zakaznikov_legs = 0;
int celkovy_pocet_zakaznikov_pull = 0;
int celkovy_pocet_zakaznikov_push = 0;
int celkovy_pocet_nespokojnych_zakaznikov = 0;

double celkovy_cas_cakania_na_kurt = 0.0;
double celkovy_cas_cakania_na_kluc_gym = 0.0;
double celkovy_cas_cakania_na_kluc_squash = 0.0;
double celkovy_cas_cakania_na_stroj_pull = 0.0;
double celkovy_cas_cakania_na_stroj_push = 0.0;
double celkovy_cas_cakania_na_stroj_legs = 0.0;
double celkovy_cas_cakania_na_box = 0.0;
double celkovy_cas_cakania_na_kardio = 0.0;

int nespokojny_zakaznici_kluce_gym = 0;
int nespokojny_zakaznici_kluce_squash = 0;
int nespokojny_zakaznici_kurt = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////

void clearStats() {
  pocet_zakaznikov = 0;
  pocet_zakaznikov_squash = 0;
  pocet_zakaznikov_gym = 0;
  pocet_zakaznikov_box = 0;
  pocet_zakaznikov_kardio = 0;
  pocet_zakaznikov_legs = 0;
  pocet_zakaznikov_pull = 0;
  pocet_zakaznikov_push = 0;
  customer_id = 0;
  nespokojny_zakaznik = 0;
  cakanie_na_kurt = 0.0;
  cakanie_na_kluc_gym = 0.0;
  cakanie_na_kluc_squash = 0.0;
  cakanie_na_stroj_pull = 0.0;
  cakanie_na_stroj_push = 0.0;
  cakanie_na_stroj_legs = 0.0;
  cakanie_na_box = 0.0;
  cakanie_na_kardio = 0.0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void printStats() {
  cout << "|                    STATISTIKY                   |" << endl;
  cout << "+-------------------------------------------------+" << endl;
  cout << fixed << setprecision(2); // Set floating-point precision to 2 decimal places

  cout << "| Pocet zakaznikov                    : " << setw(width - 40) << pocet_zakaznikov << "|" << endl;
  cout << "| Pocet nespojnych zakaznikov         : " << setw(width - 40) << nespokojny_zakaznik << "|" << endl;
  cout << "| Pocet zakaznikov pre squash         : " << setw(width - 40) << pocet_zakaznikov_squash << "|" << endl;
  cout << "| Pocet zakaznikov pre gym            : " << setw(width - 40) << pocet_zakaznikov_gym << "|" << endl;
  cout << "| Cakanie na kluce od skrinky (gym)   : " << setw(width - 40) << cakanie_na_kluc_gym << "|" << endl;
  cout << "| Cakanie na kluce od skrinky (squash): " << setw(width - 40) << cakanie_na_kluc_squash << "|" << endl;
  cout << "| Cakanie na kurt pre squash          : " << setw(width - 40) << cakanie_na_kurt << "|" << endl;
  cout << "| Cakanie na stroj `pull`             : " << setw(width - 40) << cakanie_na_stroj_pull << "|" << endl;
  cout << "| Cakanie na stroj `push`             : " << setw(width - 40) << cakanie_na_stroj_push << "|" << endl;
  cout << "| Cakanie na stroj `legs`             : " << setw(width - 40) << cakanie_na_stroj_legs << "|" << endl;
  cout << "| Cakanie pre box                     : " << setw(width - 40) << cakanie_na_box << "|" << endl;
  cout << "| Cakanie pre kardio                  : " << setw(width - 40) << cakanie_na_kardio << "|" << endl;
  cout << "+-------------------------------------------------+" << endl;
  cout << endl;
  cout << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void storeStats() {
  celkovy_pocet_zakaznikov += pocet_zakaznikov;
  celkovy_pocet_zakaznikov_squash += pocet_zakaznikov_squash;
  celkovy_pocet_zakaznikov_gym += pocet_zakaznikov_gym;
  celkovy_pocet_zakaznikov_box += pocet_zakaznikov_box;
  celkovy_pocet_zakaznikov_kardio += pocet_zakaznikov_kardio;
  celkovy_pocet_zakaznikov_legs += pocet_zakaznikov_legs;
  celkovy_pocet_zakaznikov_pull += pocet_zakaznikov_pull;
  celkovy_pocet_zakaznikov_push += pocet_zakaznikov_push;
  celkovy_pocet_nespokojnych_zakaznikov += nespokojny_zakaznik;
  celkovy_cas_cakania_na_kurt += cakanie_na_kurt;
  celkovy_cas_cakania_na_kluc_gym += cakanie_na_kluc_gym;
  celkovy_cas_cakania_na_kluc_squash += cakanie_na_kluc_squash;
  celkovy_cas_cakania_na_stroj_pull += cakanie_na_stroj_pull;
  celkovy_cas_cakania_na_stroj_push += cakanie_na_stroj_push;
  celkovy_cas_cakania_na_stroj_legs += cakanie_na_stroj_legs;
  celkovy_cas_cakania_na_box += cakanie_na_box;
  celkovy_cas_cakania_na_kardio += cakanie_na_kardio;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int week_width = 80; // Set width for the printWeekReport function

void printWeekReport() {
  cout << "+--------------------------------------------------------------------------------+" << endl;
  cout << "|                                WEEK REPORT                                     |" << endl;
  cout << "+--------------------------------------------------------------------------------+" << endl;
  cout << fixed << setprecision(2); // Set floating-point precision to 2 decimal places

  auto printRow = [&](const string& label, double value) {
    cout << "| " << left << setw(60) << label << ": " << setw(week_width - 64) << value << " |" << endl;
  };

  auto printSeparator = [&]() {
    cout << "+--------------------------------------------------------------------------------+" << endl;
  };

  printRow("Celkovy pocet zakaznikov", celkovy_pocet_zakaznikov);
  printRow("Celkovy pocet nespojnych zakaznikov", celkovy_pocet_nespokojnych_zakaznikov);
  printRow("Celkovy pocet zakaznikov pre squash", celkovy_pocet_zakaznikov_squash);
  printRow("Celkovy pocet zakaznikov pre gym", celkovy_pocet_zakaznikov_gym);
  printSeparator();
  printRow("Priemerny cas cakania na kluce gym (min)", celkovy_cas_cakania_na_kluc_gym / celkovy_pocet_zakaznikov_gym);
  printRow("Priemerny cas cakania na kluce squash (min)", celkovy_cas_cakania_na_kluc_squash / celkovy_pocet_zakaznikov_squash);
  printRow("Priemerny cas cakania na kurt (min)", celkovy_cas_cakania_na_kurt / celkovy_pocet_zakaznikov_squash);
  printRow("Priemerny cas cakania na stroj `pull` (min)", celkovy_cas_cakania_na_stroj_pull / celkovy_pocet_zakaznikov_pull);
  printRow("Priemerny cas cakania na stroj `push` (min)", celkovy_cas_cakania_na_stroj_push / celkovy_pocet_zakaznikov_push);
  printRow("Priemerny cas cakania na stroj `legs` (min)", celkovy_cas_cakania_na_stroj_legs / celkovy_pocet_zakaznikov_legs);
  printRow("Priemerny cas cakania pre box (min)", celkovy_cas_cakania_na_box / celkovy_pocet_zakaznikov_box);
  printRow("Priemerny cas cakania pre kardio (min)", celkovy_cas_cakania_na_kardio / celkovy_pocet_zakaznikov_kardio);
  printSeparator();
  printRow("Nespokojni zakaznici, ktori odisli pretoze kluce gym", nespokojny_zakaznici_kluce_gym);
  printRow("Nespokojni zakaznici, ktori odisli pretoze kluce squash", nespokojny_zakaznici_kluce_squash);
  printRow("Nespokojni zakaznici, ktori odisli pretoze kurt na squash", nespokojny_zakaznici_kurt);
  printSeparator();
  printRow("Celkovy pocet zakaznikov, ktori cvicili 'pull'", celkovy_pocet_zakaznikov_pull);
  printRow("Celkovy pocet zakaznikov, ktori cvicili 'push'", celkovy_pocet_zakaznikov_push);
  printRow("Celkovy pocet zakaznikov, ktori cvicili 'legs'", celkovy_pocet_zakaznikov_legs);
  printRow("Celkovy pocet zakaznikov, ktori cvicili 'box'", celkovy_pocet_zakaznikov_box);
  printRow("Celkovy pocet zakaznikov, ktori cvicili 'kardio'", celkovy_pocet_zakaznikov_kardio);
  printSeparator();
  cout << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// DEFINICIA STOROV  /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

Store GYM_KLUCE ("Kluce na gym", kluce_gym);
Store SQUASH_KLUCE ("Kluce na squash", kluce_squash);

Store PULL_STROJE ("Stroje na pull", stroje_pull);
Store PUSH_STROJE ("Stroje na push", stroje_push);
Store LEGS_STROJE ("Stroje na legs", stroje_legs);
Store BOX_STROJE ("Stroje na box", box_room);
Store KARDIO_STROJE ("Stroje na kardio", kardio_stroje);
Store SQUASH_KURTY ("Kurty na squash", squash_kurt);


///////////////////////////////////////////////////////////////////////////////////////////////////
class Timeout : public Event {
  Process *ptr;        // proces, ktorý čaká
  typeOfWait type;    // typ čakania
  int ID;            // ID zakaznika
  public:
    Timeout(double t, Process *p, typeOfWait type, int ID): ptr(p), type(type), ID(ID) {
      Activate(Time+t);  // kdy vyprší timeout
    }
    void Behavior() {
      ptr->Out();        // vyjmout z fronty
      if (type == Court) {
        ptr->Wait(Uniform(CHANGE_ROOM_AFTER_TRAINING, SHOWER_TIME_AFTER_TRAINING)); // Prezliekanie sa
        ptr->Leave(SQUASH_KLUCE, 2); // Vratenie klucov na squash
      }
      delete ptr;        // likvidace
      if (type == GymKeys) {
        cakanie_na_kluc_gym += NESPOKOJNY_ZAKAZNIK_GYM;
        nespokojny_zakaznici_kluce_gym++;
      }
      else if (type == SquashKeys) {
        cakanie_na_kluc_squash += NESPOKOJNY_ZAKAZNIK_SQUASH;
        nespokojny_zakaznici_kluce_squash++;
      }
      else if (type == Court) {
        cakanie_na_kurt = cakanie_na_kurt + CAKANIE_NA_KURT;
        nespokojny_zakaznici_kurt++;
      }

      nespokojny_zakaznik++;          // počitadlo
      Cancel();          // konec události (SIMLIB BUG)
    }
};

class Customer : public Process { 
  double Prichod;
  int ID;
  public :
  Customer(int ID) : Process() { this->ID = ID; }
  void Behavior() {              
    Prichod = Time; // Cas prichodu zakaznika
    pocet_zakaznikov++;
    Event *timeout;
    double vyber_cvicenia;

    // Zadelenie zakaznika do fronty na gym alebo squash
    if(Random() < 0.95) { // 95% zakaznikov ide do gymu
      pocet_zakaznikov_gym++;
      timeout = new Timeout(NESPOKOJNY_ZAKAZNIK_GYM, this, GymKeys, ID); // vytvoření události timeout
      double pomocnaGym = Time;
      Enter(GYM_KLUCE, 1); // Ziskanie kluca na gym
      cakanie_na_kluc_gym += Time - pomocnaGym;
      delete timeout; // zrušení timeoutu
      Wait(Exponential(1)); // Cakanie na recepcii

      Wait(Uniform(BEFORE_CHANGE_ROOM_TIME_MIN, BEFORE_CHANGE_ROOM_TIME_MAX)); // Prezliekanie sa
      vyber_cvicenia = Random();
      // 25% zakaznikov cvici `Pull` cviky, 25% `Push` cviky, 31% `Legs` cviky, 6% `Box` cviky, 14% `Kardio` cviky
      if (vyber_cvicenia < 0.25) {
        for (int i = 0; i < 5; i++) {
          double pomocnaPull = Time;
          Enter(PULL_STROJE, 1); // Ziskanie stroja na pull
          cakanie_na_stroj_pull += Time - pomocnaPull;
          Wait(Uniform(PULL_EXERCISES[i]-2, PULL_EXERCISES[i]+8)); // Prebieha cvicenie
          Leave(PULL_STROJE, 1); // Vratenie stroja na pull
        }
        pocet_zakaznikov_pull++;
      }
      else if (vyber_cvicenia > 0.25 && vyber_cvicenia < 0.5) {
        for (int i = 0; i < 5; i++) {
          double pomocnaPush = Time;
          Enter(PUSH_STROJE, 1); // Ziskanie stroja na push
          cakanie_na_stroj_push += Time - pomocnaPush;
          Wait(Uniform(PUSH_EXERCISES[i]-2, PUSH_EXERCISES[i]+8)); // Prebieha cvicenie
          Leave(PUSH_STROJE, 1); // Vratenie stroja na push
        }
        pocet_zakaznikov_push++;
      }
      else if (vyber_cvicenia > 0.5 && vyber_cvicenia < 0.8) {
        for (int i = 0; i < 8; i++) {
          double pomocnaLegs = Time;
          Enter(LEGS_STROJE, 1); // Ziskanie stroja na legs
          cakanie_na_stroj_legs += Time - pomocnaLegs;
          Wait(Uniform(LEGS_EXERCISES[i]-1, LEGS_EXERCISES[i]+4)); // Prebieha cvicenie
          Leave(LEGS_STROJE, 1); // Vratenie stroja na legs
        }
        pocet_zakaznikov_legs++;
      }
      else if (vyber_cvicenia > 0.8 && vyber_cvicenia < 0.86) {
        double pomocnaBox = Time;
        Enter(BOX_STROJE, 1); // Ziskanie stroja na box
        cakanie_na_box += Time - pomocnaBox;
        Wait(Uniform(MIN_TIME_BOX, MAX_TIME_BOX)); // Prebieha cvicenie
        Leave(BOX_STROJE, 1); // Vratenie stroja na box
        pocet_zakaznikov_box++;
      }
      else if (vyber_cvicenia > 0.86 && vyber_cvicenia < 1) {
        double pomocnaKardio = Time;
        Enter(KARDIO_STROJE, 1); // Ziskanie stroja na kardio
        cakanie_na_kardio += Time - pomocnaKardio;
        Wait(Uniform(MIN_TIME_KARDIO, MAX_TIME_KARDIO)); // Prebieha cvicenie
        Leave(KARDIO_STROJE, 1); // Vratenie stroja na kardio
        pocet_zakaznikov_kardio++;
      }
      Wait(Uniform(CHANGE_ROOM_AFTER_TRAINING, SHOWER_TIME_AFTER_TRAINING)); // Prezliekanie sa
      Leave(GYM_KLUCE, 1); // Vratenie kluca na gym
    }
    else { // 5% zakaznikov ide na squash (2 zakaznici ako dvojica = 1 zakaznik pre squash)
      pocet_zakaznikov_squash+=2;
      timeout = new Timeout(NESPOKOJNY_ZAKAZNIK_SQUASH, this, SquashKeys, ID); // vytvoření události timeout
      double pomocnaSquash = Time;
      Enter(SQUASH_KLUCE, 2); // Ziskanie klucov na squash
      cakanie_na_kluc_squash += Time - pomocnaSquash;
      delete timeout; // zrušení timeoutu
      Wait(Exponential(1)); // Cakanie na recepcii

      Wait(Uniform(BEFORE_CHANGE_ROOM_TIME_MIN, BEFORE_CHANGE_ROOM_TIME_MAX)); // Prezliekanie sa
      timeout = new Timeout(CAKANIE_NA_KURT, this, Court, ID); // vytvoření události timeout
      double pomocnaKurty = Time;
      Enter(SQUASH_KURTY, 1); // Ziskanie kurtu na squash
      cakanie_na_kurt += Time - pomocnaKurty;
      delete timeout; // zrušení timeoutu
      Wait(SQUASH_TIME); // Prebieha hra
      Leave(SQUASH_KURTY, 1); // Vratenie kurtu na squash
      Wait(Uniform(CHANGE_ROOM_AFTER_TRAINING, SHOWER_TIME_AFTER_TRAINING)); // Prezliekanie sa
      Leave(SQUASH_KLUCE, 2); // Vratenie klucov na squash
    }
  }  
};

class Generator : public Event {
  string day;
  public:
  Generator(string day) : day(day) {}
  void Behavior() {     
    customer_id++;
    (new Customer(customer_id))->Activate();   // new customer
    if (day == "Pondelok" || day == "Utorok" || day == "Streda" || day == "Stvrtok" || day == "Piatok") {
      if (Time < END-60 ) { // 60 min pred koncom otvaracich hodin uz neprichadzaju zakaznici
        if(Time < START+WEEKDAYS_PEEKHOURS_START) { // do 14:00 chodi menej zakaznikov kvôli škole, práci a pod.
          Activate(Time+Exponential (4));
        }
        else { // 14:00 - Zaverečná chodi viac zakaznikov
          Activate(Time+Exponential (1));
        }
      }
    }
    else {

      // Cez vikend chodi menej zakaznikov preto sa meni cas medzi prichodmi zakaznikov

      if (Time < END-60 ) { // 60 min pred koncom otvaracich hodin uz neprichadzaju zakaznici
        if(Time < START+WEEKEND_PEEKHOURS_START) { // do 14:00 chodi menej zakaznikov pretoze spia dlhsie, hobbies, je vikend a pod.
          Activate(Time+Exponential (5));
        }
        else { // 14:00 - Zaverečná chodi viac zakaznikov
          Activate(Time+Exponential (1.5));
        }
      }
    }
      
  }
};

class ClearOut : public Process {
  void Behavior() { // V case zatvorenia sa vsetci zakaznici vyradia a odovzdaju svoje kluce a stroje
    Leave(SQUASH_KLUCE, SQUASH_KLUCE.Used());
    Leave(PULL_STROJE, PULL_STROJE.Used());
    Leave(PUSH_STROJE, PUSH_STROJE.Used());
    Leave(LEGS_STROJE, LEGS_STROJE.Used());
    Leave(BOX_STROJE, BOX_STROJE.Used());
    Leave(KARDIO_STROJE, KARDIO_STROJE.Used());
    Leave(SQUASH_KURTY, SQUASH_KURTY.Used());
    Leave(GYM_KLUCE, GYM_KLUCE.Used());
  }
};

int main() {
  
  // Create random seed
  RandomSeed(time(NULL));

  for (int i = 0; i < 7; i++) {
    storeStats();
    clearStats();

    cout << "+-------------------------------------------------+" << endl;
    cout << "|" << setw((width + days[i].length()) / 2) << days[i] << setw((width - days[i].length()) / 2) << "|" << endl;
    cout << "+-------------------------------------------------+" << endl;
    
    Init(START, END);

    // Activate the customer generator
    (new Generator(days[i]))->Activate();
    (new ClearOut)->Activate(END);

    Run();

    if (i < 5) {
      START += WEEKDAYS;
      END += WEEKDAYS;
    }
    else if (i == 5) { // Sobota + Nedeľa aby sa zmenil čas medzi prichodmi zakaznikov na vikend
      START += 1000;
      END += 1000-120; // -120 aby sa prevadzka obmedzila iba na 13 hodin, pretoze vikend je otvoreny od 8:00 do 21:00
    }
    else {
      START += WEEKEND;
      END += WEEKEND;
    }
    // Vypis statistik
    printStats();
  }
  printWeekReport();
  return 0;
}
