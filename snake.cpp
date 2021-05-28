#include "snake.h"
#include <iostream>
#include <ncurses.h>
using namespace std;

#define RIGHTAR 77
#define LEFTAR 75
#define UPAR 80
#define DOWNAR 72

Snake::Snake(): ntail(3){
  for(int i = 3; i>0; i--){
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
  if(a == KEY_UP)
    dirHead = UP;
  else if(a ==KEY_RIGHT)
    dirHead = RIGHT;
  else if(a == KEY_DOWN)
    dirHead = DOWN;
  else if(a == KEY_LEFT)
    dirHead = LEFT;
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
  mapData[posHead[0]][posHead[1]] = 3;
}

int Snake::getMapData(int i, int j){
  return mapData[i][j];
}

bool Snake::die(){
  return true;
}
