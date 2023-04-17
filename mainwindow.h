#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QListWidget>
#include <QTimerEvent>
#include <QVector>
#include <unistd.h>
#include "heartwave.h"
#include "menu.h"
#include "session.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private:

    Ui::MainWindow *ui;
    int timerID;
    bool power;
    Heartwave * heartwave;
    Menu* masterMenu;
    Menu* mainMenu;
    QListWidget *activeQListWidget;
    void updateMenu(const QString, const QStringList);
    void initializeInterface(Menu*);
    void updateLight();
    void updateBeep();

    void initGraph();
    void updateGraph();
    void endOfGraph();
    void turnOnDevice();
    void shutOffDevice();
    double prevCoheranceLevel;
    bool needPlayBeep;

protected:
    void timerEvent(QTimerEvent *event);
private slots:
    void navigateDownMenu();
    void navigateUpMenu();
    void navigateSubMenu();
    void navigateToMainMenu();
    void navigateBack();
    void on_batteryReplaceButton_clicked();
    void on_breathPaceComboBox_currentIndexChanged(int index);
    void on_offButton_clicked();
    void on_disconnectButton_clicked();
    void on_reconnectButton_clicked();
    void navigateRightMenu();
};
#endif // MAINWINDOW_H
