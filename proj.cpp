#include "simlib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DEFINICIA KONSTANT A CASOV  ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#define MIN_TIME_PULL 60
#define MAX_TIME_PULL 120
#define MIN_TIME_PUSH 65
#define MAX_TIME_PUSH 120
#define MIN_TIME_LEGS 70
#define MAX_TIME_LEGS 120
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
int kluce_gym = 140;
int kluce_squash = 20;
int stroje_pull = 28;
int stroje_push = 30;
int stroje_legs = 36;
int box_room = 8;
int kardio_stroje = 20;
int squash_kurt = 5;

int pocet_zakaznikov = 0;
int pocet_zakaznikov_squash = 0;
int pocet_zakaznikov_gym = 0;
int pocet_zakaznikov_box = 0;
int pocet_zakaznikov_kardio = 0;
int pocet_zakaznikov_legs = 0;
int pocet_zakaznikov_pull = 0;
int pocet_zakaznikov_push = 0;

int customer_id = 0;
int nespokojny_zakaznik = 0;

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
  Process *ptr;        // který proces
  typeOfWait type;    // typ čekání
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
        cout << "Dvojica " << ID << " vratila kluce od squashu" << endl;
      }
      delete ptr;        // likvidace
      if (type == GymKeys) {
        cout << "Zakaznik " << ID << " odisiel nespokojny lebo kluce gym" << endl;
      }
      else if (type == SquashKeys) {
        cout << "Zakaznik " << ID << " odisiel nespokojny lebo kluce squash" << endl;
      }
      else if (type == Court) {
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
    float vyber_cvicenia;

    cout << "Prichod zakaznika: "<< ID << " v case : " << Prichod << endl;
    // Zadelenie zakaznika do fronty na gym alebo squash
    if(Random() < 0.9) { // 90% zakaznikov ide na gym
      pocet_zakaznikov_gym++;
      cout << "Zakaznik " << ID << " ide do gymu" << endl;
      timeout = new Timeout(NESPOKOJNY_ZAKAZNIK_GYM, this, GymKeys, ID); // vytvoření události timeout
      cout << "Pocet volnych klucov na gym: " << GYM_KLUCE.Free() << endl;
      Enter(GYM_KLUCE, 1); // Ziskanie kluca na gym
      delete timeout; // zrušení timeoutu
      Wait(Exponential(1)); // Cakanie na recepcii

      Wait(Uniform(BEFORE_CHANGE_ROOM_TIME_MIN, BEFORE_CHANGE_ROOM_TIME_MAX)); // Prezliekanie sa
      vyber_cvicenia = Random();
      cout << "Zakaznik " << ID << " zacina cvicit v case :" << Time << endl;
      if (vyber_cvicenia < 0.23) {
        Enter(PULL_STROJE, 1); // Ziskanie stroja na pull
        cout << "Zakaznik " << ID << " zacina cvicit pull" << endl;
        Wait(Uniform(MIN_TIME_PULL, MAX_TIME_PULL)); // Prebieha cvicenie
        Leave(PULL_STROJE, 1); // Vratenie stroja na pull
        pocet_zakaznikov_pull++;
      }
      else if (vyber_cvicenia > 0.23 && vyber_cvicenia < 0.46) {
        Enter(PUSH_STROJE, 1); // Ziskanie stroja na push
        cout << "Zakaznik " << ID << " zacina cvicit push" << endl;
        Wait(Uniform(MIN_TIME_PUSH, MAX_TIME_PUSH)); // Prebieha cvicenie
        Leave(PUSH_STROJE, 1); // Vratenie stroja na push
        pocet_zakaznikov_push++;
      }
      else if (vyber_cvicenia > 0.46 && vyber_cvicenia < 0.82) {
        Enter(LEGS_STROJE, 1); // Ziskanie stroja na legs
        cout << "Zakaznik " << ID << " zacina cvicit legs" << endl;
        Wait(Uniform(MIN_TIME_LEGS, MAX_TIME_LEGS)); // Prebieha cvicenie
        Leave(LEGS_STROJE, 1); // Vratenie stroja na legs
        pocet_zakaznikov_legs++;
      }
      else if (vyber_cvicenia > 0.82 && vyber_cvicenia < 0.88) {
        Enter(BOX_STROJE, 1); // Ziskanie stroja na box
        cout << "Zakaznik " << ID << " zacina cvicit box" << endl;
        Wait(Uniform(MIN_TIME_BOX, MAX_TIME_BOX)); // Prebieha cvicenie
        Leave(BOX_STROJE, 1); // Vratenie stroja na box
        pocet_zakaznikov_box++;
      }
      else if (vyber_cvicenia > 0.88 && vyber_cvicenia < 1) {
        Enter(KARDIO_STROJE, 1); // Ziskanie stroja na kardio
        cout << "Zakaznik " << ID << " zacina cvicit kardio" << endl;
        Wait(Uniform(MIN_TIME_KARDIO, MAX_TIME_KARDIO)); // Prebieha cvicenie
        Leave(KARDIO_STROJE, 1); // Vratenie stroja na kardio
        pocet_zakaznikov_kardio++;
      }
      Wait(Uniform(CHANGE_ROOM_AFTER_TRAINING, SHOWER_TIME_AFTER_TRAINING)); // Prezliekanie sa
      Leave(GYM_KLUCE, 1); // Vratenie kluca na gym
      cout << "Zakaznik "<< ID << " skoncil cvicenie v case :" << Time << endl;
    }
    else { // 10% zakaznikov ide na squash (2 zakaznici ako dvojica = 1 zakaznik pre squash)
      cout << "Dvojica " << ID << " ide na squash" << endl;
      pocet_zakaznikov_squash+=2;
      timeout = new Timeout(NESPOKOJNY_ZAKAZNIK_SQUASH, this, SquashKeys, ID); // vytvoření události timeout
      cout << "Pocet volnych klucov na squash: " << SQUASH_KLUCE.Free() << endl;
      Enter(SQUASH_KLUCE, 2); // Ziskanie klucov na squash
      delete timeout; // zrušení timeoutu
      Wait(Exponential(1)); // Cakanie na recepcii

      Wait(Uniform(BEFORE_CHANGE_ROOM_TIME_MIN, BEFORE_CHANGE_ROOM_TIME_MAX)); // Prezliekanie sa
      timeout = new Timeout(CAKANIE_NA_KURT, this, Court, ID); // vytvoření události timeout
      Enter(SQUASH_KURTY, 1); // Ziskanie kurtu na squash
      cout << "Dvojica " << ID << " zacina hrat squash v case :" << Time << endl;
      delete timeout; // zrušení timeoutu
      Wait(SQUASH_TIME); // Prebieha hra
      Leave(SQUASH_KURTY, 1); // Vratenie kurtu na squash
      cout << "Dvojica " << ID << " skoncila hrat squash v case :" << Time << endl;
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
      Activate(Time+Exponential (3));
    }
    else { // 14:00 - 22:00 chodi viac zakaznikov
      Activate(Time+Exponential (1));
    }
  }
};

int main() {

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
  cout << "Pocet zakaznikov na pull: " << pocet_zakaznikov_pull << endl;
  cout << "Pocet zakaznikov na push: " << pocet_zakaznikov_push << endl;
  cout << "Pocet zakaznikov na legs: " << pocet_zakaznikov_legs << endl;
  cout << "Pocet zakaznikov na box: " << pocet_zakaznikov_box << endl;
  cout << "Pocet zakaznikov na kardio: " << pocet_zakaznikov_kardio << endl;

  return 0;
}