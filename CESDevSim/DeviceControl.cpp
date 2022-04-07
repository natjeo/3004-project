#include "DeviceControl.h"
#include "ui_DeviceControl.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = new DataBase();
    User* user = db->getUser(0);
    QString name = user->getName();
    printf(name.toLatin1());
}

MainWindow::~MainWindow()
{
    delete ui;
}

