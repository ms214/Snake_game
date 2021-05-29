#ifndef _SNAKE_H_
#define _SNAKE_H_
#include <iostream>
#include <queue>
#include <list>
#include <cstdlib>
#include <ctime>
using namespace std;

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3

typedef struct{
  int row;
  int col;
}pt; //pastTail

class Snake{
  int posHead[2] = {11, 11};
  int dirHead = LEFT;
  int ntail;  //tail nums
  int mapData[23][23];
  queue<pt> pastTail; //past Tail positions
  bool isDie = false;
  int grow = 0;
  int gcol=0;
  int prow=0;
  int pcol=0; //growthitem, poisonitem's position

public:
  Snake();
  void setMap(int map[][23]);
  void setdir(int a);
  void move();
  void items();
  bool die();

  int getMapData(int i, int j);
};


#endif
