#include "DeviceControl.h"
#include "ui_DeviceControl.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = new DataBase();
    this->therapy = new Therapy("MET", 45, 3);

    User* user = db->getUser(0);

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

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::recordTherapy(){
    qDebug() << this->therapy->getSession();
    if (db->addTherapyRecord(this->therapy)) {
        qInfo("therapy recorded succ");
    }

    // just testing - remove later
    QList<Therapy*> therapyHistory = db->getTherapyRecords();
    qDebug() << therapyHistory.length();

}

