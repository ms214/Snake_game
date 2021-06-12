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
  srand(time(NULL));
  score.max_length = ntail;
  score.B = score.max_length;
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
  if(mapData[posHead[0]][posHead[1]] != 7){
    pastTail.pop();
    pastTail.push({posHead[0], posHead[1]});
    mapData[posHead[0]][posHead[1]] = 4;
    mapData[trow][tcol] = 0;
  }
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
  if(mapData[posHead[0]][posHead[1]] == 5) { //when growth
    ntail ++;
    pastTail.push({trow, tcol});
    score.Growth ++;
    for(int i = 0; i<ntail-1; i++){ //queue 재조정
      int tmprow = pastTail.front().row;
      int tmpcol = pastTail.front().col;
      pastTail.pop();
      pastTail.push({tmprow, tmpcol});
    }
    mapData[trow][tcol] = 4; //꼬리 마지막 위치를 다시 꼬리로
  }
  if(mapData[posHead[0]][posHead[1]] == 6){ // when poison
    ntail --;
    score.Poison ++;
    int tmprow = pastTail.front().row;
    int tmpcol = pastTail.front().col;
    pastTail.pop();
    mapData[tmprow][tmpcol] = 0;
  }
  if(mapData[posHead[0]][posHead[1]] == 7){
    now_gating = true;
    score.G ++;
    gate.inGate(posHead, dirHead, mapData);
    move();
  }
  mapData[posHead[0]][posHead[1]] = 3;
  if(ntail < 2) isDie = true;
  if(score.max_length < ntail) score.max_length = ntail;
  score.B = ntail;
}

int Snake::getMapData(int i, int j){
  return mapData[i][j];
}

void Snake::gating(){
  if(now_gating)
    cnt_for_gating ++;
  if(cnt_for_gating > ntail+1){
    cnt_for_gating = 0;
    now_gating = false;
  }
}

void Snake::items(){
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

void Snake::fgate(){
  gate.setGate(mapData);
}

bool Snake::die(){
  return isDie;
}
