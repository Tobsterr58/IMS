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
int squash_kurt = 5

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DEFINICIA RADOV A ZARIADENI  //////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

Facility PUSH ("PUSH", stroje_push);
Facility PULL ("PULL", stroje_pull);
Facility LEGS ("LEGS", stroje_legs);
Facility BOX ("BOX", box_room);
Facility KARDIO ("KARDIO", kardio_stroje);
Facility SQUASH ("SQUASH", squash_kurt);

Queue GYM_QUE("Fronta na gym");
Queue SQUASH_QUE("Fronta na squash");

class Customer : public Process {
  void Behavior() {
    // TODO
  }
};

int main() {       

  
  return 0;
}