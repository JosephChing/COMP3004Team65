#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), timerID(0)
{
    ui->setupUi(this);
    killTimer(timerID);

    timerID = startTimer(1000);

    heartwave = new Heartwave();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::timerEvent(QTimerEvent *event)
{
    heartwave->update();




    ui->breathPaceIndicator->setValue(heartwave->breathPacer->currentPosition);
}
