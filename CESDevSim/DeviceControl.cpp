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
    qDebug() << (user->getBatteryLvl());
    db->updateBatteryLvl(0, 50);
    user = db->getUser(0);

    qDebug() << (user->getBatteryLvl());
    //QString name = user->getName();
    //printf(name.toLatin1());
}

MainWindow::~MainWindow()
{
    delete ui;
}

