#include <ncurses.h>
#include <clocale>
#include <iostream>
using namespace std;
//map의 크기는 21*21

int main(){
  WINDOW *map, *scoreBoard, *mission;

  initscr();
  start_color();
  resize_term(30, 90);
  curs_set(0);

  init_pair(1, COLOR_WHITE, COLOR_BLACK); //가장 바깥 window border
  init_pair(2, COLOR_BLUE, COLOR_WHITE);  //내부의 색깔
  init_pair(3, COLOR_BLACK, COLOR_CYAN);

  attron(COLOR_PAIR(1));
  border('|', '|', '-', '-', '+', '+', '+', '+');
  attroff(COLOR_PAIR(1));
  refresh();

  map = newwin(23, 23, 2, 2);
  wbkgd(map, COLOR_PAIR(2));
  wattron(map, COLOR_PAIR(3));
  wborder(map, '*', '*', '*', '*', '+', '+', '+', '+');
  wattroff(map, COLOR_PAIR(3));
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


  getch();
  delwin(map);
  delwin(scoreBoard);
  delwin(mission);
  endwin();

  return 0;
}
