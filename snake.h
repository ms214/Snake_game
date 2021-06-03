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

#define WALL_WALL 0 //side wall - side wall
#define WALL_INBOX 1 //side wall - in box
#define WALL_WALL2 2 //side wall - in box wall

typedef struct{
  int row;
  int col;
}pt; //pastTail

struct Gate_Data{
  int row1, col1, row2, col2; // gate의 위치
  int p1, p2; // gate생성 전 맵데이터의 값
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
  int gate_type;
  Gate_Data gate_pos{0,0,0,0,2,2};

public:
  Snake();
  void setMap(int map[][23]);
  void setdir(int a);
  void move();
  void items();
  void gate();
  void inGate();
  bool die();
  void wallwall();
  void wallbox();
  void wallwall2();

  int getMapData(int i, int j);
};


#endif
