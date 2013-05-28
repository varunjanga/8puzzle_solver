#include <iostream>
#include <cmath>
#include <map>
#include <queue>
using namespace std;
#include <stdio.h>

#ifndef BOARD_H_
#define BOARD_H_

/*------------------BOARD CLASS-----------------------------------------------
** The board class has important data members like
**   int N  ---- this is the board size like n may be 3 or 4 etc.
**   char parent ---- is used to prevent backtracking of the path.
**   queue<char> direction ---- this queue stores the directions of the path from the initstate to board.
**   int depth  ---- this variable tells us the depth from the initial state. This is equal to the above queue size.
**   int x_hole,y_hole  ---- these are the positions of the blank space in the board.
**   Many heuristic functions are present like nilsson's score , hamming , manhattan etc.
**   Few operator are overloaded for convinience.
**   Useful helper functions are declared.
*/
class Board {
  private:
    int **board;
    int distance(int x,int y,int val);

  public:
    int N;
    char parent;
    int x_hole,y_hole;
    int depth;
    Board();
    Board(char par);
    Board(int** tiles,char dir,int n);        // construct a board from an N-by-N array of tiles
    int hamming();               // return number of blocks out of place
    int manhattan();             // return sum of Manhattan distances between blocks and goal
    int blank();                 //manhattan distance of every tile from the space.for 3x3 only
    int euclidean();             //gives the sum of euclidean distances of current to goal state positions.
    double euclidean_distance(int x,int y,int val);
    int nilsson_score();      //gives the nilsson sequence score. for 3x3 only.
    int nilsson_check(int x1,int y1,int x2,int y2);
    int nilsson_manhattan();
    int heuristic_val(char);
    bool equals(Board y);    // does this board position equal y
    void print() const;
    void operator= (Board b);
    bool operator==(Board b)const;
    bool operator<(Board b)const;
    int getBoardVal(int i,int j) const;
    queue<char> neighbours();
    void makeMove(char);
    queue<char> direction;

};

#endif
