#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QStringList>
#include "DataBase.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateTimer();
    void navigateMenu();
    void electrodesPlaced(bool);
    void select();

private:
    Ui::MainWindow *ui;

    QPushButton powerOn;
    QPushButton selectUp;
    QPushButton slectDn;
    QPushButton record;
    QPushButton startSession;
    QStringList historyMenu;
    QLabel powerLED;
    QLabel batteryLevelIndicator;
    QLabel sessionIndicator;
    DataBase* db;

};
#endif // MAINWINDOW_H