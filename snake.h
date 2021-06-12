#ifndef _SNAKE_H_
#define _SNAKE_H_
#include <iostream>
#include <queue>
#include <list>
#include <cstdlib>
#include <ctime>
using namespace std;

#include "gate.h"

#define UP 2
#define DOWN 3
#define LEFT 5
#define RIGHT 7

typedef struct{
  int row;
  int col;
}pt; //pastTail

struct Scores{
  int B, Growth, Poison, G;
  int max_length;
};

class Snake{
  int posHead[2] = {11, 11};
  int dirHead = LEFT;
  int ntail;  //tail nums
  int mapData[23][23];
  queue<pt> pastTail; //past Tail positions
  bool isDie = false;
  int grow = 0, gcol = 0, prow = 0, pcol = 0;//growthitem, poisonitem's position
  int cnt_growth = 0, cnt_poison = 0;
  Gate gate;
  bool now_gating = false;
  int cnt_for_gating = 0;
  Scores score{0,0,0,0};

public:
  Snake();
  void setMap(int map[][23]);
  void setdir(int a);
  void move();
  void items();
  void fgate();
  bool die();

  int getMapData(int i, int j);
  bool isGate(){return now_gating;}
  void gating();
  Scores getscore() {return score;}
};


#endif
