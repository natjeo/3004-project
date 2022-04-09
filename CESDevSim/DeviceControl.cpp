#include "DeviceControl.h"
#include "ui_DeviceControl.h"
#include <QDebug>
#include <QGraphicsTextItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = new DataBase();
    this->therapy = new Therapy(1, "MET", 45, 3);
    ui->historyList->setVisible(false);

    this->user = db->getUser(0);

    // test database stuff -  remove later
        user = db->getUser(0);

        Therapy* therapy = new Therapy(1, "MET", 45, 2);
        if (db->addTherapyRecord(therapy)){
            qInfo("therapy added succ");
        }

        QList<Therapy*> therapyHistory = db->getTherapyRecords();
        qDebug() << therapyHistory.length();
        //qDebug() << therapyHistory.first()->getSession();

        //QString name = user->getName();
        //printf(name.toLatin1());

    connect(ui->btn_save, &QPushButton::pressed, this, &MainWindow::recordTherapy);
    connect(ui->btn_home, &QPushButton::pressed, this, &MainWindow::displayHistory);
    connect(ui->btn_up, &QPushButton::pressed, this, &MainWindow::pressUp);
    connect(ui->btn_dn, &QPushButton::pressed, this, &MainWindow::pressDn);
    connect(ui->btn_ok, &QPushButton::pressed, this, &MainWindow::selectPressed);
    connect(ui->btn_ok, &QPushButton::released, this, &MainWindow::selectReleased);

//    connect(ui->btn, &QPushButton::pressed, this, &MainWindow::selectMET);
//    connect(ui->btn, &QPushButton::pressed, this, &MainWindow::selectSubDelta);
//    connect(ui->btn, &QPushButton::pressed, this, &MainWindow::selectDelta);
//    connect(ui->btn, &QPushButton::pressed, this, &MainWindow::selectTheta);
//    connect(ui->btn, &QPushButton::pressed, this, &MainWindow::selectAlpha);
//    connect(ui->btn, &QPushButton::pressed, this, &MainWindow::selectSMR);
//    connect(ui->btn, &QPushButton::pressed, this, &MainWindow::selectBeta);
//    connect(ui->btn, &QPushButton::pressed, this, &MainWindow::select100Hz);


//    connect(ui->btn, &QPushButton::pressed, this, &MainWindow::select20min);
//    connect(ui->btn, &QPushButton::pressed, this, &MainWindow::select45min);
//    connect(ui->btn, &QPushButton::pressed, this, &MainWindow::select3h);
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recordTherapy(){
    ui->historyList->setVisible(false);
    qDebug() << this->therapy->getSession();
    if (db->addTherapyRecord(this->therapy)) {
        qInfo("therapy recorded succ");
        QGraphicsScene* scene = new QGraphicsScene;
        scene->addText("therapy saved succ");
        ui->table_menu->setScene(scene);
    }
}

void MainWindow::displayHistory(){
    ui->table_menu->scene();
    ui->historyList->clear();
    QList<Therapy*> therapyList = db->getTherapyRecords();

    for (int i = 0; i < therapyList.length(); i++) {
        QString info = "#:" + QString::number(i+1)+ " Duration " + QString::number(therapyList.at(i)->getDuration()) + "; Session type: " + (therapyList.at(i)->getSession()) + "; Intensity: " + QString::number(therapyList.at(i)->getIntensity());
        ui->historyList->addItem(info);
    }

    ui->historyList->setVisible(true);
}

void MainWindow::pressUp()
{
    int intensity = this->therapy->getIntensity();
    if (intensity < 8){
        this->therapy->setIntensity(intensity+1);
    }
    qDebug() << this->therapy->getIntensity();
    qInfo("up");
}

void MainWindow::pressDn()
{
    int intensity = this->therapy->getIntensity();
    if (intensity > 1){
        this->therapy->setIntensity(intensity-1);
    }
    qDebug() << this->therapy->getIntensity();
    qInfo("down");
}

void MainWindow::on_btn_power_clicked()
{

}

void MainWindow::updatePreferences(){
    qDebug() << db->getPreference("MET");
    if (db->updatePreference(this->therapy->getSession(), this->therapy->getIntensity())) {
        qInfo("preferences updated succ");
        QString message = "Preferences updated";

       qDebug() << db->getPreference("MET");
        displayMessage(message);
    }
}


void MainWindow::displayMessage(QString message){
        QTimer::singleShot(2000, this, &MainWindow::cleanMessage);
        ui->historyList->setVisible(false);
        QGraphicsScene* scene = new QGraphicsScene;
        scene->addText(message);
        ui->table_menu->setScene(scene);
}

void MainWindow::selectPressed(){
    elapsedTimer.start();
}

void MainWindow::cleanMessage(){
    ui->table_menu->scene()->clear();
    ui->historyList->clear();
}

void MainWindow::selectReleased(){
    int timeMilliSecs = elapsedTimer.elapsed();
    if (timeMilliSecs >= 1000) {
        updatePreferences();
    }
}

void MainWindow::selectMET(){
    this->therapy->setSession("MET");
}

void MainWindow::selectSubDelta(){
    this->therapy->setSession("Sub-Delta");
}

void MainWindow::selectDelta(){
    this->therapy->setSession("Delta");
}

void MainWindow::selectTheta(){
    this->therapy->setSession("Theta");
}

void MainWindow::selectAlpha(){
    this->therapy->setSession("Alpha");
}

void MainWindow::selectSMR(){
    this->therapy->setSession("SMR");
}

void MainWindow::selectBeta(){
    this->therapy->setSession("Beta");
}

void MainWindow::select100Hz(){
    this->therapy->setSession("100Hz");
}

void MainWindow::select20min(){
    this->therapy->setDuration(20);
}

void MainWindow::select45min(){
    this->therapy->setDuration(45);
}

void MainWindow::select3h(){
    this->therapy->setDuration(180);
}

