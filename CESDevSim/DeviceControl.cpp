#include "DeviceControl.h"
#include "ui_DeviceControl.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // setup database for user
    db = new DataBase();
    User* user = db->getUser(0);
    qDebug() << (user->getPreferences());
    db->updatePreferences(0, 5);
    user = db->getUser(0);

    // setup therapy
    Therapy* therapy = new Therapy("MET", 45, 2);
    if (db->addTherapyRecord(therapy)){
        qInfo("therapy added succ");
    }

    qDebug() << (user->getPreferences());

    QList<Therapy*> therapyHistory = db->getTherapyRecords();
    qDebug() << therapyHistory.length();
    qDebug() << therapyHistory.first()->getSession();

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
