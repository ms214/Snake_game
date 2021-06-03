#include <ncurses.h>
#include <iostream>
#include <cstdlib>
#include <thread>
using namespace std;
using namespace std::chrono;
//map의 크기는 21*21


/*wall : 1
  corner : 2
  Snake Head : 3
  Snake Tail : 4
  Growthitem : 5
  Poison Item : 6
  Gate : 7*/

#include "snake.h"

void setInit();
void refreshMap(int i, int j, int value);
void fail();
void endPro();

WINDOW *map, *scoreBoard, *mission, *failBox;
int mapData[23][23] = {0, };
bool isDie = false;
int cntitem = 10;
int cntgate = 0;

int main(){
  setInit();
  Snake sn{};
  sn.setMap(mapData);
  nodelay(stdscr, TRUE);
  while(true){
    this_thread::sleep_for(milliseconds(500));
    int input = 0;
    if((input = getch()) != ERR){
      sn.setdir(input);
    }
    cntitem++;
    cntgate++;
    sn.move();
    isDie = sn.die();
    if(isDie) break;
    if(cntitem >= 10){sn.items(); cntitem = 0;}
    if(cntgate >= 15){
      sn.gate(); cntgate = 0;
    }
    for(int i = 0; i<23; i++)
      for(int j = 0; j<23; j++)
        refreshMap(i, j, sn.getMapData(i,j));
    wrefresh(map);
  }
  if(isDie) {fail(); nodelay(stdscr, FALSE);}

  endPro();
  return 0;
}

void setInit(){
  //테두리 데이터
  mapData[0][0] = 2;
  mapData[22][0] = 2;
  mapData[0][22] = 2;
  mapData[22][22] = 2;
  for(int i = 1; i<22; i++){
    mapData[0][i] = 1; // 상단
    mapData[22][i] = 1; //하단
    mapData[i][0] = 1; // 좌측
    mapData[i][22] = 1; //우측
  }

  initscr();
  start_color();
  resize_term(30, 90);

  keypad(stdscr, TRUE);
  curs_set(0);
  noecho();
  cbreak();

  init_pair(1, COLOR_WHITE, COLOR_BLACK); //가장 바깥 window border
  init_pair(2, COLOR_WHITE, COLOR_WHITE);  //내부의 색깔
  init_pair(3, COLOR_BLACK, COLOR_CYAN);
  init_pair(4, COLOR_MAGENTA, COLOR_MAGENTA); //head color
  init_pair(5, COLOR_CYAN, COLOR_CYAN); // tail color
  init_pair(6, COLOR_GREEN, COLOR_GREEN); // growthItem
  init_pair(7, COLOR_RED, COLOR_RED); // poisonItem
  init_pair(8, COLOR_YELLOW, COLOR_YELLOW); //Gate

  attron(COLOR_PAIR(1));
  border('|', '|', '-', '-', '+', '+', '+', '+');
  attroff(COLOR_PAIR(1));
  refresh();

  map = newwin(23, 23, 2, 2);
  wbkgd(map, COLOR_PAIR(2));
  wattron(map, COLOR_PAIR(3));
  wborder(map, '*', '*', '*', '*', '+', '+', '+', '+');
  wattroff(map, COLOR_PAIR(3));
  //내부 맵 표시
  wattron(map, COLOR_PAIR(2));
  for(int i = 1; i<22; i++){
    for(int j = 1; j<22; j++){
        mvwprintw(map, i, j, "%d",mapData[i][j]);
    }
  }
  wattroff(map, COLOR_PAIR(2));
  wrefresh(map);

  scoreBoard = newwin(10, 15, 2, 40);
  wbkgd(scoreBoard, COLOR_PAIR(2));
  wattron(scoreBoard, COLOR_PAIR(2));
  mvwprintw(scoreBoard, 1, 1, "Score Board");
  //내용 입력
  mvwprintw(scoreBoard, 3, 1, "Content"); // "Content"대신 내용 출력
  wattroff(scoreBoard, COLOR_PAIR(2));

  wrefresh(scoreBoard);

  mission = newwin(10, 15, 15, 40);
  wbkgd(mission, COLOR_PAIR(2));
  wattron(mission, COLOR_PAIR(2));
  mvwprintw(mission, 1, 1, "Mission");
  //내용
  mvwprintw(mission, 3, 1, "Content"); // "Content"대신 내용 출력
  wattroff(mission, COLOR_PAIR(2));

  wrefresh(mission);
}

void refreshMap(int i, int j, int value){
    if(value == 0){
      wattron(map, COLOR_PAIR(2));
      mvwprintw(map, i, j, "%d",value);
      wattroff(map, COLOR_PAIR(2));
    }else if(value == 1){
      wattron(map, COLOR_PAIR(3));
      mvwprintw(map, i, j, "*");
      wattroff(map, COLOR_PAIR(3));
    }else if(value == 2){
      wattron(map, COLOR_PAIR(3));
      mvwprintw(map, i, j, "+");
      wattroff(map, COLOR_PAIR(3));
    }else if(value == 3){ //head
      wattron(map, COLOR_PAIR(4));
      mvwprintw(map, i, j, "%d",value);
      wattroff(map, COLOR_PAIR(4));
    }else if(value == 4){ // tail
      wattron(map, COLOR_PAIR(5));
      mvwprintw(map, i, j, "%d",value);
      wattroff(map, COLOR_PAIR(5));
    }else if(value == 5){ // growthitem
      wattron(map, COLOR_PAIR(6));
      mvwprintw(map, i, j, "%d", value);
      wattroff(map, COLOR_PAIR(6));
    }else if(value == 6){ // poisonItem
      wattron(map, COLOR_PAIR(7));
      mvwprintw(map, i, j, "%d", value);
      wattroff(map, COLOR_PAIR(7));
    }else if(value == 7){
      wattron(map, COLOR_PAIR(8));
      mvwprintw(map, i, j, "%d", value);
      wattroff(map, COLOR_PAIR(8));
    }
}

void fail(){
  failBox = newwin(3, 9, 20, 9);
  wbkgd(failBox, COLOR_PAIR(3));
  mvwprintw(failBox, 1, 1, "Failed");
  wrefresh(failBox);
}

void endPro(){
  getch();
  delwin(map);
  delwin(scoreBoard);
  delwin(mission);
  endwin();
}
