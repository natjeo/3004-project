#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QStringList>
#include <QElapsedTimer>
#include <QTimer>
#include "DataBase.h"
#include "Battery.h"

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
    void pressUp();
    void pressDn();
    void savePreference();
    void recordTherapy();
    void displayHistory();
    void updatePreferences();
    void displayMessage(QString);
    void selectPressed(); // this is a temporary name we need a better one
    void selectReleased(); // this is a temporary name we need a better one
    void cleanMessage();
		void indicateBatteryLevel();
		void displayBatteryLevel(int, bool = false);

private slots:
    void on_btn_power_clicked();

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
    Therapy* therapy;
    User* user;
    Battery* battery;
		QTimer *batteryDisplayTimer;
    QElapsedTimer elapsedTimer;

};
#endif // MAINWINDOW_H
