#include "Solver.h"
#include <cstring>
#define COST_LIMIT 35

typedef pair<int,Board> mypair;
bool operator<(const mypair& e1, const mypair& e2) {

  return ((e1.first > e2.first) || ( (e1.first == e2.first) &&
                       (e1.second < e2.second)));
}

Solver::Solver(){}

void Solver::operator()(int **tiles){//Stores the input Board in Solver
    Board initial(tiles,0,N);
    initstate = initial;
    while(!solution_steps.empty()){
        solution_steps.pop();
    }
}

void Solver::setAlgo(char c)
{
    algo = c;
}

void Solver::setHeuristic(char c)
{
    heuristic = c;
}

pair<bool,queue<Board> > Solver::solve(){
    //qDebug() << "solver " << isSolvable() <<endl;

    if (algo == 'a'){
        pair<bool,queue<Board> > temp(a_star_algorithm(),solution_steps);
        return temp;
    }
    else { //if(algo == 'i'){
        pair<bool,queue<Board> > temp(ida_star_algorithm(),solution_steps);
        return temp;
    }
}

/*
    If the grid width is odd, then the number of inversions in a solvable situation is even.
    If the grid width is even, and the blank is on an even row counting from the bottom (second-last, fourth-last etc), then the number of inversions in a solvable situation is odd.
    If the grid width is even, and the blank is on an odd row counting from the bottom (last, third-last, fifth-last etc) then the number of inversions in a solvable situation is even.
*/
bool Solver::isSolvable()const{
  int inversions = 0;
  int* temp;
  temp = new int [N*N];
  for (int i = 0; i < N*N; i++){
    temp[i] = initstate.getBoardVal( i/N , i%N );
  }

  for(int i = 0; i < N - 1; i++)
    for(int j = i+1; j < N; j++)
      if(temp[i] > temp[j])
        inversions++;

  if( N%2 == 0){
    if ((N - (initstate.x_hole + 1))%2 == 0) return inversions%2 == 1 ;
    else  return inversions%2 == 0 ;
  }
  else return inversions%2 == 0 ;
}

//$$$$$$$$$$$$$$$$$$$$$    HELPER FUNCTIONS FOR A STAR ALGORITHM   $$$$$$$$$$$$$$$$$$$$$$$$//

/* --------------CHECKMOVE------------------------------------------------------
** This function when passed a board checks whether the board is already stored
** in the multimap MOVES or not. If it is stored in the multimap then it means
** that we already came across same board before so now we need not consider it now.
** This is done to avoid repeatition.
*/

bool checkMove ( Board b,char heuristic,multimap<int,Board>& moves ) {
     pair<multimap<int,Board>::iterator,multimap<int,Board>::iterator> ret;
     int score = b.depth + b.heuristic_val(heuristic) ;
     ret = moves.equal_range(score);
     multimap<int,Board>::iterator it = ret.first ;
     while (it!=ret.second){
         if(it->second == b) {
           return true;
         }
           it++;
    }
    return false;
}

/* --------------GENERATEMOVES--------------------------------------------------
** The generateMoves funtion is just a helper to the a_star_algo function.
** here in this function given a board returns the  queue which contains all
** the non repeated children to the board. Generally a board configuration has
** 2 or 3 children depending upon the empty space position.
*/

queue<Board> Solver::generateMoves ( Board b, char heuristic ,multimap<int,Board>& moves)const{
   queue<Board> neighbours;      // queue storing the children of the noard b.
    if(b.x_hole > 0){
      Board left;
      left = b;                     //left to equaled to b.
      left.makeMove('u');           //moving the position of the empty space.
      left.depth = b.depth + 1;     //depth is incremented.
      left.direction.push('u');
      if(!checkMove(left,heuristic,moves)){         //checks whether repeated or not.
         neighbours.push( left );    //if not repeated pushed into the queue.
         int score = left.depth + left.heuristic_val(heuristic) ;  //score
         moves.insert(pair<int,Board>(score,left));  //inserted into the multimap.
      }
    }
    if(b.x_hole < N-1){
      Board right;
      right = b;                    //right to equaled to b.
      right.makeMove('d');          //moving the position of the empty space.
      right.depth = b.depth + 1;    //depth is incremented.
      right.direction.push('d');
      if(!checkMove(right,heuristic,moves)){  //checks whether repeated or not.
         neighbours.push( right );    //if not repeated pushed into the queue.
         int score = right.depth + right.heuristic_val(heuristic) ;  //score
         moves.insert(pair<int,Board>(score,right));   //inserted into the multimap.
      }
    }
    if(b.y_hole > 0){
      Board up;
      up = b;                       //up to equaled to b.
      up.makeMove('l');             //moving the position of the empty space.
      up.depth = b.depth + 1;       //depth is incremented.
      up.direction.push('l');
      if(!checkMove(up,heuristic,moves)){           //checks whether repeated or not.
         neighbours.push( up);     //if not repeated pushed into the queue.
         int score = up.depth + up.heuristic_val(heuristic) ;      //score
         moves.insert(pair<int,Board>(score,up));    //inserted into the multimap.
      }
    }
    if(b.y_hole < N-1){
      Board down;
      down = b;                     //down to equaled to b.
      down.makeMove('r');           //moving the position of the empty space.
      down.depth = b.depth + 1;     //depth is incremented.
      down.direction.push('r');
      if(!checkMove(down,heuristic,moves)){         //checks whether repeated or not.
         neighbours.push( down );   //if not repeated pushed into the queue.
         int score = down.depth + down.heuristic_val(heuristic) ;      //score
         moves.insert(pair<int,Board>(score,down));  //inserted into the multimap.
      }
    }
    return neighbours;     //returns the queue.
}

