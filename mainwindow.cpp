#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QObject>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graph->xAxis->setRange(0.0,10.0);
    ui->graph->yAxis->setRange(0.0,10.0);
    QTimer *graphTimer = new QTimer(this);
    ui->graph->addGraph();
    ui->graph->graph(0)->setScatterStyle(QCPScatterStyle::ssPeace);
    ui->graph->graph(0)->setVisible(true);

    for(double i = 0; i < 60; i++){
        xVec.append(i);
//        yVec.append(i);
    }
    yVec.append(1);
    yVec.append(2);
    yVec.append(3);
    yVec.append(4);
    yVec.append(5);
    yVec.append(6);
    yVec.append(7);
    yVec.append(8);
    yVec.append(7);
    yVec.append(6);
    yVec.append(5);
    yVec.append(4);
    yVec.append(3);
    yVec.append(2);
    yVec.append(1);
    yVec.append(2);
    yVec.append(3);
    yVec.append(4);

    yVec.append(5);
    yVec.append(6);
    yVec.append(7);
    yVec.append(8);
    yVec.append(7);
    yVec.append(6);
    yVec.append(5);
    yVec.append(4);
    yVec.append(3);
    yVec.append(2);
    yVec.append(1);
    yVec.append(2);
    yVec.append(3);





//    xAx->insert(0,)
    graphTimer->setInterval(500);
    connect(graphTimer,SIGNAL(timeout()),this,SLOT(updateGraph()));
    graphTimer->start(500);

}




void MainWindow::updateGraph(){
    this->ui->graph->graph(0)->addData(xVec[0],yVec[0]);

    if(xVec[0]>10){
        this->ui->graph->xAxis->setRange(xVec[0]-10,xVec[0]+1);
    }
    this->ui->graph->replot();
    xVec.remove(0);
    yVec.remove(0);
    qDebug()<<xVec[0];
}

MainWindow::~MainWindow()
{
    delete ui;
}

