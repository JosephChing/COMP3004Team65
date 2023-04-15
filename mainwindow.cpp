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
<<<<<<< Updated upstream
    timerID = startTimer(300);
=======
    timerID = startTimer(200);
>>>>>>> Stashed changes
    initGraph();
    heartwave = new Heartwave();
    masterMenu = new Menu("MAIN MENU", {"SETTINGS","SELECT SESSION","LOG/HISTORY"}, nullptr);
    mainMenu = masterMenu;
    initializeMainMenu(masterMenu);


    activeQListWidget = ui->mainMenuListView;
    activeQListWidget->addItems(masterMenu->getMenuItems());
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(masterMenu->getName());
    ui->breathPaceComboBox->setVisible(false);

    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::navigateUpMenu);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::navigateDownMenu);
    connect(ui->selectButton, &QPushButton::pressed, this, &MainWindow::navigateSubMenu);
    connect(ui->menuButton, &QPushButton::pressed, this, &MainWindow::navigateToMainMenu);
    connect(ui->returnButton, &QPushButton::pressed, this, &MainWindow::navigateBack);
}






//function updates the info of graph each cycle of clock
//it uses currentGraphSecond global variable to index array
void MainWindow::updateGraph(){
    if(heartwave->currentSession != nullptr) {
        if(heartwave->currentSession->clock == 0) {
            initGraph();
        }
        if(heartwave->currentSession->paused == false || heartwave->currentSession->ended == true) {
            ui->graph->graph(0)->addData(heartwave->currentSession->clock, heartwave->currentSession->heartRate);
        }
        if(heartwave->currentSession->clock > 15) {
            ui->graph->xAxis->setRange(this->heartwave->currentSession->clock - 15,this->heartwave->currentSession->clock + 10);
        }

        this->ui->graph->replot();
        updateLight();
    }
}

void MainWindow::endOfGraph()
{
    currentGraphSecond = 0;
    this->heartwave->setActivePulseReading(false);

//    ui->graph->graph(0)->

    ui->graph->replot();
    QPixmap  noLight(":/lightsPictures/noLights.png");
    ui->lightPicture->setPixmap(noLight);
    /*if(m->getMenuItems()[index] == "Currently running session 1 (click to end)") {
        ui->summary->setPlainText("Summary 1");
       endOfGraph();
        qInfo("This is where end of graph 1 should run"); // end of graph function should run here for graph 1
        heartwave->currentSession->interruptSession();
    }
    if(m->getMenuItems()[index] == "Currently running session 2 (click to end)") {
        ui->summary->setPlainText("Summary 2");
       endOfGraph();
        qInfo("This is where end of graph 1 should run"); // end of graph function should run here for graph 1
        heartwave->currentSession->interruptSession();
    }
    if(m->getMenuItems()[index] == "Currently running session 3 (click to end)") {
        ui->summary->setPlainText("Summary 3");
       endOfGraph();
        qInfo("This is where end of graph 1 should run"); // end of graph function should run here for graph 1
        heartwave->currentSession->interruptSession();
    }
    ui->summary->show();
    ui->summary->setVisible(true);*/

}

void MainWindow::updateLight()
{
    QPixmap  noLight(":/lightsPictures/noLights.png");
    QPixmap  red(":/lightsPictures/redLight.png");
    QPixmap  yellow(":/lightsPictures/yellowLight.png");
    QPixmap  green(":/lightsPictures/greenLight.png");

    if(heartwave->currentSession == nullptr) {
        ui->lightPicture->setPixmap(noLight);
        return;
    }




    if(heartwave->currentSession->ended) {
        ui->lightPicture->setPixmap(noLight);
    }

    if(heartwave->currentSession->coheranceRating <= 1){
        ui->lightPicture->setPixmap(red);
        return;
    }
    else if(heartwave->currentSession->coheranceRating > 1 && heartwave->currentSession->coheranceRating <= 2){
        ui->lightPicture->setPixmap(yellow);
        return;
    }
    else if(heartwave->currentSession->coheranceRating > 1){
        ui->lightPicture->setPixmap(green);
        return;
    }
    else
    {
        ui->lightPicture->setPixmap(noLight);
    }
}




