#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

#include <QObject>
#include <QDebug>

int currentGraphSecond = 0;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), timerID(0)
{

    ui->setupUi(this);
    killTimer(timerID);

    ui->graph->setVisible(false);

    this->heartwave = new Heartwave;
    timerID = startTimer(10);
    initGraph();
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






void MainWindow::initGraph()
{
    ui->graph->addGraph(0);
    ui->graph->yAxis->setRange(40,90);
    ui->graph->xAxis->setRange(0,30);
    ui->graph->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);

}


    timerID = startTimer(1000);

    heartwave = new Heartwave();


//function updates the info of graph each cycle of clock
//it uses currentGraphSecond global variable to index array
void MainWindow::updateGraph(){

    this->ui->graph->graph(0)->addData(this->heartwave->currentSession->seconds[currentGraphSecond],this->heartwave->currentSession->hrArray[currentGraphSecond]);
    if(this->heartwave->currentSession->seconds[currentGraphSecond]>15){
        this->ui->graph->xAxis->setRange(this->heartwave->currentSession->seconds[currentGraphSecond]-15,this->heartwave->currentSession->seconds[currentGraphSecond]+10);
    }

    this->ui->graph->replot();

    if(currentGraphSecond+1 == this->heartwave->currentSession->length){

        qInfo()<<"End of array";
        this->heartwave->setActivePulseReading(false);
        endOfGraph();
    }

     this->ui->graph->replot();

    currentGraphSecond += 1;

    qInfo()<<this->heartwave->currentSession->hrArray.capacity();
}



void MainWindow::endOfGraph()
{
    ui->graph->clearItems();
    ui->graph->replot();
    currentGraphSecond = 0;
    this->heartwave->currentSession->generateSummary();
    QString qstr = QString::fromStdString(this->heartwave->currentSession->summary);
    updateMenu("HEY",{qstr});
}


void MainWindow::updateGraph(){
    this->ui->graph->graph(0)->addData(xVec[currentGraphSecond],yVec[currentGraphSecond]);

    if(xVec[0]>10){
        this->ui->graph->xAxis->setRange(xVec[currentGraphSecond]-10,xVec[currentGraphSecond]+1);
    }
    this->ui->graph->replot();/*
    xVec.remove(0);
    yVec.remove(0);*/
    currentGraphSecond += 1;
    qDebug()<<xVec[0];

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::timerEvent(QTimerEvent *event)
{

    heartwave->update();
//    heartwave->update();

    if ((this->heartwave->getActivePulseReading() == true)){
        updateGraph();
    }

//    ui->breathPaceIndicator->setValue(heartwave->breathPacer->currentPosition);
}

void MainWindow::initializeMainMenu(Menu* m) {
    Menu* settings = new Menu("SETTINGS", {"BREATH PACER"}, m);
    Menu* sessions = new Menu("SELECT SESSION", {"START SESSION 1", "START SESSION 2", "START SESSION 3"}, m);
    Menu* history = new Menu("LOG/HISTORY", {"VIEW","CLEAR"}, m);
//    Menu* summary = new Menu(this->heartwave->currentSession->generateSummary(),{},m);


    m->addChildMenu(settings);
    m->addChildMenu(sessions);
    m->addChildMenu(history);
    Menu* pacer = new Menu("BREATH PACER", {"10", "20", "30"}, settings);
    settings->addChildMenu(pacer);
    Menu* startSession1 = new Menu("START SESSION 1", {}, m);
    Menu* startSession2 = new Menu("START SESSION 2", {}, m);
    Menu* startSession3 = new Menu("START SESSION 3", {}, m);
    sessions->addChildMenu(startSession1);
    sessions->addChildMenu(startSession2);
    sessions->addChildMenu(startSession3);
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
    if(masterMenu->getName() == "SELECT SESSION"){
        if(masterMenu->getMenuItems()[index] == "START SESSION 1"){
            this->heartwave->setActivePulseReading(true);
            ui->graph->setVisible(true);

            qInfo("Session 1"); // GRAPH 1
            return;
        }
        else if (masterMenu->getMenuItems()[index] == "START SESSION 2"){
            qInfo("Session 2"); // GRAPH 2
            return;
        }
        else if (masterMenu->getMenuItems()[index] == "START SESSION 3"){
            qInfo("Session 3"); // GRAPH 3
            return;
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

        //Start of heartwave session
    if (masterMenu->get(index)->getMenuItems().length() > 0) {
        masterMenu = masterMenu->get(index);
        if(masterMenu->getName() == "SELECT SESSION") {
            //if there is no current session selected
//            if(this->heartwave->currentSession == nullptr){
//                qInfo()<<"no current session";
//            }
//            this->heartwave->setActivePulseReading(true);
//            ui->graph->setVisible(true);

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
    ui->graph->setVisible(false);

    while (masterMenu->getName() != "MAIN MENU") {
        masterMenu = masterMenu->getParent();
    }

    updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
}




    ui->breathPaceIndicator->setValue(heartwave->breathPacer->currentPosition);
}