/* -------------------A-STAR ALGORITHM SOLVER-----------------------------------
*/
bool Solver::a_star_algorithm(){

   multimap<int,Board> moves;             //multimap stores all the visited boards.
   priority_queue < mypair > boardQueue;  //priority queue stores boards and their scores.
   queue<Board> path;            //queue stores the path from initial state to goal state.
   bool solvable = true ;     //tells whether the problem is solvable or not.
   int startBoardScore ;         //initialboard score.
   initstate.depth = 0;          //initialboard depth.
   startBoardScore = initstate.heuristic_val(heuristic) ;
   boardQueue.push( pair<int,Board>(startBoardScore,initstate) );

   Board goalstate;  //goalState.

    while(1){
      Board b;
      b = boardQueue.top().second ;  //taking the best prefered and processing it.
      boardQueue.pop() ;    //poping the procesed board configuration.

      if(b.heuristic_val(heuristic) == 0){
         goalstate = b;
         break;
      }

      //breaking if the priority queue size exceeds 10000
      if(boardQueue.size() > 10000 ){
         solvable = false;
         break;
      }

      queue<Board> newMoves;   //newmoves is the queue returned from generateMoves.
      newMoves = generateMoves(b,heuristic,moves);

      while(!newMoves.empty()){         //till newmoves becomes empty.

         Board move;
         move = newMoves.front() ;        //taking the top of the queue.

         newMoves.pop();                  //poping the top of the queue.

         int score ;
         score = move.depth + move.heuristic_val(heuristic);

         boardQueue.push( pair<int,Board>(score,move) );  //pushing the move into the priority queue.
      }
   }

   //These are used to back track the path from the path direction stored in the
   //queue of the goal state.

   Board dummy;
   dummy = initstate;

   path.push(initstate);

   // back tracking the path.

   while(!(goalstate.direction.empty())){     //till path is empty.

       Board temp;
        temp = dummy;
        temp.makeMove(goalstate.direction.front());
        dummy = temp;

        path.push(temp);

        goalstate.direction.pop();
    }

   solution_steps = path;
   return solvable;
}


/*********************** IDA-Star Algo ********************
pseudocode:
algorithm IDA*(start):
    cost_limit = h(start)//h is the heuristic function that estimates the path from a node to the goal.

    loop:
        solution, cost_limit = depth_limited_search(0, [start], cost_limit)
        if solution != None:
            return solution
        if cost_limit == ∞:
            return None
*/

int debug_i=0;///////debug
bool Solver::ida_star_algorithm(){

  int cost_limit = initstate.heuristic_val(heuristic);
  queue <Board> solution;
  solution.push(initstate);
  pair < queue <Board> , int > pr;

  while(1){
    pr = depth_limited_search(0, solution , cost_limit);
    cost_limit = pr.second;//cost_limit updated..

    if(! pr.first.empty()) {//Solution found
      solution_steps = pr.first;
      break;
    }
    else if(cost_limit > COST_LIMIT) return false;
    else continue;//searching again with updated cost_limit
  }
  return true;
}

pair < queue <Board> , int > Solver::depth_limited_search(int start_cost, queue <Board> path_so_far, int cost_limit){

  Board node = path_so_far.back();//Last element in path_so_far
  int minimum_cost = start_cost + node.heuristic_val(heuristic);//minimum_cost required to reach this node

  if ( minimum_cost > cost_limit ) {//break as it is a bad move
    queue <Board> empty_q;
    pair < queue <Board> , int > temp_pr(empty_q,minimum_cost);
    return temp_pr;
  }
  if (node.heuristic_val(heuristic) == 0){//Found the path and returning here
    pair < queue <Board> , int > temp_pr(path_so_far,cost_limit);
    return temp_pr;
  }

  int next_cost_limit = 999999999;
  queue<char>successors_list = node.neighbours();

  while (! successors_list.empty()){
    char next_path = successors_list.front();
    successors_list.pop();
    Board newNode(next_path);
    newNode = node;
    newNode.makeMove(next_path);

    int new_start_cost = start_cost + 1;//Distance from root increased

    pair < queue <Board> , int > temp_pr;
    queue <Board> extended_path = path_so_far;
    extended_path.push(newNode);

    temp_pr = depth_limited_search(new_start_cost,extended_path,cost_limit);
    if (! temp_pr.first.empty())
      return temp_pr;
    next_cost_limit = min (next_cost_limit , temp_pr.second);
  }

  queue <Board> temp_q;
  //sent from here to update the cost_limit
  pair < queue <Board> , int > temp_pr(temp_q,next_cost_limit);
  return temp_pr;
}