MainWindow::~MainWindow()
{
    delete ui;
}





void MainWindow::timerEvent(QTimerEvent *event)
{
    if (heartwave != nullptr) {
        heartwave->update();
        if ((this->heartwave->getActivePulseReading() == true)){
            updateGraph();
        }

        // Breath Pacer UI.
        ui->breathPaceIndicator->setValue(heartwave->breathPacer->currentPosition);

        // Battery UI.
        if (!heartwave->battery->isLow) {
            ui->lowBatteryLabel->setVisible(false);
        } else {
            ui->lowBatteryLabel->setVisible(true);
        }
        ui->batteryLabel->setNum(heartwave->battery->getBatteryLevel());

        if(heartwave->currentSession != nullptr) {
            if(heartwave->currentSession->started) {
                // Status display UI.
                ui->coheranceLabel->setNum(heartwave->currentSession->coheranceRating);
                ui->achievmentLabel->setNum(heartwave->currentSession->achievementScore);
                ui->lengthLabel->setNum(heartwave->currentSession->clock);
            } else {
                ui->coheranceLabel->setNum(0.0);
                ui->achievmentLabel->setNum(0.0);
                ui->lengthLabel->setNum(0.0);
            }
        }


        if(heartwave->currentSession != nullptr) {
            if(heartwave->currentSession->ended && heartwave->getActivePulseReading() == true) {
                ui->summary->setVisible(true);
                ui->summary->setPlainText(QString::fromStdString(heartwave->currentSession->generateSummary()));

            } else {
                ui->summary->setVisible(false);
            }
        }

    }
}

void MainWindow::initializeMainMenu(Menu* m) {

    Menu* settings = new Menu("SETTINGS", {"BREATH PACER"}, m);
    Menu* sessions = new Menu("SELECT SESSION", {"START SESSION 1", "START SESSION 2", "START SESSION 3"}, m);
    Menu* history = new Menu("LOG/HISTORY", {"VIEW","CLEAR"}, m);

    m->addChildMenu(settings);
    m->addChildMenu(sessions);
    m->addChildMenu(history);
    Menu* pacer = new Menu("BREATH PACER", {"CHANGE IN DROP BOX"}, settings);
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
        ui->breathPaceComboBox->setVisible(true);
        return;
    }
    if(masterMenu->getName() == "SELECT SESSION"){
        currentGraphSecond = 0;

        if(masterMenu->getMenuItems()[index] == "START SESSION 1"){
            initGraph();
            ui->summary->setVisible(false);
            this->heartwave->setCurrentSession(1);
            this->heartwave->currentSession->start();
            this->heartwave->setActivePulseReading(true);
            ui->graph->setVisible(true);
            qInfo("Session 1"); // GRAPH 1
        }
        else if (masterMenu->getMenuItems()[index] == "START SESSION 2"){
            initGraph();
            ui->summary->setVisible(false);
            this->heartwave->setCurrentSession(2);
            this->heartwave->currentSession->start();
            this->heartwave->setActivePulseReading(true);
            ui->graph->setVisible(true);
            qInfo("Session 2"); // GRAPH 2
        }
        else if (masterMenu->getMenuItems()[index] == "START SESSION 3"){\
            initGraph();
            ui->summary->setVisible(false);
            this->heartwave->setCurrentSession(3);
            this->heartwave->currentSession->start();
            this->heartwave->setActivePulseReading(true);
            ui->graph->setVisible(true);
            qInfo("Session 3"); // GRAPH 3
        }
    }

