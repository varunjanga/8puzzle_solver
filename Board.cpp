#include "Board.h"

//Members are defined below
//-----------DEFAULT CONSTRUCTOR------------------------------------------------

Board::Board(){}

//--------------BOARD CONSTRUCTOR-----------------------------------------------

Board::Board(char par){
  parent = par;
}

//-------------BOARD CONSTRUCTOR------------------------------------------------
// HERE INPUTS ARE BOARD SIZE ,CONFIGURATION AND DIRECTION.

Board::Board(int** tiles,char dir,int n){
  N = n;
  parent = dir;
  board = new int*[N];
  for (int i = 0; i < N; i++){
    board[i] = new int[N];
    for (int j = 0; j < N; j++){
      if(tiles[i][j] == 0) {
        x_hole = i;
        y_hole = j;
      }
      board[i][j] = tiles[i][j];
    }
  }
}

//-----------PRINT FUNCTION-----------------------------------------------------
//prints the board on the terminal

void Board::print()const{
  for (int i = 0; i < N; i++){
    for (int j = 0; j < N; j++)
      cout << board[i][j]<< ' ';
    cout << endl;
  }
  cout << endl;
}

//-------------HEURISTIC CHOOSING FUNCTION--------------------------------------
// returns desired heuristic.

int Board::heuristic_val(char choose){
  if (choose == 'h')
   return hamming();
  else if (choose == 'm')
   return manhattan();
  else if (choose == 'e')
   return euclidean();
  else if (choose == 'n')
   return nilsson_score();
  else if (choose == 'b')
   return blank();
  else{
      cout << "Board::heuristic(char choose)\nBug here!!!!!!!!!!\n";
      return 1;
  }
}

//-------------ABS (absolute)---------------------------------------------------
//gives the absolute of the given value

int abs_(int x){
  if (x >= 0) return x;
  else return -x;
}


/*
**                  • ADIMISSIBLE HEURISTICS
**    An admissible heuristic never overestimates the cost of reaching the goal.
** Using an admissible heuristic will always result in an optimal solution.
                    • NON-ADIMISSIBLE HEURISTICS
**    A non-admissible heuristic may overestimate the cost of reaching the
** goal. It may or may not result in an optimal solution. However, the
** advantage is that sometimes, a non-admissible heuristic expands much
** fewer nodes. Thus, the total cost (= search cost + path cost) may actually
** be lower than an optimal solution using an admissible heuristic.
*/


/*---------------BLANK (HEURISTIC)----------------------------------------------
**          NON ADMISSIBLE HEURISTIC
** The sum of the Manhattan distances from a tile to the blank square plus
** nilsson's manhattan.
** for this heuristic the final board state is
**
**       1 2 3
**       8 0 4
**       7 6 5
*/

int Board::blank(){
   int sum_blank = 0;
   for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++)
         sum_blank += abs_(i-x_hole) + abs_(j-y_hole);
   return sum_blank - 12 + nilsson_manhattan();
}

/*-----------------EUCLIDEAN  (HEURISTIC)---------------------------------------
**          ADMISSIBLE HEURISTIC
** Sum of Eucledian distances of the tiles from their goal positions.
** for this heuristic the final board state is
**
**       1 2 3
**       4 5 6
**       7 8 0
*/

int Board::euclidean(){
   double sum_euclidean = 0;
   for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      sum_euclidean += euclidean_distance(i,j,board[i][j]);
      return floor(sum_euclidean);
}

/*-----------------HELPER FUNTION-----------------------------------------------
** return the euclidean distance value for a particular tile.
** x1,y1 be goalstate positions and X1,Y1 be current state position.
** then the euclidean distance is given by square root of (x1-X1)^2 + (y1-Y1)^2
*/

double Board::euclidean_distance(int x,int y,int val){
   if(val==0)
      return 0;
   double distance = 0;
   distance = sqrt(pow(x - ((val-1)/N),2) + pow((y+1-val)%N,2) );
   return distance;
}

