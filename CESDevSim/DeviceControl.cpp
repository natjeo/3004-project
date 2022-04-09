#include "DeviceControl.h"
#include "ui_DeviceControl.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = new DataBase();
    User* user = db->getUser(0);
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
    qDebug() << therapyHistory.first()->getSession();

    //QString name = user->getName();
    //printf(name.toLatin1());
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_power_clicked()
{

}
