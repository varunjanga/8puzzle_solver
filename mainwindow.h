/*
The MainWindow Class
This class has all the functions which are used to add GUI.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Solver.h"
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemAnimation>
#include <QTimeLine>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    /*************************Solve Button Handler*********************
    The main function which send the current settings to the Solver object
    It takes 1.Size
             2.Input board
             3.Algorithm
             4.Heuristic
    Then solver is called to give the Shortest Path to the goal
    */
    void on_pushButton_clicked();                           //Solve button handler


    /*----------------------BoardSize change handler----------------------
    Modifies the following in Window when the size is changed.
    1.Window size
    2.Trace output box
    3.Input box
    4.Board display box
    5.NextMove button
    6.Heuristic list.
    */
    void on_comboBox_size_currentIndexChanged(int index);   //Board size change handler

    /*----------------------NextMove Button handler------------------------
    Slides an image using QGraphicsItemAnimation and timer
    Finds which image to move and sends it to the helper function move(...)
    */
    void on_pushButton_nextMove_clicked();                  //NextMove button handler

    void on_pushButton_help_clicked();

private:
    int N;                                      //Board size
    Ui::MainWindow *ui;                         //MainWindow object pointer represents the Window

    //Main functions used in Solving the Puzzle
    Solver* solver;                             //Solver object pointer which has the Solver operations
    queue<Board> currentSolution;               //updated for NextMove functionality
    bool parseInput(QString,int**);             //gives int** from lineInput given

    //Following are graphics related
    void show_grid(int **tiles);                //Shows input in the form of grid
    QTimeLine *timer;                       //The main Timer pointer which is used for animation(in NextMove button)
    QGraphicsScene *scene;                  //The current scene on the Graphics View
    QPixmap *pix[16];                       //Pixmap objects for images
    QGraphicsPixmapItem *pixItem[16];       //PixmapItem objects for Pixmap objects
    QGraphicsItemAnimation *animation[16] ; //Animation objects which animates the PixmapItem object with timer

    /**
    move :Helper function for NextMove which shows Animation for Sliding an image
    */
    void move(int x,int y,int xshift,int yshift,int index); //helper function for NextMove button handler
};

#endif // MAINWINDOW_H
