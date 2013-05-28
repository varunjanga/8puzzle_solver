#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <stdlib.h>
#include <ctime>

QString convertInt(int number);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    solver = new Solver;
    scene = new QGraphicsScene( ui->graphicsView );
    timer = new QTimeLine;
    QMessageBox::information(this,
                             "Welcome!!",
                             "    Welcome to 8puzzle Solver.\n\nNote:\nIn 3x3, The Goal State for Hammimg,\n  Manhattan and Euclidean Heuristics is\n          1 2 3\n          4 5 6\n          7 8 0\n  whereas for NilsonScore and Blank it is\n          1 2 3\n          8 0 4\n          7 6 5\n\nIn 4x4,The Goal State for provided heuristics is\n            1  2  3  4\n            5  6  7  8\n            9 10 11 12\n           13 14 15  0");
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*************************Solve Button Handler*********************
The main function which send the current settings to the Solver object
It takes 1.Size
         2.Input board
         3.Algorithm
         4.Heuristic
Then solver is called to give the Shortest Path to the goal
*/
void MainWindow::on_pushButton_clicked()
{
    delete scene;
    scene = new QGraphicsScene( ui->graphicsView );
    solver->N = 3 + ui->comboBox_size->currentIndex();
    int** tiles;
    N = solver->N;
    tiles = new int*[N];
    for (int i = 0; i < N; i++)
      tiles[i] = new int[N];

    QString input_string = ui->lineEdit->text();

    bool parsed  = parseInput(input_string,tiles);
    if(!parsed) {
        QMessageBox::information(this,"Error","Give input of proper size.");
        return;
    }
    else {
        show_grid(tiles);
        switch(ui->comboBox_algo->currentIndex()){
            case 0:
                solver->setAlgo('i');
                break;
            case 1:
                solver->setAlgo('a');
                break;
        }
        switch(ui->comboBox_heuristic->currentIndex()){
        case 0:
            solver->setHeuristic('m');
            break;
        case 1:
            solver->setHeuristic('h');
            break;
        case 2:
            solver->setHeuristic('e');
            break;
        case 3:
            solver->setHeuristic('n');
            break;
        case 4:
            solver->setHeuristic('b');
            break;
        }
        (*solver)(tiles);
/*
        QTimeLine *timer2;
        timer2 = new QTimeLine(50000);
        timer2->setFrameRange(0, 1000);
        timer2->start();
*/
        time_t start,end; //= clock();
        time(&start);
        pair<bool,queue<Board> > temp_pr = (solver->solve());
        time(&end);
        //int end_ = clock();

        //qDebug() << difftime(end,start);
        QString timeStr = "Time taken: ";
        timeStr += convertInt((difftime(end,start))) + " seconds";
        QMessageBox::information(this,"Time",timeStr);

        if(temp_pr.first){
            //cout << temp_pr.second.size() - 1;
            QString s ,no_of_moves = "No. of moves: ";
            queue<Board> temp = temp_pr.second;
            currentSolution = temp;
            no_of_moves += convertInt(temp.size()-1);
            while(!temp.empty()){
                for (int i = 0; i < N; i++){
                    for (int j = 0; j < N; j++){
                        QString temp_str = convertInt(temp.front().getBoardVal(i,j));
                        s += temp_str;
                        s.push_back(' ');
                    }
                }
                s.push_back('\n');
                temp.pop();
            }
            s += no_of_moves;
            ui->textBrowser->clear();
            ui->textBrowser->append(s);
        }
        else
        {
            if(N==3)
                QMessageBox::information(
                            this,
                            "Error",
                            "Searched till depth 30\nIn 99% of cases reached here,\nit is not be solvable.");
            else if(N==4)
                QMessageBox::information(
                            this,
                            "Error",
                            "Searched till depth 30\nIt is not be solvable.\n           or\nOur efficiency has to be improved");
        }
    }
}

//Parses the input string into 2d array
bool MainWindow::parseInput(QString input_string, int **tiles)
{
    int count = 0;
    if (input_string.size() == 0) return false;
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++){
          while(input_string[count] == ' ') {
              count++;
              if(count >= input_string.size())
                  return false;
          }
          int temp = 0;
          while(input_string[count] != ' '){
              temp = 10*temp + (input_string[count].toAscii()-48);
              count++;
              if (count == input_string.size()){
                  if((i!=N-1)||(j!=N-1))
                      return false;
                  else
                      break;
              }
          }
          tiles[i][j] = temp;
      };
    while(input_string[count] == ' ') {
        count++;
        if(count == input_string.size())
            break;
    }
    if(count != input_string.size())
        return false;
    return true;
}

