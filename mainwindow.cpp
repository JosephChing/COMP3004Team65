#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), timerID(0)
{
    ui->setupUi(this);
    killTimer(timerID);
    ui->widget->setVisible(false);

    timerID = startTimer(1000);

    heartwave = new Heartwave();
    masterMenu = new Menu("MAIN MENU", {"SETTINGS","SELECT SESSION","LOG/HISTORY"}, nullptr);
    mainMenu = masterMenu;
    initializeMainMenu(masterMenu);

    activeQListWidget = ui->mainMenuListView;
    activeQListWidget->addItems(masterMenu->getMenuItems());
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(masterMenu->getName());


    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::navigateUpMenu);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::navigateDownMenu);
    connect(ui->selectButton, &QPushButton::pressed, this, &MainWindow::navigateSubMenu);
    connect(ui->menuButton, &QPushButton::pressed, this, &MainWindow::navigateToMainMenu);
    connect(ui->returnButton, &QPushButton::pressed, this, &MainWindow::navigateBack);


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

void MainWindow::initializeMainMenu(Menu* m) {
    Menu* settings = new Menu("SETTINGS", {"BREATH PACER"}, m);
    Menu* sessions = new Menu("SELECT SESSION", {"START SESSION 1", "START SESSION 2", "START SESSION 3"}, m);
    Menu* history = new Menu("LOG/HISTORY", {"VIEW","CLEAR"}, m);

    m->addChildMenu(settings);
    m->addChildMenu(sessions);
    m->addChildMenu(history);
    Menu* pacer = new Menu("BREATH PACER", {"10", "20", "30"}, settings);
    settings->addChildMenu(pacer);
    Menu* startSession1 = new Menu("START SESSION 1", {"Currently running session 1 (click to end)"}, sessions);
    Menu* startSession2 = new Menu("START SESSION 2", {"Currently running session 2 (click to end)"}, sessions);
    Menu* startSession3 = new Menu("START SESSION 3", {"Currently running session 3 (click to end)"}, sessions);
    Menu* endSession1 = new Menu("Currently running session 1 (click to end)", {}, startSession1);
    Menu* endSession2 = new Menu("Currently running session 2 (click to end)", {}, startSession2);
    Menu* endSession3 = new Menu("Currently running session 3 (click to end)", {}, startSession3);
    sessions->addChildMenu(startSession1);
    sessions->addChildMenu(startSession2);
    sessions->addChildMenu(startSession3);
    startSession1->addChildMenu(endSession1);
    startSession2->addChildMenu(endSession2);
    startSession3->addChildMenu(endSession3);
    Menu* viewHistory = new Menu("VIEW",{"Something for now"}, history);
    Menu* clearHistory = new Menu("CLEAR", {"YES","NO"}, history);
    history->addChildMenu(viewHistory);
    history->addChildMenu(clearHistory);
}

void MainWindow::navigateUpMenu() {

    int nextIndex = activeQListWidget->currentRow() - 1;

    if (nextIndex < 0) {
        nextIndex = activeQListWidget->count() - 1;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}

void MainWindow::navigateDownMenu() {

    int nextIndex = activeQListWidget->currentRow() + 1;

    if (nextIndex > activeQListWidget->count() - 1) {
        nextIndex = 0;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}

void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems) {

    activeQListWidget->clear();
    activeQListWidget->addItems(menuItems);
    activeQListWidget->setCurrentRow(0);

    ui->menuLabel->setText(selectedMenuItem);
}

void MainWindow::navigateSubMenu() {

    int index = activeQListWidget->currentRow();
    if (index < 0) return;

    if (masterMenu->getName() == "VIEW") {
        return;
    }
    /*if(masterMenu->getName() == "SELECT SESSION"){
        if(masterMenu->getMenuItems()[index] == "START SESSION 1"){
            qInfo("Session 1"); // GRAPH 1
        }
        else if (masterMenu->getMenuItems()[index] == "START SESSION 2"){
            qInfo("Session 2"); // GRAPH 2
        }
        else if (masterMenu->getMenuItems()[index] == "START SESSION 3"){
            qInfo("Session 3"); // GRAPH 3
        }
    }*/

    if(masterMenu->getName() == "START SESSION 1") {
        if(masterMenu->getMenuItems()[index] == "Currently running session 1 (click to end)") {
            qInfo("This is where end of graph 1 should run"); // end of graph function should run here for graph 1
        }
    }
    if(masterMenu->getName() == "START SESSION 2") {
        if(masterMenu->getMenuItems()[index] == "Currently running session 2 (click to end)") {
            qInfo("This is where end of graph 2 should run"); // end of graph function should run here for graph 2
        }
    }
    else if(masterMenu->getName() == "START SESSION 3") {
        if(masterMenu->getMenuItems()[index] == "Currently running session 3 (click to end)") {
            qInfo("This is where end of graph 3 should run"); // end of graph function should run here for graph 3
        }
    }

    if (masterMenu->getName() == "CLEAR") {
        if (masterMenu->getMenuItems()[index] == "YES") {
            //delete sessions array
            qInfo("YEES");
            return;
        }
        else {
            navigateBack();
            return;

        }
    }

    if (masterMenu->get(index)->getMenuItems().length() > 0) {
        masterMenu = masterMenu->get(index);
        if(masterMenu->getName() == "SELECT SESSION") {
            /*if(masterMenu->getMenuItems()[index] == "START SESSION 1") {
                //session 1 stuff
                qInfo("Session 1");
                return;
            }*/
        }
        MainWindow::updateMenu(masterMenu->getName(), masterMenu->getMenuItems());


    }

    else if (masterMenu->get(index)->getName() == "VIEW") {
        masterMenu = masterMenu->get(index);
        //MainWindow::updateMenu("LOG/HISTORY", sessionsArray); -> this lists the sessions array
    }
}

void MainWindow::navigateToMainMenu() {
        if (masterMenu->getParent()->getName() == "SESSIONS") {
            // stop the session get the time, and add to sessions array
        }
        else {
            // same shit as above if theres a session going on and it hasnt ended
        }

    if (masterMenu->getName() == "MAIN MENU") {
        activeQListWidget->setCurrentRow(0);
    }
    else {
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    }
    ui->widget->setVisible(false);

    while (masterMenu->getName() != "MAIN MENU") {
        masterMenu = masterMenu->getParent();
    }

    updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
}

void MainWindow::navigateBack() {
    ui->rightButton->blockSignals(true);
    ui->leftButton->blockSignals(true);
        if (masterMenu->getParent()->getName() == "START SESSION") {
            // stop the session get the time, and add to sessions array
        }
        else {
            // same shit as above if theres a session going on and it hasnt ended
        }

    if (masterMenu->getName() == "MAIN MENU") {
        activeQListWidget->setCurrentRow(0);
    }
    else {
        masterMenu = masterMenu->getParent();
        updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    }
    ui->widget->setVisible(false);
}