/*-------------------NILSSON'S SEQUENCE SCORE-----------------------------------
**          NON ADMISSIBLE HEURISTIC.
**    Nilsson’s Sequence Score
**  h(n) = P(n) + 3 S(n)
**  P(n) : Sum of Manhattan distances of each tile from its proper position
**  S(n) : A sequence score obtained by checking around the non-central
**  squares in turn, allotting 2 for every tile not followed by its proper
**  successor and 0 for every other tile, except that a piece in the center
**  scores 1
** for this heuristic the final board state is
**
**       1 2 3
**       8 0 4
**       7 6 5
*/

int Board::nilsson_score(){
   int sequence_score = 0;
   if(board[1][1]!=0)
      sequence_score += 1;
   sequence_score += nilsson_check(0,0,0,1);
   sequence_score += nilsson_check(0,1,0,2);
   sequence_score += nilsson_check(0,2,1,2);
   sequence_score += nilsson_check(1,2,2,2);
   sequence_score += nilsson_check(2,2,2,1);
   sequence_score += nilsson_check(2,1,2,0);
   sequence_score += nilsson_check(2,0,1,0);
   sequence_score += nilsson_check(1,0,0,0);
   return 3 * sequence_score + nilsson_manhattan();
}

/*------------------HELPER FUNCTION---------------------------------------------
** NILSSONS_CHECK
** checks the sequence between the two inputs and returns 2 or 0 appropriately.
*/

int Board::nilsson_check(int x1,int y1,int x2,int y2){
   if(board[x2][y2]-board[x1][y1]!=1 && board[x2][y2]-board[x1][y1]!=-7)
      return 2;
   return 0;
}

/*-------------------HELPER FUNCTION--------------------------------------------
**  NILSSON_MANHATTAN
** gives the manhattan distance of a particular tile.
*/

int Board::nilsson_manhattan(){
   int manhattan_score = 0;
   for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++){
      if(board[i][j]==0){
         manhattan_score +=  abs_(i-1) + abs_(j-1);
      }
      if(board[i][j]==1){
         manhattan_score +=  abs_(i-0) + abs_(j-0);
      }
      if(board[i][j]==2){
         manhattan_score +=  abs_(i-0) + abs_(j-1);
      }
      if(board[i][j]==3){
         manhattan_score +=  abs_(i-0) + abs_(j-2);
      }
      if(board[i][j]==4){
         manhattan_score +=  abs_(i-1) + abs_(j-2);
      }
      if(board[i][j]==5){
         manhattan_score +=  abs_(i-2) + abs_(j-2);
      }
      if(board[i][j]==6){
         manhattan_score +=  abs_(i-2) + abs_(j-1);
      }
      if(board[i][j]==7){
         manhattan_score +=  abs_(i-2) + abs_(j-0);
      }
      if(board[i][j]==8){
         manhattan_score +=  abs_(i-1) + abs_(j-0);
      }
      }
   return manhattan_score;
}

/*--------------------------HAMMING   (HEURISTIC)-------------------------------
**      ADMISSIBLE HEURISTIC
**  It is the no of miss placed tiles.
**       1 2 3
**       4 5 6
**       7 8 0
*/

int Board::hamming(){
  int sum_mismatch = 0;
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      if (N*i + j+1 != board[i][j])
        sum_mismatch ++;
  if(board[N-1][N-1] == 0) sum_mismatch--;
  return sum_mismatch;
}

/*---------------------MANHATTAN   (HEURISTIC)-----------------------------------
**       ADMISSIBLE HEURISTIC
** Sum of Manhattan distances of the tiles from their goal positions.
**       1 2 3
**       4 5 6
**       7 8 0
*/

int Board::manhattan(){
  int sum_Distances = 0;
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      sum_Distances += distance(i,j,board[i][j]);
  return sum_Distances;
}

/*--------------DISTANCE HELPER FUNCTION----------------------------------------
** gives the manhattan distance for the given input.
*/

