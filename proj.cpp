#include "simlib.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DEFINICIA KONSTANT A CASOV  ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

#define MIN_TIME_PULL 45
#define MAX_TIME_PULL 120
#define MIN_TIME_PUSH 50
#define MAX_TIME_PUSH 120
#define MIN_TIME_LEGS 70
#define MAX_TIME_LEGS 120
#define MIN_TIME_BOX 30
#define MAX_TIME_BOX 60
#define MIN_TIME_KARDIO 15
#define MAX_TIME_KARDIO 60
#define SQUASH_TIME 60
#define BEFORE_CHANGE_ROOM_TIME_MIN 3
#define BEFORE_CHANGE_ROOM_TIME_MAX 10
#define CHANGE_ROOM_AFTER_TRAINING 5
#define SHOWER_TIME_AFTER_TRAINING 20
#define NESPOKOJNY_ZAKAZNIK_SQUASH 30
#define NESPOKOJNY_ZAKAZNIK_GYM 20

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

///////////////////////////////////////////////////////////////////////////////////////////////////

class Customer : public Process { 
  double Prichod;
  void Behavior() {               
    Prichod = Time; // Cas prichodu zakaznika
    pocet_zakaznikov++;

    // Zadelenie zakaznika do fronty na gym alebo squash
    if(Random() < 0.85) { // 85% zakaznikov ide na gym
      pocet_zakaznikov_gym++;
      GYM_QUE.Insert(this);
      Passivate();
    }
    else { // 15% zakaznikov ide na squash
      pocet_zakaznikov_squash++;
      SQUASH_QUE.Insert(this);
      Passivate();
    }

  }
};

class Generator : public Event {
  void Behavior() {               
    (new Customer)->Activate();   // new customer
    if(Time < 420) { // 7:00 - 14:00 chodi menej zakaznikov kvôli škole, práci a pod.
      Activate(Time+Exponential (5));
    }
    else { // 14:00 - 22:00 chodi viac zakaznikov
      Activate(Time+Exponential (1.5));
    }
  }
};

int main() {       
  //SetOutput("proj.out");

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

  return 0;
}