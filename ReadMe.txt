------------------------------------------------------------------------
About
------------------------------------------------------------------------
This program is a Graphical application made on Qt which can take input 
to the 8Puzzle/15Puzzle and returns the solution steps. 
Also the pictorial sliding is shown in the application. 

We Have implemented the (N^2 - 1)-Puzzle Solver using 2 algorithms.
1) A* Algorithm
2) IDA* Algorithm

With n=3 the heuristics provided are 
	a) Hammimg
	b) Manhattan
	c) Euclidean
	d) NilsonScore(n=3 specific)
	e) Blank(n=3 specific)
With n=4 the heuristics provided are
	a) Hammimg
	b) Manhattan
	c) Euclidean


------------------------------------------------------------------------
NOTE
------------------------------------------------------------------------
1) This program when used with IDA* Algorithm and Manhattan Heuristics 
   can solve even the hardest 8-Puzzle (31 Moves).
2) In 15-puzzle cases to test inputs with higher search depths, the 
   COST_LIMIT macro in "Solver.cpp" can be increased upto 50.
   (If you are patient enough to wait for 5-10 minutes. Even after 
   waiting you might be dissappointed if the Puzzle is unsolvable, due  
   to the following fact)
4) The interesting thing about these puzzles is half of the possible 
   arrangements are impossible to be solved.

------------------------------------------------------------------------
Setup and Running
------------------------------------------------------------------------

1.Install Qt.
  Method-1: (Use this if you want to develop code in Qt-IDE)
    sudo apt-get install qtcreator
    (Found that qt-at-spi was leading to some errors so I removed it)
    sudo apt-get remove qt-at-spi 
    
  Method 2: Run the following commands(if u dont want IDE)
    sudo apt-get install libqt4-dev
    sudo apt-get install qmake

3.Run the command 
  qmake && make
  
4.Now you can start the program with the executable
  ./8puzzle
  
  Our observation IDA* with Hamming was giving the fastest output.

------------------------------------------------------------------------
Developed by
------------------------------------------------------------------------
1.Janga Varun - varunjanga[at]gmail[dot]com
2.T Pradeep Kumar - pradeep170993[at]gmail[dot]com
