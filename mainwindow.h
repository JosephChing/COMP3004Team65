#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimerEvent>
#include "heartwave.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
<<<<<<< Updated upstream
=======
    void initGraph();
    void updateGraph();
    void endOfGraph();
>>>>>>> Stashed changes

private:
    Ui::MainWindow *ui;

    int timerID;


    Heartwave * heartwave;
protected:
    void timerEvent(QTimerEvent *event);
};
#endif // MAINWINDOW_H
