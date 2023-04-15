#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include "mainwindow.h"

int currentGraphSecond = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), timerID(0)
{

    ui->setupUi(this);
    killTimer(timerID);
    ui->graph->setVisible(false);
    ui->summary->setVisible(false);

    this->heartwave = new Heartwave;
    timerID = startTimer(100);
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
    if(currentGraphSecond == 0){
        ui->graph->clearGraphs();
        ui->graph->clearItems();
    }

    ui->graph->addGraph(0);
    ui->graph->yAxis->setRange(40,200);
    ui->graph->xAxis->setRange(0,30);
    ui->graph->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->graph->replot();

}



//function updates the info of graph each cycle of clock
//it uses currentGraphSecond global variable to index array
void MainWindow::updateGraph(){


    if(currentGraphSecond==0){
        initGraph();
    }

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
    currentGraphSecond = 0;
    this->heartwave->setActivePulseReading(false);

//    ui->graph->graph(0)->

    ui->graph->replot();


}




MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::timerEvent(QTimerEvent *event)
{
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
    if(masterMenu->getName() == "BREATH PACER") {
        return;
    }
    if(masterMenu->getName() == "SELECT SESSION"){
        currentGraphSecond = 0;

        if(masterMenu->getMenuItems()[index] == "START SESSION 1"){

            this->heartwave->setCurrentSession(1);
            this->heartwave->setActivePulseReading(true);
            ui->graph->setVisible(true);
            qInfo("Session 1"); // GRAPH 1
        }
        else if (masterMenu->getMenuItems()[index] == "START SESSION 2"){
            this->heartwave->setCurrentSession(2);
            this->heartwave->setActivePulseReading(true);
            ui->graph->setVisible(true);
            qInfo("Session 2"); // GRAPH 2
        }
        else if (masterMenu->getMenuItems()[index] == "START SESSION 3"){
            this->heartwave->setCurrentSession(3);
            this->heartwave->setActivePulseReading(true);
            ui->graph->setVisible(true);
            qInfo("Session 3"); // GRAPH 3
        }
    }

    if(masterMenu->getName() == "START SESSION 1") {
        endOfGraph();
        heartwave->setCurrentSession(1);
        if(masterMenu->getMenuItems()[index] == "Currently running session 1 (click to end)") {
            ui->summary->setPlainText("Summary 1");
            ui->summary->show();
            ui->summary->setVisible(true);
           endOfGraph();
            qInfo("This is where end of graph 1 should run"); // end of graph function should run here for graph 1
            heartwave->currentSession->interruptSession();
        }
    }
    if(masterMenu->getName() == "START SESSION 2") {
        endOfGraph();
        heartwave->setCurrentSession(2);
        if(masterMenu->getMenuItems()[index] == "Currently running session 2 (click to end)") {
            ui->summary->setPlainText("Summary 2");
            ui->summary->show();
            ui->summary->setVisible(true);
            qInfo("This is where end of graph 2 should run"); // end of graph function should run here for graph 2
            heartwave->currentSession->interruptSession();
        }
    }
    else if(masterMenu->getName() == "START SESSION 3") {
        endOfGraph();
        heartwave->setCurrentSession(3);
        if(masterMenu->getMenuItems()[index] == "Currently running session 3 (click to end)") {
            ui->summary->setPlainText("Summary 3");
            ui->summary->show();
            ui->summary->setVisible(true);
            qInfo("This is where end of graph 3 should run"); // end of graph function should run here for graph 3
            heartwave->currentSession->interruptSession();
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
            if(this->heartwave->currentSession == nullptr){
                qInfo()<<"no current session";
            }

//            this->heartwave->setActivePulseReading(true);


        }
        MainWindow::updateMenu(masterMenu->getName(), masterMenu->getMenuItems());

    }

    else if (masterMenu->get(index)->getName() == "VIEW") {
        masterMenu = masterMenu->get(index);
        //MainWindow::updateMenu("LOG/HISTORY", sessionsArray); -> this lists the sessions array
    }
}

void MainWindow::navigateToMainMenu() {
    if(masterMenu->getName() == "MAIN MENU") {
        return;
    }
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
    ui->summary->setVisible(false);

    while (masterMenu->getName() != "MAIN MENU") {
        masterMenu = masterMenu->getParent();
    }

    updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
}

void MainWindow::navigateBack() {
    if(masterMenu->getName() == "MAIN MENU") {
        return;
    }
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
    ui->graph->setVisible(false);
    ui->summary->setVisible(false);
}
