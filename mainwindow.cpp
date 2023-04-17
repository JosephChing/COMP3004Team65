#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include "mainwindow.h"

int currentGraphSecond = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), timerID(0), power(false),prevCoheranceLevel(-1), needPlayBeep(false)
{
    ui->setupUi(this);

    ui->graph->setVisible(false);
    ui->summary->setVisible(false);
    ui->summaryarray->setVisible(false);
    this->heartwave = new Heartwave;



    initGraph();
    heartwave = new Heartwave();
    ui->mainMenuListView->setStyleSheet("background-color: black");
    ui->breathPaceComboBox->setVisible(false);

    masterMenu = new Menu("MAIN MENU", {"SETTINGS","SELECT SESSION","LOG/HISTORY"}, nullptr);
    initializeInterface(masterMenu);




}


void MainWindow::initializeInterface(Menu* m) {

    // Initial button states.
    ui->batteryLabel->setNum(100);
    ui->upButton->setEnabled(false);
    ui->downButton->setEnabled(false);
    ui->selectButton->setEnabled(false);
    ui->menuButton->setEnabled(false);
    ui->returnButton->setEnabled(false);
    ui->leftButton->setEnabled(false);
    ui->rightButton->setEnabled(false);

    // Slot connections.
    connect(ui->upButton, &QPushButton::pressed, this, &MainWindow::navigateUpMenu);
    connect(ui->downButton, &QPushButton::pressed, this, &MainWindow::navigateDownMenu);
    connect(ui->selectButton, &QPushButton::pressed, this, &MainWindow::navigateSubMenu);
    connect(ui->menuButton, &QPushButton::pressed, this, &MainWindow::navigateToMainMenu);
    connect(ui->returnButton, &QPushButton::pressed, this, &MainWindow::navigateBack);
    connect(ui->rightButton, &QPushButton::pressed,  this, &MainWindow::navigateRightMenu);

    // Menu Element Setups.

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
    Menu* shutDown = new Menu("",{},nullptr);
    sessions->addChildMenu(startSession1);
    sessions->addChildMenu(startSession2);
    sessions->addChildMenu(startSession3);
    startSession1->addChildMenu(endSession1);
    startSession2->addChildMenu(endSession2);
    startSession3->addChildMenu(endSession3);
    Menu* viewHistory = new Menu("VIEW", {}, history);
    Menu* clearHistory = new Menu("CLEAR", {"YES","NO"}, history);
    history->addChildMenu(viewHistory);
    history->addChildMenu(clearHistory);



}



//This functions allows the user to turn on the device and be ready for use
void MainWindow::turnOnDevice()
{
    ui->mainMenuListView->setStyleSheet("background-color: white");
    ui->summary->setStyleSheet("background-color: white");
    ui->graph->setStyleSheet("background-color: white");
    ui->mainMenuListView->clear();
    mainMenu = masterMenu;

    killTimer(timerID);
    this->timerID = startTimer(300);





    activeQListWidget = ui->mainMenuListView;
    activeQListWidget->addItems(masterMenu->getMenuItems());
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(masterMenu->getName());
    ui->breathPaceComboBox->setVisible(false);

    heartwave->setActivePulseReading(false);

    // Enable all buttons

    ui->upButton->setEnabled(true);
    ui->downButton->setEnabled(true);
    ui->selectButton->setEnabled(true);
    ui->menuButton->setEnabled(true);
    ui->returnButton->setEnabled(true);
    ui->leftButton->setEnabled(true);
    ui->rightButton->setEnabled(true);

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
    this->heartwave->setActivePulseReading(false);
    ui->graph->replot();
    QPixmap  noLight(":/lightsPictures/noLights.png");
}