int Board::distance(int x,int y,int val){
  if(val == 0) return (N-1 - x) + (N-1 - y);
  return abs_(x - ((val-1)/N)) + abs_(y+1-val)%N;
}

//-------------GETBOARDVAL-----------------------------------------------------
// returns the board value for a particula i,j values.

int Board::getBoardVal(int i,int j) const{
  return board[i][j];
}

/*-----------------OPERATOR= OVERLOADING----------------------------------------
** overloading = operator for the board.
** wherever we equal two board data types this function copies all the data
** members into the left operand.
*/

void Board::operator=(Board b){
  this->direction = b.direction;
  this->N = b.N;
  board = new int*[N];
  for (int i = 0; i < N; i++){
    board[i] = new int[N];
    for (int j = 0; j < N; j++){
      int temp = b.getBoardVal(i,j);
      if (temp == 0){
        x_hole = i;
        y_hole = j;
      }
      board[i][j] = temp;
    }
  }
  x_hole = b.x_hole;
  y_hole = b.y_hole;
  depth = b.depth ;
}

/*-----------------OPERATOR== OVERLOADING----------------------------------------
** overloading == operator for the board.
** wherever two board data types are compared this function returns a
** bool.
*/

bool Board::operator==(Board b)const{
   for(int i = 0;i < N; i++){
      for (int j = 0; j < N; j++){
         if(this->getBoardVal(i,j) != b.getBoardVal(i,j))
            return false;
      }
   }
   return true;
}

/*-----------------OPERATOR< OVERLOADING----------------------------------------
** overloading < operator for the board.
** wherever two board data types are compared this function always true.
*/

bool Board::operator<(Board)const{
   return true;
}

/*----------------------NEIGHBOURS----------------------------------------------
** This function returns the queue containing all the possible outcomes
** of the given board.
*/

queue<char> Board::neighbours(){
  queue <char> neighbours_q;
  map<char,bool> dir_map;

  dir_map['u'] = true;//up
  dir_map['d'] = true;//down
  dir_map['l'] = true;//left
  dir_map['r'] = true;//right

  if(x_hole == 0)   dir_map['u'] = false;
  if(x_hole == N-1) dir_map['d'] = false;
  if(y_hole == 0)   dir_map['l'] = false;
  if(y_hole == N-1) dir_map['r'] = false;

  if(dir_map['u'] && parent != 'd') neighbours_q.push('u');
  if(dir_map['d'] && parent != 'u') neighbours_q.push('d');
  if(dir_map['r'] && parent != 'l') neighbours_q.push('r');
  if(dir_map['l'] && parent != 'r') neighbours_q.push('l');

  return neighbours_q;
}
/*--------------------MAKEMOVE--------------------------------------------------
** This function when given a char (either of 'u','d','l','r') , modifies the
** board such that the blank space is move according to the input.
** code is very simple to understand.
*/
void Board::makeMove(char path){
//  printf("x %d,y %d parent %c\n",x_hole,y_hole,parent);
  int temp;
  parent = path;
  switch (path){
    case 'u':{
      temp = board[x_hole - 1][y_hole];
      board[x_hole - 1][y_hole] = 0;
      board[x_hole][y_hole] = temp;
      x_hole--;
      break;
    }
    case 'd':{
      temp = board[x_hole + 1][y_hole];
      board[x_hole + 1][y_hole] = 0;
      board[x_hole][y_hole] = temp;
      x_hole++;
      break;
    }
    case 'l':{
      temp = board[x_hole][y_hole - 1];
      board[x_hole][y_hole - 1] = 0;
      board[x_hole][y_hole] = temp;
      y_hole--;
      break;
    }
    case 'r':{
      int temp = board[x_hole][y_hole + 1];
      board[x_hole][y_hole + 1] = 0;
      board[x_hole][y_hole] = temp;
      y_hole++;
      break;
    }
  }
}
