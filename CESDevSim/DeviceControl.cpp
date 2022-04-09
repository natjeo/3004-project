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
    this->therapy = new Therapy("MET", 45, 3);
    ui->historyList->setVisible(false);

    this->user = db->getUser(0);

    // test database stuff -  remove later
        qDebug() << (user->getPreferences());
        db->updatePreferences(0, 5);
        user = db->getUser(0);

        Therapy* therapy = new Therapy("MET", 45, 2);
        if (db->addTherapyRecord(therapy)){
            qInfo("therapy added succ");
        }

        qDebug() << (user->getPreferences());

        QList<Therapy*> therapyHistory = db->getTherapyRecords();
        qDebug() << therapyHistory.length();
        //qDebug() << therapyHistory.first()->getSession();

        //QString name = user->getName();
        //printf(name.toLatin1());

    connect(ui->btn_save, &QPushButton::pressed, this, &MainWindow::recordTherapy);
    connect(ui->btn_home, &QPushButton::pressed, this, &MainWindow::displayHistory);
    connect(ui->btn_ok, &QPushButton::pressed, this, &MainWindow::selectPressed);
    connect(ui->btn_ok, &QPushButton::released, this, &MainWindow::selectReleased);

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

void MainWindow::on_btn_power_clicked()
{

}

void MainWindow::updatePreferences(){
    if (db->updatePreferences(this->user->getID(), this->therapy->getIntensity())) {
        qInfo("preferences updated succ");
        QString message = "Preferences updated";
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
