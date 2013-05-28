/*********************** Solver Object *****************************
 * The Solver Object has methods which solve are used
 * to find the shortest path from initial state of
 * the board to the goal state
 */


#include "Board.h"
#include <queue>
#include <list>
#include <map>
#include <QDebug>
#include <fstream>

#ifndef SOLVER_H_
#define SOLVER_H_

class Solver {
  private:
    Board initstate;
    char algo;          //signifies which algorithm to use.
    char heuristic;     //signifies which heuristic to use.
    bool a_star_algorithm();     // a-star algorithm :Details in source file.
    bool ida_star_algorithm();   //ida-star algorithm :Details in source file.
    queue <Board> solution_steps;  //final path from initial to goal state.

    pair < queue <Board> , int > depth_limited_search(int start_cost, queue <Board> path_so_far, int cost_limit);
    queue<Board> generateMoves ( Board b, char heuristic ,multimap<int,Board>& moves)const;

  public:
    int N;
    Solver();
    void operator()(int **);      //Stores the Solverinput Board in Solver

    bool isSolvable() const;      // is the initial board solvable?
    void setAlgo(char c);
    void setHeuristic(char c);
    pair<bool,queue<Board> > solve(); //the function which solves and returns true as first if solution is found.
};

#endif
