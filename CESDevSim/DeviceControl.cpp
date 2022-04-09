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

				//this->battery = new Battery(user->getBatteryLvl());
				this->battery = new Battery;
				this->batteryDisplayTimer = new QTimer(this);
				connect(this->batteryDisplayTimer, &QTimer::timeout, this, &MainWindow::indicateBatteryLevel);
				this->batteryDisplayTimer->start(5000);

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

    // group duration button selections
    selDur = new QButtonGroup();
    selDur->addButton(ui->dur_20min);
    selDur->setId(ui->dur_20min, 1);
    selDur->addButton(ui->dur_45min);
    selDur->setId(ui->dur_45min, 2);
    selDur->addButton(ui->dur_3hrs);
    selDur->setId(ui->dur_3hrs, 3);
    selDur->addButton(ui->dur_custom);
    selDur->setId(ui->dur_custom, 4);
    selDur->setExclusive(true);
    connect(selDur, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), [=](QAbstractButton* button){
          button->setChecked(true);
          selectDuration(selDur->checkedId());
    });

    // group session button selections
    selSes = new QButtonGroup();
    selSes->addButton(ui->ses_met);
    selSes->setId(ui->ses_met, 1);
    selSes->addButton(ui->ses_subDelta);
    selSes->setId(ui->ses_subDelta, 2);
    selSes->addButton(ui->ses_delta);
    selSes->setId(ui->ses_delta, 3);
    selSes->addButton(ui->ses_theta);
    selSes->setId(ui->ses_theta, 4);
    selSes->addButton(ui->ses_alpha);
    selSes->setId(ui->ses_alpha, 5);
    selSes->addButton(ui->ses_beta1);
    selSes->setId(ui->ses_beta1, 6);
    selSes->addButton(ui->ses_beta2);
    selSes->setId(ui->ses_beta2, 7);
    selSes->addButton(ui->ses_100Hz);
    selSes->setId(ui->ses_100Hz, 8);
    connect(selSes, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), [=](QAbstractButton* button){
          button->setChecked(true);
          selectSession(selSes->checkedId());
    });
}

void MainWindow::selectSession(int btnId)
{
    ui->ses_met->setStyleSheet("border-image: url(:/icons/MET.png);");
    ui->ses_subDelta->setStyleSheet("border-image: url(:/icons/Sub-Delta.png);");
    ui->ses_delta->setStyleSheet("border-image: url(:/icons/Delta.png);");
    ui->ses_theta->setStyleSheet("border-image: url(:/icons/Theta.png);");
    ui->ses_alpha->setStyleSheet("border-image: url(:/icons/Alpha.png);");
    ui->ses_beta1->setStyleSheet("border-image: url(:/icons/Beta 1.png);");
    ui->ses_beta2->setStyleSheet("border-image: url(:/icons/Beta 2.png);");
    ui->ses_100Hz->setStyleSheet("border-image: url(:/icons/100Hz.png);");

    switch (btnId){
        case 1:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/MET_on.png);");
            break;

        case 2:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/Sub-Delta_on.png);");
            break;

        case 3:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/Delta_on.png);");
            break;

        case 4:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/Theta_on.png);");
            break;

        case 5:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/Alpha_on.png);");
            break;

        case 6:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/Beta 1_on.png);");
            break;

        case 7:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/Beta 2_on.png);");
            break;

        case 8:
            selSes->checkedButton()->setStyleSheet("border-image: url(:/icons/100Hz_on.png);");
            break;

    }
}

void MainWindow::selectDuration(int btnId)
{
    ui->dur_20min->setStyleSheet("border-image: url(:/icons/20min Session.png);");
    ui->dur_45min->setStyleSheet("border-image: url(:/icons/45min Session.png);");
    ui->dur_3hrs->setStyleSheet("border-image: url(:/icons/3hr Session.png);");
    ui->dur_custom->setStyleSheet("border-image: url(:/icons/User Session.png);");

    switch (btnId){
        case 1:
            selDur->checkedButton()->setStyleSheet("border-image: url(:/icons/20min Session_on.png);");
            break;

        case 2:
            selDur->checkedButton()->setStyleSheet("border-image: url(:/icons/45min Session_on.png);");
            break;

        case 3:
            selDur->checkedButton()->setStyleSheet("border-image: url(:/icons/3hr Session_on.png);");
            break;

        case 4:
            selDur->checkedButton()->setStyleSheet("border-image: url(:/icons/Sessions_on.png);");
            break;
    }
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
  	this->db->updateBatteryLvl(this->user->getID(), this->battery->getLevel());
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

void MainWindow::indicateBatteryLevel() {
	int batteryStatus = this->battery->curStatus();
	int batteryLevel = this->battery->getLevelForDisplayGraph();

	if (batteryStatus == NORMAL) {
		this->displayBatteryLevel(batteryLevel);	
	} else {
		this->displayBatteryLevel(batteryLevel, true);	

		if (batteryStatus == CRITICALLY_LOW) {
			// End session early
			qDebug() << "Ending session early";
		}
	}
}

void MainWindow::displayBatteryLevel(int levels, bool flash) {
	// Update battery graph UI with int from 0-8
	qDebug() << "Battery graph: " << levels;
	if (flash)
		qDebug() << "*FLASH*";
}
