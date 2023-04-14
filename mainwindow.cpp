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
    masterMenu = new Menu("MAIN MENU", {"SETTINGS","START SESSION","LOG/HISTORY"}, nullptr);
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
    Menu* sessions = new Menu("START SESSION", {"Something 2"}, m);
    Menu* history = new Menu("LOG/HISTORY", {"VIEW","CLEAR"}, m);

    m->addChildMenu(settings);
    m->addChildMenu(sessions);
    m->addChildMenu(history);
    Menu* pacer = new Menu("BREATH PACER", {"10", "20", "30"}, m);
    settings->addChildMenu(pacer);
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

    if (masterMenu->getName() == "CLEAR") {
        if (masterMenu->getMenuItems()[index] == "YES") {
            //delete sessions array
        }
        else {
            navigateBack();
            return;

        }
    }

    if (masterMenu->get(index)->getMenuItems().length() > 0) {
        masterMenu = masterMenu->get(index);
        if(masterMenu->getName() == "START SESSION") {
            ui->widget->setVisible(true);
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