void MainWindow::updateLight()
{
    QPixmap  noLight(":/lightsPictures/noLights.png");
    QPixmap  red(":/lightsPictures/redLight.png");
    QPixmap  yellow(":/lightsPictures/yellowLight.png");
    QPixmap  green(":/lightsPictures/greenLight.png");

    if(heartwave->currentSession == nullptr || heartwave->getActivePulseReading()==false || heartwave->currentSession->getCoheranceRating()==0) {
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

void MainWindow::updateBeep() {
    if(heartwave->currentSession->getCoheranceRating() != prevCoheranceLevel) {
        needPlayBeep = true;
        prevCoheranceLevel = heartwave->currentSession->getCoheranceRating();
    }
    if (needPlayBeep) {
        QApplication::beep();
        qInfo() << "Beep! New coherance level: " << heartwave->currentSession->getCoheranceRating();
        needPlayBeep = false;
    }

}




void MainWindow::timerEvent(QTimerEvent *event)
{
    updateLight();
    updateBeep();

    if(heartwave->battery->getBatteryLevel()<=0){
        shutOffDevice();
        ui->batteryLabel->setText("0");
        return;
    }
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
                ui->summary->setPlainText(heartwave->currentSession->generateSummary());

            } else {
                ui->summary->setVisible(false);
            }


        }
        if(heartwave->getActivePulseReading()) {
            // Heartwave monitor connected UI.
            QPixmap  red(":/lightsPictures/redHeart.png");
            ui->monitorPicture->setPixmap(red);
        } else {
            QPixmap  grey(":/lightsPictures/greyHeart.png");
            ui->monitorPicture->setPixmap(grey);
        }

    }
}
//This function allows the user to use the up button to navigate the menu
void MainWindow::navigateUpMenu() {

    int nextIndex = activeQListWidget->currentRow() - 1;
    qDebug() << nextIndex;

    if (nextIndex < 0) {
        nextIndex = activeQListWidget->count() - 1;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}
void MainWindow::navigateRightMenu() {
    qDebug() << activeQListWidget->currentRow();
    qDebug() << masterMenu->getName();
}
//This functiona llows the user to use the down button to navigate the menu
void MainWindow::navigateDownMenu() {

    int nextIndex = activeQListWidget->currentRow() + 1;

    if (nextIndex > activeQListWidget->count() - 1) {
        nextIndex = 0;
    }

    activeQListWidget->setCurrentRow(nextIndex);
}
//This function updates the menu to the screen
void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems) {

    activeQListWidget->clear();
    activeQListWidget->addItems(menuItems);
    activeQListWidget->setCurrentRow(0);

    ui->menuLabel->setText(selectedMenuItem);
}



