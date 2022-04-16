#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QStringList>
#include <QElapsedTimer>
#include <QTimer>
#include <QButtonGroup>
#include <QThread>
#include <QTime>
#include <QRandomGenerator>

#include "DataBase.h"
#include "Battery.h"

#define BATTERY_DISPLAY_INTERVAL 10000

#define EXCELLENT_CONNECTION 1
#define OKAY_CONNECTION 2

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void displayHistory();

private:
    Ui::MainWindow *ui;

    DataBase* db;
    Therapy* therapy;
    User* user;

    void updateTimer();
    void navigateMenu();
    void electrodesPlaced(bool);
    void pressUp();
    void pressDn();
    void savePreference();
    void recordTherapy();
    void updatePreferences();
    void displayMessage(QString);
    void selectPressed(); // this is a temporary name we need a better one
    void selectReleased(); // this is a temporary name we need a better one
    void delayPower();
    void cleanMessage();
    void indicateBatteryLevel();
    void displayBatteryLevel(int, bool = false);
    void displayHomeScreen();

    void updatePowerState();
    void powerBtnPressed();
    void selectDuration(int);
    void selectSession(int);
    void updateSessionTimer();
    void stopSession();	
    void illuminateGraphBar(int);
    void darkenGraphBar(int);
    void flashGraphBar(int, int, int = 1);
    int performConnectionTest();
    void delay(int);
    void autoPower();
    void powerBtnReleased();

    Battery* battery;
    QTimer *batteryDisplayTimer;
    QElapsedTimer elapsedTimer;
    QButtonGroup* selDur;
    QButtonGroup* selSes;
    QVector<QLabel*>* graphBars;
    bool powerState;
    bool isAdjustingIntensity;
    bool isRunningTest;
    QTimer *sessionTimer;
    int sessionTime;
    bool sessionInProgress;
    int autoShutOffTime = 12000;
QTimer* intervalTimer;
QLabel* modeLight;
QString modeLightOnStyle;
QString modeLightOffStyle;
bool readyToStartSession;

    bool eventFilter(QObject *obj, QEvent *event) override;
    QTimer* userInactive;
};
#endif // MAINWINDOW_H
