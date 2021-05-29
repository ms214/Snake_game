#include "snake.h"
#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
using namespace std;

#define RIGHTAR 77
#define LEFTAR 75
#define UPAR 80
#define DOWNAR 72

Snake::Snake(): ntail(2){
  for(int i = ntail; i>0; i--){
    pastTail.push({posHead[0], i+posHead[0]});
  }
}

void Snake::setMap(int map[][23]){
  for(int i = 0; i<23; i++)
    for(int j = 0; j<23; j++)
      this->mapData[i][j] = map[i][j];

  mapData[posHead[0]][posHead[1]] = 3;
  for(int i = 0; i<ntail; i++){
      int trow = pastTail.front().row;
      int tcol = pastTail.front().col;
      pastTail.pop();
      mapData[trow][tcol] = 4;
      pastTail.push({trow, tcol});
  }
}

void Snake::setdir(int a){
  keypad(stdscr, TRUE);
  int p_dirHead = dirHead;
  if(a == KEY_UP)
    dirHead = UP;
  else if(a ==KEY_RIGHT)
    dirHead = RIGHT;
  else if(a == KEY_DOWN)
    dirHead = DOWN;
  else if(a == KEY_LEFT)
    dirHead = LEFT;

  switch(dirHead){
    case UP:
      if(p_dirHead == DOWN) isDie = true;
      break;
    case DOWN:
      if(p_dirHead == UP) isDie = true;
      break;
    case RIGHT:
      if(p_dirHead == LEFT) isDie = true;
      break;
    case LEFT:
      if(p_dirHead == RIGHT) isDie = true;
      break;
  }
}

void Snake::move(){
  int trow = pastTail.front().row;
  int tcol = pastTail.front().col;
  pastTail.pop();
  pastTail.push({posHead[0], posHead[1]});
  mapData[posHead[0]][posHead[1]] = 4;
  mapData[trow][tcol] = 0;
  switch(dirHead){
    case UP:
      posHead[0] --;
      break;

    case DOWN:
      posHead[0] ++;
      break;

    case RIGHT:
      posHead[1] ++;
      break;

    case LEFT:
      posHead[1] --;
      break;
  }
  if(mapData[posHead[0]][posHead[1]] == 1) isDie = true;
  mapData[posHead[0]][posHead[1]] = 3;
}

int Snake::getMapData(int i, int j){
  return mapData[i][j];
}

void Snake::items(){
  srand(time(NULL));

  mapData[grow][gcol] = (mapData[grow][gcol] == 5)? 0 : mapData[grow][gcol]; //previous growthitem delete
  mapData[prow][pcol] = (mapData[prow][pcol] == 6)? 0 : mapData[prow][pcol]; //previous poisonitem delete
  //growthItem
  grow = 1+rand() % 21;
  gcol = 1+rand() % 21;
  while(mapData[grow][gcol] != 0){
    grow = 1+rand() % 21;
    gcol = 1+rand() % 21;
  }
  mapData[grow][gcol] = 5;
  //poisonItem
  prow = 1+rand() % 21;
  pcol = 1+rand() % 21;
  while(mapData[prow][pcol] != 0){
    prow = 1+rand() % 21;
    pcol = 1+rand() % 21;
  }
  mapData[prow][pcol] = 6;
}

bool Snake::die(){
  return isDie;
}