//Shows the input in the Board Display box
void MainWindow::show_grid(int **tiles)
{
    ui->graphicsView->setScene( scene );

    pix[1] = new QPixmap("img/1.xpm");
    pix[2] = new QPixmap("img/2.xpm");
    pix[3] = new QPixmap("img/3.xpm");
    pix[4] = new QPixmap("img/4.xpm");
    pix[5] = new QPixmap("img/5.xpm");
    pix[6] = new QPixmap("img/6.xpm");
    pix[7] = new QPixmap("img/7.xpm");
    pix[8] = new QPixmap("img/8.xpm");
    pix[9] = new QPixmap("img/9.xpm");
    pix[10] = new QPixmap("img/10.xpm");
    pix[11] = new QPixmap("img/11.xpm");
    pix[12] = new QPixmap("img/12.xpm");
    pix[13] = new QPixmap("img/13.xpm");
    pix[14] = new QPixmap("img/14.xpm");
    pix[15] = new QPixmap("img/15.xpm");

    timer = new QTimeLine(10);
    timer->setFrameRange(0, 100);
    timer->start();

    for(int i=1; i<N*N ;i++){
        pixItem[i] = new QGraphicsPixmapItem(*pix[i]);
        animation[i] = new QGraphicsItemAnimation;
        animation[i]->setTimeLine(timer);
        animation[i]->setItem(pixItem[i]);
        scene->addItem(pixItem[i]);
    }
    for (int i = 0; i < N; i++)
      for (int j = 0; j < N; j++){
          if(tiles[i][j])
              animation[tiles[i][j]]->setPosAt(0, QPointF(35*j,35*i));
      }
}

/*
Helper function for NextMove which shows Animation for Sliding an image
*/
void MainWindow::move(int x, int y, int xshift, int yshift,int index)
{
    timer = new QTimeLine(5000);
    timer->setFrameRange(0, 100);
    animation[index]->setTimeLine(timer);
    for(int i = 0; i <= 10; ++i)
        animation[index]->setPosAt(i / 2000.0,
                                   QPointF( 35*(y + (yshift*(i/10.0))),
                                            35*(x + (xshift*(i/10.0))) ));
    timer->start();
}

//Function to convert number to string
QString convertInt(int number)
{
    if (number == 0)
        return "0";
    QString temp="";
    QString returnvalue="";
    while (number>0){
        temp+=number%10+48;
        number/=10;
    }
    for (int i=0;i<temp.length();i++)
        returnvalue+=temp[temp.length()-i-1];
    return returnvalue;
}


/*----------------------BoardSize change handler----------------------
Modifies the following in Window when the size is changed.
1.Window size
2.Trace output box
3.Input box
4.Board display box
5.NextMove button
6.Heuristic list.
*/
void MainWindow::on_comboBox_size_currentIndexChanged(int index)
{
    if(index == 0){
        ui->textBrowser->clear();
        ui->textBrowser->resize(140,232);
        ui->lineEdit->clear();
        ui->lineEdit->resize(140,25);
        ui->graphicsView->resize(111,111);
        scene->setSceneRect(0, 0, 111, 111);
        ui->pushButton_nextMove->move(50, 290);
        this->resize(433, 425);
        ui->comboBox_heuristic->addItem("NilsonScore");
        ui->comboBox_heuristic->addItem("Blank");
    }
    if(index == 1){
        ui->textBrowser->clear();
        ui->textBrowser->resize(270,232);
        ui->lineEdit->clear();
        ui->lineEdit->resize(270,25);
        ui->graphicsView->resize(150,150);
        scene->setSceneRect(0, 0, 150, 150);
        ui->pushButton_nextMove->move(70, 325);
        this->resize(563, 425);
        ui->comboBox_heuristic->removeItem(4);
        ui->comboBox_heuristic->removeItem(3);
    }
}


/*----------------------NextMove Button handler------------------------
Slides an image using QGraphicsItemAnimation and timer
Finds which image to move and sends it to the helper function move(...)
*/
void MainWindow::on_pushButton_nextMove_clicked()
{
    if(currentSolution.empty()) return;
    Board cur = currentSolution.front();
    currentSolution.pop();
    if(currentSolution.empty()){
        QMessageBox::information(this,"","Goal State reached.");
    }
    else{
        Board next = currentSolution.front();
        if (cur.x_hole == next.x_hole){
            if (cur.y_hole > next.y_hole){
                move(cur.x_hole,cur.y_hole-1,0,1,cur.getBoardVal(cur.x_hole,cur.y_hole-1));
            }
            else if(cur.y_hole < next.y_hole){
                move(cur.x_hole,cur.y_hole+1,0,-1,cur.getBoardVal(cur.x_hole,cur.y_hole+1));
            }
            else {
                QMessageBox::information(this,"Error 1","Something Went wrong.");
            }
        }
        else if(cur.x_hole < next.x_hole){
            if (cur.y_hole == next.y_hole){
                move(cur.x_hole+1,cur.y_hole,-1,0,cur.getBoardVal(cur.x_hole+1,cur.y_hole));
            }
            else{
                QMessageBox::information(this,"Error 2","Something Went wrong.");
            }
        }
        else{
            if (cur.y_hole == next.y_hole){
                move(cur.x_hole-1,cur.y_hole,+1,0,cur.getBoardVal(cur.x_hole-1,cur.y_hole));
            }
            else{
                QMessageBox::information(this,"Error 3","Something Went wrong.");
            }
        }
    }
}