<<<<<<< Updated upstream
//    if(masterMenu->getName() == "START SESSION 1") {
//        this->heartwave->setActivePulseReading(true);
//        endOfGraph();
//        heartwave->setCurrentSession(1);
//        if(masterMenu->getMenuItems()[index] == "Currently running session 1 (click to end)") {
//            ui->summary->setPlainText("Summary 1");
//            ui->summary->show();
//            ui->summary->setVisible(true);
//           endOfGraph();
//            qInfo("This is where end of graph 1 should run"); // end of graph function should run here for graph 1
//            heartwave->currentSession->interruptSession();
//        }
//    }
//    if(masterMenu->getName() == "START SESSION 2") {
//        this->heartwave->setActivePulseReading(true);
//        endOfGraph();
//        heartwave->setCurrentSession(2);
//        if(masterMenu->getMenuItems()[index] == "Currently running session 2 (click to end)") {
//            ui->summary->setPlainText("Summary 2");
//            ui->summary->show();
//            ui->summary->setVisible(true);
//            qInfo("This is where end of graph 2 should run"); // end of graph function should run here for graph 2
//            heartwave->currentSession->interruptSession();
//        }
//    }
//    else if(masterMenu->getName() == "START SESSION 3") {
//        this->heartwave->setActivePulseReading(true);
//        endOfGraph();
//        heartwave->setCurrentSession(3);
//        if(masterMenu->getMenuItems()[index] == "Currently running session 3 (click to end)") {
//            ui->summary->setPlainText("Summary 3");
//            ui->summary->show();
//            ui->summary->setVisible(true);
//            qInfo("This is where end of graph 3 should run"); // end of graph function should run here for graph 3
//            heartwave->currentSession->interruptSession();
//        }
//    }


=======
    if(masterMenu->getName() == "START SESSION 1") {
        initGraph();
        this->heartwave->setActivePulseReading(true);
//        endOfGraph();
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
        initGraph();
        this->heartwave->setActivePulseReading(true);
//        endOfGraph();
        heartwave->setCurrentSession(2);
        if(masterMenu->getMenuItems()[index] == "Currently running session 2 (click to end)") {
            ui->summary->setPlainText("Summary 2");
            ui->summary->show();
            ui->summary->setVisible(true);
            endOfGraph();
            qInfo("This is where end of graph 2 should run"); // end of graph function should run here for graph 2
            heartwave->currentSession->interruptSession();
        }
    }
    else if(masterMenu->getName() == "START SESSION 3") {
        initGraph();
        this->heartwave->setActivePulseReading(true);
//        endOfGraph();
        heartwave->setCurrentSession(3);
        if(masterMenu->getMenuItems()[index] == "Currently running session 3 (click to end)") {
            ui->summary->setPlainText("Summary 3");
            ui->summary->show();
            ui->summary->setVisible(true);
            qInfo("This is where end of graph 3 should run"); // end of graph function should run here for graph 3
            heartwave->currentSession->interruptSession();
            endOfGraph();
        }
    }
>>>>>>> Stashed changes

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

            this->heartwave->setActivePulseReading(true);


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


void MainWindow::initGraph()
{
    ui->graph->clearGraphs();
    ui->graph->clearItems();
    ui->graph->addGraph(0);
    ui->graph->yAxis->setRange(40,200);
    ui->graph->xAxis->setRange(0,30);
    ui->graph->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->graph->replot();
}




void MainWindow::navigateBack() {
    ui->breathPaceComboBox->setVisible(false);
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

    qInfo()<< "Clearing graph data";

    heartwave->setActivePulseReading(false);

    heartwave->currentSession->stop();

    initGraph();
}

void MainWindow::on_batteryReplaceButton_clicked()
{
    this->heartwave->replaceBattery();
}

void MainWindow::on_breathPaceComboBox_currentIndexChanged(int index)
{
    this->heartwave->breathPacer->changeFrequency(index+1);
    qInfo()<<this->heartwave->breathPacer->frequency;
}
