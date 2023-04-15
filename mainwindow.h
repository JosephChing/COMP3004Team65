#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTimerEvent>
#include <QVector>

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
    void initGraph();
    void updateGraph();
    void endOfGraph();
    void updateLight();

private:
    Ui::MainWindow *ui;
    int timerID;
    Heartwave * heartwave;
    Menu* masterMenu;
    Menu* mainMenu;
    QListWidget *activeQListWidget;
    void updateMenu(const QString, const QStringList);
    void initializeMainMenu(Menu*);

protected:
    void timerEvent(QTimerEvent *event);
private slots:
    void navigateDownMenu();
    void navigateUpMenu();
    void navigateSubMenu();
    void navigateToMainMenu();
    void navigateBack();
    void on_batteryReplaceButton_clicked();
};
#endif // MAINWINDOW_H