void MainWindow::navigateSubMenu() {
    int index = activeQListWidget->currentRow();
    if (masterMenu->getMenuItems()[index] == "VIEW") {
        ui->summaryarray->setVisible(true);
        qInfo()<<"hello";
        return;
    }



    if (index < 0) return;
    //prevent crash if ok button selected

    if(masterMenu->getName() == "BREATH PACER") {
        ui->breathPaceComboBox->setVisible(true);
        return;
    }
    //if statements to run the correct session that the user selects
    if(masterMenu->getName() == "SELECT SESSION"){
        currentGraphSecond = 0;

        if(masterMenu->getMenuItems()[index] == "START SESSION 1"){
            initGraph();
            ui->summary->setVisible(false);
            this->heartwave->setCurrentSession(1);
            this->heartwave->currentSession->start();
            this->heartwave->setActivePulseReading(true);
            ui->graph->setVisible(true);
        }
        else if (masterMenu->getMenuItems()[index] == "START SESSION 2"){
            initGraph();
            ui->summary->setVisible(false);
            this->heartwave->setCurrentSession(2);
            this->heartwave->currentSession->start();
            this->heartwave->setActivePulseReading(true);
            ui->graph->setVisible(true);
        }
        else if (masterMenu->getMenuItems()[index] == "START SESSION 3"){\
            initGraph();
            ui->summary->setVisible(false);
            this->heartwave->setCurrentSession(3);
            this->heartwave->currentSession->start();
            this->heartwave->setActivePulseReading(true);
            ui->graph->setVisible(true);
        }
    }
    //if statements to end the session when user desires to
    if(masterMenu->getName() == "START SESSION 1") {
        this->heartwave->setActivePulseReading(true);
        heartwave->setCurrentSession(1);
        if(masterMenu->getMenuItems()[index] == "Currently running session 1 (click to end)") {
            heartwave->currentSession->stop();
            heartwave->currentSession->interruptSession();

            ui->summaryarray->setText(ui->summaryarray->toPlainText() += "------------------\n");
            ui->summaryarray->setText(ui->summaryarray->toPlainText() += heartwave->currentSession->generateSummary());
        }
    }
    else if(masterMenu->getName() == "START SESSION 2") {
        this->heartwave->setActivePulseReading(true);
        heartwave->setCurrentSession(2);
        if(masterMenu->getMenuItems()[index] == "Currently running session 2 (click to end)") {
            heartwave->currentSession->stop();
            heartwave->currentSession->interruptSession();

            ui->summaryarray->setText(ui->summaryarray->toPlainText() += "------------------\n");
            ui->summaryarray->setText(ui->summaryarray->toPlainText() += heartwave->currentSession->generateSummary());
        }
    }
    else if(masterMenu->getName() == "START SESSION 3") {
        this->heartwave->setActivePulseReading(true);
        heartwave->setCurrentSession(3);
        if(masterMenu->getMenuItems()[index] == "Currently running session 3 (click to end)") {
            heartwave->currentSession->stop();
            heartwave->currentSession->interruptSession();

            ui->summaryarray->setText(ui->summaryarray->toPlainText() += "------------------\n");
            ui->summaryarray->setText(ui->summaryarray->toPlainText() += heartwave->currentSession->generateSummary());
        }
    }

    //allows user to go into clear menu, and decide if they want to clear the session history if user desires
    if (masterMenu->getName() == "CLEAR") {
        ui->summaryarray->setText("");
        //        if (masterMenu->getMenuItems()[index] == "YES") {
//            ui->summaryarray->setText("");
//            return;
//        }
//        else {
//            navigateBack();
            return;
   }


    //start heartwave session
    if (masterMenu->get(index)->getMenuItems().length() > 0) {
        masterMenu = masterMenu->get(index);
        if(masterMenu->getName() == "SELECT SESSION") {
            this->heartwave->setActivePulseReading(true);
        }
        MainWindow::updateMenu(masterMenu->getName(), masterMenu->getMenuItems());
    }

    else if (masterMenu->get(index)->getName() == "VIEW") {
        masterMenu = masterMenu->get(index);
        ui->summaryarray->setVisible(true);
        //MainWindow::updateMenu("LOG/HISTORY", sessionsArray); -> this lists the sessions array
    }
}
//this function allows the user to get back to the main menu from whichever menu they are currently in2
void MainWindow::navigateToMainMenu() {
    ui->summaryarray->setVisible(false);
    ui->breathPaceComboBox->setVisible(false);
    //checks to see if user is already in main menu
    if(masterMenu->getName() == "MAIN MENU") {
        activeQListWidget->setCurrentRow(0);
        return;
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
    heartwave->setActivePulseReading(false);

    heartwave->currentSession->stop();

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
    ui->summaryarray->setVisible(false);
    ui->breathPaceComboBox->setVisible(false);
    if(masterMenu->getName() == "MAIN MENU") {
        return;
    }
    ui->rightButton->blockSignals(true);
    ui->leftButton->blockSignals(true);
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
    ui->batteryLabel->setNum(heartwave->battery->getBatteryLevel());
    ui->lowBatteryLabel->setVisible(false);
}

void MainWindow::on_breathPaceComboBox_currentIndexChanged(int index)
{
    this->heartwave->breathPacer->changeFrequency(index+1);
    qInfo()<<this->heartwave->breathPacer->frequency;
}



void MainWindow::on_offButton_clicked()
{

    if(power == false){
            turnOnDevice();
            this->power =true;
    }else{
        shutOffDevice();
    }
}



void MainWindow::shutOffDevice()
{
    this->killTimer(timerID);

    //set screens black
    ui->mainMenuListView->setStyleSheet("background-color: black;color: black; selection-background-color: black; selection-color:black");
    ui->summary->setStyleSheet("background-color: black");

    this->power = false;


    //disable buttons
    ui->upButton->setEnabled(false);
    ui->downButton->setEnabled(false);
    ui->selectButton->setEnabled(false);
    ui->menuButton->setEnabled(false);
    ui->returnButton->setEnabled(false);
    ui->leftButton->setEnabled(false);
    ui->rightButton->setEnabled(false);
}

void MainWindow::on_disconnectButton_clicked()
{
    heartwave->setActivePulseReading(false);
}

void MainWindow::on_reconnectButton_clicked()
{
    heartwave->setActivePulseReading(true);
}
