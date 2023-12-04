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
#define NESPOKOJNY_ZAKAZNIK_SQUASH 30
#define NESPOKOJNY_ZAKAZNIK_GYM 20

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

int customer_id = 0;
int nespokojny_zakaznik = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DEFINICIA RADOV A ZARIADENI  //////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

Store GYM_KLUCE ("Kluce na gym", kluce_gym);
Store SQUASH_KLUCE ("Kluce na squash", kluce_squash);

Store PULL_STROJE ("Stroje na pull", stroje_pull);
Store PUSH_STROJE ("Stroje na push", stroje_push);
Store LEGS_STROJE ("Stroje na legs", stroje_legs);
Store BOX_STROJE ("Stroje na box", box_room);
Store KARDIO_STROJE ("Stroje na kardio", kardio_stroje);
Store SQUASH_KURTY ("Kurty na squash", squash_kurt);

Queue GYM_QUE("Fronta na gym");
Queue SQUASH_QUE("Fronta na squash");


//ENTER a LEAVE

///////////////////////////////////////////////////////////////////////////////////////////////////

class Timeout : public Event {
  Process *ptr;        // který proces
  typeOfWait type;    // typ čekání
  public:
    Timeout(double t, Process *p, typeOfWait type): ptr(p), type(type) {
      Activate(Time+t);  // kdy vyprší timeout
    }
    void Behavior() {
      ptr->Out();        // vyjmout z fronty
      delete ptr;        // likvidace
      cout << type << endl;
      if (type == GymKeys) {
        cout << "Zakaznik odisiel nespokojny lebo kluce gym" << endl;
      }
      else if (type == SquashKeys) {
        cout << "Zakaznik odisiel nespokojny lebo kluce squash" << endl;
      }
      nespokojny_zakaznik++;          // počitadlo
      Cancel();          // konec události (SIMLIB BUG)
    }
};

class Customer : public Process { 
  double Prichod;
  int ID;
  public : Customer(int ID) : Process() { this->ID = ID; }
  void Behavior() {               
    Prichod = Time; // Cas prichodu zakaznika
    pocet_zakaznikov++;
    Event *timeout;

    cout << "Prichod zakaznika: "<< ID << " v case : " << Prichod << endl;
    // Zadelenie zakaznika do fronty na gym alebo squash
    if(Random() < 0.9) { // 90% zakaznikov ide na gym
      pocet_zakaznikov_gym++;
      cout << "Zakaznik " << ID << " ide do gymu" << endl;
      timeout = new Timeout(NESPOKOJNY_ZAKAZNIK_GYM, this, GymKeys); // vytvoření události timeout
    }
    else { // 10% zakaznikov ide na squash (2 zakaznici ako dvojica = 1 zakaznik pre squash)
      cout << "Dvojica " << ID << " ide na squash" << endl;
      pocet_zakaznikov_squash+=2;
      timeout = new Timeout(NESPOKOJNY_ZAKAZNIK_SQUASH, this, SquashKeys); // vytvoření události timeout
    }

      cout << "Pocet volnych klucov na gym: " << GYM_KLUCE.Free() << endl;
      Enter(GYM_KLUCE, 1); // Ziskanie kluca na gym
      delete timeout; // zrušení timeoutu
      Wait(Exponential(1)); // Cakanie na recepcii

      Wait(Uniform(BEFORE_CHANGE_ROOM_TIME_MIN, BEFORE_CHANGE_ROOM_TIME_MAX)); // Prezliekanie sa
      cout << "Zakaznik " << ID << " zacina cvicit" << endl;
      Wait(Uniform(MIN_TIME_LEGS, MAX_TIME_LEGS)); // Cakanie na cvicenie
      Wait(Uniform(CHANGE_ROOM_AFTER_TRAINING, SHOWER_TIME_AFTER_TRAINING)); // Prezliekanie sa
      Leave(GYM_KLUCE, 1); // Vratenie kluca na gym
      cout << "Zakaznik "<< ID << " skoncil cvicenie v case :" << Time << endl;
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

  return 0;
}