#include <ncurses.h>
#include <iostream>
#include <cstdlib>
using namespace std;
//map의 크기는 21*21

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "snake.h"

void setInit();
void refreshMap(int i, int j, int value);
void endPro();

WINDOW *map, *scoreBoard, *mission;
int mapData[23][23] = {0, };

int main(){
  setInit();
  Snake sn{};
  sn.setMap(mapData);
  nodelay(stdscr, TRUE);
  while(true){
    sleep(1);
    sn.move();
    for(int i = 0; i<23; i++)
      for(int j = 0; j<23; j++)
        refreshMap(i, j, sn.getMapData(i,j));
    wrefresh(map);
    int input = 0;


    if((input = getch()) == ERR){
      continue;
    }else{
      sn.setdir(input);
    }
  }

  endPro();
  return 0;
}

void setInit(){
  //테도리 데이터
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
  init_pair(2, COLOR_BLUE, COLOR_WHITE);  //내부의 색깔
  init_pair(3, COLOR_BLACK, COLOR_CYAN);
  init_pair(4, COLOR_MAGENTA, COLOR_MAGENTA); //head color
  init_pair(5, COLOR_CYAN, COLOR_CYAN); // tail color

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
    }else if(value == 3){ //head
      wattron(map, COLOR_PAIR(4));
      mvwprintw(map, i, j, "%d",value);
      wattroff(map, COLOR_PAIR(4));
    }else if(value == 4){ // tail
      wattron(map, COLOR_PAIR(5));
      mvwprintw(map, i, j, "%d",value);
      wattroff(map, COLOR_PAIR(5));
    }
}

void endPro(){
  getch();
  delwin(map);
  delwin(scoreBoard);
  delwin(mission);
  endwin();
}
