#include <ncurses.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
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
void createMap();
void refreshMap(int i, int j, int value);
void refreshScoreBoard(Snake &sn);
bool refreshMissionBoard(Snake &sn);
void timer();
void fail();
void success();
void endPro();

struct toMission{
  int max_length, Growth, Poison, G;
} mis;

WINDOW *map, *scoreBoard, *mission;
int mapData[23][23] = {0, };
bool isDie = false;
bool isMission = false;
int t_time = 0;
int level = 0; // 0단계 ~ 3단계까지 진행

int cntitem = 20;
int cntgate = 0;
//mission mis;

int main(){
  setInit();
  createMap();
  Snake sn;
  sn.setMap(mapData);
  nodelay(stdscr, TRUE);
  thread t_timer(timer);
  while(true){// fail혹은 마지막 단계에서 success 일 때 break
    this_thread::sleep_for(milliseconds(500)); //0.5초 동안 sleep
    int input = 0;
    if((input = getch()) != ERR){
      sn.setdir(input);
    }
    // item, gate 새로고침을 위한 변수 증가
    cntitem++;
    cntgate++;
    sn.move(); //snake 움직임
    sn.gating(); //게이트 진입 중이거나 진입중이 아닐 때 처리 함수
    isDie = sn.die(); // snake가 죽었는지 여부 확인
    if(cntitem >= 20){sn.items(); cntitem = 0;}
    if(cntgate >= 30&& !sn.isGate()){sn.fgate(); cntgate = 0;}
    for(int i = 0; i<23; i++)
      for(int j = 0; j<23; j++)
        refreshMap(i, j, sn.getMapData(i,j));
    wrefresh(map);

    refreshScoreBoard(sn);
    isMission = refreshMissionBoard(sn);

    if(isDie) break;
    if(isMission) {
      success();  //successBox 출력
      level++;    //다음 단계로
      createMap();//다음단계 맵
      isMission = false;
      sn = {}; // 새로운 snake 객체 생성
      sn.setMap(mapData);
      setInit();
      t_time = 0;
      t_timer.join();
      t_timer = thread(timer);
    }
    if(level >=4){
      isMission = true;
      success();
      break;
    }
  }
  if(isDie) {nodelay(stdscr, FALSE); fail();}

  t_timer.join();
  endPro();
  return 0;
}

void setInit(){
  //ncurses 초기 설
  initscr();
  start_color();
  resize_term(30, 90);

  keypad(stdscr, TRUE);
  curs_set(0);
  noecho();
  cbreak();

  //COLOR_PAIR 설
  init_pair(9, COLOR_BLUE, COLOR_WHITE); // scoreBoard and MissionBoard
  init_pair(1, COLOR_WHITE, COLOR_BLACK); //가장 바깥 window border
  init_pair(2, COLOR_WHITE, COLOR_WHITE);  //내부의 색깔
  init_pair(3, COLOR_BLACK, COLOR_CYAN);
  init_pair(4, COLOR_MAGENTA, COLOR_MAGENTA); //head color
  init_pair(5, COLOR_CYAN, COLOR_CYAN); // tail color
  init_pair(6, COLOR_GREEN, COLOR_GREEN); // growthItem
  init_pair(7, COLOR_RED, COLOR_RED); // poisonItem
  init_pair(8, COLOR_YELLOW, COLOR_YELLOW); //Gate

  // 메인 윈도우 테두리 표시 설정
  attron(COLOR_PAIR(1));
  border('|', '|', '-', '-', '+', '+', '+', '+');
  attroff(COLOR_PAIR(1));
  refresh();

  // 메인 윈도우 안에 sub-window 생성 및 초기 설정
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
  wbkgd(scoreBoard, COLOR_PAIR(9));
  wattron(scoreBoard, COLOR_PAIR(9));
  mvwprintw(scoreBoard, 1, 1, "Score Board");
  wrefresh(scoreBoard);

  mission = newwin(10, 15, 15, 40);
  wbkgd(mission, COLOR_PAIR(9));
  wattron(mission, COLOR_PAIR(9));
  mvwprintw(mission, 1, 1, "Mission");
  wrefresh(mission);

  srand(time(NULL));
  //사용될 변수 초기화
  cntitem = 20;
  cntgate = 0;
  mis.max_length = 4 + rand() % 7;
  mis.Growth = 1 + rand() % 10;
  mis.Poison = 1 + rand() % 10;
  mis.G = 1 + rand() % 10;
}

