#include "simlib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DEFINICIA KONSTANT A CASOV  ///////////////////////////////////////////
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
  Box,
  Kardio,
  Legs,
  Pull,
  Push,
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
        cout << "Zakaznik " << ID << " odisiel nespokojny lebo kluce gym" << endl;
      }
      else if (type == SquashKeys) {
        cakanie_na_kluc_squash += NESPOKOJNY_ZAKAZNIK_SQUASH;
        cout << "Zakaznik " << ID << " odisiel nespokojny lebo kluce squash" << endl;
      }
      else if (type == Court) {
        cakanie_na_kurt = cakanie_na_kurt + CAKANIE_NA_KURT;
        cout << "Zakaznik " << ID << " odisiel nespokojny lebo cakal na kurt pre squash moc dlho" << endl;
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
          Wait(Uniform(LEGS_EXERCISES[i]-2, LEGS_EXERCISES[i]+8)); // Prebieha cvicenie
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
  void Behavior() {     
    customer_id++;
    (new Customer(customer_id))->Activate();   // new customer
    if(Time < 420) { // 7:00 - 14:00 chodi menej zakaznikov kvôli škole, práci a pod.
      Activate(Time+Exponential (4));
    }
    else { // 14:00 - 22:00 chodi viac zakaznikov
      Activate(Time+Exponential (1));
    }
  }
};

int main() {

  // Create random seed
  RandomSeed(time(NULL));

  // Initialize the simulation
  Init(0, 900);

  // Activate the customer generator
  (new Generator)->Activate();

  // Run the simulation
  Run();

  // Print the results
  cout << "Pocet zakaznikov: " << pocet_zakaznikov << endl;
  cout << "Pocet zakaznikov gym: " << pocet_zakaznikov_gym << endl;
  cout << "Pocet zakaznikov squash: " << pocet_zakaznikov_squash << endl;
  cout << "Pocet nespokojnych zakaznikov: " << nespokojny_zakaznik << endl;
  // cout << "Pocet zakaznikov na pull: " << pocet_zakaznikov_pull << endl;
  // cout << "Pocet zakaznikov na push: " << pocet_zakaznikov_push << endl;
  // cout << "Pocet zakaznikov na legs: " << pocet_zakaznikov_legs << endl;
  // cout << "Pocet zakaznikov na box: " << pocet_zakaznikov_box << endl;
  // cout << "Pocet zakaznikov na kardio: " << pocet_zakaznikov_kardio << endl;
  cout << "Zakaznici stravili cakanim na kurt: " << cakanie_na_kurt << endl;
  cout << "Zakaznici stravili cakanim na kluc gym: " << cakanie_na_kluc_gym << endl;
  cout << "Zakaznici stravili cakanim na kluc squash: " << cakanie_na_kluc_squash << endl;
  cout << "Zakaznici stravili cakanim na stroj pull: " << cakanie_na_stroj_pull << endl;
  cout << "Zakaznici stravili cakanim na stroj push: " << cakanie_na_stroj_push << endl;
  cout << "Zakaznici stravili cakanim na stroj legs: " << cakanie_na_stroj_legs << endl;
  cout << "Zakaznici stravili cakanim na box: " << cakanie_na_box << endl;
  cout << "Zakaznici stravili cakanim na kardio: " << cakanie_na_kardio << endl;

  return 0;
}