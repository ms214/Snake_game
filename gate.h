#ifndef _GATE_H_
#define _GATE_H_
#include <iostream>
using namespace std;

#define RIGHT 7
#define LEFT 5
#define UP 2
#define DOWN 3

class Gate{
  int row1, col1, row2, col2; //Gate poses
  int p1, p2; //mapData before Gate

public:
  Gate(int row1 = 0, int col1 = 0, int row2 = 0, int col2 = 0, int p1 = 2, int p2 = 2);

  void inGate(int (*pos), int &dirHead, int (*map)[23]);
  void setGate(int (*map)[23]);

};

#endif