void createMap(){
  //초기 mapData의 모든 값 0으로 초기화
  for(int i = 0; i<23; i++)
    for(int j = 0; j<23; j++)
      mapData[i][j] = 0;

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
  //단계별 맵 안에 벽 생성
  switch(level){
    case 1:
      for(int i = 0; i<6; i++){
        mapData[i+3][11] = 1;
        mapData[i+14][11] = 1;
      }
    break;

    case 2:
      for(int i = 0; i<6; i++){
        mapData[11][i+3] = 1;
        mapData[11][i+14] = 1;
      }
    break;

    case 3:
      for(int i = 0; i<6; i++){
        mapData[i+3][11] = 1;
        mapData[i+14][11] = 1;
        mapData[11][i+3] = 1;
        mapData[11][i+14] = 1;
      }
    break;
  }

}

void refreshMap(int i, int j, int value){
  // mapData[i][j]의 값에 따라 color, 값 출력
  // (text색상과 배경색상이 동일해 실제로 텍스트는 보이지 않음)
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

void refreshScoreBoard(Snake &sn){
  //scroeboard 변경된 값 재출력
  wattron(scoreBoard, COLOR_PAIR(9));
  mvwprintw(scoreBoard, 3, 1, "             "); //B: %d / %d 출력 전 초기화
  mvwprintw(scoreBoard, 3, 1, "B: %d / %d", sn.getscore().B+1, sn.getscore().max_length+1);
  mvwprintw(scoreBoard, 4, 1, "+: %d", sn.getscore().Growth);
  mvwprintw(scoreBoard, 5, 1, "-: %d", sn.getscore().Poison);
  mvwprintw(scoreBoard, 6, 1, "G: %d", sn.getscore().G);
  mvwprintw(scoreBoard, 7, 1, "time: %d", t_time);
  wattroff(scoreBoard, COLOR_PAIR(9));
  wrefresh(scoreBoard);
}

bool refreshMissionBoard(Snake &sn){
  //mission 달성 여부를 bool 변수로 저장
  bool b_b = sn.getscore().max_length+1>=mis.max_length;
  bool b_grow = sn.getscore().Growth>=mis.Growth;
  bool b_poison = sn.getscore().Poison>=mis.Poison;
  bool b_g = sn.getscore().G>=mis.G;

  //mission 달성여부와 mission 내용 출력
  wattron(mission, COLOR_PAIR(9));
  mvwprintw(mission, 3, 1, "B: %d", mis.max_length);
  mvwprintw(mission, 3, 8, b_b? "(v)" : "( )");

  mvwprintw(mission, 4, 1, "+: %d", mis.Growth);
  mvwprintw(mission, 4, 8, b_grow? "(v)" : "( )");

  mvwprintw(mission, 5, 1, "-: %d", mis.Poison);
  mvwprintw(mission, 5, 8, b_poison? "(v)" : "( )");

  mvwprintw(mission, 6, 1, "G: %d", mis.G);
  mvwprintw(mission, 6, 8, b_g ? "(v)" : "( )");

  wattroff(mission, COLOR_PAIR(9));
  wrefresh(mission);

  return (b_b && b_grow && b_poison && b_g);
}

void timer(){
  while(!isDie && !isMission){
    this_thread::sleep_for(milliseconds(1000));
    t_time ++;
  }
}

void fail(){
  WINDOW *failBox = newwin(3, 9, 20, 9);
  wbkgd(failBox, COLOR_PAIR(3));
  mvwprintw(failBox, 1, 1, "Failed");
  wrefresh(failBox);
}

void success(){
  WINDOW *successBox = newwin(3, 9, 20, 9);
  wbkgd(successBox, COLOR_PAIR(3));
  mvwprintw(successBox, 1, 1, "Success");
  wrefresh(successBox);
  nodelay(stdscr, FALSE);
  getch();
  nodelay(stdscr, TRUE);
}

void endPro(){
  getch(); // 프로그램 종료 전 키 입력받기 (없으면 failbox, successbox 등이 출력 안됨)
  delwin(map);
  delwin(scoreBoard);
  delwin(mission);
  endwin();
}
