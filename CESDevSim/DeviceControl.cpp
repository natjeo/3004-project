#include "DeviceControl.h"
#include "ui_DeviceControl.h"
#include <QDebug>
#include <QGraphicsTextItem>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setup database for user
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

    //QString name = user->getName();
    //printf(name.toLatin1());

    // setup device power status
    powerState = false;
    updatePowerState();
    connect(ui->btn_power, &QPushButton::released, this, &MainWindow::powerBtnPressed);
}

void MainWindow::powerBtnPressed()
{
    powerState  = !powerState;
    updatePowerState();

    sesDur = new QButtonGroup();
    sesDur->setId(ui->dur_20min, 1);
    sesDur->addButton(ui->dur_20min);
    sesDur->setId(ui->dur_45min, 2);
    sesDur->addButton(ui->dur_45min);
    sesDur->setExclusive(true);
    connect(sesDur, static_cast<void(QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked), this, &MainWindow::selectDuration);
//    connect(sesDur, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), [=](QAbstractButton *button){
//        qDebug() << sesDur->id(button);
//    });
}

void MainWindow::selectDuration()
{
    qDebug() << "test";
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

void MainWindow::updatePowerState()
{
    ui->dur_20min->setVisible(powerState);
    ui->dur_45min->setVisible(powerState);
    ui->dur_3hrs->setVisible(powerState);
    ui->dur_custom->setVisible(powerState);

    ui->mode_L->setVisible(powerState);
    ui->mode_R->setVisible(powerState);
    ui->connection_CES->setVisible(powerState);
    ui->connection_audio->setVisible(powerState);

    ui->batteryLevel->setVisible(powerState);
    ui->skin_menu->setVisible(powerState);
    ui->table_menu->setVisible(powerState);

    ui->ses_100Hz->setVisible(powerState);
    ui->ses_alpha->setVisible(powerState);
    ui->ses_beta1->setVisible(powerState);
    ui->ses_beta2->setVisible(powerState);
    ui->ses_delta->setVisible(powerState);
    ui->ses_met->setVisible(powerState);
    ui->ses_subDelta->setVisible(powerState);
    ui->ses_theta->setVisible(powerState);

    ui->extra_1->setVisible(powerState);
    ui->extra_2->setVisible(powerState);
    ui->extra_025->setVisible(powerState);
    ui->extra_050->setVisible(powerState);
    ui->extra_075->setVisible(powerState);
    ui->extra_100->setVisible(powerState);
    ui->extra_150->setVisible(powerState);
    ui->extra_200->setVisible(powerState);
    ui->extra_tdcs->setVisible(powerState);

    ui->bar_1->setVisible(powerState);
    ui->bar_2->setVisible(powerState);
    ui->bar_3->setVisible(powerState);
    ui->bar_4->setVisible(powerState);
    ui->bar_5->setVisible(powerState);
    ui->bar_6->setVisible(powerState);
    ui->bar_7->setVisible(powerState);
    ui->bar_8->setVisible(powerState);

    if (powerState){
        ui->powerLED->setStyleSheet("image: url(:/icons/power_on.png);");
    } else {
        ui->powerLED->setStyleSheet("image: url(:/icons/power_off.png);");
    }
}


MainWindow::~MainWindow()
{
    delete ui;
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
