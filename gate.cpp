#include "gate.h"
#include <cstdlib>
#include <ctime>

Gate::Gate(int row1, int col1, int row2, int col2, int p1, int p2):row1(row1), col1(col1), row2(row2), col2(col2), p1(p1), p2(p2){
  srand(time(NULL));
}

void Gate::inGate(int (*pos), int &dirHead, int (*map)[23]){
  //pos[0], pos[1] == headPos
  int dirs[4] = {UP, RIGHT, DOWN, LEFT};
  if(pos[0] == row1 && pos[1] == col1){
    //gate2의 상하좌우 중 비어 있는 공간
    bool g2_UP = map[row2-1][col2] == 0 && row2-1 >= 0;
    bool g2_DOWN = map[row2+1][col2] == 0 && row2+1 < 23;
    bool g2_RIGHT = map[row2][col2+1] == 0 && col2+1 < 23;
    bool g2_LEFT = map[row2][col2-1] == 0 && col2 -1 >= 0;

    int muls = 1; // 움직일 수 있는 방향

    if(g2_UP) muls *= 2;
    if(g2_DOWN) muls *= 3;
    if(g2_LEFT) muls*= 5;
    if(g2_RIGHT) muls*= 7;

      switch(muls){
        case 2: case 3: case 5: case 7: // 한 방향
          if(dirHead != muls) dirHead = muls;
          pos[0] = row2; pos[1] = col2;
          //outGate(pos[0], pos[1], map, muls);
        break;

        case 210: // 네 방향
          pos[0] = row2; pos[1] = col2;
        break;

        default: // 세 방향
          int i = 0;
          int pHead = dirHead;
          while (dirHead != dirs[i]) i++;
          if(muls % dirHead == 0) dirHead = dirHead;
          else if(muls % dirs[(i+1) % 4] == 0) dirHead = dirs[(i+1) % 4];
          else if(muls % dirs[(i+3) % 4] == 0) dirHead = dirs[(i+3) % 4];
          else dirHead = dirs[(i+2) % 4];

          pos[0] = row2; pos[1] = col2;
        break;

      }
  }else{
    //head가 2번 게이트에 도착
    bool g2_UP = map[row1-1][col1] == 0 && row1-1 >= 0;
    bool g2_DOWN = map[row1+1][col1] == 0 && row1+1 < 23;
    bool g2_RIGHT = map[row1][col1+1] == 0 && col1+1 < 23;
    bool g2_LEFT = map[row1][col1-1] == 0 && col1 -1 >= 0;

    int muls = 1; // 움직일 수 있는 방향

    if(g2_UP) muls *= 2;
    if(g2_DOWN) muls *= 3;
    if(g2_LEFT) muls*= 5;
    if(g2_RIGHT) muls*= 7;

      switch(muls){
        case 2: case 3: case 5: case 7: // 한 방향
          if(dirHead != muls) dirHead = muls;
          pos[0] = row1; pos[1] = col1;
          //outGate(pos[0], pos[1], map, muls);
        break;

        case 210: // 네 방향
          pos[0] = row1; pos[1] = col1;
        break;

        default: // 세 방향
          int i = 0;
          while (dirHead != dirs[i]) i++;
          if(muls % dirHead == 0) dirHead = dirHead;
          else if(muls % dirs[(i+1) % 4] == 0) dirHead = dirs[(i+1) % 4];
          else if(muls % dirs[(i+3) % 4] == 0) dirHead = dirs[(i+3) % 4];
          else dirHead = dirs[(i+2) % 4];

          pos[0] = row1; pos[1] = col1;
        break;

      }
  }
}

void Gate::setGate(int (*map)[23]){
  int row1 = rand() % 23, col1 = rand() % 23, row2 = rand() % 23, col2 = rand() % 23;
  while(map[row1][col1] != 1 || map[row1][col1] == 2){
    row1 = rand() % 23;
    col1 = rand() % 23;
  }
  while(map[row2][col2] != 1 || row1 == row2 && col1 == col2){
    row2 = rand() % 23;
    col2 = rand() % 23;
  }
  map[this->row1][this->col1] = p1;
  map[this->row2][this->col2] = p2;

  p1 = map[row1][col1];
  p2 = map[row2][col2];
  this->row1 = row1;
  this->col1 = col1;
  this->row2 = row2;
  this->col2 = col2;

  map[row1][col1] = 7;
  map[row2][col2] = 7;
}
