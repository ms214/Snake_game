#include "snake.h"
#include "gate.h"
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
  if(mapData[posHead[0]][posHead[1]] == 5) { //when growth
    ntail ++;
    pastTail.push({trow, tcol});
    cnt_growth ++;
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
    cnt_poison ++;
    int tmprow = pastTail.front().row;
    int tmpcol = pastTail.front().col;
    pastTail.pop();
    mapData[tmprow][tmpcol] = 0;
  }
  mapData[posHead[0]][posHead[1]] = 3;

  if(ntail < 2) isDie = true;
}

int Snake::getMapData(int i, int j){
  return mapData[i][j];
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

void Snake::wallwall(){
  gate_pos.row1 = rand() % 2;
  gate_pos.col1 = rand() % 2;
  gate_pos.row2 = rand() % 2;
  gate_pos.col2 = rand() % 2;

  if(gate_pos.row1 == 1) gate_pos.row1 = 22;
  if(gate_pos.col1 == 1) gate_pos.col1 = 22;
  if(gate_pos.row2 == 1) gate_pos.row2 = 22;
  if(gate_pos.col2 == 1) gate_pos.col2 = 22;

  if(gate_pos.row1 == 0 || gate_pos.row1 == 22){
    gate_pos.col1 = rand() % 23;
    while(gate_pos.row1 == gate_pos.col1) gate_pos.col1 = rand() % 23;
  }
  if(gate_pos.col1 == 0 || gate_pos.col1 == 22){
    gate_pos.row1 = rand() % 23;
    while(gate_pos.row1 == gate_pos.col1) gate_pos.row1 = rand() % 23;
  }

  if(gate_pos.row2 == 0 || gate_pos.row2 == 22){
    gate_pos.col2 = rand() % 23;
    while(gate_pos.row2 == gate_pos.col2) gate_pos.col2 = rand() % 23;
  }
  if(gate_pos.col2 == 0 || gate_pos.col2 == 22){
    gate_pos.row2 = rand() % 23;
    while(gate_pos.row2 == gate_pos.col2) gate_pos.row2 = rand() % 23;
  }
}

void Snake::wallbox(){
  gate_pos.row1 = rand() % 2;
  gate_pos.col1 = rand() % 2;
  gate_pos.row2 = 2+rand() % 19;
  gate_pos.col2 = 2+rand() % 19;

  if(gate_pos.row1 == 1) gate_pos.row1 = 22;
  if(gate_pos.col1 == 1) gate_pos.col1 = 22;

  if(gate_pos.row1 == 0 || gate_pos.row1 == 22){
    gate_pos.col1 = rand() % 23;
    while(gate_pos.row1 == gate_pos.col1) gate_pos.col1 = rand() % 23;
  }
  if(gate_pos.col1 == 0 || gate_pos.col1 == 22){
    gate_pos.row1 = rand() % 23;
    while(gate_pos.row1 == gate_pos.col1) gate_pos.row1 = rand() % 23;
  }

  while(mapData[gate_pos.row2][gate_pos.col2] != 0){
    gate_pos.row2 = 2+rand() % 19;
    gate_pos.col2 = 2+rand() % 19;
  }
}

void Snake::wallwall2(){
  
}

void Snake::gate(){
  gate_type = 1;
  mapData[gate_pos.row1][gate_pos.col1] = gate_pos.p1;
  mapData[gate_pos.row2][gate_pos.col2] = gate_pos.p2;
  switch(gate_type){
    case 0:
      wallwall();
    break;

    case 1:
      wallbox();
    break;

    case 2:
      wallwall2();
    break;
  }

  gate_pos.p1 = mapData[gate_pos.row1][gate_pos.col1];
  gate_pos.p2 = mapData[gate_pos.row2][gate_pos.col2];
  mapData[gate_pos.row1][gate_pos.col1] = 7;
  mapData[gate_pos.row2][gate_pos.col2] = 7;
}

bool Snake::die(){
  return isDie;
}
